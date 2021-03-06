#ifndef __ERRORDATA
#define __ERRORDATA

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>

// statsgen includes
#include "WindowIDs.h"
//#define DEBUG 1
#ifdef DEBUG
#define STATSGEN_DEBUG_FUNCTION_START(CLASSNAME,FUNCTIONNAME) \
char *StatsgenDebugClassName=CLASSNAME; \
char *StatsgenDebugFunctionName=FUNCTIONNAME; \
statsgenDebug.Entering(StatsgenDebugClassName,StatsgenDebugFunctionName);

#define STATSGEN_DEBUG_FUNCTION_END \
statsgenDebug.Leaving(StatsgenDebugClassName,StatsgenDebugFunctionName);

#define STATSGEN_DEBUG(LEVEL,MESSAGE) \
statsgenDebug.Debug(StatsgenDebugClassName,StatsgenDebugFunctionName,LEVEL,MESSAGE);

#define STATSGEN_DEBUG_INITIATE \
statsgenDebug.UpdateDebugConfig();

#define STATSGEN_DEBUG_UPDATE_DEBUG_LEVEL(PROGRESS_PANEL) \
statsgenDebug.UpdateCurrentDebugLevel(PROGRESS_PANEL);

#define STATSGEN_DEBUG_CODE(CODE) \
if (statsgenDebug.DebugOn()) \
{ \
	CODE \
}
#else
#define STATSGEN_DEBUG_FUNCTION_START(CLASSNAME,FUNCTIONNAME)
#define STATSGEN_DEBUG_FUNCTION_END
#define STATSGEN_DEBUG(LEVEL,MESSAGE)
#define STATSGEN_DEBUG_INITIATE
#define STATSGEN_DEBUG_UPDATE_DEBUG_LEVEL(PROGRESS_PANEL)
#define STATSGEN_DEBUG_CODE(CODE)
#endif
#define DEBUG_OFF	0
#define DEBUG_ALWAYS	1
#define DEBUG_SOMETIMES	2
#define DEBUG_RARELY	3

class StatsgenDebugObject
{
	public:
		wxString	debugFilename;
		wxArrayInt	debugMaxLevel;

		StatsgenDebugObject();
		void UpdateDebugConfig();
		void Debug(const char *classname,
			const char *functionname,
			int DebugLevel,
			wxString &message);
		void Debug(const char *classname,
			const char *functionname,
			int DebugLevel,
			const char *message);
		void Entering(const char *classname,
			const char *functionname);
		void Leaving(const char *classname,
			const char *functionname);
		void UpdateCurrentDebugLevel(int progressPanelID);
		bool DebugOn();
		bool initiateDebugFile;
		int currentDebugLevel;
};

extern StatsgenDebugObject statsgenDebug;

enum ErrorCodes
{
	ERROR_CODE_OK=0,
	ERROR_CODE_FILE_OPEN
};
	
class ErrorData
{
	public:
		ErrorData();
		void ClearError();
		ErrorCodes CurrentError();
		bool IsOK();
		void SetError(ErrorCodes error,char *description);
		void SetError(ErrorCodes error,wxString &description);

		
	private:
		ErrorCodes	errorCode;
		wxString	errorDescription;
};

#endif
