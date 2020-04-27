#ifndef __CONFIGITEM
#define __CONFIGITEM

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class ConfigItem;

WX_DECLARE_OBJARRAY(ConfigItem,ArrayOfConfigItem);
class ConfigItem
{
	public:
		ConfigItem();
		virtual ~ConfigItem();
		wxString	key;
		wxString	value;
		bool		committed;
};

#endif
