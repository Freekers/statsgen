#ifndef __DROPLISTDIALOG
#define __DROPLISTDIALOG

#include <wx/wx.h>
#include <wx/dialog.h>

// Statsgen Includes
#include "GenericOKCancelDialog.h"
#include "DropListEditorPanel.h"
#include "WindowIDs.h"

class DropListDialog : public GenericOKCancelDialog
{
	public:
		DropListDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);

		virtual ~DropListDialog();

		virtual void OnSave(wxCommandEvent& event);
		virtual void OnQuit(wxCommandEvent& event);
		virtual bool DisplayDialog();

	protected:

	private:
		DropListEditorPanel	*panel;
};

#endif
