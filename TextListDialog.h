#ifndef __TEXTLISTDIALOG
#define __TEXTLISTDIALOG

#include <wx/wx.h>
#include <wx/dialog.h>

// Statsgen Includes
#include "GenericOKCancelDialog.h"
#include "TextListEditorPanel.h"
#include "WindowIDs.h"

class TextListDialog : public GenericOKCancelDialog
{
	public:
		TextListDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				const char *group
				);

		virtual ~TextListDialog();

		virtual void OnSave(wxCommandEvent& event);
		virtual void OnQuit(wxCommandEvent& event);
		virtual void OnNew(wxCommandEvent& event);
		virtual void OnResize(wxSizeEvent &event);
		virtual bool DisplayDialog();
		virtual void SortOn(bool value);
		virtual void SingleWords(bool value);
		wxButton	newButton;

	protected:

	private:
		TextListEditorPanel	*panel;
		wxString		group;
		bool			singleWords;
		DECLARE_EVENT_TABLE()
};

#endif
