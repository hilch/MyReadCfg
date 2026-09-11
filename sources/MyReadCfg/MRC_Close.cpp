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
 * MyReadCfg - MRC_Close.
 */

#include "MRC_Internal.h"

/* Closes a configuration and frees all resources. 0 = OK */
signed long MRC_Close(unsigned long Ident)
{
	struct MRC_Slot* slot = mrcSlotGet(Ident);

	if (slot == 0)
	{
		mrcSetGlobalError(MRC_ERR_INVALID_IDENT);
		mrcLogText(MRC_EVENT_WARN, "MRC_Close", "invalid ident");
		return MRC_ERR_INVALID_IDENT;
	}

	mrcSlotRelease(slot);
	mrcSetGlobalError(MRC_ERR_OK);

	return MRC_ERR_OK;
}
