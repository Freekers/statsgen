// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/filename.h>

// Statsgen includes
#include "QuakeWarsLogFile.h"
#include "QuakeWarsLogFileReader.h"
#include "GlobalStatistics.h"
#include "Progress.h"
#include "Server.h"

QuakeWarsLogFile::QuakeWarsLogFile() : LogFile()
{
	STATSGEN_DEBUG_FUNCTION_START("QuakeWarsLogFile","Constructor")
//	logFileReader=NULL;
//	serverType="";
	STATSGEN_DEBUG_FUNCTION_END
}

long QuakeWarsLogFile::Size()
{
	int			fileCount;
	int			fileIndex;
	wxString	filename;
	long		totalFileSize;

	totalFileSize=0;
	fileCount=filenames.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filename=filenames.Item(fileIndex);
		wxFile	logfile(filename);
		totalFileSize+=logfile.Length();
	}
	fileCount=secondaryFilenames.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filename=secondaryFilenames.Item(fileIndex);
		wxFile	logfile(filename);
		totalFileSize+=logfile.Length();
	}

	return (totalFileSize);

}

bool QuakeWarsLogFile::Initiate(wxString &serverTypeIn, 
								wxArrayString &filenameIn,
								wxArrayString &secondaryFilenameIn
								)
{
	bool retVal=true;

	if (logFileReader!=NULL)
	{
		delete(logFileReader);
		logFileReader=NULL;
	}
	serverType=serverTypeIn;
	filenames=filenameIn;
	secondaryFilenames=secondaryFilenameIn;

	// here we create the LogFileReader appropriate to this logfile
	// which is dependent on the serverType - currently only
	// have Quake Wars
	logFileReader=(LogFileReader *)new QuakeWarsLogFileReader(filenames,secondaryFilenames,serverType);
	return (retVal);
}

QuakeWarsLogFile::~QuakeWarsLogFile()
{
	STATSGEN_DEBUG_FUNCTION_START("QuakeWarsLogFile","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

