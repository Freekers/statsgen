// Statsgen Includes
#include "GenericConfigGUI.h"
#include "GenericConfigPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericConfigGUI, wxDialog)
		EVT_SIZE(GenericConfigGUI::OnResize)
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
	firstTimeResize=false;
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



	OnResize(event);
}

void GenericConfigGUI::DisplayDialog()
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

void GenericConfigGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	int		quitWidth;
	int		quitHeight;
	int		saveWidth;
	int		saveHeight;
	int		configPanelWidth;
	int		configPanelHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		yPosition;

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
	yPosition=0;
	// Config Panels resize themselves to fit
	if (configPanel!=NULL)
	{
		itemSize=configPanel->GetSize();
		if (firstTimeResize)
		{
			configPanelHeight=itemSize.GetHeight();
			dialogHeight=configPanelHeight+
						saveHeight+
						DIALOG_BOTTOM_BORDER_SIZE;
		}
		configPanel->SetSize(0,yPosition,configPanelWidth,
										configPanelHeight);
		itemSize=configPanel->GetSize();
		if (firstTimeResize)
		{
			configPanelHeight=itemSize.GetHeight();
			dialogHeight=configPanelHeight+
						saveHeight+
						DIALOG_BOTTOM_BORDER_SIZE;
		}
	}
	// Save button
	itemPosition.x=BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	saveButton.SetPosition(itemPosition);

	// Quit button
	itemPosition.x=saveWidth+BUTTON_WIDTH_GAP+BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-quitHeight-DIALOG_BOTTOM_BORDER_SIZE;
	quitButton.SetPosition(itemPosition);

	if (firstTimeResize)
	{
		SetSize(dialogWidth,dialogHeight);
	}
	firstTimeResize=false;
}

