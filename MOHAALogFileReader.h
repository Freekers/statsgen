#ifndef __MOHAALOGFILEREADER
#define __MOHAALOGFILEREADER

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "ErrorData.h"
#include "Round.h"
#include "LogFileReader.h"
#include "DynamicArrays.h"

class MOHAALogFileReader:public LogFileReader
{
	public:
		virtual bool IsWorldPlayer(wxString &id,wxString &team,wxString &name);
		MOHAALogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn);
		virtual ~MOHAALogFileReader();
		virtual bool DecodeRoundStart(wxString &line,DecodedRoundStart &decode,Round *round);
		virtual bool DecodePlayerJoin(wxString &line,DecodedPlayerJoin &decode);
		virtual bool DecodePlayerQuit(wxString &line,DecodedPlayerQuit &decode);
		virtual bool DecodeKill(wxString &line,DecodedKill &decode);
		virtual bool DecodeTeamWin(wxString &line,DecodedTeamWin &decode);
		virtual bool DecodeTeamLoss(wxString &line,DecodedTeamLoss &decode);
		virtual bool DecodeAction(wxString &line,DecodedAction &decode);
		virtual bool DecodeSpeech(wxString &line,DecodedSpeech &decode);

	protected:
		bool DecodeClientUserinfoChanged(wxString &line,
										wxString &inRoundID,
										wxString &name,
										wxString &teamname,
										wxString &classname);
		void UpdateValue(wxString &destination,wxString source);
		wxString FindPlayerRoundInfo(wxString &name,
									wxArrayString &listnames,
									wxArrayString &listIDs,
									char *defaultValue);
		void UpdatePlayerRoundInfo(wxString &name,
									wxArrayString &listnames,
									wxArrayString &listIDs,
									wxString &id);
		void UpdateRegularExpression(char *id,
									wxString &regularExpressionString,
									wxArrayString *replacements,
									wxArrayInt *subexpressionIndex);
		void NextSubexpression(wxString &regularExpressionString,
											int *charIndex,
											wxString &idFound,
											wxString &replacementString);

		virtual bool StripTimeFromLine(wxString &lineRead,wxDateTime &lineTime);
		virtual bool IsRoundStart(wxString &line);
		virtual bool IsRoundEnd(wxString &line);

		wxArrayString	regularExpressionLines;
		wxArrayString	replacementPlayerWeapon;
		wxArrayString	replacementPlayerAmmo;
		wxArrayString	replacementTargetLocation;
		wxArrayString	replacementAction;
		wxArrayString	replacementPlayerTeam;
		wxArrayString	replacementPlayerClass;
		wxArrayString	replacementTargetTeam;
		wxArrayString	replacementTargetClass;
		wxArrayString	replacementMap;
		wxArrayString	replacementGameType;

		wxArrayString	playerTeamListNames;
		wxArrayString	playerTeamList;
		wxArrayString	playerClassListNames;
		wxArrayString	playerClassList;
		wxArrayString	playerIDListNames;
		wxArrayString	playerIDList;

		wxArrayInt		subexpressionIndexMap;
		wxArrayInt		subexpressionIndexGameType;
		wxArrayInt		subexpressionIndexPlayerName;
		wxArrayInt		subexpressionIndexPlayerGUID;
		wxArrayInt		subexpressionIndexPlayerInRoundID;
		wxArrayInt		subexpressionIndexTargetName;
		wxArrayInt		subexpressionIndexTargetGUID;
		wxArrayInt		subexpressionIndexPlayerWeapon;
		wxArrayInt		subexpressionIndexPlayerAmmo;
		wxArrayInt		subexpressionIndexTargetLocation;
		wxArrayInt		subexpressionIndexAction;
		wxArrayInt		subexpressionIndexSpeech;
		wxArrayInt		subexpressionIndexPlayerClass;
		wxArrayInt		subexpressionIndexPlayerTeam;
		wxArrayInt		subexpressionIndexTargetClass;
		wxArrayInt		subexpressionIndexTargetTeam;
		ArrayOfPointers	compiledRegularExpressions;
		wxString RetrieveRegularExpMatch(wxString &line,
										int expressionIndex,
										wxArrayInt *subexpressions,
										wxArrayString *replacements);
		bool MatchingExpression(wxString &line,
								char *idCode,
								int *expressionIndex);


	private:
};

#endif
