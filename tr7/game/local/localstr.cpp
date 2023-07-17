#include <stdio.h>
#include <stdlib.h>

#include "localstr.h"

#include "cdc/runtime/cdcSys/UTF8String.h"
#include "cdc/runtime/cdcSys/Assert.h"

#include "game/Main.h"
#include "game/Specialisation.h"

static language_t this_language;
static bool s_gfxNextGen = false;

static LocalizationHeader* LocalizationTable;
static char** LocalStrings;

static char* s_badStr = "ERROR OUT OF TABLE RANGE";

language_t localstr_get_language()
{
	return this_language;
}

bool localstr_verifyexistence()
{
	char name[256];
	sprintf(name, "%slocal\\locals.bin", ActiveBuildDir);

	auto fileSystem = GetFS();
	return fileSystem->FileExists(name);
}

void localstr_set_language(language_t lang)
{
	if (lang == language_default)
	{
		lang = language_english;
	}

	this_language = lang;

	Specialisation::BlockingChange((1 << this_language) | (s_gfxNextGen ? 0 : 0x80000000));
}

void localstr_set_gfx_gen(bool on)
{
	s_gfxNextGen = true;

	Specialisation::BlockingChange((1 << this_language) | (s_gfxNextGen ? 0 : 0x80000000));
}

char* localstr_get(int id)
{
	if (LocalStrings && id < LocalizationTable->numStrings)
	{
		return LocalStrings[id];
	}

	return s_badStr;
}

void localstr_reload()
{
	char name[256];
	sprintf(name, "%slocal\\locals.bin", ActiveBuildDir);

	if (LocalizationTable)
	{
		free(LocalizationTable);
	}

	auto fileSystem = GetFS();
	auto header = (LocalizationHeader*)FSHelper_ReadFile(name, 0, fileSystem);

	LocalizationTable = header;

	if (header)
	{
		if (header->numStrings > 0)
		{
			LocalStrings = &header->strings;

			for (int i = 0; i < header->numStrings; i++)
			{
				auto offset = LocalStrings[i];

				if (offset)
				{
					auto string = (char*)((int)header + offset);

					int dummy;
					if (!cdc::StrIsValidUTF8(string, &dummy))
					{
						string = "Invalid UTF8 encoding";
					}

					LocalStrings[i] = string;
				}
				else
				{
					LocalStrings[i] = "";
				}
			}
		}
	}
	else
	{
		cdc::FatalError("ERROR: Could Not Load Localization Table: %s", name);
	}
}