#pragma once

#include <mutex>

#include "Base/Definitions.h"
#include "ModelBasic/Definitions.h"

class QGraphicsItem;
class QGraphicsView;
class QGraphicsScene;
class QTabWidget;
class QTableWidgetItem;
class QSignalMapper;
class QAction;
class QBuffer;

class CellItem;
class ParticleItem;
class CellConnectionItem;
class ItemConfig;
class MonitorView;
class MetadataManager;
class UniverseView;
class PixelUniverseView;
class VectorUniverseView;
class ItemUniverseView;
class ItemManager;
class DataRepository;
class InfoController;
class ViewportInterface;
class ViewportController;
class MarkerItem;
class DataEditController;
class DataEditContext;
class ToolbarController;
class ToolbarContext;
class ToolbarView;
class ActionModel;
class DataEditModel;
class DataEditView;
class DataRepository;
class ClusterEditTab;
class CellEditTab;
class MetadataEditTab;
class CellComputerEditTab;
class ParticleEditTab;
class SelectionEditTab;
class SymbolEditTab;
class HexEditWidget;
class TokenEditTabWidget;
class TokenEditTab;
class Notifier;
class MainView;
class MainModel;
class MainController;
class VersionController;
class SimulationViewWidget;
class ActionHolder;
class ActionController;
class MonitorController;
class PixelImageSectionItem;
class VectorImageSectionItem;
class VectorViewport;
class PixelViewport;
class ItemViewport;

struct MonitorData;
using MonitorDataSP = boost::shared_ptr<MonitorData>;

enum class ActiveView {
    PixelScene,
    VectorScene,
    ItemScene
};
enum class Receiver { Simulation, VisualEditor, DataEditor, ActionController };
enum class UpdateDescription { All, AllExceptToken, AllExceptSymbols };
enum class NotifyScrollChanged { No, Yes };

class _SimulationConfig;
using SimulationConfig = boost::shared_ptr<_SimulationConfig>;

class WebSimulationSelectionView;
class WebSimulationSelectionController;
class WebSimulationTableModel;

class WebSimulationController;

enum class ModelComputationType
{
	Gpu = 1
};

class DataAnalyzer;
class Queue;
class GettingStartedWindow;