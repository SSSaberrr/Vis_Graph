#pragma once

#include "typeOperation.h"
#include "ColorMap.h"

class BarDisplay {
public:
	BarDisplay() {
		mBarColor = ColorMap::getColorByName(ColorMap::Harvard_crimson);
		mMargin = 0.05f;
	}

	void setData(const std::vector<Range>& data) {
		mData.assign(data.begin(), data.end());
	}

	Range getBarRange(const int& i) {
		Range ret;
		ret.lower = interpolate(mRange.lower, mRange.upper, mData[i].lower, 0.0f, 1.0f);
		ret.upper = interpolate(mRange.lower, mRange.upper, mData[i].upper, 0.0f, 1.0f);
		return ret;
	}

	void display() {
		float fac = 1.0f - 2.0f * mMargin;
		vec2f inc = (1.0f / mData.size()) * mArea.row_axis;
		RectDisplayArea area;
		area.origin = mArea.origin + mMargin * (mArea.row_axis + mArea.col_axis);
		area.row_axis = fac * inc;
		area.col_axis = fac * mArea.col_axis;
		for (int i = 0; i < mData.size(); ++i) {
			Range r = getBarRange(i);
			RectDisplayArea a = area;
			a.origin += r.lower * area.col_axis;
			a.col_axis = (r.upper - r.lower) * area.col_axis;
			DisplayWidget::drawRect(a, mBarColor, 0.0f);
			area.origin += inc;
		}
	}

	void setRange(const float& lower, const float& upper) {
		mRange = makeRange(lower, upper);
	}

	void setArea(const RectDisplayArea& area) {
		mArea = area;
	}

	void setMargin(const float& margin) {
		mMargin = margin;
	}

private:
	float mMargin;
	vec4f mBarColor;
	RectDisplayArea mArea;
	Range mRange;
	std::vector<Range> mData;
};