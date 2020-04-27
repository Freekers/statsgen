// Statsgen Includes
#include "TextConfigItemGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(TextConfigItemGUI, wxPanel)
		EVT_SIZE(TextConfigItemGUI::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,TextConfigItemGUI::OnTextChange)
END_EVENT_TABLE()

TextConfigItemGUI::TextConfigItemGUI()
{
	maxCharacters=-1;
}

void TextConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	wxString		key;
	wxString		value;
	wxString		keyOnly;
	wxString		pathOnly;
	wxString		weaponGroupKey;
	wxArrayString	weaponGroupKeys;
	int				keyCount;
	int				keyIndex;
	wxString		weaponGroupConfigKey;

	value=textEdit.GetValue();
	if (configKey.Length()>0)
	{
		keyOnly=configKey.AfterLast('/');
		pathOnly=configKey.BeforeLast('/');
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
							pathOnly.GetData(),
							keyOnly.GetData());
					globalStatistics.configData.WriteTextValue(
								weaponGroupConfigKey,
								value);
				}
			}
		}
		else
		{
			globalStatistics.configData.WriteTextValue(configKey,
										value);
		}
	}
	if (GetParent()!=NULL)
	{
		wxCommandEvent	newEvent;
		newEvent.SetId(WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		newEvent.SetEventType(wxEVT_COMMAND_TEXT_UPDATED);
		GetParent()->AddPendingEvent(newEvent);
	}
}

TextConfigItemGUI::~TextConfigItemGUI()
{
}

void TextConfigItemGUI::SetConfigKey(wxString &configKeyIn)
{
	wxString	value;
	wxString		keyOnly;
	wxString		pathOnly;
	wxString		weaponGroupKey;
	wxArrayString	weaponGroupKeys;
	int				keyCount;
	int				keyIndex;
	wxString		weaponGroupConfigKey;
	wxString		weaponGroupValue;
	wxString		msg;

	configKey=configKeyIn;
	if (configKey.Length()>0)
	{
		keyOnly=configKey.AfterLast('/');
		pathOnly=configKey.BeforeLast('/');
		value="";
		if (globalStatistics.configData.IsWeaponGroupKey(keyOnly))
		{
//msg.Printf("SetConfigKey::%s:%s:%s",configKeyIn.GetData(),pathOnly.GetData(),keyOnly.GetData());wxMessageBox(msg);
			weaponGroupKeys=globalStatistics.configData.WeaponGroupKeys(keyOnly);
			keyCount=weaponGroupKeys.GetCount();
			for (keyIndex=0;keyIndex<keyCount;keyIndex++)
			{
				keyOnly=weaponGroupKeys.Item(keyIndex);
				weaponGroupConfigKey.Printf("%s/weapon_%s",
						pathOnly.GetData(),
						keyOnly.GetData());
				globalStatistics.configData.ReadTextValue(weaponGroupConfigKey,
										&weaponGroupValue,
										(char *)defaultValue.GetData());
//msg.Printf("SetConfigKey::%s:%s:%s",value.GetData(),weaponGroupConfigKey.GetData(),weaponGroupValue.GetData());wxMessageBox(msg);
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
			globalStatistics.configData.ReadTextValue(configKey,
										&value,
										(char *)defaultValue.GetData());
		}
		textEdit.SetValue(value);
	}
}

void TextConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn,
			int maxCharactersIn)
{
	wxSizeEvent	event;

	labelText=labelTextIn;
	defaultValue=defaultValueIn;
	maxCharacters=maxCharactersIn;
	 
	//SetConfigKey(configKey);

	// Create the config items
	label.Create(this,
				-1,
				labelText,
				wxPoint(0,0));

	textEdit.Create(this,
				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
				defaultValue,
				wxDefaultPosition,
				wxDefaultSize,
				0,
				wxDefaultValidator);
	configKey=configKeyIn;
	SetConfigKey(configKey);
				
	OnResize(event);
}

wxString TextConfigItemGUI::GetValue()
{
	return (textEdit.GetValue());
}

int TextConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;

	itemSize=label.GetSize();
	return (itemSize.GetWidth());
}

void TextConfigItemGUI::SetLabelWidth(int width)
{
	wxSize		itemSize;
	wxPoint		itemPosition;

	itemSize=label.GetSize();
	itemPosition=label.GetPosition();

	label.SetSize(itemPosition.x,
						itemPosition.y,
						width,
						itemSize.GetHeight());
}

void TextConfigItemGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	wxSize		itemSize;
	int			textWidth;
	int			textHeight;
	int			labelWidth;
	int			labelHeight;
	int			panelWidth;
	int			panelHeight;
	int			widthForTextEdit;
	int			fixedWidth;
	

	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();

	itemSize=label.GetSize();
	labelWidth=itemSize.GetWidth();
	labelHeight=itemSize.GetHeight();

	label.SetSize(0,0,labelWidth,labelHeight);
	labelWidth+=5;

	itemSize=textEdit.GetSize();
	textWidth=itemSize.GetWidth();
	textHeight=itemSize.GetHeight();

	widthForTextEdit=panelWidth-labelWidth;
	fixedWidth=(maxCharacters * FONT_CHAR_WIDTH)+FONT_TEXT_CTRL_GAP;
	if (maxCharacters>0)
	{
		// We have an overriding field width
		if (fixedWidth<widthForTextEdit)
		{
			widthForTextEdit=fixedWidth;
		}
	}
	if (widthForTextEdit<1)
	{
		widthForTextEdit=1;
	}

	textEdit.SetSize(labelWidth,0,widthForTextEdit,textHeight);
}
