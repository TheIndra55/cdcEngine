#pragma once

enum language_t
{
	language_default = -1,
	language_english,
	language_french,
	language_german,
	language_italian,
	language_spanish,
	language_japanese,
	language_portugese,
	language_polish,
	language_ukenglish,
	language_num_states
};

struct LocalizationHeader
{
	language_t language;
	int numStrings;
	char* strings;
};

language_t localstr_get_language();
bool localstr_verifyexistence();
void localstr_set_gfx_gen(bool on);
void localstr_set_language(language_t lang);
char* localstr_get(int id);
void localstr_reload();