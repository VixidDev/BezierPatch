/////////////////////////////////////////////////////////////////
//
//  We're using the Model-View-Controller pattern
//  so most of the control logic goes here
//  which means we need a slot for substantially
//  every possible UI manipulation
//
/////////////////////////////////////////////////////////////////

#include "RenderController.h"
#include <stdio.h>

// constructor
RenderController::RenderController
        (
        // the Bezier patch control points to show
        ControlPoints         *newPatchControlPoints,
        // the render parameters to use
        RenderParameters    *newRenderParameters,
        // the render window that it controls
        RenderWindow        *newRenderWindow
        )
    :
    patchControlPoints  (newPatchControlPoints),
    renderParameters(newRenderParameters),
    renderWindow    (newRenderWindow),
    dragButton      (Qt::NoButton)
    { // RenderController::RenderController()

    // connect up signals to slots

    // signals for arcballs
    QObject::connect(   renderWindow->modelRotator,                 SIGNAL(RotationChanged()),
                        this,                                       SLOT(objectRotationChanged()));

    // signals for main widget to control arcball
    QObject::connect(   renderWindow->renderWidget,                 SIGNAL(BeginScaledDrag(int, float, float)),
                        this,                                       SLOT(BeginScaledDrag(int, float, float)));
    QObject::connect(   renderWindow->renderWidget,                 SIGNAL(ContinueScaledDrag(float, float)),
                        this,                                       SLOT(ContinueScaledDrag(float, float)));
    QObject::connect(   renderWindow->renderWidget,                 SIGNAL(EndScaledDrag(float, float)),
                        this,                                       SLOT(EndScaledDrag(float, float)));

    // signals for opengl widget to control arcball
    QObject::connect(   renderWindow->bezierPatchRenderWidget,         SIGNAL(BeginScaledDrag(int, float, float)),
                        this,                                       SLOT(BeginScaledDrag(int, float, float)));
    QObject::connect(   renderWindow->bezierPatchRenderWidget,         SIGNAL(ContinueScaledDrag(float, float)),
                        this,                                       SLOT(ContinueScaledDrag(float, float)));
    QObject::connect(   renderWindow->bezierPatchRenderWidget,         SIGNAL(EndScaledDrag(float, float)),
                        this,                                       SLOT(EndScaledDrag(float, float)));

    // signal for zoom slider
    QObject::connect(   renderWindow->zTranslateSlider,                   SIGNAL(valueChanged(int)),
                        this,                                       SLOT(zTranslateChanged(int)));

    // signal for x translate sliders
    QObject::connect(   renderWindow->xTranslateSlider,             SIGNAL(valueChanged(int)),
                        this,                                       SLOT(xTranslateChanged(int)));
    QObject::connect(   renderWindow->secondXTranslateSlider,       SIGNAL(valueChanged(int)),
                        this,                                       SLOT(xTranslateChanged(int)));

    // signal for y translate slider
    QObject::connect(   renderWindow->yTranslateSlider,             SIGNAL(valueChanged(int)),
                        this,                                       SLOT(yTranslateChanged(int)));


    // signal for check box
    QObject::connect(   renderWindow->showNetBox,              SIGNAL(stateChanged(int)),
                        this,                                       SLOT(showNetCheckChanged(int)));
    QObject::connect(   renderWindow->showPlanesBox,             SIGNAL(stateChanged(int)),
                        this,                                       SLOT(showPlanesCheckChanged(int)));
    QObject::connect(   renderWindow->showVerticesBox,                    SIGNAL(stateChanged(int)),
                        this,                                       SLOT(showVerticesBoxCheckChanged(int)));
    QObject::connect(   renderWindow->showBezierBox,                SIGNAL(stateChanged(int)),
                        this,                                       SLOT(showBezierBoxChanged(int)));
    QObject::connect(   renderWindow->orthoBox,                SIGNAL(stateChanged(int)),
                     this,                                       SLOT(orthoBoxChanged(int)));

    // to make keyboard presses active, need to setFocusPolicy(Qt::StrongFocus) in the relevant widget

    // copy the rotation matrix from the widgets to the model
    renderParameters->rotationMatrix = renderWindow->modelRotator->RotationMatrix();
    } // RenderController::RenderController()

// slot for responding to arcball rotation for object
void RenderController::objectRotationChanged()
    { // RenderController::objectRotationChanged()
    // copy the rotation matrix from the widget to the model
    renderParameters->rotationMatrix = renderWindow->modelRotator->RotationMatrix();

    // reset the interface
    renderWindow->ResetInterface();
    } // RenderController::objectRotationChanged()

// slot for responding to x translate sliders
void RenderController::xTranslateChanged(int value)
    { // RenderController::xTranslateChanged()
    // reset the model's x translation (slider ticks are 1/100 each)
    renderParameters->xTranslate = value / 100.0f;

    // clamp it
    if (renderParameters->xTranslate < TRANSLATE_MIN)
        renderParameters->xTranslate = TRANSLATE_MIN;
    else if (renderParameters->xTranslate > TRANSLATE_MAX)
        renderParameters->xTranslate = TRANSLATE_MAX;

    // reset the interface
    renderWindow->ResetInterface();
    } // RenderController::xTranslateChanged()

// slot for responding to y translate slider
void RenderController::yTranslateChanged(int value)
    { // RenderController::tTranslateChanged()
    // reset the model's y translation (slider ticks are 1/100 each)
    renderParameters->yTranslate =  value / 100.0f;

    // clamp it
    if (renderParameters->yTranslate < TRANSLATE_MIN)
        renderParameters->yTranslate = TRANSLATE_MIN;
    else if (renderParameters->yTranslate > TRANSLATE_MAX)
        renderParameters->yTranslate = TRANSLATE_MAX;

    // reset the interface
    renderWindow->ResetInterface();
    } // RenderController::yTranslateChanged()

// slot for responding to z translate sliders
void RenderController::zTranslateChanged(int value)
    { // RenderController::zTranslateChanged()
    // reset the model's x translation (slider ticks are 1/100 each)
    renderParameters->zTranslate = value / 100.0f;

    // clamp it
    if (renderParameters->zTranslate < TRANSLATE_MIN)
        renderParameters->zTranslate = TRANSLATE_MIN;
    else if (renderParameters->zTranslate > TRANSLATE_MAX)
        renderParameters->zTranslate = TRANSLATE_MAX;

    renderParameters->triggerResize = true;

    // reset the interface
    renderWindow->ResetInterface();
    } // RenderController::xTranslateChanged()




void RenderController::showNetCheckChanged(int state)
    {
    // reset the model's flag
    renderParameters->netEnabled = (state == Qt::Checked);

    // reset the interface
    renderWindow->ResetInterface();
    }

void RenderController::showPlanesCheckChanged(int state)
    {
    // reset the model's flag
    renderParameters->planesEnabled = (state == Qt::Checked);

    // reset the interface
    renderWindow->ResetInterface();
    }

void RenderController::showVerticesBoxCheckChanged(int state)
    {
    // reset the model's flag
    renderParameters->verticesEnabled = (state == Qt::Checked);

    // reset the interface
    renderWindow->ResetInterface();
    }

void RenderController::showBezierBoxChanged(int state)
    {
    // reset the model's flag
    renderParameters->bezierEnabled = (state == Qt::Checked);

    // reset the interface
    renderWindow->ResetInterface();
    }

void RenderController::orthoBoxChanged(int state)
    {
        // reset the model's flag
        renderParameters->orthoProjection = (state == Qt::Checked);
        renderParameters->triggerResize = true;

        // reset the interface
        renderWindow->ResetInterface();
    }




// slots for responding to arcball manipulations
// these are general purpose signals which pass the mouse moves to the controller
// after scaling to the notional unit sphere
void RenderController::BeginScaledDrag(int whichButton, float x, float y)
    { // RenderController::BeginScaledDrag()
    // depends on which button was depressed, so save that for the duration
    dragButton = whichButton;

    // now switch on it to determine behaviour
    switch (dragButton)
        { // switch on the drag button
        // left button drags the model
        case Qt::LeftButton:
            renderWindow->modelRotator->BeginDrag(x, y);
            break;
        // middle button drags visually
        case Qt::MiddleButton:
            break;
        } // switch on the drag button

    // reset the interface
    renderWindow->ResetInterface();
    } // RenderController::BeginScaledDrag()

// note that Continue & End assume the button has already been set
void RenderController::ContinueScaledDrag(float x, float y)
    { // RenderController::ContinueScaledDrag()
    // switch on the drag button to determine behaviour
    switch (dragButton)
        { // switch on the drag button
        // left button drags the model
        case Qt::LeftButton:
            renderWindow->modelRotator->ContinueDrag(x, y);
            break;

        // middle button drags visually
        case Qt::MiddleButton:
            break;
        } // switch on the drag button

    // reset the interface
    renderWindow->ResetInterface();
    } // RenderController::ContinueScaledDrag()

void RenderController::EndScaledDrag(float x, float y)
    { // RenderController::EndScaledDrag()
    // now switch on it to determine behaviour
    switch (dragButton)
        { // switch on the drag button
        // left button drags the model
        case Qt::LeftButton:
            renderWindow->modelRotator->EndDrag(x, y);
            break;

        // middle button drags visually
        case Qt::MiddleButton:
            break;
        } // switch on the drag button

    // and reset the drag button
    dragButton = Qt::NoButton;

    // reset the interface
    renderWindow->ResetInterface();
    } // RenderController::EndScaledDrag()
