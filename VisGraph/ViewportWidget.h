#ifndef VIEWPORT_WIDGET_H
#define VIEWPORT_WIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#include <QGLWidget>
#endif
#include <QtGui>
#include "DisplayWidget.h"
#include <map>
#include <string>

typedef std::map<std::string, DisplayWidget*> DisplayMap;
#define foreach_widget(it) for(DisplayMap::iterator (it)=children.begin(); (it)!=children.end(); ++(it))

class ViewportWidget : public QGLWidget{
	Q_OBJECT
public:
	ViewportWidget(QWidget *parent = 0);
	bool addChild(DisplayWidget *w, std::string name);
	bool removeChild(const std::string& name);
	bool isReady(){return bReady;}
	bool isCtrlPressed(){return bCtrlPressed;}
	void linkRotation(const std::string& name1, const std::string& name2);
	void linkTranslation(const std::string& name1, const std::string& name2);
	void linkCameraDistance(const std::string& name1, const std::string& name2);
	void drawText(const vec2f& pos, const std::string& str, const float& font_size, const float& font_weight, const vec4f& color = makeVec4f(0.0f,0.0f,0.0f,1.0f));
	void updateCursorShape();
	void createPixmap(const std::string& path, const std::string& name);
	void drawPixmap(const vec2f& pos, const vec2f& size, const std::string& name);

public slots:
	void menuCallback();

protected:
	void paintGL();
	void initializeGL();
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

private:
	MouseEvent convertEvent(QMouseEvent *e);
	void createMenu(QMouseEvent *e);
	DisplayWidget* getWidget(const vec2f& pos);
	bool saveImage();

	DisplayMap children;
	DisplayWidget *focusedWidget;
	DisplayWidget *menuWidget;
	DisplayWidget *mouseOverWidget;
	bool bReady;
	bool bCtrlPressed;

	std::map<std::string,int> mPixmaps;

	CursorShapeType mCurrentCursorShape;

signals:
	void finishInit();
};

#endif //VIEWPORT_WIDGET_H