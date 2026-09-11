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
 * MyReadCfg - parser for the INI style configuration file.
 *
 * The file content is parsed in-place: keys and values are zero terminated
 * inside the file buffer, the entry table only stores pointers into it.
 */

#include "MRC_Internal.h"

#include <cstring>
#include <cctype>
#include <algorithm>

static char mrcLower(char c)
{
	return (char)std::tolower((unsigned char)c);
}

int mrcCompareKey(const char* a, const char* b)
{
	while (*a != 0 && *b != 0)
	{
		char ca = mrcLower(*a);
		char cb = mrcLower(*b);

		if (ca != cb)
		{
			return (ca < cb) ? -1 : 1;
		}
		a++;
		b++;
	}

	if (*a == *b)
	{
		return 0;
	}

	return (*a == 0) ? -1 : 1;
}

static char* mrcTrim(char* s)
{
	char* end;

	while (*s == ' ' || *s == '\t')
	{
		s++;
	}

	end = s + std::strlen(s);
	while (end > s && (end[-1] == ' ' || end[-1] == '\t'))
	{
		end--;
	}
	*end = 0;

	return s;
}

/* Name of the constant: [A-Za-z_][A-Za-z0-9_]* with optional array suffix [<n>]. */
static bool mrcIsValidKey(const char* key)
{
	const char* p = key;

	if (*p != '_' && std::isalpha((unsigned char)*p) == 0)
	{
		return false;
	}
	p++;

	while (*p != 0 && *p != '[')
	{
		if (*p != '_' && std::isalnum((unsigned char)*p) == 0)
		{
			return false;
		}
		p++;
	}

	if (*p == '[')
	{
		p++;
		if (std::isdigit((unsigned char)*p) == 0)
		{
			return false;
		}
		while (std::isdigit((unsigned char)*p) != 0)
		{
			p++;
		}
		if (*p != ']')
		{
			return false;
		}
		p++;
		if (*p != 0)
		{
			return false;
		}
	}

	return (std::strlen(key) <= MRC_MAX_KEY_LENGTH);
}

unsigned long mrcCountLines(const char* buffer, unsigned long length)
{
	unsigned long i;
	unsigned long count = 1;

	for (i = 0; i < length; i++)
	{
		if (buffer[i] == '\n' || buffer[i] == '\r')
		{
			count++;
		}
	}

	return count;
}

unsigned long mrcParse(char* buffer, unsigned long length, struct MRC_Entry* entries, unsigned long maxEntries, unsigned long* skipped)
{
	char* p = buffer;
	char* end = buffer + length;
	unsigned long count = 0;

	if (skipped != 0)
	{
		*skipped = 0;
	}

	/* skip UTF-8 BOM */
	if (length >= 3 && (unsigned char)p[0] == 0xEF && (unsigned char)p[1] == 0xBB && (unsigned char)p[2] == 0xBF)
	{
		p += 3;
	}

	while (p < end)
	{
		char* line = p;
		char* sep;
		char* key;
		char* value;

		while (p < end && *p != '\r' && *p != '\n')
		{
			p++;
		}
		if (p < end)
		{
			*p = 0;
			p++;
		}

		line = mrcTrim(line);

		if (*line == 0)
		{
			continue;
		}
		if (line[0] == ';' || line[0] == '#')
		{
			continue;
		}
		if (line[0] == '/' && line[1] == '/')
		{
			continue;
		}
		if (line[0] == '[')
		{
			continue; /* section header, ignored */
		}

		sep = std::strchr(line, '=');
		if (sep == 0)
		{
			if (skipped != 0)
			{
				(*skipped)++;
			}
			continue;
		}

		*sep = 0;
		key = mrcTrim(line);
		value = mrcTrim(sep + 1);

		if (!mrcIsValidKey(key) || std::strlen(value) > MRC_MAX_VALUE_LENGTH)
		{
			if (skipped != 0)
			{
				(*skipped)++;
			}
			continue;
		}

		if (entries != 0)
		{
			if (count >= maxEntries)
			{
				break;
			}
			entries[count].key = key;
			entries[count].value = value;
			entries[count].order = count;
		}
		count++;
	}

	return count;
}

struct MRC_EntryLess
{
	bool operator()(const struct MRC_Entry& a, const struct MRC_Entry& b) const
	{
		int c = mrcCompareKey(a.key, b.key);
		if (c != 0)
		{
			return c < 0;
		}
		return a.order < b.order;
	}
};

void mrcSortEntries(struct MRC_Entry* entries, unsigned long* count, unsigned long* duplicates)
{
	unsigned long n;
	unsigned long read;
	unsigned long write;

	if (entries == 0 || count == 0 || *count == 0)
	{
		return;
	}

	n = *count;
	std::sort(entries, entries + n, MRC_EntryLess());

	write = 1;
	for (read = 1; read < n; read++)
	{
		if (mrcCompareKey(entries[read].key, entries[write - 1].key) == 0)
		{
			if (duplicates != 0)
			{
				(*duplicates)++;
			}
			continue; /* first occurrence in file order wins */
		}
		entries[write] = entries[read];
		write++;
	}

	*count = write;
}

const char* mrcFindValue(const struct MRC_Slot* slot, const char* key)
{
	unsigned long lo;
	unsigned long hi;

	if (slot == 0 || slot->entries == 0 || slot->entryCount == 0 || key == 0)
	{
		return 0;
	}

	lo = 0;
	hi = slot->entryCount;

	while (lo < hi)
	{
		unsigned long mid = lo + (hi - lo) / 2;
		int c = mrcCompareKey(slot->entries[mid].key, key);

		if (c == 0)
		{
			return slot->entries[mid].value;
		}
		if (c < 0)
		{
			lo = mid + 1;
		}
		else
		{
			hi = mid;
		}
	}

	return 0;
}
