#include "Point3.h"
#include "Vector3.h"
#include "math.h"
#include <iomanip>
#include <limits>
// constructors
Vector3::Vector3()
    : x(0.0), y(0.0), z(0.0) 
    {}

Vector3::Vector3(float X, float Y, float Z)
    : x(X), y(Y), z(Z) 
    {}

Vector3::Vector3(const Vector3 &other)
    : x(other.x), y(other.y), z(other.z) 
    {}
    
// equality operator
bool Vector3::operator ==(const Vector3 &other) const
    { // Vector3::operator ==()
    return (abs(x - other.x) < std::numeric_limits<float>::epsilon() && abs(y - other.y) < std::numeric_limits<float>::epsilon() && abs(z - other.z) < std::numeric_limits<float>::epsilon());
    } // Vector3::operator ==()

// addition operator
Vector3 Vector3::operator +(const Vector3 &other) const
    { // Vector3::operator +()
    Vector3 returnVal;

    // TODO: add other values to current
    returnVal.x = x + other.x;
    returnVal.y = y + other.y;
    returnVal.z = z + other.z;
    
    return returnVal;
    } // Vector3::operator +()

// subtraction operator
Vector3 Vector3::operator -(const Vector3 &other) const
    { // Vector3::operator -()
    Vector3 returnVal;

    // TODO: subract other values from current
    returnVal.x = x - other.x;
    returnVal.y = y - other.y;
    returnVal.z = z - other.z;

    return returnVal;
    } // Vector3::operator -()

// multiplication operator
Vector3 Vector3::operator *(float factor) const
    { // Vector3::operator *()
    Vector3 returnVal;

    // TODO: multiply by the given factor
    returnVal.x = x * factor;
    returnVal.y = y * factor;
    returnVal.z = z * factor;

    return returnVal;
    } // Vector3::operator *()

// division operator
Vector3 Vector3::operator /(float factor) const
    { // Vector3::operator /()
    Vector3 returnVal;

    // TODO: compute divide by the given factor
    returnVal.x = x / factor;
    returnVal.y = y / factor;
    returnVal.z = z / factor;

    return returnVal;
    } // Vector3::operator /()

// dot product routine
float Vector3::dot(const Vector3 &other) const
    { // Vector3::dot()

    // TODO: compute the dot product
    return (x * other.x) + (y * other.y) + (z * other.z);
    } // Vector3::dot()

// cross product routine
Vector3 Vector3::cross(const Vector3 &other) const
    { // Vector3::cross()
    Vector3 returnVal;

    // TODO: compute the cross product
    returnVal.x = (y * other.z) - (z * other.y);
    returnVal.y = (z * other.x) - (x * other.z);
    returnVal.z = (x * other.y) - (y * other.x);

    return returnVal;
    } // Vector3::cross()

// routine to find the length
float Vector3::length() const
    { // Vector3::length()

    // TODO: compute the Euclidean norm
    return std::sqrt((x * x) + (y * y) + (z * z));
    } // Vector3::length()

// normalisation routine
Vector3 Vector3::unit() const
    { // Vector3::unit()
    Vector3 returnVal;

    // TODO: divide each element by the Euclidean norm
    float length = this->length();
    returnVal.x = x / length;
    returnVal.y = y / length;
    returnVal.z = z / length;

    return returnVal;
    } // Vector3::unit()

// operator that allows us to use array indexing instead of variable names
float &Vector3::operator [] (const int index)
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
const float &Vector3::operator [] (const int index) const
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

// addition operator (Point + Vector is valid, but not the other way round)
Point3 operator +(const Point3 &left, const Vector3 &right)
    { // operator +()
    Point3 returnVal;

    // TODO: add a vector to a point
    returnVal.x = left.x + right.x;
    returnVal.y = left.y + right.y;
    returnVal.z = left.z + right.z;

    return returnVal;
    } // operator +()

// subtraction operator (can subtract a point from point, resulting a diff vector)
Vector3 operator -(const Point3 &left, const Point3 &right)
    { // operator -()
    Vector3 returnVal;

    // TODO: subtract two points to get a difference vector between them
    returnVal.x = left.x - right.x;
    returnVal.y = left.y - right.y;
    returnVal.z = left.z - right.z;

    return returnVal;
    } // operator -()

// multiplication operator by a scalar
Vector3 operator *(float factor, const Vector3 &right)
    { // operator *
    Vector3 returnVal;

    // TODO: scalar multiplication
    returnVal.x = right.x * factor;
    returnVal.y = right.y * factor;
    returnVal.z = right.z * factor;

    return returnVal;
    } // operator *

// stream input
std::istream & operator >> (std::istream &inStream, Vector3 &value)
    { // stream output
    inStream >> value.x >> value.y >> value.z;
    return inStream;
    } // stream output
        
// stream output
std::ostream & operator << (std::ostream &outStream, const Vector3 &value)
    { // stream output
    outStream << std::setprecision(4) << value.x << " " << std::setprecision(4) << value.y << " " << std::setprecision(4) << value.z;
    return outStream;
    } // stream output

        
