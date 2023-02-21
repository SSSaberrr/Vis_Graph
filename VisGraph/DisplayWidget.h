#ifndef DISPLAY_WIDGET_H
#define DISPLAY_WIDGET_H

#include "typeOperation.h"
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#include <QGLWidget>
#endif
#include <string>
#include <map>
#include "CursorShape.h"

inline void CheckOpenGLError(const char* fname, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("OpenGL error %08x, at %s:%i\n", err, fname, line);
		abort();
	}
}

#ifdef _DEBUG
#define GL_DEBUG_CHECK() CheckOpenGLError(__FILE__, __LINE__)
#else
#define GL_DEBUG_CHECK()
#endif

#define GL_CHECK() CheckOpenGLError(__FILE__, __LINE__)

typedef enum{
	MOUSE_NO_BUTTON = 0,
	MOUSE_LEFT_BUTTON,
	MOUSE_MIDDLE_BUTTON,
	MOUSE_RIGHT_BUTTON
} MouseButtonType;

typedef struct{
	vec2f pos;
	MouseButtonType button;
} MouseEvent;

typedef struct{
	std::string name;
	bool enabled;
	vec4f color;
} MenuItem;
typedef std::vector<MenuItem> MenuItemSet;

typedef struct{
	Area area;
	std::string text;
} DisplayLabel;

inline DisplayLabel makeDisplayLabel(const vec2f& pos, const vec2f& size, std::string text){
	DisplayLabel label = {pos, size, text};
	return label;
}

class ViewportWidget;

class DisplayBase {
public:
	DisplayBase(){}
	~DisplayBase(){}
	virtual void display() = 0;
	virtual int getClickElement(const vec2f& p) = 0;
	virtual void setSelectElement(const int& eid) = 0;
	virtual void clearSelection() = 0;
};

class DisplayWidget{
public:
	DisplayWidget(int x, int y, int w, int h, std::string name);

	std::string& getName(){return mName;}

	void setViewportWidget(ViewportWidget* w){mViewport=w;}

	//menu
	MenuItemSet* getMenuItems(){return &mMenuItems;}
	void addMenuItem(std::string name, const bool& bEnabled = true, const vec4f& color = makeVec4f(0.0f, 0.0f, 0.0f, 0.0f));
	void removeMenuItem(const std::string& name);
	void clearMenuItem();

	//mouse
	CursorShapeType getCursorType();
	void setCursorShape(const CursorShapeType& shape);
	void updateCursorShape();

	void setTransferFunc(float* tf, const int& num);

	void drawBoundary(const char& lrbt_hint = 0xf);

	void setViewport();
	float* getRotationMatrix();
	float* getRotationQuat();
	void setRotationQuatRef(float* quat);
	float getCameraDistance(){return *mCameraDist;}
	float* getCameraDistanceRef(){return mCameraDist;}
	void setCameraDistanceRef(float* cam_dist);
	vec2f* getTranslationRef(){return mTranslate;}
	void setTranslationRef(vec2f* translate);
	vec2f getTranslation(){return *mTranslate;}
	vec2i getPosition(){return makeVec2i(mWinX,mWinY);}
	MouseEvent& getPreviousMouseEvent(){return mPrevMouseEvent;}
	MouseEvent& getFirstMouseEvent(){return mFirstMouseEvent;}

	void updateOpenGLMVPMatrices();
	float* getModelViewMat() { return mModelViewMat; }
	float* getProjectMat() { return mProjectMat; }
	int getViewIndex() { return mViewIndex; }

	bool saveViewParameters();
	bool loadViewParameters();
	bool saveViewParameters(const char* file_path);
	bool loadViewParameters(const char* file_path);

	virtual void init()=0;
	virtual void display()=0;
	virtual void menuCallback(const std::string& message)=0;
	void makeCurrentGLContext();

	bool isCtrlPressed();
	bool isInRotation(){return mbInRotate;}
	bool isInside(const vec2f& p);
	bool isInside(const Area& a);
	void resetInRotationStatus(){mbInRotate=false;}
	virtual void mousePressEvent(const MouseEvent& e);
	virtual void mouseMoveEvent(const MouseEvent& e);
	virtual void mouseReleaseEvent(const MouseEvent& e);
	virtual void wheelEvent(const float& delta);

	//static drawing
	static void glColor(const vec4f& color);
	static void glVertex(const vec2f& p);
	static void glVertex(const vec3f& p);
	static void drawBox(const vec3f& center, const vec3f& size, const vec4f& color);
	static void drawBoxFrame(const vec3f& center, const vec3f& size, const float& w, const vec4f& color);
	static void drawLine(const vec2f& p1, const vec2f& p2, const float& lineWidth = 2.0f, const vec4f& color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawTriangle(const vec2f& p1, const vec2f& p2, const vec4f& color, const float& lineWidth = 2.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawArrow(const vec2f& p1, const vec2f& p2, const float& arrow_size, const float& offset, const float& lineWidth, const vec4f& color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawCurve(const vec2f& p1, const vec2f& p2, const float& arc_angle = 1.0f, const float& arrow_size = 0.0f, const float& offset = 0.0f, const float& lineWidth = 1.0f, const vec4f& color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawHorizontalCurve(const vec2f& p1, const vec2f p2, const float& lineWidth, const vec4f& color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawCube(const vec3f& center, const float& len, const vec4f& color);
	static void drawSphere(const vec3f& center, const float& radius, const vec4f& color);
	static void drawCircle(const vec2f& center, const float& radius, const vec4f& color, const float& lineWidth = 2.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawDashedCircle(const vec2f& center, const float& radius, const vec4f& color, const float& lineWidth = 2.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f), const int& num_edges = 32);
	static void drawPie(const vec2f& center, const float& radius, const float& angle, const vec4f& color);
	static void drawArc(const vec2f& center, const float& inner_radius, const float& outer_radius, const float& angle, const vec4f& color, const float& lineWidth = 0.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawArc(const vec2f& center, const float& inner_radius, const float& outer_radius, const float& start_angle, const float& end_angle, const vec4f& color, const float& lineWidth = 0.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawRect(const vec2f& p1, const vec2f p2, const vec4f& color, const float& lineWidth = 2.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawRect(const RectDisplayArea& rect, const vec4f& color, const float& lineWidth = 2.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawSquare(const vec2f& center, const float& size, const vec4f& color, const float& lineWidth = 2.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawPentagon(const vec2f& center, const float& size, const vec4f& color, const float& lineWidth = 2.0f, const vec4f& line_color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	static void drawPentagonHalo(const vec2f& center, const float& size, const vec4f& color);
	static void drawRectHalo(const vec2f& p1, const vec2f p2, const vec4f& color);
	static void drawCircleHalo(const vec2f& center, const float& radius, const vec4f& color);
	static void drawSquareHalo(const vec2f& center, const float& size, const vec4f& color);
	static void drawVolume(float *data, int numx, int numy, int numz, float gridx, float gridy, float gridz);

protected:
	//display
	void redraw();
	void setViewport(float l, float r, float b, float t);

	//non-static drawing
	void drawLabel(const DisplayLabel& label, const float& font_size, const float& font_weight, const vec4f& color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	void drawText(const vec2f& pos, const std::string& str, const float& font_size, const float& font_weight, const vec4f& color = makeVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	void drawVerticalText(const vec2f& pos, const std::string& str, const float& font_size, const float& font_weight, const vec4f& color);
	void drawPixmap(const vec2f& pos, const vec2f& size, const std::string& name);

	//map position
	vec2f normalizeCoord(const vec2f& pos);

	//texture
	GLuint bindTexture(const std::string& file_name);
	void deleteTexture(const GLuint& texture);

	//color
	vec3f rgb2hsv(const vec3f& color);
	vec4f rgb2hsv(const vec4f& color);
	vec3f hsv2rgb(const vec3f& color);
	vec4f hsv2rgb(const vec4f& color);
	vec4f getColor(float v);
	vec4f getD3Color(const int& id);
	vec4f getD3ColorNoGray(const int& id);
	vec4f getPerceptualColor(float v);

	std::string openFile();
	std::string saveFile();
	vec2f getTextSize(const std::string& str, const float& font_size);
	std::string getTransposeText(const std::string& str);

	//cursor
	CursorShapeType	mCursorShape;

	void updateLabelSize(DisplayLabel& label, const float& font_size);
	void updateLabelPos(DisplayLabel& label, const vec2f& pos, const float& font_size=15.0f);
	void clampLabelPos(DisplayLabel& label, const float& font_size);
	void computeNonOverlapLabelList(std::vector<int>& ret, const std::vector<int>& label_ids, const std::vector<DisplayLabel>& labels, const float& margin = 0.0f);
	vec2f findPointOnCurve(const float& ratio, const vec2f& p1, const vec2f& p2, const float& arc_angle, const float& offset);

	//geometry
	int mWinX, mWinY, mWidth, mHeight;

private:
	//name
	std::string		mName;

	//viewport
	ViewportWidget* mViewport;

	//rotation
	bool				mbInRotate;
	float*				mRotQuat;
	float				mRotMat[4][4];

	//opengl matrices
	float mModelViewMat[16];
	float mProjectMat[16];
	int mViewIndex;

	//translation
	float*	mCameraDist;
	vec2f*	mTranslate;

	//mouse
	MouseEvent		mPrevMouseEvent;
	MouseEvent		mFirstMouseEvent;

	//Pop-up menu
	MenuItemSet		mMenuItems;

	//color map
	std::vector<vec4f>	mTransferFunc;

	//drawing parameters
	float mTextRowMargin;
};

#endif //DISPLAY_WIDGET_H