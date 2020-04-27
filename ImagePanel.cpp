#include <wx/wx.h>
#include <wx/image.h>
#include "ImagePanel.h"
#include "ErrorData.h"

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::OnPaint)
    EVT_SIZE(ImagePanel::OnResize)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent,
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name):wxPanel(parent,
								id,
								pos,
								size,
								style,
								name)
{
	mCurrentScale	= 1.0;
}
ImagePanel::ImagePanel(wxWindow *parent,
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxImage &imageIn):wxPanel(parent,
								id,
								pos,
								size,
								style,
								name)
{
	STATSGEN_DEBUG_FUNCTION_START("ImagePanel","ImagePanel")
	mImage	= imageIn;
	STATSGEN_DEBUG_FUNCTION_END
}

ImagePanel::~ImagePanel()
{
}

void ImagePanel::SetImage(wxImage &imageIn)
{
	STATSGEN_DEBUG_FUNCTION_START("ImagePanel","SetImage")
	if (mImage.IsOk())
	{
		mImage.Destroy();
	}
	mImage	= imageIn.Copy();
	wxClientDC dc(this);
	Render(dc);
	STATSGEN_DEBUG_FUNCTION_END
}

void ImagePanel::UpdateImageScale(wxDC &dc)
{
	STATSGEN_DEBUG_FUNCTION_START("ImagePanel","UpdateImageScale")
	wxSize	panelSize;
	int	panelWidth;
	int	panelHeight;
	int	imageWidth;
	int	imageHeight;
	float	xscale;
	float	yscale;
	bool	growing;
	float	smallestScale;
	float	largestScale;
	wxString	msg;

	//panelSize	= GetSize();
	panelSize	= dc.GetSize();
	panelWidth	= panelSize.GetWidth();
	panelHeight	= panelSize.GetHeight();
	imageWidth	= mImage.GetWidth();
	imageHeight	= mImage.GetHeight();
	msg.Printf("Panel [%d,%d] Image [%d,%d]",panelWidth,panelHeight,imageWidth,imageHeight);
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	if ((imageWidth==0)||(imageHeight==0))
	{
		mCurrentScale = 1.0;
	}
	else
	{
		if ((imageWidth>panelWidth)||(imageHeight>panelHeight))
		{
			growing=false;
		}
		else
		{
			growing=true;
		}
		xscale=(float)panelWidth/(float)imageWidth;
		yscale=(float)panelHeight/(float)imageHeight;

		smallestScale=yscale;
		largestScale=xscale;
		if (xscale<yscale)
		{
			smallestScale=xscale;
			largestScale=yscale;
		}
		if (growing)
		{
			mCurrentScale = smallestScale;
		}
		else
		{
			mCurrentScale = smallestScale;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void ImagePanel::Render(wxDC &dc)
{
	STATSGEN_DEBUG_FUNCTION_START("ImagePanel","Render")
	int			imageWidth;
	int			imageHeight;
	int			newHeight;
	int			newWidth;
	wxImage		newImage;
	wxString	msg;
	wxImage		image;

	if (mImage.IsOk())
	{
		UpdateImageScale(dc);
		imageWidth	= mImage.GetWidth();
		imageHeight	= mImage.GetHeight();
		newHeight	= (int)((float)imageHeight * mCurrentScale);
		newWidth	= (int)((float)imageWidth * mCurrentScale);
		if ((newHeight > 0)&&(newWidth > 0))
		{
			image	= mImage.Copy();
			newImage	= image.Rescale(newWidth,newHeight);
			wxBitmap	bitmap(newImage);

			PrepareDC(dc);
			dc.Clear();
			dc.DrawBitmap(bitmap,0,0);
		}
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Image is not ok");
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void ImagePanel::OnPaint(wxPaintEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("ImagePanel","OnPaint")
	wxPaintDC	dc(this);
	Render(dc);
	STATSGEN_DEBUG_FUNCTION_END
}

void ImagePanel::OnResize(wxSizeEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("ImagePanel","OnResize")
	wxClientDC dc(this);
	Render(dc);
	Refresh();
	event.Skip();
	STATSGEN_DEBUG_FUNCTION_END
}

void ImagePanel::Clear()
{
	wxClientDC dc(this);
	PrepareDC(dc);
	dc.Clear();
}
