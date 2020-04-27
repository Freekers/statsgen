#ifndef __PLAYERCACHEENTRY
#define __PLAYERCACHEENTRY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class PlayerCacheEntry;

WX_DECLARE_OBJARRAY(PlayerCacheEntry,ArrayOfPlayerCacheEntry);
class PlayerCacheEntry
{
	public:
		PlayerCacheEntry();
		~PlayerCacheEntry();
		wxString	guid;
		wxString	name;
		static int GUIDCompare(
					PlayerCacheEntry **item1,
					PlayerCacheEntry **item2);
};


#endif
