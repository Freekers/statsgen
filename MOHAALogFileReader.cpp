// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "GlobalStatistics.h"
#include "MOHAALogFileReader.h"
#include "ErrorData.h"
#include "Round.h"
#include "ConfigData.h"
#include "StaticFunctions.h"
#include "Progress.h"
#include "Server.h"
#include "Round.h"

void MOHAALogFileReader::NextSubexpression(wxString &regularExpressionString,
											int *charIndex,
											wxString &idFound,
											wxString &replacementString)
{
	// Need to scan through from charIndex onwards
	// looking for either "(.+)", "$$id=replacement$$", "$$id$$"
	// as soon as we find any of these we decode them and return
	// the values

	wxString	stringToCheck;
	int			terminatorIndex;

	idFound="";
	replacementString="";
	stringToCheck=regularExpressionString.Mid(*charIndex);
	while (stringToCheck.Length()>0)
	{
		stringToCheck=regularExpressionString.Mid(*charIndex);
		if (stringToCheck.StartsWith("(.+)"))
		{
			break;
		}
		if (stringToCheck.StartsWith("$$"))
		{
			// Skip past the initial $$
			stringToCheck=stringToCheck.Mid(2);
			// find the terminating $$
			terminatorIndex=stringToCheck.Find("$$");
			if (terminatorIndex<0)
			{
				// no terminating $$ - sod it just go to the end of the line
				terminatorIndex=stringToCheck.Length();
			}
			// strip off the terminating $$
			stringToCheck=stringToCheck.Left(terminatorIndex);
			idFound=stringToCheck.BeforeFirst('=');
			replacementString=stringToCheck.AfterFirst('=');
			break;
		}
		(*charIndex)++;
	}
}

						
void MOHAALogFileReader::UpdateRegularExpression(char *id,
												wxString &regularExpressionString,
												wxArrayString *replacements,
												wxArrayInt *subexpressionIndex)
{
	int			charIndex;
	wxString	idFound;			
	wxString	replacementString;
	int			expressionIndex;
	wxString	originalString;
	bool		foundIt;

	expressionIndex=1;
	charIndex=0;
	// scan through each expression in the regular expression
	// until we find the one we are checking for
	foundIt=false;
	while (charIndex<regularExpressionString.Length())
	{
		// get next expression
		NextSubexpression(regularExpressionString,
						&charIndex,
						idFound,
						replacementString);
		if (idFound.Cmp(id)==0)
		{
			foundIt=true;
			// Found the expression we are looking for
			// replace it with a subexpression
			if (replacementString.Length()>0)
			{
				originalString.Printf("$$%s=%s$$",
										STRING_TO_CHAR(idFound),
										STRING_TO_CHAR(replacementString));
				regularExpressionString.Replace(STRING_TO_CHAR(originalString),"");
				originalString=""; // to make it move on 1 character
			}
			else
			{
				originalString.Printf("$$%s$$",
										STRING_TO_CHAR(idFound));
				regularExpressionString.Replace(STRING_TO_CHAR(originalString),"(.+)");
				originalString="(.+)";
			}
			if (replacements!=NULL)
			{
				replacements->Add(replacementString);
			}
			if (replacementString.Length()>0)
			{
				// this is a replacement line - it is not a match
				// so add the -1 to the expression index
				subexpressionIndex->Add(-1);
			}
			else
			{
				subexpressionIndex->Add(expressionIndex);
			}
		}
		else
		{
			// Found an expression - but it is not the one we want
			if (idFound.Length()==0)
			{
				// No ID Found so originalString must be a straight subexpression
				originalString="(.+)";
			}
			else
			{
				// ID found (not appropriate to this id) - so it needs to be skipped past
				if (replacementString.Length()>0)
				{
					originalString.Printf("$$%s=%s$$",
											STRING_TO_CHAR(idFound),
											STRING_TO_CHAR(replacementString));
				}
				else
				{
					originalString.Printf("$$%s$$",
											STRING_TO_CHAR(idFound));
				}
			}
			// move onto next expression
			expressionIndex++;
		}
		// skip past the subexpression in the string
		charIndex+=(originalString.Length());
		if (foundIt)
		{
			break;
		}
	}
	if (!foundIt)
	{
		subexpressionIndex->Add(-1);
		replacementString="";
		if (replacements!=NULL)
		{
			replacements->Add(replacementString);
		}
	}
}

MOHAALogFileReader::MOHAALogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn): LogFileReader(filenameIn,serverTypeIn)
{
	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","Constructor");
	wxString	configGroup;
	int			expressionCount;
	int			expressionIndex;
	wxString	regularExpressionString;
	wxString	msg;
	wxString	dummy;
	int			dummyInt;
	wxRegEx		*regExp;

	configGroup="LINEID"+serverType;
	globalStatistics.configData.ReadList(configGroup,regularExpressionLines);
	expressionCount=regularExpressionLines.GetCount();
	for (expressionIndex=0;expressionIndex<expressionCount;expressionIndex++)
	{
		regularExpressionString=regularExpressionLines.Item(expressionIndex).AfterFirst(',');
		STATSGEN_DEBUG(DEBUG_ALWAYS,regularExpressionString);
		UpdateRegularExpression((char *)"PLAYERNAME",regularExpressionString,NULL,&subexpressionIndexPlayerName);
		UpdateRegularExpression((char *)"PLAYERGUID",regularExpressionString,NULL,&subexpressionIndexPlayerGUID);
		UpdateRegularExpression((char *)"PLAYERROUNDID",regularExpressionString,NULL,&subexpressionIndexPlayerInRoundID);
		UpdateRegularExpression((char *)"TARGETNAME",regularExpressionString,NULL,&subexpressionIndexTargetName);
		UpdateRegularExpression((char *)"TARGETGUID",regularExpressionString,NULL,&subexpressionIndexTargetGUID);
		UpdateRegularExpression((char *)"PLAYERWEAPON",regularExpressionString,&replacementPlayerWeapon,&subexpressionIndexPlayerWeapon);
		UpdateRegularExpression((char *)"PLAYERAMMO",regularExpressionString,&replacementPlayerAmmo,&subexpressionIndexPlayerAmmo);
		UpdateRegularExpression((char *)"TARGETLOCATION",regularExpressionString,&replacementTargetLocation,&subexpressionIndexTargetLocation);
		UpdateRegularExpression((char *)"ACTION",regularExpressionString,&replacementAction,&subexpressionIndexAction);
		UpdateRegularExpression((char *)"SPEECH",regularExpressionString,NULL,&subexpressionIndexSpeech);
		UpdateRegularExpression((char *)"PLAYERTEAM",regularExpressionString,&replacementPlayerTeam,&subexpressionIndexPlayerTeam);
		UpdateRegularExpression((char *)"PLAYERCLASS",regularExpressionString,&replacementPlayerClass,&subexpressionIndexPlayerClass);
		UpdateRegularExpression((char *)"TARGETTEAM",regularExpressionString,&replacementTargetTeam,&subexpressionIndexTargetTeam);
		UpdateRegularExpression((char *)"TARGETCLASS",regularExpressionString,&replacementTargetClass,&subexpressionIndexTargetClass);
		UpdateRegularExpression((char *)"MAP",regularExpressionString,&replacementMap,&subexpressionIndexMap);
		UpdateRegularExpression((char *)"GAMETYPE",regularExpressionString,&replacementGameType,&subexpressionIndexGameType);

		// by now we should have restructured the regular expression
		// need to compile it and add it to the list of compiled regular expressions
		STATSGEN_DEBUG(DEBUG_ALWAYS,regularExpressionString);
		dummyInt=subexpressionIndexPlayerName.Item(expressionIndex); dummy="";
		STATSGEN_DEBUG_CODE(msg.Printf("PLAYERNAME [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexPlayerGUID.Item(expressionIndex); dummy="";
		STATSGEN_DEBUG_CODE(msg.Printf("PLAYERGUID [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexTargetName.Item(expressionIndex); dummy="";
		STATSGEN_DEBUG_CODE(msg.Printf("TARGETNAME [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexTargetGUID.Item(expressionIndex); dummy="";
		STATSGEN_DEBUG_CODE(msg.Printf("TARGETGUID [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexPlayerWeapon.Item(expressionIndex); dummy=replacementPlayerWeapon.Item(expressionIndex); 
		STATSGEN_DEBUG_CODE(msg.Printf("PLAYERWEAPON [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexPlayerAmmo.Item(expressionIndex); dummy=replacementPlayerAmmo.Item(expressionIndex);
		STATSGEN_DEBUG_CODE(msg.Printf("PLAYERAMMO [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexTargetLocation.Item(expressionIndex);
		dummy=replacementTargetLocation.Item(expressionIndex);
		STATSGEN_DEBUG_CODE(msg.Printf("TARGETLOCATION [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexAction.Item(expressionIndex);
		dummy=replacementAction.Item(expressionIndex);
		STATSGEN_DEBUG_CODE(msg.Printf("ACTION [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		dummyInt=subexpressionIndexSpeech.Item(expressionIndex); dummy="";
		STATSGEN_DEBUG_CODE(msg.Printf("SPEECH [%d]=[%s]",dummyInt,STRING_TO_CHAR(dummy));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		regExp=new wxRegEx(regularExpressionString);
		compiledRegularExpressions.Add(regExp,wxRE_ADVANCED);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

bool MOHAALogFileReader::DecodeClientUserinfoChanged(
										wxString &line,
										wxString &inRoundID,
										wxString &name,
										wxString &teamname,
										wxString &classname)
{
	bool		retVal=false;
	wxString	remainder;
	wxString	key;
	int			tokenIndex;
	int			tokenCount;
	wxString	value;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodeClientUserinfoChanged")
	if (line.StartsWith("ClientUserinfoChanged:"))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,line)
		inRoundID="";
		name="";
		teamname="";
		classname="";
		retVal=true;
		remainder=line;
		// skip past ClientUserinfoChanged:
		remainder.Trim(true);
		remainder.Trim(false);
		remainder=line.AfterFirst(' ');
		// skip past player id
		inRoundID=remainder.BeforeFirst(' ');
		remainder=remainder.AfterFirst(' ');
		STATSGEN_DEBUG(DEBUG_RARELY,remainder)
		key=GetLineToken(remainder,(char *)"\\",1,&tokenCount);
		for (tokenIndex=1;tokenIndex<=tokenCount;tokenIndex++)
		{
			if ((tokenIndex % 2)==0)
			{
				// odd numbers are the keys
				value=GetLineToken(remainder,(char *)"\\",tokenIndex,&tokenCount);
				// We have decoded a value - the key should represent what
				// it means
				if (key.CmpNoCase("n")==0)
				{
					name=value;
				}
				else
				if (key.CmpNoCase("t")==0)
				{
					teamname=value;
				}
				else
				if (key.CmpNoCase("c1")==0)
				{
					classname=value;
				}
				else
				if (key.CmpNoCase("c")==0)
				{
					classname=value;
				}

			}
			else
			{
				// even numbers are the keys
				key=GetLineToken(remainder,(char *)"\\",tokenIndex,&tokenCount);
			}
		}

	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool MOHAALogFileReader::DecodeTeamLoss(wxString &line,DecodedTeamLoss &decode)
{
	bool		retVal=false;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodeTeamLoss");
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool MOHAALogFileReader::DecodeTeamWin(wxString &line,DecodedTeamWin &decode)
{
	bool		retVal=false;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodeTeamWin");
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool MOHAALogFileReader::IsWorldPlayer(wxString &id,wxString &team,wxString &name)
{
	bool	retVal=false;

	if ((name.Length()==0) ||
		(name.Cmp("<world>")==0))
	{
		retVal=true;
	}

	return (retVal);
}

bool MOHAALogFileReader::MatchingExpression(wxString &line,
											char *idCode,
											int *expressionIndex)
{
	int			expressionCount;
	bool		retVal=false;
	wxRegEx		*regExp;
	wxString	regExpString;
	wxString	idCodeString;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","MatchingExpression")

	expressionCount=compiledRegularExpressions.GetCount();
	for ((*expressionIndex)=0;
			(*expressionIndex)<expressionCount;
			(*expressionIndex)++)
	{
		regExp=(wxRegEx *)compiledRegularExpressions.Item((*expressionIndex));
		retVal=regExp->Matches(STRING_TO_CHAR(line));
		if (retVal)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Found matching expression");
			regExpString=regularExpressionLines.Item(*expressionIndex);
			idCodeString=regExpString.BeforeFirst(',');
			if (idCodeString.Length()>0)
			{
				*idCode=idCodeString[0];
			}
			else
			{
				*idCode=' ';
			}
			break;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}
											
wxString MOHAALogFileReader::RetrieveRegularExpMatch(
													wxString &line,
													int expressionIndex,
													wxArrayInt *subexpressions,
													wxArrayString *replacements)
{
	wxString	retVal="";
	wxRegEx		*regExp;
	int			subexpressionIndex;
	wxString	replacement;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","RetrieveRegularExpMatch")
	if (replacements!=NULL)
	{
		replacement=replacements->Item(expressionIndex);
	}
	else
	{
		replacement="";
	}
	subexpressionIndex=subexpressions->Item(expressionIndex);
	if (subexpressionIndex<0)
	{
		// subexpression not appropriate - return the replacement value
		retVal=replacement;
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Using subexpression")
		// need to retrieve the match
		regExp=(wxRegEx *)compiledRegularExpressions.Item(expressionIndex);
		retVal=regExp->GetMatch(line,subexpressionIndex);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

wxString MOHAALogFileReader::FindPlayerRoundInfo(wxString &name,
												wxArrayString &listnames,
												wxArrayString &listIDs,
												char *defaultValue)
{
	wxString	retVal=defaultValue;
	int			listCount;
	int			listIndex;
	wxString	listname;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","FindPlayerRoundInfo")
	listCount=listnames.GetCount();

	STATSGEN_DEBUG(DEBUG_RARELY,name)
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		listname=listnames.Item(listIndex);
		if (listname.Cmp(name)==0)
		{
			retVal=listIDs.Item(listIndex);
			STATSGEN_DEBUG(DEBUG_RARELY,retVal)
			break;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool MOHAALogFileReader::DecodeKill(wxString &line,DecodedKill &decode)
{
	bool		retVal=false;
	int			expressionIndex;
	char		idCode;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodeKill");
	if (MatchingExpression(line,&idCode,&expressionIndex))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"MATCHED")
		STATSGEN_DEBUG(DEBUG_RARELY,line)
		if (idCode == 'K')
		{
			decode.playerTeam="";
			decode.playerClass="";
			decode.targetTeam="";
			decode.targetClass="";
			STATSGEN_DEBUG_CODE(msg.Printf("Kill found [%s]",STRING_TO_CHAR(line));)
			STATSGEN_DEBUG(DEBUG_RARELY,msg);
			retVal=true;
			// This is a kill - wahoo
			decode.playerName=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerName,NULL);
			STATSGEN_DEBUG(DEBUG_RARELY,decode.playerName)
			decode.playerID=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerGUID,NULL);
			decode.playerClass=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerClass,&replacementPlayerClass);
			decode.playerWeapon=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerWeapon,&replacementPlayerWeapon);
			decode.playerAmmo=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerAmmo,&replacementPlayerAmmo);
			decode.targetDamage="";
			decode.targetLocation=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexTargetLocation,&replacementTargetLocation);
			decode.targetName=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexTargetName,NULL);
			decode.targetID=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexTargetGUID,NULL);
			if (decode.playerClass.Length()==0)
			{
				decode.playerClass=FindPlayerRoundInfo(decode.playerName,
														playerClassListNames,
														playerClassList,
														(char *)"Soldier");
			}
			if (decode.playerTeam.Length()==0)
			{
				decode.playerTeam=FindPlayerRoundInfo(decode.playerName,
														playerTeamListNames,
														playerTeamList,
														(char *)"PlayerTeam");
			}
			if (decode.targetTeam.Length()==0)
			{
				decode.targetTeam=FindPlayerRoundInfo(decode.targetName,
														playerTeamListNames,
														playerTeamList,
														(char *)"TargetTeam");
			}
			STATSGEN_DEBUG(DEBUG_RARELY,decode.playerClass);
			STATSGEN_DEBUG(DEBUG_RARELY,decode.playerTeam);
			STATSGEN_DEBUG(DEBUG_RARELY,decode.targetTeam);
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool MOHAALogFileReader::DecodeAction(wxString &line,DecodedAction &decode)
{
	bool		retVal=false;
	wxString	msg;
	int			expressionIndex;
	char		idCode;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodeAction");
	if (MatchingExpression(line,&idCode,&expressionIndex))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"MATCHED")
		STATSGEN_DEBUG(DEBUG_RARELY,line)
		if (idCode == 'A')
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Action found [%s]",STRING_TO_CHAR(line));)
			STATSGEN_DEBUG(DEBUG_RARELY,msg);
			retVal=true;
			// This is a action - wahoo
			decode.playerName=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerName,NULL);
			decode.playerRoundID=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerInRoundID,NULL);
			decode.playerTeam=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerTeam,&replacementPlayerTeam);
			STATSGEN_DEBUG(DEBUG_RARELY,decode.playerName)
			decode.playerID=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerGUID,NULL);
			decode.action=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexAction,&replacementAction);
			if (decode.playerName.Length()==0)
			{
				decode.playerName=FindPlayerRoundInfo(decode.playerRoundID,
														playerIDListNames,
														playerIDList,
														(char *)"");
			}
			if (decode.playerTeam.Length()==0)
			{
				decode.playerTeam=FindPlayerRoundInfo(decode.playerName,
														playerTeamListNames,
														playerTeamList,
														(char *)"PlayerTeam");
			}
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool MOHAALogFileReader::DecodeSpeech(wxString &line,DecodedSpeech &decode)
{
	bool			retVal=false;
	int				expressionIndex;
	wxString		msg;
	char			idCode;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodeSpeech");
	if (MatchingExpression(line,&idCode,&expressionIndex))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"MATCHED")
		STATSGEN_DEBUG(DEBUG_RARELY,line)
		if (idCode == 'S')
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Speech found [%s]",STRING_TO_CHAR(line));)
			STATSGEN_DEBUG(DEBUG_RARELY,msg);
			retVal=true;
			// This is a kill - wahoo
			decode.playerName=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerName,NULL);
			STATSGEN_DEBUG(DEBUG_RARELY,decode.playerName)
			decode.playerID=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerGUID,NULL);
			decode.speech=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexSpeech,NULL);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

void MOHAALogFileReader::UpdatePlayerRoundInfo(wxString &name,
								wxArrayString &listnames,
								wxArrayString &listIDs,
								wxString &id)
{
	int			listCount;
	int			listIndex;
	wxString	listname;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","UpdatePlayerRoundInfo")
	if (id.Length()==0)
	{
		return;
	}
	listCount=listnames.GetCount();
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		listname=listnames.Item(listIndex);
		if (listname.Cmp(name)==0)
		{
			listnames.RemoveAt(listIndex);
			listIDs.RemoveAt(listIndex);
			break;
		}
	}
	STATSGEN_DEBUG_CODE(msg.Printf("[%s] = [%s]",STRING_TO_CHAR(name),STRING_TO_CHAR(id));)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)
	listnames.Add(name);
	listIDs.Add(id);

	STATSGEN_DEBUG_FUNCTION_END
}

bool MOHAALogFileReader::DecodePlayerJoin(wxString &line,DecodedPlayerJoin &decode)
{
	bool		retVal=false;
	char		idCode;
	int			expressionIndex;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodePlayerJoin");
	STATSGEN_DEBUG(DEBUG_RARELY,line);
	if (MatchingExpression(line,&idCode,&expressionIndex))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"MATCHED")
		STATSGEN_DEBUG(DEBUG_RARELY,line)
		if (idCode == 'J')
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Join found [%s]",STRING_TO_CHAR(line));)
			STATSGEN_DEBUG(DEBUG_RARELY,msg);
			retVal=true;
			// This is a join
			decode.playerName=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerName,NULL);
			decode.playerID=RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerGUID,NULL); 
			UpdateValue(decode.playerClass,RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerClass,&replacementPlayerClass)); 
			UpdateValue(decode.playerTeam,RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexPlayerTeam,&replacementPlayerTeam)); 
			UpdatePlayerRoundInfo(decode.playerName,
								playerTeamListNames,
								playerTeamList,
								decode.playerTeam);
			UpdatePlayerRoundInfo(decode.playerName,
								playerClassListNames,
								playerClassList,
								decode.playerClass);
								
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool MOHAALogFileReader::DecodePlayerQuit(wxString &line,DecodedPlayerQuit &decode)
{
	bool	retVal=false;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodePlayerQuit");

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

void MOHAALogFileReader::UpdateValue(wxString &destination,wxString source)
{
	if (source.Length()>0)
	{
		destination=source;
	}
}

bool MOHAALogFileReader::DecodeRoundStart(wxString &line,
									DecodedRoundStart &decode,
									Round *round)
{
	bool		retVal=false;
	wxString	combinedString;
	char		idCode;
	int			expressionIndex;
	wxString	msg;
	wxString	replacement;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","DecodeRoundStart");

	if (MatchingExpression(line,&idCode,&expressionIndex))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,line)
		if (idCode == 'R')
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Round Start found [%s]",STRING_TO_CHAR(line));)
			STATSGEN_DEBUG(DEBUG_RARELY,msg);
			playerTeamListNames.Clear();
			playerTeamList.Clear();
			playerClassListNames.Clear();
			playerClassList.Clear();
			playerIDListNames.Clear();
			playerIDList.Clear();
			round->logfileVariableKeys.Clear();
			round->logfileVariableValues.Clear();
			retVal=true;
			// This is a Round Start
			UpdateValue(decode.mapName,RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexMap,&replacementMap));
			UpdateValue(decode.gameType,RetrieveRegularExpMatch(line,expressionIndex,&subexpressionIndexGameType,&replacementGameType));
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool MOHAALogFileReader::IsRoundStart(wxString &line)
{
	bool	retVal;
	char	idCode;
	int		expressionIndex;
	retVal=false;
	retVal=(line.StartsWith("Server:"));

	if (!retVal)
	{
		if (MatchingExpression(line,&idCode,&expressionIndex))
		{
			if (idCode == 'R')
			{
				retVal=true;
			}
		}
	}
	return (retVal);
}

bool MOHAALogFileReader::IsRoundEnd(wxString &line)
{
	bool	retVal;

	retVal=false;
	retVal=(line.StartsWith("==== ShutdownGame ===="));

	return (retVal);
}

bool MOHAALogFileReader::StripTimeFromLine(wxString &lineRead,wxDateTime &lineTime)
{
	bool		retVal=false;
	int			minutes=0;
	int			seconds=0;
	time_t		since1970;
	

	retVal=true;
	since1970=(60*minutes)+seconds;
	lineTime.Set(since1970);

	return(retVal);
}

MOHAALogFileReader::~MOHAALogFileReader()
{
	// delete the regular expressions
	int			expressionCount;
	int			expressionIndex;
	wxRegEx		*regExp;

	expressionCount=compiledRegularExpressions.GetCount();
	for (expressionIndex=0;expressionIndex<expressionCount;expressionIndex++)
	{
		regExp=(wxRegEx *)compiledRegularExpressions.Item(expressionIndex);
		delete (regExp);
	}
	compiledRegularExpressions.Clear();
}

