// Statsgen Includes
#include "AliasDialog.h"
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(AliasDialog, GenericOKCancelDialog)
		EVT_BUTTON(WINDOW_ID_BUTTON_AUTO,AliasDialog::OnAuto)
END_EVENT_TABLE()

AliasDialog::AliasDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name) : GenericOKCancelDialog
											(parent, 
											id,
											title,
											pos,
											size,
											style,
											name)
{
	panel=new AliasEditorPanel();
	panel->Create(this,
				-1,
				wxDefaultPosition,
				wxDefaultSize);
}

bool AliasDialog::DisplayDialog()
{
	return (GenericOKCancelDialog::DisplayDialog((wxPanel *)panel));
}

AliasDialog::~AliasDialog()
{
}

void AliasDialog::CreateDialog()
{
	wxString	label="Generic Configuration";
	wxString	defaultValue="";
	wxString	configKey;
	wxSizeEvent	event;

	wxPoint		configItemsPosition=wxDefaultPosition;
	wxSize		configItemsSize=wxDefaultSize;

	STATSGEN_DEBUG_FUNCTION_START("AliasDialog","CreateDialog")
	autoButton.Create(this,
					WINDOW_ID_BUTTON_AUTO,
					_T(WINDOW_ID_BUTTON_AUTO_TEXT),
					wxDefaultPosition);
	GenericOKCancelDialog::CreateDialog();
	//OnResize(event);
	STATSGEN_DEBUG_FUNCTION_END
}

void AliasDialog::OnResize(wxSizeEvent &event)
{
	wxString	msg;
	int			saveWidth;
	wxSize		itemSize;

	wxPoint		itemPosition;
	STATSGEN_DEBUG_FUNCTION_START("AliasDialog","OnResize")

	GenericOKCancelDialog::OnResize(event);

	STATSGEN_DEBUG(DEBUG_ALWAYS,"Generic Resize Done")
	// Auto button
	itemSize=quitButton.GetSize();
	itemPosition=quitButton.GetPosition();
	itemPosition.x+=(itemSize.GetWidth()+BUTTON_WIDTH_GAP);
	STATSGEN_DEBUG_CODE(msg.Printf("AUTO POSITION X=[%d] Y=[%d]",itemPosition.x,itemPosition.y);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	itemSize=autoButton.GetSize();
	STATSGEN_DEBUG_CODE(msg.Printf("AUTO SIZE=[%d] HEIGHT=[%d]",itemSize.GetWidth(),itemSize.GetWidth());)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	autoButton.SetPosition(itemPosition);

	STATSGEN_DEBUG_FUNCTION_END
}

void AliasDialog::OnAuto(wxCommandEvent &event)
{
	wxString			msg;
	wxArrayInt			shownPlayers;
	PlayerCacheEntry	cacheEntry;
	int					cacheIndex;
	int					shownIndex;
	int					shownCount;
	ArrayOfPlayerCacheEntry	shownPlayerCache;
	wxString			parentGUID;
	wxString			parentName;
	wxString			childGUID;
	wxString			childName;
	AliasListEntry		aliasEntry;

	panel->GetShownPlayers(shownPlayers);
	shownCount=shownPlayers.GetCount();
	for (shownIndex=0;shownIndex<shownCount;shownIndex++)
	{
		cacheIndex=shownPlayers.Item(shownIndex);
		cacheEntry=globalStatistics.playerCache.Item(cacheIndex);
		shownPlayerCache.Add(cacheEntry);
	}
	shownPlayerCache.Sort(PlayerCacheEntry::GUIDCompare);
	parentGUID="";
	parentName="";

	for (shownIndex=0;shownIndex<shownCount;shownIndex++)
	{
		cacheEntry=shownPlayerCache.Item(shownIndex);
		childGUID=cacheEntry.guid;
		childName=cacheEntry.name;
		if (parentGUID.Cmp(childGUID)!=0)
		{
			// this guid is different from the current parent
			// so it must be a new parent
			parentGUID=childGUID;
			parentName=childName;
		}
		// We need to add an alias for the parent and child now
		aliasEntry.primaryGUID=parentGUID;
		aliasEntry.primaryName=parentName;
		aliasEntry.aliasGUID=childGUID;
		aliasEntry.aliasName=childName;
		globalStatistics.AddAlias(aliasEntry);
		
	}

	panel->RefreshAliasTree();
	// TEMP FOR TEST
	//globalStatistics.WriteAliasList();
	//globalStatistics.configData.CommitChanges();
	// TEMP FOR TEST

}

void AliasDialog::OnSave(wxCommandEvent &event)
{
	wxString			msg;

	// Write the Alias List to disk
	globalStatistics.WriteAliasList();
	// Do any standard Save
	GenericOKCancelDialog::OnSave(event);
}

