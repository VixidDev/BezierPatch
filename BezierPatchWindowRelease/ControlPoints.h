///////////////////////////////////////////////////
//
//  Hamish Carr
//  October, 2024
//
//  ------------------------
//  ControlPoints.h
//  ------------------------
//  
//  Base code for rendering assignments.
//
//  Minimalist code for reading in space-separated
//  point coordinates
//  
///////////////////////////////////////////////////

// include guard for ControlPoints
#ifndef TEXTURED_OBJECT_H
#define TEXTURED_OBJECT_H

// include the C++ standard libraries we need for the header
#include <vector>
#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// include the unit with Cartesian 3-vectors
#include "Point3.h"

//trying not to break includes
class RenderParameters;
#include "RenderParameters.h"

class ControlPoints
    { // class
    public:
    // vector of vertices
    std::vector<Point3> vertices;

    // constructor will initialise to safe values
    ControlPoints();
    
    // read point cloud data routine, returns true on success, failure otherwise
    static ControlPoints ReadPointStream(std::istream &pointStream);

    }; // class ControlPoints

// end of include guard for ControlPoints
#endif
