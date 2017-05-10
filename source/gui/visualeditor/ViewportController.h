#ifndef VIEWPORTCONTROLLER_H
#define VIEWPORTCONTROLLER_H

#include <QMatrix>
#include "ViewportInfo.h"

enum class ActiveScene {
	PixelScene, ShapeScene
};

class ViewportController
	: public ViewportInfo
{
	Q_OBJECT
public:
	ViewportController(QObject* parent = nullptr) : ViewportInfo(parent) {}
	virtual ~ViewportController() = default;

	virtual void init(QGraphicsView* view, QGraphicsScene* pixelScene, QGraphicsScene* shapeScene, ActiveScene activeScene);
	virtual void initViewMatrices();

	virtual void setActiveScene(ActiveScene activeScene);
	virtual ActiveScene getActiveScene() const;

	virtual QRectF getRect() const override;
	virtual QVector2D getCenter() const override;

	virtual void zoomIn();
	virtual void zoomOut();
	virtual qreal getZoomFactor() const;

private:
	void setSceneToView(ActiveScene activeScene);
	void saveScenePos(ActiveScene activeScene);
	void loadScenePos(ActiveScene activeScene);


	QGraphicsView* _view = nullptr;
	QGraphicsScene* _pixelScene = nullptr;
	QGraphicsScene* _shapeScene = nullptr;

	QMatrix _pixelSceneViewMatrix;
	QMatrix _shapeSceneViewMatrix;
	IntVector2D _pixelSceneScrollbarPos;
	IntVector2D _shapeSceneScrollbarPos;

	ActiveScene _activeScene = ActiveScene::PixelScene;
};

#endif // VIEWPORTCONTROLLER_H
