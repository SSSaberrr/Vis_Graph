#pragma once

#include "typeOperation.h"
//#include "StressMajorization.h"
#include "ColorMap.h"
#include "DisplayWidget.h"
//#include "WindowsTimer.h"

template <typename T>
class GraphDisplay : public DisplayBase {
public:
	GraphDisplay() {
		mData = NULL;
		mEdgeFilter = makeRange(-1e30, 1e30);
		mEdgeNormalizeRange = makeRange(-1e30, 1e30);
		mNum = 0;
		mArea = makeRectDisplayArea(makeVec2f(0.0f, 0.0f), makeVec2f(100.0f, 0.0f), makeVec2f(0.0f, 100.0f));
		mRadius = 8.0f;
		mBorderSize = 2.0f;
		mDefaultSelectionRadius = 5.0f;
		mDefaultNodeBorderColor = makeVec4f(1.0f);
		mSelectedNodeBorderColor = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f);
		mSelectBoxColor = makeVec4f(0.1f);
		mSelectBoxBorderColor = makeVec4f(0.6f);
		mbInSelection = false;
		mBrushThresh = 5.0f;
		mSelectionRadius = 10.0f;
	}

	~GraphDisplay() {}

	void setData(MatrixData<T>* data) {
		if (data->width() != data->height()) {
			printf("Graph Display Error: distance matrix is not square.\n");
		}

		//assign data
		mNum = data->width();
		mData = data;

		initDisplay(std::vector<vec2f>());
	}

	void setEdgeNormalization(const float& domain_lower, const float& domain_upper,
		const float& range_lower, const float& range_upper)
	{
		mEdgeNormalizeDomain = makeRange(domain_lower, domain_upper);
		mEdgeNormalizeRange = makeRange(range_lower, range_upper);
	}

	void setEdgeFilter(const float& lower, const float& upper) {
		mEdgeFilter = makeRange(lower, upper);
	}

	float getEdgeNormalizedWeight(const float& w) {
		return interpolate(mEdgeNormalizeDomain.lower, mEdgeNormalizeDomain.upper,
			w, mEdgeNormalizeRange.lower, mEdgeNormalizeRange.upper);
	}

	void initDisplay(const std::vector<vec2f>& init_points) {
		if (mData == NULL) {
			mNum = init_points.size();
		}
		if (init_points.empty()) {
			mNodePositions.resize(mNum);
		}
		else {
			setNodePositions(init_points);
		}
		updateNodeColor(ColorMap::getPerceptualColor(1.0f));
		mNodeSelectedMarks.assign(mNum, false);
	}

	void setNodePositions(const std::vector<vec2f>& points) {
		mNodePositions.assign(points.begin(), points.end());
		updateNodePositionRange();
	}

	void updateNodePosition(const bool& b_rand_init) {
		//use stress majorization to update positions of nodes
		T* X_data, ** X;
		allocateMatrix(X_data, X, mNum, 2);
		if (!b_rand_init) {
			for (int i = 0; i < mNum; ++i) {
				X[0][i] = mNodePositions[i].x;
				X[1][i] = mNodePositions[i].y;
			}
		}

		int max_iter = b_rand_init ? 2000 : 200;
		float stress = stressMajorizationLocalized(mData->getMatrixPointer(), X, mNum, 2, b_rand_init, max_iter);
		if (std::isnan(stress) || std::isinf(stress)) {
			stressMajorizationLocalized(mData->getMatrixPointer(), X, mNum, 2, true, 2000);
		}

		mNodePositions.resize(mNum);
		for (int i = 0; i < mNum; ++i) {
			mNodePositions[i] = makeVec2f(X[0][i], X[1][i]);
		}

		updateNodePositionRange();

		delete[] X_data;
		delete[] X;
		updateNodeDisplayPosition();
	}

	void updateNodePositionRange() {
		if (mNodePositions.empty()) return;
		mNodePosXRange.lower = mNodePosXRange.upper = mNodePositions[0].x;
		mNodePosYRange.lower = mNodePosYRange.upper = mNodePositions[0].y;
		for (int i = 1; i < mNodePositions.size(); ++i) {
			if (mNodePositions[i].x > mNodePosXRange.upper) {
				mNodePosXRange.upper = mNodePositions[i].x;
			}
			else if (mNodePositions[i].x < mNodePosXRange.lower) {
				mNodePosXRange.lower = mNodePositions[i].x;
			}
			if (mNodePositions[i].y > mNodePosYRange.upper) {
				mNodePosYRange.upper = mNodePositions[i].y;
			}
			else if (mNodePositions[i].y < mNodePosYRange.lower) {
				mNodePosYRange.lower = mNodePositions[i].y;
			}
		}
	}

	void updateNodeColor(const vec4f& color) {
		mNodeColors.assign(mNum, color);
	}

	void updateNodeColor(const std::vector<float>& node_weight, const LinearColorMapType& color_scheme) {
		mNodeColors.resize(mNum);
		for (int i = 0; i < mNum; ++i) {
			mNodeColors[i] = ColorMap::getLinearColor(node_weight[i], color_scheme);
		}
	}

	void updateNodeColor(const std::vector<int>& node_group_ids, const LinearColorMapType& color_scheme) {
		mNodeColors.resize(mNum);
		for (int i = 0; i < mNum; ++i) {
			mNodeColors[i] = ColorMap::getLinearColor(node_group_ids[i], color_scheme);
		}
	}

	void updateNodeColorOpacity(const float& alpha) {
		for (auto& c : mNodeColors) c.a = alpha;
	}

	void updateNodeDisplayPosition() {
		mNodeDisplayPositions.resize(mNum);

		float canvas_w = length(mArea.row_axis);
		float canvas_h = length(mArea.col_axis);
		float graph_w = mNodePosXRange.upper - mNodePosXRange.lower;
		float graph_h = mNodePosYRange.upper - mNodePosYRange.lower;
		Range mapped_x, mapped_y;
		if (graph_w * canvas_h > graph_h * canvas_w) {//fit width
			float half_mapped_h = 0.5f * graph_h / graph_w * canvas_w / canvas_h;
			mapped_x = makeRange(0.0f, 1.0f);
			mapped_y = makeRange(0.5f - half_mapped_h, 0.5f + half_mapped_h);
		}
		else {
			float half_mapped_x = 0.5f * graph_w / graph_h * canvas_h / canvas_w;
			mapped_x = makeRange(0.5f - half_mapped_x, 0.5f + half_mapped_x);
			mapped_y = makeRange(0.0f, 1.0f);
		}

		for (int i = 0; i < mNum; ++i) {
			float x = interpolate(mNodePosXRange.lower, mNodePosXRange.upper, mNodePositions[i].x, mapped_x.lower, mapped_x.upper);
			float y = interpolate(mNodePosYRange.lower, mNodePosYRange.upper, mNodePositions[i].y, mapped_y.lower, mapped_y.upper);
			mNodeDisplayPositions[i] = mArea.origin + x * mArea.row_axis + y * mArea.col_axis;
		}
	}

	//draw a line between two nodes
	//not necessarily an edge
	void drawLine(const int& i, const int& j, const vec4f& color) {
		glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_LINES);
		DisplayWidget::glColor(color);
		DisplayWidget::glVertex(mNodeDisplayPositions[i]);
		DisplayWidget::glVertex(mNodeDisplayPositions[j]);
		glEnd();

		glPopAttrib();
	}

	void drawEdges() {
		if (mData == NULL) return;

		glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float** mat = mData->getMatrixPointer();
		glBegin(GL_LINES);
		for (int i = 0; i < mNum; ++i) {
			for (int j = i + 1; j < mNum; ++j) {
				if (inRange(mEdgeFilter, mat[i][j])) {
					float w = getEdgeNormalizedWeight(mat[i][j]);
					vec4f color = ColorMap::getGrayScale(w);
					color.w = w;
					DisplayWidget::glColor(color);
					DisplayWidget::glVertex(mNodeDisplayPositions[i]);
					DisplayWidget::glVertex(mNodeDisplayPositions[j]);
				}
			}
		}
		glEnd();

		glPopAttrib();
	}

	void drawNodes() {
		for (int i = 0; i < mNum; ++i) if (!mNodeSelectedMarks[i]) {
			DisplayWidget::drawCircle(mNodeDisplayPositions[i], mRadius, mNodeColors[i], mBorderSize, mDefaultNodeBorderColor);
		}
		for (int i = 0; i < mNum; ++i) if (mNodeSelectedMarks[i]) {
			DisplayWidget::drawCircle(mNodeDisplayPositions[i], mRadius, mNodeColors[i], mBorderSize, mSelectedNodeBorderColor);
		}
	}

	void drawNodes(const std::vector<int>& node_ids, const float& radius, const vec4f& color,
		const float& border_size = 1.0f, const vec4f& border_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f))
	{
		for (const int& nid : node_ids) {
			DisplayWidget::drawCircle(mNodeDisplayPositions[nid], radius, color, border_size, border_color);
		}
	}

	void drawSelectionBox() {
		if (getSelectionBoxSize() < mBrushThresh) {
			DisplayWidget::drawCircle(mSelectionAnchor, mSelectionRadius, mSelectBoxColor, 2.0f, mSelectedNodeBorderColor);
		}
		else {
			DisplayWidget::drawRect(mSelectionAnchor, mSelectionMovePoint, mSelectBoxColor, 2.0f, mSelectBoxBorderColor);
		}
	}

	void display() {
		//drawEdges();
		drawNodes();
		if (mbInSelection) {
			drawSelectionBox();
		}
	}

	int getNodeWithPos(const vec2f& p, const float& thresh) {
		int ret_id = -1;
		float closest_dist = thresh;
		for (int i = 0; i < mNodeDisplayPositions.size(); ++i) {
			float dist = length(p - mNodeDisplayPositions[i]);
			if (dist < closest_dist) {
				closest_dist = dist;
				ret_id = i;
			}
		}
		return ret_id;
	}

	int getClickElement(const vec2f& p) {
		if (!inRectDisplayArea(p, mArea)) return -1;
		return getNodeWithPos(p, mDefaultSelectionRadius);
	}

	void setSelectElement(const int& eid) {
		setSelectedNode(eid);
	}

	void getNodesInRect(std::vector<int>& ret, const Range& x_range, const Range& y_range) {
		ret.clear();
		ret.reserve(mNum);
		for (int i = 0; i < mNodeDisplayPositions.size(); ++i) {
			const vec2f& p = mNodeDisplayPositions[i];
			if (inRange(x_range, p.x) && inRange(y_range, p.y)) {
				ret.push_back(i);
			}
		}
	}

	bool isNodeIDValid(const int& nid) {
		return (nid >= 0 && nid < mNum);
	}

	inline bool inSelection() {
		return mbInSelection;
	}

	void setSelectedNode(const int& nid) {
		if (isNodeIDValid(nid)) {
			mNodeSelectedMarks[nid] = true;
		}
	}

	void setSelectedNodes(const std::vector<int>& nids) {
		for (const int& nid : nids) {
			setSelectedNode(nid);
		}
	}

	void clearSelection() {
		mNodeSelectedMarks.assign(mNum, false);
	}

	void setArea(const RectDisplayArea& area) {
		mArea = area;
		updateNodeDisplayPosition();
	}

	void setRadius(const float& r) { mRadius = r; }
	void setBorderSize(const float& s) { mBorderSize = s; }

	void setDefaultBorderColor(const vec4f& color) {
		mDefaultNodeBorderColor = color;
	}

	void setSelectedBorderColor(const vec4f& color) {
		mSelectedNodeBorderColor = color;
	}

	void setSelectionAnchor(const vec2f& p) {
		mSelectionAnchor = mSelectionMovePoint = p;
		mbInSelection = true;
	}

	void updateSelectionBox(const vec2f& p) { mSelectionMovePoint = p; }

	void finishSelection(std::vector<int>& ret) {
		Range x_range = makeRange(mSelectionAnchor.x, mSelectionMovePoint.x);
		if (x_range.upper < x_range.lower) std::swap(x_range.upper, x_range.lower);
		Range y_range = makeRange(mSelectionAnchor.y, mSelectionMovePoint.y);
		if (y_range.upper < y_range.lower) std::swap(y_range.upper, y_range.lower);
		getNodesInRect(ret, x_range, y_range);
		mbInSelection = false;
	}

	void endSelection() {
		mbInSelection = false;
	}

	float getSelectionBoxSize() {
		return length(mSelectionAnchor - mSelectionMovePoint);
	}

	void setBrushThresh(const float& t) {
		mBrushThresh = t;
	}

	void setSelectionRadius(const float& r) {
		mSelectionRadius = r;
	}

	inline bool inDisplayArea(const vec2f& p) {
		return inRectDisplayArea(p, mArea);
	}

	MatrixData<T>* getData() { return mData; }
	void freeData() { delete mData; }

	std::vector<vec2f>& getNodePositions() { return mNodePositions; }
	std::vector<vec2f>& getNodeDisplayPosition() { return mNodeDisplayPositions; }
private:
	int mNum;
	Range mEdgeNormalizeDomain;
	Range mEdgeNormalizeRange;
	Range mEdgeFilter;
	Range mNodePosXRange;
	Range mNodePosYRange;
	float mRadius;
	float mBorderSize;
	float mDefaultSelectionRadius;
	vec4f mDefaultNodeBorderColor;
	vec4f mSelectedNodeBorderColor;
	vec4f mSelectBoxColor;
	vec4f mSelectBoxBorderColor;
	RectDisplayArea mArea;
	MatrixData<T>* mData;
	std::vector<vec2f> mNodeDisplayPositions;
	std::vector<vec2f> mNodePositions;
	std::vector<vec4f> mNodeColors;
	std::vector<bool> mNodeSelectedMarks;
	//for selection
	bool mbInSelection;
	vec2f mSelectionAnchor;
	vec2f mSelectionMovePoint;
	float mBrushThresh;
	float mSelectionRadius;
};
