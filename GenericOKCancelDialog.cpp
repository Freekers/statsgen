// Statsgen Includes
#include <wx/wx.h>
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericOKCancelDialog, wxDialog)
		EVT_SIZE(GenericOKCancelDialog::OnResize)
		EVT_BUTTON(WINDOW_ID_BUTTON_SAVE,GenericOKCancelDialog::OnSave)
		EVT_BUTTON(WINDOW_ID_BUTTON_QUIT,GenericOKCancelDialog::OnQuit)
END_EVENT_TABLE()

GenericOKCancelDialog::GenericOKCancelDialog(wxWindow *parent, 
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
}

void GenericOKCancelDialog::OnQuit(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void GenericOKCancelDialog::OnSave(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

GenericOKCancelDialog::~GenericOKCancelDialog()
{
}

void GenericOKCancelDialog::SetPanel(wxPanel *configPanelIn)
{
	configPanel=configPanelIn;
}

void GenericOKCancelDialog::CreateDialog()
{
	wxString	label="Generic Configuration";
	wxString	defaultValue="";
	wxString	configKey;
	wxSizeEvent	event;
	wxPanel		*configPanel;

	wxPoint		configItemsPosition=wxDefaultPosition;
	wxSize		configItemsSize=wxDefaultSize;
	STATSGEN_DEBUG_FUNCTION_START("GenericOKCancelDialog","CreateDialog")


	saveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T(WINDOW_ID_BUTTON_SAVE_TEXT),
					wxDefaultPosition);
	quitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);



	OnResize(event);
	STATSGEN_DEBUG_FUNCTION_END
}

bool GenericOKCancelDialog::DisplayDialog(wxPanel *panel)
{
	int			dialogRetVal;
	bool			retVal;

	// Called when we want to pop the dialog box
	// into existance for the first time
	SetPanel(panel);

	// First we want to create all the items in the dialog box
	CreateDialog();

	// Now we can resize every item in the dialog to fit nicely

	// Then we pop it into existance
	dialogRetVal=ShowModal();

	// Now we do what is necessary dependent on the return code
	switch (dialogRetVal)
	{
		case WINDOW_ID_BUTTON_SAVE:
			retVal=true;
			break;
		case WINDOW_ID_BUTTON_QUIT:
		default:
			retVal=false;
			break;
	}

	return (retVal);
}

void GenericOKCancelDialog::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	int		quitWidth;
	int		quitHeight;
	int		saveWidth;
	int		saveHeight;
	int		panelWidth;
	int		panelHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		yPosition;

	STATSGEN_DEBUG_FUNCTION_START("GenericOKCancelDialog","OnResize")

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

	panelHeight=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	panelWidth=dialogWidth;

	// Panel
	yPosition=0;
	configPanel->SetSize(0,yPosition,panelWidth,panelHeight);
	// Save button
	itemPosition.x=BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	saveButton.SetPosition(itemPosition);

	// Quit button
	itemPosition.x=saveWidth+BUTTON_WIDTH_GAP+BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-quitHeight-DIALOG_BOTTOM_BORDER_SIZE;
	quitButton.SetPosition(itemPosition);

	STATSGEN_DEBUG_FUNCTION_END
}

