#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

#include "game/Gameloop.h"
#include "game/sound/Sound.h"
#include "game/menu/UIFadeGroup.h"
#include "game/GameDifficulty.h"

struct MainTracker
{
	int mainState;
	int previousState;
	int movieNum;
	int done;
	int screen;
	const char* attractMovieName;
	bool dontClearSave;
	bool checkMemCardNoCard;
	bool checkMemCardFull;
	bool checkLevelComplete;
};

struct PlayerObjects
{
	__int16 numPlayerObjects;
	__int16 playerObjectList[];
};

struct MenuItems
{
};

struct PlayerInventory
{
	char uInvLoadout;
	char iInvHandguns;
	char iInvHeavyWeapon;
	char holdingHeavyWeapon;

	unsigned __int16 heavyWeaponClipAmmo;
	unsigned __int16 heavyWeaponBankAmmo;

	char uInvGrenades;
	char uInvHealthpacks;

	unsigned __int16 pad;
};

struct ChapterSpec
{
	char name[32];
	char unit[16];

	__int16 checkPointID;
	__int16 playerObjectID;
	__int16 nextChapterNumber;
	__int16 saveGameTextID;
	__int16 saveGameTimeFormatTextID;
	__int16 locationTextID;
	__int16 missionTextID;
	__int16 loadingScreenID;
	__int16 totalBronzeRewards;
	__int16 totalSilverRewards;
	__int16 totalGoldRewards;
	__int16 chapterFlags;

	int loadAmount;
	int timeTrialTimeSec;
	int abilities;
	int attackWaveValue;
	int goalpoint;

	PlayerInventory inventory;
};

static_assert(sizeof(ChapterSpec) == 104, "Wrong ChapterSpec size");

struct SoundEffectsOBSOLETE
{
};

struct MenuPositions
{
};

struct HardCodedHints
{
};

struct PlayerLightList
{
};

struct TitleUnit
{
	float cameraHeight;
	float cameraX;
	float cameraY;

	char unitName[16];
	char playerName[32];

	float PlayerX;
	float PlayerY;
	float PlayerZ;
	float PlayerRotation;
	float PlayerRotX;
	float PlayerRotY;

	float CameraXRot;
	float CameraYRot;
	float CameraZRot;
	float CameraXRotSpeed;
	float CameraYRotSpeed;
	float CameraZRotSpeed;
	float CameraProjection;

	unsigned __int16 RainAmount;
	unsigned __int16 SnowAmount;

	PlayerLightList* playerLights;
};

static_assert(sizeof(TitleUnit) == 120, "Wrong TitleUnit size");

struct TitleScreen
{
	char musicName[20];

	int numUnits;
	TitleUnit* unitList;
};

struct DarkChronicle
{
	__int16 chronicleName;
	char chronicleCinematic[30];
};

struct UnlockItem
{
	__int16 nameTextID;
	char gameUnlockID;
	char catagory;
	__int16 rewardLevelRule;
	__int16 bronzeUnlockRule;
	__int16 silverUnlockRule;
	__int16 goldUnlockRule;
	int specialFlags;
};

struct UnlocksTable
{
	__int16 conceptBioCatName;
	__int16 costumeVarCatName;
	__int16 galleryCatName;
	__int16 modelViewCatName;
	__int16 specialCostumesCatName;
	__int16 debugCatName;
	__int16 extrasCatName;
	__int16 storyCatName;
	__int16 upgradesCatName;
	__int16 cinematicsCatName;

	int unlockCount;
	UnlockItem* unlockItems[];
};

struct ActionNames
{
	__int16 up;
	__int16 down;
	__int16 left;
	__int16 right;
	__int16 lift_attack;
	__int16 telekinesis;
	__int16 jump;
	__int16 aim;
	__int16 autoface;
	__int16 soul_suck;
	__int16 look_around;
	__int16 sneak;
	__int16 pause;
	__int16 status_menu;
	__int16 reaver_select_left;
	__int16 reaver_select_right;
	__int16 spectral_shift;
	__int16 action;
	__int16 analyze;
};

struct MovieRef
{
	char movieName[32];
};

struct CameraDefaults
{
};

struct WetDripDefaults
{
};

struct SuperActionDefaults
{
};

struct InputConfigs
{
};

struct FontList
{
	int numFonts;
	int fontDTPID[];
};

struct RADInfo
{
};

struct GlobalMarkupTuneData
{
	float targetLoZRadius;
	float targetHiZRadius;
	float maxTargetXYRadius;
};

struct GlobalMarkupTuneDataTable
{
	GlobalMarkupTuneData WallVPole;
	GlobalMarkupTuneData VPole;
	GlobalMarkupTuneData Rope;
	GlobalMarkupTuneData Ladder;
	GlobalMarkupTuneData HPole;
	GlobalMarkupTuneData Zipline;
	GlobalMarkupTuneData Ledge;
	GlobalMarkupTuneData Water;
	GlobalMarkupTuneData JumpLanding;
};

struct GammaCurve
{
	bool bLiveUpdate;
	char RedCurve[256];
	char GreenCurve[256];
	char BlueCurve[256];
};

struct GlobalInfo
{
	unsigned int PCNGNumTerrainUnits;
	void* pPCNGExtraTerrainGroups;

	char* serializationInfo;

	PlayerObjects* playerObjects;

	MenuItems* menuItems;

	int fadeGroupCount;
	UIFadeGroupData** fadeGroupData;

	int chapterCount;
	ChapterSpec* chapterList;

	SoundEffectsOBSOLETE* soundEffectsOBSOLETE;

	__int16* keyboardLocals;

	MenuPositions* menuPositionsPS2;
	MenuPositions* menuPositionsXBox;
	MenuPositions* menuPositionsPC;

	float automaticInstanceRemovalFloor;

	HardCodedHints* hardCodedHints;

	TitleScreen* titleScreen;

	int numDarkChronicles;
	DarkChronicle* darkChronicleList;

	UnlocksTable* unlockableTable;

	unsigned __int16 costumeChangeEventVar;
	unsigned __int16 samplerDemoEventVar;

	ActionNames* actionNames;

	int attractMovieCount;
	MovieRef* attractMovies;

	PlayerLightList* playerLights;

	CameraDefaults* cameraDefaults;
	WetDripDefaults* wetDripDefaults;
	SuperActionDefaults* superActionDefaults;

	InputConfigs* inputconfigs;

	FontList* fontList;

	GameDifficultySettings* gameDifficulty;

	int cheatCodesDTPID;

	RADInfo* pRADInfo;

	float percentCompleteRatio;

	int soundGroupCountOBSOLETE;
	SoundGroup* soundGroupOBSOLETE;
	SoundGroupAssignmentsOBSOLETE* soundGroupAssignmentsOBSOLETE;

	GlobalMarkupTuneDataTable markupTuneData;

	GammaCurve xboxGammaCurve;
};

static_assert(sizeof(GlobalInfo) == 1028, "Wrong GlobalInfo size");

struct GlobalSoundInfo
{
	SoundEffects* soundEffects;

	int soundGroupCount;
	SoundGroup* soundGroup;
};

extern char ActiveBuildName[16];
extern char ActiveBuildDir[32];

extern GlobalInfo* globalInfo;

cdc::FileSystem* GetFS();

void InitFS();
void InitArchive();

bool MainG2();
void MAIN_DoMainInit();

void SetupBuildDir(const char* configName);

char* FSHelper_ReadFile(const char* fileName, char memType, cdc::FileSystem* pFS);

void LOAD_ObjectFileName(char* name, char* object, char* extension);
void LOAD_UnitFileName(char* name, char* unit, char* extension);

bool ProcessArgs(char* baseAreaName, GameTracker* gameTracker);