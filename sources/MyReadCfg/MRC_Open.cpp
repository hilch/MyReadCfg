/*

https://github.com/hilch/MyReadCfg

Helper Library for ACOPOStrak diagnosis

MIT License

Copyright (c) 2026 https://github.com/hilch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
 * MyReadCfg - MRC_Open.
 *
 * The configuration file is read exactly once. Because the function is meant to
 * be called from the INIT part of a task, the asynchronous FileIO function
 * blocks are driven in a local loop with a guard counter instead of cyclically.
 */

#include "MRC_Internal.h"

#include <cstring>
#include <cstdio>

static unsigned short mrcRunFileOpen(struct FileOpen* fb)
{
	unsigned long guard = MRC_FILEIO_MAX_CALLS;

	fb->enable = 1;
	do
	{
		FileOpen(fb);
	} while (fb->status == ERR_FUB_BUSY && --guard != 0);

	return fb->status;
}

static void mrcResetFileOpen(struct FileOpen* fb)
{
	fb->enable = 0;
	FileOpen(fb);
}

static unsigned short mrcRunFileRead(struct FileRead* fb)
{
	unsigned long guard = MRC_FILEIO_MAX_CALLS;

	fb->enable = 1;
	do
	{
		FileRead(fb);
	} while (fb->status == ERR_FUB_BUSY && --guard != 0);

	return fb->status;
}

static void mrcCloseFile(unsigned long fileIdent)
{
	struct FileClose fbClose;
	unsigned long guard = MRC_FILEIO_MAX_CALLS;

	std::memset(&fbClose, 0, sizeof(fbClose));
	fbClose.ident = fileIdent;
	fbClose.enable = 1;
	do
	{
		FileClose(&fbClose);
	} while (fbClose.status == ERR_FUB_BUSY && --guard != 0);

	fbClose.enable = 0;
	FileClose(&fbClose);
}

/* Reads the complete file into slot->buffer. The file is closed in every path. */
static signed long mrcLoadFile(struct MRC_Slot* slot, unsigned long pDeviceName, unsigned long pFileName)
{
	struct FileOpen fbOpen;
	unsigned short status;
	unsigned long fileLen;
	signed long result = MRC_ERR_OK;

	std::memset(&fbOpen, 0, sizeof(fbOpen));
	fbOpen.pDevice = pDeviceName;
	fbOpen.pFile = pFileName;
	fbOpen.mode = FILE_R;

	status = mrcRunFileOpen(&fbOpen);
	if (status != ERR_OK)
	{
		mrcResetFileOpen(&fbOpen);
		return (status == ERR_FUB_BUSY) ? MRC_ERR_TIMEOUT : MRC_ERR_FILE;
	}

	fileLen = fbOpen.filelen;

	if (fileLen > MRC_MAX_FILE_SIZE)
	{
		result = MRC_ERR_FILE_TOO_BIG;
	}
	else
	{
		slot->bufferSize = fileLen + 1;
		if (TMP_alloc(slot->bufferSize, (void**)&slot->buffer) != 0 || slot->buffer == 0)
		{
			slot->buffer = 0;
			slot->bufferSize = 0;
			result = MRC_ERR_MEMORY;
		}
		else
		{
			unsigned long offset = 0;

			while (offset < fileLen && result == MRC_ERR_OK)
			{
				struct FileRead fbRead;
				unsigned long chunk = fileLen - offset;

				if (chunk > MRC_READ_CHUNK)
				{
					chunk = MRC_READ_CHUNK;
				}

				std::memset(&fbRead, 0, sizeof(fbRead));
				fbRead.ident = fbOpen.ident;
				fbRead.offset = offset;
				fbRead.pDest = (unsigned long)(slot->buffer + offset);
				fbRead.len = chunk;

				status = mrcRunFileRead(&fbRead);
				fbRead.enable = 0;
				FileRead(&fbRead);

				if (status != ERR_OK)
				{
					result = (status == ERR_FUB_BUSY) ? MRC_ERR_TIMEOUT : MRC_ERR_FILE;
				}
				else
				{
					offset += chunk;
				}
			}

			slot->buffer[fileLen] = 0;
		}
	}

	mrcCloseFile(fbOpen.ident);
	mrcResetFileOpen(&fbOpen);

	return result;
}

/* Reads a configuration file into memory, returns Ident (0 = error) */
unsigned long MRC_Open(unsigned long pDeviceName, unsigned long pFileName)
{
	struct MRC_Slot* slot;
	signed long result;
	char text[200];

	mrcLogInit();

	if (pDeviceName == 0 || pFileName == 0 ||
		*(const char*)pDeviceName == 0 || *(const char*)pFileName == 0)
	{
		mrcSetGlobalError(MRC_ERR_PARAM);
		mrcLogText(MRC_EVENT_ERR, "MRC_Open", "invalid parameter");
		return 0;
	}

	slot = mrcSlotAlloc();
	if (slot == 0)
	{
		mrcSetGlobalError(MRC_ERR_NO_HANDLE);
		mrcLogText(MRC_EVENT_ERR, "MRC_Open", "no free handle");
		return 0;
	}

	result = mrcLoadFile(slot, pDeviceName, pFileName);

	if (result == MRC_ERR_OK)
	{
		unsigned long contentLength = slot->bufferSize - 1;
		unsigned long maxEntries = mrcCountLines(slot->buffer, contentLength);

		slot->entriesSize = maxEntries * sizeof(struct MRC_Entry);
		if (TMP_alloc(slot->entriesSize, (void**)&slot->entries) != 0 || slot->entries == 0)
		{
			slot->entries = 0;
			slot->entriesSize = 0;
			result = MRC_ERR_MEMORY;
		}
		else
		{
			slot->entryCount = mrcParse(slot->buffer, contentLength, slot->entries, maxEntries, &slot->skippedLines);
			mrcSortEntries(slot->entries, &slot->entryCount, &slot->skippedLines);
		}
	}

	if (result != MRC_ERR_OK)
	{
		std::snprintf(text, sizeof(text), "open failed: %s/%s, error %ld",
			(const char*)pDeviceName, (const char*)pFileName, (long)result);
		mrcSlotRelease(slot);
		mrcSetGlobalError(result);
		mrcLogText(MRC_EVENT_ERR, "MRC_Open", text);
		return 0;
	}

	std::snprintf(text, sizeof(text), "%s/%s: %lu entries",
		(const char*)pDeviceName, (const char*)pFileName, (unsigned long)slot->entryCount);
	mrcLogText(MRC_EVENT_INFO, "MRC_Open", text);

	if (slot->skippedLines > 0)
	{
		std::snprintf(text, sizeof(text), "%s/%s: %lu lines skipped",
			(const char*)pDeviceName, (const char*)pFileName, (unsigned long)slot->skippedLines);
		mrcLogText(MRC_EVENT_WARN, "MRC_Open", text);
	}

	mrcSetError(slot, MRC_ERR_OK);

	return slot->ident;
}
