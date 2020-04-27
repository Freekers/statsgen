// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "FileConfigItemGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(FileConfigItemGUI, wxPanel)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,FileConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,FileConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

FileConfigItemGUI::FileConfigItemGUI()
{
	mDirectoryConfig	= NULL;
}

FileConfigItemGUI::~FileConfigItemGUI()
{
}

void FileConfigItemGUI::CreateDisplay(
			wxWindow *parent,
			int id,
			wxString &labelTextIn,
			DirectoryConfigItemGUI *directoryConfigIn)
{
	wxSize		buttonSize(16,16);	
	Create(parent,id);
	mLabelText			= labelTextIn;
	mDirectoryConfig	= directoryConfigIn;
	 
	//SetConfigKey(configKey);

	// Create the config items
	mLabel = new wxStaticText();
	mLabel->Create(this,wxID_ANY, mLabelText);

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

void FileConfigItemGUI::OnTextChange(wxCommandEvent& event)
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

void FileConfigItemGUI::ApplyConfigKeyChange()
{
	wxString	value;

	if (mConfigKey.Length() == 0)
	{
			return;
	}
	globalStatistics.configData.ReadTextValue(mConfigKey,
										&value,
										mDefaultValue);
	mTextEdit->SetValue(value);
}

wxString FileConfigItemGUI::GetValue()
{
	return (mTextEdit->GetValue());
}

void FileConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newFilename;
	wxString	message;
	wxString	path;

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
	newFilename=wxFileSelector(message,path);
	if (newFilename.Length()>0)
	{
		wxFileName	filename(newFilename);
		if (mDirectoryConfig!=NULL)
		{
			mTextEdit->SetValue(filename.GetFullName());
			path=filename.GetPath();
			mDirectoryConfig->SetValue(path);
		}
		else
		{
			mTextEdit->SetValue(filename.GetFullPath());
		}
	}
}
