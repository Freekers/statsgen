// Statsgen Includes
#include "SelectionConfigItemGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(SelectionConfigItemGUI, wxPanel)
		EVT_COMBOBOX(WINDOW_ID_TEXTCTRL_CONFIGVALUE,SelectionConfigItemGUI::OnTextChange)
END_EVENT_TABLE()

SelectionConfigItemGUI::SelectionConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionConfigItemGUI","SelectionConfigItemGUI")
	mLabel		= NULL;
	mTextEdit	= NULL;
	STATSGEN_DEBUG_FUNCTION_END
}

SelectionConfigItemGUI::~SelectionConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionConfigItemGUI","~SelectionConfigItemGUI")
	STATSGEN_DEBUG_FUNCTION_END
}

void SelectionConfigItemGUI::SetSelection(wxArrayString &codes,wxArrayString &names)
{
	int index;
	mCodes.Empty();
	mNames.Empty();
	wxString	value;

	for (index=0;index<codes.GetCount();index++)
	{
		mCodes.Add(codes[index]);
		mNames.Add(names[index]);
		if (codes[index].Cmp(mDefaultValue)==0)
		{
			value = names[index];
		}
	}
	if (mTextEdit != NULL)
	{
		mTextEdit->Set(mNames);
		mTextEdit->SetValue(value);
	}
}

wxString SelectionConfigItemGUI::GetNameFromCode(wxString code)
{
	int			codeIndex;
	wxString	name;

	name = "";
	for (codeIndex=0;codeIndex<mCodes.GetCount();codeIndex++)
	{
		if (mCodes[codeIndex].Cmp(code) == 0)
		{
			return (mNames[codeIndex]);
		}
	}

	return name;
}

void SelectionConfigItemGUI::CreateDisplay(wxWindow *parent,
				int id,
			wxString labelTextIn)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionConfigItemGUI","CreateDisplay")
	wxString	msg;

	mLabelText	= labelTextIn;

	Create(parent,id);

	mLabel = new wxStaticText();
	mLabel->Create(this, wxID_ANY, mLabelText);

	mTextEdit = new wxComboBox();
	mTextEdit->Create(this,
				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
				GetNameFromCode(mDefaultValue),
				wxDefaultPosition,
				wxDefaultSize,
				mNames,
				wxCB_DROPDOWN|
				wxCB_READONLY);

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mLabel,2,wxEXPAND);
	mMainSizer->Add(mTextEdit,8,wxEXPAND);

	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

void SelectionConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionConfigItemGUI","OnTextChange")
	wxString	msg;
	wxString	key;
	wxString	value;
	int			codeCount;
	int			codeIndex;
	wxString	name;
	wxString	code;

	if (mConfigKey.Length() == 0)
	{
		return;
	}
	value=mTextEdit->GetValue();
	codeCount=mCodes.GetCount();
	for (codeIndex=0;codeIndex<codeCount;codeIndex++)
	{
		code=mCodes.Item(codeIndex);
		name=mNames.Item(codeIndex);
		if (name.Cmp(value)==0)
		{
			value=code;
			break;
		}
	}
	globalStatistics.configData.WriteTextValue(mConfigKey, value);
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

void SelectionConfigItemGUI::ApplyConfigKeyChange()
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionConfigItemGUI","ApplyConfigKeyChange")
	wxString	value;
	int			codeCount;
	int			codeIndex;
	wxString	code;
	wxString	name;

	if (mConfigKey.Length() == 0)
	{
		return;
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,mConfigKey)
	globalStatistics.configData.ReadTextValue(mConfigKey, &value, mDefaultValue);
	STATSGEN_DEBUG(DEBUG_ALWAYS,value)
	codeCount=mCodes.GetCount();
	for (codeIndex=0;codeIndex<codeCount;codeIndex++)
	{
		code=mCodes.Item(codeIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,code)
		name=mNames.Item(codeIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,name)
		if (code.Cmp(value)==0)
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Found it - setting value")
			value=name;
			break;
		}
	}
	mTextEdit->SetValue(value);
	STATSGEN_DEBUG_FUNCTION_END
}

wxString SelectionConfigItemGUI::GetValue()
{
	return (mTextEdit->GetValue());
}
