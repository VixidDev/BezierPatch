//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////


#include "Homogeneous4.h"
#include "math.h"
#include <iomanip>

// constructors
Homogeneous4::Homogeneous4() 
    : 
    x(0.0), 
    y(0.0), 
    z(0.0), 
    w(0.0)
    {}

Homogeneous4::Homogeneous4(float X, float Y, float Z, float W)
    : 
    x(X), 
    y(Y), 
    z(Z),
    w(W) 
    {}

Homogeneous4::Homogeneous4(const Point3 &other)
    :
    x(other.x),
    y(other.y),
    z(other.z),
    w(1)
    {}

Homogeneous4::Homogeneous4(const Vector3 &other)
    :
    x(other.x),
    y(other.y),
    z(other.z),
    w(0)
{}

Homogeneous4::Homogeneous4(const Homogeneous4 &other)
    : 
    x(other.x), 
    y(other.y), 
    z(other.z), 
    w(other.w)
    {}

// routine to get a point by perspective division
Point3 Homogeneous4::Point() const
    { // Homogeneous4::Point()
    Point3 returnVal;

    // TODO: convert from Homogeneous coordinates to a Point
    returnVal.x = x / w;
    returnVal.y = y / w;
    returnVal.z = z / w;

    return returnVal;
    } // Homogeneous4::Point()

// routine to get a vector by dropping w (assumed to be 0)
Vector3 Homogeneous4::Vector() const
    { // Homogeneous4::Vector()
    Vector3 returnVal;

    // TODO: convert from Homogeneous coordinates to a Vector
    returnVal.x = x;
    returnVal.y = y;
    returnVal.z = z;

    return returnVal;
    } // Homogeneous4::Vector()


// addition operator
Homogeneous4 Homogeneous4::operator +(const Homogeneous4 &other) const
    { // Homogeneous4::operator +()
    Homogeneous4 returnVal;

    // TODO: add
    returnVal.x = x + other.x;
    returnVal.y = y + other.y;
    returnVal.z = z + other.z;
    returnVal.w = w + other.w;

    return returnVal;
    } // Homogeneous4::operator +()

// subtraction operator
Homogeneous4 Homogeneous4::operator -(const Homogeneous4 &other) const
    { // Homogeneous4::operator -()
    Homogeneous4 returnVal;

    // TODO: subtract
    returnVal.x = x - other.x;
    returnVal.y = y - other.y;
    returnVal.z = z - other.z;
    returnVal.w = w - other.w;

    return returnVal;
    } // Homogeneous4::operator -()

// multiplication operator
Homogeneous4 Homogeneous4::operator *(float factor) const
    { // Homogeneous4::operator *()
    Homogeneous4 returnVal;

    // TODO: scalar multiplication
    returnVal.x = x * factor;
    returnVal.y = y * factor;
    returnVal.z = z * factor;
    returnVal.w = w * factor;

    return returnVal;
    } // Homogeneous4::operator *()

// division operator
Homogeneous4 Homogeneous4::operator /(float factor) const
    { // Homogeneous4::operator /()
    Homogeneous4 returnVal;

    // TODO: scalar division
    returnVal.x = x / factor;
    returnVal.y = y / factor;
    returnVal.z = z / factor;
    returnVal.w = w / factor;

    return returnVal;
    } // Homogeneous4::operator /()

// operator that allows us to use array indexing instead of variable names
float &Homogeneous4::operator [] (const int index)
    { // operator []
    // use default to catch out of range indices
    // we could throw an exception, but will just return the 0th element instead
    switch (index)
        { // switch on index
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        // actually the error case
        default:
            return x;       
        } // switch on index
    } // operator []

// operator that allows us to use array indexing instead of variable names
const float &Homogeneous4::operator [] (const int index) const
    { // operator []
    // use default to catch out of range indices
    // we could throw an exception, but will just return the 0th element instead
    switch (index)
        { // switch on index
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        // actually the error case
        default:
            return x;       
        } // switch on index
    } // operator []

// multiplication operator
Homogeneous4 operator *(float factor, const Homogeneous4 &right)
    { // operator *
    // scalar multiplication is commutative, so flip & return
    return right * factor;
    } // operator *

Homogeneous4 Homogeneous4::modulate(Homogeneous4 b)
    { // Homogeneous4::Vector()
        return Homogeneous4(x*b.x,y*b.y,z*b.z,w*b.w);
    } // Homogeneous4::Vector()

// stream input
std::istream & operator >> (std::istream &inStream, Homogeneous4 &value)
    { // stream output
    inStream >> value.x >> value.y >> value.z >> value.w;
    return inStream;
    } // stream output
        
// stream output
std::ostream & operator << (std::ostream &outStream, const Homogeneous4 &value)
    { // stream output
    outStream << std::setprecision(4) << value.x << " " << std::setprecision(4) << value.y << " " << std::setprecision(4) << value.z << " " << std::setprecision(4) << value.w;
    return outStream;
    } // stream output
        
