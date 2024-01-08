#pragma once

enum UIScreenID
{
	kUIScreenNoneID						= 0,

	kUIScreenPauseID					= 1,
	kUIScreenOptionsID					= 2,
	kUIScreenConfirmQuitID				= 3,
	kUIScreenE3PauseID					= 4,
	kUIScreenE3SystemID					= 5,
	kUIScreenSystemID					= 6,
	kUIScreenCenterID					= 7,
	kUIScreenLevelCompleteID			= 8,
	kUIScreenUnlocksLevelCompleteID		= 13,
	kUIScreenEndCreditsID				= 14,
	kUIScreenEndGameID					= 15,
	kUIScreenMansionLCompleteID			= 16,
	kUIScreenEndDemoID					= 17,
	kUIScreenControlsHelpID				= 19,
	kUIScreenLvlCompBkgdID				= 21,
	kUIScreenDisplayModeID				= 22,
	kUIScreenTestDisplayModeID			= 23,
	kUIScreenButtonEntryID				= 24,

	kUIScreenInGameEndRangeID			= 1000,
	kUIScreenFrontEndStartID			= 1000,
	kUIScreenFETitleID					= 1000,
	kUIScreenFEMainID					= 1001,
	kUIScreenFEUnlocksID				= 1002,
	kUIScreenFEMainE3ID					= 1003,
	kUIScreenFELoadLevelID				= 1004,
	kUIScreenFELanguageID				= 1007,
	kUIScreenFELegalID					= 1008,
	kUIScreenFELoadExtrasID				= 1010,
	kUIScreenFEBootSettingsID			= 1030,
	kUIScreenFERatingsID				= 1031,
	kUIScreenFESetupID					= 1032,

	kUIScreenFEEndRangeID				= 2000,
	kUIScreenLoadingStartID				= 2000,
	kUIScreenLoadingNoneID				= 2000,
	kUIScreenLoadingDefaultID			= 2100,
	kUIScreenLoadingDeathID				= 2101,
	kUIScreenLoadingFinishedGameID		= 2102,

	kUIScreenSharedStartID				= 3000,
	kUIScreenSHControllerRemovedID		= 3000,
	kUIScreenCostumeSelectID			= 3001,
	kUIScreenSHControllerRemMCPresentID	= 3002,
	kUIScreenSHControllerRemMCMissingID	= 3003,
	kUIScreenXboxLiveID					= 3500,
	kUIScreenXLUserSelection			= 3501,
	kUIScreenXLFriends					= 3502,
	kUIScreenXLPasscodeDialog			= 3503,
	kUIScreenXLSigninDialog				= 3504,
	kUIScreenXLFriendRequestsDialog		= 3505,
	kUIScreenXLFriendRequestedDialog	= 3506,
	kUIScreenXLFriendDialog				= 3507,
	kUIScreenXLVoiceMessage				= 3508,
	kUIScreenXLVoiceFeedback			= 3509,
	kUIScreenXLContent					= 3600,
	kUIScreenXLContentDownload			= 3601,
	kUIScreenXLDownloadOffering			= 3602,
	kUIScreenXLInsertDisc				= 3603,

	kUIScreenMCAccessingID				= 4000,
	kUIScreenMCBootMissingID			= 4001,
	kUIScreenMCLoadMansionSlotsID		= 4002,
	kUIScreenMCUnused1ID				= 4003,
	kUIScreenMCUnused2ID				= 4004,
	kUIScreenMCBootNoSpaceID			= 4005,
	kUIScreenMCEnterSaveID				= 4006,
	kUIScreenMCSaveDoneID				= 4007,
	kUIScreenMCEnterFormatID			= 4008,
	kUIScreenMCFormatDoneID				= 4009,
	kUIScreenMCEnterLoadID				= 4010,
	kUIScreenMCLoadDoneID				= 4011,
	kUIScreenMCOverwriteID				= 4012,
	kUIScreenMCStartGameID				= 4013,
	kUIScreenMCLoadRecentGameID			= 4014,
	kUIScreenMCSaveSlotsID				= 4015,
	kUIScreenMCLoadSlotsID				= 4016,
	kUIScreenMCMainMenuRecentGameInfoID	= 4017,
	kUIScreenMCEnterLoadFEID			= 4018,
	kUIScreenMCAskDoFormatID			= 4019,
	kUIScreenMCUnformattedID			= 4020,
	kUIScreenMCSaveMissingID			= 4021,
	kUIScreenMCSaveNoSpaceID			= 4022,
	kUIScreenMCCantLoadGameID			= 4500,
	kUIScreenMCLastScreenID				= 4501,
	kUIScreenSGSaveGameID				= 4600,
	kUIScreenSGLoadGameID				= 4601,
	kUIScreenSGSavingID					= 4602,
	kUIScreenSGDummyID					= 4603,
	kUIScreenApplyDisplaySettingsID		= 4700,

	kUIScreenSharedEndRange				= 5000,
	kUIScreenHUDStart					= 5000,
	kUIScreenPlayerHUD					= 5000,
	kUIScreenBinocularsHUD				= 5001,
	kUIScreenVehicleHUD					= 5002,
	kUIScreenCinematicHUD				= 5003,

	kUIDialogOKStd						= 6000,
	kUIDialogActionBtnStd				= 6010,
	kUIDialogNoYesStd					= 6020,
	kUIDialogYesNoStd					= 6021,
	kUIDialogDynYesNo					= 6022,
	kUIDialogDynNoYes					= 6023,
	kUIDialogDynYesNoCancel				= 6024,
	kUIDialogDynNoYesCancel				= 6025,

	kUIScreenMansionBaseID				= 7000,
	kUIScreenMansionUnlocksID			= 7000,
	kUIScreenMansionComputerID			= 7001,
	kUIScreenMansionCostumeID			= 7004
};

class UIScreen
{
};

class UIScreenManager
{
public:
	static void DeregisterAllData();
};