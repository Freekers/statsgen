#ifndef _GUIWIZARDS_H
#define _GUIWIZARDS_H

#include <wx/wx.h>
#include <wx/wizard.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/stattext.h>

#include "WindowIDs.h"
#include "ProgressPanel.h"
#include "StatsgenWeb.h"
#include "ImagePanel.h"

#define WIZARD_CHOICE_GAMESERVER	"gameserver"
#define WIZARD_CHOICE_HOMEPC		"homepc"

#define WIZARD_CHOICE_NEW		"New"
#define WIZARD_CHOICE_EDIT		"Edit"
#define WIZARD_CHOICE_DELETE	"Delete"

class StatsgenWizard;
class GUIWizardPageStatsgen	: public wxWizardPage
{
	public:
		GUIWizardPageStatsgen(StatsgenWizard *wizardIn,wxString parentPageID,wxString pageID);
		virtual ~GUIWizardPageStatsgen();
		virtual wxWizardPage *GetPrev() const;
		virtual wxWizardPage *GetNext() const;
		void SetPreviousPage(wxWizardPage *page);
		void SetNextPage(wxWizardPage *page);
		void SetHelp(const char *helpTextIn);

		void SetCallBack(void(* callBackIn)(void *object),void *callBackObjectIn );
		void SetPageCounter(int pageIndexIn,int pageCountIn);
		void UpdateProgress();
		void Resize(int maxHeight=-1);
		virtual bool AutoAdvance(); 
		void SetSectionTitle(const char *titleChars); 
		virtual wxString GetValue()=0;
		wxString GetPageID();
		void PageContentsChanged();

		StatsgenWizard	*wizard;
	protected:
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);

	protected:
		virtual void UpdateScreen();
		void (* callBack)(void *object);
		void *callBackObject;

		int		pageIndex;
		int		wizardPageCount;
		
	private:
		wxWizardPage	*previousPage;
		wxWizardPage	*nextPage;
		wxString		helpText;
		wxRichTextCtrl	*helpTextCtrl;
		wxGauge			*installProgress;
		wxStaticText	*sectionTitle;
		wxString		pageID;

};

class GUIPageLinks
{
	public:
		void AddPageLink(GUIWizardPageStatsgen *page,
					const char *allowRule,
					const char *continueRule);
		void SetPageLinks(bool ignoreRules);
		void SetPageCounters();
		GUIWizardPageStatsgen *GetFirstPage();
		void ResizePages();
	private:
		GUIWizardPageStatsgen *FindPage(int pageIndex,int direction,bool ignoreRules);
		bool EvaluateRule(wxString &rule,bool ignoreRules);
		bool EvaluateRuleSub(wxString &rule);
		void PushValue(wxArrayString &valueQueue,wxString &value); 
		wxString PopValue(wxArrayString &valueQueue); 
		int ValueCount(wxArrayString &valueQueue);
		wxString Bool2Value(bool value);
		bool Value2Bool(wxString &value);
		wxString GetValue(wxString &id);
		wxArrayPtrVoid	guiPages;
		wxArrayString	allowRules;
		wxArrayString	continueRules;
};

class GUIWizardPageNull : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageNull(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageNull();
		virtual wxString GetValue();
		virtual bool AutoAdvance(); 
};

class GUIWizardPageConfig : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageConfig(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfig();
		void SetConfigKey(wxString &configKeyIn,const char *defaultValueIn=NULL);
		void SetDefault(const char *defaultValueIn);
		void SetConfigTitle(const char *titleIn);
		virtual void OnValueChanged(wxCommandEvent &event);
		virtual wxString GetValue();
		virtual void UpdateValueFromConfig();
		void VetoCtrlUpdates();
		void AllowCtrlUpdates();
		bool CtrlUpdatesAllowed();
	protected:
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		virtual void UpdateConfigFromValue();
		virtual void UpdateCtrlFromValue();
		virtual bool ConfigReady();
		virtual void UpdateScreen();

		virtual bool ScreenReady()=0;
		virtual void SetCtrlFromValue()=0;
		virtual void SetValueFromCtrl()=0;

		wxString		value;
		wxString		title;
		wxString		configKey;
		wxString		defaultValue;
	private:
		wxStaticText	*titleCtrl;
		bool		vetoUpdates;
};

class GUIWizardPageConfigText : public GUIWizardPageConfig
{
	public:
		GUIWizardPageConfigText(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigText();
	protected:
		virtual void CreateControl();
		virtual void SetCtrlFromValue();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		virtual bool ScreenReady();
		virtual void SetValueFromCtrl();
	protected:
		wxTextCtrl		*valueCtrl;
		
	DECLARE_EVENT_TABLE();
};

class GUIWizardPageConfigBoolean : public GUIWizardPageConfig
{
	public:
		GUIWizardPageConfigBoolean(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigBoolean();
		void SetTrueFalseLabels(const char *trueLabelIn,const char *falseLabelIn);
	protected:
		virtual void CreateControl();
		virtual void SetCtrlFromValue();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		virtual bool ScreenReady();
		virtual void SetValueFromCtrl();
	protected:
		wxCheckBox		*valueCtrl;
	private:
		wxString	trueLabel;
		wxString	falseLabel;
		void UpdateLabel();
		
	DECLARE_EVENT_TABLE();
};

class GUIWizardPageConfigFile : public GUIWizardPageConfig
{
	public:
		GUIWizardPageConfigFile(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigFile();
		void OnButtonPressed(wxCommandEvent &event);
		virtual void UpdateValueFromConfig();
		virtual void SplitValue(wxString &valueDirectory,
						wxString &valueFilename)=0;
	protected:
		virtual void CreateControl();
		virtual void SetCtrlFromValue();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		virtual bool ScreenReady();
		virtual void SetValueFromCtrl();
		virtual void UpdateConfigFromValue();
		virtual wxString RequestFileFromUser()=0;
	protected:
		wxTextCtrl		*valueCtrl;
		wxButton		*browseButton;
	protected:
		void GetConfigKeys(wxString &configDirectory,
						wxString &configFilename);
		virtual void CombineValue();

		wxString		directory;
		wxString		filename;
		
	DECLARE_EVENT_TABLE();
};

class GUIWizardPageConfigRemoteFile : public GUIWizardPageConfigFile
{
	public:
		GUIWizardPageConfigRemoteFile(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigRemoteFile();
		virtual void SplitValue(wxString &valueDirectory,
						wxString &valueFilename);
	protected:
		virtual wxString RequestFileFromUser();
		virtual void CombineValue();
};

class GUIWizardPageConfigLocalFile : public GUIWizardPageConfigFile
{
	public:
		GUIWizardPageConfigLocalFile(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual void SplitValue(wxString &valueDirectory,
						wxString &valueFilename);
	protected:
		virtual wxString RequestFileFromUser();
		virtual void CombineValue();
};

class GUIWizardPageConfigWildcard : public GUIWizardPageConfig
{
	public:
		GUIWizardPageConfigWildcard(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigWildcard();
		void OnButtonPressed(wxCommandEvent &event);
		virtual void UpdateValueFromConfig();
		virtual wxString GetDirectoryFromUser()=0;
		wxString GetWildcard();
	protected:
		virtual void CreateControl();
		virtual void SetCtrlFromValue();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		virtual bool ScreenReady();
		virtual void SetValueFromCtrl();
		virtual void UpdateConfigFromValue();
	protected:
		wxTextCtrl		*valueCtrl;
		wxTextCtrl		*wildcardCtrl;
		wxButton		*browseButton;
		wxString		wildcardValue;
		wxStaticText	*directoryLabel;
		wxStaticText	*wildcardLabel;
	private:
		void GetConfigKeys(wxString &configDirectory,
						wxString &configFilename);

		
	DECLARE_EVENT_TABLE();
};

class GUIWizardPageConfigLocalWildcard : public GUIWizardPageConfigWildcard
{
	public:
		GUIWizardPageConfigLocalWildcard(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigLocalWildcard();
		virtual wxString GetDirectoryFromUser();
};

class GUIWizardPageConfigRemoteWildcard : public GUIWizardPageConfigWildcard
{
	public:
		GUIWizardPageConfigRemoteWildcard(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigRemoteWildcard();
		virtual wxString GetDirectoryFromUser();
};

class GUIWizardPageConfigRemoteDirectory : public GUIWizardPageConfig
{
	public:
		GUIWizardPageConfigRemoteDirectory(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageConfigRemoteDirectory();
		void OnButtonPressed(wxCommandEvent &event);
		virtual void UpdateValueFromConfig();
	protected:
		virtual void CreateControl();
		virtual void SetCtrlFromValue();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		virtual bool ScreenReady();
		virtual void SetValueFromCtrl();
		virtual void UpdateConfigFromValue();
	protected:
		wxTextCtrl		*valueCtrl;
		wxButton		*browseButton;
		
	DECLARE_EVENT_TABLE();
};


class GUIWizardPageConfigChoice : public GUIWizardPageConfig
{
	public:
		GUIWizardPageConfigChoice(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID,
								wxArrayString &answerCodesIn,
								wxArrayString &buttonTextsIn,
								wxArrayString &buttonDescriptionsIn);
		virtual ~GUIWizardPageConfigChoice();
		wxString TestResult();
		void OnButtonPressed(wxCommandEvent &event);
		wxString GetChoice();
	protected:
		virtual void CreateControl();
		virtual void SetCtrlFromValue();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		virtual bool ScreenReady();
		virtual void SetValueFromCtrl();
	public:

		void SetSelection(const char *selection);
		void SetSelection(wxString &selection);
	protected:

		wxArrayPtrVoid	buttons;
		wxArrayPtrVoid	labels;
		wxArrayString	answerCodes;
		wxArrayString	buttonTexts;
		wxArrayString	buttonDescriptions;
		wxTextCtrl		*valueCtrl;
		wxStaticText	*selectionLabel;

		wxString		choice;
		
	DECLARE_EVENT_TABLE();
};

class GUIWizardPageRemoteTest : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageRemoteTest(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageRemoteTest();
		void SetGroupPrefix(wxString &groupPrefixIn);
		bool TestResult();
		bool PerformTest();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		void OnButtonPressed(wxCommandEvent &event);
		void ResetTestResult();
		virtual wxString GetValue();

	private:
		wxStaticText	*statusCtrl;
		wxRichTextCtrl	*errorCtrl;
		wxButton		*testButton;

		wxString		groupPrefix;
	private:
		bool			lastConnectionResult;
		

	DECLARE_EVENT_TABLE();
};

class GUIWizardPageRun : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageRun(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageRun();
		bool TestResult();
		bool PerformTest();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		void OnButtonPressed(wxCommandEvent &event);
		void ResetTestResult();
		virtual wxString GetValue();
		void SetTestParameters(bool testRun);
		void SetSelection(const char *selection);

	private:
		wxStaticText	*statusCtrl;
		wxRichTextCtrl	*errorCtrl;
		wxButton		*testButton;
		ProgressPanel	*progressPanel;

	private:
		bool			runSuccessful;

	DECLARE_EVENT_TABLE();
};

class GUIWizardPageImagePacks : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageImagePacks(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageImagePacks();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		void OnButtonPressed(wxCommandEvent &event);
		virtual wxString GetValue();

	private:
		wxScrolledWindow	*imagesCtrl;
		wxButton			*testButton;
		ProgressPanel		*progressPanel;
		StatsgenWeb			statsgenSite;

	private:
		void CreateImagePackPanels();
		void SelectImagePackPanel(wxString &serverType);
		void SelectImagePackPanels();
		void ResizeImagePackPanels();

	private:
		wxArrayString				fileDescriptions;
		wxArrayString				filePaths;
		wxArrayString				serverTypes;
		wxArrayString				thumbnails;
		wxArrayString				upgradeFiles;
		wxArrayPtrVoid				imagePackPanels;

	DECLARE_EVENT_TABLE();
};

class GUIWizardPageRemoteFileTest : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageRemoteFileTest(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageRemoteFileTest();
		void SetGroupPrefix(wxString &groupPrefixIn);
		bool TestResult();
		bool PerformTest();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		void OnButtonPressed(wxCommandEvent &event);
		void ResetTestResult();
		void SetFile(wxString &fileDirectoryIn, wxString &fileNameIn);
		virtual wxString GetValue();

	private:
		wxStaticText	*statusCtrl;
		wxRichTextCtrl	*errorCtrl;
		wxButton		*testButton;

		wxString		groupPrefix;
		wxString		fileDirectory;
		wxString		fileName;
	private:
		bool			lastResult;
		wxArrayString	filenameList;
		wxArrayInt		filesizeList;
		

	DECLARE_EVENT_TABLE();
};

class GUIWizardPageRemoteDirectoryTest : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageRemoteDirectoryTest(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageRemoteDirectoryTest();
		void SetGroupPrefix(wxString &groupPrefixIn);
		bool TestResult();
		bool PerformTest();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		void OnButtonPressed(wxCommandEvent &event);
		void ResetTestResult();
		void SetDirectory(wxString &fileDirectoryIn);
		virtual wxString GetValue();

	private:
		wxStaticText	*statusCtrl;
		wxRichTextCtrl	*errorCtrl;
		wxButton		*testButton;

		wxString		groupPrefix;
		wxString		fileDirectory;
	private:
		bool			lastResult;
		

	DECLARE_EVENT_TABLE();
};

class GUIWizardPages
{
	public:
		GUIWizardPages();
		virtual ~GUIWizardPages();
		void SetPageID(wxString parentID,wxString pageIDIn);
		virtual void CreateWizardPages(StatsgenWizard *wizard)=0;
	protected:
		wxString pageID;
		StatsgenWizard	*wizard;
};

class GUIWizardPagesRemoteMachine : public GUIWizardPages
{
	public:
		GUIWizardPagesRemoteMachine();
		virtual ~GUIWizardPagesRemoteMachine();
		void SetGroupPrefix(wxString &groupPrefixIn);
		virtual void CreateWizardPages(StatsgenWizard *wizard);
		void ResizePages();
		static void ConfigChangedCallBack(void *callingObject);
		void SetCallBack(void(* callBackIn)(void *object),void *callBackObjectIn );
		static void ConnectionTestCallBack(void *callingObject);
		void SetSectionTitle(const char *titleChars); 
		void PopCallBack();

		bool IsFTPEnabled();
		wxString GetEnabledPageID();

	private:
		void UpdateConfigKeys();
		void ConnectionTest();

	private:
		wxString	groupPrefix;
		GUIWizardPageConfigText		*pageHostname;
		GUIWizardPageConfigBoolean	*pageEnabled;
		GUIWizardPageConfigText		*pageUsername;
		GUIWizardPageConfigText		*pagePassword;
		GUIWizardPageConfigText		*pageFTPPort;
		GUIWizardPageConfigBoolean	*pagePassive;
		GUIWizardPageRemoteTest		*pageTest;

		void (* callBack)(void *object);
		void *callBackObject;
};

class GUIWizardPageChoice : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageChoice(
			StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID,
			wxArrayString &answerCodesIn,
			wxArrayString &buttonTextsIn,
			wxArrayString &buttonDescriptionsIn);
		virtual ~GUIWizardPageChoice();
		void UpdateChoices(
			wxArrayString &answerCodesIn,
			wxArrayString &buttonTextsIn,
			wxArrayString &buttonDescriptionsIn);
		wxString TestResult();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		void OnButtonPressed(wxCommandEvent &event);
		wxString GetChoice();
		void SelectChoice(wxString &choiceIn);
		virtual wxString GetValue();

	public:
		void SetSelection(const char *selection);
		void SetSelection(wxString &selection);
	private:
		wxArrayPtrVoid	buttons;
		wxArrayPtrVoid	labels;
		wxArrayString	answerCodes;
		wxArrayString	buttonTexts;
		wxArrayString	buttonDescriptions;
		wxStaticText	*selectedChoice;
		wxStaticText	*selectionLabel;

		wxString		choice;

	DECLARE_EVENT_TABLE();
};

class GUIWizardPagesServer : public GUIWizardPages
{
	public:
		GUIWizardPagesServer();
		virtual ~GUIWizardPagesServer();
		virtual void CreateWizardPages(StatsgenWizard *wizard);
		void ResizePages();
		void SetTestParameters();

		void SetGroupPrefix(wxString &groupPrefixIn);

		void UpdateConfigKeys();
		static void ConfigChangedCallBack(void *callingObject);
		void SetSectionTitle(const char *titleChars); 
		void PrepareServerConfig();
		void SetLogfileLimit(int limit);
		void SetLogfileLimit();

	private:
		wxString	groupPrefix;
		GUIWizardPagesRemoteMachine	remoteMachine;
		GUIWizardPageConfigChoice			*pageServerType;
		GUIWizardPageConfigChoice			*pageLocalOrRemote;
		GUIWizardPageConfigBoolean			*pageMessagingEnabled;
		GUIWizardPageConfigText				*pageRCONPassword;
		GUIWizardPageConfigText				*pageRCONPort;
		GUIWizardPageConfigRemoteDirectory	*pagePBRemoteDirectory;
		GUIWizardPageRemoteDirectoryTest	*pagePBRemoteDirectoryTest;
		GUIWizardPageConfigRemoteFile		*pageRemoteLatestFilename;
		GUIWizardPageRemoteFileTest			*pageRemoteLatestFilenameTest;
		GUIWizardPageConfigRemoteFile		*pageRemoteLatestSecondaryFilename;
		GUIWizardPageRemoteFileTest			*pageRemoteLatestSecondaryFilenameTest;
		GUIWizardPageConfigRemoteWildcard	*pageRemoteArchiveFilename;
		GUIWizardPageRemoteFileTest			*pageRemoteArchiveFilenameTest;
		GUIWizardPageConfigLocalFile		*pageLocalLatestFilename;
		GUIWizardPageConfigLocalWildcard	*pageLocalArchiveFilename;
		GUIWizardPageConfigLocalFile		*pageLocalLatestSecondaryFilename;

		int							logfileLimit;
};

class GUIWizardPagesMessaging : public GUIWizardPages
{
	public:
		GUIWizardPagesMessaging();
		virtual ~GUIWizardPagesMessaging();
		virtual void CreateWizardPages(StatsgenWizard *wizard);
		void ResizePages();
		void SetSectionTitle(const char *sectionTitle);
		static void ConfigChangedCallBack(void *object);
		void UpdateConfigKeys();
	private:
		GUIWizardPageConfigBoolean			*pageMessagingEnabled;
		GUIWizardPageConfigText				*pageMessagingFrequency;
		GUIWizardPageConfigText				*pageMessagingPort;
};

class GUIWizardPagesServers : public GUIWizardPages
{
	public:
		GUIWizardPagesServers();
		virtual ~GUIWizardPagesServers();
		virtual void CreateWizardPages(StatsgenWizard *wizard);
		void ResizePages();

		static void DeleteEditNewCallBack(void *callingObject);
		static void ServerSelectedCallBack(void *callingObject);
		void SetServerChoices();
		void SetDeleteEditNewChoices();
		void SelectServer();
		void SelectServer(wxString &prefix);
		wxString GetDeleteEditNewChoice();
		void PrepareServerConfig();
		void SetLogfileLimit();
		void SetLogfileLimit(int limit);

	private:
		GUIWizardPagesServer			pageServer;
		GUIWizardPageChoice				*pageDeleteEditNew;
		GUIWizardPageChoice				*pageServerSelection;

		int							logfileLimit;
};

class GUIWizardPagesWebsite : public GUIWizardPages
{
	public:
		GUIWizardPagesWebsite();
		virtual ~GUIWizardPagesWebsite();
		virtual void CreateWizardPages(StatsgenWizard *wizard);
		void ResizePages();

		void UpdateConfigKeys();
		void SetGroupPrefix();
		static void ConfigChangedCallBack(void *callingObject);
		void SetDirectory();

	private:
		GUIWizardPagesRemoteMachine				remoteMachine;
		GUIWizardPageConfigRemoteDirectory		*pageRemoteDirectory;
		GUIWizardPageRemoteDirectoryTest		*pageRemoteDirectoryTest;

		wxString								groupPrefix;
};
class ImagePackPanel : wxPanel
{
	public:
		ImagePackPanel(wxWindow *parent,
						wxString &fileDescriptionIn,
						wxString &filePathIn,
						wxString &serverTypeIn,
						wxString &upgradeFileIn,
						wxString &thumbnailIn);
		virtual ~ImagePackPanel();

		wxString GetServerType();
		wxString GetDescription();
		wxString GetFilePath();
		wxString GetUpgradeFile();
		void SelectForUpload();
		bool Selected();
		wxSize ResizeControl(int x,int y,int width,int height,bool resize);

	private:
		wxString		fileDescription;
		wxString		filePath;
		wxString		serverType;
		wxString		thumbnail;
		wxString		upgradeFile;
		wxCheckBox		*checkCtrl;
		wxStaticText	*descriptionCtrl;
		wxImage			thumbnailImage;
		ImagePanel		*thumbnailPanel;
};

class GUIWizardPagesImagePacks : public GUIWizardPages
{
	public:
		GUIWizardPagesImagePacks();
		virtual ~GUIWizardPagesImagePacks();
		virtual void CreateWizardPages(StatsgenWizard *wizard);
		void ResizePages();

	private:
		GUIWizardPageImagePacks		*pageImagePacks;


};

class GUIWizardPagesFullConfig : public GUIWizardPages
{
	public:
		GUIWizardPagesFullConfig();
		virtual ~GUIWizardPagesFullConfig();
		virtual void CreateWizardPages(StatsgenWizard *wizard);


	private:
		GUIWizardPagesServers		pageServers;
		GUIWizardPagesWebsite		pageWebsite;

};

class StatsgenWizard : public wxWizard
{
	public:
		StatsgenWizard(wxWindow *parent,
					int id = -1,
					wxString &title = wxEmptyString,
					const wxBitmap &bitmap=wxNullBitmap,
					const wxPoint &pos = wxDefaultPosition,
					long style = wxDEFAULT_DIALOG_STYLE);
		void OnWizardPageChanged(wxWizardEvent &event);
		void OnWizardFinish(wxWizardEvent &event);
		void OnWizardCancel(wxWizardEvent &event);
		void ResizePages();
		GUIPageLinks	pageLinks;
		void AddPageLink(GUIWizardPageStatsgen *page,
					const char *allowRule,
					const char *continueRule);
		void UpdatePageLinks(bool fullList);
		void PageContentsChanged();
		void SetInitialPageLinks();
		GUIWizardPageStatsgen *GetFirstPage();
	private:
		bool initialised;

	DECLARE_EVENT_TABLE();
};

class GUIWizardPagesFirstConfig : public GUIWizardPages
{
	public:
		GUIWizardPagesFirstConfig();
		virtual ~GUIWizardPagesFirstConfig();
		virtual void CreateWizardPages(StatsgenWizard *wizard);


	private:
		GUIWizardPagesServers		pageServers;
		GUIWizardPagesWebsite		pageWebsite;
		GUIWizardPagesMessaging		pageMessaging;
		GUIWizardPagesImagePacks	pageImagePacks;
		GUIWizardPageRun			*pageTestRun;

};

class GUIWizardPageTemplatePacks : public GUIWizardPageStatsgen
{
	public:
		GUIWizardPageTemplatePacks(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID);
		virtual ~GUIWizardPageTemplatePacks();
		virtual wxSize ResizeControl(int x,int y,int width,bool resize);
		void OnButtonPressed(wxCommandEvent &event);
		virtual wxString GetValue();

	private:
		wxScrolledWindow	*imagesCtrl;
		wxButton			*testButton;
		ProgressPanel		*progressPanel;
		StatsgenWeb			statsgenSite;

	private:
		void CreateTemplatePackPanels();
		void SelectTemplatePackPanel(wxString &serverType);
		void SelectTemplatePackPanels();
		void ResizeTemplatePackPanels();

	private:
		wxArrayString				fileDescriptions;
		wxArrayString				filePaths;
		wxArrayString				secondFilePaths;
		wxArrayString				templateFilenames;
		wxArrayString				serverTypes;
		wxArrayString				upgradePaths;
		wxArrayString				thumbnails;
		wxArrayPtrVoid				templatePackPanels;

	DECLARE_EVENT_TABLE();
};

class GUIWizardPagesTemplatePacks : public GUIWizardPages
{
	public:
		GUIWizardPagesTemplatePacks();
		virtual ~GUIWizardPagesTemplatePacks();
		virtual void CreateWizardPages(StatsgenWizard *wizard);
		void ResizePages();

	private:
		GUIWizardPageTemplatePacks		*pageTemplatePacks;


};

class TemplatePackPanel : wxPanel
{
	public:
		TemplatePackPanel(wxWindow *parent,
						wxString &fileDescriptionIn,
						wxString &filePathIn,
						wxString &serverTypeIn,
						wxString &secondPathIn,
						wxString &upgradePathIn,
						wxString &templateFilenameIn,
						wxString &thumbnailIn);
		virtual ~TemplatePackPanel();

		wxString GetServerType();
		wxString GetDescription();
		wxString GetFilePath();
		wxString GetUpgradePath();
		wxString GetSecondFilePath();
		wxString GetTemplateFilename();
		void SelectForUpload();
		bool Selected();
		wxSize ResizeControl(int x,int y,int width,int height,bool resize);

	private:
		wxString		fileDescription;
		wxString		filePath;
		wxString		upgradePath;
		wxString		secondFilePath;
		wxString		templateFilename;
		wxString		serverType;
		wxString		thumbnail;
		wxCheckBox		*checkCtrl;
		wxStaticText	*descriptionCtrl;
		wxImage			thumbnailImage;
		ImagePanel		*thumbnailPanel;
};

#endif
