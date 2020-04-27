// Statsgen Includes
#include "WebsiteConfigGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(WebsiteConfigGUI, wxDialog)
		EVT_SIZE(WebsiteConfigGUI::OnResize)
		EVT_BUTTON(WINDOW_ID_BUTTON_SAVE,WebsiteConfigGUI::OnSave)
		EVT_BUTTON(WINDOW_ID_BUTTON_QUIT,WebsiteConfigGUI::OnQuit)
END_EVENT_TABLE()

WebsiteConfigGUI::WebsiteConfigGUI(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name):
			wxDialog(parent,
				id,
				title,
				pos,
				size,
				style,
				name)

{
}

void WebsiteConfigGUI::OnQuit(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void WebsiteConfigGUI::OnSave(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

WebsiteConfigGUI::~WebsiteConfigGUI()
{
}

void WebsiteConfigGUI::CreateDialog()
{
	wxString	label="Website Configuration";
	wxString	defaultValue="";
	wxString	configKey;
	wxSizeEvent	event;
	GroupedConfigItemsPanel	*configPanel;

	wxPoint		configItemsPosition=wxDefaultPosition;
	wxSize		configItemsSize=wxDefaultSize;


	saveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T(WINDOW_ID_BUTTON_SAVE_TEXT),
					wxDefaultPosition);
	quitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);

	configPanel=new GroupedConfigItemsPanel("FTP Connection Settings");
	configPanel->Create(this,
						-1,
						wxDefaultPosition,
						wxDefaultSize);

	configKey="/website/IPAddress";
	configPanel->Add("Hostname",configKey,"",-1);
	configKey="/website/FTPUsername";
	configPanel->Add("Username",configKey,"",-1);
	configKey="/website/FTPPassword";
	configPanel->Add("Password",configKey,"",-1);
	configKey="/website/FTPPort";
	configPanel->Add("Port",configKey,"21",5);
	configKey="/website/FTPEnabled";
	configPanel->Add("Enabled",configKey,"n",1);
	configKey="/website/FTPPassive";
	configPanel->Add("Passive",configKey,"y",1);

	configItems.Add((void *)configPanel);

	OnResize(event);
}

void WebsiteConfigGUI::DisplayDialog()
{
	int			dialogRetVal;

	// Called when we want to pop the dialog box
	// into existance for the first time

	// First we want to create all the items in the dialog box
	CreateDialog();

	// Now we can resize every item in the dialog to fit nicely

	// Then we pop it into existance
	dialogRetVal=ShowModal();

	// Now we do what is necessary dependent on the return code
	switch (dialogRetVal)
	{
		case WINDOW_ID_BUTTON_SAVE:
			// We have been asked to save the changes
			// just commit the config changes
			globalStatistics.configData.CommitChanges();
			break;
		case WINDOW_ID_BUTTON_QUIT:
		default:
			// We have been asked to quit without saving
			// rollback the config changes
			globalStatistics.configData.RollbackChanges();
			break;
	}
}

void WebsiteConfigGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	int		quitWidth;
	int		quitHeight;
	int		saveWidth;
	int		saveHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		configPanelHeight;
	int		configPanelWidth;
	GroupedConfigItemsPanel	*configPanel;

	itemSize=GetSize();
	dialogWidth=itemSize.GetWidth();
	dialogHeight=itemSize.GetHeight();

	// Quit and Save buttons are at the bottom of the screen
	itemSize=quitButton.GetSize();
	quitWidth=itemSize.GetWidth();
	quitHeight=itemSize.GetHeight();

	itemSize=saveButton.GetSize();
	saveWidth=itemSize.GetWidth();
	saveHeight=itemSize.GetHeight();

	configPanelHeight=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	configPanelWidth=dialogWidth;

	// Config Panel
	configPanel=(GroupedConfigItemsPanel *)configItems.Item(0);
	configPanel->SetSize(0,0,configPanelWidth,configPanelHeight);

	// Save button
	itemPosition.x=BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	saveButton.SetPosition(itemPosition);

	// Quit button
	itemPosition.x=saveWidth+BUTTON_WIDTH_GAP+BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-quitHeight-DIALOG_BOTTOM_BORDER_SIZE;
	quitButton.SetPosition(itemPosition);

}

