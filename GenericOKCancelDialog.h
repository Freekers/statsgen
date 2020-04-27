#ifndef __GENERICOKCANCELDIALOG
#define __GENERICOKCANCELDIALOG

#include <wx/wx.h>
#include <wx/dialog.h>

// Statsgen Includes
#include "WindowIDs.h"

class GenericOKCancelDialog : public wxDialog
{
	public:
		GenericOKCancelDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~GenericOKCancelDialog();

		virtual void OnQuit(wxCommandEvent& event);
		virtual void OnSave(wxCommandEvent& event);

		virtual bool DisplayDialog(wxPanel *panel);
		void SetPanel(wxPanel *configPanel);
		virtual void ControlsSizerPre();
		virtual void ControlsSizerPost();

	protected:
		virtual void CreateDialog();

		wxPanel		*mConfigPanel;
		wxButton	mSaveButton;
		wxButton	mQuitButton;
		wxBoxSizer	*mMainSizer;
		wxBoxSizer	*mControlsSizer;
	private:
		DECLARE_EVENT_TABLE()
};

#endif
