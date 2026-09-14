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
 * MyReadCfg - handle table.
 *
 * The Ident handed out to the application is not a pointer but
 * ((slotIndex + 1) & 0xFF) | (generation << 8). Stale idents are therefore
 * detected reliably and no library pointer leaves the IEC interface.
 */

#include "MRC_Internal.h"

#include <cstring>

unsigned long bur_heap_size = 0x10000;

static struct MRC_Slot mrcSlots[MRC_MAX_HANDLES];
static unsigned long mrcGeneration = 0;
static signed long mrcGlobalError = MRC_ERR_OK;

struct MRC_Slot* mrcSlotAlloc(void)
{
	unsigned long i;

	for (i = 0; i < MRC_MAX_HANDLES; i++)
	{
		if (mrcSlots[i].ident == 0)
		{
			std::memset(&mrcSlots[i], 0, sizeof(struct MRC_Slot));

			mrcGeneration++;
			if (mrcGeneration > 0x00FFFFFFUL)
			{
				mrcGeneration = 1;
			}

			mrcSlots[i].ident = ((i + 1) & 0xFF) | (mrcGeneration << 8);
			return &mrcSlots[i];
		}
	}

	return 0;
}

struct MRC_Slot* mrcSlotGet(unsigned long ident)
{
	unsigned long index;

	if (ident == 0)
	{
		return 0;
	}

	index = ident & 0xFF;
	if (index == 0 || index > MRC_MAX_HANDLES)
	{
		return 0;
	}
	index--;

	if (mrcSlots[index].ident != ident)
	{
		return 0;
	}

	return &mrcSlots[index];
}

void mrcSlotRelease(struct MRC_Slot* slot)
{
	if (slot == 0)
	{
		return;
	}

	if (slot->buffer != 0)
	{
		TMP_free(slot->bufferSize, (void*)slot->buffer);
		slot->buffer = 0;
	}

	if (slot->entries != 0)
	{
		TMP_free(slot->entriesSize, (void*)slot->entries);
		slot->entries = 0;
	}

	std::memset(slot, 0, sizeof(struct MRC_Slot));
}

void mrcSetError(struct MRC_Slot* slot, signed long error)
{
	if (slot == 0)
	{
		return;
	}

	if (error == MRC_ERR_OK)
	{
		if (slot->lastError != MRC_ERR_OK)
		{
			return; /* keep the last real failure sticky */
		}
	}

	slot->lastError = error;
}

void mrcSetGlobalError(signed long error)
{
	mrcGlobalError = error;
}

signed long mrcGetGlobalError(void)
{
	return mrcGlobalError;
}
