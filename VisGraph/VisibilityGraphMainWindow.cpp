#include "VisibilityGraphMainWindow.h"
#include "DataManager.h"
#include "ViewportWidget.h"
#include "VisibilityGraphDisplayWidget.h"
#include "VisibilityGraph.h"
#include "VisGraphRenderWidget.h"
#include <QHBoxLayout>
#include <QFileDialog>

#define SINGLE_WIN_WIDTH	1000
#define SINGLE_WIN_HEIGHT	1200
#define PANEL_WIDTH		250
#define NUM_WIN_ROW		1
#define NUM_WIN_COL		2
#define GROUP_BOX_MARGIN 20
VisibilityGraphMainWindow::VisibilityGraphMainWindow(QWidget* parent, Qt::WindowFlags flags)
	:QMainWindow(parent, flags)
{
	this->setMinimumSize(SINGLE_WIN_WIDTH * NUM_WIN_COL + PANEL_WIDTH + 200, SINGLE_WIN_HEIGHT * NUM_WIN_ROW + 20);
	this->setGeometry(50, 50, SINGLE_WIN_WIDTH * NUM_WIN_COL + PANEL_WIDTH, SINGLE_WIN_HEIGHT * NUM_WIN_ROW + 20);
	ui.setupUi(this);

	mViewport = new ViewportWidget;
	mViewport->setGeometry(PANEL_WIDTH, 0, NUM_WIN_COL * SINGLE_WIN_WIDTH, NUM_WIN_ROW * SINGLE_WIN_HEIGHT);
	DataManager::sharedManager()->createPointer("View Port.Pointer", reinterpret_cast<PointerType>(mViewport));

	mLayout = new QHBoxLayout;
	mLayout->addWidget(mViewport, mViewport->width());
	ui.centralWidget->setLayout(mLayout);

	//start produce_all_samples
	connect(mViewport, SIGNAL(finishInit()), this, SLOT(start()));
}

VisibilityGraphMainWindow::~VisibilityGraphMainWindow() {
}

void VisibilityGraphMainWindow::start() {
	std::string meta_path = "D:/OneDrive - 中山大学/Project/data/Vortex/vorts/";
	VisibilityGraphDataManager* data = new VisibilityGraphDataManager(meta_path.c_str());

	//create display widgets

	mVisGraphDisplay = new VisibilityGraphDisplayWidget(0, 0, SINGLE_WIN_WIDTH, SINGLE_WIN_HEIGHT, "Graph Display", data);
	mViewport->addChild(mVisGraphDisplay, mVisGraphDisplay->getName());

	mRenderWidget = new VisGraphStreamlineRenderWidget(SINGLE_WIN_WIDTH, 0, SINGLE_WIN_WIDTH, SINGLE_WIN_HEIGHT, "Render Widget", data);
	mViewport->addChild(mRenderWidget, mRenderWidget->getName());

	/*
	//create global variables
	DataManager* manager = DataManager::sharedManager();
	manager->createInt(SELECTED_LINE_ID_NAME, 0, data->getNumStreamlines() - 1, 0, mVisGraphDisplay, true);

	//create control panels
	QWidget* mVisGraph_widget = new QWidget;
	mVisGraphPanel = mVisGraphDisplay->createPanel(mVisGraph_widget);
	mVisGraphPanel->move(0, 0);
	mVisGraphPanel->show();
	mLayout->insertWidget(0, mVisGraph_widget, mVisGraphPanel->width());

	myFlowEncoder = new FlowEncoder(SINGLE_WIN_WIDTH, 0, SINGLE_WIN_WIDTH, SINGLE_WIN_HEIGHT, "FlowEncoder", false);
	mViewport->addChild(myFlowEncoder, myFlowEncoder->getName());

	myFlowEncoder22 = new FlowEncoder(0, 0, SINGLE_WIN_WIDTH, SINGLE_WIN_HEIGHT, "FlowEncoder22", true);
	mViewport->addChild(myFlowEncoder22, myFlowEncoder22->getName());

	//create control panels
	QWidget* mRender_widget = new QWidget;
	mRenderPanel = mRenderWidget->createPanel(mRender_widget);
	mRenderPanel->move(0, mVisGraphPanel->y() + mVisGraphPanel->height() + GROUP_BOX_MARGIN);
	mRenderPanel->show();
	mLayout->insertWidget(0, mRender_widget, mRenderPanel->width());
*/
}
/*
void VisibilityGraphMainWindow::produce_all_samples() {
	std::string dir_root_path = "D:/data/project_data/VisibilityGraph/432/";
	//5cp combustion computer_room crayfish GL3D2 plume two_swirl
	std::vector<std::string> datasets = { "5cp", "combustion", "computer_room", "crayfish",
		"GL3D2", "plume", "two_swirl" };

	for (auto& dataset : datasets) {
		std::string dir_path = dir_root_path + dataset;
		VisibilityGraphDataManager* data = new VisibilityGraphDataManager(dir_path.c_str(), false);
		delete data;
	}
}
*/