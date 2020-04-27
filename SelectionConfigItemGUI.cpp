// Statsgen Includes
#include "SelectionConfigItemGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(SelectionConfigItemGUI, wxPanel)
		EVT_SIZE(SelectionConfigItemGUI::OnResize)
		EVT_COMBOBOX(WINDOW_ID_TEXTCTRL_CONFIGVALUE,SelectionConfigItemGUI::OnTextChange)
END_EVENT_TABLE()

SelectionConfigItemGUI::SelectionConfigItemGUI()
{
}

void SelectionConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	int			codeCount;
	int			codeIndex;
	wxString	name;
	wxString	code;

	value=textEdit.GetValue();
	codeCount=codes.GetCount();
	for (codeIndex=0;codeIndex<codeCount;codeIndex++)
	{
		code=codes.Item(codeIndex);
		name=names.Item(codeIndex);
		if (name.Cmp(value)==0)
		{
			value=code;
			break;
		}
	}
	if (configKey.Length()>0)
	{
		globalStatistics.configData.WriteTextValue(configKey,
										value);
	}
	if (GetParent()!=NULL)
	{
		wxCommandEvent	newEvent;
		newEvent.SetId(WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		newEvent.SetEventType(wxEVT_COMMAND_TEXT_UPDATED);
		GetParent()->AddPendingEvent(newEvent);
	}
}

SelectionConfigItemGUI::~SelectionConfigItemGUI()
{
}

void SelectionConfigItemGUI::SetConfigKey(wxString &configKeyIn)
{
	wxString	value;
	int			codeCount;
	int			codeIndex;
	wxString	code;
	wxString	name;

	STATSGEN_DEBUG_FUNCTION_START("SelectionConfigItemGUI","SetConfigKey")

	configKey=configKeyIn;
	STATSGEN_DEBUG(DEBUG_ALWAYS,configKey)
	if (configKey.Length()>0)
	{
		globalStatistics.configData.ReadTextValue(configKey,
										&value,
										(char *)defaultValue.GetData());
		STATSGEN_DEBUG(DEBUG_ALWAYS,value)
		codeCount=codes.GetCount();
		for (codeIndex=0;codeIndex<codeCount;codeIndex++)
		{
			code=codes.Item(codeIndex);
			STATSGEN_DEBUG(DEBUG_ALWAYS,code)
			name=names.Item(codeIndex);
			STATSGEN_DEBUG(DEBUG_ALWAYS,name)
			if (code.Cmp(value)==0)
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,"Found it - setting value")
				value=name;
				break;
			}
		}
		textEdit.SetValue(value);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void SelectionConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn,
			wxArrayString &codesIn,
			wxArrayString &namesIn)
{
	wxSizeEvent	event;
	int			codeCount;
	int			codeIndex;
	wxString	code;
	wxString	msg;
	wxString	name;
	wxString	defaultName="";

	configKey=configKeyIn;
	labelText=labelTextIn;
	codes=codesIn;
	names=namesIn;
	defaultValue=defaultValueIn;
	codeCount=codes.GetCount();
	for (codeIndex=0;codeIndex<codeCount;codeIndex++)
	{
		code=codes.Item(codeIndex);
		name=names.Item(codeIndex);
		if (code.Cmp(defaultValue)==0)
		{
			defaultName=name;
			break;
		}
	}

	SetConfigKey(configKey);
	// Create the config items
	label.Create(this,
				-1,
				labelText,
				wxPoint(0,0));

	textEdit.Create(this,
				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
				defaultName,
				wxDefaultPosition,
				wxDefaultSize,
				names,
				wxCB_DROPDOWN|
				wxCB_READONLY);
	SetConfigKey(configKey);
				
	OnResize(event);
}

wxString SelectionConfigItemGUI::GetValue()
{
	return (textEdit.GetValue());
}

int SelectionConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;

	itemSize=label.GetSize();
	return (itemSize.GetWidth());
}

void SelectionConfigItemGUI::SetLabelWidth(int width)
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

void SelectionConfigItemGUI::OnResize(wxSizeEvent &event)
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
	widthForTextEdit=textWidth;

	textEdit.SetSize(labelWidth,0,widthForTextEdit,textHeight);
}
