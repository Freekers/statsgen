// wxWindows includes
#include <wx/dynarray.h>
#include <wx/arrimpl.cpp>

// statsgen includes
#include "TemplateFile.h"
#include "GUITriggerItem.h"
#include "Player.h"
#include "KillData.h"
#include "AKAListEntry.h"
#include "ActionData.h"
#include "SpeechData.h"
#include "TeamWinData.h"
#include "TeamLossData.h"
#include "Round.h"
#include "ConfigItem.h"
#include "StreakData.h"
#include "GenericKey.h"
#include "TemplateOpenFile.h"
#include "TemplateVariableField.h"
#include "TemplateOpenQuery.h"
#include "Server.h"
#include "LogFile.h"
#include "PlayerCacheEntry.h"
#include "AliasListEntry.h"
#include "WeightListEntry.h"
#include "AwardDefinition.h"
#include "Clan.h"
#include "DropListEntry.h"
#include "KillTotal.h"
#include "PlayerDataEntry.h"
#include "KeyedIndex.h"
#include "XPPoints.h"

WX_DEFINE_OBJARRAY(ArrayOfXPPoints);
WX_DEFINE_OBJARRAY(ArrayOfGUITriggerItem);
WX_DEFINE_OBJARRAY(ArrayOfWeightListEntry);
WX_DEFINE_OBJARRAY(ArrayOfPlayerDataEntry);
WX_DEFINE_OBJARRAY(ArrayOfPlayerCacheEntry);
WX_DEFINE_OBJARRAY(ArrayOfAliasListEntry);
WX_DEFINE_OBJARRAY(ArrayOfAKAListEntry);
WX_DEFINE_OBJARRAY(ArrayOfDropListEntry);
WX_DEFINE_OBJARRAY(ArrayOfPlayers);
WX_DEFINE_OBJARRAY(ArrayOfPlayersInRound);
WX_DEFINE_OBJARRAY(ArrayOfKillData);
WX_DEFINE_OBJARRAY(ArrayOfActionData);
WX_DEFINE_OBJARRAY(ArrayOfTeamWinData);
WX_DEFINE_OBJARRAY(ArrayOfTeamLossData);
WX_DEFINE_OBJARRAY(ArrayOfSpeechData);
WX_DEFINE_OBJARRAY(ArrayOfRoundLines);
WX_DEFINE_OBJARRAY(ArrayOfRounds);
WX_DEFINE_OBJARRAY(ArrayOfConfigItem);
WX_DEFINE_OBJARRAY(ArrayOfStreakData);
WX_DEFINE_OBJARRAY(ArrayOfGenericKey);
WX_DEFINE_OBJARRAY(ArrayOfTemplateOpenFile);
WX_DEFINE_OBJARRAY(ArrayOfTemplateVariableField);
WX_DEFINE_OBJARRAY(ArrayOfTemplateOpenQuery);
WX_DEFINE_OBJARRAY(ArrayOfServer);
WX_DEFINE_OBJARRAY(ArrayOfLogFile);
WX_DEFINE_OBJARRAY(ArrayOfAwardDefinition);
WX_DEFINE_OBJARRAY(ArrayOfClan);
WX_DEFINE_OBJARRAY(ArrayOfKillTotal);
WX_DEFINE_OBJARRAY(ArrayOfCompiledTemplateLine);
//WX_DEFINE_SORTED_ARRAY(KeyedIndex *,ArrayOfKeyedIndex);

//WX_DEFINE_ARRAY(void *,ArrayOfPointers); 
