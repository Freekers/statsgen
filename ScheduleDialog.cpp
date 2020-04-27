// Statsgen Includes
#include "ScheduleDialog.h"
#include "PlayerDataList.h"
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"

ScheduleDialog::ScheduleDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				wxString &value,
				bool intervalWithBase,
				wxPoint &pos,
				wxSize &size,
				long style,
				const wxString &name) : GenericOKCancelDialog
											(parent, 
											id,
											title,
											pos,
											size,
											style,
											name
											)
{
	
	panel=new ScheduleEditorPanel(value,intervalWithBase);
	panel->Create(this,
				-1,
				wxDefaultPosition,
				wxDefaultSize);
}

bool ScheduleDialog::DisplayDialog()
{
	return (GenericOKCancelDialog::DisplayDialog((wxPanel *)panel));
}

ScheduleDialog::~ScheduleDialog()
{
}

void ScheduleDialog::OnSave(wxCommandEvent &event)
{
	GenericOKCancelDialog::OnSave(event);
}

void ScheduleDialog::OnQuit(wxCommandEvent &event)
{
	GenericOKCancelDialog::OnQuit(event);
}

wxString ScheduleDialog::GetValue()
{
	return (panel->GetValue());
}
