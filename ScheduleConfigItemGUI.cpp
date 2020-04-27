// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "ScheduleConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "ScheduleDialog.h"

BEGIN_EVENT_TABLE(ScheduleConfigItemGUI, wxPanel)
		EVT_SIZE(ScheduleConfigItemGUI::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,ScheduleConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,ScheduleConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

ScheduleConfigItemGUI::ScheduleConfigItemGUI()
{
	configKey="";
	intervalWithBase=false;
}

void ScheduleConfigItemGUI::OnTextChange(wxCommandEvent& event)
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

ScheduleConfigItemGUI::~ScheduleConfigItemGUI()
{
}

void ScheduleConfigItemGUI::SetConfigKey(wxString &configKeyIn)
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

void ScheduleConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn,
			bool intervalWithBaseIn)
{
	wxSizeEvent	event;
	wxSize		buttonSize(16,16);

	labelText=labelTextIn;
	defaultValue=defaultValueIn;
	intervalWithBase=intervalWithBaseIn;
	 
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

	//button.Create(this,
	//			WINDOW_ID_CONFIG_BUTTON,
	//			wxArtProvider::GetIcon(wxART_FILE_OPEN,
	//									wxART_OTHER,
	//									buttonSize));
	wxString	resourceID="CLOCK_ICON";
	wxIcon buttonIcon(resourceID,
					wxBITMAP_TYPE_ICO_RESOURCE,
					16,16);
	button.Create(this,
				WINDOW_ID_CONFIG_BUTTON,
				buttonIcon);
	SetConfigKey(configKey);
				
	OnResize(event);
}

wxString ScheduleConfigItemGUI::GetValue()
{
	return (textEdit.GetValue());
}

int ScheduleConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;

	itemSize=label.GetSize();
	return (itemSize.GetWidth());
}

void ScheduleConfigItemGUI::SetLabelWidth(int width)
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

void ScheduleConfigItemGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	wxSize		itemSize;
	int			textWidth;
	int			textHeight;
	int			labelWidth;
	int			labelHeight;
	int			panelWidth;
	int			panelHeight;
	int			buttonWidth;
	int			buttonHeight;
	int			widthForTextEdit;
	

	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();

	itemSize=label.GetSize();
	labelWidth=itemSize.GetWidth();
	labelHeight=itemSize.GetHeight();

	itemSize=button.GetSize();
	buttonWidth=itemSize.GetWidth();
	buttonHeight=itemSize.GetHeight();

	label.SetSize(0,0,labelWidth,labelHeight);
	labelWidth+=5;

	itemSize=textEdit.GetSize();
	textWidth=itemSize.GetWidth();
	textHeight=itemSize.GetHeight();

	widthForTextEdit=panelWidth-labelWidth-buttonWidth;
	if (widthForTextEdit<1)
	{
		widthForTextEdit=1;
	}

	button.SetSize(labelWidth,0,buttonWidth,buttonHeight);
	textEdit.SetSize(labelWidth+buttonWidth,0,widthForTextEdit,textHeight);
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
	wxWindowID		id=-1;
	wxPoint	position=wxDefaultPosition;
	wxSize	size=wxSize(200,100);

	value=textEdit.GetValue();
	ScheduleDialog	scheduleDialog((wxWindow *)this,
									(wxWindowID)id,
									title,
									value,
									(bool)intervalWithBase,
									position,
									size,
									(long)style,
									name);
	if (scheduleDialog.DisplayDialog())
	{
		textEdit.SetValue(scheduleDialog.GetValue());
	}
	else
	{
		// Cancel
	}
}
