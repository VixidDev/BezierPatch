//////////////////////////////////////////////////////////////////////
//
//	University of Leeds
//	COMP 5812M Foundations of Modelling & Rendering
//	User Interface for Coursework
//
//	October, 2024
//
//  -----------------------------
//  Bezier Patch Render Widget
//  -----------------------------
//
//	Provides a widget that displays a fixed image
//	Assumes that the image will be edited (somehow) when Render() is called
//  
////////////////////////////////////////////////////////////////////////

// include guard
#ifndef BEZIER_PATCH_RENDER_WIDGET_H
#define BEZIER_PATCH_RENDER_WIDGET_H

#include <vector>

// include the relevant QT headers
#include <QOpenGLWidget>
#include <QMouseEvent>

// and include all of our own headers that we need
#include "ControlPoints.h"
#include "RenderParameters.h"
#include "RGBAImage.h"


// class for a render widget with arcball linked to an external arcball widget
class BezierPatchRenderWidget : public QOpenGLWidget
    { // class BezierPatchRenderWidget
	Q_OBJECT
	private:	
    // the Bezier patch control points to be rendered
    ControlPoints *patchControlPoints;

	// the render parameters to use
	RenderParameters *renderParameters;

    // An image to use as a framebuffer ...
    // ... that we will set individual pixels to
	RGBAImage frameBuffer;

	// Projection matrix
	Matrix4 projectionMatrix;
	// View matrix
	Matrix4 viewMatrix;
	// Model matrix
	Matrix4 modelMatrix;
	// Translation matrix
	Matrix4 translationMatrix;

	public:
	// constructor
    BezierPatchRenderWidget
			(
            // the Bezier patch control points to show
            ControlPoints 		*newPatchControlPoints,
			// the render parameters to use
			RenderParameters 	*newRenderParameters,
			// parent widget in visual hierarchy
			QWidget 			*parent
			);
	
	// destructor
    ~BezierPatchRenderWidget();

	void drawLine(Point3 start, Point3 end, RGBAValue colour);
	void drawPoint(Point3 point);
			
	protected:
	// called when OpenGL context is set up
	void initializeGL();
	// called every time the widget is resized
	void resizeGL(int w, int h);
	// called every time the widget needs painting
	void paintGL();

	// mouse-handling
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

    private:

    void forceRepaint();

	signals:
	// these are general purpose signals, which scale the drag to 
	// the notional unit sphere and pass it to the controller for handling
	void BeginScaledDrag(int whichButton, float x, float y);
	// note that Continue & End assume the button has already been set
	void ContinueScaledDrag(float x, float y);
	void EndScaledDrag(float x, float y);

    }; // class BezierPatchRenderWidget

#endif
