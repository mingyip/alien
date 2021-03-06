#pragma once

#include "Definitions.h"

class MODELBASIC_EXPORT SimulationController
	: public QObject
{
    Q_OBJECT
public:
	SimulationController(QObject* parent = nullptr) : QObject(parent) {}
	virtual ~SimulationController() = default;

    virtual bool getRun() = 0;
    virtual void setRun(bool run) = 0;
	virtual void calculateSingleTimestep() = 0;
	virtual SimulationContext* getContext() const = 0;
	virtual void setRestrictTimestepsPerSecond(optional<int> tps) = 0;
    virtual void setEnableCalculateFrames(bool enabled) = 0;

    Q_SIGNAL void nextFrameCalculated();
	Q_SIGNAL void nextTimestepCalculated();
};

