//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  October, 2024
//
//  ------------------------
//  Point3.h
//  ------------------------
//  
//  A minimal class for a point in Cartesian space
//  
///////////////////////////////////////////////////

#ifndef POINT3_H
#define POINT3_H

#include <iostream>

// forward declaration, include in Point3.cpp
class Vector3;

// the class - we will rely on POD for sending to GPU
class Point3
    { // Point3
    public:
    // the coordinates
    float x, y, z;

    // constructors
    Point3();
    Point3(float X, float Y, float Z);
    Point3(const Point3 &other);
    
    // equality operator
    bool operator ==(const Point3 &other) const;

    
    // operator that allows us to use array indexing instead of variable names
    float &operator [] (const int index);
    const float &operator [] (const int index) const;

    }; // Point3


// stream input
std::istream & operator >> (std::istream &inStream, Point3 &value);

// stream output
std::ostream & operator << (std::ostream &outStream, const Point3 &value);
        
#endif
