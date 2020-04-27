// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "DirectoryConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "Progress.h"

BEGIN_EVENT_TABLE(DirectoryConfigItemGUI, wxPanel)
		EVT_SIZE(DirectoryConfigItemGUI::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,DirectoryConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,DirectoryConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

DirectoryConfigItemGUI::DirectoryConfigItemGUI()
{
	maxCharacters=-1;
	configKey="";
}

void DirectoryConfigItemGUI::OnTextChange(wxCommandEvent& event)
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

DirectoryConfigItemGUI::~DirectoryConfigItemGUI()
{
}

void DirectoryConfigItemGUI::SetConfigKey(wxString &configKeyIn)
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

void DirectoryConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn,
			int maxCharactersIn)
{
	wxSizeEvent	event;
	wxSize		buttonSize(16,16);

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
	button.Create(this,
				WINDOW_ID_CONFIG_BUTTON,
				wxArtProvider::GetIcon(wxART_FOLDER,
										wxART_OTHER,
										buttonSize));
				
	OnResize(event);
}

wxString DirectoryConfigItemGUI::GetValue()
{
	return (textEdit.GetValue());
}

int DirectoryConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;

	itemSize=label.GetSize();
	return (itemSize.GetWidth());
}

void DirectoryConfigItemGUI::SetLabelWidth(int width)
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

void DirectoryConfigItemGUI::OnResize(wxSizeEvent &event)
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
	int			fixedWidth;
	

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

	button.SetSize(labelWidth,0,buttonWidth,buttonHeight);
	textEdit.SetSize(labelWidth+buttonWidth,0,widthForTextEdit,textHeight);
}

void DirectoryConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
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

void DirectoryConfigItemGUI::SetValue(wxString &value)
{
	textEdit.SetValue(value);
}

