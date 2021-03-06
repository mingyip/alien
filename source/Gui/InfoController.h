﻿#pragma once
#include <QLabel>

#include "Definitions.h"

class InfoController
	: public QObject
{
	Q_OBJECT

public:
	InfoController(QObject * parent = nullptr);
	~InfoController() = default;

	void init(QLabel* infoLabel, MainController* mainController);

	void increaseTimestep();
	void setZoomFactor(double factor);

    enum class Rendering
    { Pixel, Vector, Item};
    void setRendering(Rendering value);

private:
	Q_SLOT void oneSecondTimerTimeout();

	void updateInfoLabel();

	QTimer* _oneSecondTimer = nullptr;
	QLabel* _infoLabel = nullptr;
	MainController* _mainController = nullptr;
	int _tpsCounting = 0;
	int _tps = 0;
	double _zoomFactor = 4.0;
    Rendering _rendering = Rendering::Vector;
};
