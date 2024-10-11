/////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  October, 2024
//
//  -----------------------------
//  Render Window
//  -----------------------------
//
//  The render window class is really just a container
//  for tracking the visual hierarchy.  While it is
//  entirely possible to use Qt Creator, I try to avoid
//  over-commitment to it because I need to write code in
//  multiple environments, some of which are not well-suited
//  to IDEs in general, let alone Qt Creator
//
//  Also, by providing sample code, the didactic purpose of
//  showing how things fit together is better served.
//
/////////////////////////////////////////////////////////////////

// include guard
#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <vector>

// include standard Qt widgets
#include <QtWidgets>

// include a custom arcball widget
#include "ArcBallWidget.h"
// include the custom render widget
#include "RenderWidget.h"
// and a second widget which the student will edit
#include "BezierPatchRenderWidget.h"

// a window that displays an Bezier patch with controls
class RenderWindow : public QWidget
    { // class RenderWindow
    private:
    // the Bezier patch control points being shown
    ControlPoints          *patchControlPoints;

    // the values set in the interface
    RenderParameters            *renderParameters;

    // window layout
    QGridLayout                 *windowLayout;

    // custom widgets
    ArcBallWidget               *modelRotator;
    RenderWidget                *renderWidget;
    BezierPatchRenderWidget        *bezierPatchRenderWidget;

    // standard widgets
    // check boxes to control render options
    QCheckBox                   *showPlanesBox;
    QCheckBox                   *showNetBox;
    QCheckBox                   *showVerticesBox;
    QCheckBox                   *showBezierBox;

    // check boxes for projection options
    QCheckBox                   *orthoBox;


    // sliders for spatial manipulation
    QSlider                     *xTranslateSlider;
    // we want one slider under each widget
    QSlider                     *secondXTranslateSlider;
    QSlider                     *yTranslateSlider;
    QSlider                     *zTranslateSlider;



    // labels for sliders & arcballs
    QLabel                      *modelRotatorLabel;
    QLabel                      *yTranslateLabel;
    QLabel                      *zoomLabel;

    public:
    // constructor
    RenderWindow
        (
        // the object to be rendered
        ControlPoints          *newPatchControlPoints,
        // the model object storing render parameters
        RenderParameters        *newRenderParameters,
        // the title for the window (with default value)
        const char              *windowName = "Object Renderer"
        );

    // routine to reset the interface
    // sets every visual control to match the model
    // gets called by the controller after each change in the model
    void ResetInterface();

    // declare the render controller class a friend so it can access the UI elements
    friend class RenderController;

    }; // class RenderWindow

// end of include guard
#endif
