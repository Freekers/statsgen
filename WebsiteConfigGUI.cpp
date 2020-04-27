// Statsgen Includes
#include "WebsiteConfigGUI.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(WebsiteConfigGUI, wxDialog)
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
	GroupedConfigItemsPanel	*configPanel;
	wxBoxSizer	*mControlsSizer;

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

	configPanel=new GroupedConfigItemsPanel((char *)"FTP Connection Settings");
	configPanel->CreateDisplay(this,wxID_ANY);

	configKey="/website/IPAddress";
	configPanel->Add((char *)"Hostname",configKey,(char *)"",-1);
	configKey="/website/FTPUsername";
	configPanel->Add((char *)"Username",configKey,(char *)"",-1);
	configKey="/website/FTPPassword";
	configPanel->Add((char *)"Password",configKey,(char *)"",-1);
	configKey="/website/FTPPort";
	configPanel->Add((char *)"Port",configKey,(char *)"21",5);
	configKey="/website/FTPEnabled";
	configPanel->Add((char *)"Enabled",configKey,(char *)"n",1);
	configKey="/website/FTPPassive";
	configPanel->Add((char *)"Passive",configKey,(char *)"y",1);

	configItems.Add((void *)configPanel);


	mMainSizer	= new wxBoxSizer(wxVERTICAL);
	mControlsSizer	= new wxBoxSizer(wxHORIZONTAL);

	mControlsSizer->Add(&saveButton,1,wxEXPAND);
	mControlsSizer->Add(&quitButton,1,wxEXPAND);

	mMainSizer->Add(configPanel,1,wxEXPAND);
	mMainSizer->Add(mControlsSizer,1,wxEXPAND);

	ConfigureSizer();
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

void WebsiteConfigGUI::ConfigureSizer()
{
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
}
