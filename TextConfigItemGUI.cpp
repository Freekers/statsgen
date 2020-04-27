// Statsgen Includes
#include "TextConfigItemGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(TextConfigItemGUI, wxPanel)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,TextConfigItemGUI::OnTextChange)
END_EVENT_TABLE()

TextConfigItemGUI::TextConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("TextConfigItemGUI","TextConfigItemGUI")
	STATSGEN_DEBUG_FUNCTION_END
}

TextConfigItemGUI::~TextConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("TextConfigItemGUI","~TextConfigItemGUI")
	STATSGEN_DEBUG_FUNCTION_END
}

void TextConfigItemGUI::CreateDisplay(wxWindow *parent,int id,wxString labelText)
{
	Create(parent,id);
	mLabelText		= labelText;
	mLabel = new wxStaticText();
	mLabel->Create(this, wxID_ANY, labelText);
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
	mMainSizer->Add(mTextEdit,8,wxEXPAND);

	ConfigureSizer();
}

void TextConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("TextConfigItemGUI","OnTextChange")
	wxString		key;
	wxString		value;
	wxString		keyOnly;
	wxString		pathOnly;
	wxString		weaponGroupKey;
	wxArrayString	weaponGroupKeys;
	int				keyCount;
	int				keyIndex;
	wxString		weaponGroupConfigKey;

	if (mConfigKey.Length()==0)
	{
		return;
	}
	value = GetValue();
	keyOnly=mConfigKey.AfterLast('/');
	pathOnly=mConfigKey.BeforeLast('/');
	if (globalStatistics.configData.IsWeaponGroupKey(keyOnly))
	{
		if (value.Length()>0)
		{
			weaponGroupKeys=globalStatistics.configData.WeaponGroupKeys(keyOnly);
			keyCount=weaponGroupKeys.GetCount();
			for (keyIndex=0;keyIndex<keyCount;keyIndex++)
			{
				keyOnly=weaponGroupKeys.Item(keyIndex);
				weaponGroupConfigKey.Printf("%s/weapon_%s",
					STRING_TO_CHAR(pathOnly),
					STRING_TO_CHAR(keyOnly));
				globalStatistics.configData.WriteTextValue(
						weaponGroupConfigKey,
						value);
			}
		}
	}
	else
	{
		globalStatistics.configData.WriteTextValue(mConfigKey,
										value);
	}
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

void TextConfigItemGUI::ApplyConfigKeyChange()
{
	STATSGEN_DEBUG_FUNCTION_START("TextConfigItemGUI","ApplyConfigKeyChange")
	wxString		value;
	wxString		keyOnly;
	wxString		pathOnly;
	wxString		weaponGroupKey;
	wxArrayString	weaponGroupKeys;
	int				keyCount;
	int				keyIndex;
	wxString		weaponGroupConfigKey;
	wxString		weaponGroupValue;
	wxString		msg;

	if (mConfigKey.Length() == 0)
	{
		return;
	}
	keyOnly=mConfigKey.AfterLast('/');
	pathOnly=mConfigKey.BeforeLast('/');
	value="";
	if (globalStatistics.configData.IsWeaponGroupKey(keyOnly))
	{
		weaponGroupKeys=globalStatistics.configData.WeaponGroupKeys(keyOnly);
		keyCount=weaponGroupKeys.GetCount();
		for (keyIndex=0;keyIndex<keyCount;keyIndex++)
		{
			keyOnly=weaponGroupKeys.Item(keyIndex);
			weaponGroupConfigKey.Printf("%s/weapon_%s",
						STRING_TO_CHAR(pathOnly),
						STRING_TO_CHAR(keyOnly));
			globalStatistics.configData.ReadTextValue(weaponGroupConfigKey,
										&weaponGroupValue,
										mDefaultValue);
			if (keyIndex==0)
			{
				value=weaponGroupValue;
			}
			else
			{
				if (value.CmpNoCase(weaponGroupValue)!=0)
				{
					value="";
					break;
				}
			}
		}
	}
	else
	{
		globalStatistics.configData.ReadTextValue(mConfigKey,
										&value,
										mDefaultValue);
	}
	mTextEdit->SetValue(value);
	STATSGEN_DEBUG_FUNCTION_END
}

wxString TextConfigItemGUI::GetValue()
{
	return (mTextEdit->GetValue());
}
