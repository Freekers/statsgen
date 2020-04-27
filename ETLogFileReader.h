#ifndef __ETLOGFILEREADER
#define __ETLOGFILEREADER

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "ErrorData.h"
#include "Round.h"
#include "MOHAALogFileReader.h"
#include "DynamicArrays.h"
#include "Round.h"

class ETLogFileReader:public MOHAALogFileReader
{
	public:
		virtual bool IsWorldPlayer(wxString &id,wxString &team,wxString &name);
		ETLogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn);
		virtual ~ETLogFileReader();
		virtual bool DecodeTeamLoss(wxString &line,DecodedTeamLoss &teamLoss);
		virtual bool DecodeTeamWin(wxString &line,DecodedTeamWin &teamWin);
		virtual bool DecodePlayerJoin(wxString &line,DecodedPlayerJoin &decode);
		virtual bool DecodeRoundStart(wxString &line,
						DecodedRoundStart &decode,
						Round *round);

	protected:

		virtual bool StripTimeFromLine(wxString &lineRead,wxDateTime &lineTime);
		virtual bool IsRoundStart(wxString &line);
		virtual bool IsRoundEnd(wxString &line);

	private:
};

#endif
