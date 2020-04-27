#ifndef __TEMPLATEOPENFILE
#define __TEMPLATEOPENFILE

#include <stdio.h>
// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class TemplateOpenFile;

WX_DECLARE_OBJARRAY(TemplateOpenFile,ArrayOfTemplateOpenFile);
class TemplateOpenFile
{
	public:
		TemplateOpenFile();
		virtual ~TemplateOpenFile();

	public:
		bool Open(wxString &filename,char *mode);
		bool Write(wxString &text);

	private:
		FILE		*fp;
		wxString	filename;
};


#endif
