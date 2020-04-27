// Statsgen Includes
#include "BooleanConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "Progress.h"

BEGIN_EVENT_TABLE(BooleanConfigItemGUI, wxPanel)
		EVT_CHECKBOX(WINDOW_ID_TEXTCTRL_CONFIGVALUE,BooleanConfigItemGUI::OnTextChange)
END_EVENT_TABLE()

BooleanConfigItemGUI::BooleanConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","Constructor")
	STATSGEN_DEBUG_FUNCTION_END
}

BooleanConfigItemGUI::~BooleanConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void BooleanConfigItemGUI::CreateDisplay(wxWindow *parent,
				int id,
				wxString labelText)
{
	Create(parent,id);
	mLabelText		= labelText;
	mLabel			= new wxStaticText();
	mLabel->Create(this, wxID_ANY, labelText);

	mTextEdit = new wxCheckBox();
	mTextEdit->Create(this,
				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				0,
				wxDefaultValidator);

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mLabel,2,wxEXPAND);
	mMainSizer->Add(mTextEdit,8,wxEXPAND);

	ConfigureSizer();
}

void BooleanConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","OnTextChange")

	if (mConfigKey.Length() == 0)
	{
		return;
	}
	value = GetValue();
	globalStatistics.configData.WriteTextValue(mConfigKey,
										value);
	if (GetParent()!=NULL)
	{
		wxCommandEvent		newEvent;
		newEvent.SetId(WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		newEvent.SetEventType(wxEVT_COMMAND_TEXT_UPDATED);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Popping event");
		GetParent()->GetEventHandler()->AddPendingEvent(newEvent);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void BooleanConfigItemGUI::ApplyConfigKeyChange()
{
	wxString	value;

	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","ApplyConfigKeyChange")
	if (mConfigKey.Length()==0)
	{
		return;
	}
	globalStatistics.configData.ReadTextValue(mConfigKey,
										&value,
										mDefaultValue);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"textEdit.SetValue")
	mTextEdit->SetValue( (value.CmpNoCase("Y")==0));
	STATSGEN_DEBUG_FUNCTION_END
}

wxString BooleanConfigItemGUI::GetValue()
{
	wxString	value;
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","GetValue")

	if (mTextEdit->GetValue())
	{
		value="Y";
	}
	else
	{
		value="N";
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (value);
}
