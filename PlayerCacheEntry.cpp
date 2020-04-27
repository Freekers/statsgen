// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "PlayerCacheEntry.h"
#include "GlobalStatistics.h"

PlayerCacheEntry::PlayerCacheEntry()
{
}

PlayerCacheEntry::~PlayerCacheEntry()
{
}

int PlayerCacheEntry::GUIDCompare(
				PlayerCacheEntry **item1,
				PlayerCacheEntry **item2
				)
{
	return ((*item1)->guid.Cmp((*item2)->guid));
}
