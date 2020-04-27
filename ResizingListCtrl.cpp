#include <wx/wx.h>
#include <wx/listctrl.h>

#include "ResizingListCtrl.h"
#include "ErrorData.h"
BEGIN_EVENT_TABLE(ResizingListCtrl, wxListCtrl)
	EVT_SIZE(ResizingListCtrl::OnSize)
END_EVENT_TABLE()

ResizingListCtrl::ResizingListCtrl(wxWindow *parent,int id,wxPoint pos,wxSize size,int style): wxListCtrl(parent,id,pos,size,style)
{
	STATSGEN_DEBUG_FUNCTION_START("ResizingListCtrl","ResizingListCtrl")
	mResizing= false;
	STATSGEN_DEBUG_FUNCTION_END
}

void ResizingListCtrl::OnSize(wxSizeEvent &event)
{
	// OK - for some reason - I cannot get list control to repaint it's parent
	// it leaves ugly trail lines when a window is sized to be bigger than
	// the listctrl needs
	// this object is a compromise - it at least forces a resize of the columns
	// which in turn forces the listctrl to refresh to fill horizontally
	// ok
	// but vertically the bit of the parent that is not filled by the listctrl
	// is not redrawn
	STATSGEN_DEBUG_FUNCTION_START("ResizingListCtrl","OnSize")
	if (mResizing)
	{
		mResizing = false;
	}
	else
	{
		wxSize size = event.GetSize();
		wxString msg;
		msg.Printf("size [%d,%d]",size.GetWidth(),size.GetHeight());
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		//mResizing = true;
		int columnIndex;
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Setting column widths");
		for (columnIndex = 0;columnIndex<GetColumnCount();columnIndex++)
		{
			SetColumnWidth(columnIndex,wxLIST_AUTOSIZE_USEHEADER);
		}
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Set column widths");
		//PostSizeEventToParent();
	}
	STATSGEN_DEBUG_FUNCTION_END
}
