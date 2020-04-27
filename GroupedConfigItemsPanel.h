#ifndef __GROUPEDCONFIGITEMSPANEL
#define __GROUPEDCONFIGITEMSPANEL

#include <wx/wx.h>
#include <wx/panel.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "SelectionFreeFormConfigItemGUI.h"
#include "TextConfigItemGUI.h"
#include "FileConfigItemGUI.h"
#include "RemoteFileConfigItemGUI.h"
#include "RemoteDirectoryConfigItemGUI.h"
#include "DirectoryConfigItemGUI.h"
#include "DynamicArrays.h"
#include "GUITriggerList.h"

class GroupedConfigItemsPanel : public wxPanel
{
	public:
		int PreferredHeight();
		GroupedConfigItemsPanel(char *titleIn);
		~GroupedConfigItemsPanel();
		void AddSchedule(char *label,
			wxString &configKey,
			wxString &defaultValue,
			bool intervalWithBase,
			GUITriggerList *triggerList=NULL);
		void AddBoolean(char *label,
			wxString &configKey,
			bool defaultValue,
			GUITriggerList *triggerList=NULL);
		void Add(char *label,
			wxString &configKey,
			char *defaultValue,
			int charWidth,
			GUITriggerList *triggerList=NULL);
		void AddSelectionFreeForm(char *label,
			wxString &configKey,
			char *defaultValue,
			wxArrayString &codes,
			wxArrayString &names,
			int charWidth,
			GUITriggerList *triggerList=NULL);
		void AddSelection(char *label,
			wxString &configKey,
			char *defaultValue,
			wxArrayString &codes,
			wxArrayString &names,
			GUITriggerList *triggerList=NULL);
		FileConfigItemGUI *AddFile(char *label,
			wxString &configKey,
			char *defaultValue,
			int charWidth,
			DirectoryConfigItemGUI *directoryGUI=NULL,
			GUITriggerList *triggerList=NULL);
		DirectoryConfigItemGUI *AddDirectory(char *label,
			wxString &configKey,
			char *defaultValue,
			int charWidth,
			GUITriggerList *triggerList=NULL);
		RemoteFileConfigItemGUI *AddRemoteFile(char *label,
			wxString &configKey,
			char *defaultValue,
			int charWidth,
			wxString &FTPIDIn,
			RemoteDirectoryConfigItemGUI *directoryGUI=NULL,
			GUITriggerList *triggerList=NULL);
		RemoteDirectoryConfigItemGUI *AddRemoteDirectory(char *label,
			wxString &configKey,
			char *defaultValue,
			int charWidth,
			wxString &FTPIDIn,
			GUITriggerList *triggerList=NULL);
			
			void OnResize(wxSizeEvent &event);
			void OnTextChange(wxCommandEvent &event);
		bool UpdateFromTrigger();

	protected:

	private:
		wxString	title;
		wxStaticBox	staticBox;
		bool		staticBoxAdded;
		ArrayOfPointers	configItemPanels;
		DECLARE_EVENT_TABLE()
};

#endif
