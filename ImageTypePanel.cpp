// wxWindows includes
#include <wx/wx.h>
#include <wx/sizer.h>

// Statsgen includes
#include "ImageTypePanel.h"
#include "Server.h"
#include "GlobalStatistics.h"
#include "WindowIDs.h"

wxString ImageTypePanel::GetLabel()
{
	wxString	label="Stat Type";
	return (label);
}

wxString ImageTypePanel::AllName()
{
	wxString	all="All Stat Types";

	return (all);
}

wxString ImageTypePanel::AllCode()
{
	wxString	all="";

	return (all);
}

wxString ImageTypePanel::FindCodeFromName(wxString &name)
{
	int			count;
	int			index;
	wxString	foundName;
	wxString	retVal="";

	count=IMAGE_TYPE_COUNT;

	for (index=0;index<count;index++)
	{
		if (strcmp(ImageTypeNames[index],STRING_TO_CHAR(name))==0)
		{
			retVal=ImageTypeCodes[index];
		}
	}

	return (retVal);
}

void ImageTypePanel::DropDownEntries(wxArrayString &entries,
									wxString &defaultSelection)
{
	int			index;
	wxString	imageName;
	int			count;

	entries.Clear();
	defaultSelection="";

	count=IMAGE_TYPE_COUNT;
	for (index=0;index<count;index++)
	{
		
		imageName=ImageTypeNames[index];
		entries.Add(imageName);
		if (index==0)
		{
			defaultSelection=imageName;
		}
	}
}


