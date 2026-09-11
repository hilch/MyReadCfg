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
 * MyReadCfg - internal declarations, not part of the public library interface.
 */

#ifndef MRC_INTERNAL_H
#define MRC_INTERNAL_H

#include <bur/plctypes.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include "MyReadCfg.h"
#ifdef __cplusplus
};
#endif

/* Automation Studio does not export STRING constants as C defines, therefore the logger name is declared here. */
#define MRC_LOGGER_NAME "MRC_log"

struct MRC_Entry
{
	const char* key;
	const char* value;
	unsigned long order; /* position in file, decides which duplicate wins */
};

struct MRC_Slot
{
	unsigned long ident; /* 0 = slot is free */
	char* buffer; /* complete file content, parsed in-place */
	unsigned long bufferSize;
	struct MRC_Entry* entries;
	unsigned long entriesSize; /* allocated size in bytes */
	unsigned long entryCount;
	unsigned long skippedLines;
	signed long lastError;
};

/* MRC_Handle.cpp */
struct MRC_Slot* mrcSlotAlloc(void);
struct MRC_Slot* mrcSlotGet(unsigned long ident);
void mrcSlotRelease(struct MRC_Slot* slot);
void mrcSetError(struct MRC_Slot* slot, signed long error);
void mrcSetGlobalError(signed long error);
signed long mrcGetGlobalError(void);

/* MRC_Parse.cpp */
int mrcCompareKey(const char* a, const char* b);
unsigned long mrcCountLines(const char* buffer, unsigned long length);
unsigned long mrcParse(char* buffer, unsigned long length, struct MRC_Entry* entries, unsigned long maxEntries, unsigned long* skipped);
void mrcSortEntries(struct MRC_Entry* entries, unsigned long* count, unsigned long* duplicates);
const char* mrcFindValue(const struct MRC_Slot* slot, const char* key);

/* MRC_Log.cpp */
void mrcLogInit(void);
void mrcLogText(signed long eventID, const char* objectID, const char* text);

#endif
