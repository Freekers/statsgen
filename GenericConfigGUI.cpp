// Statsgen Includes
#include "GenericConfigGUI.h"
#include "GenericConfigPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericConfigGUI, wxDialog)
		EVT_BUTTON(WINDOW_ID_BUTTON_SAVE,GenericConfigGUI::OnSave)
		EVT_BUTTON(WINDOW_ID_BUTTON_QUIT,GenericConfigGUI::OnQuit)
END_EVENT_TABLE()

GenericConfigGUI::GenericConfigGUI(wxWindow *parent, 
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
	configPanel=NULL;
	mMainSizer = new wxBoxSizer(wxVERTICAL);
}

void GenericConfigGUI::OnQuit(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void GenericConfigGUI::OnSave(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

GenericConfigGUI::~GenericConfigGUI()
{
}

void GenericConfigGUI::SetConfigPanel(GenericConfigPanel *configPanelIn)
{
	configPanel=configPanelIn;
}

void GenericConfigGUI::CreateDialog()
{
	wxString	label="Generic Configuration";
	wxString	defaultValue="";
	wxString	configKey;
	wxSizeEvent	event;
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

	mControlsSizer = new wxBoxSizer(wxHORIZONTAL);
	mControlsSizer->Add(&saveButton);
	mControlsSizer->Add(&quitButton);
}

void GenericConfigGUI::DisplayDialog()
{
	int			dialogRetVal;

	// Called when we want to pop the dialog box
	// into existance for the first time

	// First we want to create all the items in the dialog box
	CreateDialog();
	ConfigureSizer();

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

void GenericConfigGUI::ConfigureSizer()
{
	mMainSizer->Clear();
	mMainSizer->Add(configPanel,1,wxEXPAND);
	mMainSizer->Add(mControlsSizer);
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
}
