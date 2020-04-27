// Statsgen Includes
#include "GenericNumberedConfigGUI.h"
#include "GenericConfigPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(GenericNumberedConfigGUI, wxDialog)
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
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","GenericNumberedConfigGUI")
	mConfigPanel		= NULL;
	mListGroup			= listGroupIn;
	mListGroupPrefix	= listGroupPrefixIn;
	mNewPanelFunction	= newPanelFunctionIn;
	mMainSizer			= NULL;
	mControlsSizer		= NULL;
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::OnListItemSelected(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","OnListItemSelected")
	// we have a new selection
	// remove the current panel (if present) and retrieve
	// a new one using the retrieve function
	wxString	currentSelection;
	wxSize		currentSize;

	currentSize = GetSize();
	if (mConfigPanel!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Removing old config panel");
		// first remove the old panel
		mConfigPanel->Destroy();
		//delete(configPanel);
		mConfigPanel=NULL;
	}
	currentSelection=mIDList.GetValue();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"currentSelection");
	STATSGEN_DEBUG(DEBUG_ALWAYS,currentSelection);
	if (currentSelection.Length()>0)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"currentSelection is valid");
		mConfigPanel	= mNewPanelFunction(this,currentSelection);
		//configPanel->OnResize(dummyEvent);
		SetConfigPanel(mConfigPanel);
	}
	SetSize(currentSize);
	PostSizeEvent();
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::OnQuit(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","OnQuit")
	EndModal(event.GetId());
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::OnSave(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","OnSave")
	EndModal(event.GetId());
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::WriteIDList()
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","WriteIDList")
	int		idCount;
	int		idIndex;
	wxArrayString	ids;
	wxString		id;

	idCount=mIDList.GetCount();
	for (idIndex=0;idIndex<idCount;idIndex++)
	{
		id=mIDList.GetString(idIndex);
		ids.Add(id);
	}
	
	globalStatistics.configData.WriteList(mListGroup,ids);
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::OnNew(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","OnNew")
	wxString	caption="Enter New ID";
	wxString	message="Enter New ID";
	wxString	chosen;
	wxCommandEvent		dummyEvent;

	chosen=wxGetTextFromUser(message,caption);
	if (chosen.Length()>0)
	{
		SafeString(chosen);
		mIDList.Append(chosen);
		mIDList.SetValue(chosen);
		OnListItemSelected(dummyEvent);
		WriteIDList();
	}
	PostSizeEvent();
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::OnDelete(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","OnDelete")
	wxString		currentSelection;
	wxString		group;
	int				currentSelectionIndex;
	wxCommandEvent	dummyEvent;
	int				idCount;
	wxString		msg;

	currentSelection=mIDList.GetValue();

	if (currentSelection.Length()>0)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,currentSelection)
		currentSelectionIndex=mIDList.GetSelection();
		msg.Printf("currentSelectionIndex=%d",currentSelectionIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		if (currentSelectionIndex!=wxNOT_FOUND)
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Found it")
			// delete the group from the config file
			group=mListGroupPrefix+currentSelection;
			globalStatistics.configData.DeleteGroup(group);

			// delete the current selection from the id list
			mIDList.Delete(currentSelectionIndex);

			// select another panel
			idCount=mIDList.GetCount();
			if (idCount>0)
			{
				mIDList.SetSelection(0);
			}

			// Need to force the selection event
			OnListItemSelected(dummyEvent);

			// Write the ID List back to the config
			WriteIDList();
		}
		// Write the ID List to the config
	}
	PostSizeEvent();
	STATSGEN_DEBUG_FUNCTION_END
}

GenericNumberedConfigGUI::~GenericNumberedConfigGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","~GenericNumberedConfigGUI")
	STATSGEN_DEBUG_FUNCTION_END
}

//void GenericNumberedConfigGUI::SetConfigPanel(GenericConfigPanel *configPanelIn)
void GenericNumberedConfigGUI::SetConfigPanel(wxPanel *configPanelIn)
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","SetConfigPanel")
	mConfigPanel	= configPanelIn;
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::CreateDialog()
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","CreateDialog")
	wxString		label="Generic Configuration";
	wxString		defaultValue="";
	wxString		configKey;
	wxCommandEvent		event;
	GroupedConfigItemsPanel	*configPanel;

	wxPoint			configItemsPosition=wxDefaultPosition;
	wxSize			configItemsSize=wxDefaultSize;
	wxArrayString	ids;
	wxString		initialSelection="";
	

	globalStatistics.configData.ReadList(mListGroup,ids);

	if (ids.GetCount()>0)
	{
		initialSelection=ids.Item(0);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating idList");
	mIDList.Create(this, 
					WINDOW_ID_LISTBOX_CONFIGITEMS,
					initialSelection,
					wxDefaultPosition,
					wxDefaultSize,
					ids,
					wxCB_DROPDOWN | wxCB_READONLY | wxLB_SINGLE
					);
					
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating save button");
	mSaveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T(WINDOW_ID_BUTTON_SAVE_TEXT),
					wxDefaultPosition);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating delete button");
	mDeleteButton.Create(this,
					WINDOW_ID_BUTTON_DELETE,
					_T(WINDOW_ID_BUTTON_DELETE_TEXT),
					wxDefaultPosition);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating new button");
	mNewButton.Create(this,
					WINDOW_ID_BUTTON_NEW,
					_T(WINDOW_ID_BUTTON_NEW_TEXT),
					wxDefaultPosition);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating quit button");
	mQuitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating new sizer:2");
	mControlsSizer = new wxBoxSizer(wxHORIZONTAL);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating new sizer:3");
	mControlsSizer->Add(&mNewButton);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating new sizer:4");
	mControlsSizer->Add(&mDeleteButton);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating new sizer:5");
	mControlsSizer->Add(&mSaveButton);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating new sizer:6");
	mControlsSizer->Add(&mQuitButton);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"force list item selected");
	OnListItemSelected(event);
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::ConfigureSizer()
{
	bool	resizeNeeded = false;
	wxSize	windowSize;
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","ConfigureSizer")
	if (mMainSizer == NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating new sizer");
		mMainSizer = new wxBoxSizer(wxVERTICAL);
	}
	else
	{
		windowSize	= GetSize();
		resizeNeeded = true;
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"detaching items from sizer:idList");
		mMainSizer->Detach(&mIDList);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"detaching items from sizer:configPanel");
		mMainSizer->Detach(mConfigPanel);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"detaching items from sizer:controls");
		mMainSizer->Detach(mControlsSizer);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding idList");
	mMainSizer->Add(&mIDList);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding configPanel");
	mMainSizer->Add(mConfigPanel,1,wxEXPAND);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding controls");
	mMainSizer->Add(mControlsSizer);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Setting sizer");
	SetSizer(mMainSizer);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Setting size hints");
	mMainSizer->SetSizeHints(this);
	if (resizeNeeded)
	{
		PostSizeEventToParent();
		//SetSize(windowSize);Fit();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GenericNumberedConfigGUI::DisplayDialog()
{
	STATSGEN_DEBUG_FUNCTION_START("GenericNumberedConfigGUI","DisplayDialog")
	int			dialogRetVal;

	// Called when we want to pop the dialog box
	// into existance for the first time

	// First we want to create all the items in the dialog box
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"create dialog");
	CreateDialog();

	// Now we can resize every item in the dialog to fit nicely

	// Then we pop it into existance
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"show modal");
	dialogRetVal=ShowModal();

	// Now we do what is necessary dependent on the return code
	switch (dialogRetVal)
	{
		case WINDOW_ID_BUTTON_SAVE:
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"save pressed");
			// We have been asked to save the changes
			// just commit the config changes
			globalStatistics.configData.CommitChanges();
			break;
		case WINDOW_ID_BUTTON_QUIT:
		default:
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"quit or other pressed");
			// We have been asked to quit without saving
			// rollback the config changes
			globalStatistics.configData.RollbackChanges();
			break;
	}
	STATSGEN_DEBUG_FUNCTION_END
}
