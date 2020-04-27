#ifndef __ALIASCACHEENTRY
#define __ALIASCACHEENTRY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

class AliasCacheEntry;

WX_DEFINE_SORTED_ARRAY(AliasCacheEntry *,ArrayOfAliasCacheEntry);
class ArrayOfAliasCacheEntry;
class AliasCacheEntry
{
	public:
		static int Compare(AliasCacheEntry *item1,AliasCacheEntry *item2);
		wxString	name;
		wxString	guid;
		int			index;
};

#endif
