// wx includes
#include "wx/log.h"

// Statsgen Includes
#include "StatsgenLog.h"
#include "Progress.h"

void StatsgenLog::DoLog(wxLogLevel level,
			const char *msg,
			time_t timestamp)
{
	int			newSeverity;
	wxString	messageString;

	switch (level)
	{
		case wxLOG_FatalError:
		case wxLOG_Error:
			newSeverity=SeverityError;
			break;
		case wxLOG_Message:
		case wxLOG_Status:
		case wxLOG_Info:
			newSeverity=SeverityOK;
			break;
		case wxLOG_Warning:
			newSeverity=SeverityCaution;
			break;
		default:
			newSeverity=SeverityCaution;
			break;
	}

	messageString=msg;

	progress->LogError(messageString,newSeverity);
}

