#ifndef __WEAPONGROUPDIALOG
#define __WEAPONGROUPDIALOG

#include <wx/wx.h>
#include <wx/dialog.h>

// Statsgen Includes
#include "WindowIDs.h"

class WeaponGroupDialog : public wxDialog
{
	public:
		WeaponGroupDialog(wxWindow *parent, 
				wxWindowID id,
				wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~WeaponGroupDialog();
		void OnWeaponSelected(wxCommandEvent &event);
		void OnWeaponGroupSelected(wxCommandEvent &event);
		void OnSave(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnNew(wxCommandEvent &event);
		void OnDelete(wxCommandEvent &event);
		void CreateScreen();
		void PopulateGroupsList();
		void PopulateKeysList();
		void CheckKeysList();
		void DisplayDialog();
		void WriteCurrentWeaponGroup();

	protected:

	private:
		wxListBox			*weaponGroupsList;				
		wxCheckListBox		*weaponKeysList;				
		wxButton			*newButton;
		wxButton			*deleteButton;
		wxButton			*saveButton;
		wxButton			*quitButton;
		wxString			currentWeaponGroup;
		wxArrayString		weaponKeys;
		wxArrayString		weaponNames;
		wxBoxSizer			*mainSizer;
		wxBoxSizer			*buttonSizer;
		wxBoxSizer			*displaySizer;		

		DECLARE_EVENT_TABLE()
};

#endif
