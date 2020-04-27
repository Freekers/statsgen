#ifndef __STATSGENLOG
#define __STATSGENLOG

#include <wx/wx.h>
#include <wx/log.h>

// Statsgen Includes

class StatsgenLog : public wxLog
{
	public:
	//	virtual void DoLog(wxLogLevel level,
	//				const char *msg,
	//				time_t timestamp);
		virtual void DoLogTextAtLevel(wxLogLevel level,
					const wxString &message);
		void LogMessages(wxLogLevel level,wxString message);
};

#endif
