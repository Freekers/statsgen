#include <wx/thread.h>
#include <wx/filename.h>
#include <wx/help.h>
#include <wx/fs_zip.h>
#include <wx/html/helpctrl.h>
#include <wx/cshelp.h>
#include <wx/aboutdlg.h>


// Statsgen Includes
#include "Version.h"
#include "StatsgenApp.h"
#include "StatsgenFrame.h"
#include "LogFileReader.h"
#include "TemplateFile.h"
#include "ErrorData.h"
#include "Server.h"
#include "WindowIDs.h"
#include "ConfigEditorGUI.h"
#include "ServerTypePanel.h"
#include "SkillTypePanel.h"
#include "ImageTypePanel.h"
#include "GenericConfigGUI.h"
#include "GenericNumberedConfigGUI.h"
#include "GenericNumberedConfigPanel.h"
#include "GroupedConfigItemsPanel.h"
#include "FTPConnectionPanel.h"
#include "PlayerCachePanel.h"
#include "AliasEditorPanel.h"
#include "AliasDialog.h"
#include "AwardEditorPanel.h"
#include "GenericOKCancelDialog.h"
#include "Progress.h"
#include "ClanEditorPanel.h"
#include "DropListDialog.h"
#include "TextListDialog.h"
#include "StatsgenApp.h"
#include "Progress.h"
#include "TemplateFile.h"
#include "StaticFunctions.h"
#include "PlayerDataDialog.h"
#include "PriorityPanel.h"
#include "TemplateFile.h"
#include "GUITriggerList.h"
#include "GlobalStatistics.h"
#include "PerformUpgradeDialog.h"
#include "StatsgenWeb.h"
#include "GUIWizards.h"
#include "WeaponGroupDialog.h"


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------
IMPLEMENT_CLASS(wxWidgetsFrame, wxFrame)

BEGIN_EVENT_TABLE(wxWidgetsFrame, wxFrame)
	EVT_TIMER(wxID_ANY,									wxWidgetsFrame::OnTimerTick)
	EVT_MENU(WINDOW_ID_WIZARD,							wxWidgetsFrame::OnWizard)
	EVT_MENU(WINDOW_ID_MENU_ABOUT,						wxWidgetsFrame::OnAbout)
	EVT_MENU(WINDOW_ID_MENU_HELP,						wxWidgetsFrame::OnHelp)
	EVT_MENU(WINDOW_ID_MENU_THREAD,						wxWidgetsFrame::OnConfigThread)
	EVT_MENU(WINDOW_ID_MENU_QUIT,						wxWidgetsFrame::OnQuit)
	EVT_MENU(WINDOW_ID_MENU_GO,							wxWidgetsFrame::OnGo)
	EVT_MENU(WINDOW_ID_MENU_ADVANCED,					wxWidgetsFrame::OnAdvanced)
	EVT_MENU(WINDOW_ID_MENU_RESET_RECOVERY,				wxWidgetsFrame::OnResetRecovery)
	EVT_MENU(WINDOW_ID_MENU_STORE_PROGRESS,				wxWidgetsFrame::OnStoreProgress)
	EVT_MENU(WINDOW_ID_MENU_EMPTY_DATABASE,				wxWidgetsFrame::OnDatabaseEmpty)
	EVT_MENU(WINDOW_ID_MENU_SCORE_WEIGHTS,				wxWidgetsFrame::OnEditScoreWeights)
	EVT_MENU(WINDOW_ID_MENU_SKILL_WEIGHTS,				wxWidgetsFrame::OnEditSkillWeights)
	EVT_MENU(WINDOW_ID_MENU_REAL_NAMES,					wxWidgetsFrame::OnEditRealNames)
	EVT_MENU(WINDOW_ID_MENU_WEAPON_GROUPS,				wxWidgetsFrame::OnEditWeaponGroups)
	EVT_MENU(WINDOW_ID_MENU_IMAGES,						wxWidgetsFrame::OnEditImages)
	EVT_MENU(WINDOW_ID_MENU_MESSAGE_FORMATS,			wxWidgetsFrame::OnEditMessageFormats)
	EVT_MENU(WINDOW_ID_MENU_WEBSITE,					wxWidgetsFrame::OnWebsiteConfig)
	EVT_MENU(WINDOW_ID_MENU_IMAGEPACKS,					wxWidgetsFrame::OnImagePackConfig)
	EVT_MENU(WINDOW_ID_MENU_TEMPLATEPACKS,				wxWidgetsFrame::OnTemplatePackConfig)
	EVT_MENU(WINDOW_ID_MENU_FULL_CONFIG,				wxWidgetsFrame::OnFullConfig)
	EVT_MENU(WINDOW_ID_MENU_OUTPUT,						wxWidgetsFrame::OnOutputConfig)
	EVT_MENU(WINDOW_ID_MENU_EXTERNAL_DATABASE,			wxWidgetsFrame::OnExternalDatabaseConfig)
	EVT_MENU(WINDOW_ID_MENU_TEMPLATE_CONFIG,			wxWidgetsFrame::OnTemplateConfig)
	EVT_MENU(WINDOW_ID_MENU_SERVER,						wxWidgetsFrame::OnServerConfig)
	EVT_MENU(WINDOW_ID_MENU_CLAN,						wxWidgetsFrame::OnClanConfig)
	EVT_MENU(WINDOW_ID_MENU_ALIAS,						wxWidgetsFrame::OnAliasConfig)
	EVT_MENU(WINDOW_ID_MENU_DROPLIST,					wxWidgetsFrame::OnDropListConfig)
	EVT_MENU(WINDOW_ID_MENU_BAD_WORDS,					wxWidgetsFrame::OnBadWordsListConfig)
	EVT_MENU(WINDOW_ID_MENU_WEB_SERVER,					wxWidgetsFrame::OnWebServerConfig)
	EVT_MENU(WINDOW_ID_MENU_CUSTOM_MESSAGES,			wxWidgetsFrame::OnCustomMessagesConfig)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATAAVATAR,			wxWidgetsFrame::OnPlayerDataAvatarConfig)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATAREALNAME,			wxWidgetsFrame::OnPlayerDataRealNameConfig)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATAPICTURE,			wxWidgetsFrame::OnPlayerDataPictureConfig)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATAXFIRE,			wxWidgetsFrame::OnPlayerDataXfireConfig)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATAWEBSITE,			wxWidgetsFrame::OnPlayerDataWebsiteConfig)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATACLANROLE,			wxWidgetsFrame::OnPlayerDataClanRoleConfig)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATAMISC1,			wxWidgetsFrame::OnPlayerDataMisc1Config)
	EVT_MENU(WINDOW_ID_MENU_PLAYERDATAMISC2,			wxWidgetsFrame::OnPlayerDataMisc2Config)
	EVT_MENU(WINDOW_ID_MENU_AWARD,						wxWidgetsFrame::OnAwardConfig)
	EVT_MENU(WINDOW_ID_MENU_RCON_CONFIG,				wxWidgetsFrame::OnRCONSpamConfig)
	EVT_MENU(WINDOW_ID_MENU_DEBUG,						wxWidgetsFrame::OnDebugConfig)
	EVT_MENU(WINDOW_ID_MENU_EXPORT_TEMPLATE,			wxWidgetsFrame::OnExportTemplate)
	EVT_MENU(WINDOW_ID_MENU_PERFORM_UPGRADE,			wxWidgetsFrame::OnPerformUpgrade)
	EVT_MENU(WINDOW_ID_MENU_EXPORT_UPGRADE,				wxWidgetsFrame::OnExportUpgrade)
	EVT_MENU(WINDOW_ID_MENU_EXPORT_BUILD_INI,			wxWidgetsFrame::OnExportBuildINI)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_GAMETYPE,	wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_MAP,			wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_CLASS,		wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_TEAM,		wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPON,		wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_AMMO,		wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPONAMMO,	wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_LOCATION,	wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(WINDOW_ID_MENU_LOG_SUBSTITUTE_ACTION,		wxWidgetsFrame::OnLogSubstitute)
	EVT_MENU(EVENT_ID_PROGRESS_STATUS_TEXT,				wxWidgetsFrame::OnProgressUpdate)
	EVT_MENU(EVENT_ID_PROGRESS_BUTTON,			wxWidgetsFrame::OnProgressUpdate)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------
wxWidgetsFrame::wxWidgetsFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	bool		runOnceFlag;
	quitNow=false;
	workerThread=NULL;
	globalStatistics.mainEventHandler=this;

	wxMenu *helpMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *fileMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *displayMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *calculationMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *mandatoryMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *optionalMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *rconMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *logSubstituteMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *devMenu = new wxMenu((char *)"", wxMENU_TEAROFF);
	wxMenu *playerData = new wxMenu((char *)"", wxMENU_TEAROFF);

	int argIndex;

	globalStatistics.configData.Initiate();
	STATSGEN_DEBUG_INITIATE
	STATSGEN_DEBUG_UPDATE_DEBUG_LEVEL(WINDOW_ID_PROGRESS_PANEL_GENERAL)
	globalStatistics.configData.CommitChanges();

	helpMenu->Append(WINDOW_ID_MENU_HELP, (char *)"&Help", (char *)"Help");
	helpMenu->Append(WINDOW_ID_MENU_ABOUT, (char *)"About", (char *)"Version");
	fileMenu->AppendCheckItem(WINDOW_ID_MENU_ADVANCED, (char *)"Advanced Menus", (char *)"Advanced");
	fileMenu->Append(WINDOW_ID_WIZARD, (char *)"First Time Configuration Run", (char *)"Run this the first time you start statsgen");
	fileMenu->Append(WINDOW_ID_MENU_GO, (char *)"Go\tf1", (char *)"Go");
	fileMenu->Append(WINDOW_ID_MENU_EMPTY_DATABASE, (char *)"Empty Database", (char *)"Empty Database");
	fileMenu->Append(WINDOW_ID_MENU_THREAD, (char *)"Priority\tf2", (char *)"Change Priority");
	fileMenu->Append(WINDOW_ID_MENU_PERFORM_UPGRADE, (char *)"Upgrade Configuration", (char *)"Use an Upgrade file to update configuration");
	fileMenu->Append(WINDOW_ID_MENU_RESET_RECOVERY, (char *)"Clear Recovery Status", (char *)"Clear Recovery Status");
	fileMenu->Append(WINDOW_ID_MENU_STORE_PROGRESS, (char *)"Write Errors To File", (char *)"Write Errors To File");
	fileMenu->Append(WINDOW_ID_MENU_QUIT, (char *)"E&xit\tAlt-X", (char *)"Quit this program");

	displayMenu->Append(WINDOW_ID_MENU_WEAPON_GROUPS, (char *)"Weapon Groups", (char *)"Edit Weapon Groupings");
	displayMenu->Append(WINDOW_ID_MENU_REAL_NAMES, (char *)"Real Names", (char *)"Edit Real Names");
	displayMenu->Append(WINDOW_ID_MENU_IMAGES, (char *)"Images", (char *)"Edit Image References");

	playerData->Append(WINDOW_ID_MENU_PLAYERDATAAVATAR, (char *)"Avatar Editing", (char *)"Edit Player Avatar Images");
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAREALNAME, (char *)"Real Name Editing", (char *)"Edit Player Real Names");
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAPICTURE, (char *)"Personal Picture", (char *)"Edit Player Personal Pictures");
	playerData->Append(WINDOW_ID_MENU_PLAYERDATACLANROLE, (char *)"Clan Role", (char *)"Edit Player's Clan Role");
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAXFIRE, (char *)"Xfire", (char *)"Edit Player's Xfire ID");
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAWEBSITE, (char *)"Website", (char *)"Edit Player's Personal Website");
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAMISC1, (char *)"Miscellaneous 1", (char *)"Edit Player's Miscellaneous Data 1");
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAMISC2, (char *)"Miscellaneous 2", (char *)"Edit Player's Miscellaneous Data 2");

	calculationMenu->Append(WINDOW_ID_MENU_SCORE_WEIGHTS, (char *)"Score Weights", (char *)"Edit Score Weights");
	calculationMenu->Append(WINDOW_ID_MENU_SKILL_WEIGHTS, (char *)"Skill Weights", (char *)"Edit Skill Weights");
	calculationMenu->Append(WINDOW_ID_MENU_AWARD, (char *)"Award Editing", (char *)"Edit Award Configuration");

	mandatoryMenu->Append(WINDOW_ID_MENU_TEMPLATE_CONFIG, (char *)"Template Variable Config", (char *)"Change variables used by the template during output");
	mandatoryMenu->Append(WINDOW_ID_MENU_OUTPUT, (char *)"Output Config", (char *)"Edit How And Where Output Is Created");
	mandatoryMenu->Append(WINDOW_ID_MENU_SERVER, (char *)"Server Config", (char *)"Edit Server Connection Details");
	mandatoryMenu->Append(WINDOW_ID_MENU_WEBSITE, (char *)"Website Config", (char *)"Edit Website Connection Details");
	mandatoryMenu->Append(WINDOW_ID_MENU_IMAGEPACKS, (char *)"Upload Image Packs", (char *)"Upload appropriate image packs");
	mandatoryMenu->Append(WINDOW_ID_MENU_TEMPLATEPACKS, (char *)"Upload Template Packs", (char *)"Upload Alternative Templates");

	optionalMenu->Append(WINDOW_ID_MENU_CLAN, (char *)"Clan Config", (char *)"Edit Clan Details");
	optionalMenu->Append(WINDOW_ID_MENU_ALIAS, (char *)"Alias Editing", (char *)"Edit Player Alias Entries");
	optionalMenu->Append(WINDOW_ID_MENU_DROPLIST, (char *)"Drop List Editing", (char *)"Edit List Of Allowed Players");
	optionalMenu->Append(WINDOW_ID_MENU_BAD_WORDS, (char *)"Banned Speech Editing", (char *)"Edit List Of Words Not Allowed In Speech");
	optionalMenu->Append(WINDOW_ID_MENU_WEB_SERVER, (char *)"Web Server Setting", (char *)"Configure Embedded Webserver");
	optionalMenu->Append(WINDOW_ID_MENU_EXTERNAL_DATABASE, (char *)"External Database Config", (char *)"Configure MySQL connection");
	rconMenu->Append(WINDOW_ID_MENU_RCON_CONFIG, (char *)"Message Centre", (char *)"Configure Game Server Messages");
	rconMenu->Append(WINDOW_ID_MENU_MESSAGE_FORMATS, (char *)"Message Centre Formats", (char *)"Edit Message Centre Formats");
	rconMenu->Append(WINDOW_ID_MENU_CUSTOM_MESSAGES, (char *)"Custom Messages", (char *)"Edit Custom Messages");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_GAMETYPE, (char *)"Game Type", (char *)"Log File Substitution For Game Type");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_MAP, (char *)"Map", (char *)"Log File Substitution For Map");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_CLASS, (char *)"Class", (char *)"Log File Substitution For Class");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_TEAM, (char *)"Team", (char *)"Log File Substitution For Team");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPON, (char *)"Weapon", (char *)"Log File Substitution For Weapon");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_AMMO, (char *)"Ammo", (char *)"Log File Substitution For Ammo");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPONAMMO, (char *)"Weapon And Ammo", (char *)"Log File Substitution For Weapon And Ammo");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_LOCATION, (char *)"Location", (char *)"Log File Substitution For Location");
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_ACTION, (char *)"Action", (char *)"Log File Substitution For Action");

	devMenu->Append(WINDOW_ID_MENU_DEBUG, (char *)"Debug Settings", (char *)"Developer Debug Settings");
	devMenu->Append(WINDOW_ID_MENU_EXPORT_TEMPLATE, (char *)"Export Template", (char *)"Export Template To C++");
	devMenu->Append(WINDOW_ID_MENU_EXPORT_UPGRADE, (char *)"Export Upgrade File", (char *)"Prepare An Upgrade File");
	devMenu->Append(WINDOW_ID_MENU_EXPORT_BUILD_INI, (char *)"Export Build INI File", (char *)"The Build INI File");

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, (char *)"&Run");
	menuBar->Append(calculationMenu, (char *)"&Calculation");
	menuBar->Append(displayMenu, (char *)"&Display");
	menuBar->Append(mandatoryMenu, (char *)"&Mandatory");
	menuBar->Append(optionalMenu, (char *)"&Optional");
	menuBar->Append(playerData, (char *)"&Player Data");
	menuBar->Append(rconMenu, (char *)"&Message Centre");
	menuBar->Append(logSubstituteMenu, (char *)"&Logfile Substitutions");
	menuBar->Append(devMenu, (char *)"&Developer");
	menuBar->Append(helpMenu, (char *)"&Help");
	SetMenuBar(menuBar);
	CreateStatusBar(3);
	progress=new StatusPanel();
	progress->Create(this,wxID_ANY);
	progress->CreatePanels();
	SetStatusText((char *)"Status Text");
	globalStatistics.ReadScheduleConfig();
/*
	if (FirstTimeRun())
	{
		GUIWizardPagesFirstConfig	wizardPages;
		RunWizard(&wizardPages);
	}
	Upgrade();
*/
	if (!NeedsFirstTimeRun())
	{
		Upgrade();
	}
	scheduleTimer.SetOwner(this);
	scheduleTimer.Start(1000);

	runOnceFlag=false;
	globalStatistics.hiddenFeatures=false;
	for (argIndex=1;argIndex<wxGetApp().argc;argIndex++)
	{
		if (wxGetApp().argc>1)
		{
			if (strcmp(wxGetApp().argv[argIndex],"-runonce")==0)
			{
				runOnceFlag=true;
			}
			if (strcmp(wxGetApp().argv[argIndex],"-elric")==0)
			{
				globalStatistics.hiddenFeatures=true;
			}
		}
	}
	if (runOnceFlag)
	{
			quitNow=true;
			wxCommandEvent	dummyEvent;
			OnGo(dummyEvent);
	}
	SetMenuItems();
	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(progress,1,wxEXPAND);
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
	Fit();

//	globalStatistics.webServer.StartServer();
}

void wxWidgetsFrame::OnFullConfig(wxCommandEvent &event)
{
	GUIWizardPagesFullConfig	wizardPages;
	RunWizard(&wizardPages);
}

void wxWidgetsFrame::SetMenuItems()
{
	STATSGEN_DEBUG_FUNCTION_START("wxWidgetsFrame","SetMenuItems")
	wxString		configKey;
	wxString		configValue;
	wxMenuBar		*menuBar;
	configKey="/General/advancedmenus";
	bool			firstTimeRun;

	runningWizard=false;
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"n");
	globalStatistics.configData.CommitChanges();
	advanced=(configValue.CmpNoCase("Y")==0);
	menuBar = GetMenuBar();
	firstTimeRun=NeedsFirstTimeRun();
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Wizard")
	menuBar->Enable(WINDOW_ID_WIZARD, firstTimeRun || advanced);

	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Advanced")
	menuBar->Enable(WINDOW_ID_MENU_ADVANCED, true && (!firstTimeRun));
	menuBar->Check(WINDOW_ID_MENU_ADVANCED, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Help")
	menuBar->Enable(WINDOW_ID_MENU_HELP, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling About")
	menuBar->Enable(WINDOW_ID_MENU_ABOUT, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Go")
	menuBar->Enable(WINDOW_ID_MENU_GO, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Empty DB")
	menuBar->Enable(WINDOW_ID_MENU_EMPTY_DATABASE, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Thread")
	menuBar->Enable(WINDOW_ID_MENU_THREAD, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Upgrade")
	menuBar->Enable(WINDOW_ID_MENU_PERFORM_UPGRADE, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Reset Recovery")
	menuBar->Enable(WINDOW_ID_MENU_RESET_RECOVERY, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Store Progress")
	menuBar->Enable(WINDOW_ID_MENU_STORE_PROGRESS, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Quit")
	menuBar->Enable(WINDOW_ID_MENU_QUIT, true && (!firstTimeRun));

	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Weapon Groups")
	menuBar->Enable(WINDOW_ID_MENU_WEAPON_GROUPS, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Real names")
	menuBar->Enable(WINDOW_ID_MENU_REAL_NAMES, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Images")
	menuBar->Enable(WINDOW_ID_MENU_IMAGES, advanced && (!firstTimeRun));
	// after
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Full Config")
//	menuBar->Enable(WINDOW_ID_MENU_FULL_CONFIG, true && (!firstTimeRun));

//before
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Playerdata")
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAAVATAR, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAREALNAME, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAPICTURE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATACLANROLE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAXFIRE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAWEBSITE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAMISC1, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAMISC2, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling Score/skill weights")
	menuBar->Enable(WINDOW_ID_MENU_SCORE_WEIGHTS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_SKILL_WEIGHTS, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling awards")
	menuBar->Enable(WINDOW_ID_MENU_AWARD, advanced && (!firstTimeRun));

	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling template config")
	menuBar->Enable(WINDOW_ID_MENU_TEMPLATE_CONFIG, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling output")
	menuBar->Enable(WINDOW_ID_MENU_OUTPUT, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling server")
	menuBar->Enable(WINDOW_ID_MENU_SERVER, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling website")
	menuBar->Enable(WINDOW_ID_MENU_WEBSITE, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling imagepacks")
	menuBar->Enable(WINDOW_ID_MENU_IMAGEPACKS, true && (!firstTimeRun));

	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling clan")
	menuBar->Enable(WINDOW_ID_MENU_CLAN, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling alias")
	menuBar->Enable(WINDOW_ID_MENU_ALIAS, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling droplist")
	menuBar->Enable(WINDOW_ID_MENU_DROPLIST, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling bad words")
	menuBar->Enable(WINDOW_ID_MENU_BAD_WORDS, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling web server")
	menuBar->Enable(WINDOW_ID_MENU_WEB_SERVER, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling external db")
	menuBar->Enable(WINDOW_ID_MENU_EXTERNAL_DATABASE, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling rcon config")
	menuBar->Enable(WINDOW_ID_MENU_RCON_CONFIG, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling message formats")
	menuBar->Enable(WINDOW_ID_MENU_MESSAGE_FORMATS, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling custom messages")
	menuBar->Enable(WINDOW_ID_MENU_CUSTOM_MESSAGES, true && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling log substitution")
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_GAMETYPE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_MAP, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_CLASS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_TEAM, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPON, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_AMMO, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPONAMMO, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_LOCATION, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_ACTION, advanced && (!firstTimeRun));

	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling advanced")
	menuBar->Enable(WINDOW_ID_MENU_DEBUG, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling template")
	menuBar->Enable(WINDOW_ID_MENU_EXPORT_TEMPLATE, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling upgrade")
	menuBar->Enable(WINDOW_ID_MENU_EXPORT_UPGRADE, advanced && (!firstTimeRun));
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Enabling build ini")
	menuBar->Enable(WINDOW_ID_MENU_EXPORT_BUILD_INI, advanced && (!firstTimeRun));
	STATSGEN_DEBUG_FUNCTION_END
}

void wxWidgetsFrame::OnAdvanced(wxCommandEvent &event)
{
	wxString		configKey;
	wxString		configValue;
	configKey="/General/advancedmenus";

	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"n");
	advanced=(configValue.CmpNoCase("Y")==0);
	if (advanced)
	{
		configValue="n";
	}
	else
	{
		configValue="y";
	}
	globalStatistics.configData.WriteTextValue(configKey,configValue);
	SetMenuItems();
}

wxWidgetsFrame::~wxWidgetsFrame()
{
	scheduleTimer.Stop();
}

void wxWidgetsFrame::OnTimerTick(wxTimerEvent& event)
{
	wxStatusBar		*statusBar;
	bool			needsToRun;
	long			timeLeft;
	long			hours;
	long			minutes;
	long			seconds;
	wxString		statusText;
	ScheduleItem	*schedule;
	wxString		schedulePrefix;
	int				scheduleIndex;
	int				fieldIndex;
	wxString		configKey;
	wxString		override;

	scheduleTimer.Stop();
	statusBar=GetStatusBar();

	if ((statusBar!=NULL)&&(!runningWizard))
	{
		for (scheduleIndex=0;scheduleIndex<2;scheduleIndex++)
		{
			override="y";
			switch (scheduleIndex)
			{
				case 0:
					schedule=&globalStatistics.messageTimer;
					schedulePrefix="Message Centre";
					configKey="/RCONSettings/Enabled";
					globalStatistics.configData.ReadTextValue(configKey,&override,(char *)"n");
					break;
				case 1:
					schedule=&globalStatistics.productionRunTimer;
					schedulePrefix="Logfile Processing";
					break;
			}
			fieldIndex=scheduleIndex+1;
			needsToRun=(schedule->TimeToRun(&timeLeft)&&(override.CmpNoCase("y")==0));
			if (needsToRun)
			{
				statusText.Printf("%s: Running",STRING_TO_CHAR(schedulePrefix));
			}
			else
			{
				if ((schedule->disabled)||(override.CmpNoCase("n")==0))
				{
					statusText.Printf("%s: Disabled",
								STRING_TO_CHAR(schedulePrefix));
				}
				else
				{
					hours=timeLeft/3600;
					minutes=timeLeft-(hours*3600);
					seconds=minutes % 60;
					minutes/=60;
					statusText.Printf("%s: %02ld:%02ld:%02ld",
								STRING_TO_CHAR(schedulePrefix),
								hours,minutes,seconds);
				}
			}
			SetStatusText(statusText,fieldIndex);
			if (needsToRun)
			{
				TemplateFile	templateFile;

				switch (scheduleIndex)
				{
					case 0:
						globalStatistics.ReadServersFromConfig();
						globalStatistics.SendMessages();
						globalStatistics.ReadMessageTimerConfig();
						break;
					case 1:
						wxBusyCursor	cursor;
						globalStatistics.ProduceStatsInitiate();
						globalStatistics.ProduceStatsDownload();
#ifdef USE_THREADS
						workerThread=new WorkerThread(wxTHREAD_JOINABLE);
						if (workerThread!=NULL)
						{
							workerThread->Create();
							workerThread->SetPriority(globalStatistics.ReadPriority(true));
							workerThread->Run();
							workerThread->Wait();
							delete (workerThread);
						}
						workerThread=NULL;
#else
						globalStatistics.ProduceStatsProcess();
#endif
						globalStatistics.ProduceStatsTransfer();
						globalStatistics.ProduceStatsFinalise();

						if (quitNow)
						{
							progress->DisableUpdate();
							Close(TRUE);
						}
						globalStatistics.ReadProductionRunTimerConfig();
						globalStatistics.ReadQuickRunTimerConfig();
						break;
				}
			}
		}
	}
	scheduleTimer.Start(1000);
}

void wxWidgetsFrame::OnDatabaseEmpty(wxCommandEvent& event)
{
	wxBusyCursor	busy;
	globalStatistics.statsgenDatabase.OpenDB();
	
	globalStatistics.statsgenDatabase.Zap();
}
void wxWidgetsFrame::OnGo(wxCommandEvent& event)
{
	wxString	msg;

	globalStatistics.productionRunTimer.StartNow();

/*
	int			index;
	int			maxSize=100;
	progress->Initiate(maxSize,
					(char *)"rounds",
					1,
					(char *)"rounds",
					1);
	for (index=0;index<maxSize;index++)
	{
		msg.Printf("doing interation %d",index);
		progress->Update(index);
		progress->SetStatus(msg);
		wxMilliSleep(1000);
	}
	progress->Finalise();

*/
}

void wxWidgetsFrame::OnQuit(wxCommandEvent& event)
{
//	globalStatistics.webServer.StopServer();
	progress->DisableUpdate();
	Close(TRUE);
}

void wxWidgetsFrame::OnEditScoreWeights(wxCommandEvent& event)
{
	wxString		group="ScoreWeights";
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	ServerTypePanel	*serverType=new ServerTypePanel();

	ConfigEditorGUI	*gui;

	gui=new ConfigEditorGUI(this,
						-1,
						(char *)"Edit Score Weights",
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"",
						(BoxedDropDown *)NULL,
						(BoxedDropDown *)serverType,
						(GroupedConfigItemsPanel *)NULL,
						false);

	globalStatistics.configData.ReadGroup(group,
							configKeys,
							configValues);
	
	keyCount=configKeys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=configKeys.Item(keyIndex);
		gui->AddConfigKey(group,key,true);
	}

	gui->DisplayDialog();

	delete gui;
}

void wxWidgetsFrame::OnEditSkillWeights(wxCommandEvent& event)
{
	wxString		group="SkillWeights";
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	ServerTypePanel	*serverType=new ServerTypePanel();
	SkillTypePanel	*skillType=new SkillTypePanel();
	wxString		configKey;
	wxArrayString	weaponGroups;

	weaponGroups=globalStatistics.configData.ReadWeaponGroups();

	GroupedConfigItemsPanel	*configPanel=new GroupedConfigItemsPanel((char *)"Negative Skill");

	ConfigEditorGUI	gui(this,
						wxID_ANY,
						(char *)"Edit Skill Weights",
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"",
						(BoxedDropDown *)skillType,
						(BoxedDropDown *)serverType,
						configPanel,
						false);

	configPanel->CreateDisplay(&gui,wxID_ANY);

	configKey=globalStatistics.SkillCalculationTypeConfigKey();
	configPanel->AddBoolean((char *)"Allow Negative Skill",configKey,true);
	globalStatistics.configData.ReadGroup(group,
							configKeys,
							configValues);
	
	WX_APPEND_ARRAY(configKeys,weaponGroups);
	keyCount=configKeys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=configKeys.Item(keyIndex);
		gui.AddConfigKey(group,key,true);
	}

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnEditRealNames(wxCommandEvent& event)
{
	wxString		group="RealNames";
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	ServerTypePanel	*serverType=new ServerTypePanel();
	ImageTypePanel	*imageType=new ImageTypePanel();
	ConfigEditorGUI	gui(this,
						-1,
						(char *)"Edit Real Names",
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"",
						(BoxedDropDown *)imageType,
						(BoxedDropDown *)serverType,
						(GroupedConfigItemsPanel *)NULL,
						false);

	globalStatistics.configData.ReadGroup(group,
							configKeys,
							configValues);
	
	keyCount=configKeys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=configKeys.Item(keyIndex);
		gui.AddConfigKey(group,key,false);
	}

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnEditImages(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("wxWidgetsFrame","OnEditImages")
	wxString		group="Images";
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating Server Type Panel");
	ServerTypePanel	*serverType=new ServerTypePanel();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating Image Type Panel");
	ImageTypePanel	*imageType=new ImageTypePanel();
	wxArrayString	weaponGroups;

	weaponGroups=globalStatistics.configData.ReadWeaponGroups();

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating GUI");
	ConfigEditorGUI	gui(this,
						-1,
						(char *)"Edit Image References",
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"",
						(BoxedDropDown *)imageType,
						(BoxedDropDown *)serverType,
						(GroupedConfigItemsPanel *)NULL,
						true);

	globalStatistics.configData.ReadGroup(group,
							configKeys,
							configValues);

	WX_APPEND_ARRAY(configKeys,weaponGroups);
	
	keyCount=configKeys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=configKeys.Item(keyIndex);
		gui.AddConfigKey(group,key,true);
	}

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Starting GUI");
	gui.DisplayDialog();
	STATSGEN_DEBUG_FUNCTION_END
}

void wxWidgetsFrame::OnWebsiteConfig(wxCommandEvent& event)
{
	if (advanced)
	{
		wxString				title="Website Config";
		GroupedConfigItemsPanel	*remoteDirectory;
		wxString				configKey;
		wxString				configGroup="website";
		GUITriggerItem			triggerItem;
		GUITriggerList			triggerList;
		wxString				enabledKey;

		enabledKey.Printf("/%s/FTPEnabled",STRING_TO_CHAR(configGroup));
		triggerItem.SetPositiveDisableTrigger(enabledKey,(char *)"N");
		triggerList.Add(triggerItem);

		GenericConfigGUI	gui(this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"");

		FTPConnectionPanel	*ftpSettings;
		GenericConfigPanel	*configPanel;

		configPanel=new GenericConfigPanel(&gui,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

		// Panel holds the various boxed configs
		remoteDirectory=new GroupedConfigItemsPanel((char *)"Remote Directory");
		remoteDirectory->CreateDisplay(configPanel,wxID_ANY);

		ftpSettings=new FTPConnectionPanel();
		ftpSettings->CreateDisplay(configPanel, wxID_ANY);
		ftpSettings->CreateConfigs(configGroup);


		configKey="/website/FTPRemoteDirectory";
		remoteDirectory->AddRemoteDirectory((char *)"Remote Directory",configKey,(char *)"",-1,configGroup,&triggerList);

		configPanel->AddConfigGroup(ftpSettings);
		configPanel->AddConfigGroup(remoteDirectory);
	
		gui.SetConfigPanel(configPanel);
		gui.DisplayDialog();
	}
	else
	{
		GUIWizardPagesWebsite	wizardPages;
		RunWizard(&wizardPages);
	}
}

void wxWidgetsFrame::OnImagePackConfig(wxCommandEvent& event)
{
	GUIWizardPagesImagePacks	wizardPages;
	RunWizard(&wizardPages);
}

void wxWidgetsFrame::OnTemplatePackConfig(wxCommandEvent& event)
{
	GUIWizardPagesTemplatePacks	wizardPages;
	RunWizard(&wizardPages);
}

wxPanel *NewServerPanel(wxWindow *parentID,wxString &serverID)
{
	FTPConnectionPanel				*ftpSettings;
	GenericConfigPanel				*serverConfigPanel;
	GroupedConfigItemsPanel			*serverType;
	GroupedConfigItemsPanel			*mainLogFile;
	GroupedConfigItemsPanel			*archiveLogFile;
	GroupedConfigItemsPanel			*banFile1;
	GroupedConfigItemsPanel			*banFile2;
	wxString						configGroup;
	wxString						configKey;
	DirectoryConfigItemGUI			*directoryGUI;
	DirectoryConfigItemGUI			*secondarydirectoryGUI;
	RemoteDirectoryConfigItemGUI	*remoteDirectoryGUI;
	RemoteDirectoryConfigItemGUI	*secondaryremoteDirectoryGUI;
	wxArrayString					serverTypeCodes;
	wxArrayString					serverTypeNames;
	int								serverTypeIndex;
	wxString						serverTypeCode;
	wxString						serverTypeName;
	wxFileName						baseDirectory(wxGetApp().argv[0]);
	wxFileName						logDirectory;
	wxFileName						archiveDirectory;
	wxFileName						banDirectory;
	wxString						logDirectoryName;
	wxString						archiveDirectoryName;
	wxString						banDirectoryName;
	wxString						baseDirectoryName;
	wxString						serverTypeConfigKey;
	GUITriggerList					ftpTriggerList;
	GUITriggerList					serverTypeTriggerList;
	GUITriggerList					serverTypeAndFtpTriggerList;
	GUITriggerItem					triggerItem;
	wxString						enabledKey;
	wxArrayString					wildCardCodes;
	wxArrayString					wildCardNames;
	wxString						code;
	wxString						name;

	code="$$DAY$$_$$MONTH$$_$$YEAR$$_gameserver.log";
	name="Tatakai Filename Format";
	wildCardCodes.Add(code);
	wildCardNames.Add(name);
	//baseDirectory=baseDirectory.GetPath();
	baseDirectory.AppendDir(serverID);
	baseDirectory.MakeAbsolute();
	logDirectory=baseDirectory;
	archiveDirectory=baseDirectory;
	banDirectory=baseDirectory;
	archiveDirectory.AppendDir((char *)"archivelogs");
	banDirectory.AppendDir((char *)"ban");
	logDirectory.AppendDir((char *)"logs");

	logDirectoryName=logDirectory.GetPath();
	banDirectoryName=banDirectory.GetPath();
	archiveDirectoryName=archiveDirectory.GetPath();
	baseDirectoryName=baseDirectory.GetPath();

	#ifdef _LINUX
		mkdir(baseDirectoryName,0777);
		mkdir(banDirectoryName,0777);
		mkdir(logDirectoryName,0777);
		mkdir(archiveDirectoryName,0777);
	#else
		wxMkDir(baseDirectoryName);
		wxMkDir(banDirectoryName);
		wxMkDir(logDirectoryName);
		wxMkDir(archiveDirectoryName);
	#endif

	for (serverTypeIndex=0;serverTypeIndex<SERVER_TYPE_COUNT;serverTypeIndex++)
	{
		serverTypeCode=ServerTypeCodes[serverTypeIndex];
		serverTypeName=ServerTypeNames[serverTypeIndex];
		serverTypeCodes.Add(serverTypeCode);
		serverTypeNames.Add(serverTypeName);
	}
	configGroup="Server"+serverID;

	enabledKey.Printf("/%s/FTPEnabled",STRING_TO_CHAR(configGroup));
	serverTypeConfigKey.Printf("/%s/serverType",STRING_TO_CHAR(configGroup));

	
	triggerItem.SetNegativeHideTrigger(serverTypeConfigKey,(char *)"QUAKEWARS");
	serverTypeTriggerList.Add(triggerItem);
	serverTypeAndFtpTriggerList.Add(triggerItem);

	triggerItem.SetPositiveHideTrigger(enabledKey,(char *)"N");
	ftpTriggerList.Add(triggerItem);
	serverTypeAndFtpTriggerList.Add(triggerItem);

	

	serverConfigPanel=new GenericConfigPanel(parentID,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

	// Panels holds the various boxed configs
	serverType		=new GroupedConfigItemsPanel((char *)"Server Type");
	serverType->CreateDisplay(serverConfigPanel,wxID_ANY);
	serverType->AddSelection((char *)"Server Type",serverTypeConfigKey,(char *)"COD2",serverTypeCodes,serverTypeNames);

	mainLogFile		=new GroupedConfigItemsPanel((char *)"Main Log File");
	mainLogFile->CreateDisplay(serverConfigPanel,wxID_ANY);

	configKey.Printf("/%s/GuessTeamIDs",STRING_TO_CHAR(configGroup));
	mainLogFile->AddBoolean((char *)"Guess players team if none found",configKey,true);

	configKey.Printf("/%s/MaxLogfileSize",STRING_TO_CHAR(configGroup));
	mainLogFile->Add((char *)"Max Logfile Size in MB (0 for no maximum)",configKey,(char *)"0",-1);
	configKey.Printf("/%s/latestDirectory",STRING_TO_CHAR(configGroup));
	directoryGUI=mainLogFile->AddDirectory((char *)"Latest Active Logfile Directory",
											configKey,
											logDirectoryName,-1);
	configKey.Printf("/%s/latestFilename",STRING_TO_CHAR(configGroup));
	mainLogFile->AddFile((char *)"Latest Active Logfile Name",configKey,(char *)"",-1,directoryGUI);

	configKey.Printf("/%s/FTPLatestDirectory",STRING_TO_CHAR(configGroup));
	remoteDirectoryGUI=mainLogFile->AddRemoteDirectory((char *)"Remote Active Logfile Directory",configKey,(char *)"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPLatestFilename",STRING_TO_CHAR(configGroup));
	mainLogFile->AddRemoteFile((char *)"Remote Active Logfile Name",configKey,(char *)"",-1,configGroup,remoteDirectoryGUI,&ftpTriggerList);

	// Quake Wars bollox :-(
	configKey.Printf("/%s/secondarylatestDirectory",STRING_TO_CHAR(configGroup));
	secondarydirectoryGUI=mainLogFile->AddDirectory((char *)"(Quake Wars Objective) Latest Active Logfile Directory",
											configKey,
											logDirectoryName,-1,&serverTypeTriggerList);

	configKey.Printf("/%s/secondarylatestFilename",STRING_TO_CHAR(configGroup));
	mainLogFile->AddFile((char *)"(Quake Wars Objective) Latest Active Logfile Name",configKey,(char *)"",-1,secondarydirectoryGUI,&serverTypeTriggerList);

	configKey.Printf("/%s/FTPSecondaryLatestDirectory",STRING_TO_CHAR(configGroup));
	secondaryremoteDirectoryGUI=mainLogFile->AddRemoteDirectory((char *)"(Quake Wars Objective) Remote Active Logfile Directory",configKey,(char *)"",-1,configGroup,&serverTypeAndFtpTriggerList);
	configKey.Printf("/%s/FTPSecondaryLatestFilename",STRING_TO_CHAR(configGroup));
	mainLogFile->AddRemoteFile((char *)"(Quake Wars Objective) Remote Active Logfile Name",configKey,(char *)"",-1,configGroup,secondaryremoteDirectoryGUI,&serverTypeAndFtpTriggerList);

	
	archiveLogFile	=new GroupedConfigItemsPanel((char *)"Archive Log Files");
	archiveLogFile->CreateDisplay(serverConfigPanel,wxID_ANY);

	configKey.Printf("/%s/archiveDirectory",STRING_TO_CHAR(configGroup));
	archiveLogFile->AddDirectory((char *)"Archive Directory",
								configKey,
								archiveDirectoryName,-1);
	configKey.Printf("/%s/archiveWildcard",STRING_TO_CHAR(configGroup));
	archiveLogFile->AddSelectionFreeForm((char *)"Archive Wildcard",configKey,(char *)"",wildCardCodes,wildCardNames,-1);
	configKey.Printf("/%s/FTPArchiveDirectory",STRING_TO_CHAR(configGroup));
	remoteDirectoryGUI=archiveLogFile->AddRemoteDirectory((char *)"Remote Archive Directory",configKey,(char *)"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPArchiveWildcard",STRING_TO_CHAR(configGroup));
	//archiveLogFile->Add("Remote Archive Wildcard",configKey,"",-1,&ftpTriggerList);
	archiveLogFile->AddSelectionFreeForm((char *)"Remote Archive Wildcard",configKey,(char *)"",wildCardCodes,wildCardNames,-1,&ftpTriggerList);

	// More quake war bollox
	configKey.Printf("/%s/secondaryarchiveDirectory",STRING_TO_CHAR(configGroup));
	archiveLogFile->AddDirectory((char *)"(Quake Wars Objective) Archive Directory",
								configKey,
								archiveDirectoryName,-1,&serverTypeTriggerList);
	configKey.Printf("/%s/secondaryarchiveWildcard",STRING_TO_CHAR(configGroup));
	//archiveLogFile->Add("(Quake Wars Objective) Archive Wildcard",configKey,"",-1,&serverTypeTriggerList);
	archiveLogFile->AddSelectionFreeForm((char *)"(Quake Wars Objective) Archive Wildcard",configKey,(char *)"",wildCardCodes,wildCardNames,-1,&serverTypeTriggerList);
	configKey.Printf("/%s/FTPSecondaryArchiveDirectory",STRING_TO_CHAR(configGroup));
	secondaryremoteDirectoryGUI=archiveLogFile->AddRemoteDirectory((char *)"(Quake Wars Objective) Remote Archive Directory",configKey,(char *)"",-1,configGroup,&serverTypeAndFtpTriggerList);
	configKey.Printf("/%s/FTPSecondaryArchiveWildcard",STRING_TO_CHAR(configGroup));
	//archiveLogFile->Add("(Quake Wars Objective) Remote Archive Wildcard",configKey,"",-1,&serverTypeAndFtpTriggerList);
	archiveLogFile->AddSelectionFreeForm((char *)"(Quake Wars Objective) Remote Archive Wildcard",configKey,(char *)"",wildCardCodes,wildCardNames,-1,&serverTypeTriggerList);

	banFile1		=new GroupedConfigItemsPanel((char *)"Ban File 1");
	banFile1->CreateDisplay(serverConfigPanel,wxID_ANY);

	banFile2		=new GroupedConfigItemsPanel((char *)"Ban File 2");
	banFile2->CreateDisplay(serverConfigPanel,wxID_ANY);

	wxArrayString	banCodes;
	wxArrayString	banNames;
	wxString		banCode;
	wxString		banName;

	banCode="PUNKBUSTER";banName="Punk Buster";banCodes.Add(banCode);banNames.Add(banName);
	banCode="COD";banName="Call Of Duty";banCodes.Add(banCode);banNames.Add(banName);

	configKey.Printf("/%s/banFile1Type",STRING_TO_CHAR(configGroup));
	banFile1->AddSelection((char *)"Ban File Type",configKey,(char *)"PUNKBUSTER",banCodes,banNames);
	configKey.Printf("/%s/FTPBanFile1Directory",STRING_TO_CHAR(configGroup));
	remoteDirectoryGUI=banFile1->AddRemoteDirectory((char *)"Remote Ban 1 File Directory",configKey,(char *)"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPBanFile1Filename",STRING_TO_CHAR(configGroup));
	banFile1->AddRemoteFile((char *)"Remote Ban 1 File Filename",configKey,(char *)"",-1,configGroup,remoteDirectoryGUI,&ftpTriggerList);
	configKey.Printf("/%s/Ban1Directory",STRING_TO_CHAR(configGroup));
	directoryGUI=banFile1->AddDirectory((char *)"Local Ban Directory",
										configKey,
										banDirectoryName,-1);

	configKey.Printf("/%s/banFile2Type",STRING_TO_CHAR(configGroup));
	banFile2->AddSelection((char *)"Ban File Type",configKey,(char *)"COD",banCodes,banNames);
	configKey.Printf("/%s/FTPBanFile2Directory",STRING_TO_CHAR(configGroup));
	remoteDirectoryGUI=banFile2->AddRemoteDirectory((char *)"Remote Ban 2 File Directory",configKey,(char *)"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPBanFile2Filename",STRING_TO_CHAR(configGroup));
	banFile2->AddRemoteFile((char *)"Remote Ban 2 File Filename",configKey,(char *)"",-1,configGroup,remoteDirectoryGUI,&ftpTriggerList);
	configKey.Printf("/%s/Ban2Directory",STRING_TO_CHAR(configGroup));
	directoryGUI=banFile2->AddDirectory((char *)"Local Ban Directory",
										configKey,
										banDirectoryName,-1);

//	configKey.Printf("/%s/serverType",configGroup.GetData());
//	mainLogFile->Add("Server Type",configKey,"",-1);

	ftpSettings=new FTPConnectionPanel();
	ftpSettings->CreateDisplay(serverConfigPanel,wxID_ANY);

	ftpSettings->CreateConfigs(configGroup);
	serverConfigPanel->AddConfigGroup(serverType);
	serverConfigPanel->AddConfigGroup(ftpSettings);
	serverConfigPanel->AddConfigGroup(mainLogFile);
	serverConfigPanel->AddConfigGroup(archiveLogFile);
	serverConfigPanel->AddConfigGroup(banFile1);
	serverConfigPanel->AddConfigGroup(banFile2);
	serverConfigPanel->ConfigureSizer();

	return (serverConfigPanel);

}

void wxWidgetsFrame::OnServerConfig(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("wxWidgetsFrame","OnServerConfig")
	if (advanced)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Advanced");
		wxString			title="Server Config";
		wxString			listGroup="SERVERS";
		wxString			listPrefix="Server";

		GenericNumberedConfigGUI	gui(
							NewServerPanel,
							listGroup,
							listPrefix,
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"");

		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Display Dialog");
		gui.DisplayDialog();
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Simple");
		GUIWizardPagesServers	wizardPages;
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Run Wizard");
		RunWizard(&wizardPages);
	}
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Clear Servers");
	globalStatistics.ClearServers();
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Read Servers From Config");
	globalStatistics.ReadServersFromConfig();
	STATSGEN_DEBUG_FUNCTION_END
}

wxPanel *NewClanPanel(wxWindow *parentID,wxString &clanID)
{
	ClanEditorPanel		*configPanel;
	wxString			configGroup;

	configGroup="Clan"+clanID;
	configPanel=new ClanEditorPanel(configGroup,
									parentID,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

	return (configPanel);

}

void wxWidgetsFrame::OnClanConfig(wxCommandEvent& event)
{
	wxString			title="Clan Config";
	wxString			listGroup="CLANS";
	wxString			listPrefix="Clan";

	GenericNumberedConfigGUI	gui(
							NewClanPanel,
							listGroup,
							listPrefix,
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"");

	gui.DisplayDialog();
}

GenericConfigPanel *NewRCONServerPanel(wxWindow *parentID,wxString &serverID);
void wxWidgetsFrame::OnRCONSpamConfig(wxCommandEvent& event)
{
	wxString			title="RCON Spamming Config";
	wxString			listGroup="SERVERS";
	wxString			listPrefix="Server";
	wxString			configKey;
	GroupedConfigItemsPanel	*commonConfigs;
	GenericNumberedConfigPanel	*serverSpecificConfigs;
	GenericConfigPanel	*configPanel;
	GUITriggerList		triggerList;
	GUITriggerItem		triggerItem;

	GenericConfigGUI	gui(this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"");


	configPanel=new GenericConfigPanel(&gui,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

	// Panel holds the various boxed configs
	serverSpecificConfigs=new GenericNumberedConfigPanel
							(
							NewRCONServerPanel,
							listGroup,
							listPrefix,
							configPanel,
							wxID_ANY,
							wxDefaultPosition,
							wxDefaultSize,
							0,
							(char *)"");
	serverSpecificConfigs->CreateDialog();
	
	commonConfigs=new GroupedConfigItemsPanel((char *)"Common RCON Settings");
	commonConfigs->CreateDisplay(configPanel,wxID_ANY);

	configKey="/RCONSettings/Enabled";
	commonConfigs->AddBoolean((char *)"Message Center Enabled",configKey,false);
	triggerItem.SetPositiveDisableTrigger(configKey,(char *)"N");
	triggerList.Add(triggerItem);

	configKey="/RCONSettings/MessageIntervalTimer";
	commonConfigs->Add((char *)"Message Interval In Seconds",configKey,(char *)"60",5,&triggerList);
	configKey="/RCONSettings/CustomMessageInterval";
	commonConfigs->Add((char *)"Custom Message Interval",configKey,(char *)"0",5,&triggerList);
	configKey="/RCONSettings/MessagePrefix";
	commonConfigs->Add((char *)"Message Prefix",configKey,(char *)"^7",-1,&triggerList);
	configKey="/RCONSettings/ClientPort";
	commonConfigs->Add((char *)"Local TCP/IP Port",configKey,(char *)"8000",5,&triggerList);

	configPanel->AddConfigGroup(commonConfigs);
	configPanel->AddConfigGroup(serverSpecificConfigs);

	gui.SetConfigPanel(configPanel);
	gui.DisplayDialog();
	globalStatistics.InitiateMessageCentres();
}

GenericConfigPanel *NewRCONServerPanel(wxWindow *parentID,wxString &serverID)
{
	GenericConfigPanel		*configPanel;
	GroupedConfigItemsPanel	*punkBusterSettings;
	GroupedConfigItemsPanel	*gameServer;
	wxString				configGroup;
	wxString				configKey;
	GUITriggerList			triggerList;
	GUITriggerList			mainTriggerList;
	GUITriggerList			pbTriggerList;
	GUITriggerItem			triggerItem;
	wxString				usePunkbusterKey;

	configGroup="Server"+serverID;
	usePunkbusterKey.Printf("/%s/RCONUsePunkBuster",STRING_TO_CHAR(configGroup));

	configKey="/RCONSettings/Enabled";
	triggerItem.SetPositiveDisableTrigger(configKey,(char *)"N");
	triggerList.Add(triggerItem);
	mainTriggerList.Add(triggerItem);
	pbTriggerList.Add(triggerItem);

	configKey.Printf("/%s/MessagingEnabled",STRING_TO_CHAR(configGroup));
	triggerItem.SetPositiveDisableTrigger(configKey,(char *)"N");
	triggerList.Add(triggerItem);
	pbTriggerList.Add(triggerItem);

	triggerItem.SetPositiveDisableTrigger(usePunkbusterKey,(char *)"N");
	pbTriggerList.Add(triggerItem);

	configPanel=new GenericConfigPanel(parentID,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

	// Panels holds the various boxed configs
	punkBusterSettings=new GroupedConfigItemsPanel((char *)"PunkBuster Settings");
	punkBusterSettings->CreateDisplay(configPanel,wxID_ANY);

	punkBusterSettings->AddBoolean((char *)"Use Punkbuster Scheduled Tasks",usePunkbusterKey,false,&triggerList);
	configKey.Printf("/%s/PunkBusterPostCommand",STRING_TO_CHAR(configGroup));
	punkBusterSettings->Add((char *)"Post Messaging PB Command",configKey,(char *)"",-1,&pbTriggerList);
	configKey.Printf("/%s/FTPBanFile1Directory",STRING_TO_CHAR(configGroup));
	punkBusterSettings->AddRemoteDirectory((char *)"Remote Ban 1 File Directory",configKey,(char *)"",-1,configGroup,&pbTriggerList);
	configKey.Printf("/%s/FTPBanFile2Directory",STRING_TO_CHAR(configGroup));
	punkBusterSettings->AddRemoteDirectory((char *)"Remote Ban 2 File Directory",configKey,(char *)"",-1,configGroup,&pbTriggerList);


	gameServer	=new GroupedConfigItemsPanel((char *)"Game Server Settings");
	gameServer->CreateDisplay(configPanel,wxID_ANY);
	configKey.Printf("/%s/MessagingEnabled",STRING_TO_CHAR(configGroup));
	gameServer->AddBoolean((char *)"Messaging Enabled",configKey,false,&mainTriggerList);
	configKey.Printf("/%s/gameIP",STRING_TO_CHAR(configGroup));
	gameServer->Add((char *)"Game IP (if different from FTP Hostname)",configKey,(char *)"",-1,&triggerList);
	configKey.Printf("/%s/gamePort",STRING_TO_CHAR(configGroup));
	gameServer->Add((char *)"Game Port",configKey,(char *)"",5,&triggerList);
	configKey.Printf("/%s/RCONPassword",STRING_TO_CHAR(configGroup));
	gameServer->Add((char *)"RCON Password",configKey,(char *)"",-1,&triggerList);

	configPanel->AddConfigGroup(gameServer);
	configPanel->AddConfigGroup(punkBusterSettings);
	return (configPanel);

}

void wxWidgetsFrame::OnAliasConfig(wxCommandEvent& event)
{
	wxString			title="Alias Config";

	AliasDialog	gui(
					this,
					wxID_ANY,
					title,
					wxDefaultPosition,
					wxDefaultSize,
					wxCAPTION |
					wxCLOSE_BOX |
					wxSYSTEM_MENU |
					wxRESIZE_BORDER |
					wxMAXIMIZE_BOX,
					(char *)"");

	gui.DisplayDialog();
}

wxPanel *NewAwardPanel(wxWindow *parentID,wxString &awardID)
{
	AwardEditorPanel	*awardPanel;

	// Panels holds the various boxed configs
	awardPanel=new AwardEditorPanel(
						parentID,
						wxID_ANY,
						wxDefaultPosition,
						wxDefaultSize,
						0,
						(char *)"",
						awardID);

	awardPanel->Enable();
	return (awardPanel);
}

void wxWidgetsFrame::OnAwardConfig(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("wxWidgetsFrame","OnAwardConfig")
	wxString			title="Award Editor";
	wxString			listGroup="AWARDLIST";
	wxString			listPrefix="AWARDDEFINITION";

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating GUI");
	GenericNumberedConfigGUI	gui(
							NewAwardPanel,
							listGroup,
							listPrefix,
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"");

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Displaying GUI");
	gui.DisplayDialog();
	STATSGEN_DEBUG_FUNCTION_END
}

void wxWidgetsFrame::OnLogSubstitute(wxCommandEvent& event)
{
	wxString		group;
	wxString		title;
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	ServerTypePanel	*serverType=new ServerTypePanel();

	switch (event.GetId())
	{
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_GAMETYPE:
			title-"Edit Logfile Substitutions For Game Types";
			group="LogSubstituteGAMETYPE";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_MAP:
			title-"Edit Logfile Substitutions For Maps";
			group="LogSubstituteMAP";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_CLASS:
			title-"Edit Logfile Substitutions For Player Class";
			group="LogSubstituteCLASS";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_TEAM:
			title-"Edit Logfile Substitutions For Teams";
			group="LogSubstituteTEAM";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPON:
			title-"Edit Logfile Substitutions For Weapons";
			group="LogSubstituteWEAPON";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_AMMO:
			title-"Edit Logfile Substitutions For Ammo";
			group="LogSubstituteAMMO";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPONAMMO:
			title-"Edit Logfile Substitutions For Weapon And Ammo Combinations";
			group="LogSubstituteWEAPONAMMO";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_LOCATION:
			title-"Edit Logfile Substitutions For Locations";
			group="LogSubstituteLOCATION";
			break;
		case WINDOW_ID_MENU_LOG_SUBSTITUTE_ACTION:
			title-"Edit Logfile Substitutions For Actions";
			group="LogSubstituteACTION";
			break;
	}
	ConfigEditorGUI	gui(this,
						wxID_ANY,
						title,
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"",
						(BoxedDropDown *)serverType,
						(BoxedDropDown *)NULL,
						(GroupedConfigItemsPanel *)NULL,
						false);

	globalStatistics.configData.ReadGroup(group,
							configKeys,
							configValues);
	
	keyCount=configKeys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=configKeys.Item(keyIndex);
		gui.AddConfigKey(group,key,false);
	}

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnDropListConfig(wxCommandEvent& event)
{
	wxString			title="Drop List Editor";

	DropListDialog	gui(
						this,
						wxID_ANY,
						title,
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"");

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnDebugConfig(wxCommandEvent& event)
{
	wxString			title="Developer Debug Settings";
	wxString			configKey;
	GroupedConfigItemsPanel	*stepsEnabled;
	GroupedConfigItemsPanel	*debugLevels;
	GenericConfigPanel	*debugConfigPanel;

	GenericConfigGUI	gui(this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"");


	debugConfigPanel=new GenericConfigPanel(&gui,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

	// Panel holds the various boxed configs
	stepsEnabled=new GroupedConfigItemsPanel((char *)"Steps Enabled");
	stepsEnabled->CreateDisplay(debugConfigPanel,wxID_ANY);

	configKey.Printf("/Debug/Step%s",STEP_CREATE_DATABASE);
	stepsEnabled->AddBoolean((char *)"Create Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_APPLY_ALIASES);
	stepsEnabled->AddBoolean((char *)"Apply Aliases",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_DOWNLOAD_LOGS);
	stepsEnabled->AddBoolean((char *)"Download Logs",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_DATABASE_READ_STATIC);
	stepsEnabled->AddBoolean((char *)"Read Static",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_EMPTY_DATABASE);
	stepsEnabled->AddBoolean((char *)"Empty Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_CREATE_LOG_FILES);
	stepsEnabled->AddBoolean((char *)"Create Logs",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_PROCESS_LOG_FILES);
	stepsEnabled->AddBoolean((char *)"Process Logs",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_DROP_PLAYERS);
	stepsEnabled->AddBoolean((char *)"Drop Players",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_CALCULATE_SCORES);
	stepsEnabled->AddBoolean((char *)"Calculate Scores",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_WRITE_DATABASE);
	stepsEnabled->AddBoolean((char *)"Write Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_READ_TEMPLATE);
	stepsEnabled->AddBoolean((char *)"Read Template",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_PROCESS_TEMPLATE);
	stepsEnabled->AddBoolean((char *)"Process Template",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_POST_PROCESS_TEMPLATE);
	stepsEnabled->AddBoolean((char *)"Post Process Template",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_TRANSFER_DATABASE);
	stepsEnabled->AddBoolean((char *)"Transfer Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_WEBSITE);
	stepsEnabled->AddBoolean((char *)"Website Transfer",configKey,true);

	debugLevels=new GroupedConfigItemsPanel((char *)"Debug Levels 0 (off) - 3 (max)");
	debugLevels->CreateDisplay(debugConfigPanel,wxID_ANY);
	int			panelIndex;
	wxString	debugTitle;

	configKey="/Debug/DebugFile";
	debugLevels->AddFile((char *)"Debug File (blank for no debug)",configKey,(char *)"",-1,NULL);
	for (panelIndex=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		panelIndex<WINDOW_ID_PROGRESS_PANEL_MAX;
		panelIndex++)
	{
		configKey.Printf("/Debug/DebugLevel%d",
						(panelIndex-WINDOW_ID_PROGRESS_PANEL_GENERAL));
		debugTitle=StatusPanel::PanelName(panelIndex);
		debugLevels->Add(debugTitle,configKey,(char *)"0",1);
	}

	debugConfigPanel->AddConfigGroup(stepsEnabled);
	debugConfigPanel->AddConfigGroup(debugLevels);

	gui.SetConfigPanel(debugConfigPanel);
	gui.DisplayDialog();

	STATSGEN_DEBUG_INITIATE
}

void wxWidgetsFrame::OnOutputConfig(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("wxWidgetsFrame","OnOutputConfig")
	wxString				title="Output Config";
	wxString				defaultValue;
	GroupedConfigItemsPanel	*databaseConfig;
	GroupedConfigItemsPanel	*fullTemplateConfig;
	GroupedConfigItemsPanel	*outputConfig;
	wxString				configKey;
	wxString				configGroup="website";

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"gui");
	GenericConfigGUI	gui(this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"");

	GenericConfigPanel	*configPanel;

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"configPanel");
	configPanel=new GenericConfigPanel(&gui,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

	// Database Configuration
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"databaseConfig");
	databaseConfig=new GroupedConfigItemsPanel((char *)"Database Config");
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"databaseConfig create");
	databaseConfig->CreateDisplay(configPanel,wxID_ANY);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"dbFilename");
	wxString	dbFilename=BaseDirectoryFileName((char *)"statsgen2.db").GetFullPath();
	configKey="/Database/filename";
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddFile dbFilename");
	databaseConfig->AddFile((char *)"Database File",configKey,dbFilename,-1,NULL);
	configKey="/Database/CreateIndexes";
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddBoolean indexes");
	databaseConfig->AddBoolean((char *)"Create Indexes",configKey,true);

	// Full Template Configuration
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"fullTemplateConfig");
	fullTemplateConfig=new GroupedConfigItemsPanel((char *)"Template Config");
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"fullTemplateConfig create");
	fullTemplateConfig->CreateDisplay(configPanel,wxID_ANY);

	configKey="/Template/FullRun";
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddFile FullRun");
	fullTemplateConfig->AddFile((char *)"Template",configKey,(char *)"",-1,NULL);
	configKey="/Schedule/ProductionRun";
	defaultValue="00:00:00,86400";
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddSchedule FullRun");
	fullTemplateConfig->AddSchedule((char *)"Schedule",configKey,defaultValue,true);

	// Output Configuration
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"local output config");
	outputConfig=new GroupedConfigItemsPanel((char *)"Local Output Config");
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"local output config create");
	outputConfig->CreateDisplay(configPanel,wxID_ANY);
	configKey="/General/LocalOutput";
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddDirectory localOutput");
	outputConfig->AddDirectory((char *)"Local Output Location",configKey,(char *)"",-1);

	configKey=CONFIG_KEY_SPEECHFILE;
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddFile speech");
	outputConfig->AddFile((char *)"Speech File (blank for none)",configKey,(char *)"",-1);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddConfigGroup databaseConfig");
	configPanel->AddConfigGroup(databaseConfig);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddConfigGroup fullTemplateConfig");
	configPanel->AddConfigGroup(fullTemplateConfig);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AddConfigGroup outputConfig");
	configPanel->AddConfigGroup(outputConfig);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"SetConfigPanel");
	gui.SetConfigPanel(configPanel);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"DisplayDialog");
	gui.DisplayDialog();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Read Timer Configs");
	globalStatistics.ReadProductionRunTimerConfig();
	globalStatistics.ReadQuickRunTimerConfig();
	STATSGEN_DEBUG_FUNCTION_END
}

void wxWidgetsFrame::OnTemplateConfig(wxCommandEvent& event)
{
	wxString		group="TemplateConfigs";
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	ServerTypePanel	*serverType=NULL;
	ImageTypePanel	*imageType=NULL;
	ConfigEditorGUI	gui(this,
						wxID_ANY,
						(char *)"Edit Template Config Items",
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"",
						(BoxedDropDown *)imageType,
						(BoxedDropDown *)serverType,
						(GroupedConfigItemsPanel *)NULL,
						false);

	globalStatistics.configData.ReadGroup(group,
							configKeys,
							configValues);
	
	keyCount=configKeys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=configKeys.Item(keyIndex);
		gui.AddConfigKey(group,key,false);
	}

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPerformUpgrade(wxCommandEvent& event)
{
	ManualUpgrade();
}

void wxWidgetsFrame::OnExportTemplate(wxCommandEvent& event)
{
	wxString	templateFilename;
	wxString	cppFilename;
	wxString	message;
	wxString	prefix="XXX";

	message="Select Template File";
	prefix="Full";
	templateFilename=wxFileSelector(message);
	message="Select CPP File";
	cppFilename=wxFileSelector(message);

	TemplateFile	templateFile;
	templateFile.ReadTemplate(templateFilename);
	templateFile.ExportToCPP(cppFilename,prefix);
}

void wxWidgetsFrame::OnEditMessageFormats(wxCommandEvent& event)
{
	wxString		group="MessageCentreFormats";
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	ServerTypePanel	*serverType=NULL;
	ImageTypePanel	*imageType=NULL;
	ConfigEditorGUI	gui(this,
						wxID_ANY,
						(char *)"Edit Message Centre Formats",
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						(char *)"",
						(BoxedDropDown *)imageType,
						(BoxedDropDown *)serverType,
						(GroupedConfigItemsPanel *)NULL,
						false);

	globalStatistics.configData.ReadGroup(group,
							configKeys,
							configValues);
	
	keyCount=configKeys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=configKeys.Item(keyIndex);
		gui.AddConfigKey(group,key,true);
	}

	gui.DisplayDialog();
	globalStatistics.InitiateMessageCentres();
}

void wxWidgetsFrame::OnWizard(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("wxWidgetsFrame","OnWizard")
	if (FirstTimeRun())
	{
		GUIWizardPagesFirstConfig	wizardPages;
		RunWizard(&wizardPages);
	}
	Upgrade();
	SetMenuItems();
	STATSGEN_DEBUG_FUNCTION_END
}

void wxWidgetsFrame::OnAbout(wxCommandEvent& event)
{
	wxString	msg;

	msg.Printf(
		"Statsgen 2\n"
		"http://www.statsgen.co.uk - Version %s\n"
		"\n"
		"http://www.sqlite.org     - Version %s\n"
		"http://www.wxwidgets.org  - Version %s\n"
		"\n"
		"Default Templates created by Nazarine and\n"
		"Omega of www.ssrangers.ca\n"
		"\n"
		"Big thanks to Omega for all the work on the templates\n"
		"\n"
		"Another big thanks to Elric666 (www.snl-clan.com) for all\n"
		"the help testing Quake Wars processing with me\n"
		,
		PROGRAM_VERSION,
		SQLITE_VERSION,
		wxVERSION_STRING
		);
	wxMessageBox(msg, (char *)"About",wxOK | wxICON_INFORMATION,this);

}

void wxWidgetsFrame::RunWizard(GUIWizardPages *wizardPages)
{
	wxString		title		="Statsgen Wizard";
	long			wizardStyle	=	wxRESIZE_BORDER |
									wxMAXIMIZE_BOX |
									wxMINIMIZE_BOX;

	wxSizer	*pageSizer;
	runningWizard=true;
	StatsgenWizard		wizard(NULL,wxID_ANY,title,wxNullBitmap,wxDefaultPosition, wizardStyle);

	wizardPages->CreateWizardPages(&wizard);
	wizard.SetInitialPageLinks();
	//wizard.FitToPage(wizard.GetFirstPage());
	//pageSizer = wizard.GetPageAreaSizer();
	//pageSizer->Add(wizard.GetFirstPage());
	wizard.RunWizard(wizard.GetFirstPage());
	runningWizard=false;
}

void wxWidgetsFrame::OnPlayerDataAvatarConfig(wxCommandEvent& event)
{
	wxString			title="Avatar Editor";
	wxString			playerDefaultValue="Insert Avatar Image Reference Here";
	wxString			playerTitle="Avatar Reference";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataAvatars,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataPictureConfig(wxCommandEvent& event)
{
	wxString			title="Personal Picture Editor";
	wxString			playerDefaultValue="Insert Personal Picture Reference Here";
	wxString			playerTitle="Personal Picture Reference";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataPicture,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataWebsiteConfig(wxCommandEvent& event)
{
	wxString			title="Personal Website Editor";
	wxString			playerDefaultValue="Insert Personal Website Reference Here";
	wxString			playerTitle="Personal Website Reference";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataWebsite,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataXfireConfig(wxCommandEvent& event)
{
	wxString			title="Personal Xfire Editor";
	wxString			playerDefaultValue="Insert Xfire ID Here";
	wxString			playerTitle="Xfire ID";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataXfire,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataClanRoleConfig(wxCommandEvent& event)
{
	wxString			title="Clan Role Editor";
	wxString			playerDefaultValue="Insert Clan Role Here";
	wxString			playerTitle="Clan Role";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataClanRole,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataRealNameConfig(wxCommandEvent& event)
{
	wxString			title="Real Name Editor";
	wxString			playerDefaultValue="Insert Real Name Here";
	wxString			playerTitle="Real Name";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataRealName,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataMisc1Config(wxCommandEvent& event)
{
	wxString			title="Miscellaneous Data 1 Editor";
	wxString			playerDefaultValue="Insert Data Here";
	wxString			playerTitle="Data";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataMisc1,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataMisc2Config(wxCommandEvent& event)
{
	wxString			title="Miscellaneous Data 2 Editor";
	wxString			playerDefaultValue="Insert Data Here";
	wxString			playerTitle="Data";

	PlayerDataDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						globalStatistics.playerDataMisc2,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnBadWordsListConfig(wxCommandEvent& event)
{
	wxString			title="Bad Words List Editor";

	TextListDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						(char *)"BannedSpeech");

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnHelp(wxCommandEvent& event)
{
	m_advancedHtmlHelp.DisplayContents();
}

void wxWidgetsFrame::OnCustomMessagesConfig(wxCommandEvent& event)
{
	wxString			title="Custom Message List";

	TextListDialog	gui(
							this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"",
						(char *)"CustomMessages");
	gui.SortOn(false);
	gui.SingleWords(false);

	gui.DisplayDialog();
}

void wxWidgetsFrame::CopyGroupToFile(const char *groupID,FILE *fp)
{
	wxArrayString	keys;
	wxArrayString	values;
	int		keyCount;
	int		keyIndex;
	wxString	key;
	wxString	value;
	wxString	groupIDStr;

	groupIDStr=groupID;
	globalStatistics.configData.ReadGroup(groupIDStr,keys,values);
	keyCount=keys.GetCount();
	fprintf(fp,"[%s]\n",groupID);
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keys.Item(keyIndex);
		value=values.Item(keyIndex);
		fprintf(fp,"%s=%s\n",STRING_TO_CHAR(key),STRING_TO_CHAR(value));
	}
}
void wxWidgetsFrame::OnExportBuildINI(wxCommandEvent& event)
{
	wxString	buildINIFileNameStr;
	wxFileName	buildINIFileName;
	wxString	imagepackINIFileNameStr;
	wxFileName	imagepackINIFileName;
	FILE		*fp;
	wxArrayString	awardIDs;
	int		awardCount;
	wxString	awardID;
	int		awardIndex;
	wxArrayString	weaponGroups;
	wxString	weaponGroup;
	int		weaponGroupCount;
	int		weaponGroupIndex;

	buildINIFileName=BaseDirectoryFileName((char *)"build.ini");
	buildINIFileNameStr=buildINIFileName.GetFullPath();
	fp=fopen(STRING_TO_CHAR(buildINIFileNameStr),"w");
	if (fp==NULL)
	{
		return;
	}

	// General
	fprintf(fp,"[General]\n");
	fprintf(fp,"BuildVersion=%d\n",CONFIG_VERSION);
	fprintf(fp,"DatabaseVersion=%d\n",DATABASE_VERSION);
	fprintf(fp,"TemplateVersion=%d\n",TEMPLATE_VERSION);
	fprintf(fp,"AllowNegativeSkill=n\n");
	fprintf(fp,"FirstTimeRun=y\n");

	CopyGroupToFile((char *)"Colours",fp);
	CopyGroupToFile((char *)"DEATHMATCH",fp);
	CopyGroupToFile((char *)"TEAMSWAP",fp);
	CopyGroupToFile((char *)"LINEIDMOHAA",fp);
	CopyGroupToFile((char *)"LINEIDWOLFENSTEIN",fp);
	CopyGroupToFile((char *)"AutoAliasBlockedGUIDS",fp);

	CopyGroupToFile((char *)"ScoreWeights",fp);
	CopyGroupToFile((char *)"RealNames",fp);
	CopyGroupToFile((char *)"Images",fp);

	CopyGroupToFile((char *)"LogEntriesACTION",fp);
	CopyGroupToFile((char *)"LogEntriesWEAPON",fp);
	CopyGroupToFile((char *)"LogEntriesLOCATION",fp);

	CopyGroupToFile((char *)"MessageCentreFormats",fp);

	CopyGroupToFile((char *)"LogSubstituteACTION",fp);
	CopyGroupToFile((char *)"LogSubstituteAMMO",fp);
	CopyGroupToFile((char *)"LogSubstituteGAMETYPE",fp);
	CopyGroupToFile((char *)"LogSubstituteMAP",fp);
	CopyGroupToFile((char *)"LogSubstituteCLASS",fp);
	CopyGroupToFile((char *)"LogSubstituteTEAM",fp);
	CopyGroupToFile((char *)"LogSubstituteWEAPON",fp);
	CopyGroupToFile((char *)"LogSubstituteWEAPONAMMO",fp);
	CopyGroupToFile((char *)"LogSubstituteLOCATION",fp);


	CopyGroupToFile((char *)"AWARDLIST",fp);
	globalStatistics.configData.ReadList((char *)"AWARDLIST",awardIDs);
	awardCount=awardIDs.GetCount();
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		awardID.Printf("AWARDDEFINITION%s",STRING_TO_CHAR(awardIDs.Item(awardIndex)));
		CopyGroupToFile(STRING_TO_CHAR(awardID),fp);
	}



	// Weapon Groups
	weaponGroups=globalStatistics.configData.ReadWeaponGroups();
	weaponGroupCount=weaponGroups.GetCount();
	for (weaponGroupIndex=0;weaponGroupIndex<weaponGroupCount;weaponGroupIndex++)
	{
		weaponGroup=weaponGroups.Item(weaponGroupIndex);
		CopyGroupToFile(STRING_TO_CHAR(weaponGroup),fp);
	}
	fclose(fp);

	CopyImagesToUpgradeFile((char *)"cod1images.ini",(char *)"cod1");
	CopyImagesToUpgradeFile((char *)"cod2images.ini",(char *)"cod2");
	CopyImagesToUpgradeFile((char *)"cod4images.ini",(char *)"cod4");
	CopyImagesToUpgradeFile((char *)"cod5images.ini",(char *)"cod5");
	CopyImagesToUpgradeFile((char *)"mohaaimages.ini",(char *)"mohaa");
	CopyImagesToUpgradeFile((char *)"wolfensteinimages.ini",(char *)"wolfenstein");
	CopyImagesToUpgradeFile((char *)"quakewarsimages.ini",(char *)"quakewars");

}
void wxWidgetsFrame::CopyImagesToUpgradeFile(const char *upgradefilename,const char *filter)
{
	wxFileName	upgradeFileName;
	wxString	upgradeFileNameStr;
	FILE		*fp;
	wxArrayString	keys;
	wxArrayString	values;
	int		keyCount;
	int		keyIndex;
	wxString	key;
	wxString	value;
	wxString	serverType;

	upgradeFileName=BaseDirectoryFileName(upgradefilename);
	upgradeFileNameStr=upgradeFileName.GetFullPath();
	fp=fopen(STRING_TO_CHAR(upgradeFileNameStr),"w");


	if (fp==NULL)
	{
		return;
	}

	fprintf(fp,(char *)"[UPGRADELIST]\n");
	fprintf(fp,(char *)"Description=Image Pack for %s images\n",filter);
	fprintf(fp,(char *)"UpgradeType=Configuration File\n");
	fprintf(fp,(char *)"LISTCOUNT=2\n");
	fprintf(fp,(char *)"LISTITEM001=Images\n");
	fprintf(fp,(char *)"LISTITEM002=RealNames\n");
	fprintf(fp,(char *)"[Images]\n");
	fprintf(fp,(char *)"UPGRADEDESCRIPTION=Image References\n");
	fprintf(fp,(char *)"UPGRADERECOMMENDATION=Update\n");
	fprintf(fp,(char *)"UPGRADEANSWERTYPE=0\n");

	globalStatistics.configData.ReadGroup((char *)"Images",keys,values);
	keyCount=keys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keys.Item(keyIndex);
		value=values.Item(keyIndex);
		serverType=key.AfterFirst('_');
		serverType=serverType.BeforeFirst('_');
		if (serverType.CmpNoCase(filter)==0)
		{
			fprintf(fp,"%s=%s\n",STRING_TO_CHAR(key),STRING_TO_CHAR(value));
		}
	}
	fprintf(fp,(char *)"[RealNames]\n");
	fprintf(fp,(char *)"UPGRADEDESCRIPTION=Real Names References\n");
	fprintf(fp,(char *)"UPGRADERECOMMENDATION=Update\n");
	fprintf(fp,(char *)"UPGRADEANSWERTYPE=0\n");
	globalStatistics.configData.ReadGroup((char *)"RealNames",keys,values);
	keyCount=keys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keys.Item(keyIndex);
		value=values.Item(keyIndex);
		serverType=key.AfterFirst('_');
		serverType=serverType.BeforeFirst('_');
		if (serverType.CmpNoCase(filter)==0)
		{
			fprintf(fp,"%s=%s\n",STRING_TO_CHAR(key),STRING_TO_CHAR(value));
		}
	}
	fclose(fp);
}

void wxWidgetsFrame::OnExportUpgrade(wxCommandEvent& event)
{
	wxString	upgradeFilename;
	wxString	cppFilename;
	wxString	message;
	wxString	prefix="XXX";

	message="Select Upgrade File";
	upgradeFilename=wxFileSelector(message);
	if (upgradeFilename.Length()>0)
	{
		globalStatistics.GenerateUpgrade(upgradeFilename);
	}
}

void wxWidgetsFrame::ManualUpgrade()
{
	wxString	title="Select The Upgrade File And Then Press Perform Upgrade Button";
	wxString	name="performupgrade";
	long		style= wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX;

	PerformUpgradeDialog	dialog(this,
									wxID_ANY,
									title,
									wxDefaultPosition,
									wxDefaultSize,
									style,
									name);
	globalStatistics.statsgenDatabase.OpenDB();

	dialog.DisplayDialog();
}

void wxWidgetsFrame::Upgrade()
{
	UpgradeConfig();
	UpgradeTemplate();
	UpgradeDatabase();
}

void wxWidgetsFrame::UpgradeDatabase()
{
	wxString	configKey;
	wxString	configValue;
	int			buildVersion;
	wxString	message;

	configKey="/General/DatabaseVersion";
	globalStatistics.configData.ReadTextValue(configKey,&configValue);
	buildVersion=atoi(STRING_TO_CHAR(configValue));
	configValue.Printf("%d",DATABASE_VERSION);
	globalStatistics.configData.WriteTextValue(configKey,configValue);
	if (buildVersion<DATABASE_VERSION)
	{
		switch (buildVersion)
		{
			case 0:	// Statsgen 1.60 or less (not persistant db)
				message="Database requires upgrading, this involves deleting it's contents and changing it to disk based if necessary - statsgen will do this for you now";
				wxMessageBox(message);
				globalStatistics.statsgenDatabase.CloseDB();
				if (globalStatistics.statsgenDatabase.Filename().StartsWith(":"))
				{
					wxFileName	dbFilename;
					wxString	defaultDBFilename;

					dbFilename=BaseDirectoryFileName("statsgen2.db");
					defaultDBFilename=dbFilename.GetFullPath();
					configKey="/Database/filename";
					globalStatistics.configData.WriteTextValue(configKey,defaultDBFilename);
					
				}
				else
				{
					wxRemoveFile(globalStatistics.statsgenDatabase.Filename());
				}
				globalStatistics.statsgenDatabase.OpenDB();
				break;
			case 1:
				message="As many of the awards and default settings have changed it is strongly "
							"recommended that you remove the old database to start stats again.\n"
							"Failure to do this will give you odd (and inaccurate) results for awards"
							"\n"
							"Do you want to do this now - highly recommended";
				if (wxMessageBox(message,"Confirm?",wxYES_NO)==wxYES)
				{
					globalStatistics.statsgenDatabase.CloseDB();
					wxRemoveFile(globalStatistics.statsgenDatabase.Filename());
					globalStatistics.statsgenDatabase.OpenDB();
				}
				break;
		}

	}
	globalStatistics.configData.CommitChanges();
}

void wxWidgetsFrame::UpgradeTemplate()
{
	wxString	configKey;
	wxString	configValue;
	int			buildVersion;

	configKey="/General/TemplateVersion";
	globalStatistics.configData.ReadTextValue(configKey,&configValue);
	buildVersion=atoi(STRING_TO_CHAR(configValue));
	configValue.Printf("%d",TEMPLATE_VERSION);
	globalStatistics.configData.WriteTextValue(configKey,configValue);
	globalStatistics.configData.CommitChanges();
	if (buildVersion<TEMPLATE_VERSION)
	{
		int			answer;
		wxString	message="Old version of template Detected - Do you want to upgrade it? - this will overwrite you current template";
		wxString	caption="Upgrade";

		answer=wxMessageBox(message,caption,wxYES_NO);
		if (answer==wxYES)
		{
			wxString	templateDirectory;
			configKey="/Template/FullRun";
			globalStatistics.configData.ReadTextValue(configKey,&configValue);
			wxFileName	templateFilename(configValue);
			templateDirectory=templateFilename.GetPath();
			ExportTemplateFull(templateDirectory);
		}
	}
}

void wxWidgetsFrame::UpgradeConfig()
{
	wxString	configKey;
	wxString	configValue;
	int			buildVersion;

	configKey="/General/BuildVersion";
	globalStatistics.configData.ReadTextValue(configKey,&configValue);
	buildVersion=atoi(STRING_TO_CHAR(configValue));
	configValue.Printf("%d",CONFIG_VERSION);
	globalStatistics.configData.WriteTextValue(configKey,configValue);
	globalStatistics.configData.CommitChanges();
	if (buildVersion<CONFIG_VERSION)
	{
		int			answer;
		wxString	message="Old version of configuration Detected - Do you want to upgrade it?";
		wxString	caption="Upgrade";

		answer=wxMessageBox(message,caption,wxYES_NO);
		if (answer==wxYES)
		{
			ManualUpgrade();
		}
	}

}

void wxWidgetsFrame::OnExternalDatabaseConfig(wxCommandEvent& event)
{
	wxString				title="External Database Config";
	wxString				defaultValue;
	GroupedConfigItemsPanel	*databaseConfig;
	wxString				configKey;
	wxString				configGroup="ExternalDatabase";
	GUITriggerList			triggerList;
	GUITriggerItem			triggerItem;

	GenericConfigGUI	gui(this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));

	GenericConfigPanel	*configPanel;

	configPanel=new GenericConfigPanel(&gui,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panel holds the various boxed configs
	databaseConfig=new GroupedConfigItemsPanel((char *)"External Database Config");
	databaseConfig->CreateDisplay(configPanel,wxID_ANY);

	configKey="/ExternalDatabase/Enabled";
	databaseConfig->AddBoolean((char *)"Enabled",configKey,false);

	triggerItem.SetPositiveDisableTrigger(configKey,(char *)"N");
	triggerList.Add(triggerItem);

	configKey="/ExternalDatabase/CreateIndexes";
	databaseConfig->AddBoolean((char *)"Create Table Indexes",configKey,true,&triggerList);

	configKey="/ExternalDatabase/Hostname";
	databaseConfig->Add((char *)"Hostname",configKey,(char *)"",-1,&triggerList);

	configKey="/ExternalDatabase/Username";
	databaseConfig->Add((char *)"Username",configKey,(char *)"",-1,&triggerList);

	configKey="/ExternalDatabase/Password";
	databaseConfig->Add((char *)"Password",configKey,(char *)"",-1,&triggerList);

	configKey="/ExternalDatabase/DatabaseName";
	databaseConfig->Add((char *)"Database Name",configKey,(char *)"statsgen",-1,&triggerList);

	configKey="/ExternalDatabase/Port";
	databaseConfig->Add((char *)"Port",configKey,(char *)"3306",5,&triggerList);

	configPanel->AddConfigGroup(databaseConfig);

	gui.SetConfigPanel(configPanel);
	gui.DisplayDialog();
}

void wxWidgetsFrame::OnConfigThread(wxCommandEvent& event)
{
	wxString				title="Change Running Priorities, 0 = Low, 100 = High";
	wxSize					dialogSize(400,150);
	wxString				name="threadconfig";
	long					style= wxCAPTION |
									wxCLOSE_BOX |
									wxSYSTEM_MENU |
									wxRESIZE_BORDER |
									wxMAXIMIZE_BOX;
	GenericOKCancelDialog	dialog(this,
									wxID_ANY,
									title,
									wxDefaultPosition,
									dialogSize,
									style,
									name);
	PriorityPanel			*priorityPanel=new PriorityPanel();
	priorityPanel->Create(&dialog,
							wxID_ANY,
							wxDefaultPosition,
							wxDefaultSize,
							wxTAB_TRAVERSAL,
							(char *)"PANEL");

	dialog.DisplayDialog(priorityPanel);
	globalStatistics.configData.CommitChanges();
																	
}

WorkerThread::WorkerThread(wxThreadKind threadKind):wxThread(threadKind)
{
}

void * WorkerThread::Entry()
{
	globalStatistics.currentRunningThread=this;
	globalStatistics.ProduceStatsProcess();
	return NULL;
}

void WorkerThread::SetRunType(bool runTypeIn,wxArrayInt &allowedStepsIn,TemplateFile *templateFileIn)
{
	runType=runTypeIn;
	allowedSteps=allowedStepsIn;
	templateFile=templateFileIn;
}

void WorkerThread::Exit(void *exitCode)
{
	globalStatistics.currentRunningThread=NULL;
}
void wxWidgetsFrame::OnProgressUpdate(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("wxWidgetsFrame","OnProgressUpdate")
	wxString	eventString=event.GetString();

	wxSafeYield();
	if (progress->GetEventUpdating())
	{
		switch (event.GetId())
		{
			case EVENT_ID_PROGRESS_STATUS_TEXT:
				wxStaticText	*label;
				label=(wxStaticText *)event.GetClientData();
				if (label!=NULL)
				{
					label->SetLabel(eventString);
					progress->UpdateColouredBar();
				}
				break;
			case EVENT_ID_PROGRESS_BUTTON:
				wxBitmapButton	*button;
				wxBitmap		bitmap=ProgressPanel::SeverityImage(event.GetInt());
				button=(wxBitmapButton *)event.GetClientData();
				if (button!=NULL)
				{
					button->SetBitmapLabel(bitmap);
				}
				break;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void wxWidgetsFrame::OnWebServerConfig(wxCommandEvent& event)
{
	// This was meant to be so you could remote control statsgen
	// but it was never implemented
	wxString				title="Internal Web Server Config";
	wxString				defaultValue;
	GroupedConfigItemsPanel	*webserverConfig;
	wxString				configKey;
	GUITriggerList			triggerList;
	GUITriggerItem			triggerItem;

	GenericConfigGUI	gui(this,
							wxID_ANY,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							(char *)"");

	GenericConfigPanel	*configPanel;

	configPanel=new GenericConfigPanel(&gui,
									wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									(char *)"PANEL");

	// Panel holds the various boxed configs
	webserverConfig=new GroupedConfigItemsPanel((char *)"Internal Web Server Config");
	webserverConfig->CreateDisplay(configPanel,wxID_ANY);

	configKey=globalStatistics.webServer.ConfigKeyEnabled();
	webserverConfig->AddBoolean((char *)"Enabled",configKey,false);

	triggerItem.SetPositiveDisableTrigger(configKey,(char *)"N");
	triggerList.Add(triggerItem);

	configKey=globalStatistics.webServer.ConfigKeyPort();
	webserverConfig->Add((char *)"Listen Port",configKey,(char *)"9000",-1,&triggerList);

	configKey=globalStatistics.webServer.ConfigKeyAdminUser();
	webserverConfig->Add((char *)"Admin Username",configKey,(char *)"",-1,&triggerList);

	configKey=globalStatistics.webServer.ConfigKeyAdminPassword();
	webserverConfig->Add((char *)"Admin Password",configKey,(char *)"",-1,&triggerList);

	configPanel->AddConfigGroup(webserverConfig);

	gui.SetConfigPanel(configPanel);
	gui.DisplayDialog();

//	globalStatistics.webServer.StartServer();
}
/*
void wxWidgetsFrame::OnTimerTickOrg(wxTimerEvent& event)
{
	wxStatusBar		*statusBar;
	bool			needsToRun;
	long			timeLeft;
	long			hours;
	long			minutes;
	long			seconds;
	wxString		statusText;
	ScheduleItem	*schedule;
	wxString		schedulePrefix;
	int				scheduleIndex;
	int				fieldIndex;
	wxString		configKey;
	wxString		override;
	wxArrayInt		allowedStepsDownload;
	wxArrayInt		allowedStepsProcess;
	wxArrayInt		allowedStepsTransferDatabase;
	wxArrayInt		allowedStepsTemplate;
	wxArrayInt		allowedStepsWebsite;
	bool			quickTemplateEnabled;


	quickTemplateEnabled=false;
	allowedStepsDownload.Add(WINDOW_ID_PROGRESS_PANEL_DOWNLOAD_LOGS);

	allowedStepsProcess.Add(WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS);
	allowedStepsProcess.Add(WINDOW_ID_PROGRESS_PANEL_DROPPING_PLAYERS);
	allowedStepsProcess.Add(WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES1);
	allowedStepsProcess.Add(WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES2);
	allowedStepsProcess.Add(WINDOW_ID_PROGRESS_PANEL_CREATE_DATABASE);
	allowedStepsProcess.Add(WINDOW_ID_PROGRESS_PANEL_EMPTY_DATABASE);
	allowedStepsProcess.Add(WINDOW_ID_PROGRESS_PANEL_WRITE_DATABASE);

	allowedStepsTransferDatabase.Add(WINDOW_ID_PROGRESS_PANEL_TRANSFER_DATABASE);

	allowedStepsTemplate.Add(WINDOW_ID_PROGRESS_PANEL_READ_TEMPLATE);
	allowedStepsTemplate.Add(WINDOW_ID_PROGRESS_PANEL_PROCESS_TEMPLATE);

	allowedStepsWebsite.Add(WINDOW_ID_PROGRESS_PANEL_WEBSITE);

	scheduleTimer.Stop();
	statusBar=GetStatusBar();

	if (statusBar!=NULL)
	{
		for (scheduleIndex=0;scheduleIndex<3;scheduleIndex++)
		{
			override="y";
			switch (scheduleIndex)
			{
				case 0:
					schedule=&globalStatistics.messageTimer;
					schedulePrefix="Message";
					configKey="/RCONSettings/Enabled";
					globalStatistics.configData.ReadTextValue(configKey,&override,"n");
					break;
				case 1:
					schedule=&globalStatistics.quickRunTimer;
					schedulePrefix="Quick";
					break;
				case 2:
					schedule=&globalStatistics.productionRunTimer;
					schedulePrefix="Full";
					break;
			}
			fieldIndex=scheduleIndex+1;
			needsToRun=(schedule->TimeToRun(&timeLeft)&&(override.CmpNoCase("y")==0));
			if (needsToRun)
			{
				statusText.Printf("%s: Running",schedulePrefix.GetData());
			}
			else
			{
				if ((schedule->disabled)||(override.CmpNoCase("n")==0))
				{
					statusText.Printf("%s: Disabled",
								schedulePrefix.GetData());
					if (scheduleIndex==1)
					{
						quickTemplateEnabled=true;
					}
				}
				else
				{
					hours=timeLeft/3600;
					minutes=timeLeft-(hours*3600);
					seconds=minutes % 60;
					minutes/=60;
					statusText.Printf("%s: %02ld:%02ld:%02ld",
								schedulePrefix.GetData(),
								hours,minutes,seconds);
				}
			}
			SetStatusText(statusText,fieldIndex);
			if (needsToRun)
			{
				TemplateFile	templateFile;

				switch (scheduleIndex)
				{
					case 0:
						globalStatistics.ReadServersFromConfig();
						globalStatistics.SendMessages();
						globalStatistics.ReadMessageTimerConfig();
						break;
					case 1:
						globalStatistics.ReadServersFromConfig();
						globalStatistics.ProduceStats(false,allowedStepsDownload,templateFile);
						workerThread=new WorkerThread(wxTHREAD_JOINABLE);
						if (workerThread!=NULL)
						{
							workerThread->SetRunType(false,allowedStepsProcess,&templateFile);
							workerThread->Create();
							workerThread->SetPriority(globalStatistics.ReadPriority(false));
							workerThread->Run();
							workerThread->Wait();
							delete (workerThread);
						}
						workerThread=NULL;
						globalStatistics.ProduceStats(false,allowedStepsTransferDatabase,templateFile);

						workerThread=new WorkerThread(wxTHREAD_JOINABLE);
						if (workerThread!=NULL)
						{
							workerThread->SetRunType(false,allowedStepsTemplate,&templateFile);
							workerThread->Create();
							workerThread->SetPriority(globalStatistics.ReadPriority(false));
							workerThread->Run();
							workerThread->Wait();
							delete (workerThread);
						}
						workerThread=NULL;
						globalStatistics.ProduceStats(false,allowedStepsWebsite,templateFile);

						globalStatistics.ReadQuickRunTimerConfig();
						break;

					case 2:
						globalStatistics.ClearServers();
						globalStatistics.ReadServersFromConfig();
						globalStatistics.ProduceStats(true,allowedStepsDownload,templateFile);
						workerThread=new WorkerThread(wxTHREAD_JOINABLE);
						if (workerThread!=NULL)
						{
							workerThread->SetRunType(true,allowedStepsProcess,&templateFile);
							workerThread->Create();
							workerThread->SetPriority(globalStatistics.ReadPriority(true));
							workerThread->Run();
							workerThread->Wait();
							delete (workerThread);
						}
						workerThread=NULL;
						globalStatistics.ProduceStats(true,allowedStepsTransferDatabase,templateFile);

						workerThread=new WorkerThread(wxTHREAD_JOINABLE);
						if (workerThread!=NULL)
						{
							workerThread->SetRunType(true,allowedStepsTemplate,&templateFile);
							workerThread->Create();
							workerThread->SetPriority(globalStatistics.ReadPriority(true));
							workerThread->Run();
							workerThread->Wait();
							delete (workerThread);
						}
						workerThread=NULL;
						globalStatistics.ProduceStats(true,allowedStepsWebsite,templateFile);
						if (quitNow)
						{
							progress->DisableUpdate();
							Close(TRUE);
						}
						globalStatistics.ReadProductionRunTimerConfig();
						globalStatistics.ReadQuickRunTimerConfig();
						if (!quickTemplateEnabled)
						{
							// If quick template not enabled we can release memory
							globalStatistics.ResetProcessingStatus();
						}

						break;
				}
			}
		}
	}
	scheduleTimer.Start(1000);
}
*/

void wxWidgetsFrame::OnResetRecovery(wxCommandEvent& event)
{
	progress->ClearErrors();
}

void wxWidgetsFrame::OnStoreProgress(wxCommandEvent& event)
{
	wxString		filename;
	wxString		message="Where do you want to store the errors";

	filename=wxFileSelector(message);
	if (filename.Length()>0)
	{
		progress->StoreProgress(filename);
	}
}

void wxWidgetsFrame::OnEditWeaponGroups(wxCommandEvent& event)
{
	wxString	name="";
	wxString	title="Edit Weapon Groups";
	WeaponGroupDialog	gui(this,
						wxID_ANY,
						title,
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						name);

	gui.DisplayDialog();
}

