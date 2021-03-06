﻿#pragma once
#include <QObject>

#include "Web/Definitions.h"

#include "Definitions.h"

class ActionController
	: public QObject
{
	Q_OBJECT
public:
	ActionController(QObject * parent = nullptr);
	virtual ~ActionController() = default;

	void init(MainController* mainController, MainModel* mainModel, MainView* mainView, SimulationViewWidget* visualEditor
		, Serializer* serializer, InfoController* infoController, DataEditController* dataEditor, ToolbarController* toolbar
		, MonitorController* monitor, DataRepository* repository, Notifier* notifier, WebSimulationController* websimController);

    void close();

	ActionHolder* getActionHolder();

private:
	Q_SLOT void onNewSimulation();
    Q_SLOT void onWebSimulation(bool toogled);
    Q_SLOT void onSaveSimulation();
	Q_SLOT void onLoadSimulation();
	Q_SLOT void onRunClicked(bool toggled);
	Q_SLOT void onStepForward();
	Q_SLOT void onStepBackward();
	Q_SLOT void onMakeSnapshot();
	Q_SLOT void onRestoreSnapshot();
    Q_SLOT void onAcceleration(bool toggled);
    Q_SLOT void onSimulationChanger(bool toggled);

	Q_SLOT void onConfigureGrid();
	Q_SLOT void onEditSimulationParameters();
	Q_SLOT void onEditSymbolTable();

	Q_SLOT void onToggleEditorMode(bool toggled);
    Q_SLOT void onToggleMonitor(bool toggled);
	Q_SLOT void onZoomInClicked();
	Q_SLOT void onZoomOutClicked();
    Q_SLOT void onToggleDisplayLink(bool toggled);
    Q_SLOT void onToggleFullscreen(bool toogled);
    Q_SLOT void onToggleGlowEffect(bool toogled);

	Q_SLOT void onNewCell();
	Q_SLOT void onNewParticle();
	Q_SLOT void onCopyEntity();
	Q_SLOT void onDeleteEntity();
	Q_SLOT void onPasteEntity();
	Q_SLOT void onNewToken();
	Q_SLOT void onCopyToken();
	Q_SLOT void onPasteToken();
	Q_SLOT void onDeleteToken();
	Q_SLOT void onToggleCellInfo(bool show);
	Q_SLOT void onCenterSelection(bool show);
    Q_SLOT void onCopyToClipboard();
    Q_SLOT void onPasteFromClipboard();

	Q_SLOT void onNewRectangle();
	Q_SLOT void onNewHexagon();
	Q_SLOT void onNewParticles();
	Q_SLOT void onLoadCollection();
	Q_SLOT void onSaveCollection();
	Q_SLOT void onCopyCollection();
	Q_SLOT void onPasteCollection();
	Q_SLOT void onDeleteSelection();
	Q_SLOT void onDeleteCollection();
	Q_SLOT void onRandomMultiplier();
	Q_SLOT void onGridMultiplier();

    Q_SLOT void onMostFrequentCluster();

	Q_SLOT void onShowAbout();
    Q_SLOT void onToggleGettingStarted(bool toggled);
	Q_SLOT void onShowDocumentation();

	Q_SLOT void onToggleRestrictTPS(bool toggled);

	Q_SLOT void receivedNotifications(set<Receiver> const& targets);

private:
	void settingUpNewSimulation(SimulationConfig const& config);
	void updateActionsEnableState();
    void setPixelOrVectorView();

	ActionModel* _model = nullptr;
	MainController* _mainController = nullptr;
	MainModel* _mainModel = nullptr;
	MainView* _mainView = nullptr;
	DataRepository* _repository = nullptr;
	Notifier* _notifier = nullptr;
	Serializer* _serializer = nullptr;

	SimulationViewWidget* _simulationViewWidget = nullptr;
	DataEditController* _dataEditor = nullptr;
	InfoController* _infoController = nullptr;
	ToolbarController* _toolbar = nullptr;
	MonitorController* _monitor = nullptr;
	NumberGenerator* _numberGenerator = nullptr;
    WebSimulationController* _webSimController = nullptr;
};
