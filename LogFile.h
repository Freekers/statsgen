#ifndef __LOGFILE
#define __LOGFILE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "LogFileReader.h"

class LogFile;
class Server;

WX_DECLARE_OBJARRAY(LogFile,ArrayOfLogFile);
class LogFile
{
	public:
		LogFile();
		void FakeTeamIDs(bool value);
		bool Process(Server *server,int serverIndex,long maxLogfileSize);
		virtual bool Initiate(wxString &serverTypeIn,
				wxArrayString &filenamesIn);
		virtual long Size();
		virtual ~LogFile();
		void StartFromBeginning();
		void SetLastRoundPositions(long lastRoundEndedAt,
								long secondaryLastRoundEndedAt);	
		void RetrieveLastRoundPositions(long *lastRoundEndedAt,
								long *secondaryLastRoundEndedAt);	

	protected:
		LogFileReader	*logFileReader;
		wxArrayString	filenames;
		wxArrayString	secondaryFilenames;
		wxString	serverType;
};

#endif
