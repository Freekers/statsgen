#include <wx/wx.h>
#include <wx/image.h>
#include "ImagePanel.h"

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
	painting=false;
	currentScale=1.0;
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
	image=imageIn;
}

void ImagePanel::SetImage(wxImage &imageIn)
{
	image.Destroy();
	image=imageIn.Copy();
	DrawImage();
}

void ImagePanel::Maximise()
{
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

	panelSize=GetSize();
	panelWidth=panelSize.GetWidth();
	panelHeight=panelSize.GetHeight();
	imageWidth=image.GetWidth();
	imageHeight=image.GetHeight();
	if ((imageWidth==0)||(imageHeight==0))
	{
		currentScale=1.0;
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
			currentScale=smallestScale;
		}
		else
		{
			currentScale=smallestScale;
		}
	}
}

void ImagePanel::Scale(float scaleIn)
{
	currentScale=scaleIn;
}


void ImagePanel::DrawImage()
{
	wxPaintDC	pdc(this);
	wxClientDC	wdc(this);
	wxDC		*dc;
	int			imageWidth;
	int			imageHeight;
	int			newHeight;
	int			newWidth;
	wxImage		newImage;

	if (painting)
	{
		dc=&pdc;
	}
	else
	{
		dc=&wdc;
	}
	//dc=&pdc;
	imageWidth=image.GetWidth();
	imageHeight=image.GetHeight();
	newHeight=(int)((float)imageHeight*currentScale);
	newWidth=(int)((float)imageWidth*currentScale);
	newImage=image.Rescale(newWidth,newHeight);
	wxBitmap	bitmap(newImage);

	PrepareDC(*dc);
	dc->Clear();
	dc->DrawBitmap(bitmap,0,0);
}

void ImagePanel::Clear()
{
	wxClientDC	dc(this);
	dc.Clear();
}

void ImagePanel::Proportion(int maxWidth,int maxHeight)
{
	int		imageWidth;
	int		imageHeight;
	float		widthRatio;
	float		heightRatio;
	float		newRatio;
	float		smallestScale;
	float		largestScale;
	bool		growing;
	wxString	msg;

	imageWidth=image.GetWidth();
	imageHeight=image.GetHeight();

	if ((imageWidth==0)||(imageHeight==0))
	{
		return;
	}
	widthRatio	=(float)maxWidth/(float)imageWidth;
	heightRatio	=(float)maxHeight/(float)imageHeight;
//	msg.Printf("[%d][%d] [%d][%d] [%.2f][%.2f]",maxWidth,maxHeight,imageWidth,imageHeight,widthRatio,heightRatio);wxMessageBox(msg);
	if ((imageWidth>maxWidth)||(imageHeight>maxHeight))
	{
		growing=false;
	}
	else
	{
		growing=true;
	}

	smallestScale	=widthRatio;
	largestScale	=heightRatio;
	if (widthRatio>heightRatio)
	{
		smallestScale	=heightRatio;
		largestScale	=widthRatio;
	}
	if (growing)
	{
		newRatio=smallestScale;
	}
	else
	{
		newRatio=smallestScale;
	}
//msg.Printf("newRatio [%.2f] [%.2f][%.2f]",newRatio,
//		(float)imageWidth * newRatio,
//		(float)imageHeight * newRatio);wxMessageBox(msg);
	Scale(newRatio);
	DrawImage();
}

void ImagePanel::OnPaint(wxPaintEvent &event)
{
	painting=true;
	DrawImage();
	painting=false;
}

ImagePanel::~ImagePanel()
{
}

void ImagePanel::OnResize(wxSizeEvent &event)
{
	//Maximise();
	//
	painting=false;
	DrawImage();
}
