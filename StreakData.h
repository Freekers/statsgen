#ifndef __STREAKDATA
#define __STREAKDATA

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes
#include "KillData.h"

class StreakData;

WX_DECLARE_OBJARRAY(StreakData,ArrayOfStreakData);
class StreakData
{
	public:
		StreakData();
		virtual ~StreakData();
		int streakSize;
		int streakEndedPlayerIndex;
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int roundIndex,int playerIndex,
							int itemIndex,KillTypes streakType);

		void Debug();
};


#endif
