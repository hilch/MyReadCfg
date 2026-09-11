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
 * MyReadCfg - own persistent logger 'MRC_log'.
 *
 * MyReadCfg never writes into the system logger. Errors of the logging itself
 * are swallowed on purpose: they must not make MRC_Open or a read function fail.
 */

#include "MRC_Internal.h"

#include <cstring>

static ArEventLogIdentType mrcLogIdent = 0;

static ArEventLogIdentType mrcLogGetIdent(void)
{
	struct ArEventLogGetIdent fb;
	ArEventLogIdentType ident = 0;

	std::memset(&fb, 0, sizeof(fb));
	std::strcpy((char*)fb.Name, MRC_LOGGER_NAME);
	fb.Execute = 1;
	ArEventLogGetIdent(&fb); /* executed synchronously */

	if (fb.Done)
	{
		ident = fb.Ident;
	}

	fb.Execute = 0;
	ArEventLogGetIdent(&fb);

	return ident;
}

void mrcLogInit(void)
{
	struct ArEventLogCreate fb;
	unsigned long guard;

	if (mrcLogIdent != 0)
	{
		return;
	}

	mrcLogIdent = mrcLogGetIdent();
	if (mrcLogIdent != 0)
	{
		return;
	}

	std::memset(&fb, 0, sizeof(fb));
	std::strcpy((char*)fb.Name, MRC_LOGGER_NAME);
	fb.Size = MRC_LOGGER_SIZE;
	fb.Persistence = arEVENTLOG_PERSISTENCE_PERSIST;
	fb.Info = arEVENTLOG_ADDFORMAT_TEXT;
	fb.Execute = 1;

	guard = MRC_FILEIO_MAX_CALLS;
	do
	{
		ArEventLogCreate(&fb);
	} while (!fb.Done && !fb.Error && --guard != 0);

	fb.Execute = 0;
	ArEventLogCreate(&fb);

	/* An already existing logger is reused, therefore the ident is fetched again in any case. */
	mrcLogIdent = mrcLogGetIdent();
}

void mrcLogText(signed long eventID, const char* objectID, const char* text)
{
	struct ArEventLogWrite fb;

	if (mrcLogIdent == 0 || text == 0)
	{
		return;
	}

	std::memset(&fb, 0, sizeof(fb));
	fb.Ident = mrcLogIdent;
	fb.EventID = eventID;
	fb.OriginRecordID = 0;
	if (objectID != 0)
	{
		std::strncpy((char*)fb.ObjectID, objectID, sizeof(fb.ObjectID) - 1);
	}
	fb.AddDataSize = (unsigned long)std::strlen(text) + 1;
	fb.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
	fb.AddData = (unsigned long)text;
	fb.TimeStamp = 0;
	fb.Execute = 1;
	ArEventLogWrite(&fb); /* executed synchronously */

	fb.Execute = 0;
	ArEventLogWrite(&fb);
}
