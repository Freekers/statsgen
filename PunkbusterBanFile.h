#ifndef __PUNKBUSTERBANFILE
#define __PUNKBUSTERBANFILE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/filename.h>

// Statsgen Includes

class PunkbusterBanFile
{
	public:
		PunkbusterBanFile(wxFileName &filenameIn);
		virtual ~PunkbusterBanFile();
		void AddToDropList();

	private:
		void LoadFile();
		wxFileName	filename;

		wxArrayString	names;
		wxArrayString	reasons;
		
};


#endif
