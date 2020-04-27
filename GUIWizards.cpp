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
	wxFont		font;
	previousPage	= NULL;
	nextPage		= NULL;
	helpTextCtrl	= NULL;
	sectionTitle	= NULL;
	installProgress		= NULL;
	helpText		= "";
	callBack		= NULL;
	callBackObject	= NULL;

	wizard=wizardIn;

	helpTextCtrl	= new wxRichTextCtrl(this);
	sectionTitle	= new wxStaticText(this,-1,"");
	font = sectionTitle->GetFont();
	font.SetPointSize(3 * font.GetPointSize());
	font.SetWeight(wxFONTWEIGHT_BOLD);
	
	sectionTitle->SetFont(font);
	
	installProgress		= new wxGauge(this,-1,30);
	helpTextCtrl->SetEditable(false);
	pageID=parentPageID;
	pageID+=thisPageID;
}


GUIWizardPageStatsgen::~GUIWizardPageStatsgen()
{
}

wxString GUIWizardPageStatsgen::GetPageID()
{
	return (pageID);
}

void GUIWizardPageStatsgen::SetSectionTitle(const char *titleChars)
{
	sectionTitle->SetLabel(titleChars);
}

void GUIWizardPageStatsgen::SetCallBack(void(* callBackIn)(void *object),void *callBackObjectIn)
{
	callBack		= callBackIn;
	callBackObject	= callBackObjectIn;
}

wxWizardPage *GUIWizardPageStatsgen::GetPrev() const
{
	return (previousPage);
}

wxWizardPage *GUIWizardPageStatsgen::GetNext() const
{
	return (nextPage);
}

void GUIWizardPageStatsgen::SetNextPage(wxWizardPage *page)
{
	nextPage		= page;
}

void GUIWizardPageStatsgen::SetPreviousPage(wxWizardPage *page)
{
	previousPage	= page;
}

void GUIWizardPageStatsgen::SetHelp(const char *helpTextIn)
{
	helpText = helpTextIn;
	UpdateScreen();
}

void GUIWizardPageStatsgen::UpdateScreen()
{
	if (helpTextCtrl!=NULL)
	{
		wxString		workingString;
		wxString		textSection;
		wxString		codeSection;
		bool			startSection;
		char			code;

		workingString	=helpText;
		helpTextCtrl->Clear();
		helpTextCtrl->BeginFontSize(15);

		while (workingString.Length()>0)
		{
			textSection=workingString.BeforeFirst('[');
			helpTextCtrl->AppendText(textSection);

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
						helpTextCtrl->BeginBold();
					}
					else
					{
						helpTextCtrl->EndBold();
					}
					break;
				case 'i':
				case 'I':
					if (startSection)
					{
						helpTextCtrl->BeginItalic();
					}
					else
					{
						helpTextCtrl->EndItalic();
					}
					break;
			}
			workingString=codeSection.AfterFirst(']');
		}
		//helpTextCtrl->SetValue(helpText);
	}
}

wxSize GUIWizardPageStatsgen::ResizeControl(int x,int y,int width,bool resize)
{
	wxSize	controlSize;

	controlSize.SetWidth(width);
	controlSize.SetHeight(0);

	return (controlSize);
}

void GUIWizardPageStatsgen::Resize(int maxHeight)
{
	int		sectionTitleX;
	int		sectionTitleY;
	int		sectionTitleWidth;
	int		sectionTitleHeight;

	int		progressX;
	int		progressY;
	int		progressWidth;
	int		progressHeight;
	int		helpX;
	int		helpY;
	int		helpWidth;
	int		helpHeight;
	int		controlX;
	int		controlY;
	int		controlHeight;
	wxSize	controlSize;
	wxSize	pageSize;

	int		width=700;
	int		height;
	wxString	msg;

	if (helpTextCtrl!=NULL)
	{
		controlSize = ResizeControl(0,0,width,false);
		controlHeight = controlSize.GetHeight();
		if (controlSize.GetWidth()>width)
		{
			width=controlSize.GetWidth();
		}
		sectionTitleX		= 0;
		sectionTitleY		= 0;
		sectionTitleWidth	= width;
		sectionTitleHeight	= 60;
		progressX		= 0;
		progressY		= sectionTitleY + sectionTitleHeight;
		progressHeight	= 20;
		progressWidth	= width;

		controlX		= 0;
		controlY		= progressY+progressHeight+5;

		helpX			= 0;
		helpY			= controlY + controlHeight;
		helpWidth		= width;
		helpHeight		= 100;

		sectionTitle->SetSize(sectionTitleX,sectionTitleY,sectionTitleWidth,sectionTitleHeight);
		installProgress->SetSize(progressX,progressY,progressWidth,progressHeight);
		if (maxHeight>=0)
		{
			helpHeight = maxHeight - helpY;
		}
		helpTextCtrl->SetSize(helpX,helpY,helpWidth,helpHeight);

		ResizeControl(controlX,controlY,width,true);

		height = helpY + helpHeight;

		SetSize(wxSize(width,height));
	}
}

void GUIWizardPageStatsgen::SetPageCounter(int pageIndexIn,int pageCountIn)
{
	pageIndex		= pageIndexIn;
	wizardPageCount	= pageCountIn;

	installProgress->SetRange(wizardPageCount);
}

void GUIWizardPageStatsgen::UpdateProgress()
{
	installProgress->SetValue(pageIndex+1);
}

bool GUIWizardPageStatsgen::AutoAdvance()
{
	return false;
}

void GUIWizardPageStatsgen::PageContentsChanged()
{
	if (wizard!=NULL)
	{
		wizard->PageContentsChanged();
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
	titleCtrl		= NULL;
	configKey		= "";
	defaultValue	= "";
	title			= "";
	AllowCtrlUpdates();

	titleCtrl		= new wxStaticText(this,-1,title);
}

GUIWizardPageConfig::~GUIWizardPageConfig()
{
}

bool GUIWizardPageConfig::ConfigReady()
{
	if (configKey.Length()>0)
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
		defaultValue = "";
	}
	else
	{
		defaultValue = defaultValueIn;
	}
}

void GUIWizardPageConfig::SetConfigKey(wxString &configKeyIn,const char *defaultValueIn)
{
	configKey = configKeyIn;
	SetDefault(defaultValueIn);
	UpdateScreen();
}

void GUIWizardPageConfig::SetConfigTitle(const char *titleIn)
{
	title=titleIn;
	UpdateScreen();
}

void GUIWizardPageConfig::VetoCtrlUpdates()
{
	vetoUpdates	=true;
}

void GUIWizardPageConfig::AllowCtrlUpdates()
{
	vetoUpdates	=false;
}

bool GUIWizardPageConfig::CtrlUpdatesAllowed()
{
	return (vetoUpdates);
}

void GUIWizardPageConfig::OnValueChanged(wxCommandEvent &event)
{
	if (ScreenReady() && ConfigReady() && !CtrlUpdatesAllowed())
	{
		SetValueFromCtrl();
		UpdateConfigFromValue();
		PageContentsChanged();
	}
}

void GUIWizardPageConfig::UpdateConfigFromValue()
{
	if (ConfigReady())
	{
		globalStatistics.configData.WriteTextValue(configKey,value);
		if (callBack!=NULL)
		{
			callBack(callBackObject);
		}
	}
}

void GUIWizardPageConfig::UpdateValueFromConfig()
{
	if (ConfigReady())
	{
		globalStatistics.configData.ReadTextValue(configKey,&value,(char *)defaultValue.GetData());
	}
}

void GUIWizardPageConfig::UpdateCtrlFromValue()
{
	if (ScreenReady() && ConfigReady())
	{
		UpdateValueFromConfig();
		VetoCtrlUpdates();
		SetCtrlFromValue();
		AllowCtrlUpdates();
	}
}

void GUIWizardPageConfig::UpdateScreen()
{
	if (titleCtrl!=NULL)
	{
		titleCtrl->SetLabel(title);
	}
	UpdateCtrlFromValue();
	GUIWizardPageStatsgen::UpdateScreen();
}

wxSize GUIWizardPageConfig::ResizeControl(int x,int y,int width,bool resize)
{
	int			titleX;
	int			titleY;
	int			titleWidth;
	int			titleHeight;
	wxSize		controlSize;

	if (titleCtrl!=NULL)
	{
		titleX			=x;
		titleY			=y;
		titleWidth		=width;
		titleHeight		=30;

		controlSize.SetWidth(titleWidth);
		controlSize.SetHeight(titleHeight);
		if (resize)
		{
			titleCtrl->SetSize(titleX,titleY,titleWidth,titleHeight);
		}
	}

	return (controlSize);
}

wxString GUIWizardPageConfig::GetValue()
{
	//UpdateValueFromConfig();
	return (value);
}

GUIWizardPageConfigText::GUIWizardPageConfigText(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	valueCtrl = FALSE;
	CreateControl();
}

GUIWizardPageConfigText::~GUIWizardPageConfigText()
{
	valueCtrl = FALSE;
}
void GUIWizardPageConfigText::CreateControl()
{
	valueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
}

wxSize GUIWizardPageConfigText::ResizeControl(int x,int y,int width,bool resize)
{
	wxSize		baseControlSize;
	wxSize		controlSize;
	int			baseX;
	int			baseY;
	int			baseWidth;
	int			baseHeight;
	int			valueX;
	int			valueY;
	int			valueWidth;
	int			valueHeight;

	baseX			= x;
	baseY			= y;
	baseControlSize = GUIWizardPageConfig::ResizeControl(baseX,baseY,width,resize);
	baseWidth		= baseControlSize.GetWidth();
	baseHeight		= baseControlSize.GetHeight();

	valueX			= x;
	valueY			= baseY + baseHeight;
	valueWidth		= width;
	valueHeight		= 30;

	controlSize.SetWidth(width);
	controlSize.SetHeight(baseHeight + valueHeight);

	if (resize)
	{
		valueCtrl->SetSize(valueX,valueY,valueWidth,valueHeight);
	}

	return (controlSize);
}

void GUIWizardPageConfigText::SetCtrlFromValue()
{
	valueCtrl->SetValue(value);
}

bool GUIWizardPageConfigText::ScreenReady()
{
	return (valueCtrl!=NULL);
}

void GUIWizardPageConfigText::SetValueFromCtrl()
{
	value=valueCtrl->GetValue();
}

GUIWizardPageRemoteTest::GUIWizardPageRemoteTest(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	statusCtrl		= new wxStaticText(this,-1,"Test Not Run");
	errorCtrl		= new wxRichTextCtrl(this,-1,"");
	errorCtrl->SetEditable(false);
	testButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Connection");
	lastConnectionResult = false;
	SetHelp("[b]Press the button[/b] to confirm FTP Access Details are correct");
}

GUIWizardPageRemoteTest::~GUIWizardPageRemoteTest()
{
	lastConnectionResult = false;
}

void GUIWizardPageRemoteTest::SetGroupPrefix(wxString &groupPrefixIn)
{
	groupPrefix		= groupPrefixIn;
}

bool GUIWizardPageRemoteTest::TestResult()
{
	return (lastConnectionResult);
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
	RemoteMachine		remoteMachine(groupPrefix);
	RestartingFTP				ftpConnection;
	
	lastConnectionResult	=remoteMachine.Connect(ftpConnection);

	if (lastConnectionResult)
	{
		statusCtrl->SetLabel("Connection Successful");
		errorCtrl->SetValue("");
	}
	else
	{
		errorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
		statusCtrl->SetLabel("Failed to connect");
	}

	return (lastConnectionResult);
}

void GUIWizardPageRemoteTest::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
	PageContentsChanged();
}

wxSize GUIWizardPageRemoteTest::ResizeControl(int x,int y,int width,bool resize)
{
	int			statusX;
	int			statusY;
	int			statusWidth;
	int			statusHeight;

	int			errorX;
	int			errorY;
	int			errorWidth;
	int			errorHeight;

	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;
	wxSize		objectSize;

	wxSize		controlSize;

	if (statusCtrl!=NULL)
	{
		objectSize		= testButton->GetSize();
		buttonX			=x;
		buttonY			=y;
		buttonWidth		=objectSize.GetWidth();
		buttonHeight	=objectSize.GetHeight();

		if (buttonWidth > width)
		{
			width = buttonWidth;
		}

		statusX			=x;
		statusY			=buttonY + buttonHeight;
		statusWidth		=width;
		statusHeight	=30;

		errorX			=x;
		errorY			=statusY + statusHeight;
		errorWidth		=width;
		errorHeight		=90;

		if (resize)
		{
			testButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
			statusCtrl->SetSize(statusX,statusY,statusWidth,statusHeight);
			errorCtrl->SetSize(errorX,errorY,errorWidth,errorHeight);
		}
		controlSize.SetWidth(width);
		controlSize.SetHeight(buttonHeight + statusHeight + errorHeight);
	}

	return (controlSize);
}

void GUIWizardPageRemoteTest::ResetTestResult()
{
	statusCtrl->SetLabel("Test Not Run");
	errorCtrl->SetValue("");
	lastConnectionResult = false;
}


GUIWizardPagesRemoteMachine::GUIWizardPagesRemoteMachine()
{
	pageHostname		= NULL;
	pageEnabled			= NULL;
	pageUsername		= NULL;
	pagePassword		= NULL;
	pageFTPPort			= NULL;
	pagePassive			= NULL;
	pageTest			= NULL;
	groupPrefix			= "";

	callBack			= NULL;
	callBackObject		= NULL;
}

GUIWizardPagesRemoteMachine::~GUIWizardPagesRemoteMachine()
{
}

void GUIWizardPagesRemoteMachine::SetCallBack(void(* callBackIn)(void *object),void *callBackObjectIn)
{
	callBack		= callBackIn;
	callBackObject	= callBackObjectIn;
}

void GUIWizardPagesRemoteMachine::SetSectionTitle(const char *titleChars)
{
	pageHostname->SetSectionTitle(titleChars);
	pageEnabled->SetSectionTitle(titleChars);
	pageUsername->SetSectionTitle(titleChars);
	pagePassword->SetSectionTitle(titleChars);
	pageFTPPort->SetSectionTitle(titleChars);
	pagePassive->SetSectionTitle(titleChars);
	pageTest->SetSectionTitle(titleChars);
}

bool GUIWizardPagesRemoteMachine::IsFTPEnabled()
{
	return (pageEnabled->GetValue().CmpNoCase("y")==0);
}

void GUIWizardPagesRemoteMachine::UpdateConfigKeys()
{
	wxString		configKey;

	if ((pageHostname != NULL) && (groupPrefix.length()>0))
	{
		configKey.Printf("/%s/IPAddress",groupPrefix.GetData());
		pageHostname->SetConfigKey(configKey);

		configKey.Printf("/%s/FTPUsername",groupPrefix.GetData());
		pageUsername->SetConfigKey(configKey);

		configKey.Printf("/%s/FTPPassword",groupPrefix.GetData());
		pagePassword->SetConfigKey(configKey);

		configKey.Printf("/%s/FTPPort",groupPrefix.GetData());
		pageFTPPort->SetConfigKey(configKey,"21");

		configKey.Printf("/%s/FTPPassive",groupPrefix.GetData());
		pagePassive->SetConfigKey(configKey,"y");

		configKey.Printf("/%s/FTPEnabled",groupPrefix.GetData());
		pageEnabled->SetConfigKey(configKey,"y");

		pageTest->SetGroupPrefix(groupPrefix);
	}
}

void GUIWizardPagesRemoteMachine::SetGroupPrefix(wxString &groupPrefixIn)
{
	groupPrefix = groupPrefixIn;

	UpdateConfigKeys();
}

wxString GUIWizardPagesRemoteMachine::GetEnabledPageID()
{
	return (pageEnabled->GetPageID());
}

void GUIWizardPagesRemoteMachine::CreateWizardPages(StatsgenWizard *wizardIn)
{
	wxString	ruleEnabled;
	wxString	ruleTestPassed;

	wizard=wizardIn;
	pageHostname = new GUIWizardPageConfigText(wizard,pageID,_T("HOSTNAME"));
	pageEnabled	 = new GUIWizardPageConfigBoolean(wizard,pageID,_T("FTPENABLED"));
	pageUsername = new GUIWizardPageConfigText(wizard,pageID,_T("FTPUSERNAME"));
	pagePassword = new GUIWizardPageConfigText(wizard,pageID,_T("FTPPASSWORD"));
	pageFTPPort	 = new GUIWizardPageConfigText(wizard,pageID,_T("FTPPORT"));
	pagePassive	 = new GUIWizardPageConfigBoolean(wizard,pageID,_T("FTPPASSIVE"));
	pageTest	 = new GUIWizardPageRemoteTest(wizard,pageID,_T("TEST"));

	pageHostname->SetConfigTitle("Hostname");
	pageHostname->SetHelp(
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

	pageEnabled->SetConfigTitle("Transfer Enabled?");
	pageEnabled->SetHelp("Enable / Disable FTP Transfer to this remote machine");
	pageEnabled->SetTrueFalseLabels("Yes", "No");

	pageUsername->SetConfigTitle("FTP Username");
	pageUsername->SetHelp("Enter the [b]FTP username[/b] used to access the hostname");

	pagePassword->SetConfigTitle("FTP Password");
	pagePassword->SetHelp("Enter the [b]FTP password[/b] used to access the hostname");

	pageFTPPort->SetConfigTitle("FTP Port");
	pageFTPPort->SetHelp(
			"Enter the [b]FTP port[/b] used to access the hostname\n"
			"This is typically 21"
						);

	pagePassive->SetConfigTitle("FTP Passive Mode?");
	pagePassive->SetHelp(
			"Enter whether in passive mode.\n"
			"This is typically On"
						);
	pagePassive->SetTrueFalseLabels("On", "Off");


	pageHostname->SetCallBack(ConfigChangedCallBack,this);
	pageEnabled->SetCallBack(ConfigChangedCallBack,this);
	pageUsername->SetCallBack(ConfigChangedCallBack,this);
	pagePassword->SetCallBack(ConfigChangedCallBack,this);
	pageFTPPort->SetCallBack(ConfigChangedCallBack,this);
	pagePassive->SetCallBack(ConfigChangedCallBack,this);
	pageTest->SetCallBack(ConnectionTestCallBack,this);

	ruleEnabled.Printf("%s=\"Y\"",pageEnabled->GetPageID().GetData());
	ruleTestPassed.Printf("%s=\"TRUE\"",pageTest->GetPageID().GetData());

	wizard->AddPageLink(pageHostname,	"",				"");
	wizard->AddPageLink(pageEnabled,	"",				"");
	wizard->AddPageLink(pageUsername,	ruleEnabled,	"");
	wizard->AddPageLink(pagePassword,	ruleEnabled,	"");
	wizard->AddPageLink(pageFTPPort,	ruleEnabled,	"");
	wizard->AddPageLink(pagePassive,	ruleEnabled,	"");
	wizard->AddPageLink(pageTest,		ruleEnabled,	ruleTestPassed);
	ResizePages();
}

void GUIWizardPagesRemoteMachine::ResizePages()
{
	pageHostname->Resize();
	pageEnabled->Resize();
	pageUsername->Resize();
	pagePassword->Resize();
	pageFTPPort->Resize();
	pagePassive->Resize();
	pageTest->Resize();
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
	if (callBack!=NULL)
	{
		callBack(callBackObject);
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
	valueCtrl		= FALSE;
	trueLabel		= "Yes";
	falseLabel		= "No";
	CreateControl();
}

GUIWizardPageConfigBoolean::~GUIWizardPageConfigBoolean()
{
	valueCtrl = FALSE;
}


void GUIWizardPageConfigBoolean::SetTrueFalseLabels(const char *trueLabelIn, const char *falseLabelIn)
{
	trueLabel	= trueLabelIn;
	falseLabel	= falseLabelIn;
}

void GUIWizardPageConfigBoolean::CreateControl()
{
	valueCtrl		= new wxCheckBox(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,title);
}

wxSize GUIWizardPageConfigBoolean::ResizeControl(int x,int y,int width,bool resize)
{
	wxSize		baseControlSize;
	wxSize		controlSize;
	int			baseX;
	int			baseY;
	int			baseWidth;
	int			baseHeight;
	int			valueX;
	int			valueY;
	int			valueWidth;
	int			valueHeight;

	baseX			= x;
	baseY			= y;
	baseControlSize = GUIWizardPageConfig::ResizeControl(baseX,baseY,width,resize);
	baseWidth		= baseControlSize.GetWidth();
	baseHeight		= baseControlSize.GetHeight();

	valueX			= x;
	valueY			= baseY + baseHeight;
	valueWidth		= width;
	valueHeight		= 30;

	controlSize.SetWidth(width);
	controlSize.SetHeight(baseHeight + valueHeight);

	if (resize)
	{
		valueCtrl->SetSize(valueX,valueY,valueWidth,valueHeight);
	}

	return (controlSize);
}

void GUIWizardPageConfigBoolean::UpdateLabel()
{
	bool		valueBool;
	wxString	titleStr;

	valueBool = (value.CmpNoCase("y")==0);
	titleStr=title;
	titleStr+=" ";
	if (valueBool)
	{
		titleStr+=trueLabel;
	}
	else
	{
		titleStr+=falseLabel;
	}
	valueCtrl->SetLabel(titleStr);
}

void GUIWizardPageConfigBoolean::SetCtrlFromValue()
{
	bool		valueBool;

	valueBool = (value.CmpNoCase("y")==0);
	valueCtrl->SetValue(valueBool);
	UpdateLabel();
}

bool GUIWizardPageConfigBoolean::ScreenReady()
{
	return (valueCtrl!=NULL);
}

void GUIWizardPageConfigBoolean::SetValueFromCtrl()
{
	if (valueCtrl->GetValue())
	{
		value="y";
	}
	else
	{
		value="n";
	}
	UpdateLabel();
}

GUIWizardPagesServer::GUIWizardPagesServer()
{
	pageServerType							= NULL;
	pageLocalOrRemote						= NULL;
	pageRCONPassword						= NULL;
	pageRCONPort							= NULL;
	pageMessagingEnabled					= NULL;
	pageRemoteLatestFilename				= NULL;
	pageRemoteLatestFilenameTest			= NULL;
	pageRemoteLatestSecondaryFilename		= NULL;
	pageRemoteLatestSecondaryFilenameTest	= NULL;
	pageRemoteArchiveFilename				= NULL;
	pageRemoteArchiveFilenameTest			= NULL;
	pageLocalLatestFilename					= NULL;
	pageLocalArchiveFilename				= NULL;
	pageLocalLatestSecondaryFilename		= NULL;
	groupPrefix								= "";

	logfileLimit					=0;

}

GUIWizardPagesServer::~GUIWizardPagesServer()
{
}

void GUIWizardPagesServer::UpdateConfigKeys()
{
	wxString		configKey;

	if ((pageServerType != NULL) && (groupPrefix.length()>0))
	{
		configKey.Printf("/%s/serverType",groupPrefix.GetData());
		pageServerType->SetConfigKey(configKey,SERVER_TYPE_COD4);

		configKey.Printf("/%s/localorremote",groupPrefix.GetData());
		pageLocalOrRemote->SetConfigKey(configKey,WIZARD_CHOICE_HOMEPC);

		configKey.Printf("/%s/RCONPassword",groupPrefix.GetData());
		pageRCONPassword->SetConfigKey(configKey);

		configKey.Printf("/%s/gamePort",groupPrefix.GetData());
		pageRCONPort->SetConfigKey(configKey);

		configKey.Printf("/%s/MessagingEnabled",groupPrefix.GetData());
		pageMessagingEnabled->SetConfigKey(configKey,"n");

		configKey.Printf("/%s/FTPLatest",groupPrefix.GetData());
		pageRemoteLatestFilename->SetConfigKey(configKey);
		pageRemoteLatestFilenameTest->SetGroupPrefix(groupPrefix);

		configKey.Printf("/%s/FTPSecondaryLatest",groupPrefix.GetData());
		pageRemoteLatestSecondaryFilename->SetConfigKey(configKey);
		pageRemoteLatestSecondaryFilenameTest->SetGroupPrefix(groupPrefix);

		configKey.Printf("/%s/FTPArchive",groupPrefix.GetData());
		pageRemoteArchiveFilename->SetConfigKey(configKey);
		pageRemoteArchiveFilenameTest->SetGroupPrefix(groupPrefix);

		configKey.Printf("/%s/latest",groupPrefix.GetData());
		pageLocalLatestFilename->SetConfigKey(configKey);

		configKey.Printf("/%s/archive",groupPrefix.GetData());
		pageLocalArchiveFilename->SetConfigKey(configKey);

		configKey.Printf("/%s/secondarylatest",groupPrefix.GetData());
		pageLocalLatestSecondaryFilename->SetConfigKey(configKey);

		configKey.Printf("/%s/BanFile1Type",groupPrefix.GetData());
		globalStatistics.configData.WriteTextValue(configKey,"COD");

		configKey.Printf("/%s/BanFile2Type",groupPrefix.GetData());
		globalStatistics.configData.WriteTextValue(configKey,"PUNKBUSTER");

		configKey.Printf("/%s/FTPBanFile2Directory",groupPrefix.GetData());
		pagePBRemoteDirectory->SetConfigKey(configKey);

		SetTestParameters();
		SetLogfileLimit();
	}
}

void GUIWizardPagesServer::SetTestParameters()
{
	wxString	file;
	wxString	directory;
	wxString	configKey;

	if (groupPrefix.Length()>0)
	{
		pageRemoteLatestFilename->SplitValue(directory,file);
		pageRemoteLatestFilenameTest->SetFile(directory,file);

		pageRemoteLatestSecondaryFilename->SplitValue(directory,file);
		pageRemoteLatestSecondaryFilenameTest->SetFile(directory,file);

		directory=pageRemoteArchiveFilename->GetValue();
		file=pageRemoteArchiveFilename->GetWildcard();
		pageRemoteArchiveFilenameTest->SetFile(directory,file);

		pagePBRemoteDirectoryTest->SetGroupPrefix(groupPrefix);

		configKey.Printf("/%s/RCONUsePunkBuster",groupPrefix.GetData());
		if (pageLocalOrRemote->GetValue().Cmp(WIZARD_CHOICE_HOMEPC)==0)
		{
			globalStatistics.configData.WriteTextValue(configKey,"Y");
		}
		else
		{
			globalStatistics.configData.WriteTextValue(configKey,"N");
		}
	}
}

void GUIWizardPagesServer::SetLogfileLimit(int limit)
{
	logfileLimit=limit;
	SetLogfileLimit();
}

void GUIWizardPagesServer::SetLogfileLimit()
{
	wxString	configKey;
	wxString	configValue;

	if (groupPrefix.Length()>0)
	{
		configKey.Printf("/%s/MaxLogfileSize",groupPrefix.GetData());
		configValue.Printf("%d",logfileLimit);
		globalStatistics.configData.WriteTextValue(configKey,configValue);
	}
}

void GUIWizardPagesServer::SetGroupPrefix(wxString &groupPrefixIn)
{
	groupPrefix = groupPrefixIn;

	remoteMachine.SetGroupPrefix(groupPrefix);
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

	wizard=wizardIn;

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
	pageServerType					= new GUIWizardPageConfigChoice(wizard,
											pageID,_T("SERVERTYPE"),
											serverTypeAnswers,
											serverTypeButtonTexts,
											serverTypeButtonDescriptions);
	pageLocalOrRemote				= new GUIWizardPageConfigChoice(wizard,
											pageID,_T("LOCALORREMOTE"),
											localOrRemoteAnswers,
											localOrRemoteButtonTexts,
											localOrRemoteButtonDescriptions);
	pageRCONPassword				= new GUIWizardPageConfigText(wizard,pageID,_T("RCONPASSWORD"));
	pageRCONPort					= new GUIWizardPageConfigText(wizard,pageID,_T("RCONPORT"));
	pagePBRemoteDirectory			= new GUIWizardPageConfigRemoteDirectory(wizard,pageID,_T("PBREMOTEDIRECTORY"));
	pagePBRemoteDirectoryTest		= new GUIWizardPageRemoteDirectoryTest(wizard,pageID,_T("PBREMOTEDIRECTORYTEST"));
	pageMessagingEnabled			= new GUIWizardPageConfigBoolean(wizard,pageID,_T("MESSAGINGENABLED"));
	pageMessagingEnabled			= new GUIWizardPageConfigBoolean(wizard,pageID,_T("MESSAGINGENABLED"));
	pageRemoteLatestFilename		= new GUIWizardPageConfigRemoteFile(wizard,pageID,_T("REMOTELATESTFILENAME"));
	pageRemoteLatestFilenameTest	= new GUIWizardPageRemoteFileTest(wizard,pageID,_T("REMOTELATESTFILENAMETEST"));
	pageRemoteLatestSecondaryFilename	= new GUIWizardPageConfigRemoteFile(wizard,pageID,_T("REMOTELATESTSECONDARYFILENAME"));
	pageRemoteLatestSecondaryFilenameTest	= new GUIWizardPageRemoteFileTest(wizard,pageID,_T("REMOTELATESTSECONDARYFILENAMETEST"));
	pageRemoteArchiveFilename		= new GUIWizardPageConfigRemoteWildcard(wizard,pageID,_T("REMOTEARCHIVEFILENAME"));
	pageRemoteArchiveFilenameTest	= new GUIWizardPageRemoteFileTest(wizard,pageID,_T("REMOTEARCHIVEFILENAMETEST"));
	pageLocalLatestFilename			= new GUIWizardPageConfigLocalFile(wizard,pageID,_T("LOCALLATESTFILENAME"));
	pageLocalArchiveFilename		= new GUIWizardPageConfigLocalWildcard(wizard,pageID,_T("LOCALARCHIVEFILENAME"));
	pageLocalLatestFilename			= new GUIWizardPageConfigLocalFile(wizard,pageID,_T("LOCALLATESTFILENAME"));
	pageLocalArchiveFilename		= new GUIWizardPageConfigLocalWildcard(wizard,pageID,_T("LOCALARCHIVEFILENAME"));
	pageLocalLatestSecondaryFilename= new GUIWizardPageConfigLocalFile(wizard,pageID,_T("LOCALLATESTSECONDARYFILENAME"));

	pageLocalOrRemote->SetHelp(
				"Depending on where you are running statsgen dictates various"
				" configurations.\nYou can run statsgen on the [b]game server[/b]"
				" itself, or if that is not available you can run statsgen"
				" from your [b]home pc[/b]."
				);
	pageLocalOrRemote->SetConfigTitle("Statsgen Location");

	pageServerType->SetConfigTitle("Game Server Type");
	pageServerType->SetHelp(
		"What sort of Server is this\n"
		);

	pageRCONPassword->SetConfigTitle("Game Server RCON Password?");
	pageRCONPassword->SetHelp("Enter the servers [b]RCON Password[/b].\n"
						"This is used to send messages to the game server.");

	pagePBRemoteDirectory->SetConfigTitle("Punkbuster Remote Directory");
	pagePBRemoteDirectory->SetHelp("Enter the location on your game server of the [b]Punkbuster Directory[/b]");

	pagePBRemoteDirectoryTest->SetHelp("[b]Press the button[/b] to test the Punkbuster Directory");

	pageRCONPort->SetConfigTitle("Game Server Port?");
	pageRCONPort->SetHelp("Enter the Game Servers Port Number.\n"
						"e.g.\n28960");

	pageMessagingEnabled->SetConfigTitle("Game Server Messaging Enabled?");
	pageMessagingEnabled->SetHelp("Do you want to send stats messages to this paticular game server?");
	pageMessagingEnabled->SetTrueFalseLabels("Yes", "No");

	pageRemoteLatestFilename->SetConfigTitle("Kills Logfile");
	pageRemoteLatestFilename->SetHelp(
			"The logfile on the game server where the [b]kills[/b] are stored, typically on COD"
			" games this is called [b]games_mp.log[/b]."
						);
	pageRemoteLatestFilenameTest->SetHelp(
			"[b]Push the button[/b] to ensure Kills logfile can be accessed."
						);

	pageRemoteLatestSecondaryFilename->SetConfigTitle("QuakeWars Objective Logfile");
	pageRemoteLatestSecondaryFilename->SetHelp(
			"The logfile on the game server where the [b]objectives[/b] are stored"
						);
	pageRemoteLatestSecondaryFilenameTest->SetHelp(
			"[b]Push the button[/b] to ensure objective logfile can be accessed.");

	pageLocalLatestFilename->SetConfigTitle("Local Logfile Name");
	pageLocalLatestFilename->SetHelp(
			"The logfile on the game server where the [b]kills[/b] are stored, typically on COD"
			" games this is called [b]games_mp.log[/b]."
						);

	pageLocalLatestSecondaryFilename->SetConfigTitle("Quakewars Objective Logfile Name");
	pageLocalLatestSecondaryFilename->SetHelp(
			"The logfile on the game server where the [b]objectives[/b] are stored"
						);
	pageLocalArchiveFilename->SetConfigTitle("Multiple Kill File Wildcard");
	pageLocalArchiveFilename->SetHelp(archiveHelp);
	pageRemoteArchiveFilename->SetConfigTitle("Multiple Kill File Wildcard");
	pageRemoteArchiveFilename->SetHelp(archiveHelp);
	pageRemoteArchiveFilenameTest->SetHelp("[b]Push the button[/b] to check that the logfiles are accessible");

	pageServerType->SetCallBack(ConfigChangedCallBack,this);
	pageLocalOrRemote->SetCallBack(ConfigChangedCallBack,this);
	pageRCONPassword->SetCallBack(ConfigChangedCallBack,this);
	pagePBRemoteDirectory->SetCallBack(ConfigChangedCallBack,this);
	pagePBRemoteDirectoryTest->SetCallBack(ConfigChangedCallBack,this);
	pageRCONPort->SetCallBack(ConfigChangedCallBack,this);
	pageMessagingEnabled->SetCallBack(ConfigChangedCallBack,this);
	pageRemoteLatestFilename->SetCallBack(ConfigChangedCallBack,this);
	pageRemoteLatestFilenameTest->SetCallBack(ConfigChangedCallBack,this);
	pageRemoteLatestSecondaryFilename->SetCallBack(ConfigChangedCallBack,this);
	pageRemoteLatestSecondaryFilenameTest->SetCallBack(ConfigChangedCallBack,this);
	pageRemoteArchiveFilename->SetCallBack(ConfigChangedCallBack,this);
	pageRemoteArchiveFilenameTest->SetCallBack(ConfigChangedCallBack,this);
	pageLocalLatestFilename->SetCallBack(ConfigChangedCallBack,this);
	pageLocalArchiveFilename->SetCallBack(ConfigChangedCallBack,this);
	pageLocalLatestSecondaryFilename->SetCallBack(ConfigChangedCallBack,this);



	ruleServerTypeContinue.Printf("%s!\"\"",pageServerType->GetPageID().GetData());
	ruleLocalOrRemoteContinue.Printf("%s!\"\"",pageLocalOrRemote->GetPageID().GetData());
	ruleLocalOrRemoteHomePC.Printf("%s=\"%s\"",pageLocalOrRemote->GetPageID().GetData(),WIZARD_CHOICE_HOMEPC);
	ruleLocalOrRemoteGameServer.Printf("%s=\"%s\"",pageLocalOrRemote->GetPageID().GetData(),WIZARD_CHOICE_GAMESERVER);

	ruleRemoteLatestFilenameTestPassed.Printf("%s=\"TRUE\"",pageRemoteLatestFilenameTest->GetPageID().GetData());
	ruleRemoteArchiveFilenameTestPassed.Printf("%s=\"TRUE\"",pageRemoteArchiveFilenameTest->GetPageID().GetData());
	rulePunkbusterTestPassed.Printf("%s=\"TRUE\"",pagePBRemoteDirectoryTest->GetPageID().GetData());

	ruleQuakewars.Printf("%s=\"%s\"",pageServerType->GetPageID().GetData(),SERVER_TYPE_QUAKEWARS);
	ruleQuakewarsHomePC.Printf("(%s)&(%s)",ruleLocalOrRemoteHomePC.GetData(),ruleQuakewars.GetData());
	ruleQuakewarsGameServer.Printf("(%s)&(%s)",ruleLocalOrRemoteGameServer.GetData(),ruleQuakewars.GetData());
	ruleMOHAA.Printf("%s=\"%s\"",pageServerType->GetPageID().GetData(),SERVER_TYPE_MOHAA);
	ruleMOHAAHomePC.Printf("(%s)&(%s)",ruleLocalOrRemoteHomePC.GetData(),ruleMOHAA.GetData());
	ruleMOHAAGameServer.Printf("(%s)&(%s)",ruleLocalOrRemoteGameServer.GetData(),ruleMOHAA.GetData());
	ruleMessagingServers.Printf("((%s=\"%s\")|(%s=\"%s\")|(%s=\"%s\")|(%s=\"%s\")|(%s=\"%s\"))",
			pageServerType->GetPageID().GetData(),SERVER_TYPE_COD1,
			pageServerType->GetPageID().GetData(),SERVER_TYPE_COD2,
			pageServerType->GetPageID().GetData(),SERVER_TYPE_COD4,
			pageServerType->GetPageID().GetData(),SERVER_TYPE_COD5,
			pageServerType->GetPageID().GetData(),SERVER_TYPE_QUAKEWARS);
	ruleMessagingEnabled.Printf("(%s=\"%s\")", pageMessagingEnabled->GetPageID().GetData(),"Y");
	ruleMessaging.Printf("%s&%s",ruleMessagingServers.GetData(),ruleMessagingEnabled.GetData());
	rulePunkbuster.Printf("(%s)&(%s)",ruleMessaging.GetData(),ruleLocalOrRemoteHomePC.GetData());
	ruleHomePCNotMOHAA.Printf("(%s)&(%s!\"%s\")",
								ruleLocalOrRemoteHomePC.GetData(),
								pageServerType->GetPageID().GetData(),
								SERVER_TYPE_MOHAA);
	ruleGameServerNotMOHAA.Printf("(%s)&(%s!\"%s\")",
								ruleLocalOrRemoteGameServer.GetData(),
								pageServerType->GetPageID().GetData(),
								SERVER_TYPE_MOHAA);

	wizard->AddPageLink(pageServerType,					""							,ruleServerTypeContinue);
	wizard->AddPageLink(pageLocalOrRemote,				""							,ruleLocalOrRemoteContinue);
	remoteMachine.SetPageID(pageID,"REMOTEMACHINE");
	remoteMachine.CreateWizardPages(wizard);
	wizard->AddPageLink(pageRCONPort,					""				,"");
	wizard->AddPageLink(pageMessagingEnabled,			ruleMessagingServers		,"");
	wizard->AddPageLink(pageRCONPassword,				ruleMessaging				,"");
	wizard->AddPageLink(pagePBRemoteDirectory,			rulePunkbuster				,"");
	wizard->AddPageLink(pagePBRemoteDirectoryTest,		rulePunkbuster				,rulePunkbusterTestPassed);
	wizard->AddPageLink(pageRemoteLatestFilename,		ruleHomePCNotMOHAA		,"");
	wizard->AddPageLink(pageRemoteLatestFilenameTest,	ruleHomePCNotMOHAA		,ruleRemoteLatestFilenameTestPassed);
	wizard->AddPageLink(pageRemoteLatestSecondaryFilename,		ruleQuakewarsHomePC		,"");
	wizard->AddPageLink(pageRemoteLatestSecondaryFilenameTest,	ruleQuakewarsHomePC		,ruleRemoteLatestFilenameTestPassed);
	wizard->AddPageLink(pageRemoteArchiveFilename,		ruleMOHAAHomePC		,"");
	wizard->AddPageLink(pageRemoteArchiveFilenameTest,	ruleMOHAAHomePC		,ruleRemoteArchiveFilenameTestPassed);
	wizard->AddPageLink(pageLocalLatestFilename,		ruleGameServerNotMOHAA	,"");
	wizard->AddPageLink(pageLocalArchiveFilename,		ruleMOHAAGameServer	,"");
	wizard->AddPageLink(pageLocalLatestSecondaryFilename,		ruleQuakewarsGameServer	,"");

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

	ResizePages();
}

void GUIWizardPagesServer::SetSectionTitle(const char *sectionTitle)
{
	pageServerType->SetSectionTitle(sectionTitle);
	pageLocalOrRemote->SetSectionTitle(sectionTitle);
	pageRCONPassword->SetSectionTitle(sectionTitle);
	pageRCONPort->SetSectionTitle(sectionTitle);
	pagePBRemoteDirectory->SetSectionTitle(sectionTitle);
	pagePBRemoteDirectoryTest->SetSectionTitle(sectionTitle);
	pageMessagingEnabled->SetSectionTitle(sectionTitle);
	pageRemoteLatestFilename->SetSectionTitle(sectionTitle);
	pageRemoteLatestFilenameTest->SetSectionTitle(sectionTitle);
	pageRemoteLatestSecondaryFilename->SetSectionTitle(sectionTitle);
	pageRemoteLatestSecondaryFilenameTest->SetSectionTitle(sectionTitle);
	pageLocalLatestFilename->SetSectionTitle(sectionTitle);
	pageLocalLatestSecondaryFilename->SetSectionTitle(sectionTitle);
	pageRemoteArchiveFilename->SetSectionTitle(sectionTitle);
	pageRemoteArchiveFilenameTest->SetSectionTitle(sectionTitle);
	pageLocalArchiveFilename->SetSectionTitle(sectionTitle);
	remoteMachine.SetSectionTitle(sectionTitle);
}

void GUIWizardPagesServer::ResizePages()
{
	pageServerType->Resize();
	pageLocalOrRemote->Resize();
	pageRCONPassword->Resize();
	pageRCONPort->Resize();
	pagePBRemoteDirectory->Resize();
	pagePBRemoteDirectoryTest->Resize();
	pageMessagingEnabled->Resize();
	pageRemoteLatestFilename->Resize();
	pageRemoteLatestFilenameTest->Resize();
	pageRemoteLatestSecondaryFilename->Resize();
	pageRemoteLatestSecondaryFilenameTest->Resize();
	pageLocalLatestFilename->Resize();
	pageLocalLatestSecondaryFilename->Resize();
	pageRemoteArchiveFilename->Resize();
	pageRemoteArchiveFilenameTest->Resize();
	pageLocalArchiveFilename->Resize();
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

	baseDirectory.AppendDir(groupPrefix);
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

	pageLocalLatestFilename->SetDefault(logDirectoryName);
	pageLocalLatestFilename->UpdateValueFromConfig();
	pageLocalLatestSecondaryFilename->SetDefault(logDirectoryName);
	pageLocalLatestSecondaryFilename->UpdateValueFromConfig();
	pageLocalArchiveFilename->SetDefault(archiveDirectoryName);
	pageLocalArchiveFilename->UpdateValueFromConfig();


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
	selectionLabel		= NULL;
	selectedChoice		= NULL;
	choice				= "";

	UpdateChoices(answerCodesIn,buttonTextsIn,buttonDescriptionsIn);
}

GUIWizardPageChoice::~GUIWizardPageChoice()
{
}

void GUIWizardPageChoice::UpdateChoices(
						wxArrayString &answerCodesIn,
						wxArrayString &buttonTextsIn,
						wxArrayString &buttonDescriptionsIn)
{
	int				buttonCount;
	int				buttonIndex;
	wxButton		*button;
	wxStaticText	*description;
	int				answerCount;
	int				answerIndex;
	wxString		answerCode;
	wxString		buttonText;
	wxString		buttonDescription;

	// Remove existing stuff
	buttonCount=buttons.GetCount();
	for (buttonIndex=0;buttonIndex<buttonCount;buttonIndex++)
	{
		button		= (wxButton *)buttons.Item(buttonIndex);
		description	= (wxStaticText *)labels.Item(buttonIndex);
		button->Destroy();
		description->Destroy();
	}
	buttons.Clear();
	labels.Clear();

	// Create new buttons and labels
	answerCodes 		= answerCodesIn;
	buttonTexts			= buttonTextsIn;
	buttonDescriptions	= buttonDescriptionsIn;

	answerCount = answerCodes.GetCount();
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		answerCode			= answerCodes.Item(answerIndex);
		buttonText			= buttonTexts.Item(answerIndex);
		buttonDescription	= buttonDescriptions.Item(answerIndex);
		button				= new wxButton(this,
										WINDOW_ID_BUTTON_NEW,
										buttonText);
		description			= new wxStaticText(this, -1, buttonDescription);
		buttons.Add(button);
		labels.Add(description);
	}
	if (selectedChoice == NULL)
	{
		selectedChoice	= new wxStaticText(this, -1, "");
		selectionLabel	= new wxStaticText(this,-1,"Current Selection: ");
	}
	SetSelection("Nothing");
	if (answerCodes.GetCount()==1)
	{
		choice = buttonTexts.Item(0);
		SetSelection(choice);
		choice = answerCodes.Item(0);
	}
	Resize();

}

void GUIWizardPageChoice::SetSelection(const char *selection)
{
	wxString	selectionStr;

	selectionStr=selection;
	SetSelection(selectionStr);
}

void GUIWizardPageChoice::SetSelection(wxString &selection)
{
	selectedChoice->SetLabel(selection);
}

void GUIWizardPageChoice::OnButtonPressed(wxCommandEvent &event)
{
	int			answerCount;
	int			answerIndex;
	wxButton	*button=NULL;
	wxString	selection;
	

	answerCount = buttons.GetCount();

	choice = "";
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		button=(wxButton *)buttons.Item(answerIndex);
		if (button == event.GetEventObject())
		{
			choice = answerCodes.Item(answerIndex);
			selection = buttonTexts.Item(answerIndex);
			SetSelection(selection);
			break;
		}
	}
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
	PageContentsChanged();

}

wxString GUIWizardPageChoice::GetChoice()
{
	return (choice);
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

	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		button=(wxButton *)buttons.Item(answerIndex);
		choice = answerCodes.Item(answerIndex);
		if (choice.CmpNoCase(choiceIn)==0)
		{
			selection = buttonTexts.Item(answerIndex);
			SetSelection(selection);
			break;
		}
		choice = "";
	}
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
}

wxSize GUIWizardPageChoice::ResizeControl(int x,int y,int width,bool resize)
{

	int				selectionX;
	int				selectionY;
	int				selectionWidth;
	int				selectionHeight;

	int				labelX;
	int				labelY;
	int				labelWidth;
	int				labelHeight;

	int				buttonX;
	int				buttonY;
	int				buttonWidth;
	int				buttonHeight;
	wxSize			objectSize;
	int				answerCount;
	int				answerIndex;
	wxButton		*button;
	wxStaticText	*label;
	int				maxButtonWidth;
	int				currentY;
	int				lineHeight;
	int				selectionLabelX;
	int				selectionLabelY;
	int				selectionLabelWidth;
	int				selectionLabelHeight;

	wxSize		controlSize;

	if (selectedChoice!=NULL)
	{
		answerCount = buttons.GetCount();

		maxButtonWidth	= 0;

		for (answerIndex = 0; answerIndex<answerCount; answerIndex++)
		{
			button			= (wxButton *)buttons.Item(answerIndex);
			objectSize		= button->GetSize();
			buttonWidth		= objectSize.GetWidth();
			buttonHeight	= objectSize.GetHeight();
			if (buttonWidth > maxButtonWidth)
			{
				maxButtonWidth = buttonWidth;
			}
		}
		if (maxButtonWidth > width)
		{
			width = maxButtonWidth;
		}
		// Make the width at least 100 more than the button sizes
		if ((width - maxButtonWidth) <= 100)
		{
			width = width + 100;
		}

		currentY = y;
		for (answerIndex = 0; answerIndex<answerCount; answerIndex++)
		{
			button			= (wxButton *)buttons.Item(answerIndex);
			objectSize		= button->GetSize();
			buttonX			= x;
			buttonY			= currentY;
			buttonWidth		= maxButtonWidth;
			buttonHeight	= objectSize.GetHeight();
		
			label			= (wxStaticText *)labels.Item(answerIndex);
			labelX			= maxButtonWidth + 10;
			labelY			= currentY;
			labelWidth		= width - (maxButtonWidth + 10);
			labelHeight		= 30;

			if (labelHeight > buttonHeight)
			{
				lineHeight	= labelHeight;
			}
			else
			{
				lineHeight	= buttonHeight;
			}

			if (resize)
			{
				button->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
				label->SetSize(labelX,labelY,labelWidth,labelHeight);
			}
			currentY+=lineHeight;
			currentY+=10;
		}

		selectionLabelX			=x;
		selectionLabelY			=currentY;
		selectionLabelWidth		=100;
		selectionLabelHeight	=30;

		selectionX		=selectionLabelX + selectionLabelWidth;
		selectionY		=currentY;
		selectionHeight	=30;
		selectionWidth	=width;

		if (resize)
		{
			selectionLabel->SetSize(selectionLabelX,selectionLabelY,
								selectionLabelWidth,selectionLabelHeight);
			selectedChoice->SetSize(selectionX,selectionY,
							selectionWidth,selectionHeight);

		}

		currentY+=selectionHeight;

		controlSize.SetWidth(width);
		controlSize.SetHeight(currentY - y);
	}

	return (controlSize);
}

GUIWizardPageConfigFile::GUIWizardPageConfigFile(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	valueCtrl		= FALSE;
	browseButton	= FALSE;
	CreateControl();
}

GUIWizardPageConfigFile::~GUIWizardPageConfigFile()
{
	valueCtrl		= FALSE;
	browseButton	= FALSE;
}

void GUIWizardPageConfigFile::CreateControl()
{
	valueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	browseButton	= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Browse For File");
}

wxSize GUIWizardPageConfigFile::ResizeControl(int x,int y,int width,bool resize)
{
	wxSize		baseControlSize;
	wxSize		controlSize;
	int			baseX;
	int			baseY;
	int			baseWidth;
	int			baseHeight;
	int			valueX;
	int			valueY;
	int			valueWidth;
	int			valueHeight;
	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;
	wxSize		objectSize;

	objectSize		= browseButton->GetSize();
	buttonWidth		= objectSize.GetWidth();
	buttonHeight	= objectSize.GetHeight();

	if (width < buttonWidth)
	{
		width = buttonWidth;
	}

	baseX			= x;
	baseY			= y;
	baseControlSize = GUIWizardPageConfig::ResizeControl(baseX,baseY,width,resize);
	baseWidth		= baseControlSize.GetWidth();
	baseHeight		= baseControlSize.GetHeight();

	valueX			= x;
	valueY			= baseY + baseHeight;
	valueWidth		= width;
	valueHeight		= 30;

	buttonX			= x;
	buttonY			= valueY + valueHeight;

	controlSize.SetWidth(width);
	controlSize.SetHeight(baseHeight + valueHeight + buttonHeight);

	if (resize)
	{
		valueCtrl->SetSize(valueX,valueY,valueWidth,valueHeight);
		browseButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
	}

	return (controlSize);
}

void GUIWizardPageConfigFile::SetCtrlFromValue()
{
	valueCtrl->SetValue(value);
}

bool GUIWizardPageConfigFile::ScreenReady()
{
	return (valueCtrl!=NULL);
}

void GUIWizardPageConfigFile::SetValueFromCtrl()
{
	value = valueCtrl->GetValue();
}

void GUIWizardPageConfigFile::GetConfigKeys(wxString &configKeyDirectory,
													wxString &configKeyFilename)
{
	configKeyDirectory	= configKey + "Directory";
	configKeyFilename	= configKey + "Filename";
}

void GUIWizardPageConfigFile::SplitValue(wxString &valueDirectory,
													wxString &valueFilename)
{
	wxFileName	filename(value,wxPATH_UNIX);

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
		value=userFilename;
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
		SplitValue(directory,filename);
		globalStatistics.configData.WriteTextValue(configKeyDirectory,directory);
		globalStatistics.configData.WriteTextValue(configKeyFilename,filename);
		if (callBack!=NULL)
		{
			callBack(callBackObject);
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
		globalStatistics.configData.ReadTextValue(configKeyDirectory,&directory);
		globalStatistics.configData.ReadTextValue(configKeyFilename,&filename);
		if ((directory.Length()==0)&&(filename.Length()==0)&&(defaultValue.Length()>0))
		{
			wxFileName	defaultFilename(defaultValue);
			directory=defaultFilename.GetPath();
			filename=defaultFilename.GetFullName();
			globalStatistics.configData.WriteTextValue(configKeyDirectory,directory);
			globalStatistics.configData.WriteTextValue(configKeyFilename,filename);
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

	groupPrefix = configKey.AfterFirst('/');
	groupPrefix = groupPrefix.BeforeFirst('/');

	GenericOKCancelDialog dialog(this,-1,
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
										directory,
										true);
	browserPanel->Create(&dialog,
						-1,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	browserPanel->CreateScreen();
	
	dialog.SetPanel(browserPanel);
	dialog.CreateDialog();
	result=(dialog.ShowModal()==WINDOW_ID_BUTTON_SAVE);
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
	value= directory;
	value+="/";
	value+=filename;
}

void GUIWizardPageConfigRemoteFile::SplitValue(wxString &valueDirectory,
													wxString &valueFilename)
{
	wxFileName	filename(value,wxPATH_UNIX);

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
	wxFileName	filenameStr(value);

	valueFilename	= filenameStr.GetFullName();
	valueDirectory	= filenameStr.GetPath();
}

void GUIWizardPageConfigLocalFile::CombineValue()
{
	wxFileName	filenameStr;

	filenameStr.SetFullName(filename);
	filenameStr.SetPath(directory);

	value = filenameStr.GetFullPath();
}

wxString GUIWizardPageConfigLocalFile::RequestFileFromUser()
{
	wxString	message;
	wxString	result;
	wxString	userFile;

	message="Select File";

	result=wxFileSelector(message,directory);
	if (result.Length()>0)
	{
		userFile=result;
	}

	return (userFile);
}

GUIWizardPageRemoteFileTest::GUIWizardPageRemoteFileTest(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	statusCtrl		= new wxStaticText(this,-1,"Test Not Run");
	errorCtrl		= new wxRichTextCtrl(this,-1,"");
	errorCtrl->SetEditable(false);
	testButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Remote File");
	fileDirectory	= "";
	lastResult = false;
}

GUIWizardPageRemoteFileTest::~GUIWizardPageRemoteFileTest()
{
	lastResult = false;
}

void GUIWizardPageRemoteFileTest::SetGroupPrefix(wxString &groupPrefixIn)
{
	groupPrefix		= groupPrefixIn;
}

bool GUIWizardPageRemoteFileTest::TestResult()
{
	return (lastResult);
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
	RemoteMachine		remoteMachine(groupPrefix);
	RestartingFTP				ftpConnection;
	wxString			msg;
	int					fileCount;
	wxString			filename;
	
	statusCtrl->SetLabel("Connecting To Remote Host");
	errorCtrl->SetValue("");
	lastResult	=remoteMachine.Connect(ftpConnection);

	if (lastResult)
	{
		statusCtrl->SetLabel("Retrieving Directory Listing");
		lastResult = remoteMachine.GetRemoteDirectoryListing(fileDirectory,fileName,filenameList);

		if (lastResult)
		{
			fileCount = filenameList.GetCount();
			if (fileCount == 0)
			{
				statusCtrl->SetLabel("Retrieving Directory Listing: Failed");
				errorCtrl->SetLabel("Directory listing received, but no files present");
				lastResult = false;
			}
			else
			{
				msg.Printf("Retrieving Directory Listing: OK - %d files located",fileCount);
				statusCtrl->SetLabel(msg);
			}
		}
		else
		{
			statusCtrl->SetLabel("Retrieving Directory Listing: Failed");
			errorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
		}
	}
	else
	{
		statusCtrl->SetLabel("Connecting To Remote Host: Failed");
		errorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
	}

	return (lastResult);
}

void GUIWizardPageRemoteFileTest::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
	PageContentsChanged();
}

wxSize GUIWizardPageRemoteFileTest::ResizeControl(int x,int y,int width,bool resize)
{
	int			statusX;
	int			statusY;
	int			statusWidth;
	int			statusHeight;

	int			errorX;
	int			errorY;
	int			errorWidth;
	int			errorHeight;

	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;
	wxSize		objectSize;

	wxSize		controlSize;

	if (statusCtrl!=NULL)
	{
		objectSize		= testButton->GetSize();
		buttonX			=x;
		buttonY			=y;
		buttonWidth		=objectSize.GetWidth();
		buttonHeight	=objectSize.GetHeight();

		if (buttonWidth > width)
		{
			width = buttonWidth;
		}

		statusX			=x;
		statusY			=buttonY + buttonHeight;
		statusWidth		=width;
		statusHeight	=30;

		errorX			=x;
		errorY			=statusY + statusHeight;
		errorWidth		=width;
		errorHeight		=90;

		if (resize)
		{
			testButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
			statusCtrl->SetSize(statusX,statusY,statusWidth,statusHeight);
			errorCtrl->SetSize(errorX,errorY,errorWidth,errorHeight);
		}
		controlSize.SetWidth(width);
		controlSize.SetHeight(buttonHeight + statusHeight + errorHeight);
	}

	return (controlSize);
}

void GUIWizardPageRemoteFileTest::ResetTestResult()
{
	statusCtrl->SetLabel("Test Not Run");
	errorCtrl->SetValue("");
	lastResult = false;
	filenameList.Clear();
	filesizeList.Clear();
}

void GUIWizardPageRemoteFileTest::SetFile(wxString &fileDirectoryIn, wxString &fileNameIn)
{
	fileDirectory	= fileDirectoryIn;
	fileName		= fileNameIn;
}

GUIWizardPageConfigChoice::GUIWizardPageConfigChoice(StatsgenWizard *wizardIn,
						wxString parentPageID,
						wxString thisPageID,
						wxArrayString &answerCodesIn,
						wxArrayString &buttonTextsIn,
						wxArrayString &buttonDescriptionsIn) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	valueCtrl			= FALSE;
	answerCodes 		= answerCodesIn;
	buttonTexts			= buttonTextsIn;
	buttonDescriptions	= buttonDescriptionsIn;

	choice				= "";
	CreateControl();
}

GUIWizardPageConfigChoice::~GUIWizardPageConfigChoice()
{
	valueCtrl = FALSE;
}
void GUIWizardPageConfigChoice::CreateControl()
{
	int				answerCount;
	int				answerIndex;
	wxButton		*button;
	wxStaticText	*description;
	wxString		answerCode;
	wxString		buttonText;
	wxString		buttonDescription;


	answerCount = answerCodes.GetCount();
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		answerCode			= answerCodes.Item(answerIndex);
		buttonText			= buttonTexts.Item(answerIndex);
		buttonDescription	= buttonDescriptions.Item(answerIndex);
		button				= new wxButton(this,
										WINDOW_ID_BUTTON_NEW,
										buttonText);
		description			= new wxStaticText(this, -1, buttonDescription);
		buttons.Add(button);
		labels.Add(description);
	}
	valueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	selectionLabel	= new wxStaticText(this,-1,"Current Selection: ");
	SetSelection("Nothing");
	if (answerCodes.GetCount()==1)
	{
		choice = buttonTexts.Item(0);
		SetSelection(choice);
		choice = answerCodes.Item(0);
	}
}

void GUIWizardPageConfigChoice::SetCtrlFromValue()
{
	valueCtrl->SetValue(value);
}

bool GUIWizardPageConfigChoice::ScreenReady()
{
	return (valueCtrl!=NULL);
}

void GUIWizardPageConfigChoice::SetValueFromCtrl()
{
	value=valueCtrl->GetValue();
}

void GUIWizardPageConfigChoice::SetSelection(const char *selection)
{
	wxString	selectionStr;

	selectionStr=selection;
}

void GUIWizardPageConfigChoice::SetSelection(wxString &selection)
{
	valueCtrl->SetValue(selection);
}

void GUIWizardPageConfigChoice::OnButtonPressed(wxCommandEvent &event)
{
	int			answerCount;
	int			answerIndex;
	wxButton	*button=NULL;
	wxString	selection;
	

	answerCount = buttons.GetCount();

	choice = "";
	for (answerIndex = 0; answerIndex < answerCount; answerIndex++)
	{
		button=(wxButton *)buttons.Item(answerIndex);
		if (button == event.GetEventObject())
		{
			choice = answerCodes.Item(answerIndex);
			selection = buttonTexts.Item(answerIndex);
			SetSelection(choice);
			break;
		}
	}
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}

	PageContentsChanged();
}

wxString GUIWizardPageConfigChoice::GetChoice()
{
	return (choice);
}

wxSize GUIWizardPageConfigChoice::ResizeControl(int x,int y,int width,bool resize)
{

	int				selectionX;
	int				selectionY;
	int				selectionWidth;
	int				selectionHeight;

	int				labelX;
	int				labelY;
	int				labelWidth;
	int				labelHeight;

	int				buttonX;
	int				buttonY;
	int				buttonWidth;
	int				buttonHeight;
	wxSize			objectSize;
	int				answerCount;
	int				answerIndex;
	wxButton		*button;
	wxStaticText	*label;
	int				maxButtonWidth;
	int				currentY;
	int				lineHeight;
	int				baseX;
	int				baseY;
	int				baseHeight;
	int				baseWidth;
	wxSize			baseSize;
	int				selectionLabelX;
	int				selectionLabelY;
	int				selectionLabelWidth;
	int				selectionLabelHeight;

	wxSize		controlSize;
	wxSize		baseControlSize;

	if (valueCtrl!=NULL)
	{
		answerCount = buttons.GetCount();

		maxButtonWidth	= 0;

		for (answerIndex = 0; answerIndex<answerCount; answerIndex++)
		{
			button			= (wxButton *)buttons.Item(answerIndex);
			objectSize		= button->GetSize();
			buttonWidth		= objectSize.GetWidth();
			buttonHeight	= objectSize.GetHeight();
			if (buttonWidth > maxButtonWidth)
			{
				maxButtonWidth = buttonWidth;
			}
		}
		if (maxButtonWidth > width)
		{
			width = maxButtonWidth;
		}
		// Make the width at least 100 more than the button sizes
		if ((width - maxButtonWidth) <= 100)
		{
			width = width + 100;
		}

		baseX			= x;
		baseY			= y;
		baseControlSize = GUIWizardPageConfig::ResizeControl(baseX,baseY,width,resize);
		baseWidth		= baseControlSize.GetWidth();
		baseHeight		= baseControlSize.GetHeight();

		currentY = baseY + baseHeight;

		for (answerIndex = 0; answerIndex<answerCount; answerIndex++)
		{
			button			= (wxButton *)buttons.Item(answerIndex);
			objectSize		= button->GetSize();
			buttonX			= x;
			buttonY			= currentY;
			buttonWidth		= maxButtonWidth;
			buttonHeight	= objectSize.GetHeight();
		
			label			= (wxStaticText *)labels.Item(answerIndex);
			labelX			= maxButtonWidth + 10;
			labelY			= currentY;
			labelWidth		= width - (maxButtonWidth + 10);
			labelHeight		= 30;

			if (labelHeight > buttonHeight)
			{
				lineHeight	= labelHeight;
			}
			else
			{
				lineHeight	= buttonHeight;
			}

			if (resize)
			{
				button->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
				label->SetSize(labelX,labelY,labelWidth,labelHeight);
			}
			currentY+=lineHeight;
			currentY+=10;
		}

		selectionLabelX			=x;
		selectionLabelY			=currentY;
		selectionLabelWidth		=100;
		selectionLabelHeight	=30;

		selectionX		=selectionLabelX + selectionLabelWidth;
		selectionY		=currentY;
		selectionHeight	=30;
		selectionWidth	=width;

		if (resize)
		{
			selectionLabel->SetSize(selectionLabelX,selectionLabelY,
								selectionLabelWidth,selectionLabelHeight);
			valueCtrl->SetSize(selectionX,selectionY,
							selectionWidth,selectionHeight);

		}

		currentY+=selectionHeight;

		controlSize.SetWidth(width);
		controlSize.SetHeight(currentY - y);
	}

	return (controlSize);
}

GUIWizardPagesServers::GUIWizardPagesServers()
{
	pageDeleteEditNew			= NULL;

}

GUIWizardPagesServers::~GUIWizardPagesServers()
{
}

void GUIWizardPagesServers::SetLogfileLimit(int limit)
{
	pageServer.SetLogfileLimit(limit);
}

void GUIWizardPagesServers::SetLogfileLimit()
{
	pageServer.SetLogfileLimit();
}

void GUIWizardPagesServers::SetDeleteEditNewChoices()
{
	wxArrayString	deleteEditNewAnswers;
	wxArrayString	deleteEditNewButtonTexts;
	wxArrayString	deleteEditNewButtonDescriptions;
	wxString		listGroup	="SERVERS";
	wxArrayString	serverIDs;
	int				serverCount;

	globalStatistics.configData.ReadList(listGroup,serverIDs);
	serverCount=serverIDs.GetCount();

	deleteEditNewAnswers.Add(WIZARD_CHOICE_NEW);
	deleteEditNewButtonTexts.Add("New");
	deleteEditNewButtonDescriptions.Add("Create New Server");
	if (serverCount > 0)
	{
		deleteEditNewAnswers.Add(WIZARD_CHOICE_EDIT);
		deleteEditNewButtonTexts.Add("Edit");
		deleteEditNewButtonDescriptions.Add("Edit Existing Server");
		deleteEditNewAnswers.Add(WIZARD_CHOICE_DELETE);
		deleteEditNewButtonTexts.Add("Delete");
		deleteEditNewButtonDescriptions.Add("Delete Existing Server");
	}

	pageDeleteEditNew->UpdateChoices(
									deleteEditNewAnswers,
									deleteEditNewButtonTexts,
									deleteEditNewButtonDescriptions);
	/*
	if (serverCount>0)
	{
		pageDeleteEditNew->SetSelection(WIZARD_CHOICE_EDIT);
	}
	*/

	pageDeleteEditNew->Resize();
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

	wizard = wizardIn;
	
	id="FINISHED";

	pageDeleteEditNew			= new GUIWizardPageChoice(wizard,
											pageID,_T("DELETEEDITNEW"),
											deleteEditNewAnswers,
											deleteEditNewButtonTexts,
											deleteEditNewButtonDescriptions);
	pageServerSelection			= new GUIWizardPageChoice(wizard,
											pageID,_T("SERVERSELECTION"),
											serverAnswers,
											serverButtonTexts,
											serverButtonDescriptions);
	SetDeleteEditNewChoices();
	SetServerChoices();
	pageServerSelection->SetHelp("select a server to modify");
	pageServerSelection->SetCallBack(ServerSelectedCallBack,this);


	pageDeleteEditNew->SetHelp(
				"Do you want to create a [b]new[/b] server configuration, "
				"[b]edit[/b] an existing one, or [b]delete[/b] an existing one.\n"
				"\n"
				"[b]Press the appropriate button[/b]."
				);

	pageDeleteEditNew->SetCallBack(DeleteEditNewCallBack,this);


	pageServer.SetPageID(pageID,"SERVER");
	ruleDeleteEditNewContinue.Printf("(%s=\"%s\")|(%s=\"%s\")",
						pageDeleteEditNew->GetPageID().GetData(),
						WIZARD_CHOICE_EDIT,
						pageDeleteEditNew->GetPageID().GetData(),
						WIZARD_CHOICE_NEW);
	ruleServerSelection.Printf("%s=\"%s\"",pageDeleteEditNew->GetPageID().GetData(),WIZARD_CHOICE_EDIT);
	ruleServerSelectionContinue.Printf("%s!\"\"",pageServerSelection->GetPageID().GetData());

	wizard->AddPageLink(pageDeleteEditNew,	"",					ruleDeleteEditNewContinue);
	wizard->AddPageLink(pageServerSelection,ruleServerSelection,ruleServerSelectionContinue);
	pageServer.CreateWizardPages(wizard);

	pageDeleteEditNew->SetSectionTitle(sectionTitle);
	pageServerSelection->SetSectionTitle(sectionTitle);
	pageServer.SetSectionTitle(sectionTitle);
	ResizePages();
}

void GUIWizardPagesServers::ResizePages()
{
	pageDeleteEditNew->Resize();
	pageServerSelection->Resize();
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

	pageServerSelection->UpdateChoices(serverAnswers,
										serverButtonTexts,
										serverButtonDescriptions);
}

wxString GUIWizardPagesServers::GetDeleteEditNewChoice()
{
	return (pageDeleteEditNew->GetChoice());
}

void GUIWizardPagesServers::PrepareServerConfig()
{
	pageServer.PrepareServerConfig();
}

void GUIWizardPagesServers::DeleteEditNewCallBack(void *object)
{
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
		serversWizard->SelectServer();
	}
	else
	if (deleteEditNewChoice.CmpNoCase(WIZARD_CHOICE_DELETE)==0)
	{
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

	serverConfigPrefix=pageServerSelection->GetChoice();
	pageServer.SetGroupPrefix(serverConfigPrefix);
	pageServer.SetLogfileLimit();
}

void GUIWizardPagesServers::SelectServer(wxString &choice)
{
	wxString	deleteEditNewChoice;
	pageServerSelection->SelectChoice(choice);
	deleteEditNewChoice=WIZARD_CHOICE_EDIT;
	pageDeleteEditNew->SelectChoice(deleteEditNewChoice);
}

GUIWizardPageConfigRemoteDirectory::GUIWizardPageConfigRemoteDirectory(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizardIn,parentPageID,thisPageID)
{
	valueCtrl		= FALSE;
	browseButton	= FALSE;
	CreateControl();
}

GUIWizardPageConfigRemoteDirectory::~GUIWizardPageConfigRemoteDirectory()
{
	valueCtrl		= FALSE;
	browseButton	= FALSE;
}

void GUIWizardPageConfigRemoteDirectory::CreateControl()
{
	valueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	browseButton	= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Browse For Directory");
}

wxSize GUIWizardPageConfigRemoteDirectory::ResizeControl(int x,int y,int width,bool resize)
{
	wxSize		baseControlSize;
	wxSize		controlSize;
	int			baseX;
	int			baseY;
	int			baseWidth;
	int			baseHeight;
	int			valueX;
	int			valueY;
	int			valueWidth;
	int			valueHeight;
	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;
	wxSize		objectSize;

	objectSize		= browseButton->GetSize();
	buttonWidth		= objectSize.GetWidth();
	buttonHeight	= objectSize.GetHeight();

	if (width < buttonWidth)
	{
		width = buttonWidth;
	}

	baseX			= x;
	baseY			= y;
	baseControlSize = GUIWizardPageConfig::ResizeControl(baseX,baseY,width,resize);
	baseWidth		= baseControlSize.GetWidth();
	baseHeight		= baseControlSize.GetHeight();

	valueX			= x;
	valueY			= baseY + baseHeight;
	valueWidth		= width;
	valueHeight		= 30;

	buttonX			= x;
	buttonY			= valueY + valueHeight;

	controlSize.SetWidth(width);
	controlSize.SetHeight(baseHeight + valueHeight + buttonHeight);

	if (resize)
	{
		valueCtrl->SetSize(valueX,valueY,valueWidth,valueHeight);
		browseButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
	}

	return (controlSize);
}

void GUIWizardPageConfigRemoteDirectory::SetCtrlFromValue()
{
	valueCtrl->SetValue(value);
}

bool GUIWizardPageConfigRemoteDirectory::ScreenReady()
{
	return (valueCtrl!=NULL);
}

void GUIWizardPageConfigRemoteDirectory::SetValueFromCtrl()
{
	value = valueCtrl->GetValue();
}

void GUIWizardPageConfigRemoteDirectory::OnButtonPressed(wxCommandEvent &event)
{
	wxString	message;
	bool		result;
	wxString	groupPrefix;

	message="Select Directory";

	groupPrefix = "WEBSITE";

	GenericOKCancelDialog dialog(this,-1,
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
										value,
										false);
	browserPanel->Create(&dialog,
						-1,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	browserPanel->CreateScreen();
	
	dialog.SetPanel(browserPanel);
	dialog.CreateDialog();
	result=(dialog.ShowModal()==WINDOW_ID_BUTTON_SAVE);
	if (result)
	{
		value=browserPanel->GetCurrentSelection();
		SetCtrlFromValue();
	}
	PageContentsChanged();
}

void GUIWizardPageConfigRemoteDirectory::UpdateConfigFromValue()
{
	if (ConfigReady())
	{
		globalStatistics.configData.WriteTextValue(configKey,value);
		if (callBack!=NULL)
		{
			callBack(callBackObject);
		}
	}
}

void GUIWizardPageConfigRemoteDirectory::UpdateValueFromConfig()
{
	if (ConfigReady())
	{
		globalStatistics.configData.ReadTextValue(configKey,&value);
	}
}

GUIWizardPagesWebsite::GUIWizardPagesWebsite()
{
	pageRemoteDirectory			= NULL;
	pageRemoteDirectoryTest		= NULL;
	groupPrefix					= "WEBSITE";

}

GUIWizardPagesWebsite::~GUIWizardPagesWebsite()
{
}

void GUIWizardPagesWebsite::UpdateConfigKeys()
{
	wxString		configKey;

	if ((pageRemoteDirectory != NULL) && (groupPrefix.length()>0))
	{
		configKey.Printf("/%s/FTPRemoteDirectory",groupPrefix.GetData());
		pageRemoteDirectory->SetConfigKey(configKey);
	}
}

void GUIWizardPagesWebsite::SetGroupPrefix()
{
	remoteMachine.SetGroupPrefix(groupPrefix);
	pageRemoteDirectoryTest->SetGroupPrefix(groupPrefix);
	UpdateConfigKeys();
}

void GUIWizardPagesWebsite::CreateWizardPages(StatsgenWizard *wizardIn)
{
	wizard = wizardIn;
	wxString ruleTestPassed;
	wxString ruleWebsiteEnabled;

	const char *sectionTitle="Configure Website";
	pageRemoteDirectory		= new GUIWizardPageConfigRemoteDirectory(wizard,pageID,_T("REMOTEDIRECTORY"));
	pageRemoteDirectoryTest	= new GUIWizardPageRemoteDirectoryTest(wizard,pageID,_T("REMOTEDIRECTORYTEST"));

	pageRemoteDirectory->SetConfigTitle("Website Directory");
	pageRemoteDirectory->SetHelp(
			"The location on your web server where you want stats pages to be "
			"uploaded too."
						);

	pageRemoteDirectoryTest->SetHelp(
			"[b]Press the button[/b] to check the website directory is configured correctly."
						);


	pageRemoteDirectory->SetCallBack(ConfigChangedCallBack,this);
	pageRemoteDirectoryTest->SetCallBack(ConfigChangedCallBack,this);
	

	remoteMachine.SetPageID(pageID,"WEBSITE");
	remoteMachine.CreateWizardPages(wizard);

	ruleTestPassed.Printf("%s=\"TRUE\"",pageRemoteDirectoryTest->GetPageID().GetData());
	ruleWebsiteEnabled.Printf("%s=\"Y\"",remoteMachine.GetEnabledPageID().GetData());
	//wxMessageBox(ruleWebsiteEnabled);
	wizard->AddPageLink(pageRemoteDirectory,		ruleWebsiteEnabled,"");
	wizard->AddPageLink(pageRemoteDirectoryTest,	ruleWebsiteEnabled,ruleTestPassed);

	remoteMachine.SetCallBack(ConfigChangedCallBack,this);
	
	pageRemoteDirectory->SetSectionTitle(sectionTitle);
	pageRemoteDirectoryTest->SetSectionTitle(sectionTitle);
	remoteMachine.SetSectionTitle(sectionTitle);

	SetGroupPrefix();

	ResizePages();
}

void GUIWizardPagesWebsite::ResizePages()
{
	pageRemoteDirectory->Resize();
	pageRemoteDirectoryTest->Resize();
}

void GUIWizardPagesWebsite::SetDirectory()
{
	wxString	directory;

	directory=pageRemoteDirectory->GetValue();
	pageRemoteDirectoryTest->SetDirectory(directory);
}

void GUIWizardPagesWebsite::ConfigChangedCallBack(void *object)
{
	GUIWizardPagesWebsite	*websiteWizard;
	websiteWizard = (GUIWizardPagesWebsite *)object;
	websiteWizard->SetDirectory();
}

GUIWizardPageRemoteDirectoryTest::GUIWizardPageRemoteDirectoryTest(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	statusCtrl		= new wxStaticText(this,-1,"Test Not Run");
	errorCtrl		= new wxRichTextCtrl(this,-1,"");
	errorCtrl->SetEditable(false);
	testButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Remote Directory");
	fileDirectory	= "";
	lastResult = false;
}

GUIWizardPageRemoteDirectoryTest::~GUIWizardPageRemoteDirectoryTest()
{
	lastResult = false;
}

void GUIWizardPageRemoteDirectoryTest::SetGroupPrefix(wxString &groupPrefixIn)
{
	groupPrefix		= groupPrefixIn;
}

bool GUIWizardPageRemoteDirectoryTest::TestResult()
{
	return (lastResult);
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
	RemoteMachine		remoteMachine(groupPrefix);
	RestartingFTP				ftpConnection;
	wxString			msg;
	int					fileCount;
	wxString			filename="*.*";
	wxArrayString		filenameList;
	
	statusCtrl->SetLabel("Connecting To Remote Host");
	errorCtrl->SetValue("");
	lastResult	=remoteMachine.Connect(ftpConnection);

	if (lastResult)
	{
		statusCtrl->SetLabel("Retrieving Directory Listing");
		lastResult = remoteMachine.GetRemoteDirectoryListing(fileDirectory,filename,filenameList);
		fileCount=filenameList.GetCount();

		if (lastResult)
		{
			msg.Printf("Retrieving Directory Listing: OK - %d files located",fileCount);
			statusCtrl->SetLabel(msg);
		}
		else
		{
			statusCtrl->SetLabel("Retrieving Directory Listing: Failed");
			errorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
		}
	}
	else
	{
		statusCtrl->SetLabel("Connecting To Remote Host: Failed");
		errorCtrl->SetValue(remoteMachine.GetLastErrorMessage());
	}

	return (lastResult);
}

void GUIWizardPageRemoteDirectoryTest::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
	PageContentsChanged();
}

wxSize GUIWizardPageRemoteDirectoryTest::ResizeControl(int x,int y,int width,bool resize)
{
	int			statusX;
	int			statusY;
	int			statusWidth;
	int			statusHeight;

	int			errorX;
	int			errorY;
	int			errorWidth;
	int			errorHeight;

	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;
	wxSize		objectSize;

	wxSize		controlSize;

	if (statusCtrl!=NULL)
	{
		objectSize		= testButton->GetSize();
		buttonX			=x;
		buttonY			=y;
		buttonWidth		=objectSize.GetWidth();
		buttonHeight	=objectSize.GetHeight();

		if (buttonWidth > width)
		{
			width = buttonWidth;
		}

		statusX			=x;
		statusY			=buttonY + buttonHeight;
		statusWidth		=width;
		statusHeight	=30;

		errorX			=x;
		errorY			=statusY + statusHeight;
		errorWidth		=width;
		errorHeight		=90;

		if (resize)
		{
			testButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
			statusCtrl->SetSize(statusX,statusY,statusWidth,statusHeight);
			errorCtrl->SetSize(errorX,errorY,errorWidth,errorHeight);
		}
		controlSize.SetWidth(width);
		controlSize.SetHeight(buttonHeight + statusHeight + errorHeight);
	}

	return (controlSize);
}

void GUIWizardPageRemoteDirectoryTest::ResetTestResult()
{
	statusCtrl->SetLabel("Test Not Run");
	errorCtrl->SetValue("");
	lastResult = false;
}

void GUIWizardPageRemoteDirectoryTest::SetDirectory(wxString &fileDirectoryIn)
{
	fileDirectory	= fileDirectoryIn;
}

GUIWizardPagesFullConfig::GUIWizardPagesFullConfig()
{
}

GUIWizardPagesFullConfig::~GUIWizardPagesFullConfig()
{
}

void GUIWizardPagesFullConfig::CreateWizardPages(StatsgenWizard *wizardIn)
{
	wizard = wizardIn;
	pageServers.SetPageID("FULLCONFIG","SERVERS");
	pageServers.CreateWizardPages(wizard);
	pageWebsite.SetPageID("FULLCONFIG","WEBSITE");
	pageWebsite.CreateWizardPages(wizard);
}

StatsgenWizard::StatsgenWizard(wxWindow *parent,
					int id,
					wxString &title,
					const wxBitmap &bitmap,
					const wxPoint &pos,
					long style): wxWizard(parent,WINDOW_ID_WIZARD,title,bitmap,pos,style)
{
	initialised		= false;
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
		if (!initialised)
		{
			initialised=true;
			ResizePages();
		}
	}
	page->UpdateProgress();
}

void StatsgenWizard::ResizePages()
{
	pageLinks.ResizePages();
}

void StatsgenWizard::SetInitialPageLinks()
{
	UpdatePageLinks(true);
	pageLinks.SetPageCounters();
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
	pageLinks.AddPageLink(page,allowRule,continueRule);
}

void StatsgenWizard::UpdatePageLinks(bool fullList)
{
	pageLinks.SetPageLinks(fullList);
}

GUIWizardPageStatsgen *StatsgenWizard::GetFirstPage()
{
	GUIWizardPageStatsgen *page = NULL;

	page = pageLinks.GetFirstPage();

	return (page);
}

GUIWizardPages::GUIWizardPages()
{
	pageID						= "";
}

GUIWizardPages::~GUIWizardPages()
{
}

void GUIWizardPages::SetPageID(wxString parentID,wxString pageIDIn)
{
	pageID=parentID+pageIDIn;
}

GUIWizardPageConfigWildcard::GUIWizardPageConfigWildcard(StatsgenWizard *wizard,wxString parentPageID,wxString thisPageID) : GUIWizardPageConfig(wizard,parentPageID,thisPageID)
{
	valueCtrl		= FALSE;
	directoryLabel	= FALSE;
	wildcardLabel	= FALSE;
	wildcardCtrl	= FALSE;
	browseButton	= FALSE;
	CreateControl();
}

GUIWizardPageConfigWildcard::~GUIWizardPageConfigWildcard()
{
	valueCtrl		= FALSE;
	wildcardCtrl	= FALSE;
	browseButton	= FALSE;
}

void GUIWizardPageConfigWildcard::CreateControl()
{
	valueCtrl		= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	wildcardCtrl	= new wxTextCtrl(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	wildcardLabel	= new wxStaticText(this,-1,"Wildcard");
	directoryLabel	= new wxStaticText(this,-1,"Directory");
	browseButton	= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Browse For Directory");
}

wxString GUIWizardPageConfigWildcard::GetWildcard()
{
	return (wildcardValue);
}

wxSize GUIWizardPageConfigWildcard::ResizeControl(int x,int y,int width,bool resize)
{
	wxSize		baseControlSize;
	wxSize		controlSize;
	int			baseX;
	int			baseY;
	int			baseWidth;
	int			baseHeight;
	int			valueX;
	int			valueY;
	int			valueWidth;
	int			valueHeight;
	int			wildcardX;
	int			wildcardY;
	int			wildcardWidth;
	int			wildcardHeight;
	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;
	int			directoryLabelX;
	int			directoryLabelY;
	int			directoryLabelWidth;
	int			directoryLabelHeight;
	int			wildcardLabelX;
	int			wildcardLabelY;
	int			wildcardLabelWidth;
	int			wildcardLabelHeight;
	wxSize		objectSize;

	objectSize		= browseButton->GetSize();

	buttonWidth		= objectSize.GetWidth();
	buttonHeight	= objectSize.GetHeight();

	if (width < buttonWidth)
	{
		width = buttonWidth;
	}


	baseX			= x;
	baseY			= y;
	baseControlSize = GUIWizardPageConfig::ResizeControl(baseX,baseY,width,resize);
	baseWidth		= baseControlSize.GetWidth();
	baseHeight		= baseControlSize.GetHeight();

	objectSize				=directoryLabel->GetSize();
	directoryLabelWidth		=objectSize.GetWidth();
	directoryLabelX			= x;
	directoryLabelY			= baseY + baseHeight;
	directoryLabelHeight	= 30;

	valueX			= directoryLabelX + directoryLabelWidth + 5;
	valueY			= baseY + baseHeight;
	valueWidth		= width - (5 + directoryLabelWidth);
	valueHeight		= 30;

	objectSize				=wildcardLabel->GetSize();
	wildcardLabelWidth		=objectSize.GetWidth();
	wildcardLabelX			= x;
	wildcardLabelY			= valueY + valueHeight;
	wildcardLabelHeight		= 30;

	wildcardX			= wildcardLabelX + wildcardLabelWidth + 5;
	wildcardY			= valueY + valueHeight;
	wildcardWidth		= width - (5 + wildcardLabelWidth);
	wildcardHeight		= 30;

	buttonX			= x;
	buttonY			= wildcardY + wildcardHeight;

	controlSize.SetWidth(width);
	controlSize.SetHeight(baseHeight + valueHeight + wildcardHeight + buttonHeight);

	if (resize)
	{
		valueCtrl->SetSize(valueX,valueY,valueWidth,valueHeight);
		wildcardCtrl->SetSize(wildcardX,wildcardY,wildcardWidth,wildcardHeight);
		browseButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
		directoryLabel->SetSize(directoryLabelX,directoryLabelY,directoryLabelWidth,directoryLabelHeight);
		wildcardLabel->SetSize(wildcardLabelX,wildcardLabelY,wildcardLabelWidth,wildcardLabelHeight);
	}

	return (controlSize);
}

void GUIWizardPageConfigWildcard::SetCtrlFromValue()
{
	VetoCtrlUpdates();
	wildcardCtrl->SetValue(wildcardValue);
	valueCtrl->SetValue(value);
	AllowCtrlUpdates();
	UpdateConfigFromValue();
}

bool GUIWizardPageConfigWildcard::ScreenReady()
{
	return (valueCtrl!=NULL);
}

void GUIWizardPageConfigWildcard::SetValueFromCtrl()
{
	value = valueCtrl->GetValue();
	wildcardValue = wildcardCtrl->GetValue();
}

void GUIWizardPageConfigWildcard::GetConfigKeys(wxString &configKeyDirectory,
													wxString &configKeyFilename)
{
	configKeyDirectory	= configKey + "Directory";
	configKeyFilename	= configKey + "Wildcard";
}

void GUIWizardPageConfigWildcard::OnButtonPressed(wxCommandEvent &event)
{
	wxString	userDirectory;

	userDirectory = GetDirectoryFromUser();
	if (userDirectory.Length()>0)
	{
		value=userDirectory;
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
		globalStatistics.configData.WriteTextValue(configKeyDirectory,value);
		globalStatistics.configData.WriteTextValue(configKeyFilename,wildcardValue);
		if (callBack!=NULL)
		{
			callBack(callBackObject);
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
		globalStatistics.configData.ReadTextValue(configKeyDirectory,&value);
		globalStatistics.configData.ReadTextValue(configKeyFilename,&wildcardValue);
		if (value.Length()==0)
		{
			globalStatistics.configData.WriteTextValue(configKeyDirectory,defaultValue);
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
	userSelection=wxDirSelector(message,value);

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

	groupPrefix = configKey.AfterFirst('/');
	groupPrefix = groupPrefix.BeforeFirst('/');

	GenericOKCancelDialog dialog(this,-1,
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
										value,
										false);
	browserPanel->Create(&dialog,
						-1,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	browserPanel->CreateScreen();
	
	dialog.SetPanel(browserPanel);
	dialog.CreateDialog();
	result=(dialog.ShowModal()==WINDOW_ID_BUTTON_SAVE);
	userDirectory="";
	if (result)
	{
		userDirectory=browserPanel->GetCurrentSelection();
	}

	return (userDirectory);
}

void GUIPageLinks::ResizePages()
{
	int						pageCount;
	int						pageIndex;
	GUIWizardPageStatsgen	*page;
	int						maxHeight=0;
	wxSize					pageSize;
	int						pageHeight=0;

	pageCount=guiPages.GetCount();
	for (pageIndex=0;pageIndex<pageCount;pageIndex++)
	{
		page=(GUIWizardPageStatsgen *)guiPages.Item(pageIndex);
		pageSize=page->GetSize();
		pageHeight=pageSize.GetHeight();
		if (pageHeight>maxHeight)
		{
			maxHeight=pageHeight;
		}
	}
	for (pageIndex=0;pageIndex<pageCount;pageIndex++)
	{
		page=(GUIWizardPageStatsgen *)guiPages.Item(pageIndex);
		page->Resize(maxHeight);
	}
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
msg.Printf("rulechar found[%c], ID=[%s] value=[%s]",ruleChar,id.GetData(),value1.GetData());if (debugOn) wxMessageBox(msg);
				if (id.Length()>0)
				{
					PushValue(valueQueue,value1);
				}
				if (ValueCount(valueQueue) == 2)
				{
					value2=PopValue(valueQueue);
					value1=PopValue(valueQueue);
msg.Printf("2 on stack:value1=[%s],value2=[%s] operatorChar=[%c]",value1.GetData(),value2.GetData(),operatorChar);if (debugOn) wxMessageBox(msg);
					switch (operatorChar)
					{
						case '&':
msg.Printf("& val1=[%s] val2=[%s]",value1.GetData(),value2.GetData()); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value((Value2Bool(value1) && Value2Bool(value2)));
							PushValue(valueQueue,value1);
msg.Printf("& result value1=[%s]",value1.GetData());if (debugOn) wxMessageBox(msg);
							break;
						case '!':
msg.Printf("! val1=[%s] val2=[%s]",value1.GetData(),value2.GetData()); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value(value1.CmpNoCase(value2)!=0);
							PushValue(valueQueue,value1);
msg.Printf("! result value1=[%s]",value1.GetData());if (debugOn) wxMessageBox(msg);
							break;
						case '=':
msg.Printf("= val1=[%s] val2=[%s]",value1.GetData(),value2.GetData()); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value(value1.CmpNoCase(value2)==0);
							PushValue(valueQueue,value1);
msg.Printf("= result value1=[%s]",value1.GetData());if (debugOn) wxMessageBox(msg);
							break;
						case '|':
msg.Printf("| val1=[%s] val2=[%s]",value1.GetData(),value2.GetData()); if (debugOn) wxMessageBox(msg);
							value1=Bool2Value((Value2Bool(value1) || Value2Bool(value2)));
							PushValue(valueQueue,value1);
msg.Printf("| result value1=[%s]",value1.GetData());if (debugOn) wxMessageBox(msg);
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
msg.Printf("( result value1=[%s]",value1.GetData());if (debugOn) wxMessageBox(msg);
						break;
					case ')':
					case 0:
						value1=PopValue(valueQueue);
msg.Printf(")/Null value1=[%s]",value1.GetData());if (debugOn) wxMessageBox(msg);
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
msg.Printf("final value1=[%s]",value1.GetData());if (debugOn) wxMessageBox(msg);
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

	wizard = wizardIn;
	pageServers.SetPageID("FULLCONFIG","SERVERS");
	pageServers.CreateWizardPages(wizard);

	pageWebsite.SetPageID("FULLCONFIG","WEBSITE");
	pageWebsite.CreateWizardPages(wizard);

	pageMessaging.SetPageID("FULLCONFIG","MESSAGING");
	pageMessaging.CreateWizardPages(wizard);

	pageTestRun = new GUIWizardPageRun(wizard,pageID,_T("TESTRUN"));
	pageTestRun->SetSectionTitle(titleChars);
	pageTestRun->SetHelp("[b]Press the button[/b] to perform an initial test run of the stats.\n"
						"Once the run has completed a small amount of the stats pages will be "
						"uploaded to your website in the directory you have configured.\n"
						"\nYou will be missing images and style settings so the pages may look "
						"a little rubbish at the moment - the image pack and style uploads "
						"are in the next step");

	rule.Printf("%s=\"TRUE\"",pageTestRun->GetPageID().GetData());
	wizard->AddPageLink(pageTestRun,		"",	rule);
	pageImagePacks.SetPageID("FULLCONFIG","IMAGEPACKS");
	pageImagePacks.CreateWizardPages(wizard);
}

GUIWizardPageRun::GUIWizardPageRun(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	statusCtrl		= new wxStaticText(this,-1,"Test Not Run");
	errorCtrl		= new wxRichTextCtrl(this,-1,"");
	errorCtrl->SetEditable(false);
	testButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Test Stats Run");
	SetHelp("[b]Press the button[/b] to perform a stats run");
	progressPanel=new ProgressPanel;
	progressPanel->Create(this,-1,wxDefaultPosition,wxDefaultSize,
			wxRAISED_BORDER);
	progressPanel->EnableTimeToGo();
	progressPanel->counterEnabled=true;

	progress->SetClone(NULL);
	runSuccessful = false;
}

GUIWizardPageRun::~GUIWizardPageRun()
{
	progress->SetClone(NULL);
}

bool GUIWizardPageRun::TestResult()
{
	return (runSuccessful);
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
		configKey.Printf("/%s/MaxLogfileSize",serverConfigPrefix.GetData());
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
	statusCtrl->SetLabel(selection);
}

bool GUIWizardPageRun::PerformTest()
{

	SetTestParameters(true);
	SetSelection("Test Running");

	progress->EventUpdating(false);
	progressPanel->EventUpdating(false);
	progressPanel->ClearErrors();

	{
		wxBusyCursor	cursor;
		wizard->Enable(false);

		progress->SetClone(progressPanel);
		globalStatistics.statsgenDatabase.OpenDB();
		globalStatistics.statsgenDatabase.Zap();
		globalStatistics.ReadServersFromConfig();
		globalStatistics.FlagStartOfRun();

		progressPanel->ClearErrors();

		globalStatistics.ProduceStatsInitiate();
		globalStatistics.ProduceStatsDownload();
		globalStatistics.ProduceStatsProcess();
		globalStatistics.ProduceStatsTransfer();
		globalStatistics.ProduceStatsFinalise();
		wizard->Enable(true);
		globalStatistics.statsgenDatabase.OpenDB();
		globalStatistics.statsgenDatabase.Zap();
	}
	SetSelection("Test Completed");
	progress->EventUpdating(true);

	runSuccessful = 
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

	return (runSuccessful);
}

void GUIWizardPageRun::OnButtonPressed(wxCommandEvent &event)
{
	PerformTest();
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
	PageContentsChanged();
}

wxSize GUIWizardPageRun::ResizeControl(int x,int y,int width,bool resize)
{
	int			statusX;
	int			statusY;
	int			statusWidth;
	int			statusHeight;

	int			errorX;
	int			errorY;
	int			errorWidth;
	int			errorHeight;

	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;

	int			progressX;
	int			progressY;
	int			progressWidth;
	int			progressHeight;
	wxSize		objectSize;

	wxSize		controlSize;

	if (statusCtrl!=NULL)
	{
		objectSize		= testButton->GetSize();
		buttonX			=x;
		buttonY			=y;
		buttonWidth		=objectSize.GetWidth();
		buttonHeight	=objectSize.GetHeight();

		if (buttonWidth > width)
		{
			width = buttonWidth;
		}

		progressX		=x;
		progressY		=buttonY + buttonHeight;
		progressWidth	=width;
		progressHeight	=30;

		statusX			=x;
		statusY			=progressY + progressHeight;
		statusWidth		=width;
		statusHeight	=30;

		errorX			=x;
		errorY			=statusY + statusHeight;
		errorWidth		=width;
		errorHeight		=90;

		if (resize)
		{
			testButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
			statusCtrl->SetSize(statusX,statusY,statusWidth,statusHeight);
			errorCtrl->SetSize(errorX,errorY,errorWidth,errorHeight);
			progressPanel->SetSize(progressX,progressY,progressWidth,progressHeight);
		}
		controlSize.SetWidth(width);
		controlSize.SetHeight(buttonHeight + progressHeight + statusHeight + errorHeight);
	}

	return (controlSize);
}

void GUIWizardPageRun::ResetTestResult()
{
	statusCtrl->SetLabel("Test Not Run");
	errorCtrl->SetValue("");
}

GUIWizardPageImagePacks::GUIWizardPageImagePacks(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	imagesCtrl		= new wxScrolledWindow(this,
							-1,
							wxDefaultPosition,
							wxDefaultSize,
							wxVSCROLL);
	testButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Upload Image Packs");
	SetHelp("[b]Press the button[/b] to perform a stats run");
	progressPanel=new ProgressPanel;
	progressPanel->Create(this,-1,wxDefaultPosition,wxDefaultSize,
			wxRAISED_BORDER);
	progress->SetClone(NULL);
	statsgenSite.GetImagePackList(fileDescriptions,filePaths,serverTypes,upgradeFiles,thumbnails);
	CreateImagePackPanels();
	SelectImagePackPanels();
}

void GUIWizardPageImagePacks::CreateImagePackPanels()
{
	int				imagePackCount;
	int				imagePackIndex;
	ImagePackPanel	*imagePackPanel;
	wxString		fileDescription;
	wxString		filePath;
	wxString		serverType;
	wxString		thumbnail;
	wxString		upgradeFile;
	wxString		msg;

	imagePackCount=fileDescriptions.GetCount();
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		fileDescription=fileDescriptions.Item(imagePackIndex);
		filePath=filePaths.Item(imagePackIndex);
		serverType=serverTypes.Item(imagePackIndex);
		thumbnail=thumbnails.Item(imagePackIndex);
		upgradeFile=upgradeFiles.Item(imagePackIndex);
		imagePackPanel=new ImagePackPanel(imagesCtrl,
										fileDescription,
										filePath,
										serverType,
										upgradeFile,
										thumbnail);
		imagePackPanels.Add(imagePackPanel);
	}
}

void GUIWizardPageImagePacks::SelectImagePackPanel(wxString &serverType)
{
	int				imagePackCount;
	int				imagePackIndex;
	ImagePackPanel	*imagePackPanel;

	imagePackCount=imagePackPanels.GetCount();
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		imagePackPanel=(ImagePackPanel *)imagePackPanels.Item(imagePackIndex);
		if (serverType.CmpNoCase(imagePackPanel->GetServerType())==0)
		{
			imagePackPanel->SelectForUpload();
		}
	}
}

void GUIWizardPageImagePacks::SelectImagePackPanels()
{
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
		configKey.Printf("/%s/serverType",groupPrefix.GetData());
		globalStatistics.configData.ReadTextValue(configKey,&serverType);
		SelectImagePackPanel(serverType);
	}
	serverType="MANDATORY";
	SelectImagePackPanel(serverType);
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
	progress->SetClone(progressPanel);
	imagePackCount=imagePackPanels.GetCount();
	wizard->Enable(false);
	storeLocally=(
		wxMessageBox("Store Image Packs Locally?","Confirm",wxYES_NO)==wxYES);
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		imagePackPanel=(ImagePackPanel *)imagePackPanels.Item(imagePackIndex);
		if (imagePackPanel->Selected())
		{
			progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WEBSITE);
			filePath=imagePackPanel->GetFilePath();
			fileDescription=imagePackPanel->GetDescription();
			upgradeFile=imagePackPanel->GetUpgradeFile();
			statsgenSite.UploadImagePack(fileDescription,filePath,storeLocally,localOutputFolder,upgradeFile);
			progress->Finalise();
		}
	}
	wizard->Enable(true);

	progress->SetClone(NULL);
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
	PageContentsChanged();
}

void GUIWizardPageImagePacks::ResizeImagePackPanels()
{
	int				imagePackCount;
	int				imagePackIndex;
	ImagePackPanel	*imagePackPanel;
	int				x;
	int				y;
	wxSize			imagePackSize;
	wxString		msg;
	int				scrollRate=10;
	int				width=100;

	imagePackCount=imagePackPanels.GetCount();
	x=0;
	y=0;
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		imagePackPanel=(ImagePackPanel *)imagePackPanels.Item(imagePackIndex);
		imagePackSize=imagePackPanel->ResizeControl(
						x,y,
						imagePackSize.GetWidth(),
						imagePackSize.GetHeight(),
						false);
		imagePackPanel->ResizeControl(x,y,
						imagePackSize.GetWidth(),
						imagePackSize.GetHeight(),
						true);
		y+=imagePackSize.GetHeight();
		y+=5;
		width=imagePackSize.GetWidth();
		scrollRate=imagePackSize.GetHeight();
	}
	imagesCtrl->SetScrollbars(1,1,width,y);
	imagesCtrl->SetScrollRate(10,10);
	imagesCtrl->SetVirtualSize(width,y);
}

wxSize GUIWizardPageImagePacks::ResizeControl(int x,int y,int width,bool resize)
{
	int			imagesX;
	int			imagesY;
	int			imagesWidth;
	int			imagesHeight;

	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;

	int			progressX;
	int			progressY;
	int			progressWidth;
	int			progressHeight;
	wxSize		objectSize;

	wxSize		controlSize;

	if (imagesCtrl!=NULL)
	{
		width			=650;

		imagesX			=x;
		imagesY			=y;
		imagesWidth		=width;
		imagesHeight	=300;

		objectSize		= testButton->GetSize();
		buttonX			=x;
		buttonY			=imagesY+imagesHeight;
		buttonWidth		=objectSize.GetWidth();
		buttonHeight	=objectSize.GetHeight();

		if (buttonWidth > width)
		{
			width = buttonWidth;
		}

		progressX		=x;
		progressY		=buttonY + buttonHeight;
		progressWidth	=width;
		progressHeight	=30;

		if (resize)
		{
			testButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
			ResizeImagePackPanels();
			imagesCtrl->SetSize(imagesX,imagesY,imagesWidth,imagesHeight);
			progressPanel->SetSize(progressX,progressY,progressWidth,progressHeight);
		}
		controlSize.SetWidth(width);
		controlSize.SetHeight(buttonHeight + progressHeight + imagesHeight);
	}

	return (controlSize);
}

GUIWizardPagesImagePacks::GUIWizardPagesImagePacks()
{
	pageImagePacks			= NULL;
}

GUIWizardPagesImagePacks::~GUIWizardPagesImagePacks()
{
}

void GUIWizardPagesImagePacks::CreateWizardPages(StatsgenWizard *wizardIn)
{
	wizard = wizardIn;

	const char *sectionTitle="Image Packs";
	pageImagePacks		= new GUIWizardPageImagePacks(wizard,pageID,_T("IMAGEPACKS"));

	pageImagePacks->SetHelp(
			"[b]Place a check[/b] in the appropriate image packs and then [b]press "
			"the Upload button[/b] to download them from the statsgen website and "
			"then upload them to your website"
						);


	wizard->AddPageLink(pageImagePacks,		"","");
	
	pageImagePacks->SetSectionTitle(sectionTitle);

	ResizePages();
}

void GUIWizardPagesImagePacks::ResizePages()
{
	pageImagePacks->Resize();
}

ImagePackPanel::ImagePackPanel(wxWindow *parent,
									wxString &fileDescriptionIn,
									wxString &filePathIn,
									wxString &serverTypeIn,
									wxString &upgradeFileIn,
									wxString &thumbnailIn) : wxPanel(parent,-1)
{
	wxString		label="";
	wxString		localimage="thumbnail.";

	fileDescription	=fileDescriptionIn;
	filePath		=filePathIn;
	serverType		=serverTypeIn;
	thumbnail		=thumbnailIn;
	upgradeFile		=upgradeFileIn;

	if (thumbnail.EndsWith("jpg"))
	{
		localimage+="jpg";
	}
	if (thumbnail.EndsWith("gif"))
	{
		localimage+="gif";
	}
	checkCtrl		=new wxCheckBox(this,-1,label);
	descriptionCtrl	=new wxStaticText(this,-1,fileDescription);
	if (thumbnail.Length()>0)
	{
		StatsgenWeb		statsgenSite;
		WebFile			*webFile;

		webFile=statsgenSite.GetWebFile(thumbnail);
		webFile->Get(localimage);
		thumbnailImage.LoadFile(localimage);

		delete(webFile);
	}
	thumbnailPanel	=new ImagePanel(this,-1,
								wxDefaultPosition,
								wxDefaultSize,
								0,
								_T(""));
	thumbnailPanel->SetImage(thumbnailImage);
}

ImagePackPanel::~ImagePackPanel()
{
}

wxString ImagePackPanel::GetServerType()
{
	return (serverType);
}
wxString ImagePackPanel::GetDescription()
{
	return (fileDescription);
}
wxString ImagePackPanel::GetFilePath()
{
	return (filePath);
}
wxString ImagePackPanel::GetUpgradeFile()
{
	return (upgradeFile);
}
bool ImagePackPanel::Selected()
{
	return (checkCtrl->GetValue());
}
void ImagePackPanel::SelectForUpload()
{
	checkCtrl->SetValue(true);
}

wxSize ImagePackPanel::ResizeControl(int x,int y,int width,int height,bool resize)
{
	wxSize		panelSize;
	wxSize		objectSize;

	int		panelX;
	int		panelY;
	int		panelWidth;
	int		panelHeight;

	int		checkX;
	int		checkY;
	int		checkWidth;
	int		checkHeight;

	int		thumbX;
	int		thumbY;
	int		thumbWidth;
	int		thumbHeight;

	int		descriptionX;
	int		descriptionY;
	int		descriptionWidth;
	int		descriptionHeight;
	int		minImageWidth=325;

	objectSize			=checkCtrl->GetSize();
	checkX				=0;
	checkY				=0;
	checkWidth			=objectSize.GetWidth();
	checkHeight			=objectSize.GetHeight();

	thumbX				=checkX+5+checkWidth;
	thumbY				=0;
	thumbWidth			=thumbnailImage.GetWidth();
	thumbHeight			=thumbnailImage.GetHeight();

	if (thumbWidth<minImageWidth)
	{
		thumbWidth=minImageWidth;
	}


	objectSize			=descriptionCtrl->GetSize();
	descriptionX		=thumbX+5+thumbWidth;
	descriptionY		=0;
	descriptionWidth	=objectSize.GetWidth();
	descriptionHeight	=objectSize.GetHeight();

	panelX				=x;
	panelY				=y;
	panelWidth			=descriptionX+descriptionWidth;
	if (checkHeight>descriptionHeight)
	{
		panelHeight		=checkHeight;
	}
	else
	{
		panelHeight		=descriptionHeight;
	}
	if (thumbHeight>panelHeight)
	{
		panelHeight=thumbHeight;
	}
	panelSize.SetWidth(panelWidth);
	panelSize.SetHeight(panelHeight);

	if (resize)
	{
		checkCtrl->SetSize(checkX,checkY,checkWidth,checkHeight);
		descriptionCtrl->SetSize(descriptionX,descriptionY,descriptionWidth,descriptionHeight);
		thumbnailPanel->SetSize(thumbX,thumbY,thumbWidth,thumbHeight);
		SetSize(panelX,panelY,panelWidth,panelHeight);
	}

	return (panelSize);
}

GUIWizardPagesMessaging::GUIWizardPagesMessaging()
{
	pageMessagingEnabled					= NULL;
	pageMessagingFrequency					= NULL;
	pageMessagingPort						= NULL;
}

GUIWizardPagesMessaging::~GUIWizardPagesMessaging()
{
}

void GUIWizardPagesMessaging::UpdateConfigKeys()
{
	wxString		configKey;
	wxString		configValue;

	if (pageMessagingEnabled != NULL)
	{
		configKey="/RCONSettings/Enabled";
		pageMessagingEnabled->SetConfigKey(configKey,"Y");

		configKey="/RCONSettings/MessageIntervalTimer";
		pageMessagingFrequency->SetConfigKey(configKey,"60");

		configKey="/RCONSettings/ClientPort";
		pageMessagingPort->SetConfigKey(configKey,"8000");

		configKey="/RCONSettings/CustomMessageInterval";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"0");

		configKey="/RCONSettings/MessagePrefix";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"^7");
	}
}

void GUIWizardPagesMessaging::CreateWizardPages(StatsgenWizard *wizardIn)
{
	const char *sectionTitle="Configure Messaging";
	wxString ruleMessagingEnabled;

	wizard=wizardIn;

	pageMessagingEnabled			= new GUIWizardPageConfigBoolean(wizard,pageID,_T("ENABLED"));
	pageMessagingFrequency			= new GUIWizardPageConfigText(wizard,pageID,_T("FREQUENCY"));
	pageMessagingPort				= new GUIWizardPageConfigText(wizard,pageID,_T("PORT"));

	pageMessagingEnabled->SetConfigTitle("Enable Messaging");
	pageMessagingEnabled->SetHelp(
		"Global Messaging Enabled / Disabled\n"
		"This switches on / off messaging for the entire system, irrespective "
		"of any individual server settings"
		);
	pageMessagingEnabled->SetTrueFalseLabels("Yes", "No");

	pageMessagingFrequency->SetConfigTitle("Message Frequency");
	pageMessagingFrequency->SetHelp(
		"How often messages should be sent to the server, in seconds"
		);

	pageMessagingPort->SetConfigTitle("Messaging Client Port");
	pageMessagingPort->SetHelp(
		"This is the local port used during communications with the game "
		"servers.\n"
		"You should have a [b]different value[/b] for this for every instance of "
		"statsgen you are running.\n"
		"[b]This is NOT the RCON port number[/b].\n"
		"\nGood values are 8000, 8001, 8002"
		);

	pageMessagingEnabled->SetCallBack(ConfigChangedCallBack,this);
	pageMessagingFrequency->SetCallBack(ConfigChangedCallBack,this);
	pageMessagingPort->SetCallBack(ConfigChangedCallBack,this);

	ruleMessagingEnabled.Printf("(%s=\"%s\")", pageMessagingEnabled->GetPageID().GetData(),"Y");

	wizard->AddPageLink(pageMessagingEnabled,		""		,"");
	wizard->AddPageLink(pageMessagingFrequency,		ruleMessagingEnabled	,"");
	wizard->AddPageLink(pageMessagingPort,			ruleMessagingEnabled	,"");
	SetSectionTitle(sectionTitle);

	UpdateConfigKeys();

	ResizePages();
}

void GUIWizardPagesMessaging::SetSectionTitle(const char *sectionTitle)
{
	pageMessagingEnabled->SetSectionTitle(sectionTitle);
	pageMessagingFrequency->SetSectionTitle(sectionTitle);
	pageMessagingPort->SetSectionTitle(sectionTitle);
}

void GUIWizardPagesMessaging::ResizePages()
{
	pageMessagingEnabled->Resize();
	pageMessagingFrequency->Resize();
	pageMessagingPort->Resize();
}

void GUIWizardPagesMessaging::ConfigChangedCallBack(void *object)
{
	GUIWizardPagesMessaging	*wizard;
	wizard = (GUIWizardPagesMessaging *)object;
}

GUIWizardPageTemplatePacks::GUIWizardPageTemplatePacks(StatsgenWizard *wizardIn,wxString parentPageID,wxString thisPageID) : GUIWizardPageStatsgen(wizardIn,parentPageID,thisPageID)
{
	imagesCtrl		= new wxScrolledWindow(this,
							-1,
							wxDefaultPosition,
							wxDefaultSize,
							wxVSCROLL);
	testButton		= new wxButton(this,WINDOW_ID_BUTTON_NEW,"Upload Template Packs");
	SetHelp("[b]Press the button[/b] to perform a stats run");
	progressPanel=new ProgressPanel;
	progressPanel->Create(this,-1,wxDefaultPosition,wxDefaultSize,
			wxRAISED_BORDER);
	progress->SetClone(NULL);
	statsgenSite.GetTemplateList(fileDescriptions,filePaths,serverTypes,secondFilePaths,templateFilenames,upgradePaths,thumbnails);
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

	imagePackCount=fileDescriptions.GetCount();
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		fileDescription=fileDescriptions.Item(imagePackIndex);
		filePath=filePaths.Item(imagePackIndex);
		serverType=serverTypes.Item(imagePackIndex);
		thumbnail=thumbnails.Item(imagePackIndex);
		templateFilename=templateFilenames.Item(imagePackIndex);
		secondFilePath=secondFilePaths.Item(imagePackIndex);
		upgradePath=upgradePaths.Item(imagePackIndex);
		imagePackPanel=new TemplatePackPanel(imagesCtrl,
										fileDescription,
										filePath,
										serverType,
										secondFilePath,
										upgradePath,
										templateFilename,
										thumbnail);
		templatePackPanels.Add(imagePackPanel);
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

	progress->SetClone(progressPanel);
	imagePackCount=templatePackPanels.GetCount();
	wizard->Enable(false);
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		templatePackPanel=(TemplatePackPanel *)templatePackPanels.Item(imagePackIndex);
		if (templatePackPanel->Selected())
		{
			progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WEBSITE);
			filePath=templatePackPanel->GetFilePath();
			secondFilePath=templatePackPanel->GetSecondFilePath();
			upgradePath=templatePackPanel->GetUpgradePath();
			templateFilename=templatePackPanel->GetTemplateFilename();
			fileDescription=templatePackPanel->GetDescription();
			statsgenSite.UploadTemplatePack(fileDescription,filePath,secondFilePath,upgradePath,templateFilename);
			progress->Finalise();
		}
	}
	wizard->Enable(true);

	progress->SetClone(NULL);
	if (callBack!=NULL)
	{
		callBack(callBackObject);
	}
	PageContentsChanged();
}

void GUIWizardPageTemplatePacks::ResizeTemplatePackPanels()
{
	int				imagePackCount;
	int				imagePackIndex;
	TemplatePackPanel	*templatePackPanel;
	int				x;
	int				y;
	wxSize			imagePackSize;
	wxString		msg;
	int				scrollRate=10;
	int				width=100;

	imagePackCount=templatePackPanels.GetCount();
	x=0;
	y=0;
	for (imagePackIndex=0;imagePackIndex<imagePackCount;imagePackIndex++)
	{
		templatePackPanel=(TemplatePackPanel *)templatePackPanels.Item(imagePackIndex);
		imagePackSize=templatePackPanel->ResizeControl(
						x,y,
						imagePackSize.GetWidth(),
						imagePackSize.GetHeight(),
						false);
		templatePackPanel->ResizeControl(x,y,
						imagePackSize.GetWidth(),
						imagePackSize.GetHeight(),
						true);
		y+=imagePackSize.GetHeight();
		y+=5;
		width=imagePackSize.GetWidth();
		scrollRate=imagePackSize.GetHeight();
	}
	imagesCtrl->SetScrollbars(1,1,width,y);
	imagesCtrl->SetScrollRate(10,10);
	imagesCtrl->SetVirtualSize(width,y);
}

wxSize GUIWizardPageTemplatePacks::ResizeControl(int x,int y,int width,bool resize)
{
	int			imagesX;
	int			imagesY;
	int			imagesWidth;
	int			imagesHeight;

	int			buttonX;
	int			buttonY;
	int			buttonWidth;
	int			buttonHeight;

	int			progressX;
	int			progressY;
	int			progressWidth;
	int			progressHeight;
	wxSize		objectSize;

	wxSize		controlSize;

	if (imagesCtrl!=NULL)
	{
		width			=650;

		imagesX			=x;
		imagesY			=y;
		imagesWidth		=width;
		imagesHeight	=300;

		objectSize		= testButton->GetSize();
		buttonX			=x;
		buttonY			=imagesY+imagesHeight;
		buttonWidth		=objectSize.GetWidth();
		buttonHeight	=objectSize.GetHeight();

		if (buttonWidth > width)
		{
			width = buttonWidth;
		}

		progressX		=x;
		progressY		=buttonY + buttonHeight;
		progressWidth	=width;
		progressHeight	=30;

		if (resize)
		{
			testButton->SetSize(buttonX,buttonY,buttonWidth,buttonHeight);
			ResizeTemplatePackPanels();
			imagesCtrl->SetSize(imagesX,imagesY,imagesWidth,imagesHeight);
			progressPanel->SetSize(progressX,progressY,progressWidth,progressHeight);
		}
		controlSize.SetWidth(width);
		controlSize.SetHeight(buttonHeight + progressHeight + imagesHeight);
	}

	return (controlSize);
}

GUIWizardPagesTemplatePacks::GUIWizardPagesTemplatePacks()
{
	pageTemplatePacks			= NULL;
}

GUIWizardPagesTemplatePacks::~GUIWizardPagesTemplatePacks()
{
}

void GUIWizardPagesTemplatePacks::CreateWizardPages(StatsgenWizard *wizardIn)
{
	wizard = wizardIn;

	const char *sectionTitle="Template Packs";
	pageTemplatePacks		= new GUIWizardPageTemplatePacks(wizard,pageID,_T("TEMPLATEPACKS"));

	pageTemplatePacks->SetHelp(
			"[b]Place a check[/b] in the appropriate template and then [b]press "
			"the Upload button[/b] to download them from the statsgen website and "
			"then upload them to your website"
						);


	wizard->AddPageLink(pageTemplatePacks,		"","");
	
	pageTemplatePacks->SetSectionTitle(sectionTitle);

	ResizePages();
}

void GUIWizardPagesTemplatePacks::ResizePages()
{
	pageTemplatePacks->Resize();
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

	fileDescription	=fileDescriptionIn;
	filePath		=filePathIn;
	serverType		=serverTypeIn;
	secondFilePath	=secondFilePathIn;
	templateFilename=templateFilenameIn;
	upgradePath		=upgradePathIn;
	thumbnail		=thumbnailIn;

	if (thumbnail.EndsWith("jpg"))
	{
		localimage+="jpg";
	}
	if (thumbnail.EndsWith("gif"))
	{
		localimage+="gif";
	}
	checkCtrl		=new wxCheckBox(this,-1,label);
	descriptionCtrl	=new wxStaticText(this,-1,fileDescription);
	if (thumbnail.Length()>0)
	{
		StatsgenWeb		statsgenSite;
		WebFile			*webFile;

		webFile=statsgenSite.GetWebFile(thumbnail);
		webFile->Get(localimage);
		thumbnailImage.LoadFile(localimage);

		delete(webFile);
	}
	thumbnailPanel	=new ImagePanel(this,-1,
								wxDefaultPosition,
								wxDefaultSize,
								0,
								_T(""));
	thumbnailPanel->SetImage(thumbnailImage);
}

TemplatePackPanel::~TemplatePackPanel()
{
}

wxString TemplatePackPanel::GetServerType()
{
	return (serverType);
}
wxString TemplatePackPanel::GetDescription()
{
	return (fileDescription);
}
wxString TemplatePackPanel::GetFilePath()
{
	return (filePath);
}
wxString TemplatePackPanel::GetSecondFilePath()
{
	return (secondFilePath);
}
wxString TemplatePackPanel::GetUpgradePath()
{
	return (upgradePath);
}
wxString TemplatePackPanel::GetTemplateFilename()
{
	return (templateFilename);
}
bool TemplatePackPanel::Selected()
{
	return (checkCtrl->GetValue());
}
void TemplatePackPanel::SelectForUpload()
{
	checkCtrl->SetValue(true);
}

wxSize TemplatePackPanel::ResizeControl(int x,int y,int width,int height,bool resize)
{
	wxSize		panelSize;
	wxSize		objectSize;

	int		panelX;
	int		panelY;
	int		panelWidth;
	int		panelHeight;

	int		checkX;
	int		checkY;
	int		checkWidth;
	int		checkHeight;

	int		thumbX;
	int		thumbY;
	int		thumbWidth;
	int		thumbHeight;

	int		descriptionX;
	int		descriptionY;
	int		descriptionWidth;
	int		descriptionHeight;
	int		minImageWidth=325;

	objectSize			=checkCtrl->GetSize();
	checkX				=0;
	checkY				=0;
	checkWidth			=objectSize.GetWidth();
	checkHeight			=objectSize.GetHeight();

	thumbX				=checkX+5+checkWidth;
	thumbY				=0;
	thumbWidth			=thumbnailImage.GetWidth();
	thumbHeight			=thumbnailImage.GetHeight();

	if (thumbWidth<minImageWidth)
	{
		thumbWidth=minImageWidth;
	}


	objectSize			=descriptionCtrl->GetSize();
	descriptionX		=thumbX+5+thumbWidth;
	descriptionY		=0;
	descriptionWidth	=objectSize.GetWidth();
	descriptionHeight	=objectSize.GetHeight();

	panelX				=x;
	panelY				=y;
	panelWidth			=descriptionX+descriptionWidth;
	if (checkHeight>descriptionHeight)
	{
		panelHeight		=checkHeight;
	}
	else
	{
		panelHeight		=descriptionHeight;
	}
	if (thumbHeight>panelHeight)
	{
		panelHeight=thumbHeight;
	}
	panelSize.SetWidth(panelWidth);
	panelSize.SetHeight(panelHeight);

	if (resize)
	{
		checkCtrl->SetSize(checkX,checkY,checkWidth,checkHeight);
		descriptionCtrl->SetSize(descriptionX,descriptionY,descriptionWidth,descriptionHeight);
		thumbnailPanel->SetSize(thumbX,thumbY,thumbWidth,thumbHeight);
		SetSize(panelX,panelY,panelWidth,panelHeight);
	}

	return (panelSize);
}

