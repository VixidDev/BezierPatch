//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  October, 2024
//
//  ------------------------
//  Vector3.h
//  ------------------------
//
//  A minimal class for a vector in Cartesian space
//  
///////////////////////////////////////////////////

#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
class Point3;

// the class - we will rely on POD for sending to GPU
class Vector3
    { // Vector3
    public:
    // the coordinates
    float x, y, z;

    // constructors
    Vector3();
    Vector3(float X, float Y, float Z);
    Vector3(const Vector3 &other);
    
    // equality operator
    bool operator ==(const Vector3 &other) const;

    // addition operator
    Vector3 operator +(const Vector3 &other) const;

    // subtraction operator
    Vector3 operator -(const Vector3 &other) const;
    
    // multiplication operator
    Vector3 operator *(float factor) const;

    // division operator
    Vector3 operator /(float factor) const;

    // dot product routine
    float dot(const Vector3 &other) const;

    // cross product routine
    Vector3 cross(const Vector3 &other) const;
    
    // routine to find the length
    float length() const;
    
    // normalisation routine
    Vector3 unit() const;
    
    // operator that allows us to use array indexing instead of variable names
    float &operator [] (const int index);
    const float &operator [] (const int index) const;

    }; // Vector3

// multiplication operator by a scalar
Vector3 operator *(float factor, const Vector3 &right);

// addition operator (Point + Vector is valid, but not the other way round)
Point3 operator +(const Point3 &left, const Vector3 &right);

// subtraction operator (can subtract a point from point, resulting a diff vector)
Vector3 operator -(const Point3 &left, const Point3 &right);

// stream input
std::istream & operator >> (std::istream &inStream, Vector3 &value);

// stream output
std::ostream & operator << (std::ostream &outStream, const Vector3 &value);
        
#endif
