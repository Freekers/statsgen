#ifndef __QUAKEWARSLOGFILE
#define __QUAKEWARSLOGFILE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "LogFileReader.h"
#include "LogFile.h"

class LogFile;
class Server;

class QuakeWarsLogFile : public LogFile
{
	public:
		QuakeWarsLogFile();
		virtual bool Initiate(wxString &serverTypeIn,
				wxArrayString &filenameIn,
				wxArrayString &secondaryFilenameIn);
		virtual long Size();
		virtual ~QuakeWarsLogFile();

};

#endif
