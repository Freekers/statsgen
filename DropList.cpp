// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "DropList.h"
#include "DropListEntry.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "Progress.h"

DropList::DropList()
{
}

DropList::~DropList()
{
}

int DropList::Compare(DropListEntry **entry1,DropListEntry **entry2)
{
	wxString	name1;
	wxString	name2;

	name1=DecolouriseName((*entry1)->name);
	name2=DecolouriseName((*entry2)->name);

	return (name1.Cmp(name2));
}

void DropList::Sort()
{
	allowList.Sort(DropList::Compare);
	dropList.Sort(DropList::Compare);
}

void DropList::Load()
{
	int					dropCount;
	int					dropIndex;
	wxTextFile			fp;
	wxString			configKey="/General/DropList";
	wxString			configValue;
	bool				retVal;
	wxString			status;
	DropListEntry		dropListEntry;

	status="Reading Drop List"; progress->SetStatus(status);
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"droplist.dat");
	if (wxFileExists(configValue))
	{
		retVal=fp.Open(configValue);
		dropList.Clear();
		allowList.Clear();
		if (retVal)
		{
			dropCount=fp.GetLineCount();
			progress->Initiate(dropCount,
							"",
							(long)1,
							"",
							1);
			for (dropIndex=0;dropIndex<dropCount;dropIndex++)
			{
				progress->Update(dropIndex+1);
				dropListEntry.ReadFromFile(&fp,dropIndex);
				if (dropListEntry.DropType())
				{
					dropList.Add(dropListEntry);
				}
				else
				{
					allowList.Add(dropListEntry);
				}
			}
		}
	}

	cheatListCache.Clear();
	dropListCache.Clear();
	notDroppedListCache.Clear();

}

void DropList::Write()
{
	DropListEntry		dropListEntry;
	int					dropCount;
	int					dropIndex;
	FILE				*fp;
	wxString			configKey="/General/DropList";
	wxString			configValue;

	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"droplist.dat");
	fp=fopen(STRING_TO_CHAR(configValue),"w");
	if (fp!=NULL)
	{
		dropCount=allowList.GetCount();
		for (dropIndex=0;dropIndex<dropCount;dropIndex++)
		{
			dropListEntry=allowList.Item(dropIndex);
			dropListEntry.WriteToFile(fp);
		}
		dropCount=dropList.GetCount();
		for (dropIndex=0;dropIndex<dropCount;dropIndex++)
		{
			dropListEntry=dropList.Item(dropIndex);
			dropListEntry.WriteToFile(fp);
		}
		fclose(fp);
	}
}

int DropList::DropIndex(wxString &name,ArrayOfDropListEntry &list)
{
	int				dropCount;
	int 			dropIndex;
	int 			retVal=-1;
	DropListEntry	dropEntry;

	dropCount=list.GetCount();
	for (dropIndex=0;dropIndex<dropCount;dropIndex++)
	{
		dropEntry=list.Item(dropIndex);
		if (dropEntry.name.Cmp(name)==0)
		{
			retVal=dropIndex;
			break;
		}
	}
	
	return (retVal);
}

void DropList::AddCheat(wxString &name)
{
	int	dropIndex;

	dropIndex=DropIndex(name,dropList);
	if (dropIndex==-1)
	{
		DropListEntry	dropListEntry;

		dropListEntry.drop=true;
		dropListEntry.cheat=true;
		dropListEntry.dontDrop=false;
		dropListEntry.coloured=true;
		dropListEntry.prefix=false;
		dropListEntry.regularExpression=false;
		dropListEntry.name=name;
		dropList.Add(dropListEntry);
	}
}

void DropList::AddEntry(DropListEntry &entry)
{
	int	dropIndex;

	if (entry.DropType())
	{
		dropIndex=DropIndex(entry.name,dropList);
		if (dropIndex==-1)
		{
			dropList.Add(entry);
		}
	}
	else
	{
		dropIndex=DropIndex(entry.name,allowList);
		if (dropIndex==-1)
		{
			allowList.Add(entry);
		}
	}
}

void DropList::RemoveEntry(DropListEntry &entry)
{
	int	dropIndex;

	if (entry.DropType())
	{
		dropIndex=DropIndex(entry.name,dropList);
		if (dropIndex!=-1)
		{
			dropList.RemoveAt(dropIndex);
		}
	}
	else
	{
		dropIndex=DropIndex(entry.name,allowList);
		if (dropIndex!=-1)
		{
			allowList.RemoveAt(dropIndex);
		}
	}
}

void DropList::AddToCache(wxString &name,bool dropped,bool cheat)
{
	if (dropped)
	{
		if (cheat)
		{
			cheatListCache.Add(name);
		}
		else
		{
			dropListCache.Add(name);
		}
	}
	else
	{
		notDroppedListCache.Add(name);
	}
}

bool DropList::InCache(wxString &name,bool *dropped,bool *cheat)
{
	int		indexFound;

	if (useCache)
	{
		indexFound=notDroppedListCache.Index(name);
		if (indexFound!=wxNOT_FOUND)
		{
			*cheat=false;
			*dropped=false;
			return (true);
		}
		indexFound=cheatListCache.Index(name);
		if (indexFound!=wxNOT_FOUND)
		{
			*cheat=true;
			*dropped=true;
			return (true);
		}
		indexFound=dropListCache.Index(name);
		if (indexFound!=wxNOT_FOUND)
		{
			*cheat=false;
			*dropped=true;
			return (true);
		}
	}

	return (false);
}

bool DropList::IsDropped(wxString &name,bool *cheat)
{
	bool			dropped=false;
	int				dropCount;
	int				allowCount;
	int				dropIndex;
	DropListEntry	*dropListEntry;

	*cheat=false;
	allowCount=allowList.GetCount();
	dropCount=dropList.GetCount();
	if ((allowCount>5)||(dropCount>5))
	{
		useCache=true;
	}
	else
	{
		useCache=false;
	}
	if (InCache(name,&dropped,cheat))
	{
		return (dropped);
	}

	// If we have an allow list then we default to dropped
	// If we do not have an allow list we default to not
	// dropped
	dropped=(allowCount>0);

	// See if it is in the allow list
	for (dropIndex=0;dropIndex<allowCount;dropIndex++)
	{
		dropListEntry=allowList.Detach(dropIndex);
		allowList.Insert(dropListEntry,dropIndex);

		if (dropListEntry->Matches(name,cheat))
		{
			// This name matches this pattern so the
			// name is allowed
			dropped=false;
			break;
		}
	}

	// we have scanned the allow list - even though we may have
	// decided to drop this player we need to see if this player
	// is a cheat or not
	for (dropIndex=0;(dropIndex<dropCount) && (!(*cheat));dropIndex++)
	{
		dropListEntry=dropList.Detach(dropIndex);
		dropList.Insert(dropListEntry,dropIndex);
		if (dropListEntry->Matches(name,cheat))
		{
			// This name matches this pattern so the
			// name is dropped
			dropped=true;
			break;
		}
	}

	AddToCache(name,dropped,*cheat);
	return (dropped);
}

