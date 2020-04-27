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
		wxString GetValue();
		void OnPaint(wxPaintEvent &event);
		void OnResize(wxSizeEvent &event);
		void Maximise();

		void DrawImage();
		void Scale(float scaleFactor);
		void Proportion(int maxWidth,int maxHeight);
		void Clear();
	private:
		wxImage			image;
		float			currentScale;
		bool			painting;
		DECLARE_EVENT_TABLE()

};

#endif
