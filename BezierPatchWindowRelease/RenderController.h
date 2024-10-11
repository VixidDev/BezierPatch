/////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  October, 2024
//
//  -----------------------------
//  Render Controller
//  -----------------------------
//
//  We're using the Model-View-Controller pattern
//  so most of the control logic goes here
//  which means we need a slot for substantially
//  every possible UI manipulation
//
/////////////////////////////////////////////////////////////////

// include guard
#ifndef RENDER_CONTROLLER_H
#define RENDER_CONTROLLER_H

#include <vector>

// QT headers
#include <QtGui>
#include <QMouseEvent>
#include <QKeyEvent>

// Local headers
#include "RenderWindow.h"
#include "ControlPoints.h"
#include "RenderParameters.h"

// class for the render controller
class RenderController : public QObject
    { // class RenderController
    Q_OBJECT
    private:
    // the Bezier patch control points to be rendered
    ControlPoints* patchControlPoints;

    // the render parameters to use
    RenderParameters *renderParameters;

    // the render window with all of the controls
    RenderWindow *renderWindow;

    // local variable for tracking mouse-drag in shared widgets
    int dragButton;

    public:
    // constructor
    RenderController
        (
        // the Bezier patch control points to show
        ControlPoints* newPatchControlPoints,
        // the render parameters to use
        RenderParameters    *newRenderParameters,
        // the render window that it controls
        RenderWindow        *newRenderWindow
        );

    public slots:
    // slot for responding to arcball rotation for object
    void objectRotationChanged();

    // slots for responding to zoom & translate sliders
    void xTranslateChanged(int value);
    void yTranslateChanged(int value);
    void zTranslateChanged(int value);

    // slots for responding to check boxes
    void showNetCheckChanged(int state);
    void showPlanesCheckChanged(int state);
    void showVerticesBoxCheckChanged(int state);
    void showBezierBoxChanged(int state);
    void orthoBoxChanged(int state);

    // slots for responding to arcball manipulations
    // these are general purpose signals which pass the mouse moves to the controller
    // after scaling to the notional unit sphere
    void BeginScaledDrag(int whichButton, float x, float y);
    // note that Continue & End assume the button has already been set
    void ContinueScaledDrag(float x, float y);
    void EndScaledDrag(float x, float y);

    }; // class RenderController

// end of include guard
#endif
