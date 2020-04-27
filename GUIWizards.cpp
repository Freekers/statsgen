#include "StaticFunctions.h"
#include "GUIWizards.h"
#include "GlobalStatistics.h"
#include "RemoteMachine.h"
#include "GenericOKCancelDialog.h"
#include "FTPBrowserPanel.h"
#include "Progress.h"
#include "StatsgenWeb.h"
#include "RestartingFTP.h"

BEGIN_EVENT_TABLE(GUIWizardPageConfigText,GUIWizardPageConfig)
	EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GUIWizardPageConfig::OnValueChanged)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageConfigChoice,GUIWizardPageConfig)
	EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GUIWizardPageConfig::OnValueChanged)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageConfigChoice::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageConfigFile,GUIWizardPageConfig)
	EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GUIWizardPageConfigFile::OnValueChanged)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageConfigFile::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageConfigWildcard,GUIWizardPageConfig)
	EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GUIWizardPageConfigWildcard::OnValueChanged)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageConfigWildcard::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageConfigRemoteDirectory,GUIWizardPageConfig)
	EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GUIWizardPageConfigRemoteDirectory::OnValueChanged)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageConfigRemoteDirectory::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageConfigBoolean,GUIWizardPageConfig)
	EVT_CHECKBOX(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GUIWizardPageConfig::OnValueChanged)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageRemoteTest,GUIWizardPageStatsgen)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageRemoteTest::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageRun,GUIWizardPageStatsgen)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageRun::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageImagePacks,GUIWizardPageStatsgen)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageImagePacks::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageTemplatePacks,GUIWizardPageStatsgen)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageTemplatePacks::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageRemoteFileTest,GUIWizardPageStatsgen)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageRemoteFileTest::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageRemoteDirectoryTest,GUIWizardPageStatsgen)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageRemoteDirectoryTest::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GUIWizardPageChoice,GUIWizardPageStatsgen)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GUIWizardPageChoice::OnButtonPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(StatsgenWizard,wxWizard)
	EVT_WIZARD_PAGE_CHANGED(WINDOW_ID_WIZARD,StatsgenWizard::OnWizardPageChanged)
	EVT_WIZARD_FINISHED(WINDOW_ID_WIZARD,StatsgenWizard::OnWizardFinish)
	EVT_WIZARD_FINISHED(WINDOW_ID_WIZARD,StatsgenWizard::OnWizardCancel)
END_EVENT_TABLE()

GUIWizardPageStatsgen::GUIWizardPageStatsgen(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : wxWizardPage(wizardIn)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageStatsgen","GUIWizardPageStatsgen")
	wxFont		font;
	mPreviousPage	= NULL;
	mNextPage		= NULL;
	mHelpTextCtrl	= NULL;
	mSectionTitle	= NULL;
	mInstallProgress		= NULL;
	mHelpText		= "";
	mCallBack		= NULL;
	mCallBackObject	= NULL;

	mWizard=wizardIn;

	mHelpTextCtrl	= new wxRichTextCtrl(this);
	mSectionTitle	= new wxStaticText(this,wxID_ANY,"");
	font = mSectionTitle->GetFont();
	font.SetPointSize(3 * font.GetPointSize());
	font.SetWeight(wxFONTWEIGHT_BOLD);
	
	mSectionTitle->SetFont(font);
	
	mInstallProgress		= new wxGauge(this,-1,30);
	mHelpTextCtrl->SetEditable(false);
	mPageID=parentPageID;
	mPageID+=thisPageID;

	mContentsSizer	= new wxBoxSizer(wxVERTICAL);
	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(mSectionTitle,0,wxEXPAND);
	mMainSizer->Add(mContentsSizer,4,wxEXPAND);
	mMainSizer->Add(mHelpTextCtrl,1,wxEXPAND);

	mMainSizer->Add(mInstallProgress,0,wxEXPAND);

	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);

	wxSizer *pageSizer;
	//pageSizer = mWizard->GetPageAreaSizer();
	//pageSizer->Add(this);
	STATSGEN_DEBUG_FUNCTION_END
}


GUIWizardPageStatsgen::~GUIWizardPageStatsgen()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageStatsgen","~GUIWizardPageStatsgen")
	STATSGEN_DEBUG_FUNCTION_END
}

wxString GUIWizardPageStatsgen::GetPageID()
{
	return (mPageID);
}

void GUIWizardPageStatsgen::SetSectionTitle(const char *titleChars)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageStatsgen","SetLabel")
	mSectionTitle->SetLabel(titleChars);
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageStatsgen::SetCallBack(void(* callBackIn)(void *object),void *callBackObjectIn)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageStatsgen","SetCallback")
	mCallBack		= callBackIn;
	mCallBackObject	= callBackObjectIn;
	STATSGEN_DEBUG_FUNCTION_END
}

wxWizardPage *GUIWizardPageStatsgen::GetPrev() const
{
	return (mPreviousPage);
}

wxWizardPage *GUIWizardPageStatsgen::GetNext() const
{
	return (mNextPage);
}

void GUIWizardPageStatsgen::SetNextPage(wxWizardPage *page)
{
	mNextPage		= page;
}

void GUIWizardPageStatsgen::SetPreviousPage(wxWizardPage *page)
{
	mPreviousPage	= page;
}

void GUIWizardPageStatsgen::SetHelp(const char *helpTextIn)
{
	mHelpText = helpTextIn;
	UpdateScreen();
}

void GUIWizardPageStatsgen::UpdateScreen()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageStatsgen","UpdateScreen")
	if (mHelpTextCtrl!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating controls");
		wxString		workingString;
		wxString		textSection;
		wxString		codeSection;
		bool			startSection;
		char			code;

		workingString	=mHelpText;
		mHelpTextCtrl->Clear();
		mHelpTextCtrl->BeginFontSize(15);

		while (workingString.Length()>0)
		{
			textSection=workingString.BeforeFirst('[');
			mHelpTextCtrl->AppendText(textSection);

			codeSection=workingString.AfterFirst('[');
			if (codeSection.StartsWith("/"))
			{
				startSection=false;
				codeSection=codeSection.AfterFirst('/');
			}
			else
			{
				startSection=true;
			}
			code='\0';
			if (codeSection.Length()>0)
			{
				code=codeSection[0];
			}
			switch (code)
			{
				case 'b':
				case 'B':
					if (startSection)
					{
						mHelpTextCtrl->BeginBold();
					}
					else
					{
						mHelpTextCtrl->EndBold();
					}
					break;
				case 'i':
				case 'I':
					if (startSection)
					{
						mHelpTextCtrl->BeginItalic();
					}
					else
					{
						mHelpTextCtrl->EndItalic();
					}
					break;
			}
			workingString=codeSection.AfterFirst(']');
		}
		//helpTextCtrl->SetValue(helpText);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageStatsgen::SetPageCounter(int pageIndexIn,int pageCountIn)
{
	mPageIndex			= pageIndexIn;
	mWizardPageCount	= pageCountIn;

	mInstallProgress->SetRange(mWizardPageCount);
}

void GUIWizardPageStatsgen::UpdateProgress()
{
	mInstallProgress->SetValue(mPageIndex+1);
}

bool GUIWizardPageStatsgen::AutoAdvance()
{
	return false;
}

void GUIWizardPageStatsgen::PageContentsChanged()
{
	if (mWizard!=NULL)
	{
		mWizard->PageContentsChanged();
	}
}

GUIWizardPageNull::GUIWizardPageNull(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
}

GUIWizardPageNull::~GUIWizardPageNull()
{
}

bool GUIWizardPageNull::AutoAdvance()
{
	return true;
}

wxString GUIWizardPageNull::GetValue()
{
	wxString	value="";
	return (value);
}

GUIWizardPageConfig::GUIWizardPageConfig(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","GUIWizardPageConfig")
	mTitleCtrl		= NULL;
	mConfigKey		= "";
	mDefaultValue	= "";
	mTitle			= "";
	AllowCtrlUpdates();

	mTitleCtrl		= new wxStaticText(this,wxID_ANY,mTitle);
	mContentsSizer->Add(mTitleCtrl,0,wxEXPAND);
	STATSGEN_DEBUG_FUNCTION_END
}

GUIWizardPageConfig::~GUIWizardPageConfig()
{
}

bool GUIWizardPageConfig::ConfigReady()
{
	if (mConfigKey.Length()>0)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

void GUIWizardPageConfig::SetDefault(const char *defaultValueIn)
{
	if (defaultValueIn==NULL)
	{
		mDefaultValue = "";
	}
	else
	{
		mDefaultValue = defaultValueIn;
	}
}

void GUIWizardPageConfig::SetConfigKey(wxString &configKeyIn,const char *defaultValueIn)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","SetConfigKey")
	mConfigKey = configKeyIn;
	SetDefault(defaultValueIn);
	UpdateScreen();
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfig::SetConfigTitle(const char *titleIn)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","SetConfigTitle")
	mTitle=titleIn;
	UpdateScreen();
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfig::VetoCtrlUpdates()
{
	mVetoUpdates	=true;
}

void GUIWizardPageConfig::AllowCtrlUpdates()
{
	mVetoUpdates	=false;
}

bool GUIWizardPageConfig::CtrlUpdatesAllowed()
{
	return (mVetoUpdates);
}

void GUIWizardPageConfig::OnValueChanged(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","OnValueChanged")
	if (ScreenReady() && ConfigReady() && !CtrlUpdatesAllowed())
	{
		SetValueFromCtrl();
		UpdateConfigFromValue();
		PageContentsChanged();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfig::UpdateConfigFromValue()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","UpdateConfigFromValue")
	if (ConfigReady())
	{
		globalStatistics.configData.WriteTextValue(mConfigKey,mValue);
		if (mCallBack!=NULL)
		{
			mCallBack(mCallBackObject);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfig::UpdateValueFromConfig()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","UpdateValueFromConfig")
	if (ConfigReady())
	{
		globalStatistics.configData.ReadTextValue(mConfigKey,&mValue,mDefaultValue);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfig::UpdateCtrlFromValue()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","UpdateCtrlFromValue")
	if (ScreenReady() && ConfigReady())
	{
		UpdateValueFromConfig();
		VetoCtrlUpdates();
		SetCtrlFromValue();
		AllowCtrlUpdates();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfig::UpdateScreen()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfig","UpdateScreen")
	if (mTitleCtrl!=NULL)
	{
		mTitleCtrl->SetLabel(mTitle);
	}
	UpdateCtrlFromValue();
	GUIWizardPageStatsgen::UpdateScreen();
	STATSGEN_DEBUG_FUNCTION_END
}

wxString GUIWizardPageConfig::GetValue()
{
	//UpdateValueFromConfig();
	return (mValue);
}

GUIWizardPageConfigText::GUIWizardPageConfigText(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfigText","GUIWizardPageConfigText")
	mValueCtrl = NULL;
	CreateControl();
	STATSGEN_DEBUG_FUNCTION_END
}

GUIWizardPageConfigText::~GUIWizardPageConfigText()
{
	mValueCtrl = NULL;
}
void GUIWizardPageConfigText::CreateControl()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfigText","CreateControl")
	mValueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	mContentsSizer->Add(mValueCtrl,0,wxEXPAND);
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfigText::SetCtrlFromValue()
{
	mValueCtrl->SetValue(mValue);
}

bool GUIWizardPageConfigText::ScreenReady()
{
	return (mValueCtrl!=NULL);
}

void GUIWizardPageConfigText::SetValueFromCtrl()
{
	mValue=mValueCtrl->GetValue();
}

GUIWizardPageRemoteTest::GUIWizardPageRemoteTest(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageRemoteTest","GUIWizardPageRemoteTest")
	wxSize	minSize;

	mStatusCtrl		= new wxStaticText(this,wxID_ANY,"Test Not Run");
	mErrorCtrl		= new wxRichTextCtrl(this,wxID_ANY,"");
	mErrorCtrl->SetEditable(false);
	mTestButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Connection");
	mLastConnectionResult = false;
	SetHelp("[b]Press the button[/b] to confirm FTP Access Details are correct");
	mContentsSizer->Add(mStatusCtrl,0,wxEXPAND);
	mContentsSizer->Add(mErrorCtrl,1,wxEXPAND|wxALL);
	mContentsSizer->Add(mTestButton);

	minSize = mErrorCtrl->GetSize();
	minSize.SetHeight(100);
	mErrorCtrl->SetMinSize(minSize);
	STATSGEN_DEBUG_FUNCTION_END
}

GUIWizardPageRemoteTest::~GUIWizardPageRemoteTest()
{
	mLastConnectionResult = false;
}

void GUIWizardPageRemoteTest::SetGroupPrefix(wxString &groupPrefixIn)
{
	mGroupPrefix		= groupPrefixIn;
}

bool GUIWizardPageRemoteTest::TestResult()
{
	return (mLastConnectionResult);
}

wxString GUIWizardPageRemoteTest::GetValue()
{
	wxString	retVal;

	if (TestResult())
	{
		retVal="TRUE";
	}
	else
	{
		retVal="FALSE";
	}

	return (retVal);
}

bool GUIWizardPageRemoteTest::PerformTest()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageRemoteTest","PerformTest")
	STATSGEN_DEBUG(DEBUG_ALWAYS,mGroupPrefix)
	RemoteMachine		remoteMachine(mGroupPrefix);
	RestartingFTP				ftpConnection;
	
	mLastConnectionResult	=remoteMachine.Connect(ftpConnection);

	if (mLastConnectionResult)
	{
		mStatusCtrl->SetLabel("Connection Successful");
		mErrorCtrl->SetValue("");
	}
	else
	{
		mErrorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
		mStatusCtrl->SetLabel("Failed to connect");
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (mLastConnectionResult);
}

void GUIWizardPageRemoteTest::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (mCallBack!=NULL)
	{
		mCallBack(mCallBackObject);
	}
	PageContentsChanged();
}

void GUIWizardPageRemoteTest::ResetTestResult()
{
	mStatusCtrl->SetLabel("Test Not Run");
	mErrorCtrl->SetValue("");
	mLastConnectionResult = false;
}


GUIWizardPagesRemoteMachine::GUIWizardPagesRemoteMachine()
{
	mPageHostname		= NULL;
	mPageEnabled		= NULL;
	mPageUsername		= NULL;
	mPagePassword		= NULL;
	mPageFTPPort		= NULL;
	mPagePassive		= NULL;
	mPageTest			= NULL;
	mGroupPrefix		= "";

	mCallBack			= NULL;
	mCallBackObject		= NULL;
}

GUIWizardPagesRemoteMachine::~GUIWizardPagesRemoteMachine()
{
}

void GUIWizardPagesRemoteMachine::SetCallBack(void(* callBackIn)(void *object),void *callBackObjectIn)
{
	mCallBack		= callBackIn;
	mCallBackObject	= callBackObjectIn;
}

void GUIWizardPagesRemoteMachine::SetSectionTitle(const char *titleChars)
{
	mPageHostname->SetSectionTitle(titleChars);
	mPageEnabled->SetSectionTitle(titleChars);
	mPageUsername->SetSectionTitle(titleChars);
	mPagePassword->SetSectionTitle(titleChars);
	mPageFTPPort->SetSectionTitle(titleChars);
	mPagePassive->SetSectionTitle(titleChars);
	mPageTest->SetSectionTitle(titleChars);
}

bool GUIWizardPagesRemoteMachine::IsFTPEnabled()
{
	return (mPageEnabled->GetValue().CmpNoCase("y")==0);
}

void GUIWizardPagesRemoteMachine::UpdateConfigKeys()
{
	wxString		configKey;

	if ((mPageHostname != NULL) && (mGroupPrefix.length()>0))
	{
		configKey.Printf("/%s/IPAddress",STRING_TO_CHAR(mGroupPrefix));
		mPageHostname->SetConfigKey(configKey);

		configKey.Printf("/%s/FTPUsername",STRING_TO_CHAR(mGroupPrefix));
		mPageUsername->SetConfigKey(configKey);

		configKey.Printf("/%s/FTPPassword",STRING_TO_CHAR(mGroupPrefix));
		mPagePassword->SetConfigKey(configKey);

		configKey.Printf("/%s/FTPPort",STRING_TO_CHAR(mGroupPrefix));
		mPageFTPPort->SetConfigKey(configKey,"21");

		configKey.Printf("/%s/FTPPassive",STRING_TO_CHAR(mGroupPrefix));
		mPagePassive->SetConfigKey(configKey,"y");

		configKey.Printf("/%s/FTPEnabled",STRING_TO_CHAR(mGroupPrefix));
		mPageEnabled->SetConfigKey(configKey,"y");

		mPageTest->SetGroupPrefix(mGroupPrefix);
	}
}

void GUIWizardPagesRemoteMachine::SetGroupPrefix(wxString &groupPrefixIn)
{
	mGroupPrefix = groupPrefixIn;

	UpdateConfigKeys();
}

wxString GUIWizardPagesRemoteMachine::GetEnabledPageID()
{
	return (mPageEnabled->GetPageID());
}

void GUIWizardPagesRemoteMachine::CreateWizardPages(StatsgenWizard *wizardIn)
{
	wxString	ruleEnabled;
	wxString	ruleTestPassed;

	mWizard=wizardIn;
	mPageHostname	= new GUIWizardPageConfigText(mWizard,mPageID,_T("HOSTNAME"));
	mPageEnabled	= new GUIWizardPageConfigBoolean(mWizard,mPageID,_T("FTPENABLED"));
	mPageUsername	= new GUIWizardPageConfigText(mWizard,mPageID,_T("FTPUSERNAME"));
	mPagePassword	= new GUIWizardPageConfigText(mWizard,mPageID,_T("FTPPASSWORD"));
	mPageFTPPort	= new GUIWizardPageConfigText(mWizard,mPageID,_T("FTPPORT"));
	mPagePassive	= new GUIWizardPageConfigBoolean(mWizard,mPageID,_T("FTPPASSIVE"));
	mPageTest		= new GUIWizardPageRemoteTest(mWizard,mPageID,_T("TEST"));

	mPageHostname->SetConfigTitle("Hostname");
	mPageHostname->SetHelp(
		"The [b]FTP Access hostname[/b], either in named form or in IP address form.\n"
		"Do not prefix the address with [i]http://[/i] or [i]ftp://[/i]\n"
		"Do not put the port number after the address.\n"
		"\n"
		"[b]Good examples.[/b]\n"
		"[i]ftp.myserver.com[/i]\n"
		"[i]192.168.0.1[/i]\n"
		"\n"
		"[b]Bad examples.[/b]\n"
		"[i]ftp://ftp.myserver.com[/i]\n"
		"[i]http://ftp.myserver.com[/i]\n"
		"[i]192.168.0.1:28960[/i]\n"
		);

	mPageEnabled->SetConfigTitle("Transfer Enabled?");
	mPageEnabled->SetHelp("Enable / Disable FTP Transfer to this remote machine");
	mPageEnabled->SetTrueFalseLabels("Yes", "No");

	mPageUsername->SetConfigTitle("FTP Username");
	mPageUsername->SetHelp("Enter the [b]FTP username[/b] used to access the hostname");

	mPagePassword->SetConfigTitle("FTP Password");
	mPagePassword->SetHelp("Enter the [b]FTP password[/b] used to access the hostname");

	mPageFTPPort->SetConfigTitle("FTP Port");
	mPageFTPPort->SetHelp(
			"Enter the [b]FTP port[/b] used to access the hostname\n"
			"This is typically 21"
						);

	mPagePassive->SetConfigTitle("FTP Passive Mode?");
	mPagePassive->SetHelp(
			"Enter whether in passive mode.\n"
			"This is typically On"
						);
	mPagePassive->SetTrueFalseLabels("On", "Off");


	mPageHostname->SetCallBack(ConfigChangedCallBack,this);
	mPageEnabled->SetCallBack(ConfigChangedCallBack,this);
	mPageUsername->SetCallBack(ConfigChangedCallBack,this);
	mPagePassword->SetCallBack(ConfigChangedCallBack,this);
	mPageFTPPort->SetCallBack(ConfigChangedCallBack,this);
	mPagePassive->SetCallBack(ConfigChangedCallBack,this);
	mPageTest->SetCallBack(ConnectionTestCallBack,this);

	ruleEnabled.Printf("%s=\"Y\"",STRING_TO_CHAR(mPageEnabled->GetPageID()));
	ruleTestPassed.Printf("%s=\"TRUE\"",STRING_TO_CHAR(mPageTest->GetPageID()));

	mWizard->AddPageLink(mPageHostname,	"",				"");
	mWizard->AddPageLink(mPageEnabled,	"",				"");
	mWizard->AddPageLink(mPageUsername,	ruleEnabled,	"");
	mWizard->AddPageLink(mPagePassword,	ruleEnabled,	"");
	mWizard->AddPageLink(mPageFTPPort,	ruleEnabled,	"");
	mWizard->AddPageLink(mPagePassive,	ruleEnabled,	"");
	mWizard->AddPageLink(mPageTest,		ruleEnabled,	ruleTestPassed);
}

void GUIWizardPagesRemoteMachine::ConnectionTest()
{
	//PageContentsChanged();
}

void GUIWizardPagesRemoteMachine::ConnectionTestCallBack(void *object)
{
	GUIWizardPagesRemoteMachine		*callingObject;

	callingObject = (GUIWizardPagesRemoteMachine *)object;

	callingObject->ConnectionTest();
}
void GUIWizardPagesRemoteMachine::PopCallBack()
{
	if (mCallBack!=NULL)
	{
		mCallBack(mCallBackObject);
	}
}

void GUIWizardPagesRemoteMachine::ConfigChangedCallBack(void *object)
{
	GUIWizardPagesRemoteMachine		*callingObject;

	callingObject = (GUIWizardPagesRemoteMachine *)object;

	callingObject->PopCallBack();
}

GUIWizardPageConfigBoolean::GUIWizardPageConfigBoolean(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	mValueCtrl		= FALSE;
	mTrueLabel		= "Yes";
	mFalseLabel		= "No";
	CreateControl();
}

GUIWizardPageConfigBoolean::~GUIWizardPageConfigBoolean()
{
	mValueCtrl = FALSE;
}


void GUIWizardPageConfigBoolean::SetTrueFalseLabels(const char *trueLabelIn, const char *falseLabelIn)
{
	mTrueLabel	= trueLabelIn;
	mFalseLabel	= falseLabelIn;
}

void GUIWizardPageConfigBoolean::CreateControl()
{
	mValueCtrl		= new wxCheckBox(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,mTitle);
	mContentsSizer->Add(mValueCtrl,0,wxEXPAND);
}

void GUIWizardPageConfigBoolean::UpdateLabel()
{
	bool		valueBool;
	wxString	titleStr;

	valueBool = (mValue.CmpNoCase("y")==0);
	titleStr = mTitle;
	titleStr+=" ";
	if (valueBool)
	{
		titleStr+=mTrueLabel;
	}
	else
	{
		titleStr+=mFalseLabel;
	}
	mValueCtrl->SetLabel(titleStr);
}

void GUIWizardPageConfigBoolean::SetCtrlFromValue()
{
	bool		valueBool;

	valueBool = (mValue.CmpNoCase("y")==0);
	mValueCtrl->SetValue(valueBool);
	UpdateLabel();
}

bool GUIWizardPageConfigBoolean::ScreenReady()
{
	return (mValueCtrl!=NULL);
}

void GUIWizardPageConfigBoolean::SetValueFromCtrl()
{
	if (mValueCtrl->GetValue())
	{
		mValue="y";
	}
	else
	{
		mValue="n";
	}
	UpdateLabel();
}

GUIWizardPagesServer::GUIWizardPagesServer()
{
	mPageServerType							= NULL;
	mPageLocalOrRemote						= NULL;
	mPageRCONPassword						= NULL;
	mPageRCONPort							= NULL;
	mPageMessagingEnabled					= NULL;
	mPageRemoteLatestFilename				= NULL;
	mPageRemoteLatestFilenameTest			= NULL;
	mPageRemoteLatestSecondaryFilename		= NULL;
	mPageRemoteLatestSecondaryFilenameTest	= NULL;
	mPageRemoteArchiveFilename				= NULL;
	mPageRemoteArchiveFilenameTest			= NULL;
	mPageLocalLatestFilename				= NULL;
	mPageLocalArchiveFilename				= NULL;
	mPageLocalLatestSecondaryFilename		= NULL;
	mGroupPrefix							= "";

	mLogfileLimit							= 0;

}

GUIWizardPagesServer::~GUIWizardPagesServer()
{
}

void GUIWizardPagesServer::UpdateConfigKeys()
{
	wxString		configKey;

	if ((mPageServerType != NULL) && (mGroupPrefix.length()>0))
	{
		configKey.Printf("/%s/serverType",STRING_TO_CHAR(mGroupPrefix));
		mPageServerType->SetConfigKey(configKey,SERVER_TYPE_COD4);

		configKey.Printf("/%s/localorremote",STRING_TO_CHAR(mGroupPrefix));
		mPageLocalOrRemote->SetConfigKey(configKey,WIZARD_CHOICE_HOMEPC);

		configKey.Printf("/%s/RCONPassword",STRING_TO_CHAR(mGroupPrefix));
		mPageRCONPassword->SetConfigKey(configKey);

		configKey.Printf("/%s/gamePort",STRING_TO_CHAR(mGroupPrefix));
		mPageRCONPort->SetConfigKey(configKey);

		configKey.Printf("/%s/MessagingEnabled",STRING_TO_CHAR(mGroupPrefix));
		mPageMessagingEnabled->SetConfigKey(configKey,"n");

		configKey.Printf("/%s/FTPLatest",STRING_TO_CHAR(mGroupPrefix));
		mPageRemoteLatestFilename->SetConfigKey(configKey);
		mPageRemoteLatestFilenameTest->SetGroupPrefix(mGroupPrefix);

		configKey.Printf("/%s/FTPSecondaryLatest",STRING_TO_CHAR(mGroupPrefix));
		mPageRemoteLatestSecondaryFilename->SetConfigKey(configKey);
		mPageRemoteLatestSecondaryFilenameTest->SetGroupPrefix(mGroupPrefix);

		configKey.Printf("/%s/FTPArchive",STRING_TO_CHAR(mGroupPrefix));
		mPageRemoteArchiveFilename->SetConfigKey(configKey);
		mPageRemoteArchiveFilenameTest->SetGroupPrefix(mGroupPrefix);

		configKey.Printf("/%s/latest",STRING_TO_CHAR(mGroupPrefix));
		mPageLocalLatestFilename->SetConfigKey(configKey);

		configKey.Printf("/%s/archive",STRING_TO_CHAR(mGroupPrefix));
		mPageLocalArchiveFilename->SetConfigKey(configKey);

		configKey.Printf("/%s/secondarylatest",STRING_TO_CHAR(mGroupPrefix));
		mPageLocalLatestSecondaryFilename->SetConfigKey(configKey);

		configKey.Printf("/%s/BanFile1Type",STRING_TO_CHAR(mGroupPrefix));
		globalStatistics.configData.WriteTextValue(configKey,(char *)"COD");

		configKey.Printf("/%s/BanFile2Type",STRING_TO_CHAR(mGroupPrefix));
		globalStatistics.configData.WriteTextValue(configKey,(char *)"PUNKBUSTER");

		configKey.Printf("/%s/FTPBanFile2Directory",STRING_TO_CHAR(mGroupPrefix));
		mPagePBRemoteDirectory->SetConfigKey(configKey);

		SetTestParameters();
		SetLogfileLimit();
	}
}

void GUIWizardPagesServer::SetTestParameters()
{
	wxString	file;
	wxString	directory;
	wxString	configKey;

	if (mGroupPrefix.Length()>0)
	{
		mPageRemoteLatestFilename->SplitValue(directory,file);
		mPageRemoteLatestFilenameTest->SetFile(directory,file);

		mPageRemoteLatestSecondaryFilename->SplitValue(directory,file);
		mPageRemoteLatestSecondaryFilenameTest->SetFile(directory,file);

		directory=mPageRemoteArchiveFilename->GetValue();
		file=mPageRemoteArchiveFilename->GetWildcard();
		mPageRemoteArchiveFilenameTest->SetFile(directory,file);

		mPagePBRemoteDirectoryTest->SetGroupPrefix(mGroupPrefix);

		configKey.Printf("/%s/RCONUsePunkBuster",STRING_TO_CHAR(mGroupPrefix));
		if (mPageLocalOrRemote->GetValue().Cmp(WIZARD_CHOICE_HOMEPC)==0)
		{
			globalStatistics.configData.WriteTextValue(configKey,(char *)"Y");
		}
		else
		{
			globalStatistics.configData.WriteTextValue(configKey,(char *)"N");
		}
	}
}

void GUIWizardPagesServer::SetLogfileLimit(int limit)
{
	mLogfileLimit=limit;
	SetLogfileLimit();
}

void GUIWizardPagesServer::SetLogfileLimit()
{
	wxString	configKey;
	wxString	configValue;

	if (mGroupPrefix.Length()>0)
	{
		configKey.Printf("/%s/MaxLogfileSize",STRING_TO_CHAR(mGroupPrefix));
		configValue.Printf("%d",mLogfileLimit);
		globalStatistics.configData.WriteTextValue(configKey,configValue);
	}
}

void GUIWizardPagesServer::SetGroupPrefix(wxString &groupPrefixIn)
{
	mGroupPrefix = groupPrefixIn;

	mRemoteMachine.SetGroupPrefix(mGroupPrefix);
	UpdateConfigKeys();
}

void GUIWizardPagesServer::CreateWizardPages(StatsgenWizard *wizardIn)
{
	const char *sectionTitle="Configure Server";
	wxString ruleServerTypeContinue;
	wxString ruleLocalOrRemoteContinue;
	wxString ruleLocalOrRemoteHomePC;
	wxString ruleLocalOrRemoteGameServer;
	wxString ruleRemoteLatestFilenameTestPassed;
	wxString ruleRemoteArchiveFilenameTestPassed;
	wxString ruleQuakewars;
	wxString ruleQuakewarsHomePC;
	wxString ruleQuakewarsGameServer;
	wxString ruleMOHAA;
	wxString ruleMOHAAHomePC;
	wxString ruleMOHAAGameServer;
	wxString ruleMessagingServers;
	wxString ruleMessagingEnabled;
	wxString ruleMessaging;
	wxString rulePunkbuster;
	wxString rulePunkbusterTestPassed;
	wxString ruleHomePCNotMOHAA;
	wxString ruleGameServerNotMOHAA;

	mWizard=wizardIn;

	wxArrayString	localOrRemoteAnswers;
	wxArrayString	localOrRemoteButtonTexts;
	wxArrayString	localOrRemoteButtonDescriptions;
	
	wxArrayString	serverTypeAnswers;
	wxArrayString	serverTypeButtonTexts;
	wxArrayString	serverTypeButtonDescriptions;
	wxString		archiveHelp;

	archiveHelp=
			"If you have more than one logfile, typical for MOHAA servers, you can set up "
			"a wildcard to download multiple logs.\n"
			"\n"
			"[b]examples of wildcard are[/b]\n"
			"[i]*.log[/i]\n"
			"[i]gameserver*.log[/i]\n"
			"\n"
			"It is important that logfiles are selected in the correct order, otherwise "
			"stats will produce odd results, sometimes alphabetical order, the default, "
			"is sufficient to achieve this, but at other times there may be components of "
			"a date / time in the filename which means alphabetical ordering is not correct "
			"to get around this problem you can specify special codes to indicate to "
			"statsgen which parts of the filename represent dates.\n"
			"\n"
			"e.g.\n"
			"If you are using Tatakai Server Scan to archive your logfiles they are named in the following way:-\n"
			"[i]8_12_2007_gameserver.log[/i]\n"
			"[i]9_12_2007_gameserver.log[/i]\n"
			"[i]10_12_2007_gameserver.log[/i]\n"
			"[i]11_12_2007_gameserver.log[/i]\n"
			"etc.\n"
			"\n"
			"if you just put \n"
			"[b]*gameserver.log[/b]\n"
			"as the wildcard statsgen would assume these files should be picked up, sorted alphabetically as follows\n"
			"[i]10_12_2007_gameserver.log[/i]\n"
			"[i]11_12_2007_gameserver.log[/i]\n"
			"[i]8_12_2007_gameserver.log[/i]\n"
			"[i]9_12_2007_gameserver.log[/i]\n"
			"This is obviously the wrong order as the 8th December file should be first.\n"
			"This is where the embedded codes can be used to specify date components of the filename. The codes are CAPITAL letters and surrounded by $$ symbols.\n"
			"[b]$$DAY$$[/b]		The day of the month, 1 - 31\n"
			"[b]$$0DAY$$[/b]	The day of the month, 01 - 31\n"
			"[b]$$MONTH$$[/b]	The month of the year, 1 - 12\n"
			"[b]$$0MONTH$$[/b]	The month of the year, 01 - 12\n"
			"[b]$$YEAR$$[/b]	The year, e.g. 2007\n"
			"[b]$$HOUR$$[/b]	The hour of the day, 00 - 23\n"
			"[b]$$MINUTE$$[/b]	The minute of the hour, 00 - 59\n"
			"[b]$$SECOND$$[/b]	The second of the minute, 00 - 59\n"
			"\n"
			"so ... in order to sort the order properly we need to set the wildcard as follows\n"
			"[b]$$DAY$$_$$MONTH$$_$$YEAR$$_gameserver.log[/b]\n";

	localOrRemoteAnswers.Add(WIZARD_CHOICE_HOMEPC);
	localOrRemoteAnswers.Add(WIZARD_CHOICE_GAMESERVER);
	localOrRemoteButtonTexts.Add("Home PC");
	localOrRemoteButtonTexts.Add("Game Server");
	localOrRemoteButtonDescriptions.Add("Statsgen is running on your home PC");
	localOrRemoteButtonDescriptions.Add("Statsgen is running on the game server");

	serverTypeAnswers.Add(SERVER_TYPE_COD1);
	serverTypeAnswers.Add(SERVER_TYPE_COD2);
	serverTypeAnswers.Add(SERVER_TYPE_COD4);
	serverTypeAnswers.Add(SERVER_TYPE_COD5);
	serverTypeAnswers.Add(SERVER_TYPE_MOHAA);
	serverTypeAnswers.Add(SERVER_TYPE_QUAKEWARS);
	serverTypeAnswers.Add(SERVER_TYPE_WOLFENSTEIN);
	serverTypeButtonTexts.Add(SERVER_TYPE_NAME_COD1);
	serverTypeButtonTexts.Add(SERVER_TYPE_NAME_COD2);
	serverTypeButtonTexts.Add(SERVER_TYPE_NAME_COD4);
	serverTypeButtonTexts.Add(SERVER_TYPE_NAME_COD5);
	serverTypeButtonTexts.Add(SERVER_TYPE_NAME_MOHAA);
	serverTypeButtonTexts.Add(SERVER_TYPE_NAME_QUAKEWARS);
	serverTypeButtonTexts.Add(SERVER_TYPE_NAME_WOLFENSTEIN);
	serverTypeButtonDescriptions.Add(SERVER_TYPE_NAME_COD1);
	serverTypeButtonDescriptions.Add(SERVER_TYPE_NAME_COD2);
	serverTypeButtonDescriptions.Add(SERVER_TYPE_NAME_COD4);
	serverTypeButtonDescriptions.Add(SERVER_TYPE_NAME_COD5);
	serverTypeButtonDescriptions.Add(SERVER_TYPE_NAME_MOHAA);
	serverTypeButtonDescriptions.Add(SERVER_TYPE_NAME_QUAKEWARS);
	serverTypeButtonDescriptions.Add(SERVER_TYPE_NAME_WOLFENSTEIN);
	mPageServerType					= new GUIWizardPageConfigChoice(mWizard,
											mPageID,_T("SERVERTYPE"),
											serverTypeAnswers,
											serverTypeButtonTexts,
											serverTypeButtonDescriptions);
	mPageLocalOrRemote				= new GUIWizardPageConfigChoice(mWizard,
											mPageID,_T("LOCALORREMOTE"),
											localOrRemoteAnswers,
											localOrRemoteButtonTexts,
											localOrRemoteButtonDescriptions);
	mPageRCONPassword				= new GUIWizardPageConfigText(mWizard,mPageID,_T("RCONPASSWORD"));
	mPageRCONPort					= new GUIWizardPageConfigText(mWizard,mPageID,_T("RCONPORT"));
	mPagePBRemoteDirectory			= new GUIWizardPageConfigRemoteDirectory(mWizard,mPageID,_T("PBREMOTEDIRECTORY"));
	mPagePBRemoteDirectoryTest		= new GUIWizardPageRemoteDirectoryTest(mWizard,mPageID,_T("PBREMOTEDIRECTORYTEST"));
	mPageMessagingEnabled			= new GUIWizardPageConfigBoolean(mWizard,mPageID,_T("MESSAGINGENABLED"));
	mPageMessagingEnabled			= new GUIWizardPageConfigBoolean(mWizard,mPageID,_T("MESSAGINGENABLED"));
	mPageRemoteLatestFilename		= new GUIWizardPageConfigRemoteFile(mWizard,mPageID,_T("REMOTELATESTFILENAME"));
	mPageRemoteLatestFilenameTest	= new GUIWizardPageRemoteFileTest(mWizard,mPageID,_T("REMOTELATESTFILENAMETEST"));
	mPageRemoteLatestSecondaryFilename	= new GUIWizardPageConfigRemoteFile(mWizard,mPageID,_T("REMOTELATESTSECONDARYFILENAME"));
	mPageRemoteLatestSecondaryFilenameTest	= new GUIWizardPageRemoteFileTest(mWizard,mPageID,_T("REMOTELATESTSECONDARYFILENAMETEST"));
	mPageRemoteArchiveFilename		= new GUIWizardPageConfigRemoteWildcard(mWizard,mPageID,_T("REMOTEARCHIVEFILENAME"));
	mPageRemoteArchiveFilenameTest	= new GUIWizardPageRemoteFileTest(mWizard,mPageID,_T("REMOTEARCHIVEFILENAMETEST"));
	mPageLocalLatestFilename			= new GUIWizardPageConfigLocalFile(mWizard,mPageID,_T("LOCALLATESTFILENAME"));
	mPageLocalArchiveFilename		= new GUIWizardPageConfigLocalWildcard(mWizard,mPageID,_T("LOCALARCHIVEFILENAME"));
	mPageLocalLatestFilename			= new GUIWizardPageConfigLocalFile(mWizard,mPageID,_T("LOCALLATESTFILENAME"));
	mPageLocalArchiveFilename		= new GUIWizardPageConfigLocalWildcard(mWizard,mPageID,_T("LOCALARCHIVEFILENAME"));
	mPageLocalLatestSecondaryFilename= new GUIWizardPageConfigLocalFile(mWizard,mPageID,_T("LOCALLATESTSECONDARYFILENAME"));

	mPageLocalOrRemote->SetHelp(
				"Depending on where you are running statsgen dictates various"
				" configurations.\nYou can run statsgen on the [b]game server[/b]"
				" itself, or if that is not available you can run statsgen"
				" from your [b]home pc[/b]."
				);
	mPageLocalOrRemote->SetConfigTitle("Statsgen Location");

	mPageServerType->SetConfigTitle("Game Server Type");
	mPageServerType->SetHelp(
		"What sort of Server is this\n"
		);

	mPageRCONPassword->SetConfigTitle("Game Server RCON Password?");
	mPageRCONPassword->SetHelp("Enter the servers [b]RCON Password[/b].\n"
						"This is used to send messages to the game server.");

	mPagePBRemoteDirectory->SetConfigTitle("Punkbuster Remote Directory");
	mPagePBRemoteDirectory->SetHelp("Enter the location on your game server of the [b]Punkbuster Directory[/b]");

	mPagePBRemoteDirectoryTest->SetHelp("[b]Press the button[/b] to test the Punkbuster Directory");

	mPageRCONPort->SetConfigTitle("Game Server Port?");
	mPageRCONPort->SetHelp("Enter the Game Servers Port Number.\n"
						"e.g.\n28960");

	mPageMessagingEnabled->SetConfigTitle("Game Server Messaging Enabled?");
	mPageMessagingEnabled->SetHelp("Do you want to send stats messages to this paticular game server?");
	mPageMessagingEnabled->SetTrueFalseLabels("Yes", "No");

	mPageRemoteLatestFilename->SetConfigTitle("Kills Logfile");
	mPageRemoteLatestFilename->SetHelp(
			"The logfile on the game server where the [b]kills[/b] are stored, typically on COD"
			" games this is called [b]games_mp.log[/b]."
						);
	mPageRemoteLatestFilenameTest->SetHelp(
			"[b]Push the button[/b] to ensure Kills logfile can be accessed."
						);

	mPageRemoteLatestSecondaryFilename->SetConfigTitle("QuakeWars Objective Logfile");
	mPageRemoteLatestSecondaryFilename->SetHelp(
			"The logfile on the game server where the [b]objectives[/b] are stored"
						);
	mPageRemoteLatestSecondaryFilenameTest->SetHelp(
			"[b]Push the button[/b] to ensure objective logfile can be accessed.");

	mPageLocalLatestFilename->SetConfigTitle("Local Logfile Name");
	mPageLocalLatestFilename->SetHelp(
			"The logfile on the game server where the [b]kills[/b] are stored, typically on COD"
			" games this is called [b]games_mp.log[/b]."
						);

	mPageLocalLatestSecondaryFilename->SetConfigTitle("Quakewars Objective Logfile Name");
	mPageLocalLatestSecondaryFilename->SetHelp(
			"The logfile on the game server where the [b]objectives[/b] are stored"
						);
	mPageLocalArchiveFilename->SetConfigTitle("Multiple Kill File Wildcard");
	mPageLocalArchiveFilename->SetHelp(archiveHelp);
	mPageRemoteArchiveFilename->SetConfigTitle("Multiple Kill File Wildcard");
	mPageRemoteArchiveFilename->SetHelp(archiveHelp);
	mPageRemoteArchiveFilenameTest->SetHelp("[b]Push the button[/b] to check that the logfiles are accessible");

	mPageServerType->SetCallBack(ConfigChangedCallBack,this);
	mPageLocalOrRemote->SetCallBack(ConfigChangedCallBack,this);
	mPageRCONPassword->SetCallBack(ConfigChangedCallBack,this);
	mPagePBRemoteDirectory->SetCallBack(ConfigChangedCallBack,this);
	mPagePBRemoteDirectoryTest->SetCallBack(ConfigChangedCallBack,this);
	mPageRCONPort->SetCallBack(ConfigChangedCallBack,this);
	mPageMessagingEnabled->SetCallBack(ConfigChangedCallBack,this);
	mPageRemoteLatestFilename->SetCallBack(ConfigChangedCallBack,this);
	mPageRemoteLatestFilenameTest->SetCallBack(ConfigChangedCallBack,this);
	mPageRemoteLatestSecondaryFilename->SetCallBack(ConfigChangedCallBack,this);
	mPageRemoteLatestSecondaryFilenameTest->SetCallBack(ConfigChangedCallBack,this);
	mPageRemoteArchiveFilename->SetCallBack(ConfigChangedCallBack,this);
	mPageRemoteArchiveFilenameTest->SetCallBack(ConfigChangedCallBack,this);
	mPageLocalLatestFilename->SetCallBack(ConfigChangedCallBack,this);
	mPageLocalArchiveFilename->SetCallBack(ConfigChangedCallBack,this);
	mPageLocalLatestSecondaryFilename->SetCallBack(ConfigChangedCallBack,this);



	ruleServerTypeContinue.Printf("%s!\"\"",STRING_TO_CHAR(mPageServerType->GetPageID()));
	ruleLocalOrRemoteContinue.Printf("%s!\"\"",STRING_TO_CHAR(mPageLocalOrRemote->GetPageID()));
	ruleLocalOrRemoteHomePC.Printf("%s=\"%s\"",STRING_TO_CHAR(mPageLocalOrRemote->GetPageID()),WIZARD_CHOICE_HOMEPC);
	ruleLocalOrRemoteGameServer.Printf("%s=\"%s\"",STRING_TO_CHAR(mPageLocalOrRemote->GetPageID()),WIZARD_CHOICE_GAMESERVER);

	ruleRemoteLatestFilenameTestPassed.Printf("%s=\"TRUE\"",STRING_TO_CHAR(mPageRemoteLatestFilenameTest->GetPageID()));
	ruleRemoteArchiveFilenameTestPassed.Printf("%s=\"TRUE\"",STRING_TO_CHAR(mPageRemoteArchiveFilenameTest->GetPageID()));
	rulePunkbusterTestPassed.Printf("%s=\"TRUE\"",STRING_TO_CHAR(mPagePBRemoteDirectoryTest->GetPageID()));

	ruleQuakewars.Printf("%s=\"%s\"",STRING_TO_CHAR(mPageServerType->GetPageID()),SERVER_TYPE_QUAKEWARS);
	ruleQuakewarsHomePC.Printf("(%s)&(%s)",STRING_TO_CHAR(ruleLocalOrRemoteHomePC),STRING_TO_CHAR(ruleQuakewars));
	ruleQuakewarsGameServer.Printf("(%s)&(%s)",STRING_TO_CHAR(ruleLocalOrRemoteGameServer),STRING_TO_CHAR(ruleQuakewars));
	ruleMOHAA.Printf("%s=\"%s\"",STRING_TO_CHAR(mPageServerType->GetPageID()),SERVER_TYPE_MOHAA);
	ruleMOHAAHomePC.Printf("(%s)&(%s)",STRING_TO_CHAR(ruleLocalOrRemoteHomePC),STRING_TO_CHAR(ruleMOHAA));
	ruleMOHAAGameServer.Printf("(%s)&(%s)",STRING_TO_CHAR(ruleLocalOrRemoteGameServer),STRING_TO_CHAR(ruleMOHAA));
	ruleMessagingServers.Printf("((%s=\"%s\")|(%s=\"%s\")|(%s=\"%s\")|(%s=\"%s\")|(%s=\"%s\"))",
			STRING_TO_CHAR(mPageServerType->GetPageID()),SERVER_TYPE_COD1,
			STRING_TO_CHAR(mPageServerType->GetPageID()),SERVER_TYPE_COD2,
			STRING_TO_CHAR(mPageServerType->GetPageID()),SERVER_TYPE_COD4,
			STRING_TO_CHAR(mPageServerType->GetPageID()),SERVER_TYPE_COD5,
			STRING_TO_CHAR(mPageServerType->GetPageID()),SERVER_TYPE_QUAKEWARS);
	ruleMessagingEnabled.Printf("(%s=\"%s\")", STRING_TO_CHAR(mPageMessagingEnabled->GetPageID()),"Y");
	ruleMessaging.Printf("%s&%s",STRING_TO_CHAR(ruleMessagingServers),STRING_TO_CHAR(ruleMessagingEnabled));
	rulePunkbuster.Printf("(%s)&(%s)",STRING_TO_CHAR(ruleMessaging),STRING_TO_CHAR(ruleLocalOrRemoteHomePC));
	ruleHomePCNotMOHAA.Printf("(%s)&(%s!\"%s\")",
								STRING_TO_CHAR(ruleLocalOrRemoteHomePC),
								STRING_TO_CHAR(mPageServerType->GetPageID()),
								SERVER_TYPE_MOHAA);
	ruleGameServerNotMOHAA.Printf("(%s)&(%s!\"%s\")",
								STRING_TO_CHAR(ruleLocalOrRemoteGameServer),
								STRING_TO_CHAR(mPageServerType->GetPageID()),
								SERVER_TYPE_MOHAA);

	mWizard->AddPageLink(mPageServerType,						""						,ruleServerTypeContinue);
	mWizard->AddPageLink(mPageLocalOrRemote,					""						,ruleLocalOrRemoteContinue);
	mRemoteMachine.SetPageID(mPageID,"REMOTEMACHINE");
	mRemoteMachine.CreateWizardPages(mWizard);
	mWizard->AddPageLink(mPageRCONPort,							""						,"");
	mWizard->AddPageLink(mPageMessagingEnabled,					ruleMessagingServers	,"");
	mWizard->AddPageLink(mPageRCONPassword,						ruleMessaging			,"");
	mWizard->AddPageLink(mPagePBRemoteDirectory,				rulePunkbuster			,"");
	mWizard->AddPageLink(mPagePBRemoteDirectoryTest,			rulePunkbuster			,rulePunkbusterTestPassed);
	mWizard->AddPageLink(mPageRemoteLatestFilename,				ruleHomePCNotMOHAA		,"");
	mWizard->AddPageLink(mPageRemoteLatestFilenameTest,			ruleHomePCNotMOHAA		,ruleRemoteLatestFilenameTestPassed);
	mWizard->AddPageLink(mPageRemoteLatestSecondaryFilename,	ruleQuakewarsHomePC		,"");
	mWizard->AddPageLink(mPageRemoteLatestSecondaryFilenameTest,ruleQuakewarsHomePC		,ruleRemoteLatestFilenameTestPassed);
	mWizard->AddPageLink(mPageRemoteArchiveFilename,			ruleMOHAAHomePC			,"");
	mWizard->AddPageLink(mPageRemoteArchiveFilenameTest,		ruleMOHAAHomePC			,ruleRemoteArchiveFilenameTestPassed);
	mWizard->AddPageLink(mPageLocalLatestFilename,				ruleGameServerNotMOHAA	,"");
	mWizard->AddPageLink(mPageLocalArchiveFilename,				ruleMOHAAGameServer		,"");
	mWizard->AddPageLink(mPageLocalLatestSecondaryFilename,		ruleQuakewarsGameServer	,"");

	/*
	wizard->AddPageLink(pageHostname,"","");
	wizard->AddPageLink(pageEnabled,"","");
	wizard->AddPageLink(pageUsername,"FTPENABLED=\"TRUE\"","");
	wizard->AddPageLink(pagePassword,"FTPENABLED=\"TRUE\"","");
	wizard->AddPageLink(pageFTPPort,"FTPENABLED=\"TRUE\"","");
	wizard->AddPageLink(pagePassive,"FTPENABLED=\"TRUE\"","");
	wizard->AddPageLink(pageTest,"FTPENABLED=\"TRUE\"","TRUE");
	wizard->AddPageLink(pageFinished,"","");
	*/
	SetSectionTitle(sectionTitle);

}

void GUIWizardPagesServer::SetSectionTitle(const char *sectionTitle)
{
	mPageServerType->SetSectionTitle(sectionTitle);
	mPageLocalOrRemote->SetSectionTitle(sectionTitle);
	mPageRCONPassword->SetSectionTitle(sectionTitle);
	mPageRCONPort->SetSectionTitle(sectionTitle);
	mPagePBRemoteDirectory->SetSectionTitle(sectionTitle);
	mPagePBRemoteDirectoryTest->SetSectionTitle(sectionTitle);
	mPageMessagingEnabled->SetSectionTitle(sectionTitle);
	mPageRemoteLatestFilename->SetSectionTitle(sectionTitle);
	mPageRemoteLatestFilenameTest->SetSectionTitle(sectionTitle);
	mPageRemoteLatestSecondaryFilename->SetSectionTitle(sectionTitle);
	mPageRemoteLatestSecondaryFilenameTest->SetSectionTitle(sectionTitle);
	mPageLocalLatestFilename->SetSectionTitle(sectionTitle);
	mPageLocalLatestSecondaryFilename->SetSectionTitle(sectionTitle);
	mPageRemoteArchiveFilename->SetSectionTitle(sectionTitle);
	mPageRemoteArchiveFilenameTest->SetSectionTitle(sectionTitle);
	mPageLocalArchiveFilename->SetSectionTitle(sectionTitle);
	mRemoteMachine.SetSectionTitle(sectionTitle);
}

void GUIWizardPagesServer::PrepareServerConfig()
{
	wxFileName						baseDirectory(wxGetApp().argv[0]);
	wxFileName						logDirectory;
	wxFileName						archiveDirectory;
	wxFileName						banDirectory;
	wxString						logDirectoryName;
	wxString						archiveDirectoryName;
	wxString						banDirectoryName;
	wxString						baseDirectoryName;

	baseDirectory.AppendDir(mGroupPrefix);
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

	logDirectoryName+=logDirectory.GetPathSeparator();
	banDirectoryName+=banDirectory.GetPathSeparator();
	archiveDirectoryName+=archiveDirectory.GetPathSeparator();

	mPageLocalLatestFilename->SetDefault(logDirectoryName);
	mPageLocalLatestFilename->UpdateValueFromConfig();
	mPageLocalLatestSecondaryFilename->SetDefault(logDirectoryName);
	mPageLocalLatestSecondaryFilename->UpdateValueFromConfig();
	mPageLocalArchiveFilename->SetDefault(archiveDirectoryName);
	mPageLocalArchiveFilename->UpdateValueFromConfig();
}

void GUIWizardPagesServer::ConfigChangedCallBack(void *object)
{
	GUIWizardPagesServer	*serverWizard;
	serverWizard = (GUIWizardPagesServer *)object;
	serverWizard->SetTestParameters();
}

GUIWizardPageChoice::GUIWizardPageChoice(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID,
						wxArrayString &answerCodesIn,
						wxArrayString &buttonTextsIn,
						wxArrayString &buttonDescriptionsIn) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageChoice","GUIWizardPageChoice")
	mSelectionLabel		= NULL;
	mSelectedChoice		= NULL;
	mChoice				= "";
	mChoiceSizer		= NULL;

	UpdateChoices(answerCodesIn,buttonTextsIn,buttonDescriptionsIn);
	STATSGEN_DEBUG_FUNCTION_END
}

GUIWizardPageChoice::~GUIWizardPageChoice()
{
}

void GUIWizardPageChoice::UpdateChoices(
						wxArrayString &answerCodesIn,
						wxArrayString &buttonTextsIn,
						wxArrayString &buttonDescriptionsIn)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageChoice","UpdateChoices")
	int				buttonCount;
	int				buttonIndex;
	wxButton		*button;
	wxStaticText	*description;
	int				answerCount;
	int				answerIndex;
	wxString		answerCode;
	wxString		buttonText;
	wxString		buttonDescription;

	if (mChoiceSizer == NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating sizer")
		mChoiceSizer = new wxBoxSizer(wxVERTICAL);
		mContentsSizer->Add(mChoiceSizer,0,wxEXPAND);
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Clearing sizer")
		mChoiceSizer->Clear();
	}
	// Remove existing stuff
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"deleting existing buttons")
	buttonCount=mButtons.GetCount();
	for (buttonIndex=0;buttonIndex<buttonCount;buttonIndex++)
	{
		button		= (wxButton *)mButtons.Item(buttonIndex);
		description	= (wxStaticText *)mLabels.Item(buttonIndex);
		button->Destroy();
		description->Destroy();
	}
	mButtons.Clear();
	mLabels.Clear();

	// Create new buttons and labels
	mAnswerCodes 		= answerCodesIn;
	mButtonTexts			= buttonTextsIn;
	mButtonDescriptions	= buttonDescriptionsIn;

	answerCount = mAnswerCodes.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating buttons")
	wxBoxSizer		*choiceButtonsSizer;
	choiceButtonsSizer = new wxBoxSizer(wxVERTICAL);
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		answerCode			= mAnswerCodes.Item(answerIndex);
		buttonText			= mButtonTexts.Item(answerIndex);
		buttonDescription	= mButtonDescriptions.Item(answerIndex);
		button				= new wxButton(this,
										WINDOW_ID_BUTTON_NEW,
										buttonText);
		description			= new wxStaticText(this, wxID_ANY, buttonDescription);
		mButtons.Add(button);
		mLabels.Add(description);

		wxBoxSizer			*buttonSizer;

		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating button sizer")
		buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonSizer->Add(button,1,wxEXPAND);
		buttonSizer->Add(description,1,wxEXPAND);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding button sizer")
		choiceButtonsSizer->Add(buttonSizer,0,wxEXPAND);
	}
	mChoiceSizer->Add(choiceButtonsSizer,0,wxEXPAND);
	if (mSelectedChoice == NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating selected choice")
		mSelectedChoice	= new wxStaticText(this, wxID_ANY, "");
		mSelectionLabel	= new wxStaticText(this,wxID_ANY,"Current Selection: ");
	}

	SetSelection(WIZARD_SELECTION_NOTHING);
	if (mAnswerCodes.GetCount()>0)
	{
		mChoice = mButtonTexts.Item(0);
		SetSelection(mChoice);
		mChoice = mAnswerCodes.Item(0);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating selected sizer")
	wxBoxSizer	*selectedSizer;
	selectedSizer = new wxBoxSizer(wxHORIZONTAL);
	selectedSizer->Add(mSelectionLabel,0,wxEXPAND);
	selectedSizer->Add(mSelectedChoice,0,wxEXPAND);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding selected sizer")
	mChoiceSizer->Add(selectedSizer,0,wxEXPAND);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Fit")
	Layout();
	PostSizeEventToParent();

	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageChoice::SetSelection(const char *selection)
{
	wxString	selectionStr;

	selectionStr=selection;
	SetSelection(selectionStr);
}

void GUIWizardPageChoice::SetSelection(wxString &selection)
{
	mSelectedChoice->SetLabel(selection);
}

void GUIWizardPageChoice::OnButtonPressed(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageChoice","OnButtonPressed")
	int			answerCount;
	int			answerIndex;
	wxButton	*button=NULL;
	wxString	selection;
	

	answerCount = mButtons.GetCount();

	mChoice = "";
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		button=(wxButton *)mButtons.Item(answerIndex);
		if (button == event.GetEventObject())
		{
			mChoice = mAnswerCodes.Item(answerIndex);
			selection = mButtonTexts.Item(answerIndex);
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Setting selection")
			SetSelection(selection);
			break;
		}
	}
	if (mCallBack!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"calling callback")
		mCallBack(mCallBackObject);
	}
	PageContentsChanged();

	STATSGEN_DEBUG_FUNCTION_END
}

wxString GUIWizardPageChoice::GetChoice()
{
	return (mChoice);
}

wxString GUIWizardPageChoice::GetValue()
{
	return (GetChoice());
}

void GUIWizardPageChoice::SelectChoice(wxString &choiceIn)
{
	int			answerCount;
	int			answerIndex;
	wxButton	*button=NULL;
	wxString	selection;

	answerCount = mButtons.GetCount();
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		button=(wxButton *)mButtons.Item(answerIndex);
		mChoice = mAnswerCodes.Item(answerIndex);
		if (mChoice.CmpNoCase(choiceIn)==0)
		{
			selection = mButtonTexts.Item(answerIndex);
			SetSelection(selection);
			break;
		}
		mChoice = "";
	}
	if (mCallBack!=NULL)
	{
		mCallBack(mCallBackObject);
	}
}

GUIWizardPageConfigFile::GUIWizardPageConfigFile(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	mValueCtrl		= NULL;
	mBrowseButton	= NULL;
	CreateControl();
}

GUIWizardPageConfigFile::~GUIWizardPageConfigFile()
{
	mValueCtrl		= NULL;
	mBrowseButton	= NULL;
}

void GUIWizardPageConfigFile::CreateControl()
{
	mValueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	mBrowseButton	= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Browse For File");
	mContentsSizer->Add(mBrowseButton);
	mContentsSizer->Add(mValueCtrl,0,wxEXPAND);
}

void GUIWizardPageConfigFile::SetCtrlFromValue()
{
	mValueCtrl->SetValue(mValue);
}

bool GUIWizardPageConfigFile::ScreenReady()
{
	return (mValueCtrl!=NULL);
}

void GUIWizardPageConfigFile::SetValueFromCtrl()
{
	mValue = mValueCtrl->GetValue();
}

void GUIWizardPageConfigFile::GetConfigKeys(wxString &configKeyDirectory,
													wxString &configKeyFilename)
{
	configKeyDirectory	= mConfigKey + "Directory";
	configKeyFilename	= mConfigKey + "Filename";
}

void GUIWizardPageConfigFile::SplitValue(wxString &valueDirectory,
													wxString &valueFilename)
{
	wxFileName	filename(mValue,wxPATH_UNIX);

	valueFilename	= filename.GetFullName();
	valueDirectory	= filename.GetPath();
	valueDirectory.Replace("\\","/");
}

void GUIWizardPageConfigFile::OnButtonPressed(wxCommandEvent &event)
{
	wxString userFilename;

	userFilename=RequestFileFromUser();
	if (userFilename.Length()>0)
	{
		mValue=userFilename;
		SetCtrlFromValue();
	}
	PageContentsChanged();
}

void GUIWizardPageConfigFile::UpdateConfigFromValue()
{
	wxString configKeyDirectory;
	wxString configKeyFilename;

	if (ConfigReady())
	{
		GetConfigKeys(configKeyDirectory,configKeyFilename);
		SplitValue(mDirectory,mFilename);
		globalStatistics.configData.WriteTextValue(configKeyDirectory,mDirectory);
		globalStatistics.configData.WriteTextValue(configKeyFilename,mFilename);
		if (mCallBack!=NULL)
		{
			mCallBack(mCallBackObject);
		}
	}
}

void GUIWizardPageConfigFile::UpdateValueFromConfig()
{
	wxString 	configKeyDirectory;
	wxString 	configKeyFilename;

	if (ConfigReady())
	{
		GetConfigKeys(configKeyDirectory,configKeyFilename);
		globalStatistics.configData.ReadTextValue(configKeyDirectory,&mDirectory);
		globalStatistics.configData.ReadTextValue(configKeyFilename,&mFilename);
		if ((mDirectory.Length()==0)&&(mFilename.Length()==0)&&(mDefaultValue.Length()>0))
		{
			wxFileName	defaultFilename(mDefaultValue);
			mDirectory=defaultFilename.GetPath();
			mFilename=defaultFilename.GetFullName();
			globalStatistics.configData.WriteTextValue(configKeyDirectory,mDirectory);
			globalStatistics.configData.WriteTextValue(configKeyFilename,mFilename);
		}

		CombineValue();
	}
}

void GUIWizardPageConfigFile::CombineValue()
{
}

GUIWizardPageConfigRemoteFile::GUIWizardPageConfigRemoteFile(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfigFile(wizardIn,parentPageID,thisPageID)
{
}

GUIWizardPageConfigRemoteFile::~GUIWizardPageConfigRemoteFile()
{
}

wxString GUIWizardPageConfigRemoteFile::RequestFileFromUser()
{
	wxString	message;
	bool		result;
	wxString	groupPrefix;
	wxString	userFilename;

	message="Select File";

	groupPrefix = mConfigKey.AfterFirst('/');
	groupPrefix = groupPrefix.BeforeFirst('/');

	GenericOKCancelDialog dialog(this,wxID_ANY,
							message,
							wxDefaultPosition,
							wxDefaultSize,
							wxCAPTION |
							wxCLOSE_BOX |
							wxSYSTEM_MENU |
							wxRESIZE_BORDER |
							wxMAXIMIZE_BOX,
							_T(""));
	FTPBrowserPanel	*browserPanel=new FTPBrowserPanel(
										groupPrefix,
										mDirectory,
										true);
	browserPanel->Create(&dialog,
						wxID_ANY,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	browserPanel->CreateScreen();
	
	result = dialog.DisplayDialog(browserPanel);
	if (result)
	{
		userFilename=browserPanel->GetCurrentSelection();
	}
	else
	{
		userFilename="";
	}
	return (userFilename);
}

void GUIWizardPageConfigRemoteFile::CombineValue()
{
	mValue= mDirectory;
	mValue+="/";
	mValue+=mFilename;
}

void GUIWizardPageConfigRemoteFile::SplitValue(wxString &valueDirectory,
													wxString &valueFilename)
{
	wxFileName	filename(mValue,wxPATH_UNIX);

	valueFilename	= filename.GetFullName();
	valueDirectory	= filename.GetPath();
	valueDirectory.Replace("\\","/");
}

GUIWizardPageConfigLocalFile::GUIWizardPageConfigLocalFile(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfigFile(wizardIn,parentPageID,thisPageID)
{
}

void GUIWizardPageConfigLocalFile::SplitValue(wxString &valueDirectory,
													wxString &valueFilename)
{
	wxFileName	filenameStr(mValue);

	valueFilename	= filenameStr.GetFullName();
	valueDirectory	= filenameStr.GetPath();
}

void GUIWizardPageConfigLocalFile::CombineValue()
{
	wxFileName	filenameStr;

	filenameStr.SetFullName(mFilename);
	filenameStr.SetPath(mDirectory);

	mValue = filenameStr.GetFullPath();
}

wxString GUIWizardPageConfigLocalFile::RequestFileFromUser()
{
	wxString	message;
	wxString	result;
	wxString	userFile;

	message="Select File";

	result=wxFileSelector(message,mDirectory);
	if (result.Length()>0)
	{
		userFile=result;
	}

	return (userFile);
}

GUIWizardPageRemoteFileTest::GUIWizardPageRemoteFileTest(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	wxSize		minSize;
	mStatusCtrl		= new wxStaticText(this,wxID_ANY,"Test Not Run");
	mErrorCtrl		= new wxRichTextCtrl(this,wxID_ANY,"");
	mErrorCtrl->SetEditable(false);
	mTestButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Remote File");
	mFileDirectory	= "";
	mLastResult = false;
	mContentsSizer->Add(mStatusCtrl,0,wxEXPAND);
	mContentsSizer->Add(mErrorCtrl,1,wxEXPAND|wxALL);
	mContentsSizer->Add(mTestButton,0,wxEXPAND);
	minSize = mErrorCtrl->GetSize();
	minSize.SetHeight(100);
	mErrorCtrl->SetMinSize(minSize);
}

GUIWizardPageRemoteFileTest::~GUIWizardPageRemoteFileTest()
{
	mLastResult = false;
}

void GUIWizardPageRemoteFileTest::SetGroupPrefix(wxString &groupPrefixIn)
{
	mGroupPrefix		= groupPrefixIn;
}

bool GUIWizardPageRemoteFileTest::TestResult()
{
	return (mLastResult);
}

wxString GUIWizardPageRemoteFileTest::GetValue()
{
	wxString	retVal;

	if (TestResult())
	{
		retVal="TRUE";
	}
	else
	{
		retVal="FALSE";
	}

	return (retVal);
}

bool GUIWizardPageRemoteFileTest::PerformTest()
{
	RemoteMachine		remoteMachine(mGroupPrefix);
	RestartingFTP				ftpConnection;
	wxString			msg;
	int					fileCount;
	wxString			filename;
	
	mStatusCtrl->SetLabel("Connecting To Remote Host");
	mErrorCtrl->SetValue("");
	mLastResult	=remoteMachine.Connect(ftpConnection);

	if (mLastResult)
	{
		mStatusCtrl->SetLabel("Retrieving Directory Listing");
		mLastResult = remoteMachine.GetRemoteDirectoryListing(mFileDirectory,mFileName,mFilenameList);

		if (mLastResult)
		{
			fileCount = mFilenameList.GetCount();
			if (fileCount == 0)
			{
				mStatusCtrl->SetLabel("Retrieving Directory Listing: Failed");
				mErrorCtrl->SetLabel("Directory listing received, but no files present");
				mLastResult = false;
			}
			else
			{
				msg.Printf("Retrieving Directory Listing: OK - %d files located",fileCount);
				mStatusCtrl->SetLabel(msg);
			}
		}
		else
		{
			mStatusCtrl->SetLabel("Retrieving Directory Listing: Failed");
			mErrorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
		}
	}
	else
	{
		mStatusCtrl->SetLabel("Connecting To Remote Host: Failed");
		mErrorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
	}

	return (mLastResult);
}

void GUIWizardPageRemoteFileTest::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (mCallBack!=NULL)
	{
		mCallBack(mCallBackObject);
	}
	PageContentsChanged();
}

void GUIWizardPageRemoteFileTest::ResetTestResult()
{
	mStatusCtrl->SetLabel("Test Not Run");
	mErrorCtrl->SetValue("");
	mLastResult = false;
	mFilenameList.Clear();
	mFilesizeList.Clear();
}

void GUIWizardPageRemoteFileTest::SetFile(wxString &fileDirectoryIn, wxString &fileNameIn)
{
	mFileDirectory	= fileDirectoryIn;
	mFileName		= fileNameIn;
}

GUIWizardPageConfigChoice::GUIWizardPageConfigChoice(StatsgenWizard *wizardIn,
						wxString parentPageID,
						wxString thisPageID,
						wxArrayString &answerCodesIn,
						wxArrayString &buttonTextsIn,
						wxArrayString &buttonDescriptionsIn) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfigChoice","GUIWizardPageConfigChoice")
	mValueCtrl			= NULL;
	mSelectionLabel		= NULL;
	mAnswerCodes 		= answerCodesIn;
	mButtonTexts		= buttonTextsIn;
	mButtonDescriptions	= buttonDescriptionsIn;

	mChoice				= "";
	mChoiceSizer		= NULL;
	CreateControl();
	STATSGEN_DEBUG_FUNCTION_END
}

GUIWizardPageConfigChoice::~GUIWizardPageConfigChoice()
{
	mValueCtrl = NULL;
}
void GUIWizardPageConfigChoice::CreateControl()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageConfigChoice","CreateControl")
	int				answerCount;
	int				answerIndex;
	wxButton		*button;
	wxStaticText	*description;
	wxString		answerCode;
	wxString		buttonText;
	wxString		buttonDescription;

	if (mChoiceSizer == NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating choice sizer")
		mChoiceSizer = new wxBoxSizer(wxVERTICAL);
		mContentsSizer->Add(mChoiceSizer,0,wxEXPAND);
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Clearing choice sizer")
		mChoiceSizer->Clear();
	}
	answerCount = mAnswerCodes.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding buttons")
	wxBoxSizer	*choiceButtonsSizer;

	choiceButtonsSizer = new wxBoxSizer(wxVERTICAL);
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		answerCode			= mAnswerCodes.Item(answerIndex);
		buttonText			= mButtonTexts.Item(answerIndex);
		buttonDescription	= mButtonDescriptions.Item(answerIndex);
		button				= new wxButton(this,
										WINDOW_ID_BUTTON_NEW,
										buttonText);
		description			= new wxStaticText(this, wxID_ANY, buttonDescription);
		mButtons.Add(button);
		mLabels.Add(description);

		wxBoxSizer			*buttonSizer;

		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating button sizer")
		buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonSizer->Add(button,1,wxEXPAND);
		buttonSizer->Add(description,1,wxEXPAND);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding button sizer")
		choiceButtonsSizer->Add(buttonSizer,0,wxEXPAND);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"added button sizer")
	}
	mChoiceSizer->Add(choiceButtonsSizer,0,wxEXPAND);
	if (mValueCtrl == NULL)
	{
		mValueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		mSelectionLabel	= new wxStaticText(this,wxID_ANY,"Current Selection: ");
	}
	SetSelection(WIZARD_SELECTION_NOTHING);
	if (mAnswerCodes.GetCount()>0)
	{
		mChoice = mButtonTexts.Item(0);
		SetSelection(mChoice);
		mChoice = mAnswerCodes.Item(0);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating selected sizer")
	wxBoxSizer	*selectedSizer;
	selectedSizer = new wxBoxSizer(wxHORIZONTAL);
	selectedSizer->Add(mSelectionLabel,0,wxEXPAND);
	selectedSizer->Add(mValueCtrl,0,wxEXPAND);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding selected sizer")
	mChoiceSizer->Add(selectedSizer,0,wxEXPAND);
	Fit();
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageConfigChoice::SetCtrlFromValue()
{
	mValueCtrl->SetValue(mValue);
}

bool GUIWizardPageConfigChoice::ScreenReady()
{
	return (mValueCtrl!=NULL);
}

void GUIWizardPageConfigChoice::SetValueFromCtrl()
{
	mValue=mValueCtrl->GetValue();
}

void GUIWizardPageConfigChoice::SetSelection(const char *selection)
{
	wxString	selectionStr;

	selectionStr=selection;
	SetSelection(selectionStr);
}

void GUIWizardPageConfigChoice::SetSelection(wxString &selection)
{
	mValueCtrl->SetValue(selection);
}

void GUIWizardPageConfigChoice::OnButtonPressed(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageChoice","OnButtonPressed")
	int			answerCount;
	int			answerIndex;
	wxButton	*button=NULL;
	wxString	selection;
	

	answerCount = mButtons.GetCount();

	mChoice = "";
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		button=(wxButton *)mButtons.Item(answerIndex);
		if (button == event.GetEventObject())
		{
			mChoice = mAnswerCodes.Item(answerIndex);
			selection = mButtonTexts.Item(answerIndex);
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"setting selection")
			SetSelection(mChoice);
			break;
		}
	}
	if (mCallBack!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"calling callback")
		mCallBack(mCallBackObject);
	}

	PageContentsChanged();
	STATSGEN_DEBUG_FUNCTION_END
}

wxString GUIWizardPageConfigChoice::GetChoice()
{
	return (mChoice);
}

GUIWizardPagesServers::GUIWizardPagesServers()
{
	mPageDeleteEditNew			= NULL;

}

GUIWizardPagesServers::~GUIWizardPagesServers()
{
}

void GUIWizardPagesServers::SetLogfileLimit(int limit)
{
	mPageServer.SetLogfileLimit(limit);
}

void GUIWizardPagesServers::SetLogfileLimit()
{
	mPageServer.SetLogfileLimit();
}

void GUIWizardPagesServers::SetDeleteEditNewChoices()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageChoice","SetDeleteEditNewChoices")
	wxArrayString	deleteEditNewAnswers;
	wxArrayString	deleteEditNewButtonTexts;
	wxArrayString	deleteEditNewButtonDescriptions;
	wxString		listGroup	="SERVERS";
	wxArrayString	serverIDs;
	int				serverCount;
	wxString		initialChoice;

	globalStatistics.configData.ReadList(listGroup,serverIDs);
	serverCount=serverIDs.GetCount();

	initialChoice = "New";
	deleteEditNewAnswers.Add(WIZARD_CHOICE_NEW);
	deleteEditNewButtonTexts.Add(initialChoice);
	deleteEditNewButtonDescriptions.Add("Create New Server");

	if (serverCount > 0)
	{
		initialChoice = "Edit";
		deleteEditNewAnswers.Add(WIZARD_CHOICE_EDIT);
		deleteEditNewButtonTexts.Add(initialChoice);
		deleteEditNewButtonDescriptions.Add("Edit Existing Server");
		deleteEditNewAnswers.Add(WIZARD_CHOICE_DELETE);
		deleteEditNewButtonTexts.Add("Delete");
		deleteEditNewButtonDescriptions.Add("Delete Existing Server");
	}

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Updating Choices")
	mPageDeleteEditNew->UpdateChoices(
									deleteEditNewAnswers,
									deleteEditNewButtonTexts,
									deleteEditNewButtonDescriptions);
	mPageDeleteEditNew->SetSelection(initialChoice);
	/*
	if (serverCount>0)
	{
		pageDeleteEditNew->SetSelection(WIZARD_CHOICE_EDIT);
	}
	*/

	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPagesServers::CreateWizardPages(StatsgenWizard *wizardIn)
{
	const char *sectionTitle = "Configure Servers";
	wxString	id;
	wxString ruleDeleteEditNewContinue;
	wxString ruleServerSelection;
	wxString ruleServerSelectionContinue;

	wxArrayString	deleteEditNewAnswers;
	wxArrayString	deleteEditNewButtonTexts;
	wxArrayString	deleteEditNewButtonDescriptions;
	wxArrayString	serverAnswers;
	wxArrayString	serverButtonTexts;
	wxArrayString	serverButtonDescriptions;

	mWizard = wizardIn;
	
	id="FINISHED";

	mPageDeleteEditNew			= new GUIWizardPageChoice(mWizard,
											mPageID,_T("DELETEEDITNEW"),
											deleteEditNewAnswers,
											deleteEditNewButtonTexts,
											deleteEditNewButtonDescriptions);
	mPageServerSelection			= new GUIWizardPageChoice(mWizard,
											mPageID,_T("SERVERSELECTION"),
											serverAnswers,
											serverButtonTexts,
											serverButtonDescriptions);
	SetDeleteEditNewChoices();
	SetServerChoices();
	mPageServerSelection->SetHelp("select a server to modify");
	mPageServerSelection->SetCallBack(ServerSelectedCallBack,this);


	mPageDeleteEditNew->SetHelp(
				"Do you want to create a [b]new[/b] server configuration, "
				"[b]edit[/b] an existing one, or [b]delete[/b] an existing one.\n"
				"\n"
				"[b]Press the appropriate button[/b]."
				"\n"
				"Press Next button after making selection"
				);

	mPageDeleteEditNew->SetCallBack(DeleteEditNewCallBack,this);


	mPageServer.SetPageID(mPageID,"SERVER");
	ruleDeleteEditNewContinue.Printf("(%s=\"%s\")|(%s=\"%s\")",
						STRING_TO_CHAR(mPageDeleteEditNew->GetPageID()),
						WIZARD_CHOICE_EDIT,
						STRING_TO_CHAR(mPageDeleteEditNew->GetPageID()),
						WIZARD_CHOICE_NEW);
	ruleServerSelection.Printf("%s=\"%s\"",STRING_TO_CHAR(mPageDeleteEditNew->GetPageID()),WIZARD_CHOICE_EDIT);
	ruleServerSelectionContinue.Printf("%s!\"\"",STRING_TO_CHAR(mPageServerSelection->GetPageID()));

	mWizard->AddPageLink(mPageDeleteEditNew,	"",					ruleDeleteEditNewContinue);
	mWizard->AddPageLink(mPageServerSelection,ruleServerSelection,ruleServerSelectionContinue);
	mPageServer.CreateWizardPages(mWizard);

	mPageDeleteEditNew->SetSectionTitle(sectionTitle);
	mPageServerSelection->SetSectionTitle(sectionTitle);
	mPageServer.SetSectionTitle(sectionTitle);
}

void GUIWizardPagesServers::SetServerChoices()
{
	wxArrayString	serverIDs;
	wxString		listGroup	="SERVERS";
	wxString		listPrefix	="Server";
	wxString		serverID;
	wxString		serverConfigPrefix;
	int				serverCount;
	int				serverIndex;
	wxArrayString	serverAnswers;
	wxArrayString	serverButtonTexts;
	wxArrayString	serverButtonDescriptions;

	globalStatistics.configData.ReadList(listGroup,serverIDs);
	serverCount=serverIDs.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		serverID=serverIDs.Item(serverIndex);
		serverConfigPrefix=listPrefix+serverID;
		serverAnswers.Add(serverConfigPrefix);
		serverButtonTexts.Add(serverID);
		serverButtonDescriptions.Add(serverID);
	}

	mPageServerSelection->UpdateChoices(serverAnswers,
										serverButtonTexts,
										serverButtonDescriptions);
}

wxString GUIWizardPagesServers::GetDeleteEditNewChoice()
{
	return (mPageDeleteEditNew->GetChoice());
}

void GUIWizardPagesServers::PrepareServerConfig()
{
	mPageServer.PrepareServerConfig();
}

void GUIWizardPagesServers::DeleteEditNewCallBack(void *object)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPagesServers","DeleteEditNewCallBack")
	wxString		captionNew="Enter new ID for Server";
	wxString		messageNew="New ID";
	wxString		captionDelete="Select ID To Remove";
	wxString		messageDelete="Removal ID";
	wxString		chosen;
	wxString		listGroup	="SERVERS";
	wxString		listPrefix	="Server";
	wxString		groupPrefix;
	wxArrayString	serverIDs;
	wxString		serverID;
	wxString		deleteEditNewChoice;

	GUIWizardPagesServers	*serversWizard;
	serversWizard = (GUIWizardPagesServers *)object;

	deleteEditNewChoice=serversWizard->GetDeleteEditNewChoice();

	if (deleteEditNewChoice.CmpNoCase(WIZARD_CHOICE_NEW)==0)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"New Server")
		chosen=wxGetTextFromUser(messageNew,captionNew);
		SafeString(chosen);
		if (chosen.Length()>0)
		{
			globalStatistics.ClearServers();
			globalStatistics.configData.ReadList(listGroup,serverIDs);
			serverIDs.Add(chosen);
			globalStatistics.configData.WriteList(listGroup,serverIDs);
			//globalStatistics.ReadServersFromConfig();
			groupPrefix=listPrefix+chosen;
			// Need to create the directories and default configurations
			// etc here
			serversWizard->SetDeleteEditNewChoices();
			serversWizard->SetServerChoices();
			serversWizard->SelectServer(groupPrefix);
			serversWizard->PrepareServerConfig();
		}
	}
	else
	if (deleteEditNewChoice.CmpNoCase(WIZARD_CHOICE_EDIT)==0)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Edit Server")
		serversWizard->SelectServer();
	}
	else
	if (deleteEditNewChoice.CmpNoCase(WIZARD_CHOICE_DELETE)==0)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Delete Server")
		globalStatistics.configData.ReadList(listGroup,serverIDs);
		serverID=wxGetSingleChoice(messageDelete,captionDelete,serverIDs);
		if (serverID.Length()>0)
		{
			serverIDs.Remove(serverID);
			globalStatistics.configData.WriteList(listGroup,serverIDs);
			groupPrefix=listPrefix+serverID;
			globalStatistics.configData.DeleteGroup(groupPrefix);
			serversWizard->SetDeleteEditNewChoices();
			serversWizard->SetServerChoices();
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPagesServers::ServerSelectedCallBack(void *object)
{
	GUIWizardPagesServers	*serversWizard;
	serversWizard = (GUIWizardPagesServers *)object;
	
	serversWizard->SelectServer();
}

void GUIWizardPagesServers::SelectServer()
{
	wxString	serverConfigPrefix;

	serverConfigPrefix=mPageServerSelection->GetChoice();
	mPageServer.SetGroupPrefix(serverConfigPrefix);
	mPageServer.SetLogfileLimit();
}

void GUIWizardPagesServers::SelectServer(wxString &choice)
{
	wxString	deleteEditNewChoice;
	mPageServerSelection->SelectChoice(choice);
	deleteEditNewChoice=WIZARD_CHOICE_EDIT;
	mPageDeleteEditNew->SelectChoice(deleteEditNewChoice);
}

GUIWizardPageConfigRemoteDirectory::GUIWizardPageConfigRemoteDirectory(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	mValueCtrl		= NULL;
	mBrowseButton	= NULL;
	CreateControl();
}

GUIWizardPageConfigRemoteDirectory::~GUIWizardPageConfigRemoteDirectory()
{
	mValueCtrl		= NULL;
	mBrowseButton	= NULL;
}

void GUIWizardPageConfigRemoteDirectory::CreateControl()
{
	mValueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	mBrowseButton	= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Browse For Directory");
	mContentsSizer->Add(mBrowseButton);
	mContentsSizer->Add(mValueCtrl,0,wxEXPAND);
}

void GUIWizardPageConfigRemoteDirectory::SetCtrlFromValue()
{
	mValueCtrl->SetValue(mValue);
}

bool GUIWizardPageConfigRemoteDirectory::ScreenReady()
{
	return (mValueCtrl!=NULL);
}

void GUIWizardPageConfigRemoteDirectory::SetValueFromCtrl()
{
	mValue = mValueCtrl->GetValue();
}

void GUIWizardPageConfigRemoteDirectory::OnButtonPressed(wxCommandEvent &event)
{
	wxString	message;
	bool		result;
	wxString	groupPrefix;

	message="Select Directory";

	groupPrefix = "WEBSITE";

	GenericOKCancelDialog dialog(this,wxID_ANY,
							message,
							wxDefaultPosition,
							wxDefaultSize,
							wxCAPTION |
							wxCLOSE_BOX |
							wxSYSTEM_MENU |
							wxRESIZE_BORDER |
							wxMAXIMIZE_BOX,
							_T(""));
	FTPBrowserPanel	*browserPanel=new FTPBrowserPanel(
										groupPrefix,
										mValue,
										false);
	browserPanel->Create(&dialog,
						wxID_ANY,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	browserPanel->CreateScreen();
	
	result = dialog.DisplayDialog(browserPanel);
	if (result)
	{
		mValue=browserPanel->GetCurrentSelection();
		SetCtrlFromValue();
	}
	PageContentsChanged();
}

void GUIWizardPageConfigRemoteDirectory::UpdateConfigFromValue()
{
	if (ConfigReady())
	{
		globalStatistics.configData.WriteTextValue(mConfigKey,mValue);
		if (mCallBack!=NULL)
		{
			mCallBack(mCallBackObject);
		}
	}
}

void GUIWizardPageConfigRemoteDirectory::UpdateValueFromConfig()
{
	if (ConfigReady())
	{
		globalStatistics.configData.ReadTextValue(mConfigKey,&mValue);
	}
}

GUIWizardPagesWebsite::GUIWizardPagesWebsite()
{
	mPageRemoteDirectory			= NULL;
	mPageRemoteDirectoryTest		= NULL;
	mGroupPrefix					= "WEBSITE";

}

GUIWizardPagesWebsite::~GUIWizardPagesWebsite()
{
}

void GUIWizardPagesWebsite::UpdateConfigKeys()
{
	wxString		configKey;

	if ((mPageRemoteDirectory != NULL) && (mGroupPrefix.length()>0))
	{
		configKey.Printf("/%s/FTPRemoteDirectory",STRING_TO_CHAR(mGroupPrefix));
		mPageRemoteDirectory->SetConfigKey(configKey);
	}
}

void GUIWizardPagesWebsite::SetGroupPrefix()
{
	mRemoteMachine.SetGroupPrefix(mGroupPrefix);
	mPageRemoteDirectoryTest->SetGroupPrefix(mGroupPrefix);
	UpdateConfigKeys();
}

void GUIWizardPagesWebsite::CreateWizardPages(StatsgenWizard *wizardIn)
{
	mWizard = wizardIn;
	wxString ruleTestPassed;
	wxString ruleWebsiteEnabled;

	const char *sectionTitle="Configure Website";
	mPageRemoteDirectory		= new GUIWizardPageConfigRemoteDirectory(mWizard,mPageID,_T("REMOTEDIRECTORY"));
	mPageRemoteDirectoryTest	= new GUIWizardPageRemoteDirectoryTest(mWizard,mPageID,_T("REMOTEDIRECTORYTEST"));

	mPageRemoteDirectory->SetConfigTitle("Website Directory");
	mPageRemoteDirectory->SetHelp(
			"The location on your web server where you want stats pages to be "
			"uploaded too."
						);

	mPageRemoteDirectoryTest->SetHelp(
			"[b]Press the button[/b] to check the website directory is configured correctly."
						);


	mPageRemoteDirectory->SetCallBack(ConfigChangedCallBack,this);
	mPageRemoteDirectoryTest->SetCallBack(ConfigChangedCallBack,this);
	

	mRemoteMachine.SetPageID(mPageID,"WEBSITE");
	mRemoteMachine.CreateWizardPages(mWizard);

	ruleTestPassed.Printf("%s=\"TRUE\"",STRING_TO_CHAR(mPageRemoteDirectoryTest->GetPageID()));
	ruleWebsiteEnabled.Printf("%s=\"Y\"",STRING_TO_CHAR(mRemoteMachine.GetEnabledPageID()));
	//wxMessageBox(ruleWebsiteEnabled);
	mWizard->AddPageLink(mPageRemoteDirectory,		ruleWebsiteEnabled,"");
	mWizard->AddPageLink(mPageRemoteDirectoryTest,	ruleWebsiteEnabled,ruleTestPassed);

	mRemoteMachine.SetCallBack(ConfigChangedCallBack,this);
	
	mPageRemoteDirectory->SetSectionTitle(sectionTitle);
	mPageRemoteDirectoryTest->SetSectionTitle(sectionTitle);
	mRemoteMachine.SetSectionTitle(sectionTitle);

	SetGroupPrefix();

}

void GUIWizardPagesWebsite::SetDirectory()
{
	wxString	directory;

	directory=mPageRemoteDirectory->GetValue();
	mPageRemoteDirectoryTest->SetDirectory(directory);
}

void GUIWizardPagesWebsite::ConfigChangedCallBack(void *object)
{
	GUIWizardPagesWebsite	*websiteWizard;
	websiteWizard = (GUIWizardPagesWebsite *)object;
	websiteWizard->SetDirectory();
}

GUIWizardPageRemoteDirectoryTest::GUIWizardPageRemoteDirectoryTest(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	wxSize	minSize;
	mStatusCtrl		= new wxStaticText(this,wxID_ANY,"Test Not Run");
	mErrorCtrl		= new wxRichTextCtrl(this,wxID_ANY,"");
	mErrorCtrl->SetEditable(false);
	mTestButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Remote Directory");
	mFileDirectory	= "";
	mLastResult = false;
	minSize = mErrorCtrl->GetSize();
	minSize.SetHeight(100);
	mErrorCtrl->SetMinSize(minSize);
	mContentsSizer->Add(mStatusCtrl,0,wxEXPAND);
	mContentsSizer->Add(mErrorCtrl,0,wxEXPAND);
	mContentsSizer->Add(mTestButton);
}

GUIWizardPageRemoteDirectoryTest::~GUIWizardPageRemoteDirectoryTest()
{
	mLastResult = false;
}

void GUIWizardPageRemoteDirectoryTest::SetGroupPrefix(wxString &groupPrefixIn)
{
	mGroupPrefix		= groupPrefixIn;
}

bool GUIWizardPageRemoteDirectoryTest::TestResult()
{
	return (mLastResult);
}

wxString GUIWizardPageRemoteDirectoryTest::GetValue()
{
	wxString	retVal;

	if (TestResult())
	{
		retVal="TRUE";
	}
	else
	{
		retVal="FALSE";
	}

	return (retVal);
}

bool GUIWizardPageRemoteDirectoryTest::PerformTest()
{
	RemoteMachine		remoteMachine(mGroupPrefix);
	RestartingFTP				ftpConnection;
	wxString			msg;
	int					fileCount;
	wxString			filename="*.*";
	wxArrayString		filenameList;
	
	mStatusCtrl->SetLabel("Connecting To Remote Host");
	mErrorCtrl->SetValue("");
	mLastResult	=remoteMachine.Connect(ftpConnection);

	if (mLastResult)
	{
		mStatusCtrl->SetLabel("Retrieving Directory Listing");
		mLastResult = remoteMachine.GetRemoteDirectoryListing(mFileDirectory,filename,filenameList);
		fileCount=filenameList.GetCount();

		if (mLastResult)
		{
			msg.Printf("Retrieving Directory Listing: OK - %d files located",fileCount);
			mStatusCtrl->SetLabel(msg);
		}
		else
		{
			mStatusCtrl->SetLabel("Retrieving Directory Listing: Failed");
			mErrorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
		}
	}
	else
	{
		mStatusCtrl->SetLabel("Connecting To Remote Host: Failed");
		mErrorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
	}

	return (mLastResult);
}

void GUIWizardPageRemoteDirectoryTest::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (mCallBack!=NULL)
	{
		mCallBack(mCallBackObject);
	}
	PageContentsChanged();
}

void GUIWizardPageRemoteDirectoryTest::ResetTestResult()
{
	mStatusCtrl->SetLabel("Test Not Run");
	mErrorCtrl->SetValue("");
	mLastResult = false;
}

void GUIWizardPageRemoteDirectoryTest::SetDirectory(wxString &fileDirectoryIn)
{
	mFileDirectory	= fileDirectoryIn;
}

GUIWizardPagesFullConfig::GUIWizardPagesFullConfig()
{
}

GUIWizardPagesFullConfig::~GUIWizardPagesFullConfig()
{
}

void GUIWizardPagesFullConfig::CreateWizardPages(StatsgenWizard *wizardIn)
{
	mWizard = wizardIn;
	mPageServers.SetPageID("FULLCONFIG","SERVERS");
	mPageServers.CreateWizardPages(mWizard);
	mPageWebsite.SetPageID("FULLCONFIG","WEBSITE");
	mPageWebsite.CreateWizardPages(mWizard);
}

StatsgenWizard::StatsgenWizard(wxWindow *parent,
					int id,
					wxString title,
					const wxBitmap &bitmap,
					const wxPoint &pos,
					long style): wxWizard(parent,WINDOW_ID_WIZARD,title,bitmap,pos,
							style|
							wxRESIZE_BORDER|
							wxMAXIMIZE_BOX|
							wxMINIMIZE_BOX)
{
	mInitialised		= false;
}
void StatsgenWizard::OnWizardFinish(wxWizardEvent &event)
{
	globalStatistics.configData.CommitChanges();
	globalStatistics.ClearServers();
	globalStatistics.ReadServersFromConfig();
}

void StatsgenWizard::OnWizardCancel(wxWizardEvent &event)
{
	globalStatistics.configData.RollbackChanges();
}

void StatsgenWizard::OnWizardPageChanged(wxWizardEvent &event)
{
	GUIWizardPageStatsgen	*page;
	page = (GUIWizardPageStatsgen *)GetCurrentPage();
	if (IsShown())
	{
		UpdatePageLinks(false);
		if (!mInitialised)
		{
			mInitialised=true;
		}
	}
	page->UpdateProgress();
}

void StatsgenWizard::SetInitialPageLinks()
{
	UpdatePageLinks(true);
	mPageLinks.SetPageCounters();
}

void StatsgenWizard::PageContentsChanged()
{
	if (IsShown())
	{
		UpdatePageLinks(false);
	}
}

void StatsgenWizard::AddPageLink(GUIWizardPageStatsgen *page,
								const char *allowRule,
								const char *continueRule)
{
	mPageLinks.AddPageLink(page,allowRule,continueRule);
}

void StatsgenWizard::UpdatePageLinks(bool fullList)
{
	mPageLinks.SetPageLinks(fullList);
}

GUIWizardPageStatsgen *StatsgenWizard::GetFirstPage()
{
	GUIWizardPageStatsgen *page = NULL;

	page = mPageLinks.GetFirstPage();

	return (page);
}

GUIWizardPages::GUIWizardPages()
{
	mPageID						= "";
}

GUIWizardPages::~GUIWizardPages()
{
}

void GUIWizardPages::SetPageID(wxString parentID,wxString pageIDIn)
{
	mPageID=parentID+pageIDIn;
}

GUIWizardPageConfigWildcard::GUIWizardPageConfigWildcard(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizard,parentPageID,thisPageID)
{
	mValueCtrl		= NULL;
	mDirectoryLabel	= NULL;
	mWildcardLabel	= NULL;
	mWildcardCtrl	= NULL;
	mBrowseButton	= NULL;
	CreateControl();
}

GUIWizardPageConfigWildcard::~GUIWizardPageConfigWildcard()
{
	mValueCtrl		= NULL;
	mWildcardCtrl	= NULL;
	mBrowseButton	= NULL;
}

void GUIWizardPageConfigWildcard::CreateControl()
{
	mValueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	mWildcardCtrl	= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	mWildcardLabel	= new wxStaticText(this,wxID_ANY,"Wildcard");
	mDirectoryLabel	= new wxStaticText(this,wxID_ANY,"Directory");
	mBrowseButton	= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Browse For Directory");

	mContentsSizer->Add(mBrowseButton);
	mContentsSizer->Add(mValueCtrl,0,wxEXPAND);
	mContentsSizer->Add(mWildcardLabel,0,wxEXPAND);
	mContentsSizer->Add(mWildcardCtrl,0,wxEXPAND);
	mContentsSizer->Add(mDirectoryLabel,0,wxEXPAND);
}

wxString GUIWizardPageConfigWildcard::GetWildcard()
{
	return (mWildcardValue);
}

void GUIWizardPageConfigWildcard::SetCtrlFromValue()
{
	VetoCtrlUpdates();
	mWildcardCtrl->SetValue(mWildcardValue);
	mValueCtrl->SetValue(mValue);
	AllowCtrlUpdates();
	UpdateConfigFromValue();
}

bool GUIWizardPageConfigWildcard::ScreenReady()
{
	return (mValueCtrl!=NULL);
}

void GUIWizardPageConfigWildcard::SetValueFromCtrl()
{
	mValue = mValueCtrl->GetValue();
	mWildcardValue = mWildcardCtrl->GetValue();
}

void GUIWizardPageConfigWildcard::GetConfigKeys(wxString &configKeyDirectory,
													wxString &configKeyFilename)
{
	configKeyDirectory	= mConfigKey + "Directory";
	configKeyFilename	= mConfigKey + "Wildcard";
}

void GUIWizardPageConfigWildcard::OnButtonPressed(wxCommandEvent &event)
{
	wxString	userDirectory;

	userDirectory = GetDirectoryFromUser();
	if (userDirectory.Length()>0)
	{
		mValue=userDirectory;
		SetCtrlFromValue();
	}
}

void GUIWizardPageConfigWildcard::UpdateConfigFromValue()
{
	wxString configKeyDirectory;
	wxString configKeyFilename;

	if (ConfigReady())
	{
		GetConfigKeys(configKeyDirectory,configKeyFilename);
		globalStatistics.configData.WriteTextValue(configKeyDirectory,mValue);
		globalStatistics.configData.WriteTextValue(configKeyFilename,mWildcardValue);
		if (mCallBack!=NULL)
		{
			mCallBack(mCallBackObject);
		}
	}
}

void GUIWizardPageConfigWildcard::UpdateValueFromConfig()
{
	wxString configKeyDirectory;
	wxString configKeyFilename;
	wxString msg;

	if (ConfigReady())
	{
		GetConfigKeys(configKeyDirectory,configKeyFilename);
		globalStatistics.configData.ReadTextValue(configKeyDirectory,&mValue);
		globalStatistics.configData.ReadTextValue(configKeyFilename,&mWildcardValue);
		if (mValue.Length()==0)
		{
			globalStatistics.configData.WriteTextValue(configKeyDirectory,mDefaultValue);
		}
	}
}

GUIWizardPageConfigLocalWildcard::GUIWizardPageConfigLocalWildcard(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfigWildcard(wizard,parentPageID,thisPageID)
{
}

GUIWizardPageConfigLocalWildcard::~GUIWizardPageConfigLocalWildcard()
{
}

wxString GUIWizardPageConfigLocalWildcard::GetDirectoryFromUser()
{
	wxString	message;
	wxString	result;
	wxString	userSelection;

	message="Select Directory";
	userSelection=wxDirSelector(message,mValue);

	return (userSelection);
}

GUIWizardPageConfigRemoteWildcard::GUIWizardPageConfigRemoteWildcard(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfigWildcard(wizard,parentPageID,thisPageID)
{
}

GUIWizardPageConfigRemoteWildcard::~GUIWizardPageConfigRemoteWildcard()
{
}

wxString GUIWizardPageConfigRemoteWildcard::GetDirectoryFromUser()
{
	wxString	message;
	bool		result;
	wxString	groupPrefix;
	wxString	userDirectory;

	message="Select Directory";

	groupPrefix = mConfigKey.AfterFirst('/');
	groupPrefix = groupPrefix.BeforeFirst('/');

	GenericOKCancelDialog dialog(this,wxID_ANY,
							message,
							wxDefaultPosition,
							wxDefaultSize,
							wxCAPTION |
							wxCLOSE_BOX |
							wxSYSTEM_MENU |
							wxRESIZE_BORDER |
							wxMAXIMIZE_BOX,
							_T(""));
	FTPBrowserPanel	*browserPanel=new FTPBrowserPanel(
										groupPrefix,
										mValue,
										false);
	browserPanel->Create(&dialog,
						wxID_ANY,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	browserPanel->CreateScreen();
	
	result = dialog.DisplayDialog(browserPanel);
	userDirectory="";
	if (result)
	{
		userDirectory=browserPanel->GetCurrentSelection();
	}

	return (userDirectory);
}

void GUIPageLinks::SetPageCounters()
{
	int						pageCount;
	int						pageIndex;
	GUIWizardPageStatsgen	*page;

	pageCount=guiPages.GetCount();
	for (pageIndex=0;pageIndex<pageCount;pageIndex++)
	{
		page=(GUIWizardPageStatsgen *)guiPages.Item(pageIndex);
		page->SetPageCounter(pageIndex,pageCount);
	}
}

void GUIPageLinks::AddPageLink(GUIWizardPageStatsgen *page,
								const char *allowRule,
								const char *continueRule)
{
	wxString	allowStr;
	wxString	continueStr;

	allowStr		= allowRule;
	continueStr		= continueRule;

	guiPages.Add(page);
	allowRules.Add(allowStr);
	continueRules.Add(continueStr);
}

void GUIPageLinks::SetPageLinks(bool ignoreRules)
{
	int						pageCount;
	int						pageIndex;
	GUIWizardPageStatsgen	*page;
	GUIWizardPageStatsgen	*prevPage;
	GUIWizardPageStatsgen	*nextPage;
	wxString				allowStr;
	wxString				continueStr;
	wxString				rule;
	wxString				prevPageID;
	wxString				nextPageID;
	wxString				prevPageValue;
	wxString				nextPageValue;
	wxString				msg;

	pageCount = guiPages.GetCount();
	page = NULL;
	for (pageIndex = 0;pageIndex < pageCount;pageIndex++)
	{
		page = (GUIWizardPageStatsgen *)guiPages.Item(pageIndex);
		nextPage	= FindPage(pageIndex,+1,ignoreRules);
		prevPage	= FindPage(pageIndex,-1,ignoreRules);
		continueStr	= continueRules.Item(pageIndex);
		if (!EvaluateRule(continueStr,ignoreRules))
		{
			nextPage=page;
		}
		page->SetPreviousPage(prevPage);
		page->SetNextPage(nextPage);
		if (prevPage!=NULL)
		{
			prevPageID=prevPage->GetPageID();
			prevPageValue=prevPage->GetValue();
		}
		else
		{
			prevPageID="NULL";
			prevPageValue="";
		}
		if (nextPage!=NULL)
		{
			nextPageID=nextPage->GetPageID();
			nextPageValue=nextPage->GetValue();
		}
		else
		{
			nextPageID="NULL";
			nextPageValue="";
		}
		/*
		msg.Printf("%d [%s](%s) -> [%s](%s) -> [%s](%s)",
			pageIndex,
			prevPageID.GetData(),
			prevPageValue.GetData(),
			page->GetPageID().GetData(),
			page->GetValue().GetData(),
			nextPageID.GetData(),
			nextPageValue.GetData());
		wxMessageBox(msg);
		*/
	}
}

GUIWizardPageStatsgen *GUIPageLinks::GetFirstPage()
{
	GUIWizardPageStatsgen *page;
	
	if (guiPages.GetCount()==0)
	{
		page = NULL;
	}
	else
	{
		page=(GUIWizardPageStatsgen *)guiPages.Item(0);
	}

	return (page);
}

GUIWizardPageStatsgen *GUIPageLinks::FindPage(int pageIndex,int direction,bool ignoreRules)
{
	int						pageCount;
	GUIWizardPageStatsgen	*page;
	GUIWizardPageStatsgen	*foundPage=NULL;
	wxString				allowStr;

	pageCount = guiPages.GetCount();

	pageIndex+=direction;
	while ((pageIndex < pageCount) && (pageIndex >= 0))
	{
		page = (GUIWizardPageStatsgen *)guiPages.Item(pageIndex);
		allowStr	= allowRules.Item(pageIndex);
		if (EvaluateRule(allowStr,ignoreRules))
		{
			foundPage = page;
			break;
		}
		
		pageIndex+=direction;
	}

	return (foundPage);
}

bool GUIPageLinks::EvaluateRule(wxString &rule,bool ignoreRules)
{
	bool		result;

	rule.Replace(" ","");
	

	if ((rule.Length()==0)|| ignoreRules)
	{
		result = true;
	}
	else
	{
		result = EvaluateRuleSub(rule);
	}

	return (result);
}

bool GUIPageLinks::EvaluateRuleSub(wxString &rule)
{
	char		ruleChar;
	char		operatorChar;
	wxString	value1;
	wxString	value2;
	wxString	id;
	bool		finished;
	wxString	msg;
	bool		debugOn;
	wxArrayString	valueQueue;


	if (rule.Contains(')'))
	{
		debugOn=false;
	}
	else
	{
		debugOn=false;
	}
	id				="";
	value1			="";
	value2			="";
	operatorChar	=' ';
	finished		=false;
	while ((rule.Length()>=0) && (!finished))
	{
		if (debugOn)
		{
			wxMessageBox(rule);
		}
		if (rule.Length()==0)
		{
			finished = true;
			ruleChar = 0;
		}
		else
		{
			ruleChar = rule[0];
			rule=rule.Mid(1);
		}
		switch (ruleChar)
		{
			case '&':
			case '=':
			case '!':
			case '|':
			case '(':
			case ')':
			case 0:
				// End of ID Char
				value1=GetValue(id);
msg.Printf("rulechar found[%c], ID=[%s] value=[%s]",ruleChar,STRING_TO_CHAR(id),STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
				if (id.Length()>0)
				{
					PushValue(valueQueue,value1);
				}
				if (ValueCount(valueQueue) == 2)
				{
					value2=PopValue(valueQueue);
					value1=PopValue(valueQueue);
msg.Printf("2 on stack:value1=[%s],value2=[%s] operatorChar=[%c]",STRING_TO_CHAR(value1),STRING_TO_CHAR(value2),operatorChar);if (debugOn) wxMessageBox(msg);
					switch (operatorChar)
					{
						case '&':
msg.Printf("& val1=[%s] val2=[%s]",STRING_TO_CHAR(value1),STRING_TO_CHAR(value2)); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value((Value2Bool(value1) && Value2Bool(value2)));
							PushValue(valueQueue,value1);
msg.Printf("& result value1=[%s]",STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
							break;
						case '!':
msg.Printf("! val1=[%s] val2=[%s]",STRING_TO_CHAR(value1),STRING_TO_CHAR(value2)); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value(value1.CmpNoCase(value2)!=0);
							PushValue(valueQueue,value1);
msg.Printf("! result value1=[%s]",STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
							break;
						case '=':
msg.Printf("= val1=[%s] val2=[%s]",STRING_TO_CHAR(value1),STRING_TO_CHAR(value2)); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value(value1.CmpNoCase(value2)==0);
							PushValue(valueQueue,value1);
msg.Printf("= result value1=[%s]",STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
							break;
						case '|':
msg.Printf("| val1=[%s] val2=[%s]",STRING_TO_CHAR(value1),STRING_TO_CHAR(value2)); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value((Value2Bool(value1) || Value2Bool(value2)));
							PushValue(valueQueue,value1);
msg.Printf("| result value1=[%s]",STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
							break;
					}
				}
				switch (ruleChar)
				{
					case '&':
					case '=':
					case '!':
					case '|':
						operatorChar = ruleChar;
						break;
					case '(':
						value1=Bool2Value(EvaluateRuleSub(rule));
						PushValue(valueQueue,value1);
msg.Printf("( result value1=[%s]",STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
						break;
					case ')':
					case 0:
						value1=PopValue(valueQueue);
msg.Printf(")/Null value1=[%s]",STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
						return (Value2Bool(value1));
						break;
				}
				id="";
				break;
			default:
				id+=ruleChar;
		}
	}

	value1=PopValue(valueQueue);
msg.Printf("final value1=[%s]",STRING_TO_CHAR(value1));if (debugOn) wxMessageBox(msg);
	return (Value2Bool(value1));
}

void GUIPageLinks::PushValue(wxArrayString &valueQueue,wxString &value)
{
	valueQueue.Add(value);
}


wxString GUIPageLinks::PopValue(wxArrayString &valueQueue)
{
	wxString		value="";
	int				queueSize;

	queueSize=ValueCount(valueQueue);
	if (queueSize>0)
	{
		value = valueQueue.Item(queueSize-1);
		valueQueue.RemoveAt(queueSize-1);
	}

	return (value);
}

int GUIPageLinks::ValueCount(wxArrayString &valueQueue)
{
	return(valueQueue.GetCount());
}

wxString GUIPageLinks::Bool2Value(bool valueBool)
{
	wxString	value;
	if (valueBool)
	{
		value="TRUE";
	}
	else
	{
		value="FALSE";
	}

	return (value);
}

bool GUIPageLinks::Value2Bool(wxString &value)
{
	bool	valueBool;

	valueBool = (value.CmpNoCase("TRUE")==0);

	return (valueBool);
}

wxString GUIPageLinks::GetValue(wxString &id)
{
	wxString					value;
	int							pageCount;
	int							pageIndex;
	GUIWizardPageStatsgen		*page;
	wxString					pageID;
	int							idLength;

	if (id.StartsWith("\""))
	{
		value = id.AfterFirst('\"');
		value = value.BeforeLast('\"');
	}
	else
	{
		idLength=id.Length();
		pageCount=guiPages.GetCount();
		for (pageIndex=0;pageIndex<pageCount;pageIndex++)
		{
			page=(GUIWizardPageStatsgen *)guiPages.Item(pageIndex);
			pageID=page->GetPageID();
			if (id.CmpNoCase(pageID.Right(idLength))==0)
			{
				// matching id found
				value=page->GetValue();
				//wxMessageBox("Found Match");
				//wxMessageBox(value);
				break;
			}
		}
	}

	return (value);
}

GUIWizardPagesFirstConfig::GUIWizardPagesFirstConfig()
{
}

GUIWizardPagesFirstConfig::~GUIWizardPagesFirstConfig()
{
}

void GUIWizardPagesFirstConfig::CreateWizardPages(StatsgenWizard *wizardIn)
{
	wxString		rule;
	const char	*titleChars="Test Run";

	mWizard = wizardIn;
	mPageServers.SetPageID("FULLCONFIG","SERVERS");
	mPageServers.CreateWizardPages(mWizard);

	mPageWebsite.SetPageID("FULLCONFIG","WEBSITE");
	mPageWebsite.CreateWizardPages(mWizard);

	mPageMessaging.SetPageID("FULLCONFIG","MESSAGING");
	mPageMessaging.CreateWizardPages(mWizard);

	mPageTestRun = new GUIWizardPageRun(mWizard,mPageID,_T("TESTRUN"));
	mPageTestRun->SetSectionTitle(titleChars);
	mPageTestRun->SetHelp("[b]Press the button[/b] to perform an initial test run of the stats.\n"
						"Once the run has completed a small amount of the stats pages will be "
						"uploaded to your website in the directory you have configured.\n"
						"\nYou will be missing images and style settings so the pages may look "
						"a little rubbish at the moment - the image pack and style uploads "
						"are in the next step");

	rule.Printf("%s=\"TRUE\"",STRING_TO_CHAR(mPageTestRun->GetPageID()));
	mWizard->AddPageLink(mPageTestRun,		"",	rule);
	mPageImagePacks.SetPageID("FULLCONFIG","IMAGEPACKS");
	mPageImagePacks.CreateWizardPages(mWizard);
}

GUIWizardPageRun::GUIWizardPageRun(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	wxSize		minSize;

	mStatusCtrl		= new wxStaticText(this,wxID_ANY,"Test Not Run");
	mErrorCtrl		= new wxRichTextCtrl(this,wxID_ANY,"");
	mErrorCtrl->SetEditable(false);
	mTestButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Stats Run");
	SetHelp("[b]Press the button[/b] to perform a stats run");
	minSize = mErrorCtrl->GetSize();
	minSize.SetHeight(100);
	mErrorCtrl->SetMinSize(minSize);

	mProgressPanel=new ProgressPanel;
	mProgressPanel->Create(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,
			wxRAISED_BORDER);
	mProgressPanel->CreateScreen();
	mProgressPanel->EnableTimeToGo();
	mProgressPanel->EnableCounter();

	mContentsSizer->Add(mStatusCtrl,0,wxEXPAND);
	mContentsSizer->Add(mErrorCtrl,1,wxEXPAND|wxALL);
	mContentsSizer->Add(mTestButton);
	mContentsSizer->Add(mProgressPanel,0,wxEXPAND);
	progress->SetClone(NULL);

	mRunSuccessful = false;
}

GUIWizardPageRun::~GUIWizardPageRun()
{
	progress->SetClone(NULL);
}

bool GUIWizardPageRun::TestResult()
{
	return (mRunSuccessful);
}

wxString GUIWizardPageRun::GetValue()
{
	wxString	retVal;

	if (TestResult())
	{
		retVal="TRUE";
	}
	else
	{
		retVal="FALSE";
	}

	return (retVal);
}

void GUIWizardPageRun::SetTestParameters(bool testRun)
{
	wxString		listGroup	="SERVERS";
	wxString		listPrefix	="Server";
	wxString		serverID;
	wxArrayString	serverIDs;
	wxString		serverConfigPrefix;
	wxString		configKey;
	wxString		configValue;
	int				serverCount;
	int				serverIndex;
	int				logfileLimit;
	int				roundsPerPlayer;
	int				playersPerRound;
	long			totalLogfileSize;
	long			logFileSize;
	Server			*server;

	globalStatistics.configData.ReadList(listGroup,serverIDs);
	serverCount=serverIDs.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		serverID=serverIDs.Item(serverIndex);
		serverConfigPrefix=listPrefix+serverID;
		if (testRun)
		{
			logfileLimit=2;
		}
		else
		{
			logfileLimit=0;
		}
		configKey.Printf("/%s/MaxLogfileSize",STRING_TO_CHAR(serverConfigPrefix));
		configValue.Printf("%d",logfileLimit);
		globalStatistics.configData.WriteTextValue(configKey,configValue);

	}

	if (testRun)
	{
		roundsPerPlayer	=0;
		playersPerRound	=0;
	}
	else
	{
		serverCount=globalStatistics.servers.GetCount();
		totalLogfileSize	=0;
		for (serverIndex=0;serverIndex<serverCount;serverIndex++)
		{
			server = (Server *)globalStatistics.servers.Item(serverIndex);
			server->FindLogfiles();
			server->CreateLogFiles();
			logFileSize = server->LogfileSize();
			totalLogfileSize+=logFileSize;
		}
		roundsPerPlayer	=0;
		playersPerRound	=0;

		if (totalLogfileSize > (1024 * 1024) * 10)
		{
			roundsPerPlayer	=5;
			playersPerRound	=2;
		}
		if (totalLogfileSize > (1024 * 1024) * 30)
		{
			roundsPerPlayer	=10;
			playersPerRound	=4;
		}
		if (totalLogfileSize > (1024 * 1024) * 50)
		{
			roundsPerPlayer	=15;
			playersPerRound	=4;
		}
		if (totalLogfileSize > (1024 * 1024) * 70)
		{
			roundsPerPlayer	=20;
			playersPerRound	=4;
		}
		if (totalLogfileSize > (1024 * 1024) * 90)
		{
			roundsPerPlayer	=30;
			playersPerRound	=4;
		}
	}
	configKey="/General/MinRoundsPerPlayer";
	configValue.Printf("%d",roundsPerPlayer);
	globalStatistics.configData.WriteTextValue(configKey,configValue);

	configKey="/General/MinPlayersInRound";
	configValue.Printf("%d",playersPerRound);
	globalStatistics.configData.WriteTextValue(configKey,configValue);
}

void GUIWizardPageRun::SetSelection(const char *selection)
{
	mStatusCtrl->SetLabel(selection);
}

bool GUIWizardPageRun::PerformTest()
{

	SetTestParameters(true);
	SetSelection("Test Running");

	progress->EventUpdating(false);
	mProgressPanel->EventUpdating(false);
	mProgressPanel->ClearErrors();

	{
		wxBusyCursor	cursor;
		mWizard->Enable(false);

		progress->SetClone(mProgressPanel);
		globalStatistics.statsgenDatabase.OpenDB();
		globalStatistics.statsgenDatabase.Zap();
		globalStatistics.ReadServersFromConfig();
		globalStatistics.FlagStartOfRun();

		mProgressPanel->ClearErrors();

		globalStatistics.ProduceStatsInitiate();
		globalStatistics.ProduceStatsDownload();
		globalStatistics.ProduceStatsProcess();
		globalStatistics.ProduceStatsTransfer();
		globalStatistics.ProduceStatsFinalise();
		mWizard->Enable(true);
		globalStatistics.statsgenDatabase.OpenDB();
		globalStatistics.statsgenDatabase.Zap();
	}
	SetSelection("Test Completed");
	//progress->EventUpdating(true);
	progress->EventUpdating(false);

	mRunSuccessful = 
		(wxYES == wxMessageBox(
				_T("Your initial stats run is now complete and the pages "
					"have been uploaded to your website, check your website "
					"to ensure you can see pages correctly.\nIf you see a red X "
					"in the progress bar this generally indicates an error that "
					"needs correcting, press the red X to get details of the "
					"failure (you can ignore any errors regarding progressbar).\n "
					"\n"
					"remember your webpages won't have any style settings or "
					"images yet so they may look a little odd.\n"
					"\n"
					"Press Yes if you see the stats pages."),
				_T("Can you see pages?"),
				wxYES_NO));

	SetTestParameters(false);

	return (mRunSuccessful);
}

void GUIWizardPageRun::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (mCallBack!=NULL)
	{
		mCallBack(mCallBackObject);
	}
	PageContentsChanged();
}

void GUIWizardPageRun::ResetTestResult()
{
	mStatusCtrl->SetLabel("Test Not Run");
	mErrorCtrl->SetValue("");
}

GUIWizardPageImagePacks::GUIWizardPageImagePacks(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageImagePacks","GUIWizardPageImagePacks")
/*
	mImagesCtrl		= new wxScrolledWindow(this,
							wxID_ANY,
							wxDefaultPosition,
							wxDefaultSize,
							wxVSCROLL);
*/
	mTestButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Upload Image Packs");
	SetHelp("[b]Press the button[/b] to perform a stats run");
	mProgressPanel=new ProgressPanel;
	mProgressPanel->Create(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,
			wxRAISED_BORDER);
	mProgressPanel->CreateScreen();
	mProgressPanel->EnableTimeToGo();
	mProgressPanel->EnableCounter();
	progress->SetClone(NULL);
	mStatsgenSite.GetImagePackList(mFileDescriptions,mFilePaths,mServerTypes,mUpgradeFiles,mThumbnails);
	mContentsSizer->Add(mTestButton);
	mContentsSizer->Add(mProgressPanel,1,wxEXPAND);
/*
	mContentsSizer->Add(mImagesCtrl,1,wxEXPAND);
*/
	CreateImagePackPanels();
	SelectImagePackPanels();
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageImagePacks::CreateImagePackPanels()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageImagePacks","CreateImagePackPanels")
	int				imagePackCount;
	int				imagePackIndex;
	ImagePackPanel	*imagePackPanel;
	wxString		fileDescription;
	wxString		filePath;
	wxString		serverType;
	wxString		thumbnail;
	wxString		upgradeFile;
	wxString		msg;
	wxSize			labelSize;
	wxSize			maxSize;

	imagePackCount=mFileDescriptions.GetCount();
	msg.Printf("Image Pack Count:%d",imagePackCount);
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		fileDescription=mFileDescriptions.Item(imagePackIndex);
		filePath=mFilePaths.Item(imagePackIndex);
		serverType=mServerTypes.Item(imagePackIndex);
		thumbnail=mThumbnails.Item(imagePackIndex);
		upgradeFile=mUpgradeFiles.Item(imagePackIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"fileDescription");
		STATSGEN_DEBUG(DEBUG_ALWAYS,fileDescription);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"filePath");
		STATSGEN_DEBUG(DEBUG_ALWAYS,filePath);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"serverType");
		STATSGEN_DEBUG(DEBUG_ALWAYS,serverType);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"thumbnail");
		STATSGEN_DEBUG(DEBUG_ALWAYS,thumbnail);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"upgradeFile");
		STATSGEN_DEBUG(DEBUG_ALWAYS,upgradeFile);
		//imagePackPanel=new ImagePackPanel(mImagesCtrl,
		imagePackPanel=new ImagePackPanel(this,
										fileDescription,
										filePath,
										serverType,
										upgradeFile,
										thumbnail);
		mImagePackPanels.Add(imagePackPanel);
		mContentsSizer->Add(imagePackPanel,1,wxEXPAND|wxALL);
		labelSize=imagePackPanel->GetLabelSize();
		if (maxSize.GetWidth()<labelSize.GetWidth())
		{
			maxSize.SetWidth(labelSize.GetWidth());
		}
		if (maxSize.GetHeight()<labelSize.GetHeight())
		{
			maxSize.SetHeight(labelSize.GetHeight());
		}
	}
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		imagePackPanel=(ImagePackPanel *)mImagePackPanels.Item(imagePackIndex);
		imagePackPanel->SetMinLabelSize(maxSize);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageImagePacks::SelectImagePackPanel(wxString &serverType)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageImagePacks","SelectImagePackPanel")
	int				imagePackCount;
	int				imagePackIndex;
	ImagePackPanel	*imagePackPanel;

	imagePackCount=mImagePackPanels.GetCount();
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		imagePackPanel=(ImagePackPanel *)mImagePackPanels.Item(imagePackIndex);
		if (serverType.CmpNoCase(imagePackPanel->GetServerType())==0)
		{
			imagePackPanel->SelectForUpload();
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GUIWizardPageImagePacks::SelectImagePackPanels()
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageImagePacks","SelectImagePackPanels")
	wxString		listGroup	="SERVERS";
	wxString		listPrefix	="Server";
	wxString		groupPrefix;
	wxString		serverID;
	int				serverCount;
	int				serverIndex;
	wxArrayString	serverIDs;
	wxString		serverType;
	wxString		configKey;

	globalStatistics.configData.ReadList(listGroup,serverIDs);
	serverCount=serverIDs.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		serverID=serverIDs.Item(serverIndex);
		groupPrefix=listPrefix+serverID;
		configKey.Printf("/%s/serverType",STRING_TO_CHAR(groupPrefix));
		globalStatistics.configData.ReadTextValue(configKey,&serverType);
		SelectImagePackPanel(serverType);
	}
	serverType="MANDATORY";
	SelectImagePackPanel(serverType);
	STATSGEN_DEBUG_FUNCTION_END
}

GUIWizardPageImagePacks::~GUIWizardPageImagePacks()
{
	progress->SetClone(NULL);
}

wxString GUIWizardPageImagePacks::GetValue()
{
	wxString	value="TRUE";

	return (value);
}

void GUIWizardPageImagePacks::OnButtonPressed(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("GUIWizardPageImagePacks","OnButtonPressed")
	int				imagePackCount;
	int				imagePackIndex;
	ImagePackPanel	*imagePackPanel;
	wxString		filePath;
	wxString		fileDescription;
	wxString		upgradeFile;
	wxBusyCursor	busy;
	bool			storeLocally=false;
	wxString		localOutputFolder;
	wxString		configKey="/General/LocalOutput";

	globalStatistics.configData.ReadTextValue(configKey,&localOutputFolder);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"setting clone")
	progress->SetClone(mProgressPanel);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"set clone")
	imagePackCount=mImagePackPanels.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"disable wizard")
	mWizard->Enable(false);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"ask if storing locally")
	storeLocally=(
		wxMessageBox("Store Image Packs Locally?","Confirm",wxYES_NO)==wxYES);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Stepping through image packs")
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		imagePackPanel=(ImagePackPanel *)mImagePackPanels.Item(imagePackIndex);
		if (imagePackPanel->Selected())
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"this image pack is selected")
			progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WEBSITE);
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"chosen the panel")
			filePath=imagePackPanel->GetFilePath();
			STATSGEN_DEBUG(DEBUG_ALWAYS,filePath);
			fileDescription=imagePackPanel->GetDescription();
			STATSGEN_DEBUG(DEBUG_ALWAYS,fileDescription);
			upgradeFile=imagePackPanel->GetUpgradeFile();
			STATSGEN_DEBUG(DEBUG_ALWAYS,upgradeFile);
			mStatsgenSite.UploadImagePack(fileDescription,filePath,storeLocally,localOutputFolder,upgradeFile);
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"uploaded the image pack")
			progress->Finalise();
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"finalised")
		}
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"enable the wizard")
	mWizard->Enable(true);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"set clone to null")
	progress->SetClone(NULL);
	if (mCallBack!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"perform callback")
		mCallBack(mCallBackObject);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"say that page contents have changed")
	PageContentsChanged();
	STATSGEN_DEBUG_FUNCTION_END
}

GUIWizardPagesImagePacks::GUIWizardPagesImagePacks()
{
	mPageImagePacks			= NULL;
}

GUIWizardPagesImagePacks::~GUIWizardPagesImagePacks()
{
}

void GUIWizardPagesImagePacks::CreateWizardPages(StatsgenWizard *wizardIn)
{
	mWizard = wizardIn;

	const char *sectionTitle="Image Packs";
	mPageImagePacks		= new GUIWizardPageImagePacks(mWizard,mPageID,_T("IMAGEPACKS"));

	mPageImagePacks->SetHelp(
			"[b]Place a check[/b] in the appropriate image packs and then [b]press "
			"the Upload button[/b] to download them from the statsgen website and "
			"then upload them to your website"
						);


	mWizard->AddPageLink(mPageImagePacks,		"","");
	
	mPageImagePacks->SetSectionTitle(sectionTitle);

}

ImagePackPanel::ImagePackPanel(wxWindow *parent,
									wxString &fileDescriptionIn,
									wxString &filePathIn,
									wxString &serverTypeIn,
									wxString &upgradeFileIn,
									wxString &thumbnailIn) : wxPanel(parent,-1)
{
	STATSGEN_DEBUG_FUNCTION_START("ImagePackPanel","ImagePackPanel")
	wxString		label="";
	wxString		localimage="thumbnail.";

	mFileDescription	=fileDescriptionIn;
	mFilePath		=filePathIn;
	mServerType		=serverTypeIn;
	mThumbnail		=thumbnailIn;
	mUpgradeFile		=upgradeFileIn;

	if (mThumbnail.EndsWith("jpg"))
	{
		localimage+="jpg";
	}
	if (mThumbnail.EndsWith("gif"))
	{
		localimage+="gif";
	}
	mCheckCtrl		=new wxCheckBox(this,wxID_ANY,label);
	mDescriptionCtrl	=new wxStaticText(this,wxID_ANY,mFileDescription);
	if (mThumbnail.Length()>0)
	{
		StatsgenWeb		statsgenSite;
		WebFile			*webFile;
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"thumbnail");
		STATSGEN_DEBUG(DEBUG_ALWAYS,mThumbnail);
		webFile=statsgenSite.GetWebFile(mThumbnail);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"got web file");
		webFile->Get(localimage);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"localimage");
		STATSGEN_DEBUG(DEBUG_ALWAYS,localimage);
		mThumbnailImage.LoadFile(localimage);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"loaded localimage");

		delete(webFile);
	}
	mThumbnailPanel	=new ImagePanel(this,wxID_ANY,
								wxDefaultPosition,
								wxDefaultSize,
								0,
								_T(""));
	mThumbnailPanel->SetImage(mThumbnailImage);

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mCheckCtrl);
	mMainSizer->Add(mDescriptionCtrl,0,wxEXPAND);
	mMainSizer->Add(mThumbnailPanel,1,wxEXPAND|wxALL);

	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
	STATSGEN_DEBUG_FUNCTION_END
}

ImagePackPanel::~ImagePackPanel()
{
}

wxString ImagePackPanel::GetServerType()
{
	return (mServerType);
}
wxString ImagePackPanel::GetDescription()
{
	return (mFileDescription);
}
wxString ImagePackPanel::GetFilePath()
{
	return (mFilePath);
}
wxString ImagePackPanel::GetUpgradeFile()
{
	return (mUpgradeFile);
}
bool ImagePackPanel::Selected()
{
	return (mCheckCtrl->GetValue());
}
void ImagePackPanel::SelectForUpload()
{
	mCheckCtrl->SetValue(true);
}

wxSize ImagePackPanel::GetLabelSize()
{
	return (mDescriptionCtrl->GetSize());
}
void ImagePackPanel::SetMinLabelSize(wxSize size)
{
	mDescriptionCtrl->SetMinSize(size);
}
GUIWizardPagesMessaging::GUIWizardPagesMessaging()
{
	mPageMessagingEnabled					= NULL;
	mPageMessagingFrequency					= NULL;
	mPageMessagingPort						= NULL;
}

GUIWizardPagesMessaging::~GUIWizardPagesMessaging()
{
}

void GUIWizardPagesMessaging::UpdateConfigKeys()
{
	wxString		configKey;
	wxString		configValue;

	if (mPageMessagingEnabled != NULL)
	{
		configKey="/RCONSettings/Enabled";
		mPageMessagingEnabled->SetConfigKey(configKey,"Y");

		configKey="/RCONSettings/MessageIntervalTimer";
		mPageMessagingFrequency->SetConfigKey(configKey,"60");

		configKey="/RCONSettings/ClientPort";
		mPageMessagingPort->SetConfigKey(configKey,"8000");

		configKey="/RCONSettings/CustomMessageInterval";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"0");

		configKey="/RCONSettings/MessagePrefix";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"^7");
	}
}

void GUIWizardPagesMessaging::CreateWizardPages(StatsgenWizard *wizardIn)
{
	const char *sectionTitle="Configure Messaging";
	wxString ruleMessagingEnabled;

	mWizard=wizardIn;

	mPageMessagingEnabled			= new GUIWizardPageConfigBoolean(mWizard,mPageID,_T("ENABLED"));
	mPageMessagingFrequency			= new GUIWizardPageConfigText(mWizard,mPageID,_T("FREQUENCY"));
	mPageMessagingPort				= new GUIWizardPageConfigText(mWizard,mPageID,_T("PORT"));

	mPageMessagingEnabled->SetConfigTitle("Enable Messaging");
	mPageMessagingEnabled->SetHelp(
		"Global Messaging Enabled / Disabled\n"
		"This switches on / off messaging for the entire system, irrespective "
		"of any individual server settings"
		);
	mPageMessagingEnabled->SetTrueFalseLabels("Yes", "No");

	mPageMessagingFrequency->SetConfigTitle("Message Frequency");
	mPageMessagingFrequency->SetHelp(
		"How often messages should be sent to the server, in seconds"
		);

	mPageMessagingPort->SetConfigTitle("Messaging Client Port");
	mPageMessagingPort->SetHelp(
		"This is the local port used during communications with the game "
		"servers.\n"
		"You should have a [b]different value[/b] for this for every instance of "
		"statsgen you are running.\n"
		"[b]This is NOT the RCON port number[/b].\n"
		"\nGood values are 8000, 8001, 8002"
		);

	mPageMessagingEnabled->SetCallBack(ConfigChangedCallBack,this);
	mPageMessagingFrequency->SetCallBack(ConfigChangedCallBack,this);
	mPageMessagingPort->SetCallBack(ConfigChangedCallBack,this);

	ruleMessagingEnabled.Printf("(%s=\"%s\")", STRING_TO_CHAR(mPageMessagingEnabled->GetPageID()),"Y");

	mWizard->AddPageLink(mPageMessagingEnabled,		""		,"");
	mWizard->AddPageLink(mPageMessagingFrequency,		ruleMessagingEnabled	,"");
	mWizard->AddPageLink(mPageMessagingPort,			ruleMessagingEnabled	,"");
	SetSectionTitle(sectionTitle);

	UpdateConfigKeys();

}

void GUIWizardPagesMessaging::SetSectionTitle(const char *sectionTitle)
{
	mPageMessagingEnabled->SetSectionTitle(sectionTitle);
	mPageMessagingFrequency->SetSectionTitle(sectionTitle);
	mPageMessagingPort->SetSectionTitle(sectionTitle);
}

void GUIWizardPagesMessaging::ConfigChangedCallBack(void *object)
{
	GUIWizardPagesMessaging	*wizard;
	wizard = (GUIWizardPagesMessaging *)object;
}

GUIWizardPageTemplatePacks::GUIWizardPageTemplatePacks(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
/*
	mImagesCtrl		= new wxScrolledWindow(this,
							wxID_ANY,
							wxDefaultPosition,
							wxDefaultSize,
							wxVSCROLL);
*/
	mTestButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Upload Template Packs");
	SetHelp("[b]Press the button[/b] to perform a stats run");
	mProgressPanel=new ProgressPanel;
	mProgressPanel->Create(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,
			wxRAISED_BORDER);
	mProgressPanel->CreateScreen();
	progress->SetClone(NULL);
	mStatsgenSite.GetTemplateList(mFileDescriptions,mFilePaths,mServerTypes,mSecondFilePaths,mTemplateFilenames,mUpgradePaths,mThumbnails);

	mContentsSizer->Add(mTestButton);
	mContentsSizer->Add(mProgressPanel,1,wxEXPAND);
	CreateTemplatePackPanels();
	SelectTemplatePackPanels();
}

void GUIWizardPageTemplatePacks::CreateTemplatePackPanels()
{
	int				imagePackCount;
	int				imagePackIndex;
	TemplatePackPanel	*imagePackPanel;
	wxString		fileDescription;
	wxString		filePath;
	wxString		secondFilePath;
	wxString		templateFilename;
	wxString		serverType;
	wxString		upgradePath;
	wxString		thumbnail;
	wxString		msg;
	wxSize			labelSize;
	wxSize			maxSize;

	imagePackCount=mFileDescriptions.GetCount();
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		fileDescription=mFileDescriptions.Item(imagePackIndex);
		filePath=mFilePaths.Item(imagePackIndex);
		serverType=mServerTypes.Item(imagePackIndex);
		thumbnail=mThumbnails.Item(imagePackIndex);
		templateFilename=mTemplateFilenames.Item(imagePackIndex);
		secondFilePath=mSecondFilePaths.Item(imagePackIndex);
		upgradePath=mUpgradePaths.Item(imagePackIndex);
		//imagePackPanel=new TemplatePackPanel(mImagesCtrl,
		imagePackPanel=new TemplatePackPanel(this,
										fileDescription,
										filePath,
										serverType,
										secondFilePath,
										upgradePath,
										templateFilename,
										thumbnail);
		mTemplatePackPanels.Add(imagePackPanel);
		mContentsSizer->Add(imagePackPanel,1,wxEXPAND|wxALL);
		labelSize=imagePackPanel->GetLabelSize();
		if (maxSize.GetWidth()<labelSize.GetWidth())
		{
			maxSize.SetWidth(labelSize.GetWidth());
		}
		if (maxSize.GetHeight()<labelSize.GetHeight())
		{
			maxSize.SetHeight(labelSize.GetHeight());
		}
	}
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		imagePackPanel=(TemplatePackPanel *)mTemplatePackPanels.Item(imagePackIndex);
		imagePackPanel->SetMinLabelSize(maxSize);
	}
}

void GUIWizardPageTemplatePacks::SelectTemplatePackPanel(wxString &serverType)
{
}

void GUIWizardPageTemplatePacks::SelectTemplatePackPanels()
{
}

GUIWizardPageTemplatePacks::~GUIWizardPageTemplatePacks()
{
	progress->SetClone(NULL);
}

wxString GUIWizardPageTemplatePacks::GetValue()
{
	wxString	value="TRUE";

	return (value);
}

void GUIWizardPageTemplatePacks::OnButtonPressed(wxCommandEvent &event)
{
	int				imagePackCount;
	int				imagePackIndex;
	TemplatePackPanel	*templatePackPanel;
	wxString		filePath;
	wxString		secondFilePath;
	wxString		upgradePath;
	wxString		templateFilename;
	wxString		fileDescription;
	wxBusyCursor	busy;

	progress->SetClone(mProgressPanel);
	imagePackCount=mTemplatePackPanels.GetCount();
	mWizard->Enable(false);
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		templatePackPanel=(TemplatePackPanel *)mTemplatePackPanels.Item(imagePackIndex);
		if (templatePackPanel->Selected())
		{
			progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WEBSITE);
			filePath=templatePackPanel->GetFilePath();
			secondFilePath=templatePackPanel->GetSecondFilePath();
			upgradePath=templatePackPanel->GetUpgradePath();
			templateFilename=templatePackPanel->GetTemplateFilename();
			fileDescription=templatePackPanel->GetDescription();
			mStatsgenSite.UploadTemplatePack(fileDescription,filePath,secondFilePath,upgradePath,templateFilename);
			progress->Finalise();
		}
	}
	mWizard->Enable(true);

	progress->SetClone(NULL);
	if (mCallBack!=NULL)
	{
		mCallBack(mCallBackObject);
	}
	PageContentsChanged();
}

GUIWizardPagesTemplatePacks::GUIWizardPagesTemplatePacks()
{
	mPageTemplatePacks			= NULL;
}

GUIWizardPagesTemplatePacks::~GUIWizardPagesTemplatePacks()
{
}

void GUIWizardPagesTemplatePacks::CreateWizardPages(StatsgenWizard *wizardIn)
{
	mWizard = wizardIn;

	const char *sectionTitle="Template Packs";
	mPageTemplatePacks		= new GUIWizardPageTemplatePacks(mWizard,mPageID,_T("TEMPLATEPACKS"));

	mPageTemplatePacks->SetHelp(
			"[b]Place a check[/b] in the appropriate template and then [b]press "
			"the Upload button[/b] to download them from the statsgen website and "
			"then upload them to your website"
						);


	mWizard->AddPageLink(mPageTemplatePacks,		"","");
	
	mPageTemplatePacks->SetSectionTitle(sectionTitle);

}

TemplatePackPanel::TemplatePackPanel(wxWindow *parent,
									wxString &fileDescriptionIn,
									wxString &filePathIn,
									wxString &serverTypeIn,
									wxString &secondFilePathIn,
									wxString &upgradePathIn,
									wxString &templateFilenameIn,
									wxString &thumbnailIn) : wxPanel(parent,-1)
{
	wxString		label="";
	wxString		localimage="thumbnail.";

	mFileDescription	=fileDescriptionIn;
	mFilePath		=filePathIn;
	mServerType		=serverTypeIn;
	mSecondFilePath	=secondFilePathIn;
	mTemplateFilename=templateFilenameIn;
	mUpgradePath		=upgradePathIn;
	mThumbnail		=thumbnailIn;

	if (mThumbnail.EndsWith("jpg"))
	{
		localimage+="jpg";
	}
	if (mThumbnail.EndsWith("gif"))
	{
		localimage+="gif";
	}
	mCheckCtrl		=new wxCheckBox(this,wxID_ANY,label);
	mDescriptionCtrl	=new wxStaticText(this,wxID_ANY,mFileDescription);
	if (mThumbnail.Length()>0)
	{
		StatsgenWeb		statsgenSite;
		WebFile			*webFile;

		webFile=statsgenSite.GetWebFile(mThumbnail);
		webFile->Get(localimage);
		mThumbnailImage.LoadFile(localimage);

		delete(webFile);
	}
	mThumbnailPanel	=new ImagePanel(this,wxID_ANY,
								wxDefaultPosition,
								wxDefaultSize,
								0,
								_T(""));
	mThumbnailPanel->SetImage(mThumbnailImage);
	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mCheckCtrl);
	mMainSizer->Add(mDescriptionCtrl,0,wxEXPAND);
	mMainSizer->Add(mThumbnailPanel,1,wxEXPAND|wxALL);

	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
}

TemplatePackPanel::~TemplatePackPanel()
{
}

wxString TemplatePackPanel::GetServerType()
{
	return (mServerType);
}
wxString TemplatePackPanel::GetDescription()
{
	return (mFileDescription);
}
wxString TemplatePackPanel::GetFilePath()
{
	return (mFilePath);
}
wxString TemplatePackPanel::GetSecondFilePath()
{
	return (mSecondFilePath);
}
wxString TemplatePackPanel::GetUpgradePath()
{
	return (mUpgradePath);
}
wxString TemplatePackPanel::GetTemplateFilename()
{
	return (mTemplateFilename);
}
bool TemplatePackPanel::Selected()
{
	return (mCheckCtrl->GetValue());
}
void TemplatePackPanel::SelectForUpload()
{
	mCheckCtrl->SetValue(true);
}

wxSize TemplatePackPanel::GetLabelSize()
{
	return (mDescriptionCtrl->GetSize());
}
void TemplatePackPanel::SetMinLabelSize(wxSize size)
{
	mDescriptionCtrl->SetMinSize(size);
}
