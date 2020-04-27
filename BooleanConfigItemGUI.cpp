// Statsgen Includes
#include "BooleanConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "Progress.h"

BEGIN_EVENT_TABLE(BooleanConfigItemGUI, wxPanel)
		EVT_SIZE(BooleanConfigItemGUI::OnResize)
		EVT_CHECKBOX(WINDOW_ID_TEXTCTRL_CONFIGVALUE,BooleanConfigItemGUI::OnTextChange)
END_EVENT_TABLE()

BooleanConfigItemGUI::BooleanConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","Constructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void BooleanConfigItemGUI::OnTextChange(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","OnTextChange")

	if (textEdit.GetValue())
	{
		value="Y";
	}
	else
	{
		value="N";
	}
	if (configKey.Length()>0)
	{
		globalStatistics.configData.WriteTextValue(configKey,
										value);
	}
	if (GetParent()!=NULL)
	{
		wxCommandEvent		newEvent;
		newEvent.SetId(WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		newEvent.SetEventType(wxEVT_COMMAND_TEXT_UPDATED);
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Popping event");
		GetParent()->AddPendingEvent(newEvent);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

BooleanConfigItemGUI::~BooleanConfigItemGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void BooleanConfigItemGUI::SetConfigKey(wxString &configKeyIn)
{
	wxString	value;

	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","SetConfigKey")
	configKey=configKeyIn;
	if (configKey.Length()>0)
	{
STATSGEN_DEBUG(DEBUG_ALWAYS,"ConfigKeyLength>0")
		globalStatistics.configData.ReadTextValue(configKey,
										&value,
										(char *)defaultValue.GetData());
STATSGEN_DEBUG(DEBUG_ALWAYS,"textEdit.SetValue")
		textEdit.SetValue( (value.CmpNoCase("Y")==0));
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void BooleanConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn)
{
	wxSizeEvent	event;

	configKey=configKeyIn;
	labelText=labelTextIn;
	defaultValue=defaultValueIn;
	 
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","Set")
	//SetConfigKey(configKey);

	STATSGEN_DEBUG(DEBUG_ALWAYS,"label.Create")
	// Create the config items
	label.Create(this,
				-1,
				labelText,
				wxPoint(0,0));

	textEdit.Create(this,
				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
				_T(""));
	SetConfigKey(configKey);
				
	OnResize(event);
	STATSGEN_DEBUG_FUNCTION_END
}

wxString BooleanConfigItemGUI::GetValue()
{
	wxString	value;
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","GetValue")

	if (textEdit.GetValue())
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

int BooleanConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","GetLabelWidth")

	itemSize=label.GetSize();
	STATSGEN_DEBUG_FUNCTION_END
	return (itemSize.GetWidth());
}

void BooleanConfigItemGUI::SetLabelWidth(int width)
{
	wxSize		itemSize;
	wxPoint		itemPosition;

	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","SetLabelWidth")
	itemSize=label.GetSize();
	itemPosition=label.GetPosition();

	label.SetSize(itemPosition.x,
						itemPosition.y,
						width,
						itemSize.GetHeight());
	STATSGEN_DEBUG_FUNCTION_END
}

void BooleanConfigItemGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	wxSize		itemSize;
	int			textWidth;
	int			textHeight;
	int			labelWidth;
	int			labelHeight;
	int			panelWidth;
	int			panelHeight;
	
	STATSGEN_DEBUG_FUNCTION_START("BooleanConfigItemGUI","OnResize")

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

	textEdit.SetSize(labelWidth,0,textWidth,textHeight);
	STATSGEN_DEBUG_FUNCTION_END
}
