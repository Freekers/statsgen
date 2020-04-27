// wxWindows includes
#include <wx/wx.h>
#include <wx/sizer.h>

// Statsgen includes
#include "ServerTypePanel.h"
#include "Server.h"
#include "GlobalStatistics.h"
#include "WindowIDs.h"

wxString ServerTypePanel::GetLabel()
{
	wxString	label="Server Type";
	return (label);
}

wxString ServerTypePanel::AllName()
{
	wxString	all="All Server Types";

	return (all);
}

wxString ServerTypePanel::AllCode()
{
	wxString	all="";

	return (all);
}

void ServerTypePanel::DropDownEntries(wxArrayString &entries,
									wxString &defaultSelection)
{
	int			index;
	wxString	serverName;
	int			count;

	entries.Clear();
	defaultSelection="";

	count=SERVER_TYPE_COUNT;
	for (index=0;index<count;index++)
	{
		
		serverName=ServerTypeNames[index];
		entries.Add(serverName);
		if (index==0)
		{
			defaultSelection=serverName;
		}
	}
}

wxString ServerTypePanel::FindCodeFromName(wxString &name)
{
	int			count;
	int			index;
	wxString	foundName;
	wxString	retVal="";

	count=SERVER_TYPE_COUNT;

	for (index=0;index<count;index++)
	{
		if (strcmp(ServerTypeNames[index],name.GetData())==0)
		{
			retVal=ServerTypeCodes[index];
		}
	}

	return (retVal);
}

