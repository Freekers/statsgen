// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "SelectionFreeFormConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "Progress.h"

BEGIN_EVENT_TABLE(SelectionFreeFormConfigItemGUI, wxPanel)
		EVT_SIZE(SelectionFreeFormConfigItemGUI::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,SelectionFreeFormConfigItemGUI::OnTextChange)
		EVT_TEXT(WINDOW_ID_SELECTION_CONFIGVALUE,SelectionFreeFormConfigItemGUI::OnSelectionChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,SelectionFreeFormConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

SelectionFreeFormConfigItemGUI::SelectionFreeFormConfigItemGUI()
{
	maxCharacters=-1;
	configKey="";
}

void SelectionFreeFormConfigItemGUI::OnSelectionChange(wxCommandEvent& event)
{
	wxString	value;
	wxString	code;
	wxString	name;
	int			codeCount;
	int			codeIndex;

	value=choices.GetValue();
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
	if (value.Cmp("")!=0)
	{
		textEdit.SetValue(value);
	}
}
void SelectionFreeFormConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;

	value=textEdit.GetValue();
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

SelectionFreeFormConfigItemGUI::~SelectionFreeFormConfigItemGUI()
{
}

void SelectionFreeFormConfigItemGUI::SetConfigKey(wxString &configKeyIn)
{
	wxString	value;

	configKey=configKeyIn;
	if (configKey.Length()>0)
	{
		globalStatistics.configData.ReadTextValue(configKey,
										&value,
										(char *)defaultValue.GetData());
		textEdit.SetValue(value);
	}
}

void SelectionFreeFormConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn,
			wxArrayString &codesIn,
			wxArrayString &namesIn,
			int maxCharactersIn)
{
	wxSizeEvent	event;
	wxString	customCode="";
	wxString	customName="Custom";
	wxString	code;
	wxString	name;

	labelText=labelTextIn;
	codes.Add(customCode);
	names.Add(customName);
	WX_APPEND_ARRAY(codes,codesIn);
	WX_APPEND_ARRAY(names,namesIn);

	defaultValue=defaultValueIn;
	maxCharacters=maxCharactersIn;
	 
	//SetConfigKey(configKey);

	// Create the config items
	label.Create(this,
				-1,
				labelText,
				wxPoint(0,0));

	choices.Create(this,
				WINDOW_ID_SELECTION_CONFIGVALUE,
				customName,
				wxDefaultPosition,
				wxDefaultSize,
				names,
				wxCB_DROPDOWN|
				wxCB_READONLY);

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

wxString SelectionFreeFormConfigItemGUI::GetValue()
{
	return (textEdit.GetValue());
}

int SelectionFreeFormConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;

	itemSize=label.GetSize();
	return (itemSize.GetWidth());
}

void SelectionFreeFormConfigItemGUI::SetLabelWidth(int width)
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

void SelectionFreeFormConfigItemGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	wxSize		itemSize;
	int			textWidth;
	int			textHeight;
	int			labelWidth;
	int			labelHeight;
	int			choicesWidth;
	int			choicesHeight;
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

	itemSize=choices.GetSize();
	choicesWidth=itemSize.GetWidth();
	choicesHeight=itemSize.GetHeight();

	label.SetSize(0,0,labelWidth,labelHeight);
	labelWidth+=5;

	itemSize=textEdit.GetSize();
	textWidth=itemSize.GetWidth();
	textHeight=itemSize.GetHeight();

	widthForTextEdit=panelWidth-labelWidth-choicesWidth;
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

	choices.SetSize(labelWidth,0,choicesWidth,choicesHeight);
	textEdit.SetSize(labelWidth+choicesWidth,0,widthForTextEdit,textHeight);
}

void SelectionFreeFormConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newDirectory;
	wxString	message;

	value=textEdit.GetValue();

	message="Select Directory";
	newDirectory=wxDirSelector(message,value);
	if (newDirectory.Length()>0)
	{
		textEdit.SetValue(newDirectory);
	}
	
}

void SelectionFreeFormConfigItemGUI::SetValue(wxString &value)
{
	textEdit.SetValue(value);
}

