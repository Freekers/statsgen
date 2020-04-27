// wxWindows includes
#include <wx/wx.h>
#include <wx/sizer.h>

// Statsgen includes
#include "SkillTypePanel.h"
#include "Server.h"
#include "GlobalStatistics.h"
#include "WindowIDs.h"

wxString SkillTypePanel::GetLabel()
{
	wxString	label="Skill Type";
	return (label);
}

wxString SkillTypePanel::AllName()
{
	wxString	all="All Skill Types";

	return (all);
}

wxString SkillTypePanel::AllCode()
{
	wxString	all="";

	return (all);
}

wxString SkillTypePanel::FindCodeFromName(wxString &name)
{
	int			count;
	int			index;
	wxString	foundName;
	wxString	retVal="";

	count=SKILL_TYPE_COUNT;

	for (index=0;index<count;index++)
	{
		if (strcmp(SkillTypeNames[index],name.GetData())==0)
		{
			retVal=SkillTypeCodes[index];
		}
	}

	return (retVal);
}

void SkillTypePanel::DropDownEntries(wxArrayString &entries,
									wxString &defaultSelection)
{
	int			index;
	wxString	skillName;
	int			count;

	entries.Clear();
	defaultSelection="";

	count=SKILL_TYPE_COUNT;
	for (index=0;index<count;index++)
	{
		
		skillName=SkillTypeNames[index];
		entries.Add(skillName);
		if (index==0)
		{
			defaultSelection=skillName;
		}
	}
}


