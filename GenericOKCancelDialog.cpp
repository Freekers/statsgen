// Statsgen Includes
#include <wx/wx.h>
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericOKCancelDialog, wxDialog)
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
	mConfigPanel=NULL;
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
	mConfigPanel =configPanelIn;
}

void GenericOKCancelDialog::CreateDialog()
{
	wxString	label="Generic Configuration";
	wxString	defaultValue="";
	wxString	configKey;
	wxSizeEvent	event;

	wxPoint		configItemsPosition=wxDefaultPosition;
	wxSize		configItemsSize=wxDefaultSize;
	STATSGEN_DEBUG_FUNCTION_START("GenericOKCancelDialog","CreateDialog")


	mSaveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T(WINDOW_ID_BUTTON_SAVE_TEXT),
					wxDefaultPosition);
	mQuitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);



	STATSGEN_DEBUG_FUNCTION_END
}

bool GenericOKCancelDialog::DisplayDialog(wxPanel *panel)
{
	int			dialogRetVal;
	bool		retVal;

	// Called when we want to pop the dialog box
	// into existance for the first time
	SetPanel(panel);

	// First we want to create all the items in the dialog box
	CreateDialog();

	mMainSizer = new wxBoxSizer(wxVERTICAL);

	mControlsSizer = new wxBoxSizer(wxHORIZONTAL);
	ControlsSizerPre();
	mControlsSizer->Add(&mSaveButton);
	mControlsSizer->Add(&mQuitButton);
	ControlsSizerPost();

	mMainSizer->Add(mConfigPanel,1,wxEXPAND|wxALL);
	mMainSizer->Add(mControlsSizer);

	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
	Layout();
	Fit();
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

void GenericOKCancelDialog::ControlsSizerPre()
{
}

void GenericOKCancelDialog::ControlsSizerPost()
{
}
