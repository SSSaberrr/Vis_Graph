#ifndef EARTHVISMAINWINDOW_H
#define EARTHVISMAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_VisGraph.h"
#include <vector>

class ViewportWidget;
class VisibilityGraphDisplayWidget;
class VisGraphStreamlineRenderWidget;
class QHBoxLayout;
class FlowEncoder;

class VisibilityGraphMainWindow : public QMainWindow
{
	Q_OBJECT
public slots:
	void start();
	void produce_all_samples();
public:
	VisibilityGraphMainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~VisibilityGraphMainWindow();

private:
	//Ui::FlowEncoderClass ui;
	QHBoxLayout* mLayout;

	ViewportWidget* mViewport;
	FlowEncoder* myFlowEncoder;

	FlowEncoder* myFlowEncoder22;

	VisibilityGraphDisplayWidget* mVisGraphDisplay;
	VisGraphStreamlineRenderWidget* mRenderWidget;
	QWidget* mVisGraphPanel;
	QWidget* mRenderPanel;
};

#endif // EARTHVISMAINWINDOW_H
