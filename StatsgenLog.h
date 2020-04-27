#ifndef __STATSGENLOG
#define __STATSGENLOG

#include <wx/wx.h>
#include <wx/log.h>

// Statsgen Includes

class StatsgenLog : public wxLog
{
	public:
		virtual void DoLog(wxLogLevel level,
					const char *msg,
					time_t timestamp);
};

#endif
