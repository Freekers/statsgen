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
	workerThread=false;
	globalStatistics.mainEventHandler=this;

	wxMenu *helpMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *fileMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *displayMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *calculationMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *mandatoryMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *optionalMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *rconMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *logSubstituteMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *devMenu = new wxMenu(_(""), wxMENU_TEAROFF);
	wxMenu *playerData = new wxMenu(_(""), wxMENU_TEAROFF);

	int argIndex;

	globalStatistics.configData.Initiate();
	STATSGEN_DEBUG_INITIATE
	STATSGEN_DEBUG_UPDATE_DEBUG_LEVEL(WINDOW_ID_PROGRESS_PANEL_GENERAL)
	globalStatistics.configData.CommitChanges();

	helpMenu->Append(WINDOW_ID_MENU_HELP, _("&Help"), _("Help"));
	helpMenu->Append(WINDOW_ID_MENU_ABOUT, _("About"), _("Version"));
	fileMenu->AppendCheckItem(WINDOW_ID_MENU_ADVANCED, _("Advanced Menus"), _("Advanced"));
	fileMenu->Append(WINDOW_ID_WIZARD, _("First Time Configuration Run"), _("Run this the first time you start statsgen"));
	fileMenu->Append(WINDOW_ID_MENU_GO, _("Go\tf1"), _("Go"));
	fileMenu->Append(WINDOW_ID_MENU_EMPTY_DATABASE, _("Empty Database"), _("Empty Database"));
	fileMenu->Append(WINDOW_ID_MENU_THREAD, _("Priority\tf2"), _("Change Priority"));
	fileMenu->Append(WINDOW_ID_MENU_PERFORM_UPGRADE, _("Upgrade Configuration"), _("Use an Upgrade file to update configuration"));
	fileMenu->Append(WINDOW_ID_MENU_RESET_RECOVERY, _("Clear Recovery Status"), _("Clear Recovery Status"));
	fileMenu->Append(WINDOW_ID_MENU_STORE_PROGRESS, _("Write Errors To File"), _("Write Errors To File"));
	fileMenu->Append(WINDOW_ID_MENU_QUIT, _("E&xit\tAlt-X"), _("Quit this program"));

	displayMenu->Append(WINDOW_ID_MENU_WEAPON_GROUPS, _("Weapon Groups"), _("Edit Weapon Groupings"));
	displayMenu->Append(WINDOW_ID_MENU_REAL_NAMES, _("Real Names"), _("Edit Real Names"));
	displayMenu->Append(WINDOW_ID_MENU_IMAGES, _("Images"), _("Edit Image References"));

	playerData->Append(WINDOW_ID_MENU_PLAYERDATAAVATAR, _("Avatar Editing"), _("Edit Player Avatar Images"));
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAREALNAME, _("Real Name Editing"), _("Edit Player Real Names"));
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAPICTURE, _("Personal Picture"), _("Edit Player Personal Pictures"));
	playerData->Append(WINDOW_ID_MENU_PLAYERDATACLANROLE, _("Clan Role"), _("Edit Player's Clan Role"));
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAXFIRE, _("Xfire"), _("Edit Player's Xfire ID"));
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAWEBSITE, _("Website"), _("Edit Player's Personal Website"));
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAMISC1, _("Miscellaneous 1"), _("Edit Player's Miscellaneous Data 1"));
	playerData->Append(WINDOW_ID_MENU_PLAYERDATAMISC2, _("Miscellaneous 2"), _("Edit Player's Miscellaneous Data 2"));

	calculationMenu->Append(WINDOW_ID_MENU_SCORE_WEIGHTS, _("Score Weights"), _("Edit Score Weights"));
	calculationMenu->Append(WINDOW_ID_MENU_SKILL_WEIGHTS, _("Skill Weights"), _("Edit Skill Weights"));
	calculationMenu->Append(WINDOW_ID_MENU_AWARD, _("Award Editing"), _("Edit Award Configuration"));

	mandatoryMenu->Append(WINDOW_ID_MENU_TEMPLATE_CONFIG, _("Template Variable Config"), _("Change variables used by the template during output"));
	mandatoryMenu->Append(WINDOW_ID_MENU_OUTPUT, _("Output Config"), _("Edit How And Where Output Is Created"));
	mandatoryMenu->Append(WINDOW_ID_MENU_SERVER, _("Server Config"), _("Edit Server Connection Details"));
	mandatoryMenu->Append(WINDOW_ID_MENU_WEBSITE, _("Website Config"), _("Edit Website Connection Details"));
	mandatoryMenu->Append(WINDOW_ID_MENU_IMAGEPACKS, _("Upload Image Packs"), _("Upload appropriate image packs"));
	mandatoryMenu->Append(WINDOW_ID_MENU_TEMPLATEPACKS, _("Upload Template Packs"), _("Upload Alternative Templates"));

	optionalMenu->Append(WINDOW_ID_MENU_CLAN, _("Clan Config"), _("Edit Clan Details"));
	optionalMenu->Append(WINDOW_ID_MENU_ALIAS, _("Alias Editing"), _("Edit Player Alias Entries"));
	optionalMenu->Append(WINDOW_ID_MENU_DROPLIST, _("Drop List Editing"), _("Edit List Of Allowed Players"));
	optionalMenu->Append(WINDOW_ID_MENU_BAD_WORDS, _("Banned Speech Editing"), _("Edit List Of Words Not Allowed In Speech"));
	optionalMenu->Append(WINDOW_ID_MENU_WEB_SERVER, _("Web Server Setting"), _("Configure Embedded Webserver"));
	optionalMenu->Append(WINDOW_ID_MENU_EXTERNAL_DATABASE, _("External Database Config"), _("Configure MySQL connection"));
	rconMenu->Append(WINDOW_ID_MENU_RCON_CONFIG, _("Message Centre"), _("Configure Game Server Messages"));
	rconMenu->Append(WINDOW_ID_MENU_MESSAGE_FORMATS, _("Message Centre Formats"), _("Edit Message Centre Formats"));
	rconMenu->Append(WINDOW_ID_MENU_CUSTOM_MESSAGES, _("Custom Messages"), _("Edit Custom Messages"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_GAMETYPE, _("Game Type"), _("Log File Substitution For Game Type"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_MAP, _("Map"), _("Log File Substitution For Map"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_CLASS, _("Class"), _("Log File Substitution For Class"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_TEAM, _("Team"), _("Log File Substitution For Team"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPON, _("Weapon"), _("Log File Substitution For Weapon"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_AMMO, _("Ammo"), _("Log File Substitution For Ammo"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPONAMMO, _("Weapon And Ammo"), _("Log File Substitution For Weapon And Ammo"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_LOCATION, _("Location"), _("Log File Substitution For Location"));
	logSubstituteMenu->Append(WINDOW_ID_MENU_LOG_SUBSTITUTE_ACTION, _("Action"), _("Log File Substitution For Action"));

	devMenu->Append(WINDOW_ID_MENU_DEBUG, _("Debug Settings"), _("Developer Debug Settings"));
	devMenu->Append(WINDOW_ID_MENU_EXPORT_TEMPLATE, _("Export Template"), _("Export Template To C++"));
	devMenu->Append(WINDOW_ID_MENU_EXPORT_UPGRADE, _("Export Upgrade File"), _("Prepare An Upgrade File"));
	devMenu->Append(WINDOW_ID_MENU_EXPORT_BUILD_INI, _("Export Build INI File"), _("The Build INI File"));

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("&Run"));
	menuBar->Append(calculationMenu, _("&Calculation"));
	menuBar->Append(displayMenu, _("&Display"));
	menuBar->Append(mandatoryMenu, _("&Mandatory"));
	menuBar->Append(optionalMenu, _("&Optional"));
	menuBar->Append(playerData, _("&Player Data"));
	menuBar->Append(rconMenu, _("&Message Centre"));
	menuBar->Append(logSubstituteMenu, _("&Logfile Substitutions"));
	menuBar->Append(devMenu, _("&Developer"));
	menuBar->Append(helpMenu, _("&Help"));

	SetMenuBar(menuBar);
	CreateStatusBar(3);
	progress=new StatusPanel();
	progress->Create(this,-1);
//	progress->CreatePanels();
	SetStatusText(_("Status Text"));
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

//	globalStatistics.webServer.StartServer();
}

void wxWidgetsFrame::OnFullConfig(wxCommandEvent &event)
{
	GUIWizardPagesFullConfig	wizardPages;
	RunWizard(&wizardPages);
}

void wxWidgetsFrame::SetMenuItems()
{
	wxString		configKey;
	wxString		configValue;
	wxMenuBar		*menuBar;
	configKey="/General/advancedmenus";
	bool			firstTimeRun;

	runningWizard=false;
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"n");
	globalStatistics.configData.CommitChanges();
	advanced=(configValue.CmpNoCase("Y")==0);
	menuBar = GetMenuBar();

	firstTimeRun=NeedsFirstTimeRun();
	menuBar->Enable(WINDOW_ID_WIZARD, firstTimeRun || advanced);

	menuBar->Enable(WINDOW_ID_MENU_ADVANCED, true && (!firstTimeRun));
	menuBar->Check(WINDOW_ID_MENU_ADVANCED, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_HELP, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_ABOUT, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_GO, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_EMPTY_DATABASE, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_THREAD, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PERFORM_UPGRADE, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_RESET_RECOVERY, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_STORE_PROGRESS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_QUIT, true && (!firstTimeRun));

	menuBar->Enable(WINDOW_ID_MENU_WEAPON_GROUPS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_REAL_NAMES, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_IMAGES, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_FULL_CONFIG, true && (!firstTimeRun));

	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAAVATAR, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAREALNAME, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAPICTURE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATACLANROLE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAXFIRE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAWEBSITE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAMISC1, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_PLAYERDATAMISC2, advanced && (!firstTimeRun));

	menuBar->Enable(WINDOW_ID_MENU_SCORE_WEIGHTS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_SKILL_WEIGHTS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_AWARD, advanced && (!firstTimeRun));

	menuBar->Enable(WINDOW_ID_MENU_TEMPLATE_CONFIG, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_OUTPUT, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_SERVER, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_WEBSITE, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_IMAGEPACKS, true && (!firstTimeRun));

	menuBar->Enable(WINDOW_ID_MENU_CLAN, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_ALIAS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_DROPLIST, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_BAD_WORDS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_WEB_SERVER, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_EXTERNAL_DATABASE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_RCON_CONFIG, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_MESSAGE_FORMATS, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_CUSTOM_MESSAGES, true && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_GAMETYPE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_MAP, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_CLASS, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_TEAM, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPON, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_AMMO, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPONAMMO, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_LOCATION, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_LOG_SUBSTITUTE_ACTION, advanced && (!firstTimeRun));

	menuBar->Enable(WINDOW_ID_MENU_DEBUG, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_EXPORT_TEMPLATE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_EXPORT_UPGRADE, advanced && (!firstTimeRun));
	menuBar->Enable(WINDOW_ID_MENU_EXPORT_BUILD_INI, advanced && (!firstTimeRun));
}

void wxWidgetsFrame::OnAdvanced(wxCommandEvent &event)
{
	wxString		configKey;
	wxString		configValue;
	configKey="/General/advancedmenus";

	globalStatistics.configData.ReadTextValue(configKey,&configValue,"n");
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
					globalStatistics.configData.ReadTextValue(configKey,&override,"n");
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
				statusText.Printf("%s: Running",schedulePrefix.GetData());
			}
			else
			{
				if ((schedule->disabled)||(override.CmpNoCase("n")==0))
				{
					statusText.Printf("%s: Disabled",
								schedulePrefix.GetData());
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
						wxBusyCursor	cursor;
						globalStatistics.ProduceStatsInitiate();
						globalStatistics.ProduceStatsDownload();
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
						_T("Edit Score Weights"),
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						_T(""),
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

	GroupedConfigItemsPanel	*configPanel=new GroupedConfigItemsPanel("Negative Skill");

	ConfigEditorGUI	gui(this,
						-1,
						_T("Edit Skill Weights"),
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						_T(""),
						(BoxedDropDown *)skillType,
						(BoxedDropDown *)serverType,
						configPanel,
						false);

	configPanel->Create(&gui,
						-1,
						wxDefaultPosition,
						wxDefaultSize);
	configKey=globalStatistics.SkillCalculationTypeConfigKey();
	configPanel->AddBoolean("Allow Negative Skill",configKey,true);
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
						_T("Edit Real Names"),
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						_T(""),
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
	wxString		group="Images";
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				keyIndex;
	int				keyCount;
	wxString		key;
	ServerTypePanel	*serverType=new ServerTypePanel();
	ImageTypePanel	*imageType=new ImageTypePanel();
	wxArrayString	weaponGroups;

	weaponGroups=globalStatistics.configData.ReadWeaponGroups();

	ConfigEditorGUI	gui(this,
						-1,
						_T("Edit Image References"),
						wxDefaultPosition,
						wxSize(480,600),
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						_T(""),
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

	gui.DisplayDialog();
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

		enabledKey.Printf("/%s/FTPEnabled",configGroup.GetData());
		triggerItem.SetPositiveDisableTrigger(enabledKey,"N");
		triggerList.Add(triggerItem);

		GenericConfigGUI	gui(this,
							-1,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));

		FTPConnectionPanel	*ftpSettings;
		GenericConfigPanel	*configPanel;

		configPanel=new GenericConfigPanel(&gui,
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

		// Panel holds the various boxed configs
		ftpSettings=new FTPConnectionPanel();
		ftpSettings->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

		ftpSettings->CreateConfigs((char *)configGroup.GetData());
		remoteDirectory=new GroupedConfigItemsPanel("Remote Directory");
		remoteDirectory->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

		configKey="/website/FTPRemoteDirectory";
		remoteDirectory->AddRemoteDirectory("Remote Directory",configKey,"",-1,configGroup,&triggerList);
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
	GenericConfigPanel				*configPanel;
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
	archiveDirectory.AppendDir("archivelogs");
	banDirectory.AppendDir("ban");
	logDirectory.AppendDir("logs");

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

	enabledKey.Printf("/%s/FTPEnabled",configGroup.GetData());
	serverTypeConfigKey.Printf("/%s/serverType",configGroup.GetData());

	
	triggerItem.SetNegativeHideTrigger(serverTypeConfigKey,"QUAKEWARS");
	serverTypeTriggerList.Add(triggerItem);
	serverTypeAndFtpTriggerList.Add(triggerItem);

	triggerItem.SetPositiveHideTrigger(enabledKey,"N");
	ftpTriggerList.Add(triggerItem);
	serverTypeAndFtpTriggerList.Add(triggerItem);

	

	configPanel=new GenericConfigPanel(parentID,
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panels holds the various boxed configs
	serverType		=new GroupedConfigItemsPanel("Server Type");
	serverType->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);
	serverType->AddSelection("Server Type",serverTypeConfigKey,"COD2",serverTypeCodes,serverTypeNames);

	mainLogFile		=new GroupedConfigItemsPanel("Main Log File");
	mainLogFile->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey.Printf("/%s/GuessTeamIDs",configGroup.GetData());
	mainLogFile->AddBoolean("Guess players team if none found",configKey,true);

	configKey.Printf("/%s/MaxLogfileSize",configGroup.GetData());
	mainLogFile->Add("Max Logfile Size in MB (0 for no maximum)",configKey,"0",-1);
	configKey.Printf("/%s/latestDirectory",configGroup.GetData());
	directoryGUI=mainLogFile->AddDirectory("Latest Active Logfile Directory",
											configKey,
											(char *)logDirectoryName.GetData(),-1);
	configKey.Printf("/%s/latestFilename",configGroup.GetData());
	mainLogFile->AddFile("Latest Active Logfile Name",configKey,"",-1,directoryGUI);

	configKey.Printf("/%s/FTPLatestDirectory",configGroup.GetData());
	remoteDirectoryGUI=mainLogFile->AddRemoteDirectory("Remote Active Logfile Directory",configKey,"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPLatestFilename",configGroup.GetData());
	mainLogFile->AddRemoteFile("Remote Active Logfile Name",configKey,"",-1,configGroup,remoteDirectoryGUI,&ftpTriggerList);

	// Quake Wars bollox :-(
	configKey.Printf("/%s/secondarylatestDirectory",configGroup.GetData());
	secondarydirectoryGUI=mainLogFile->AddDirectory("(Quake Wars Objective) Latest Active Logfile Directory",
											configKey,
											(char *)logDirectoryName.GetData(),-1,&serverTypeTriggerList);
	configKey.Printf("/%s/secondarylatestFilename",configGroup.GetData());
	mainLogFile->AddFile("(Quake Wars Objective) Latest Active Logfile Name",configKey,"",-1,secondarydirectoryGUI,&serverTypeTriggerList);

	configKey.Printf("/%s/FTPSecondaryLatestDirectory",configGroup.GetData());
	secondaryremoteDirectoryGUI=mainLogFile->AddRemoteDirectory("(Quake Wars Objective) Remote Active Logfile Directory",configKey,"",-1,configGroup,&serverTypeAndFtpTriggerList);
	configKey.Printf("/%s/FTPSecondaryLatestFilename",configGroup.GetData());
	mainLogFile->AddRemoteFile("(Quake Wars Objective) Remote Active Logfile Name",configKey,"",-1,configGroup,secondaryremoteDirectoryGUI,&serverTypeAndFtpTriggerList);

	
	archiveLogFile	=new GroupedConfigItemsPanel("Archive Log Files");
	archiveLogFile->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey.Printf("/%s/archiveDirectory",configGroup.GetData());
	archiveLogFile->AddDirectory("Archive Directory",
								configKey,
								(char *)archiveDirectoryName.GetData(),-1);
	configKey.Printf("/%s/archiveWildcard",configGroup.GetData());
	archiveLogFile->AddSelectionFreeForm("Archive Wildcard",configKey,"",wildCardCodes,wildCardNames,-1);
	configKey.Printf("/%s/FTPArchiveDirectory",configGroup.GetData());
	remoteDirectoryGUI=archiveLogFile->AddRemoteDirectory("Remote Archive Directory",configKey,"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPArchiveWildcard",configGroup.GetData());
	//archiveLogFile->Add("Remote Archive Wildcard",configKey,"",-1,&ftpTriggerList);
	archiveLogFile->AddSelectionFreeForm("Remote Archive Wildcard",configKey,"",wildCardCodes,wildCardNames,-1,&ftpTriggerList);

	// More quake war bollox
	configKey.Printf("/%s/secondaryarchiveDirectory",configGroup.GetData());
	archiveLogFile->AddDirectory("(Quake Wars Objective) Archive Directory",
								configKey,
								(char *)archiveDirectoryName.GetData(),-1,&serverTypeTriggerList);
	configKey.Printf("/%s/secondaryarchiveWildcard",configGroup.GetData());
	//archiveLogFile->Add("(Quake Wars Objective) Archive Wildcard",configKey,"",-1,&serverTypeTriggerList);
	archiveLogFile->AddSelectionFreeForm("(Quake Wars Objective) Archive Wildcard",configKey,"",wildCardCodes,wildCardNames,-1,&serverTypeTriggerList);
	configKey.Printf("/%s/FTPSecondaryArchiveDirectory",configGroup.GetData());
	secondaryremoteDirectoryGUI=archiveLogFile->AddRemoteDirectory("(Quake Wars Objective) Remote Archive Directory",configKey,"",-1,configGroup,&serverTypeAndFtpTriggerList);
	configKey.Printf("/%s/FTPSecondaryArchiveWildcard",configGroup.GetData());
	//archiveLogFile->Add("(Quake Wars Objective) Remote Archive Wildcard",configKey,"",-1,&serverTypeAndFtpTriggerList);
	archiveLogFile->AddSelectionFreeForm("(Quake Wars Objective) Remote Archive Wildcard",configKey,"",wildCardCodes,wildCardNames,-1,&serverTypeTriggerList);

	banFile1		=new GroupedConfigItemsPanel("Ban File 1");
	banFile1->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	banFile2		=new GroupedConfigItemsPanel("Ban File 2");
	banFile2->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	wxArrayString	banCodes;
	wxArrayString	banNames;
	wxString		banCode;
	wxString		banName;

	banCode="PUNKBUSTER";banName="Punk Buster";banCodes.Add(banCode);banNames.Add(banName);
	banCode="COD";banName="Call Of Duty";banCodes.Add(banCode);banNames.Add(banName);

	configKey.Printf("/%s/banFile1Type",configGroup.GetData());
	banFile1->AddSelection("Ban File Type",configKey,"PUNKBUSTER",banCodes,banNames);
	configKey.Printf("/%s/FTPBanFile1Directory",configGroup.GetData());
	remoteDirectoryGUI=banFile1->AddRemoteDirectory("Remote Ban 1 File Directory",configKey,"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPBanFile1Filename",configGroup.GetData());
	banFile1->AddRemoteFile("Remote Ban 1 File Filename",configKey,"",-1,configGroup,remoteDirectoryGUI,&ftpTriggerList);
	configKey.Printf("/%s/Ban1Directory",configGroup.GetData());
	directoryGUI=banFile1->AddDirectory("Local Ban Directory",
										configKey,
										(char *)banDirectoryName.GetData(),-1);

	configKey.Printf("/%s/banFile2Type",configGroup.GetData());
	banFile2->AddSelection("Ban File Type",configKey,"COD",banCodes,banNames);
	configKey.Printf("/%s/FTPBanFile2Directory",configGroup.GetData());
	remoteDirectoryGUI=banFile2->AddRemoteDirectory("Remote Ban 2 File Directory",configKey,"",-1,configGroup,&ftpTriggerList);
	configKey.Printf("/%s/FTPBanFile2Filename",configGroup.GetData());
	banFile2->AddRemoteFile("Remote Ban 2 File Filename",configKey,"",-1,configGroup,remoteDirectoryGUI,&ftpTriggerList);
	configKey.Printf("/%s/Ban2Directory",configGroup.GetData());
	directoryGUI=banFile2->AddDirectory("Local Ban Directory",
										configKey,
										(char *)banDirectoryName.GetData(),-1);

//	configKey.Printf("/%s/serverType",configGroup.GetData());
//	mainLogFile->Add("Server Type",configKey,"",-1);

	ftpSettings=new FTPConnectionPanel();
	ftpSettings->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	ftpSettings->CreateConfigs((char *)configGroup.GetData());
	configPanel->AddConfigGroup(serverType);
	configPanel->AddConfigGroup(ftpSettings);
	configPanel->AddConfigGroup(mainLogFile);
	configPanel->AddConfigGroup(archiveLogFile);
	configPanel->AddConfigGroup(banFile1);
	configPanel->AddConfigGroup(banFile2);

	return (configPanel);

}

void wxWidgetsFrame::OnServerConfig(wxCommandEvent& event)
{
	if (advanced)
	{
		wxString			title="Server Config";
		wxString			listGroup="SERVERS";
		wxString			listPrefix="Server";
		wxSize				dialogSize(800,600);

		GenericNumberedConfigGUI	gui(
							NewServerPanel,
							listGroup,
							listPrefix,
							this,
							-1,
							title,
							wxDefaultPosition,
							dialogSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));

		gui.DisplayDialog();
	}
	else
	{
		GUIWizardPagesServers	wizardPages;
		RunWizard(&wizardPages);
	}
	globalStatistics.ClearServers();
	globalStatistics.ReadServersFromConfig();
}

wxPanel *NewClanPanel(wxWindow *parentID,wxString &clanID)
{
	ClanEditorPanel		*configPanel;
	wxString			configGroup;

	configGroup="Clan"+clanID;
	configPanel=new ClanEditorPanel(configGroup,
									parentID,
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	return (configPanel);

}

void wxWidgetsFrame::OnClanConfig(wxCommandEvent& event)
{
	wxString			title="Clan Config";
	wxString			listGroup="CLANS";
	wxString			listPrefix="Clan";
	wxSize				guiSize(512,400);

	GenericNumberedConfigGUI	gui(
							NewClanPanel,
							listGroup,
							listPrefix,
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));

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
							-1,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));


	configPanel=new GenericConfigPanel(&gui,
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panel holds the various boxed configs
	serverSpecificConfigs=new GenericNumberedConfigPanel
							(
							NewRCONServerPanel,
							listGroup,
							listPrefix,
							configPanel,
							-1,
							wxDefaultPosition,
							wxDefaultSize,
							0,
							_T(""));
	serverSpecificConfigs->CreateDialog();
	
	commonConfigs=new GroupedConfigItemsPanel("Common RCON Settings");
	commonConfigs->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey="/RCONSettings/Enabled";
	commonConfigs->AddBoolean("Message Center Enabled",configKey,false);
	triggerItem.SetPositiveDisableTrigger(configKey,"N");
	triggerList.Add(triggerItem);

	configKey="/RCONSettings/MessageIntervalTimer";
	commonConfigs->Add("Message Interval In Seconds",configKey,"60",5,&triggerList);
	configKey="/RCONSettings/CustomMessageInterval";
	commonConfigs->Add("Custom Message Interval",configKey,"0",5,&triggerList);
	configKey="/RCONSettings/MessagePrefix";
	commonConfigs->Add("Message Prefix",configKey,"^7",-1,&triggerList);
	configKey="/RCONSettings/ClientPort";
	commonConfigs->Add("Local TCP/IP Port",configKey,"8000",5,&triggerList);

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
	usePunkbusterKey.Printf("/%s/RCONUsePunkBuster",configGroup.GetData());

	configKey="/RCONSettings/Enabled";
	triggerItem.SetPositiveDisableTrigger(configKey,"N");
	triggerList.Add(triggerItem);
	mainTriggerList.Add(triggerItem);
	pbTriggerList.Add(triggerItem);

	configKey.Printf("/%s/MessagingEnabled",configGroup.GetData());
	triggerItem.SetPositiveDisableTrigger(configKey,"N");
	triggerList.Add(triggerItem);
	pbTriggerList.Add(triggerItem);

	triggerItem.SetPositiveDisableTrigger(usePunkbusterKey,"N");
	pbTriggerList.Add(triggerItem);

	configPanel=new GenericConfigPanel(parentID,
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panels holds the various boxed configs
	punkBusterSettings=new GroupedConfigItemsPanel("PunkBuster Settings");
	punkBusterSettings->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	punkBusterSettings->AddBoolean("Use Punkbuster Scheduled Tasks",usePunkbusterKey,false,&triggerList);
	configKey.Printf("/%s/PunkBusterPostCommand",configGroup.GetData());
	punkBusterSettings->Add("Post Messaging PB Command",configKey,"",-1,&pbTriggerList);
	configKey.Printf("/%s/FTPBanFile1Directory",configGroup.GetData());
	punkBusterSettings->AddRemoteDirectory("Remote Ban 1 File Directory",configKey,"",-1,configGroup,&pbTriggerList);
	configKey.Printf("/%s/FTPBanFile2Directory",configGroup.GetData());
	punkBusterSettings->AddRemoteDirectory("Remote Ban 2 File Directory",configKey,"",-1,configGroup,&pbTriggerList);


	gameServer	=new GroupedConfigItemsPanel("Game Server Settings");
	gameServer->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);
	configKey.Printf("/%s/MessagingEnabled",configGroup.GetData());
	gameServer->AddBoolean("Messaging Enabled",configKey,false,&mainTriggerList);
	configKey.Printf("/%s/gameIP",configGroup.GetData());
	gameServer->Add("Game IP (if different from FTP Hostname)",configKey,"",-1,&triggerList);
	configKey.Printf("/%s/gamePort",configGroup.GetData());
	gameServer->Add("Game Port",configKey,"",5,&triggerList);
	configKey.Printf("/%s/RCONPassword",configGroup.GetData());
	gameServer->Add("RCON Password",configKey,"",-1,&triggerList);

	configPanel->AddConfigGroup(gameServer);
	configPanel->AddConfigGroup(punkBusterSettings);
	return (configPanel);

}

void wxWidgetsFrame::OnAliasConfig(wxCommandEvent& event)
{
	wxString			title="Alias Config";
	wxSize				guiSize(512,500);

	AliasDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));

	gui.DisplayDialog();
}

wxPanel *NewAwardPanel(wxWindow *parentID,wxString &awardID)
{
	AwardEditorPanel	*awardPanel;

	// Panels holds the various boxed configs
	awardPanel=new AwardEditorPanel(
						parentID,
						-1,
						wxDefaultPosition,
						wxSize(640,480),
						0,
						_T(""),
						awardID);

	awardPanel->Enable();
	return (awardPanel);
}

void wxWidgetsFrame::OnAwardConfig(wxCommandEvent& event)
{
	wxString			title="Award Editor";
	wxString			listGroup="AWARDLIST";
	wxString			listPrefix="AWARDDEFINITION";

	GenericNumberedConfigGUI	gui(
							NewAwardPanel,
							listGroup,
							listPrefix,
							this,
							-1,
							title,
							wxDefaultPosition,
							wxSize(640,480),
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));

	gui.DisplayDialog();
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
						-1,
						title,
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						_T(""),
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
	wxSize				guiSize(768,600);

	DropListDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnDebugConfig(wxCommandEvent& event)
{
	wxString			title="Developer Debug Settings";
	wxString			configKey;
	GroupedConfigItemsPanel	*commonConfigs;
	GroupedConfigItemsPanel	*debugLevels;
	GenericConfigPanel	*configPanel;

	GenericConfigGUI	gui(this,
							-1,
							title,
							wxDefaultPosition,
							wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""));


	configPanel=new GenericConfigPanel(&gui,
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panel holds the various boxed configs
	commonConfigs=new GroupedConfigItemsPanel("Steps Enabled");
	commonConfigs->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey.Printf("/Debug/Step%s",STEP_CREATE_DATABASE);
	commonConfigs->AddBoolean("Create Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_APPLY_ALIASES);
	commonConfigs->AddBoolean("Apply Aliases",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_DOWNLOAD_LOGS);
	commonConfigs->AddBoolean("Download Logs",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_DATABASE_READ_STATIC);
	commonConfigs->AddBoolean("Read Static",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_EMPTY_DATABASE);
	commonConfigs->AddBoolean("Empty Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_CREATE_LOG_FILES);
	commonConfigs->AddBoolean("Create Logs",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_PROCESS_LOG_FILES);
	commonConfigs->AddBoolean("Process Logs",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_DROP_PLAYERS);
	commonConfigs->AddBoolean("Drop Players",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_CALCULATE_SCORES);
	commonConfigs->AddBoolean("Calculate Scores",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_WRITE_DATABASE);
	commonConfigs->AddBoolean("Write Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_READ_TEMPLATE);
	commonConfigs->AddBoolean("Read Template",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_PROCESS_TEMPLATE);
	commonConfigs->AddBoolean("Process Template",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_POST_PROCESS_TEMPLATE);
	commonConfigs->AddBoolean("Post Process Template",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_TRANSFER_DATABASE);
	commonConfigs->AddBoolean("Transfer Database",configKey,true);

	configKey.Printf("/Debug/Step%s",STEP_WEBSITE);
	commonConfigs->AddBoolean("Website Transfer",configKey,true);

	debugLevels=new GroupedConfigItemsPanel("Debug Levels 0 (off) - 3 (max)");
	debugLevels->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);
	int			panelIndex;
	wxString	debugTitle;

	configKey="/Debug/DebugFile";
	debugLevels->AddFile("Debug File (blank for no debug)",configKey,"",-1,NULL);
	for (panelIndex=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		panelIndex<WINDOW_ID_PROGRESS_PANEL_MAX;
		panelIndex++)
	{
		configKey.Printf("/Debug/DebugLevel%d",
						(panelIndex-WINDOW_ID_PROGRESS_PANEL_GENERAL));
		debugTitle=StatusPanel::PanelName(panelIndex);
		debugLevels->Add((char *)debugTitle.GetData(),configKey,"0",1);
	}

	configPanel->AddConfigGroup(commonConfigs);
	configPanel->AddConfigGroup(debugLevels);

	gui.SetConfigPanel(configPanel);
	gui.DisplayDialog();

	STATSGEN_DEBUG_INITIATE
}

void wxWidgetsFrame::OnOutputConfig(wxCommandEvent& event)
{
	wxString				title="Output Config";
	wxString				defaultValue;
	GroupedConfigItemsPanel	*databaseConfig;
	GroupedConfigItemsPanel	*fullTemplateConfig;
	GroupedConfigItemsPanel	*outputConfig;
	wxString				configKey;
	wxString				configGroup="website";

	GenericConfigGUI	gui(this,
							-1,
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
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panel holds the various boxed configs
	databaseConfig=new GroupedConfigItemsPanel("Database Config");
	databaseConfig->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	wxString	dbFilename=BaseDirectoryFileName("statsgen2.db").GetFullPath();
	configKey="/Database/filename";
	databaseConfig->AddFile("Database File",configKey,(char *)dbFilename.GetData(),-1,NULL);
	configKey="/Database/CreateIndexes";
	databaseConfig->AddBoolean("Create Indexes",configKey,true);

	fullTemplateConfig=new GroupedConfigItemsPanel("Template Config");
	fullTemplateConfig->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey="/Template/FullRun";
	fullTemplateConfig->AddFile("Template",configKey,"",-1,NULL);
	configKey="/Schedule/ProductionRun";
	defaultValue="00:00:00,86400";
	fullTemplateConfig->AddSchedule("Schedule",configKey,defaultValue,true);

	outputConfig=new GroupedConfigItemsPanel("Local Output Config");
	outputConfig->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);
	configKey="/General/LocalOutput";
	outputConfig->AddDirectory("Local Output Location",configKey,"",-1);

	configKey=CONFIG_KEY_SPEECHFILE;
	outputConfig->AddFile("Speech File (blank for none)",configKey,"",-1);
	configPanel->AddConfigGroup(databaseConfig);
	configPanel->AddConfigGroup(fullTemplateConfig);
	configPanel->AddConfigGroup(outputConfig);

	gui.SetConfigPanel(configPanel);
	gui.DisplayDialog();
	globalStatistics.ReadProductionRunTimerConfig();
	globalStatistics.ReadQuickRunTimerConfig();
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
						-1,
						_T("Edit Template Config Items"),
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						_T(""),
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
						-1,
						_T("Edit Message Centre Formats"),
						wxDefaultPosition,
						wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						_T(""),
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
	if (FirstTimeRun())
	{
		GUIWizardPagesFirstConfig	wizardPages;
		RunWizard(&wizardPages);
	}
	Upgrade();
	SetMenuItems();
}

void wxWidgetsFrame::OnAbout(wxCommandEvent& event)
{
	wxString	msg;

	msg.Printf(
		_T("Statsgen 2\n")
		_T("http://www.statsgen.co.uk - Version %s\n")
		_T("\n")
		_T("http://www.sqlite.org     - Version %s\n")
		_T("http://www.wxwidgets.org  - Version %s\n")
		_T("\n")
		_T("Default Templates created by Nazarine and\n")
		_T("Omega of www.ssrangers.ca\n")
		_T("\n")
		_T("Big thanks to Omega for all the work on the templates\n")
		_T("\n")
		_T("Another big thanks to Elric666 (www.snl-clan.com) for all\n")
		_T("the help testing Quake Wars processing with me\n")
		,
		PROGRAM_VERSION,
		SQLITE_VERSION,
		wxVERSION_STRING
		);
	wxMessageBox(msg, _T("About"),wxOK | wxICON_INFORMATION,this);

}

void wxWidgetsFrame::RunWizard(GUIWizardPages *wizardPages)
{
	wxString		title		="Statsgen Wizard";
	long			wizardStyle	=	wxRESIZE_BORDER |
									wxMAXIMIZE_BOX |
									wxMINIMIZE_BOX;

	runningWizard=true;
	StatsgenWizard		wizard(NULL,-1,title,wxNullBitmap,wxDefaultPosition, wizardStyle);

	wizardPages->CreateWizardPages(&wizard);
	wizard.SetInitialPageLinks();
	wizard.FitToPage(wizard.GetFirstPage());
	wizard.RunWizard(wizard.GetFirstPage());
	runningWizard=false;
}

void wxWidgetsFrame::OnPlayerDataAvatarConfig(wxCommandEvent& event)
{
	wxString			title="Avatar Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Avatar Image Reference Here";
	wxString			playerTitle="Avatar Reference";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataAvatars,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataPictureConfig(wxCommandEvent& event)
{
	wxString			title="Personal Picture Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Personal Picture Reference Here";
	wxString			playerTitle="Personal Picture Reference";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataPicture,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataWebsiteConfig(wxCommandEvent& event)
{
	wxString			title="Personal Website Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Personal Website Reference Here";
	wxString			playerTitle="Personal Website Reference";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataWebsite,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataXfireConfig(wxCommandEvent& event)
{
	wxString			title="Personal Xfire Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Xfire ID Here";
	wxString			playerTitle="Xfire ID";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataXfire,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataClanRoleConfig(wxCommandEvent& event)
{
	wxString			title="Clan Role Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Clan Role Here";
	wxString			playerTitle="Clan Role";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataClanRole,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataRealNameConfig(wxCommandEvent& event)
{
	wxString			title="Real Name Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Real Name Here";
	wxString			playerTitle="Real Name";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataRealName,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataMisc1Config(wxCommandEvent& event)
{
	wxString			title="Miscellaneous Data 1 Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Data Here";
	wxString			playerTitle="Data";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataMisc1,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnPlayerDataMisc2Config(wxCommandEvent& event)
{
	wxString			title="Miscellaneous Data 2 Editor";
	wxSize				guiSize(768,600);
	wxString			playerDefaultValue="Insert Data Here";
	wxString			playerTitle="Data";

	PlayerDataDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						globalStatistics.playerDataMisc2,
						playerDefaultValue,
						playerTitle);

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnBadWordsListConfig(wxCommandEvent& event)
{
	wxString			title="Bad Words List Editor";
	wxSize				guiSize(300,400);

	TextListDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						"BannedSpeech");

	gui.DisplayDialog();
}

void wxWidgetsFrame::OnHelp(wxCommandEvent& event)
{
	m_advancedHtmlHelp.DisplayContents();
}

void wxWidgetsFrame::OnCustomMessagesConfig(wxCommandEvent& event)
{
	wxString			title="Custom Message List";
	wxSize				guiSize(300,400);

	TextListDialog	gui(
							this,
							-1,
							title,
							wxDefaultPosition,
							guiSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
							_T(""),
						"CustomMessages");
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
		fprintf(fp,"%s=%s\n",key.GetData(),value.GetData());
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

	buildINIFileName=BaseDirectoryFileName("build.ini");
	buildINIFileNameStr=buildINIFileName.GetFullPath();
	fp=fopen(buildINIFileNameStr.GetData(),"w");
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

	CopyGroupToFile("Colours",fp);
	CopyGroupToFile("DEATHMATCH",fp);
	CopyGroupToFile("TEAMSWAP",fp);
	CopyGroupToFile("LINEIDMOHAA",fp);
	CopyGroupToFile("LINEIDWOLFENSTEIN",fp);
	CopyGroupToFile("AutoAliasBlockedGUIDS",fp);

	CopyGroupToFile("ScoreWeights",fp);
	CopyGroupToFile("RealNames",fp);
	CopyGroupToFile("Images",fp);

	CopyGroupToFile("LogEntriesACTION",fp);
	CopyGroupToFile("LogEntriesWEAPON",fp);
	CopyGroupToFile("LogEntriesLOCATION",fp);

	CopyGroupToFile("MessageCentreFormats",fp);

	CopyGroupToFile("LogSubstituteACTION",fp);
	CopyGroupToFile("LogSubstituteAMMO",fp);
	CopyGroupToFile("LogSubstituteGAMETYPE",fp);
	CopyGroupToFile("LogSubstituteMAP",fp);
	CopyGroupToFile("LogSubstituteCLASS",fp);
	CopyGroupToFile("LogSubstituteTEAM",fp);
	CopyGroupToFile("LogSubstituteWEAPON",fp);
	CopyGroupToFile("LogSubstituteWEAPONAMMO",fp);
	CopyGroupToFile("LogSubstituteLOCATION",fp);


	CopyGroupToFile("AWARDLIST",fp);
	globalStatistics.configData.ReadList("AWARDLIST",awardIDs);
	awardCount=awardIDs.GetCount();
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		awardID.Printf("AWARDDEFINITION%s",awardIDs.Item(awardIndex).GetData());
		CopyGroupToFile(awardID.GetData(),fp);
	}



	// Weapon Groups
	weaponGroups=globalStatistics.configData.ReadWeaponGroups();
	weaponGroupCount=weaponGroups.GetCount();
	for (weaponGroupIndex=0;weaponGroupIndex<weaponGroupCount;weaponGroupIndex++)
	{
		weaponGroup=weaponGroups.Item(weaponGroupIndex);
		CopyGroupToFile(weaponGroup.GetData(),fp);
	}
	fclose(fp);

	CopyImagesToUpgradeFile("cod1images.ini","cod1");
	CopyImagesToUpgradeFile("cod2images.ini","cod2");
	CopyImagesToUpgradeFile("cod4images.ini","cod4");
	CopyImagesToUpgradeFile("cod5images.ini","cod5");
	CopyImagesToUpgradeFile("mohaaimages.ini","mohaa");
	CopyImagesToUpgradeFile("wolfensteinimages.ini","wolfenstein");
	CopyImagesToUpgradeFile("quakewarsimages.ini","quakewars");

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
	fp=fopen(upgradeFileNameStr.GetData(),"w");


	if (fp==NULL)
	{
		return;
	}

	fprintf(fp,"[UPGRADELIST]\n");
	fprintf(fp,"Description=Image Pack for %s images\n",filter);
	fprintf(fp,"UpgradeType=Configuration File\n");
	fprintf(fp,"LISTCOUNT=2\n");
	fprintf(fp,"LISTITEM001=Images\n");
	fprintf(fp,"LISTITEM002=RealNames\n");
	fprintf(fp,"[Images]\n");
	fprintf(fp,"UPGRADEDESCRIPTION=Image References\n");
	fprintf(fp,"UPGRADERECOMMENDATION=Update\n");
	fprintf(fp,"UPGRADEANSWERTYPE=0\n");

	globalStatistics.configData.ReadGroup("Images",keys,values);
	keyCount=keys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keys.Item(keyIndex);
		value=values.Item(keyIndex);
		serverType=key.AfterFirst('_');
		serverType=serverType.BeforeFirst('_');
		if (serverType.CmpNoCase(filter)==0)
		{
			fprintf(fp,"%s=%s\n",key.GetData(),value.GetData());
		}
	}
	fprintf(fp,"[RealNames]\n");
	fprintf(fp,"UPGRADEDESCRIPTION=Real Names References\n");
	fprintf(fp,"UPGRADERECOMMENDATION=Update\n");
	fprintf(fp,"UPGRADEANSWERTYPE=0\n");
	globalStatistics.configData.ReadGroup("RealNames",keys,values);
	keyCount=keys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keys.Item(keyIndex);
		value=values.Item(keyIndex);
		serverType=key.AfterFirst('_');
		serverType=serverType.BeforeFirst('_');
		if (serverType.CmpNoCase(filter)==0)
		{
			fprintf(fp,"%s=%s\n",key.GetData(),value.GetData());
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
	wxSize		dialogSize(600,250);
	wxString	name="performupgrade";
	long		style= wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX;

	PerformUpgradeDialog	dialog(this,
									-1,
									title,
									wxDefaultPosition,
									dialogSize,
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
	buildVersion=atoi(configValue.GetData());
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
	buildVersion=atoi(configValue.GetData());
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
	buildVersion=atoi(configValue.GetData());
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
							-1,
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
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panel holds the various boxed configs
	databaseConfig=new GroupedConfigItemsPanel("External Database Config");
	databaseConfig->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey="/ExternalDatabase/Enabled";
	databaseConfig->AddBoolean("Enabled",configKey,false);

	triggerItem.SetPositiveDisableTrigger(configKey,"N");
	triggerList.Add(triggerItem);

	configKey="/ExternalDatabase/CreateIndexes";
	databaseConfig->AddBoolean("Create Table Indexes",configKey,true,&triggerList);

	configKey="/ExternalDatabase/Hostname";
	databaseConfig->Add("Hostname",configKey,"",-1,&triggerList);

	configKey="/ExternalDatabase/Username";
	databaseConfig->Add("Username",configKey,"",-1,&triggerList);

	configKey="/ExternalDatabase/Password";
	databaseConfig->Add("Password",configKey,"",-1,&triggerList);

	configKey="/ExternalDatabase/DatabaseName";
	databaseConfig->Add("Database Name",configKey,"statsgen",-1,&triggerList);

	configKey="/ExternalDatabase/Port";
	databaseConfig->Add("Port",configKey,"3306",5,&triggerList);

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
									-1,
									title,
									wxDefaultPosition,
									dialogSize,
									style,
									name);
	PriorityPanel			*priorityPanel=new PriorityPanel();
	priorityPanel->Create(&dialog,
							-1,
							wxDefaultPosition,
							wxDefaultSize,
							wxTAB_TRAVERSAL,
							_T("PANEL"));

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
	wxString				title="Internal Web Server Config";
	wxString				defaultValue;
	GroupedConfigItemsPanel	*webserverConfig;
	wxString				configKey;
	GUITriggerList			triggerList;
	GUITriggerItem			triggerItem;

	GenericConfigGUI	gui(this,
							-1,
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
									-1,
									wxDefaultPosition,
									wxDefaultSize,
									0,
									_T("PANEL"));

	// Panel holds the various boxed configs
	webserverConfig=new GroupedConfigItemsPanel("Internal Web Server Config");
	webserverConfig->Create(configPanel,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey=globalStatistics.webServer.ConfigKeyEnabled();
	webserverConfig->AddBoolean("Enabled",configKey,false);

	triggerItem.SetPositiveDisableTrigger(configKey,"N");
	triggerList.Add(triggerItem);

	configKey=globalStatistics.webServer.ConfigKeyPort();
	webserverConfig->Add("Listen Port",configKey,"9000",-1,&triggerList);

	configKey=globalStatistics.webServer.ConfigKeyAdminUser();
	webserverConfig->Add("Admin Username",configKey,"",-1,&triggerList);

	configKey=globalStatistics.webServer.ConfigKeyAdminPassword();
	webserverConfig->Add("Admin Password",configKey,"",-1,&triggerList);

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
						-1,
						title,
						wxDefaultPosition,
						wxSize(640,480),
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
						name);

	gui.DisplayDialog();
}

