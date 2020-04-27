// Statsgen Includes
#include "GenericNumberedConfigGUI.h"
#include "GenericConfigPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(GenericNumberedConfigGUI, wxDialog)
		EVT_SIZE(GenericNumberedConfigGUI::OnResize)
		EVT_BUTTON(WINDOW_ID_BUTTON_SAVE,GenericNumberedConfigGUI::OnSave)
		EVT_BUTTON(WINDOW_ID_BUTTON_QUIT,GenericNumberedConfigGUI::OnQuit)
		EVT_BUTTON(WINDOW_ID_BUTTON_NEW,GenericNumberedConfigGUI::OnNew)
		EVT_BUTTON(WINDOW_ID_BUTTON_DELETE,GenericNumberedConfigGUI::OnDelete)
		EVT_COMBOBOX(WINDOW_ID_LISTBOX_CONFIGITEMS,GenericNumberedConfigGUI::OnListItemSelected)
END_EVENT_TABLE()

GenericNumberedConfigGUI::GenericNumberedConfigGUI(
//				GenericConfigPanel * (*newPanelFunctionIn)(wxWindow *parentID,wxString &listID),
				wxPanel * (*newPanelFunctionIn)(wxWindow *parentID,wxString &listID),
				wxString &listGroupIn,
				wxString &listGroupPrefixIn,
				wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name):
			wxDialog(parent,
				id,
				title,
				pos,
				size,
				style,
				name)

{
	configPanel=NULL;
	listGroup=listGroupIn;
	listGroupPrefix=listGroupPrefixIn;
	newPanelFunction=newPanelFunctionIn;
}

void GenericNumberedConfigGUI::OnListItemSelected(wxCommandEvent& event)
{
	wxSizeEvent		dummyEvent;
	// we have a new selection
	// remove the current panel (if present) and retrieve
	// a new one using the retrieve function
	wxString	currentSelection;

	if (configPanel!=NULL)
	{
		// first remove the old panel
		configPanel->Destroy();
		//delete(configPanel);
		configPanel=NULL;
	}
	currentSelection=idList.GetValue();
	if (currentSelection.Length()>0)
	{
		configPanel=newPanelFunction(this,currentSelection);
		//configPanel->OnResize(dummyEvent);
		SetConfigPanel(configPanel);
	}
	OnResize(dummyEvent);
}

void GenericNumberedConfigGUI::OnQuit(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void GenericNumberedConfigGUI::OnSave(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void GenericNumberedConfigGUI::WriteIDList()
{
	int		idCount;
	int		idIndex;
	wxArrayString	ids;
	wxString		id;

	idCount=idList.GetCount();
	for (idIndex=0;idIndex<idCount;idIndex++)
	{
		id=idList.GetString(idIndex);
		ids.Add(id);
	}
	
	globalStatistics.configData.WriteList(listGroup,ids);

}

void GenericNumberedConfigGUI::OnNew(wxCommandEvent& event)
{
	wxString	caption="Enter New ID";
	wxString	message="Enter New ID";
	wxString	chosen;
	wxCommandEvent		dummyEvent;

	chosen=wxGetTextFromUser(message,caption);
	if (chosen.Length()>0)
	{
		SafeString(chosen);
		idList.Append(chosen);
		idList.SetValue(chosen);
		OnListItemSelected(dummyEvent);
		WriteIDList();
	}
}

void GenericNumberedConfigGUI::OnDelete(wxCommandEvent& event)
{
	wxString		currentSelection;
	wxString		group;
	int				currentSelectionIndex;
	wxCommandEvent	dummyEvent;
	int				idCount;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","OnDelete")
	currentSelection=idList.GetValue();

	if (currentSelection.Length()>0)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,currentSelection)
		currentSelectionIndex=idList.GetSelection();
msg.Printf("currentSelectionIndex=%d",currentSelectionIndex);
STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		if (currentSelectionIndex!=wxNOT_FOUND)
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Found it")
			// delete the group from the config file
			group=listGroupPrefix+currentSelection;
			globalStatistics.configData.DeleteGroup(group);

			// delete the current selection from the id list
			idList.Delete(currentSelectionIndex);

			// select another panel
			idCount=idList.GetCount();
			if (idCount>0)
			{
				idList.SetSelection(0);
			}

			// Need to force the selection event
			OnListItemSelected(dummyEvent);

			// Write the ID List back to the config
			WriteIDList();
		}
		// Write the ID List to the config
	}
	STATSGEN_DEBUG_FUNCTION_END
}

GenericNumberedConfigGUI::~GenericNumberedConfigGUI()
{
}

//void GenericNumberedConfigGUI::SetConfigPanel(GenericConfigPanel *configPanelIn)
void GenericNumberedConfigGUI::SetConfigPanel(wxPanel *configPanelIn)
{
	configPanel=configPanelIn;
}

void GenericNumberedConfigGUI::CreateDialog()
{
	wxString		label="Generic Configuration";
	wxString		defaultValue="";
	wxString		configKey;
	wxCommandEvent		event;
	GroupedConfigItemsPanel	*configPanel;

	wxPoint			configItemsPosition=wxDefaultPosition;
	wxSize			configItemsSize=wxDefaultSize;
	wxArrayString	ids;
	wxString		initialSelection="";
	

	globalStatistics.configData.ReadList(listGroup,ids);

	if (ids.GetCount()>0)
	{
		initialSelection=ids.Item(0);
	}
	idList.Create(this, 
					WINDOW_ID_LISTBOX_CONFIGITEMS,
					initialSelection,
					wxDefaultPosition,
					wxDefaultSize,
					ids,
					wxCB_DROPDOWN | wxCB_READONLY | wxLB_SINGLE
					);
					
	saveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T(WINDOW_ID_BUTTON_SAVE_TEXT),
					wxDefaultPosition);
	deleteButton.Create(this,
					WINDOW_ID_BUTTON_DELETE,
					_T(WINDOW_ID_BUTTON_DELETE_TEXT),
					wxDefaultPosition);
	newButton.Create(this,
					WINDOW_ID_BUTTON_NEW,
					_T(WINDOW_ID_BUTTON_NEW_TEXT),
					wxDefaultPosition);
	quitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);



	OnListItemSelected(event);
}

void GenericNumberedConfigGUI::DisplayDialog()
{
	int			dialogRetVal;

	// Called when we want to pop the dialog box
	// into existance for the first time

	// First we want to create all the items in the dialog box
	CreateDialog();

	// Now we can resize every item in the dialog to fit nicely

	// Then we pop it into existance
	dialogRetVal=ShowModal();

	// Now we do what is necessary dependent on the return code
	switch (dialogRetVal)
	{
		case WINDOW_ID_BUTTON_SAVE:
			// We have been asked to save the changes
			// just commit the config changes
			globalStatistics.configData.CommitChanges();
			break;
		case WINDOW_ID_BUTTON_QUIT:
		default:
			// We have been asked to quit without saving
			// rollback the config changes
			globalStatistics.configData.RollbackChanges();
			break;
	}
}

void GenericNumberedConfigGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	int		quitWidth;
	int		quitHeight;
	int		saveWidth;
	int		saveHeight;
	int		newWidth;
	int		newHeight;
	int		deleteWidth;
	int		deleteHeight;
	int		configPanelWidth;
	int		configPanelHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		yPosition;
	int		listWidth;
	int		listHeight;

	
	itemSize=GetSize();
	dialogWidth=itemSize.GetWidth();
	dialogHeight=itemSize.GetHeight();

	itemSize=idList.GetSize();
	listWidth=itemSize.GetWidth();
	if (listWidth<150)
	{
		listWidth=150;
	}
	idList.SetSize(listWidth,listHeight);
	listHeight=itemSize.GetHeight();
	// Quit and Save buttons are at the bottom of the screen
	itemSize=quitButton.GetSize();
	quitWidth=itemSize.GetWidth();
	quitHeight=itemSize.GetHeight();

	itemSize=saveButton.GetSize();
	saveWidth=itemSize.GetWidth();
	saveHeight=itemSize.GetHeight();

	itemSize=newButton.GetSize();
	newWidth=itemSize.GetWidth();
	newHeight=itemSize.GetHeight();

	itemSize=deleteButton.GetSize();
	deleteWidth=itemSize.GetWidth();
	deleteHeight=itemSize.GetHeight();

	configPanelHeight=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	configPanelWidth=dialogWidth;

	// ID List
	idList.SetSize(0,0,listWidth,listHeight);
	// Config Panel
	yPosition=listHeight;
	// Config Panels resize themselves to fit
	if (configPanel!=NULL)
	{
		configPanel->SetSize(0,yPosition,configPanelWidth,configPanelHeight-listHeight);
	}
	// Save button
	itemPosition.x=BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	saveButton.SetPosition(itemPosition);

	// Quit button
	itemPosition.x+=(saveWidth+BUTTON_WIDTH_GAP);
	itemPosition.y=dialogHeight-quitHeight-DIALOG_BOTTOM_BORDER_SIZE;
	quitButton.SetPosition(itemPosition);

	// new button
	itemPosition.x+=(quitWidth+BUTTON_WIDTH_GAP);
	itemPosition.y=dialogHeight-newHeight-DIALOG_BOTTOM_BORDER_SIZE;
	newButton.SetPosition(itemPosition);

	// delete button
	itemPosition.x+=(newWidth+BUTTON_WIDTH_GAP);
	itemPosition.y=dialogHeight-deleteHeight-DIALOG_BOTTOM_BORDER_SIZE;
	deleteButton.SetPosition(itemPosition);

}

