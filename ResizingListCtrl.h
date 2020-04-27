#ifndef __RESIZINGLISTCTRL
#define __RESIZINGLISTCTRL
#include <wx/wx.h>
#include <wx/listctrl.h>
class  ResizingListCtrl : public wxListCtrl
{
	public:
		ResizingListCtrl(wxWindow *parent,int id,wxPoint pos,wxSize size,int style);
		void OnSize(wxSizeEvent &event);


	private:
		bool mResizing;
		DECLARE_EVENT_TABLE()
};
#endif
