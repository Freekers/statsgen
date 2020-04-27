#ifndef __SCHEDULEDIALOG
#define __SCHEDULEDIALOG

#include <wx/wx.h>
#include <wx/dialog.h>

// Statsgen Includes
#include "GenericOKCancelDialog.h"
#include "ScheduleEditorPanel.h"
#include "WindowIDs.h"

class ScheduleDialog : public GenericOKCancelDialog
{
	public:
		ScheduleDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				wxString &value,
				bool intervalWithBase,
				wxPoint &pos,
				wxSize &size,
				long style,
				const wxString &name);

		virtual ~ScheduleDialog();

		virtual void OnSave(wxCommandEvent& event);
		virtual void OnQuit(wxCommandEvent& event);
		virtual bool DisplayDialog();
		virtual wxString GetValue();

	protected:

	private:
		ScheduleEditorPanel	*panel;
};

#endif
