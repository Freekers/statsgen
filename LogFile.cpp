// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/filename.h>

// Statsgen includes
#include "LogFile.h"
#include "LogFileReader.h"
#include "COD5LogFileReader.h"
#include "MOHAALogFileReader.h"
#include "ETLogFileReader.h"
#include "GlobalStatistics.h"
#include "Progress.h"
#include "Server.h"

LogFile::LogFile()
{
	STATSGEN_DEBUG_FUNCTION_START("LogFile","Constructor")
	logFileReader=NULL;
	serverType="";
	STATSGEN_DEBUG_FUNCTION_END
}

bool LogFile::Process(Server *server, int serverIndex,long maxLogfileSize)
{
	bool		retVal=true;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("LogFile","Process")
	if (logFileReader==NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Log file reader is null")
	}
	else
	{
		msg="Processing";
		progress->SetStatus(msg);
		logFileReader->ProcessLogFile(server,serverIndex,maxLogfileSize);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"ran Log file reader")
	}

	return (retVal);
	STATSGEN_DEBUG_FUNCTION_END
}

long LogFile::Size()
{
	int			fileCount;
	int			fileIndex;
	wxString	filename;
	long		totalFileSize;

	fileCount=filenames.GetCount();
	totalFileSize=0;
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filename=filenames.Item(fileIndex);
		wxFile	logfile(filename);
		totalFileSize+=logfile.Length();
	}

	return (totalFileSize);
}

void LogFile::FakeTeamIDs(bool value)
{
	if (logFileReader!=NULL)
	{
		logFileReader->FakeTeamIDs(value);
	}
}

bool LogFile::Initiate(wxString &serverTypeIn, wxArrayString &filenameIn)
{
	bool retVal=true;

	if (logFileReader!=NULL)
	{
		delete(logFileReader);
		logFileReader=NULL;
	}
	serverType=serverTypeIn;
	filenames=filenameIn;

	// here we create the LogFileReader appropriate to this logfile
	// which is dependent on the serverType
	// for now stick with the only one we have which is COD1/2
	if (serverType.Cmp("COD1")==0)
	{
		logFileReader=new LogFileReader(filenames,serverType);
	}
	if (serverType.Cmp("COD2")==0)
	{
		logFileReader=new LogFileReader(filenames,serverType);
	}
	if (serverType.Cmp("COD4")==0)
	{
		logFileReader=new LogFileReader(filenames,serverType);
	}
	if (serverType.Cmp("COD5")==0)
	{
		logFileReader=new COD5LogFileReader(filenames,serverType);
	}
	if (serverType.Cmp("MOHAA")==0)
	{
		logFileReader=new MOHAALogFileReader(filenames,serverType);
	}
	if (serverType.Cmp("WOLFENSTEIN")==0)
	{
		logFileReader=new ETLogFileReader(filenames,serverType);
	}
	
	return (retVal);
}

LogFile::~LogFile()
{
	STATSGEN_DEBUG_FUNCTION_START("LogFile","Destructor")
	if (logFileReader!=NULL)
	{
		delete(logFileReader);
		logFileReader=NULL;
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void LogFile::StartFromBeginning()
{
	STATSGEN_DEBUG_FUNCTION_START("LogFile","StartFromBeginning")
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"hello")
	if (logFileReader==NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Log file reader is null")
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"About to call StartFromBeginning")
		logFileReader->StartFromBeginning();
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Called StartFromBeginning")
	}
	STATSGEN_DEBUG_FUNCTION_END
}
void LogFile::RetrieveLastRoundPositions(long *lastRoundEndedAt,
										long *secondaryLastRoundEndedAt)
{
	*lastRoundEndedAt=0;
	*secondaryLastRoundEndedAt=0;
	if (logFileReader!=NULL)
	{
		logFileReader->RetrieveLastRoundPositions(lastRoundEndedAt,
												secondaryLastRoundEndedAt);
	}
}
void LogFile::SetLastRoundPositions(long lastRoundEndedAt,
										long secondaryLastRoundEndedAt)
{
	if (logFileReader!=NULL)
	{
		logFileReader->SetLastRoundPositions(lastRoundEndedAt,
												secondaryLastRoundEndedAt);
	}
}
