#ifndef __PLAYERDATADIALOG
#define __PLAYERDATADIALOG

#include <wx/wx.h>
#include <wx/dialog.h>

// Statsgen Includes
#include "GenericOKCancelDialog.h"
#include "PlayerDataEditorPanel.h"
#include "PlayerDataList.h"
#include "WindowIDs.h"

class PlayerDataDialog : public GenericOKCancelDialog
{
	public:
		PlayerDataDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				PlayerDataList *playerDataList,
				wxString &defaultPlayerData,
				wxString &playerDataTitle);

		virtual ~PlayerDataDialog();

		virtual void OnSave(wxCommandEvent& event);
		virtual void OnQuit(wxCommandEvent& event);
		virtual bool DisplayDialog();

	protected:

	private:
		PlayerDataEditorPanel	*mPanel;
		PlayerDataList		*mPlayerDataList;
};

#endif
