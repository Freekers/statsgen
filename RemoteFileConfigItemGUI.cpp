// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "RemoteFileConfigItemGUI.h"
#include "RemoteDirectoryConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "GenericOKCancelDialog.h"
#include "FTPBrowserPanel.h"

BEGIN_EVENT_TABLE(RemoteFileConfigItemGUI, wxPanel)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,RemoteFileConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,RemoteFileConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

RemoteFileConfigItemGUI::RemoteFileConfigItemGUI()
{
	mDirectoryConfig	= NULL;
	mFTPID				= "";
}

RemoteFileConfigItemGUI::~RemoteFileConfigItemGUI()
{
}

void RemoteFileConfigItemGUI::SetConfigKey(wxString &configKey,wxString &defaultValue,wxString &id)
{
	BaseConfigItemGUI::SetConfigKey(configKey,defaultValue);
	mFTPID = id;
	if (mDirectoryConfig != NULL)
	{
		mDirectoryConfig->SetConfigKey(mConfigKey,mDefaultValue,id);
	}
}
void RemoteFileConfigItemGUI::CreateDisplay(
			wxWindow *parent,
			int id,
			wxString &labelTextIn,
			RemoteDirectoryConfigItemGUI *directoryConfigIn)
{
	wxSize		buttonSize(16,16);

	mLabelText			= labelTextIn;
	mDirectoryConfig	= directoryConfigIn;
	 

	Create(parent,id);

	mLabel = new wxStaticText();
	mLabel->Create(this, wxID_ANY, mLabelText);


	mTextEdit = new wxTextCtrl();
	mTextEdit->Create(this,
				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				0,
				wxDefaultValidator);

	mButton = new wxBitmapButton();
	mButton->Create(this,
				WINDOW_ID_CONFIG_BUTTON,
				wxArtProvider::GetIcon(wxART_FILE_OPEN,
										wxART_OTHER,
										buttonSize));
	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mLabel,2,wxEXPAND);
	mMainSizer->Add(mButton,0);
	mMainSizer->Add(mTextEdit,7,wxEXPAND);

	ConfigureSizer();
}

void RemoteFileConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;

	if (mConfigKey.Length() == 0)
	{
		return;
	}
	value=GetValue();
	globalStatistics.configData.WriteTextValue(mConfigKey,
										value);
	if (GetParent()!=NULL)
	{
		wxCommandEvent	newEvent;
		newEvent.SetId(WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		newEvent.SetEventType(wxEVT_COMMAND_TEXT_UPDATED);
		//GetParent()->AddPendingEvent(newEvent);
		GetParent()->GetEventHandler()->AddPendingEvent(newEvent);
	}
}

void RemoteFileConfigItemGUI::ApplyConfigKeyChange()
{
	wxString	value;

	if (mConfigKey.Length() == 0)
	{
		return;
	}

	globalStatistics.configData.ReadTextValue(mConfigKey,&value,mDefaultValue);
	mTextEdit->SetValue(value);
}
wxString RemoteFileConfigItemGUI::GetValue()
{
	return (mTextEdit->GetValue());
}

void RemoteFileConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newFilename;
	wxString	message;
	wxString	path;
	bool		result;
	long		selectedSize;

	value=GetValue();

	if (mDirectoryConfig!=NULL)
	{
		path=mDirectoryConfig->GetValue();
	}
	else
	{
		path="";
	}
	message="Select File";

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
										mFTPID,
										path,
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
		newFilename		= browserPanel->GetCurrentSelection();
		selectedSize	= browserPanel->mSelectedSize;
		// FTP and UNIX are the same format
		wxFileName	filename(newFilename,wxPATH_UNIX);
		// above path setting doesn't bloody work
		// replace by hand
		mTextEdit->SetValue(filename.GetFullName());
		if (mDirectoryConfig!=NULL)
		{
			path=filename.GetPath();
			path.Replace("\\","/");
			mDirectoryConfig->SetValue(path);
		}
	}
	else
	{
	}
	
}

