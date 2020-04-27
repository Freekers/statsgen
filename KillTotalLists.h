#ifndef __KILLTOTALLISTS
#define __KILLTOTALLISTS

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include "KillTotal.h"
#include "KillData.h"

// Statsgen Includes

class KillTotalLists
{
	public:
		ArrayOfKillTotal	maps;
		ArrayOfKillTotal	gameTypes;
		ArrayOfKillTotal	weapons;
		ArrayOfKillTotal	locations;
		ArrayOfKillTotal	teams;
		ArrayOfKillTotal	classes;

		void WriteToDatabase();
		void WriteToDatabase(ArrayOfKillTotal &list,const char *suffix);

		void Clear();
		void Add(
						ArrayOfKillTotal &maps,
						ArrayOfKillTotal &gameTypes,
						ArrayOfKillTotal &weapons,
						ArrayOfKillTotal &locations,
						ArrayOfKillTotal &teams,
						ArrayOfKillTotal &classes
						);
		static void Add(KillData &killData,
						int playerIndex,
						ArrayOfKillTotal &maps,
						ArrayOfKillTotal &gameTypes,
						ArrayOfKillTotal &weapons,
						ArrayOfKillTotal &locations,
						ArrayOfKillTotal &teams,
						ArrayOfKillTotal &classes
						);
		KillTotalLists();
		~KillTotalLists();
		void AppendKills(
						ArrayOfKillTotal &dest,
						ArrayOfKillTotal &source
						);
		static void AddKill(
				ArrayOfKillTotal &list,
				int id,
				int killType,
				int playerIndex);
};


#endif
