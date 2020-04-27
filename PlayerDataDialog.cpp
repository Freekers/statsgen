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
											style|wxFULL_REPAINT_ON_RESIZE,
											name)
{
	mPlayerDataList=playerDataListIn;
	mPanel=new PlayerDataEditorPanel(mPlayerDataList,
									defaultPlayerData,
									playerDataTitle);
	mPanel->CreateDisplay(this, wxID_ANY);

}

bool PlayerDataDialog::DisplayDialog()
{
	return (GenericOKCancelDialog::DisplayDialog((wxPanel *)mPanel));
}

PlayerDataDialog::~PlayerDataDialog()
{
}

void PlayerDataDialog::OnSave(wxCommandEvent &event)
{
	mPlayerDataList->WriteToFile();
	globalStatistics.configData.CommitChanges();
	GenericOKCancelDialog::OnSave(event);
}

void PlayerDataDialog::OnQuit(wxCommandEvent &event)
{
	globalStatistics.configData.RollbackChanges();
	GenericOKCancelDialog::OnQuit(event);
}

