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
 * MyReadCfg - typed read functions.
 *
 * All read functions work on the memory image created by MRC_Open and are
 * therefore finished within one call.
 */

#include "MRC_Internal.h"

#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cstdio>

static void mrcLogRead(const char* key, const char* type, const char* value)
{
	char text[256];

	if (key == 0 || type == 0 || value == 0)
	{
		return;
	}

	std::snprintf(text, sizeof(text), "%s (%s) = %s", key, type, value);
	mrcLogText(MRC_EVENT_INFO, "MRC_Read", text);
}

static void mrcLogReadBOOL(const char* key, plcbit value)
{
	mrcLogRead(key, "BOOL", value ? "TRUE" : "FALSE");
}

static void mrcLogReadDINT(const char* key, signed long value)
{
	char formatted[32];

	std::snprintf(formatted, sizeof(formatted), "%ld", value);
	mrcLogRead(key, "DINT", formatted);
}

static void mrcLogReadLREAL(const char* key, double value)
{
	char formatted[64];

	std::snprintf(formatted, sizeof(formatted), "%.17g", value);
	mrcLogRead(key, "LREAL", formatted);
}

static void mrcLogReadSTRING(const char* key, const char* value)
{
	mrcLogRead(key, "STRING", value);
}

static void mrcLogKeyNotFound(const char* key)
{
	char text[128];

	if (key == 0 || *key == 0)
	{
		mrcLogText(MRC_EVENT_WARN, "MRC_Read", "MRC_ERR_KEY_NOT_FOUND");
		return;
	}

	std::snprintf(text, sizeof(text), "key '%s' not found -> MRC_ERR_KEY_NOT_FOUND (%ld)", key, (long)MRC_ERR_KEY_NOT_FOUND);
	mrcLogText(MRC_EVENT_WARN, "MRC_Read", text);
}

static const char* mrcLookup(unsigned long Ident, unsigned long pKey, struct MRC_Slot** pSlot)
{
	struct MRC_Slot* slot = mrcSlotGet(Ident);
	const char* value;

	*pSlot = slot;

	if (slot == 0)
	{
		mrcSetGlobalError(MRC_ERR_INVALID_IDENT);
		return 0;
	}

	if (pKey == 0 || *(const char*)pKey == 0)
	{
		mrcSetError(slot, MRC_ERR_PARAM);
		return 0;
	}

	value = mrcFindValue(slot, (const char*)pKey);
	if (value == 0)
	{
		mrcSetError(slot, MRC_ERR_KEY_NOT_FOUND);
		mrcLogKeyNotFound((const char*)pKey);
		return 0;
	}

	mrcSetError(slot, MRC_ERR_OK);
	return value;
}

/* Error of the last MyReadCfg call of this Ident (0 = Ident independent / open error) */
signed long MRC_GetLastError(unsigned long Ident)
{
	struct MRC_Slot* slot;

	if (Ident == 0)
	{
		return mrcGetGlobalError();
	}

	slot = mrcSlotGet(Ident);
	if (slot == 0)
	{
		return MRC_ERR_INVALID_IDENT;
	}

	return slot->lastError;
}

/* TRUE if the key is present */
plcbit MRC_KeyExists(unsigned long Ident, unsigned long pKey)
{
	struct MRC_Slot* slot;
	const char* value = mrcLookup(Ident, pKey, &slot);

	if (value == 0)
	{
		if (slot != 0)
		{
			mrcSetError(slot, MRC_ERR_KEY_NOT_FOUND);
		}
		return 0;
	}

	return 1;
}

/* Reads a value as BOOL */
plcbit MRC_ReadBOOL(unsigned long Ident, unsigned long pKey, plcbit Default)
{
	struct MRC_Slot* slot;
	const char* key = (const char*)pKey;
	const char* value = mrcLookup(Ident, pKey, &slot);

	if (value == 0)
	{
		mrcLogReadBOOL(key, Default);
		return Default;
	}

	if (mrcCompareKey(value, "1") == 0 || mrcCompareKey(value, "TRUE") == 0 ||
		mrcCompareKey(value, "YES") == 0 || mrcCompareKey(value, "ON") == 0)
	{
		mrcLogReadBOOL(key, 1);
		return 1;
	}

	if (mrcCompareKey(value, "0") == 0 || mrcCompareKey(value, "FALSE") == 0 ||
		mrcCompareKey(value, "NO") == 0 || mrcCompareKey(value, "OFF") == 0)
	{
		mrcLogReadBOOL(key, 0);
		return 0;
	}

	mrcSetError(slot, MRC_ERR_CONVERSION);
	mrcLogReadBOOL(key, Default);
	return Default;
}

/* Reads a value as DINT */
signed long MRC_ReadDINT(unsigned long Ident, unsigned long pKey, signed long Default)
{
	struct MRC_Slot* slot;
	const char* key = (const char*)pKey;
	const char* value = mrcLookup(Ident, pKey, &slot);
	const char* start;
	char* endPtr;
	long converted;
	int base = 10;

	if (value == 0)
	{
		mrcLogReadDINT(key, Default);
		return Default;
	}

	start = value;
	if (start[0] == '0' && (start[1] == 'x' || start[1] == 'X'))
	{
		base = 16;
		start += 2;
	}
	else if (start[0] == '1' && start[1] == '6' && start[2] == '#')
	{
		base = 16;
		start += 3;
	}

	errno = 0;
	converted = std::strtol(start, &endPtr, base);

	if (endPtr == start || *endPtr != 0 || errno == ERANGE)
	{
		mrcSetError(slot, MRC_ERR_CONVERSION);
		mrcLogReadDINT(key, Default);
		return Default;
	}

	mrcLogReadDINT(key, (signed long)converted);
	return (signed long)converted;
}

/* Reads a value as LREAL */
double MRC_ReadLREAL(unsigned long Ident, unsigned long pKey, double Default)
{
	struct MRC_Slot* slot;
	const char* key = (const char*)pKey;
	const char* value = mrcLookup(Ident, pKey, &slot);
	const char* p;
	char* endPtr;
	double converted;

	if (value == 0)
	{
		mrcLogReadLREAL(key, Default);
		return Default;
	}

	/* only decimal floating point literals are accepted, this also rejects nan/inf */
	for (p = value; *p != 0; p++)
	{
		if ((*p < '0' || *p > '9') && *p != '+' && *p != '-' && *p != '.' && *p != 'e' && *p != 'E')
		{
			mrcSetError(slot, MRC_ERR_CONVERSION);
			mrcLogReadLREAL(key, Default);
			return Default;
		}
	}

	errno = 0;
	converted = std::strtod(value, &endPtr);

	if (endPtr == value || *endPtr != 0 || errno == ERANGE)
	{
		mrcSetError(slot, MRC_ERR_CONVERSION);
		mrcLogReadLREAL(key, Default);
		return Default;
	}

	mrcLogReadLREAL(key, converted);
	return converted;
}
/* Reads the complete file content into a STRING variable. Returns copied length or negative error */
signed long MRC_ReadContent(unsigned long Ident, unsigned long pValue, unsigned long MaxSize)
{
        struct MRC_Slot* slot;
        const char* value;
        char* dest = (char*)pValue;
        unsigned long length;
        plcbit truncated = 0;

        if (pValue == 0 || MaxSize == 0)
        {
                slot = mrcSlotGet(Ident);
                if (slot != 0)
                {
                        mrcSetError(slot, MRC_ERR_PARAM);
                }
                else
                {
                        mrcSetGlobalError(MRC_ERR_PARAM);
                }
                return MRC_ERR_PARAM;
        }

        slot = mrcSlotGet(Ident);
        if (slot == 0)
        {
                mrcSetGlobalError(MRC_ERR_INVALID_IDENT);
                return MRC_ERR_INVALID_IDENT;
        }

        value = slot->buffer;
        if (value == 0)
        {
                dest[0] = 0;
                mrcSetError(slot, MRC_ERR_OK);
                mrcLogRead("CONTENT", "STRING", "");
                return 0;
        }

        length = (unsigned long)std::strlen(value);
        if (length > MaxSize - 1)
        {
                length = MaxSize - 1;
                truncated = 1;
        }

        std::memcpy(dest, value, length);
        dest[length] = 0;

        if (truncated)
        {
                mrcSetError(slot, MRC_ERR_TRUNCATED);
        }
        else
        {
                mrcSetError(slot, MRC_ERR_OK);
        }

        mrcLogRead("CONTENT", "STRING", dest);
        return (signed long)length;
}
/* Reads a value as STRING. Returns copied length or negative error */
signed long MRC_ReadSTRING(unsigned long Ident, unsigned long pKey, unsigned long pValue, unsigned long MaxSize, unsigned long pDefault)
{
	struct MRC_Slot* slot;
	const char* key = (const char*)pKey;
	const char* value;
	char* dest = (char*)pValue;
	unsigned long length;
	plcbit truncated = 0;

	if (pValue == 0 || MaxSize == 0)
	{
		slot = mrcSlotGet(Ident);
		if (slot != 0)
		{
			mrcSetError(slot, MRC_ERR_PARAM);
		}
		else
		{
			mrcSetGlobalError(MRC_ERR_PARAM);
		}
		return MRC_ERR_PARAM;
	}

	value = mrcLookup(Ident, pKey, &slot);
	if (value == 0)
	{
		value = (const char*)pDefault;
	}

	if (value == 0)
	{
		dest[0] = 0;
		mrcLogReadSTRING(key, "");
		return 0;
	}

	length = (unsigned long)std::strlen(value);
	if (length > MaxSize - 1)
	{
		length = MaxSize - 1;
		truncated = 1;
	}

	std::memcpy(dest, value, length);
	dest[length] = 0;

	if (truncated)
	{
		mrcSetError(slot, MRC_ERR_TRUNCATED);
	}

	mrcLogReadSTRING(key, dest);
	return (signed long)length;
}
