#ifndef __FTPCONNECTIONPANEL
#define __FTPCONNECTIONPANEL

#include <wx/wx.h>
#include <wx/panel.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GroupedConfigItemsPanel.h"

class FTPConnectionPanel : public GroupedConfigItemsPanel
{
	public:
		FTPConnectionPanel();
		~FTPConnectionPanel();
		void CreateConfigs(char *group);
	protected:

	private:
};

#endif
