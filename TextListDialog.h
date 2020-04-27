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
		virtual void ControlsSizerPre();
		virtual void ControlsSizerPost();
		virtual bool DisplayDialog();
		virtual void SortOn(bool value);
		virtual void SingleWords(bool value);
		wxButton	mNewButton;

	protected:

	private:
		TextListEditorPanel	*mPanel;
		wxString		mGroup;
		bool			mSingleWords;
		DECLARE_EVENT_TABLE()
};

#endif
