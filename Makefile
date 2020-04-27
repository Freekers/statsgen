WXWIDGETS_BASE=../../wxWidgets-3.1.1
WXWIDGETS_BUILD=${WXWIDGETS_BASE}/build
SQLITE_BASE=../../sqlite-autoconf-3260000
SQLITE_BUILD=${SQLITE_BASE}/build

DEBUG_FLAG=-DDEBUG

STATSGEN_WEBSITE_ADDRESS="www.statsgen.co.uk"
STATSGEN_WEBSITE_PORT="80"

#STATSGEN_WEBSITE_ADDRESS="localhost"
#STATSGEN_WEBSITE_PORT="8000"

STATSGEN_COMPILER_OPTIONS=${DEBUG_FLAG} -DSTATSGEN_WEBSITE_ADDRESS=\"${STATSGEN_WEBSITE_ADDRESS}\" -DSTATSGEN_WEBSITE_PORT=${STATSGEN_WEBSITE_PORT} -static-libgcc -static-libstdc++
APP=statsgen2.exe
OBJECTS=AKAListEntry.o ActionData.o AliasCacheEntry.o AliasDialog.o AliasEditorPanel.o AliasListEntry.o AwardDefinition.o AwardEditorPanel.o BaseConfigItemGui.o BooleanConfigItemGUI.o BoxedDropDown.o COD5LogFileReader.o CODBanFile.o Cheater.o Clan.o ClanEditorPanel.o ConfigData.o ConfigEditorGUI.o ConfigItem.o ContentTypes.o DefaultTemplate1.o DefaultTemplate2.o DirectoryConfigItemGUI.o DropList.o DropListDialog.o DropListEditorPanel.o DropListEntry.o DynamicArrays.o ETLogFileReader.o ErrorData.o ErrorPanel.o FTPBrowserPanel.o FTPConnectionPanel.o FileConfigItemGUI.o GUITriggerItem.o GUITriggerList.o GenericConfigGUI.o GenericConfigPanel.o GenericKey.o GenericNumberedConfigGUI.o GenericNumberedConfigPanel.o GenericOKCancelDialog.o GroupedConfigItemsPanel.o ImagePanel.o ImageTypePanel.o KeyedIndex.o KillData.o KillTotal.o KillTotalLists.o LogFile.o LogFileReader.o MOHAALogFileReader.o MessageCentre.o MultiFile.o PerformUpgradeDialog.o Player.o PlayerCacheEntry.o PlayerDataDialog.o PlayerDataEditorPanel.o PlayerDataEntry.o PlayerDataList.o PlayerInRound.o PlayerList.o PriorityPanel.o Progress.o ProgressPanel.o PunkbusterBanFile.o QuakeWarsLogFile.o QuakeWarsLogFileReader.o RemoteDirectoryConfigItemGUI.o RemoteFileConfigItemGUI.o Round.o ScheduleConfigItemGUI.o ScheduleDialog.o ScheduleEditorPanel.o ScheduleItem.o SelectionConfigItemGUI.o SelectionFreeFormConfigItemGUI.o Server.o ServerTypePanel.o SkillTypePanel.o SpeechData.o StaticFunctions.o StatsgenDatabase.o StatsgenFrame.o StatsgenLog.o StatsgenWeb.o StatusPanel.o StreakData.o TeamLossData.o TeamWinData.o TemplateFile.o TemplateOpenFile.o TemplateOpenQuery.o TemplateVariable.o TemplateVariableField.o TextConfigItemGUI.o TextListDialog.o TextListEditorPanel.o WeaponGroupDialog.o WebFile.o WebServer.o WebServerRequestHandler.o Website.o WebsiteConfigGUI.o WeightList.o WeightListEntry.o XPPoints.o XPTotals.o PlayerCachePanel.o GlobalStatistics.o ExternalDatabase.o RemoteMachine.o RestartingFTP.o StatsgenApp.o GUIWizards.o ResizingListCtrl.o
HEADERS=AKAListEntry.h ActionData.h AliasCacheEntry.h AliasDialog.h AliasEditorPanel.h AliasListEntry.h AwardDefinition.h AwardEditorPanel.h BaseConfigItemGUI.h BooleanConfigItemGUI.h BoxedDropDown.h COD5LogFileReader.h CODBanFile.h Cheater.h Clan.h ClanEditorPanel.h ConfigData.h ConfigEditorGUI.h ConfigItem.h ContentTypes.h DirectoryConfigItemGUI.h DropList.h DropListDialog.h DropListEditorPanel.h DropListEntry.h DynamicArrays.h ETLogFileReader.h ErrorData.h ErrorPanel.h FTPBrowserPanel.h FTPConnectionPanel.h FileConfigItemGUI.h GUITriggerItem.h GUITriggerList.h GUIWizards.h GenericConfigGUI.h GenericConfigPanel.h GenericKey.h GenericNumberedConfigGUI.h GenericNumberedConfigPanel.h GenericOKCancelDialog.h GlobalStatistics.h GroupedConfigItemsPanel.h ImagePanel.h ImageTypePanel.h KeyedIndex.h KillData.h KillTotal.h KillTotalLists.h LogFile.h LogFileReader.h MOHAALogFileReader.h MessageCentre.h MultiFile.h PerformUpgradeDialog.h Player.h PlayerCacheEntry.h PlayerCachePanel.h PlayerDataDialog.h PlayerDataEditorPanel.h PlayerDataEntry.h PlayerDataList.h PlayerInRound.h PlayerList.h PriorityPanel.h Progress.h ProgressPanel.h PunkbusterBanFile.h QuakeWarsLogFile.h QuakeWarsLogFileReader.h RemoteDirectoryConfigItemGUI.h RemoteFileConfigItemGUI.h RemoteMachine.h RestartingFTP.h Round.h ScheduleConfigItemGUI.h ScheduleDialog.h ScheduleEditorPanel.h ScheduleItem.h SelectionConfigItemGUI.h SelectionFreeFormConfigItemGUI.h Server.h ServerTypePanel.h SkillTypePanel.h SpeechData.h StaticFunctions.h StatsgenApp.h StatsgenDatabase.h StatsgenFrame.h StatsgenLog.h StatsgenWeb.h StatusPanel.h StreakData.h TeamLossData.h TeamWinData.h TemplateFile.h TemplateOpenFile.h TemplateOpenQuery.h TemplateVariable.h TemplateVariableField.h TextConfigItemGUI.h TextListDialog.h TextListEditorPanel.h Version.h WeaponGroupDialog.h WebFile.h WebServer.h WebServerRequestHandler.h Website.h WebsiteConfigGUI.h WeightList.h WeightListEntry.h WindowIDs.h XPPoints.h XPTotals.h statsgen2_private.h ExternalDatabase.h
RESOURCE_OBJECTS=bitmaps.o

WXCONFIG=${WXWIDGETS_BUILD}/wx-config
SQLITE_OBJECTS=${SQLITE_BUILD}/sqlite3.o
SQLITE_INCLUDE_DIR=${SQLITE_BASE}
WX_LIBS=`${WXCONFIG} --libs`
WX_INCS=`${WXCONFIG} --cppflags`

COMPILER=g++ 
COMPILER_OPTIONS=${WX_INCS} -I. -g -I${SQLITE_INCLUDE_DIR} ${STATSGEN_COMPILER_OPTIONS}

LINKER=g++
LINKER_OPTIONS=${WX_LIBS}

RESOURCE_COMPILER=windres.exe
RESOURCE_COMPILER_OPTIONS=-O coff
RESOURCE_OBJECTS=bitmaps.o maindetails.o

.SUFFIXES: .cpp .o
.SUFFIXES: .rc .o
.h.cpp:
all:${APP}

clean:
	@rm ${OBJECTS}
	@rm ${APP}

${APP}:${OBJECTS} ${RESOURCE_OBJECTS}
	${LINKER} -o ${APP} ${OBJECTS} ${SQLITE_OBJECTS} ${RESOURCE_OBJECTS} ${LINKER_OPTIONS}
	#cp ${APP} testrun
	#cp statsgen2.ini testrun

.cpp.o: ${HEADERS}
	${COMPILER} ${COMPILER_OPTIONS} -c $<


.rc.o: ${RESOURCE_OBJECTS}
	${RESOURCE_COMPILER} ${RESOURCE_COMPILER_OPTIONS} -i $< -o $@


