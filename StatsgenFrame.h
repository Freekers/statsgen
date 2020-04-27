#ifndef __WXWIDGETSFRAME_H
#define __WXWIDGETSFRAME_H

#include <wx/wx.h>
#include <wx/help.h>
#include <wx/fs_zip.h>
#include <wx/html/helpctrl.h>
#include <wx/cshelp.h>

#include "TemplateFile.h"
#include "GUIWizards.h"

class WorkerThread : public wxThread
{
	public:
		WorkerThread(wxThreadKind);
		virtual void * Entry();
		virtual void Exit(void * exitCode);
		void SetRunType(bool runTypeIn,
				wxArrayInt &allowedStepsIn,
				TemplateFile *templateFileIn);
	private:
		bool runType;
		wxArrayInt	allowedSteps;
		TemplateFile	*templateFile;
};

class wxWidgetsFrame : public wxFrame
{
public:
	wxWidgetsFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~wxWidgetsFrame();

	void OnTimerTick(wxTimerEvent& event);
	void OnProgressUpdate(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnConfigThread(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
	void OnWizard(wxCommandEvent &event);
	void OnGo(wxCommandEvent& event);
	void OnDatabaseEmpty(wxCommandEvent& event);
	void OnEditScoreWeights(wxCommandEvent& event);
	void OnEditSkillWeights(wxCommandEvent& event);
	wxHtmlHelpController &GetAdvancedHtmlHelpController()
		{ return m_advancedHtmlHelp; }
	void OnEditRealNames(wxCommandEvent& event);
	void OnTemplateConfig(wxCommandEvent& event);
	void OnEditImages(wxCommandEvent& event);
	void OnEditMessageFormats(wxCommandEvent& event);
	void OnWebsiteConfig(wxCommandEvent& event);
	void OnImagePackConfig(wxCommandEvent& event);
	void OnTemplatePackConfig(wxCommandEvent& event);
	void OnFullConfig(wxCommandEvent& event);
	void OnWebServerConfig(wxCommandEvent& event);
	void OnOutputConfig(wxCommandEvent& event);
	void OnExternalDatabaseConfig(wxCommandEvent& event);
	void OnServerConfig(wxCommandEvent& event);
	void OnRCONSpamConfig(wxCommandEvent& event);
	void OnEditWeaponGroups(wxCommandEvent& event);
	void OnLogSubstitute(wxCommandEvent& event);
	void OnClanConfig(wxCommandEvent& event);
	void OnAliasConfig(wxCommandEvent& event);
	void OnDropListConfig(wxCommandEvent& event);
	void OnBadWordsListConfig(wxCommandEvent& event);
	void OnCustomMessagesConfig(wxCommandEvent& event);
	void OnPlayerDataAvatarConfig(wxCommandEvent& event);
	void OnPlayerDataPictureConfig(wxCommandEvent& event);
	void OnPlayerDataXfireConfig(wxCommandEvent& event);
	void OnPlayerDataWebsiteConfig(wxCommandEvent& event);
	void OnPlayerDataClanRoleConfig(wxCommandEvent& event);
	void OnPlayerDataRealNameConfig(wxCommandEvent& event);
	void OnPlayerDataMisc1Config(wxCommandEvent& event);
	void OnPlayerDataMisc2Config(wxCommandEvent& event);
	void OnAwardConfig(wxCommandEvent& event);
	void OnDebugConfig(wxCommandEvent& event);
	void OnExportTemplate(wxCommandEvent& event);
	void OnExportUpgrade(wxCommandEvent& event);
	void OnPerformUpgrade(wxCommandEvent& event);
	void OnAdvanced(wxCommandEvent& event);
	void Upgrade();
	void UpgradeDatabase();
	void UpgradeTemplate();
	void UpgradeConfig();
	void ManualUpgrade();
	void OnResetRecovery(wxCommandEvent &event);
	void OnStoreProgress(wxCommandEvent &event);
	void SetMenuItems();
	void RunWizard(GUIWizardPages *wizardPages);
	void CopyGroupToFile(const char *groupID,FILE *fp);
	void CopyImagesToUpgradeFile(const char *upgradefilename,const char *filter);
	void OnExportBuildINI(wxCommandEvent &event);

private:
	WorkerThread		*workerThread;
	bool			quitNow;
	wxTimer	scheduleTimer;
	wxHtmlHelpController		m_advancedHtmlHelp;
	bool				advanced;
	bool				runningWizard;
	DECLARE_CLASS(wxWidgetsFrame)

	DECLARE_EVENT_TABLE()
};

#endif //__WXWIDGETSFRAME_H
