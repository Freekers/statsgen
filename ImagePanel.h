#ifndef __IMAGEPANEL_H
#define __IMAGEPANEL_H
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/stream.h>
#include <wx/wfstream.h>

class ImagePanel : public wxPanel
{
	public:
		ImagePanel(wxWindow *parent,
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		ImagePanel(wxWindow *parent,
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxImage &imageIn);
		void SetImage(wxImage &imageIn);
		virtual ~ImagePanel();
		void OnPaint(wxPaintEvent &event);
		void OnResize(wxSizeEvent &event);
		void UpdateImageScale(wxDC &dc);
		void Clear();

		void Render(wxDC &dc);
	private:
		wxImage			mImage;
		float			mCurrentScale;
		DECLARE_EVENT_TABLE()

};

#endif
