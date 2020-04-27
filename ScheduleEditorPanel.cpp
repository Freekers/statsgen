// Statsgen Includes
#include "ScheduleEditorPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(ScheduleEditorPanel, wxPanel)
		EVT_SIZE(ScheduleEditorPanel::OnResize)
END_EVENT_TABLE()

ScheduleEditorPanel::ScheduleEditorPanel(wxString &valueIn,
					bool intervalWithBaseIn)
{
	value=valueIn;
	intervalWithBase=intervalWithBaseIn;
	baseLabel=NULL;
	baseHours=NULL;
	baseMinutes=NULL;
	baseSeconds=NULL;
	intervalLabel=NULL;
	intervalHours=NULL;
	intervalMinutes=NULL;
	intervalSeconds=NULL;
}

ScheduleEditorPanel::~ScheduleEditorPanel()
{
}

void ScheduleEditorPanel::SplitValue(
				wxString &baseHourString,
				wxString &baseMinuteString,
				wxString &baseSecondString,
				wxString &intervalHourString,
				wxString &intervalMinuteString,
				wxString &intervalSecondString)
{
	wxString	baseScheduleComponent;
	wxString	intervalScheduleComponent;
	wxString	timeComponent;
	int			hours;
	int			minutes;
	int			seconds;
	int			totalSeconds;

	baseHourString="";
	baseMinuteString="";
	baseSecondString="";
	intervalHourString="";
	intervalMinuteString="";
	intervalSecondString="";
	if (intervalWithBase)
	{
		baseScheduleComponent=value.BeforeFirst(',');
		intervalScheduleComponent=value.AfterFirst(',');
		timeComponent=baseScheduleComponent.BeforeFirst(':');
		hours=atoi(STRING_TO_CHAR(timeComponent));
		baseScheduleComponent=baseScheduleComponent.AfterFirst(':');
		timeComponent=baseScheduleComponent.BeforeFirst(':');
		minutes=atoi(STRING_TO_CHAR(timeComponent));
		baseScheduleComponent=baseScheduleComponent.AfterFirst(':');
		seconds=atoi(STRING_TO_CHAR(baseScheduleComponent));

		baseHourString.Printf("%02d",hours);
		baseMinuteString.Printf("%02d",minutes);
		baseSecondString.Printf("%02d",seconds);
	}
	else
	{
		intervalScheduleComponent=value;
	}
	
	totalSeconds=atoi(STRING_TO_CHAR(intervalScheduleComponent));
	hours=totalSeconds/3600;
	totalSeconds-=(hours*3600);
	minutes=totalSeconds/60;
	seconds=totalSeconds % 60;
	intervalHourString.Printf("%02d",hours);
	intervalMinuteString.Printf("%02d",minutes);
	intervalSecondString.Printf("%02d",seconds);
}

void ScheduleEditorPanel::CreatePanel()
{
	wxString	labelText;
	wxString	baseHourStr;
	wxString	baseMinuteStr;
	wxString	baseSecondStr;
	wxString	intervalHourStr;
	wxString	intervalMinuteStr;
	wxString	intervalSecondStr;

	SplitValue(baseHourStr,
				baseMinuteStr,
				baseSecondStr,
				intervalHourStr,
				intervalMinuteStr,
				intervalSecondStr);

	if (intervalWithBase)
	{
		labelText="Base Time";
		baseLabel=new wxStaticText();
		baseLabel->Create(this,-1,labelText,wxDefaultPosition);

		baseHours=new wxTextCtrl();
		baseHours->Create(this,-1,baseHourStr);

		baseMinutes=new wxTextCtrl();
		baseMinutes->Create(this,-1,baseMinuteStr);

		baseSeconds=new wxTextCtrl();
		baseSeconds->Create(this,-1,baseSecondStr);
	}

	labelText="Interval";
	intervalLabel=new wxStaticText();
	intervalLabel->Create(this,-1,labelText,wxDefaultPosition);

	intervalHours=new wxTextCtrl();
	intervalHours->Create(this,-1,intervalHourStr);

	intervalMinutes=new wxTextCtrl();
	intervalMinutes->Create(this,-1,intervalMinuteStr);

	intervalSeconds=new wxTextCtrl();
	intervalSeconds->Create(this,-1,intervalSecondStr);
}

void ScheduleEditorPanel::SetValue()
{
	wxString	baseTimeComponent;
	wxString	intervalTimeComponent;

	if (intervalWithBase)
	{
		baseTimeComponent.Printf("%02d:%02d:%02d,",
			atoi(STRING_TO_CHAR(baseHours->GetValue())),
			atoi(STRING_TO_CHAR(baseMinutes->GetValue())),
			atoi(STRING_TO_CHAR(baseSeconds->GetValue())));
	}
	else
	{
		baseTimeComponent="";
	}
	intervalTimeComponent.Printf("%d",
			(atoi(STRING_TO_CHAR(intervalHours->GetValue()))*3600)+
			(atoi(STRING_TO_CHAR(intervalMinutes->GetValue()))*60)+
			atoi(STRING_TO_CHAR(intervalSeconds->GetValue())));

	value=baseTimeComponent+intervalTimeComponent;
}

wxString ScheduleEditorPanel::GetValue()
{
	SetValue();
	return (value);
}

void ScheduleEditorPanel::ResizeTime(int startX,int startY,
				wxStaticText *label,
				wxTextCtrl *hour,
				wxTextCtrl *minute,
				wxTextCtrl *second,
				int *width,
				int *height)
{
	wxSize		itemSize;
	int			hourX;
	int			minuteX;
	int			secondX;
	int			hourWidth;
	int			minuteWidth;
	int			secondWidth;
	int			labelX;
	int			labelWidth;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("ScheduleEditorPanel","ResizeTime")
	itemSize=label->GetSize();
	labelWidth=itemSize.GetWidth();
	labelWidth=10*FONT_CHAR_WIDTH;
	// resize the hour to give it a basic size
	//hour->SetSize(0,0,1,1);
	
	itemSize=hour->GetSize();
	*height=itemSize.GetHeight();

	hourWidth=FONT_CHAR_WIDTH*2+FONT_TEXT_CTRL_GAP;
	minuteWidth=FONT_CHAR_WIDTH*2+FONT_TEXT_CTRL_GAP;
	secondWidth=FONT_CHAR_WIDTH*2+FONT_TEXT_CTRL_GAP;
	labelX=startX;
	hourX=labelX+labelWidth+FONT_CHAR_WIDTH;
	minuteX=hourX+hourWidth+FONT_CHAR_WIDTH;
	secondX=minuteX+minuteWidth+FONT_CHAR_WIDTH;

	label->SetSize(labelX,startY,labelWidth,*height);
	hour->SetSize(hourX,startY,hourWidth,*height);
	minute->SetSize(minuteX,startY,minuteWidth,*height);
	second->SetSize(secondX,startY,secondWidth,*height);

	*width=secondX+secondWidth;
	STATSGEN_DEBUG_CODE(msg.Printf("startX=%d,%d label=%d,%d hour=%d,%d minute=%d,%d second=%d,%d",
				startX,*height,
				labelX,labelWidth,
				hourX,hourWidth,
				minuteX,minuteWidth,
				secondX,secondWidth);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	STATSGEN_DEBUG_FUNCTION_END
}

void ScheduleEditorPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	wxSize		itemSize;
	int			panelWidth;
	int			panelHeight;
	int			baseWidth=0;
	int			baseHeight=0;
	int			intervalWidth=0;
	int			intervalHeight=0;
	int			startX;
	int			startY;

	STATSGEN_DEBUG_FUNCTION_START("ScheduleEditorPanel","OnResize")

	if (intervalHours==NULL)
	{
		CreatePanel();
	}
	startX=0;
	startY=0;
	if (intervalWithBase)
	{
		ResizeTime(startX,startY,
				baseLabel,baseHours,baseMinutes,baseSeconds,
				&baseWidth,&baseHeight);
	}
	startX=0;
	startY=baseHeight;
	ResizeTime(startX,startY,
				intervalLabel,intervalHours,intervalMinutes,intervalSeconds,
				&intervalWidth,&intervalHeight);
	itemSize=GetSize();
	if (baseWidth>intervalWidth)
	{
		panelWidth=baseWidth;
	}
	else
	{
		panelWidth=intervalWidth;
	}
	panelHeight=baseHeight+intervalHeight;
	SetSize(0,0,panelWidth,panelHeight);
	STATSGEN_DEBUG_FUNCTION_END
}
