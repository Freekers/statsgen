// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "SelectionFreeFormConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "Progress.h"

BEGIN_EVENT_TABLE(SelectionFreeFormConfigItemGUI, wxPanel)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,SelectionFreeFormConfigItemGUI::OnTextChange)
		EVT_TEXT(WINDOW_ID_SELECTION_CONFIGVALUE,SelectionFreeFormConfigItemGUI::OnSelectionChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,SelectionFreeFormConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

SelectionFreeFormConfigItemGUI::SelectionFreeFormConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","SelectionFreeFormConfigItemGUI")
	mChoices	= NULL;
	STATSGEN_DEBUG_FUNCTION_END
}

SelectionFreeFormConfigItemGUI::~SelectionFreeFormConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","~SelectionFreeFormConfigItemGUI")
	STATSGEN_DEBUG_FUNCTION_END
}

void SelectionFreeFormConfigItemGUI::SetSelection(wxArrayString &codes,wxArrayString &names)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","SetSelection")
	int			index;
	wxString	value;

	mCodes.Empty();
	mNames.Empty();

	for (index=0;index<codes.GetCount();index++)
	{
		mCodes.Add(codes[index]);
		mNames.Add(names[index]);
		if (codes[index].Cmp(mDefaultValue)==0)
		{
			value = names[index];
		}
	}
	if (mChoices != NULL)
	{
		mChoices->Set(mNames);
		mChoices->SetValue(value);
	}
	STATSGEN_DEBUG_FUNCTION_END
}
wxString SelectionFreeFormConfigItemGUI::GetNameFromCode(wxString code)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","GetNameFromCode")
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

	STATSGEN_DEBUG_FUNCTION_END
	return name;
}

void SelectionFreeFormConfigItemGUI::CreateDisplay(
			wxWindow *parent,
			int id,
			wxString labelTextIn)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","CreateDisplay")
	wxString	customCode="";
	wxString	customName="Custom";
	wxString	code;
	wxString	name;

	mLabelText	= labelTextIn;

	Create(parent,id);

	mLabel = new wxStaticText();
	mLabel->Create(this,wxID_ANY,mLabelText);

	mChoices = new wxComboBox();
	mChoices->Create(this,
				WINDOW_ID_SELECTION_CONFIGVALUE,
				customName,
				wxDefaultPosition,
				wxDefaultSize,
				mNames,
				wxCB_DROPDOWN|
				wxCB_READONLY);

	mTextEdit = new wxTextCtrl();
	mTextEdit->Create(this,
				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				0,
				wxDefaultValidator);

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mLabel,2,wxEXPAND);
	mMainSizer->Add(mChoices,2,wxEXPAND);
	mMainSizer->Add(mTextEdit,6,wxEXPAND);

	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

void SelectionFreeFormConfigItemGUI::OnSelectionChange(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","OnSelectionChange")
	wxString	value;
	wxString	code;
	wxString	name;
	int			codeCount;
	int			codeIndex;

	value=mChoices->GetValue();
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
	if (value.Cmp("")!=0)
	{
		mTextEdit->SetValue(value);
	}
	STATSGEN_DEBUG_FUNCTION_END
}
void SelectionFreeFormConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","OnTextChange")
	wxString	msg;
	wxString	key;
	wxString	value;

	if (mConfigKey.Length() == 0)
	{
		STATSGEN_DEBUG_FUNCTION_END
		return;
	}
	value=mTextEdit->GetValue();
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

void SelectionFreeFormConfigItemGUI::ApplyConfigKeyChange()
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","ApplyConfigKeyChange")
	wxString value;

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

wxString SelectionFreeFormConfigItemGUI::GetValue()
{
	return (mTextEdit->GetValue());
}

void SelectionFreeFormConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","OnButtonPressed")
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newDirectory;
	wxString	message;

	value=mTextEdit->GetValue();

	message="Select Directory";
	newDirectory=wxDirSelector(message,value);
	if (newDirectory.Length()>0)
	{
		mTextEdit->SetValue(newDirectory);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void SelectionFreeFormConfigItemGUI::SetValue(wxString &value)
{
	STATSGEN_DEBUG_FUNCTION_START("SelectionFreeFormConfigItemGUI","SetValue")
	mTextEdit->SetValue(value);
	STATSGEN_DEBUG_FUNCTION_END
}

