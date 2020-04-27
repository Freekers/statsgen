// Statsgen Includes
#include "DropListDialog.h"
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"

DropListDialog::DropListDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name) : GenericOKCancelDialog
											(parent, 
											id,
											title,
											pos,
											size,
											style,
											name)
{
	panel=new DropListEditorPanel();
	panel->Create(this,
				-1,
				wxDefaultPosition,
				wxDefaultSize);
}

bool DropListDialog::DisplayDialog()
{
	return (GenericOKCancelDialog::DisplayDialog((wxPanel *)panel));
}

DropListDialog::~DropListDialog()
{
}

void DropListDialog::OnSave(wxCommandEvent &event)
{
	wxString			msg;

	// Write the DropList List to disk
	globalStatistics.dropList.Write();
	globalStatistics.configData.CommitChanges();
	// Do any standard Save
	GenericOKCancelDialog::OnSave(event);
}

void DropListDialog::OnQuit(wxCommandEvent &event)
{
	wxString			msg;

	// Re-read the DropList List to disk
	globalStatistics.configData.RollbackChanges();
	globalStatistics.dropList.Load();
	// Do any standard Save
	GenericOKCancelDialog::OnQuit(event);
}

