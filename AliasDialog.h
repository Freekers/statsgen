#ifndef __ALIASDIALOG
#define __ALIASDIALOG

#include <wx/wx.h>
#include <wx/dialog.h>

// Statsgen Includes
#include "GenericOKCancelDialog.h"
#include "AliasEditorPanel.h"
#include "WindowIDs.h"

class AliasDialog : public GenericOKCancelDialog
{
	public:
		AliasDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);

		virtual ~AliasDialog();

		virtual void OnResize(wxSizeEvent& event);
		virtual void OnAuto(wxCommandEvent& event);
		virtual void OnSave(wxCommandEvent& event);
		virtual void CreateDialog();
		virtual bool DisplayDialog();

	protected:

	private:
		AliasEditorPanel	*panel;
		wxButton	autoButton;
		DECLARE_EVENT_TABLE()
};

#endif
