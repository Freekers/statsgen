// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "DropListEntry.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

DropListEntry::DropListEntry()
{
	lastCompiledName="";
	regExp=NULL;
}

DropListEntry::~DropListEntry()
{
	if (regExp!=NULL)
	{
		delete (regExp);
	}
}

bool DropListEntry::Matches(wxString &nameIn,bool *cheatIn)
{
	bool		matched=false;
	wxString	nameToCheck;

	if (coloured)
	{
		nameToCheck=nameIn;
	}
	else
	{
		nameToCheck=DecolouriseName(nameIn);
	}

	if (!prefix)
	{
		// Regular Expression Match
		if (nameToCheck.Cmp(name)==0)
		{
			*cheatIn=cheat;
			matched=true;
		}
	}
	else
	{
		// We are doing a prefix match of some kind
		if (regularExpression)
		{
			if (regExp==NULL)
			{
				regExp=new wxRegEx();
			}

			if (name.Cmp(lastCompiledName)!=0)
			{
				// Need to compile the regular expression
				// only do this once if possible
				regExp->Compile(name);
				lastCompiledName=name;
			}

			if (regExp->Matches(nameToCheck))
			{
				*cheatIn=cheat;
				matched=true;
			}
		}
		else
		{
			// Just a standard prefix string
			if (nameToCheck.StartsWith(name))
			{
				*cheatIn=cheat;
				matched=true;
			}
		}
	}

	if (matched && dontDrop)
	{
		// This has been set to Not Drop a matched name
		*cheatIn=false;
		matched=false;
	}
	return (matched);
}

bool DropListEntry::DropType()
{
	return (dontDrop || drop);
}
void DropListEntry::ReadFromFile(wxTextFile *fp,int index)
{
	wxString	lineRead;
	wxString	tempString;
	wxString	token;


	lineRead=fp->GetLine(index);

	dontDrop=false;
	drop=false;
	coloured=false;
	prefix=false;
	cheat=false;
	regularExpression=false;
	name="";

	tempString=lineRead;
	token=tempString.BeforeFirst(',');
	tempString=tempString.AfterFirst(',');
	if (token.CmpNoCase("D")==0)
	{
		drop=true;
		cheat=false;
		dontDrop=false;
	}
	if (token.CmpNoCase("C")==0)
	{
		drop=true;
		cheat=true;
		dontDrop=false;
	}
	if (token.CmpNoCase("X")==0)
	{
		drop=false;
		cheat=false;
		dontDrop=true;
	}

	token=tempString.BeforeFirst(',');
	tempString=tempString.AfterFirst(',');
	if (token.CmpNoCase("Y")==0)
	{
		coloured=true;
	}

	token=tempString.BeforeFirst(',');
	tempString=tempString.AfterFirst(',');
	if (token.CmpNoCase("P")==0)
	{
		prefix=true;
		regularExpression=false;
	}
	if (token.CmpNoCase("R")==0)
	{
		prefix=true;
		regularExpression=true;
	}
	if (token.CmpNoCase("E")==0)
	{
		prefix=false;
		regularExpression=false;
	}
	name=tempString;
}

void DropListEntry::WriteToFile(FILE *fp)
{
	char	dropChar;
	char	colouredChar;
	char	matchChar;

	dropChar='A';
	colouredChar='N';
	matchChar='E';
	if (drop)
	{
		if (cheat)
		{
			dropChar='C';
		}
		else
		{
			dropChar='D';
		}
	}
	if (dontDrop)
	{
		dropChar='X';
	}
	if (coloured)
	{
		colouredChar='Y';
	}
	if (prefix)
	{
		matchChar='P';
		if (regularExpression)
		{
			matchChar='R';
		}
	}
	fprintf(fp,"%c,%c,%c,%s\n",
		dropChar,
		colouredChar,
		matchChar,
		name.GetData());
}
int DropListEntry::Compare(DropListEntry *item1,DropListEntry *item2)
{
	return (item1->name.Cmp(item2->name));
}

