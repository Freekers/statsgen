// wxWindows includes
#include <wx/string.h>

// Statsgen includes
#include "ScheduleItem.h"

ScheduleItem::ScheduleItem()
{
	whenToRun=time(&whenToRun);
	disabled=true;
	startNow=false;
}

ScheduleItem::~ScheduleItem()
{
}

bool ScheduleItem::TimeToRun(long *timeRemaining)
{
	time_t	currentTime;
	bool	timeToRun;

	if (disabled)
	{
		timeToRun=false;
		*timeRemaining=0;
	}
	else
	{
		currentTime=time(&currentTime);
		*timeRemaining=(whenToRun-currentTime);
		timeToRun=((*timeRemaining)<0);
	}

	if (startNow)
	{
		timeToRun=true;
	}
	startNow=false;
	return (timeToRun);
}

void ScheduleItem::IntervalStart(wxString &interval)
{
	time_t		timeNow;
	int			value;

	value=atoi(interval.GetData());
	if (value==0)
	{
		disabled=true;
	}
	else
	{
		disabled=false;
		timeNow=time(&timeNow);
		whenToRun=timeNow+value;
	}
}

void ScheduleItem::StartNow()
{
	startNow=true;
}

void ScheduleItem::ScheduledStart(wxString &base,wxString &interval)
{
	time_t		timeNow;
	long		intervalSeconds;
	wxDateTime	currentTime=wxDateTime::Now();
	wxDateTime	baseTime;
	wxTimeSpan	intervalTimeSpan;

	intervalSeconds=atol(interval.GetData());
	if (intervalSeconds==0)
	{
		disabled=true;
	}
	else
	{
		disabled=false;
		intervalTimeSpan=wxTimeSpan::Seconds(intervalSeconds);
		timeNow=time(&timeNow);

		baseTime=currentTime;
		baseTime.ParseTime(base.GetData());
		while (baseTime.IsEarlierThan(currentTime))
		{
			baseTime.Add(intervalTimeSpan);
		}

		whenToRun=baseTime.GetTicks();
	}
}

