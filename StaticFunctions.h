#ifndef __STATICFUNCTIONS
#define __STATICFUNCTIONS

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/app.h>
#include <wx/filename.h>

// Statsgen Includes
#include "StatsgenApp.h"

#define UPGRADE_CHOICE_DEFAULT		"Default"
#define UPGRADE_CHOICE_DO_NOTHING	"Do Nothing"
#define UPGRADE_CHOICE_UPDATE		"Update"
#define UPGRADE_CHOICE_OVERWRITE	"Overwrite"

bool NeedsFirstTimeRun();
bool FirstTimeRun();
wxString DecolouriseName(wxString &name);
void SafeString(wxString &string);
wxFileName BaseDirectoryFileName(const char *filename);
void ExportTemplateFull(wxString &directory);
void UpgradeConfigFile(wxString &upgradeFilenameStr);
bool IsHexString(wxString &stringToCheck);
DECLARE_APP(wxWidgetsApp)

#endif
