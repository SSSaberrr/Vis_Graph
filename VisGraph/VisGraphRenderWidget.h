#ifndef SIMPLE_STREAMLINE_RENDER_WIDGET_H
#define SIMPLE_STREAMLINE_RENDER_WIDGET_H

#include "DisplayWidget.h"
//#include "StreamlinePool3d.h"
#include "typeOperation.h"
#include "DataUser.h"
#include <string>
#include <vector>
#include "VolumeData.h"

class cudaStreamlineRenderer;
class VisibilityGraphDataManager;

class VisGraphStreamlineRenderWidget : public DisplayWidget, public DataUser {
public:
	VisGraphStreamlineRenderWidget(int x, int y, int w, int h, std::string name, VisibilityGraphDataManager* data);

	void setStreamlineAll();

	void setStreamlines(std::vector<int> sids);

	void setStreamline(const int& sid);
	void setStreamlineSegments(const std::vector<StreamlineSegment>& segments) {
		mStreamlineSegments.assign(segments.begin(), segments.end());
	}
	QWidget* createPanel(QWidget* parent);

protected:
	void init();
	void display();

	void menuCallback(const std::string& message) {}
	void onDataItemChanged(const std::string& name);

	//void mousePressEvent(const MouseEvent& e);
	//void mouseMoveEvent(const MouseEvent& e);
	void mouseReleaseEvent(const MouseEvent& e);

private:
	void singleStepJitterDisplay();
	void drawStreamlineSegments();
	void drawStreamlines();
	void drawStreamline(const int& sid);

	int mStreamlineId;
	std::vector<StreamlineSegment> mStreamlineSegments;
	VisibilityGraphDataManager* mData;
};

#endif //SIMPLE_STREAMLINE_RENDER_WIDGET_H