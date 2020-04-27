// wx includes
#include "wx/artprov.h"
#include "wx/filename.h"

// Statsgen Includes
#include "RemoteDirectoryConfigItemGUI.h"
#include "GlobalStatistics.h"
#include "FTPBrowserPanel.h"
#include "GenericOKCancelDialog.h"

BEGIN_EVENT_TABLE(RemoteDirectoryConfigItemGUI, wxPanel)
		EVT_SIZE(RemoteDirectoryConfigItemGUI::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,RemoteDirectoryConfigItemGUI::OnTextChange)
		EVT_BUTTON(WINDOW_ID_CONFIG_BUTTON,RemoteDirectoryConfigItemGUI::OnButtonPressed)
END_EVENT_TABLE()

RemoteDirectoryConfigItemGUI::RemoteDirectoryConfigItemGUI()
{
	maxCharacters=-1;
}

void RemoteDirectoryConfigItemGUI::OnTextChange(wxCommandEvent& event)
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

RemoteDirectoryConfigItemGUI::~RemoteDirectoryConfigItemGUI()
{
}

void RemoteDirectoryConfigItemGUI::SetConfigKey(wxString &configKeyIn)
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

void RemoteDirectoryConfigItemGUI::Set(
			wxString &configKeyIn,
			wxString &labelTextIn,
			wxString &defaultValueIn,
			int maxCharactersIn,
			wxString &FTPIDIn)
{
	wxSizeEvent	event;
	wxSize		buttonSize(16,16);

	configKey=configKeyIn;
	labelText=labelTextIn;
	defaultValue=defaultValueIn;
	maxCharacters=maxCharactersIn;
	FTPID=FTPIDIn;
	 

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

	
	SetConfigKey(configKey);
	button.Create(this,
				WINDOW_ID_CONFIG_BUTTON,
				wxArtProvider::GetIcon(wxART_FOLDER,
										wxART_OTHER,
										buttonSize));
				
	OnResize(event);
}

wxString RemoteDirectoryConfigItemGUI::GetValue()
{
	return (textEdit.GetValue());
}

int RemoteDirectoryConfigItemGUI::GetLabelWidth()
{
	wxSize		itemSize;

	itemSize=label.GetSize();
	return (itemSize.GetWidth());
}

void RemoteDirectoryConfigItemGUI::SetLabelWidth(int width)
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

void RemoteDirectoryConfigItemGUI::OnResize(wxSizeEvent &event)
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

void RemoteDirectoryConfigItemGUI::OnButtonPressed(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	newDirectory;
	wxString	message;
	bool		result;
	wxString	newFilename;
	long		selectedSize;
	wxString	path;

	value=textEdit.GetValue();
	path=value;

	message="Select Directory";
	GenericOKCancelDialog dialog(this,-1,
					message,
					wxDefaultPosition,
					wxDefaultSize,
					wxCAPTION |
					wxCLOSE_BOX |
					wxSYSTEM_MENU |
					wxRESIZE_BORDER |
					wxMAXIMIZE_BOX,
					_T(""));
	FTPBrowserPanel	*browserPanel=new FTPBrowserPanel(
										FTPID,
										path,
										false);
	browserPanel->Create(&dialog,
						-1,
						wxDefaultPosition,
						wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	browserPanel->CreateScreen();
	
	dialog.SetPanel(browserPanel);
	dialog.CreateDialog();
	result=(dialog.ShowModal()==WINDOW_ID_BUTTON_SAVE);
	if (result)
	{
		newFilename=browserPanel->GetCurrentSelection();
		selectedSize=browserPanel->selectedSize;
		if (selectedSize>=0)
		{
			// Selected a file - get directory part
			textEdit.SetValue(browserPanel->CurrentPath());
		}
		else
		{
			// Selected directory - just use selection
			textEdit.SetValue(newFilename);
		}
	}
	else
	{
	}
	

	/*
	newDirectory=wxDirSelector(message,value);
	if (newDirectory.Length()>0)
	{
		textEdit.SetValue(newDirectory);
	}
	*/
	
}

void RemoteDirectoryConfigItemGUI::SetValue(wxString &value)
{
	textEdit.SetValue(value);
}

