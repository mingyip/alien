#pragma once

#include <QDialog>

#include "ModelBasic/Definitions.h"
#include "ModelBasic/SimulationParameters.h"

#include "Definitions.h"

namespace Ui {
	class NewSimulationDialog;
}

class SimulationParametersDialog;
class SymbolTableDialog;
class NewSimulationDialog : public QDialog
{
	Q_OBJECT

public:
	NewSimulationDialog(SimulationParameters const& parameters, SymbolTable const* symbols, Serializer* serializer, QWidget* parent = nullptr);
	virtual ~NewSimulationDialog();

	optional<SimulationConfig> getConfig() const;
	optional<double> getEnergy() const;

private:
	SymbolTable* getSymbolTable() const;
	SimulationParameters const& getSimulationParameters() const;

private:
	Q_SLOT void simulationParametersButtonClicked();
	Q_SLOT void symbolTableButtonClicked();
	Q_SLOT void okClicked();

private:
	Ui::NewSimulationDialog *ui;
	Serializer* _serializer = nullptr;

	SimulationParameters _parameters;
	SymbolTable* _symbolTable = nullptr;

	IntVector2D _universeSizeForModelGpu;
	IntVector2D _gridSize;
};

