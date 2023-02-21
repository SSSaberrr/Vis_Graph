#include "VisGraphRenderWidget.h"
#include "DataManager.h"
#include "MessageCenter.h"
#include <fstream>
#include <time.h>
#include "VolumeData.h"
#include "VisibilityGraph.h"
#include "ColorMap.h"

VisGraphStreamlineRenderWidget::VisGraphStreamlineRenderWidget(int x, int y, int w, int h,
	std::string name, VisibilityGraphDataManager* data)
	:DisplayWidget(x, y, w, h, name),
	mData(data),
	mStreamlineId(0)
{
	//setStreamline(0);
	setStreamlineAll();
}

void VisGraphStreamlineRenderWidget::setStreamlineAll() {
	std::vector<int> sids;
	for (int i = 0; i < mData->mTsneColorIds.size(); ++i) {
		sids.push_back(i);
	}
	setStreamlines(sids);
}

void VisGraphStreamlineRenderWidget::setStreamlines(std::vector<int> sids)
{
	mStreamlineSegments.clear();
	vec4f context_color = makeVec4f(ColorMap::getColorByName(ColorMap::Lavender_gray).xyz, 0.015f);

	mData->mRenderer->updateTubeColors(context_color);
	for (const int& sid : sids) {
		vec4f select_color = ColorMap::getLinearColor(mData->mTsneColorIds[sid], COLOR_MAP_D3);
		if (mData->mTsneColorIds[sid] == 7)
			select_color.a = 0.015f;
		mData->mRenderer->updateStreamlineColor(sid, select_color);
	}
}

void VisGraphStreamlineRenderWidget::setStreamline(const int& sid)
{
	mStreamlineId = sid;
	mStreamlineSegments.clear();

	if (sid < 0) {
		vec4f select_color = makeVec4f(ColorMap::getColorByName(ColorMap::Cambridge_Blue).xyz, 0.6f);
		mData->mRenderer->updateTubeColors(select_color);
	}
	else {
		vec4f context_color = makeVec4f(ColorMap::getColorByName(ColorMap::Lavender_gray).xyz, 0.015f);
		vec4f select_color = makeVec4f(ColorMap::getColorByName(ColorMap::Brandeis_blue).xyz, 1.0f);
		mData->mRenderer->updateTubeColors(context_color);
		mData->mRenderer->updateStreamlineColor(sid, select_color);

		mData->mRenderer->updateTubeRadius(0.2);
	}
}

void VisGraphStreamlineRenderWidget::init() {
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	float ambient = 0.5f;
	float diffuse = 0.5f;
	float specular = 0.5f;

	float lightDist = 50000.0f;

	GLfloat light0_ambient[4] = { ambient, ambient, ambient, 1.0f };
	GLfloat light0_diffuse[4] = { diffuse, diffuse, diffuse, 1.0f };
	GLfloat light0_specular[4] = { specular, specular, specular, 1.0f };
	GLfloat light0_position[4] = { 0.0f, 0.0f, lightDist, 1.0f };
	GLfloat light0_direction[3] = { -1.0f, -1.0f, -1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	GLfloat mat_ambient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_diffuse[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_specular[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 4.0f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
}

void VisGraphStreamlineRenderWidget::display() {
	//GL_CHECK();
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glClear(GL_ACCUM_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glAccum(GL_ACCUM, 0.5);
	GLint viewport[4];
	int jitter;
	glGetIntegerv(GL_VIEWPORT, viewport);
	for (jitter = 0; jitter < ACSIZE; jitter++) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		accPerspective(60.0,
			(GLdouble)viewport[2] / (GLdouble)viewport[3],
			1.0, 5000.0, j8[jitter].x, j8[jitter].y, 0.0, 0.0, 1.0);
		singleStepJitterDisplay();
		glAccum(GL_ACCUM, 0.5 / ACSIZE);
	}
	glAccum(GL_ADD, -0.5);
	glAccum(GL_RETURN, 2.0);

	glPopAttrib();
}

void VisGraphStreamlineRenderWidget::singleStepJitterDisplay() {
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vec3f vol_size = makeVec3f(mData->mDim);
	float cam_dist = vol_size.x + vol_size.y + vol_size.z + getCameraDistance() / 16.0f;
	gluLookAt(0.0f, 0.0f, cam_dist, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vec2f translate = getTranslation();
	glTranslatef(translate.x * 0.3f * cam_dist, translate.y * 0.3f * cam_dist, 0.0f);
	float* rotMat = getRotationMatrix();
	glMultMatrixf(rotMat);
	glTranslatef(-0.5f * vol_size.x, -0.5f * vol_size.y, -0.5f * vol_size.z);

	static int prev_view_id = -1;
	if (prev_view_id != getViewIndex()) {
		updateOpenGLMVPMatrices();
		prev_view_id = getViewIndex();
		mData->mRenderer->sortQuadByDepth(getModelViewMat(), getProjectMat());
	}

	//if (!mStreamlineSegments.empty()) {
	//	drawStreamlineSegments();
	//} else if (mStreamlineId < 0) {
	//	drawStreamlines();
	//} else {
	//	drawStreamline(mStreamlineId);
	//}

	mData->mRenderer->enableRenderProgram();
	drawStreamlines();

	glEnable(GL_DEPTH_TEST);

	//drawBoxFrame(0.5f*vol_size, vol_size, 1.0f, makeVec4f(0.0f,0.0f,0.0f,1.0f));

	glPopAttrib();
}

void VisGraphStreamlineRenderWidget::drawStreamlineSegments() {
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	for (int i = 0; i < mStreamlineSegments.size() - 2; ++i) {
		const StreamlineSegment& seg = mStreamlineSegments[i];
		mData->mRenderer->drawStreamlineSegment(seg.streamline_id, seg.segment, ColorMap::getD3Color(0));
	}

	if (mStreamlineSegments.size() < 2) return;

	vec4f highlight_color = ColorMap::getColorByName(ColorMap::Harvard_crimson);

	glDepthFunc(GL_LEQUAL);
	const StreamlineSegment& selected_segment = mStreamlineSegments[mStreamlineSegments.size() - 2];
	mData->mRenderer->drawStreamlineSegment(selected_segment.streamline_id, selected_segment.segment, highlight_color);

	std::vector<vec3f> points;
	const StreamlineSegment& selected_point = mStreamlineSegments[mStreamlineSegments.size() - 1];
	int global_point_id = mData->getGlobalPointId(selected_point.streamline_id, selected_point.segment.lower);

	drawSphere(mData->getPoint(global_point_id), 3.0f, highlight_color);

	//draw binormal curves
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glLineWidth(5.0f);
	//draw normal curve
	glColor(ColorMap::getD3Color(2));
	mData->getNormalCurveAtPoint(points, global_point_id);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points.size(); ++i) {
		glVertex(points[i]);
	}
	glEnd();
	//draw binormal curve
	mData->getBinormalCurveAtPoint(points, global_point_id);
	glColor(ColorMap::getD3Color(1));
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points.size(); ++i) {
		glVertex(points[i]);
	}
	glEnd();

	glPopAttrib();
}

void VisGraphStreamlineRenderWidget::drawStreamlines() {
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	mData->mRenderer->drawAllStreamline();

	glPopAttrib();
}

void VisGraphStreamlineRenderWidget::drawStreamline(const int& sid) {
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	mData->mRenderer->drawSingleStreamline(sid, ColorMap::getD3Color(0));

	glPopAttrib();
}

void VisGraphStreamlineRenderWidget::mouseReleaseEvent(const MouseEvent& e) {
	if (e.button == MOUSE_LEFT_BUTTON) {
		static bool* streamline_marks = new bool[mData->getNumStreamlines()];
		memset(streamline_marks, 1, sizeof(bool) * mData->getNumStreamlines());
		vec2f normalized_click = normalizeCoord(e.pos);
		StreamlineClickInfo info = cudaComputeClickStreamline(normalized_click,
			mData->getDevicePoints(), mData->getDeviceStreamlines(), mData->getNumPoints(), mData->getNumStreamlines(),
			streamline_marks, getModelViewMat(), getProjectMat());
		if (info.sid == -1)
			return;
		DataManager::sharedManager()->setIntValue(SELECTED_LINE_ID_NAME, info.sid);
	}
	else {
		DisplayWidget::mouseReleaseEvent(e);
		if (e.button == MOUSE_RIGHT_BUTTON && !DisplayWidget::isInRotation()) {
			if (!DisplayWidget::loadViewParameters())
			{
				DisplayWidget::saveViewParameters();
			}
		}
	}
}

void VisGraphStreamlineRenderWidget::onDataItemChanged(const std::string& name) {
	DataManager* manager = DataManager::sharedManager();
	bool b_success;
}

/*QWidget* VisGraphStreamlineRenderWidget::createPanel(QWidget* parent) {
	std::vector<std::string> pars;
	pars.push_back(mLineRadiusName);
	QWidget* ret = DataManager::sharedManager()->createInterface("Parameters", pars, parent);
	return ret;
}*/