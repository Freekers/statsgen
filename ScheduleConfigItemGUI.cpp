// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "ScheduleConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "ScheduleDialog.h"

BEGIN_EVENT_TABLE(ScheduleConfigItemGUI, wxPanel)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,ScheduleConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,ScheduleConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

ScheduleConfigItemGUI::ScheduleConfigItemGUI()
{
	mIntervalWithBase=false;
}

ScheduleConfigItemGUI::~ScheduleConfigItemGUI()
{
}

void ScheduleConfigItemGUI::CreateDisplay(wxWindow *parent,int id,wxString &labelTextIn)
{
	Create(parent,id);

	mLabelText		= labelTextIn;

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

	wxString	resourceID="CLOCK_ICON";
	wxIcon buttonIcon(resourceID, wxBITMAP_TYPE_ICO_RESOURCE, 16,16);
	mButton = new wxBitmapButton();
	mButton->Create(this,
				WINDOW_ID_CONFIG_BUTTON,
				buttonIcon);

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mLabel,2,wxEXPAND);
	mMainSizer->Add(mButton);
	mMainSizer->Add(mTextEdit,7,wxEXPAND);

	ConfigureSizer();
}

void ScheduleConfigItemGUI::OnTextChange(wxCommandEvent& event)
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

void ScheduleConfigItemGUI::SetConfigKey(wxString configKey,wxString defaultValue,bool intervalWithBase)
{
	BaseConfigItemGUI::SetConfigKey(configKey,defaultValue);
	mIntervalWithBase = intervalWithBase;
}

void ScheduleConfigItemGUI::ApplyConfigKeyChange()
{
	wxString	value;
	globalStatistics.configData.ReadTextValue(mConfigKey,&value,mDefaultValue);

	mTextEdit->SetValue(value);
}

wxString ScheduleConfigItemGUI::GetValue()
{
	return (mTextEdit->GetValue());
}

void ScheduleConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	wxString		msg;
	wxString		key;
	wxString		value;
	const wxString		title="Schedule";
	const wxString		name="ScheduleEditor";
	long			style=wxCAPTION |
							wxCLOSE_BOX |
							wxSYSTEM_MENU |
							wxRESIZE_BORDER |
							wxMAXIMIZE_BOX;
	wxWindowID		id=wxID_ANY;
	wxSize	size=wxSize(200,100);
	wxPoint	point=wxPoint(100,100);

	value=mTextEdit->GetValue();
	ScheduleDialog	scheduleDialog((wxWindow *)this,
									(wxWindowID)id,
									title,
									value,
									(bool)mIntervalWithBase,
									point,
									size,
									(long)style,
									name);
	if (scheduleDialog.DisplayDialog())
	{
		mTextEdit->SetValue(scheduleDialog.GetValue());
	}
	else
	{
		// Cancel
	}
}
