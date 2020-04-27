// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "DirectoryConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "Progress.h"

BEGIN_EVENT_TABLE(DirectoryConfigItemGUI, wxPanel)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,DirectoryConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,DirectoryConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

DirectoryConfigItemGUI::DirectoryConfigItemGUI()
{
}

DirectoryConfigItemGUI::~DirectoryConfigItemGUI()
{
}

void DirectoryConfigItemGUI::CreateDisplay(wxWindow *parent,
			int id,
			wxString &labelTextIn)
{
	wxSize		buttonSize(16,16);

	Create(parent,id);
	mLabelText	= labelTextIn;
	 

	mLabel	= new wxStaticText();
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
}

void DirectoryConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;

	if (mConfigKey.Length()==0)
	{
		return;
	}
	value=GetValue();
	globalStatistics.configData.WriteTextValue(mConfigKey, value);
	if (GetParent()!=NULL)
	{
		wxCommandEvent	newEvent;
		newEvent.SetId(WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		newEvent.SetEventType(wxEVT_COMMAND_TEXT_UPDATED);
		//GetParent()->AddPendingEvent(newEvent);
		GetParent()->GetEventHandler()->AddPendingEvent(newEvent);
	}
}

void DirectoryConfigItemGUI::ApplyConfigKeyChange()
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

wxString DirectoryConfigItemGUI::GetValue()
{
	return (mTextEdit->GetValue());
}

void DirectoryConfigItemGUI::SetValue(wxString &value)
{
	mTextEdit->SetValue(value);
}

void DirectoryConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newDirectory;
	wxString	message;

	value=GetValue();

	message="Select Directory";
	newDirectory=wxDirSelector(message,value);
	if (newDirectory.Length()>0)
	{
		mTextEdit->SetValue(newDirectory);
	}
	
}


