// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "RemoteDirectoryConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "FTPBrowserPanel.h"
#include "GenericOKCancelDialog.h"

BEGIN_EVENT_TABLE(RemoteDirectoryConfigItemGUI, wxPanel)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,RemoteDirectoryConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,RemoteDirectoryConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

RemoteDirectoryConfigItemGUI::RemoteDirectoryConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","RemoteDirectoryConfigItemGUI")
	mFTPID	= "";
	STATSGEN_DEBUG_FUNCTION_END
}

RemoteDirectoryConfigItemGUI::~RemoteDirectoryConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","~RemoteDirectoryConfigItemGUI")
	STATSGEN_DEBUG_FUNCTION_END
}

void RemoteDirectoryConfigItemGUI::SetConfigKey(wxString configKey,wxString defaultValue,wxString id)
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","SetConfigKey")
	mFTPID	= id;
	BaseConfigItemGUI::SetConfigKey(configKey,defaultValue);
	STATSGEN_DEBUG_FUNCTION_END
}

void RemoteDirectoryConfigItemGUI::CreateDisplay(
			wxWindow *parent,
			int id,
			wxString &labelTextIn)
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","CreateDisplay")
	wxSize		buttonSize(16,16);

	mLabelText=labelTextIn;

	Create(parent,id);
	 

	mLabel = new wxStaticText();
	mLabel->Create(this,wxID_ANY,mLabelText);

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
				wxArtProvider::GetIcon(wxART_FOLDER,
										wxART_OTHER,
										buttonSize));

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mLabel,2,wxEXPAND);
	mMainSizer->Add(mButton,0);
	mMainSizer->Add(mTextEdit,7,wxEXPAND);

	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

void RemoteDirectoryConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","OnTextChange")
	wxString	msg;
	wxString	key;
	wxString	value;

	if (mConfigKey.Length() == 0)
	{
		STATSGEN_DEBUG_FUNCTION_END
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
	STATSGEN_DEBUG_FUNCTION_END
}

void RemoteDirectoryConfigItemGUI::ApplyConfigKeyChange()
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","ApplyConfigKeyChange")
	wxString	value;

	if (mConfigKey.Length() == 0)
	{
		STATSGEN_DEBUG_FUNCTION_END
		return;
	}
	globalStatistics.configData.ReadTextValue(mConfigKey,
										&value,
										mDefaultValue);
	mTextEdit->SetValue(value);
	STATSGEN_DEBUG_FUNCTION_END
}

wxString RemoteDirectoryConfigItemGUI::GetValue()
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","GetValue")
	wxString 	result;
	result = mTextEdit->GetValue();

	STATSGEN_DEBUG_FUNCTION_END
	return result;
}

void RemoteDirectoryConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","OnButtonPressed")
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newDirectory;
	wxString	message;
	bool		result;
	wxString	newFilename;
	long		selectedSize;
	wxString	path;

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Getting value");
	value=mTextEdit->GetValue();
	path=value;

	message="Select Directory";
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating GenericOKCancelDialog");
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
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating FTPBrowserPanel");
	FTPBrowserPanel	*browserPanel=new FTPBrowserPanel(
										mFTPID,
										path,
										false);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"FTPBrowserPanel->Create");
	browserPanel->Create(&dialog,
						wxID_ANY,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"FTPBrowserPanel->CreateScreen");
	browserPanel->CreateScreen();
	
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"dialog.DisplayDialog");
	result = dialog.DisplayDialog(browserPanel);
	if (result)
	{
		newFilename=browserPanel->GetCurrentSelection();
		selectedSize=browserPanel->mSelectedSize;
		if (selectedSize>=0)
		{
			// Selected a file - get directory part
			mTextEdit->SetValue(browserPanel->CurrentPath());
		}
		else
		{
			// Selected directory - just use selection
			mTextEdit->SetValue(newFilename);
		}
	}
	else
	{
	}
	

	/*
	newDirectory=wxDirSelector(message,value);
	if (newDirectory.Length()>0)
	{
		textEdit.SetValue(newDirectory);
	}
	*/
	
	STATSGEN_DEBUG_FUNCTION_END
}

void RemoteDirectoryConfigItemGUI::SetValue(wxString &value)
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteDirectoryConfigItemGUI","SetValue")
	mTextEdit->SetValue(value);
	STATSGEN_DEBUG_FUNCTION_END
}

