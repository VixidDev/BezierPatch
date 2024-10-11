//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////

#include "Vector3.h"
#include "Point3.h"
#include "math.h"
#include <iomanip>
#include <limits>
// constructors
Point3::Point3()
    : x(0.0), y(0.0), z(0.0) 
    {}

Point3::Point3(float X, float Y, float Z)
    : x(X), y(Y), z(Z) 
    {}

Point3::Point3(const Point3 &other)
    : x(other.x), y(other.y), z(other.z) 
    {}
    
// equality operator
bool Point3::operator ==(const Point3 &other) const
    { // Point3::operator ==()
    return (abs(x - other.x) < std::numeric_limits<float>::epsilon() && abs(y - other.y) < std::numeric_limits<float>::epsilon() && abs(z - other.z) < std::numeric_limits<float>::epsilon());
    } // Point3::operator ==()


// operator that allows us to use array indexing instead of variable names
float &Point3::operator [] (const int index)
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
        // actually the error case
        default:
            return x;       
        } // switch on index
    } // operator []

// operator that allows us to use array indexing instead of variable names
const float &Point3::operator [] (const int index) const
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
        // actually the error case
        default:
            return x;       
        } // switch on index
    } // operator []


// stream input
std::istream & operator >> (std::istream &inStream, Point3 &value)
    { // stream output
    inStream >> value.x >> value.y >> value.z;
    return inStream;
    } // stream output
        
// stream output
std::ostream & operator << (std::ostream &outStream, const Point3 &value)
    { // stream output
    outStream << std::setprecision(4) << value.x << " " << std::setprecision(4) << value.y << " " << std::setprecision(4) << value.z;
    return outStream;
    } // stream output
        
