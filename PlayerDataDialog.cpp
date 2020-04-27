// Statsgen Includes
#include "PlayerDataDialog.h"
#include "PlayerDataList.h"
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"

PlayerDataDialog::PlayerDataDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				PlayerDataList *playerDataListIn,
				wxString &defaultPlayerData,
				wxString &playerDataTitle) : GenericOKCancelDialog
											(parent, 
											id,
											title,
											pos,
											size,
											style,
											name)
{
	playerDataList=playerDataListIn;
	panel=new PlayerDataEditorPanel(playerDataList,
									defaultPlayerData,
									playerDataTitle);
	panel->Create(this,
				-1,
				wxDefaultPosition,
				wxDefaultSize);
}

bool PlayerDataDialog::DisplayDialog()
{
	return (GenericOKCancelDialog::DisplayDialog((wxPanel *)panel));
}

PlayerDataDialog::~PlayerDataDialog()
{
}

void PlayerDataDialog::OnSave(wxCommandEvent &event)
{
	playerDataList->WriteToFile();
	globalStatistics.configData.CommitChanges();
	GenericOKCancelDialog::OnSave(event);
}

void PlayerDataDialog::OnQuit(wxCommandEvent &event)
{
	globalStatistics.configData.RollbackChanges();
	GenericOKCancelDialog::OnQuit(event);
}

