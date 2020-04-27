// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>

// Statsgen Includes
#include "PriorityPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(PriorityPanel, wxPanel)
		EVT_SIZE(PriorityPanel::OnResize)
		EVT_SCROLL(PriorityPanel::OnSliderChange)
END_EVENT_TABLE()

PriorityPanel::PriorityPanel()
{
	fullRunPriority=NULL;
}

void PriorityPanel::CreateScreen()
{
	wxString	defaultValue="";
	wxSizeEvent	event;
	long		style=wxSL_HORIZONTAL|
						wxSL_LABELS |
						wxSL_BOTTOM;

	if (fullRunPriority==NULL)
	{
		fullLabel.Create((wxWindow *)this,-1,_T("Priority"));
		fullRunPriority=new wxSlider((wxWindow *)this,
									WINDOW_ID_CONFIG_FULL_PRIORITY,
									globalStatistics.ReadPriority(true),
									0,
									100,
									wxDefaultPosition,
									wxDefaultSize,
									style);
	}
	else
	{
		// Screen is already created
	}
}

void PriorityPanel::OnResize(wxSizeEvent &event)
{
	wxSize		itemSize;
	int			panelWidth;
	int			panelHeight;
	wxString	msg;
	int			fullX;
	int			fullY;
	int			sliderWidth;
	int			sliderHeight;
	int			labelWidth=60;
	int			labelHeight=10;
	int			labelOffset=0;

	// Make sure the screen has been created
	CreateScreen();

	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();
	sliderWidth=panelWidth;
	sliderHeight=panelHeight/2;
	fullX=labelWidth;
	fullY=0;
	sliderWidth-=labelWidth;
	sliderWidth-=10;
	labelOffset=(sliderHeight-labelHeight)/2;
	labelOffset=20;
	fullRunPriority->SetSize(fullX,fullY,sliderWidth,sliderHeight);
	fullLabel.SetPosition(wxPoint(0,fullY+labelOffset));
}

void PriorityPanel::OnSliderChange(wxScrollEvent &event)
{
	int			priority;
	wxString	priorityString;

	switch (event.GetId())
	{
		case WINDOW_ID_CONFIG_FULL_PRIORITY:
			priority=fullRunPriority->GetValue();
			globalStatistics.WritePriority(true,priority);
			break;
	}
}

