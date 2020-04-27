#ifndef __SCHEDULEITEM
#define __SCHEDULEITEM

#include <time.h>
// wxWindows includes
#include <wx/datetime.h>

#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class ScheduleItem
{
	public:
		ScheduleItem();
		virtual ~ScheduleItem();
		bool TimeToRun(long *timeRemaining);
		void IntervalStart(wxString &interval);
		void ScheduledStart(wxString &base,wxString &interval);
		void StartNow();
		bool		disabled;
		
	private:
		time_t		whenToRun;
		bool		startNow;

};


#endif
