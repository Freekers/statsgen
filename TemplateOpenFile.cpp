#include <stdio.h>

// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "TemplateOpenFile.h"
#include "ErrorData.h"

TemplateOpenFile::TemplateOpenFile()
{
	fp=NULL;
}

TemplateOpenFile::~TemplateOpenFile()
{
	wxString	msg;

//	msg.Printf("TemplateOpenFile::Destructor:Entering [%s]",filename.GetData());
//	StatsgenDebug(msg);
	if (fp!=NULL)
	{
//		StatsgenDebug("TemplateOpenFile::Destructor:Closing file");
		fclose(fp);
		fp=NULL;
	}
//	StatsgenDebug("TemplateOpenFile::Destructor:Leaving");
}

bool TemplateOpenFile::Open(wxString &filenameIn,char *mode)
{
	bool		retVal=false;
	wxString	msg;

	filename=filenameIn;
//	msg.Printf("Entering:Opening file [%s] in mode [%s]",filename.GetData(),mode);
//	StatsgenDebug(msg);
	fp=fopen(filename.GetData(),mode);
	if (fp!=NULL)
	{
//		StatsgenDebug("Successfully opened");
		retVal=true;
	}
//	msg.Printf("Leaving:Opening file [%s] in mode [%s]",filename.GetData(),mode);
//	StatsgenDebug(msg);
	
	return (retVal);
}

bool TemplateOpenFile::Write(wxString &text)
{
	bool		retVal=false;
	wxString	msg;
	
	if (fp!=NULL)
	{
		retVal=true;
		fprintf(fp,"%s",text.GetData());
	}
	return (retVal);
}

