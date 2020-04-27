#ifndef __COD5LOGFILEREADER
#define __COD5LOGFILEREADER

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "LogFileReader.h"

class COD5LogFileReader : public LogFileReader
{
	public:
		COD5LogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn);
		virtual bool DecodeAction(wxString &line,DecodedAction &decode);
		bool DecodeTeamWin(wxString &line,DecodedTeamWin &decode);
		bool DecodeTeamLoss(wxString &line,DecodedTeamLoss &decode);
		bool DecodeTeamWinLoss(const char *key,wxString &line,wxString &id,wxString &roundID, wxString &name, wxString &team);
	protected:

	protected:
		wxArrayString	nonActions;

	private:
		void AddDefaultNonAction(const char *action);

};

#endif
