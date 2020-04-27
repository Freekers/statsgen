// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "FileConfigItemGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(FileConfigItemGUI, wxPanel)
		EVT_SIZE(FileConfigItemGUI::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,FileConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,FileConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

FileConfigItemGUI::FileConfigItemGUI()
{
	configKey="";
	maxCharacters=-1;
	directoryConfig=NULL;
}

void FileConfigItemGUI::OnTextChange(wxCommandEvent& event)
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

FileConfigItemGUI::~FileConfigItemGUI()
{
}

void FileConfigItemGUI::SetConfigKey(wxString &configKeyIn)
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

void FileConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn,
			int maxCharactersIn,
			DirectoryConfigItemGUI *directoryConfigIn)
{
	wxSizeEvent	event;
	wxSize		buttonSize(16,16);

	labelText=labelTextIn;
	defaultValue=defaultValueIn;
	maxCharacters=maxCharactersIn;
	directoryConfig=directoryConfigIn;
	 
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

	button.Create(this,
				WINDOW_ID_CONFIG_BUTTON,
				wxArtProvider::GetIcon(wxART_FILE_OPEN,
										wxART_OTHER,
										buttonSize));
	SetConfigKey(configKey);
				
	OnResize(event);
}

wxString FileConfigItemGUI::GetValue()
{
	return (textEdit.GetValue());
}

int FileConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;

	itemSize=label.GetSize();
	return (itemSize.GetWidth());
}

void FileConfigItemGUI::SetLabelWidth(int width)
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

void FileConfigItemGUI::OnResize(wxSizeEvent &event)
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

void FileConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newFilename;
	wxString	message;
	wxString	path;

	value=textEdit.GetValue();

	if (directoryConfig!=NULL)
	{
		path=directoryConfig->GetValue();
	}
	else
	{
		path="";
	}
	message="Select File";
	newFilename=wxFileSelector(message,path);
	if (newFilename.Length()>0)
	{
		wxFileName	filename(newFilename);
		if (directoryConfig!=NULL)
		{
			textEdit.SetValue(filename.GetFullName());
			path=filename.GetPath();
			directoryConfig->SetValue(path);
		}
		else
		{
			textEdit.SetValue(filename.GetFullPath());
		}
	}
	
}
