#ifndef __CODBANFILE
#define __CODBANFILE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/filename.h>

// Statsgen Includes

class CODBanFile
{
	public:
		CODBanFile(wxFileName &filenameIn);
		virtual ~CODBanFile();
		void AddToDropList();

	private:
		void LoadFile();
		wxFileName	filename;

		wxArrayString	names;
		
};


#endif
