#include <iostream>

#include "../BezierPatchWindowRelease/Point3.h"
#include "../BezierPatchWindowRelease/Vector3.h"
#include "../BezierPatchWindowRelease/Matrix4.h"
#include "../BezierPatchWindowRelease/Homogeneous4.h"

int main() {

    Point3 p(0, 1, 3);
    Point3 q(1, -1, 2);
    Point3 r(0, 0, 1);

    Vector3 u(-1, 2, 3);
    Vector3 v(1, 2, -1);
    Vector3 w(-1, 2, 4);

    Matrix4 m1;
    m1[0][0] = 1;
    m1[0][1] = 0;
    m1[0][2] = 3;
    m1[1][0] = 1;
    m1[1][1] = 9;
    m1[1][2] = 2;
    m1[2][0] = 4;
    m1[2][1] = 3;
    m1[2][2] = 6;

    Matrix4 m2;
    m2[0][0] = 0.707;
    m2[0][1] = 0.707;
    m2[0][2] = 0;
    m2[1][0] = -0.707;
    m2[1][1] = 0.707;
    m2[1][2] = 0;
    m2[2][0] = 0;
    m2[2][1] = 0;
    m2[2][2] = 1;

    Matrix4 m3;
    m3[0][0] = 6;
    m3[0][1] = 0;
    m3[0][2] = 0;
    m3[1][0] = -1;
    m3[1][1] = 4;
    m3[1][2] = 0;
    m3[2][0] = 4;
    m3[2][1] = 2;
    m3[2][2] = -3;

    // p + v
    Point3 result1 = p + u;
    std::cout << "p + u: " << result1 << std::endl;

    // q + m1v
    Homogeneous4 h(v); // convert v to homogeneous
    Homogeneous4 h2 = m1 * h; // multiply m1 by h
    Vector3 tempVec1 = h2.Vector(); // convert back to vector
    Point3 result2 = q + tempVec1;
    std::cout << "q + m1v: " << result2 << std::endl;

    // r + m3m2w
    Homogeneous4 h3(w); // convert w to homogeneous
    Homogeneous4 h4 = (m3 * m2) * h3; // multiply m3 by m2 by h3
    Vector3 tempVec2 = h4.Vector(); // convert back to vector
    Point3 result3 = r + tempVec2;
    std::cout << "r + m3m2w: " << result3 << std::endl;

    // m2^-1
    // since m2 is a rotation matrix, the transpose is the inverse
    std::cout << "m2^-1: " << std::endl << m2.transpose() << std::endl; 

    // u . v
    float result5 = u.dot(v);
    std::cout << "u . v: " << result5 << std::endl;

    // v x w
    Vector3 result6 = v.cross(w);
    std::cout << "v x w: " << result6 << std::endl;

    return 1;
}