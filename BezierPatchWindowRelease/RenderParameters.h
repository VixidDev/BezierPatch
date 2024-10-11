/////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  October, 2024
//
//  -----------------------------
//  Render Parameters
//  -----------------------------
//
//  This is part of the "model" in the MVC (model-view-controlled) paradigm
//  We separate out the render parameters from the object being rendered
//
/////////////////////////////////////////////////////////////////

// include guard
#ifndef RENDER_PARAMETERS_H
#define RENDER_PARAMETERS_H


#ifndef PI
#define PI 3.14159265358979
#endif

#include "Matrix4.h"
#include <vector>

//here not to break the includes
class ControlPoints;
#include "ControlPoints.h"
#include "RGBAValue.h"

// class for the render parameters
class RenderParameters
    { // class RenderParameters
    public:

    // store a pointer to the control points here ...
    // ...for changing their coordinates with UI keys
    ControlPoints *patchControlPoints;

    // we store x & y translations
    float xTranslate, yTranslate,zTranslate;

    // two homogeneous matrices for converting to screen space
    Matrix4 rotationMatrix;
    Matrix4 modelviewMatrix;

    // have the colour ready for framebuffer placeholders:
    RGBAValue theClearColor;

    // and the booleans
    // whether to show the reference planes:
    bool planesEnabled;
    // whether to show the control net:
    bool netEnabled;
    // whether to show the control vertices
    bool verticesEnabled;
    // whether to show the surface:
    bool bezierEnabled;
    // toggle between projections:
    bool orthoProjection;
    bool triggerResize;

    // width and height of window, plus initial value
    int windowSize;
    int windowWidth, windowHeight;

    // which vertex is being actively manipulated
    int activeVertex;

    // constructor
    RenderParameters(ControlPoints *newPatchControlPoints)
        :
        xTranslate(0.0),
        yTranslate(0.0),
        zTranslate(1.4), // zoom in a little bit by default
        planesEnabled(true),
        netEnabled(true),
        verticesEnabled(true),
        bezierEnabled(false),
        orthoProjection(true),
        triggerResize(false),
        theClearColor{0.8f, 0.8f, 0.6f, 1.0f},
        windowSize(640),
        activeVertex(0),
        patchControlPoints(newPatchControlPoints)
        { // constructor
        // because we are paranoid, we will initialise the matrices to the identity
        rotationMatrix.SetIdentity();
        modelviewMatrix.SetIdentity();
        } // constructor

    ~RenderParameters(){
    }

    }; // class RenderParameters

// now define some macros for bounds on parameters
#define TRANSLATE_MIN 0.0f
#define TRANSLATE_MAX 5.0f

#define ZOOM_SCALE_LOG_MIN -2.0f
#define ZOOM_SCALE_LOG_MAX 2.0f
#define ZOOM_SCALE_MIN 0.01f
#define ZOOM_SCALE_MAX 100.0f

// this is to scale to/from integer values
#define PARAMETER_SCALING 100


// end of include guard
#endif
