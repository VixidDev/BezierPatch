///////////////////////////////////////////////////
//
//  Hamish Carr
//  October, 2024
//
//  ------------------------
//  ControlPoints.cpp
//  ------------------------
//  
//  Base code for rendering assignments.
//
//  Minimalist code for reading in space-separated
//  point coordinates
//  
///////////////////////////////////////////////////

// include the header file
#include "ControlPoints.h"

// include the C++ standard libraries we want
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

// include the Cartesian 3- vector class
#include "Point3.h"

// constructor will initialise to safe values
ControlPoints::ControlPoints()
    { // ControlPoints()
    // force arrays to size 0
    vertices.resize(0);
    } // ControlPoints()

ControlPoints ControlPoints::ReadPointStream(std::istream &pointStream)
{
    ControlPoints patch;
    double vxCoordX, vxCoordY, vxCoordZ;

    // the rest of this is a loop reading lines & adding them in appropriate places
    std::string line;

    while (getline(pointStream, line))
    { // not eof
        std::stringstream ss(line);
        ss >> vxCoordX >> vxCoordY >> vxCoordZ;
        patch.vertices.emplace_back(vxCoordX, vxCoordY, vxCoordZ);
    } // not eof

    return patch;
}



