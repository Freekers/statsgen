#ifndef __BOXEDDROPDOWN
#define __BOXEDDROPDOWN

// wxWindows includes
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/combobox.h>

// Statsgen Includes

class BoxedDropDown : public wxPanel
{
	public:
		BoxedDropDown();
		void AllEntriesAllowed(bool allEntries);
		virtual ~BoxedDropDown();
		bool CreateDialog(wxWindow *parent,
					wxWindowID id,
					const wxPoint &pos=wxDefaultPosition,
					const wxSize &size=wxDefaultSize,
					long style=wxTAB_TRAVERSAL,
					const wxString &name="panel");
		wxString GetSelectedCode();
		
	protected:
		virtual wxString GetLabel()=0;
		virtual wxString AllCode()=0;
		virtual wxString AllName()=0;
		virtual wxString FindCodeFromName(wxString &code)=0;
		virtual void DropDownEntries(wxArrayString &entries,
						wxString &defaultSelection)=0;
		
	private:
		bool				allEntriesAllowed;
		wxStaticBox			staticBox;
		wxComboBox			*dropDown;

};


#endif
