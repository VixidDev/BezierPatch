//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////


#include <math.h>
#include <thread>
#include <random>
#include <QTimer>
// include open mp for the advanced tasks
#include <omp.h>
#include <algorithm>

// include the header file
#include "BezierPatchRenderWidget.h"

#include <QElapsedTimer>
#include <GL/glut.h>
#include <GL/gl.h>

#include <chrono>
#include <thread>

//	Ken Shoemake's ArcBall
#include "ArcBall.h"

#define N_THREADS 16

#define PI 3.14159265359f

// constructor
BezierPatchRenderWidget::BezierPatchRenderWidget
        (   
        // the Bezier patch control points to show
        ControlPoints       *newPatchControlPoints,
        // the render parameters to use
        RenderParameters    *newRenderParameters,
        // parent widget in visual hierarchy
        QWidget             *parent
        )
    // the : indicates a member initialiser list ...
    // ... it is good practice to use it where possible
    : 
    // start by calling inherited constructor with parent widget's pointer
    QOpenGLWidget(parent),
    // then store the pointers that were passed in
    patchControlPoints(newPatchControlPoints),
    renderParameters(newRenderParameters)
    { // constructor
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &BezierPatchRenderWidget::forceRepaint);
        timer->start(30);
    // leaves nothing to put into the constructor body
    } // constructor

void BezierPatchRenderWidget::forceRepaint(){
    update();
}
// destructor
BezierPatchRenderWidget::~BezierPatchRenderWidget()
    { // destructor
    // empty (for now)
    // all of our pointers are to data owned by another class
    // so we have no responsibility for destruction
    // and OpenGL cleanup is taken care of by Qt
    } // destructor                                                                 

// called when OpenGL context is set up
void BezierPatchRenderWidget::initializeGL()
    { // BezierPatchRenderWidget::initializeGL()
    } // BezierPatchRenderWidget::initializeGL()

// called every time the widget is resized
void BezierPatchRenderWidget::resizeGL(int w, int h)
    { // BezierPatchRenderWidget::resizeGL()
    // resize the render image
    frameBuffer.Resize(w, h);
    } // BezierPatchRenderWidget::resizeGL()


// called every time the widget needs painting
void BezierPatchRenderWidget::paintGL()
{ // BezierPatchRenderWidget::paintGL()

    // TODO:
    // To match the OpenGL widget, refer to "RenderWidget.{h/cpp}.
    // There you shall find the precise colours, positions, etc.
    // No additional OpenGL calls should be added here for drawing.
    // Make use of the renderParameters matrices for converting
    //  points from local coordinates to screen space.
    // The final screen space coordinates (row/column) should then be used
    //  to set the 2D frameBuffer array with the final colour.
    // (glDrawPixels then puts the frameBuffer on the screen
    //   to display the final image at the end of paintGL)

    // clear the (non-OpenGL) buffer where we will set pixels to:
    frameBuffer.clear(renderParameters->theClearColor);

    // now clear the OpenGL buffer:
    glClearColor(0.8, 0.8, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    Matrix4 identity_matrix;
    identity_matrix.SetIdentity();

    float w = (float) frameBuffer.width;
    float h = (float) frameBuffer.height;
    float aspectRatio = w / h;

    float near = 0.01f;
    float far = 200.0f;
    float left, right, bottom, top;

    // Projection matrix
    projectionMatrix.SetIdentity();

    // View matrix
    viewMatrix.SetIdentity();

    // Model matrix
    modelMatrix.SetIdentity();
    modelMatrix.SetTranslation(Vector3(renderParameters->xTranslate, renderParameters->yTranslate, renderParameters->zTranslate-1));
    modelMatrix = modelMatrix * renderParameters->rotationMatrix;

    if (renderParameters->orthoProjection) {
        if (aspectRatio > 1.0f) {
            left = -aspectRatio * (10.0f / renderParameters->zTranslate);
            right = aspectRatio * (10.0f / renderParameters->zTranslate);
            bottom = -10.0f / renderParameters->zTranslate;
            top = 10.0f / renderParameters->zTranslate;
        } else {
            left = -10.0f / renderParameters->zTranslate;
            right = 10.0f / renderParameters->zTranslate;
            bottom = -aspectRatio * (10.0f / renderParameters->zTranslate);
            top = aspectRatio * (10.0f / renderParameters->zTranslate);
        }

        projectionMatrix[0][0] = 2.0f / (right - left);
        projectionMatrix[1][1] = 2.0f / (top - bottom);
        projectionMatrix[2][2] = -2.0f / (far - near);
        projectionMatrix[0][3] = -(right + left) / (right - left);
        projectionMatrix[1][3] = -(top + bottom) / (top - bottom);
        projectionMatrix[2][3] = -(far * near) / (far - near);
    } else {
        if (aspectRatio > 1.0f) {
            left = -aspectRatio * 0.01f;
            right = aspectRatio * 0.01f;
            bottom = -0.01f;
            top = 0.01f;
        } else {
            left = -0.01f;
            right = 0.01f;
            bottom = -aspectRatio * 0.01f;
            top = aspectRatio * 0.01f;
        }

        // glFrustum
        projectionMatrix[0][0] = (2.0f * near) / (right - left);
        projectionMatrix[1][1] = (2.0f * near) / (top - bottom);
        projectionMatrix[2][2] = -(far + near) / (far - near);
        projectionMatrix[3][3] = 0.0f;
        projectionMatrix[0][2] = (right + left) / (right / left);
        projectionMatrix[1][2] = (top + bottom) / (top - bottom);
        projectionMatrix[3][2] = -1.0f;
        projectionMatrix[2][3] = -(2.0f * far * near) / (far - near);
    }

    // gluLookAt
    Vector3 eye(0.0f, 0.0f, 8.0f);
    Vector3 origin(0.0f, 0.0f, 0.0f);
    Vector3 up(0.0f, 1.0f, 0.0f);

    Vector3 forward = Vector3(origin - eye).unit();
    Vector3 s = forward.cross(up);

    Matrix4 mMatrix;
    mMatrix.SetIdentity();
    mMatrix[0][0] = s.x;
    mMatrix[0][1] = s.y;
    mMatrix[0][2] = s.z;
    mMatrix[1][0] = up.z;
    mMatrix[1][1] = up.y;
    mMatrix[1][2] = up.z;
    mMatrix[2][0] = -forward.x;
    mMatrix[2][1] = -forward.y;
    mMatrix[2][2] = -forward.z;

    Matrix4 eyeTranslation;
    eyeTranslation.SetIdentity();
    eyeTranslation[0][3] = -eye.x;
    eyeTranslation[1][3] = -eye.y;
    eyeTranslation[2][3] = -eye.z;

    viewMatrix = eyeTranslation * mMatrix;

    // Model-view-projection matrix
    mvpMatrix.SetIdentity();
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    if(renderParameters->verticesEnabled)
    {// UI control for showing vertices
        Matrix4 tM = Matrix4();
        tM.SetTranslation(Vector3(-3.0f, 3.0f, 4.0f));
        Matrix4 pM = projectionMatrix * viewMatrix * (modelMatrix * tM);
        for (float phi = 0.0f; phi < 2.0f * PI; phi += PI / 30.0f) {
                for (float theta = 0.0f; theta < 2.0f * PI; theta += PI / 30.0f) {
                    Homogeneous4 point(Point3(
                        0.1f * std::cos(phi) * std::cos(theta), 
                        0.1f * std::cos(phi) * std::sin(theta),
                        0.1f * std::sin(phi)));
                    Homogeneous4 transformedPoint = pM * point;
                    Homogeneous4 ndcs(transformedPoint.Point());

                    ndcs.x = (ndcs.x + 1) / 2 * frameBuffer.width;
                    ndcs.y = (ndcs.y + 1) / 2 * frameBuffer.height;
                        
                    Point3 screenCoord(ndcs.x, ndcs.y, ndcs.z);

                    if (screenCoord.x >= 0 && screenCoord.x <= frameBuffer.width - 1 && screenCoord.y >= 0 && screenCoord.y <= frameBuffer.height - 1) {
                        frameBuffer[(int)screenCoord.y][(int)screenCoord.x] = RGBAValue(0.0f, 255.0f, 0.0f, 255.0f);
                    }
                }
            }

        for(int i = 0 ; i < (*patchControlPoints).vertices.size(); i++)
        {
            // draw each vertex as a point
            // (paint the active vertex in red, ...
            //  ... keep the others in white)

            // consider ways to make the rendered points bigger than just 1x1 pixel on the screen

            RGBAValue colour;
            if (i == renderParameters->activeVertex) {
                colour = RGBAValue(255.0f, 0.0f, 0.0f, 255.0f);
            } else {
                colour = RGBAValue(255.0f * 0.75, 255.0f * 0.75, 255.0f * 0.75, 255.0f);
            }

            Matrix4 translationMatrix = Matrix4();
            translationMatrix.SetTranslation(Vector3(
                (*patchControlPoints).vertices[(i/4)*4+(i%4)][0],
                (*patchControlPoints).vertices[(i/4)*4+(i%4)][1],
                (*patchControlPoints).vertices[(i/4)*4+(i%4)][2]));
            
            Matrix4 pointMatrix = projectionMatrix * viewMatrix * (modelMatrix * translationMatrix);

            float radius = 0.1f;

            for (float phi = 0.0f; phi < 2.0f * PI; phi += PI / 30.0f) {
                for (float theta = 0.0f; theta < 2.0f * PI; theta += PI / 30.0f) {
                    Homogeneous4 point(Point3(
                        radius * std::cos(phi) * std::cos(theta), 
                        radius * std::cos(phi) * std::sin(theta),
                        radius * std::sin(phi)));
                    Homogeneous4 transformedPoint = pointMatrix * point;
                    Homogeneous4 ndcs(transformedPoint.Point());

                    ndcs.x = (ndcs.x + 1) / 2 * frameBuffer.width;
                    ndcs.y = (ndcs.y + 1) / 2 * frameBuffer.height;
                        
                    Point3 screenCoord(ndcs.x, ndcs.y, ndcs.z);

                    if (screenCoord.x >= 0 && screenCoord.x <= frameBuffer.width - 1 && screenCoord.y >= 0 && screenCoord.y <= frameBuffer.height - 1) {
                        frameBuffer[(int)screenCoord.y][(int)screenCoord.x] = colour;
                    }
                }
            }
        }
    }// UI control for showing vertices

    if(renderParameters->planesEnabled)
    {// UI control for showing axis-aligned planes

        // Planes are axis aligned grids made up of lines
        for (int i = -5; i <= 5; i += 2) {
            drawLine(Point3(-5, 0, i), Point3(5, 0, i), RGBAValue(255.0f / 4, 0.0f, 255.0f / 4, 255.0f)); // x plane horizontal
            drawLine(Point3(i, 0, -5), Point3(i, 0, 5), RGBAValue(255.0f / 4, 0.0f, 255.0f / 4, 255.0f)); // x plane vertical
            drawLine(Point3(0, i, -5), Point3(0, i, 5), RGBAValue(0.0f, 255.0f / 4, 255.0f / 4, 255.0f)); // z plane horizontal
            drawLine(Point3(0, -5, i), Point3(0, 5, i), RGBAValue(0.0f, 255.0f / 4, 255.0f / 4, 255.0f)); // z plane vertical
        }

        for (int i = -5; i <= 5; i++) {
            drawLine(Point3(-5, i, 0), Point3(5, i, 0), RGBAValue(255.0f / 4, 255.0f / 4, 0.0f, 255.0f)); // y plane horizontal
            drawLine(Point3(i, -5, 0), Point3(i, 5, 0), RGBAValue(255.0f / 4, 255.0f / 4, 0.0f, 255.0f)); // y plane vertical
        }

        // Refer to RenderWidget.cpp for the precise colours.

    }// UI control for showing axis-aligned planes

    if(renderParameters->netEnabled)
    {// UI control for showing the Bezier control net
     // (control points connected with lines)
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                Point3 controlPoint1(
                    (*patchControlPoints).vertices[i*4+j][0],
                    (*patchControlPoints).vertices[i*4+j][1],
                    (*patchControlPoints).vertices[i*4+j][2]);
                Point3 controlPoint2(
                    (*patchControlPoints).vertices[i*4+j+1][0],
                    (*patchControlPoints).vertices[i*4+j+1][1],
                    (*patchControlPoints).vertices[i*4+j+1][2]);
                
                drawLine(controlPoint1, controlPoint2, RGBAValue(0.0f, 255.0f, 0.0f, 255.0f));
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                Point3 controlPoint1(
                    (*patchControlPoints).vertices[i+j*4][0],
                    (*patchControlPoints).vertices[i+j*4][1],
                    (*patchControlPoints).vertices[i+j*4][2]);
                Point3 controlPoint2(
                    (*patchControlPoints).vertices[i+4+j*4][0],
                    (*patchControlPoints).vertices[i+4+j*4][1],
                    (*patchControlPoints).vertices[i+4+j*4][2]);
                
                drawLine(controlPoint1, controlPoint2, RGBAValue(0.0f, 255.0f, 0.0f, 255.0f));
            }
        }
    }// UI control for showing the Bezier control net

    if(renderParameters->bezierEnabled)
    {// UI control for showing the Bezier curve
        for (float s = 0.0; s <= 1.0; s += 0.01)
        {// s parameter loop

            for (float t = 0.0; t <= 1.0; t += 0.01)
            { // t parameter loop

                // set the pixel for this parameter value using s, t for colour
            } // t parameter loop
        } // s parameter loop
    }

    // Put the custom framebufer on the screen to display the image
    glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.block);

} // BezierPatchRenderWidget::paintGL()

void BezierPatchRenderWidget::drawLine(Point3 start, Point3 end, RGBAValue colour) {
    Vector3 difference = end - start;
    
    for (float t = 0.0f; t < 1.0f; t += 0.001f) {
        Homogeneous4 pointOnLine(Point3(start + difference * t));
        Homogeneous4 transformedPoint = mvpMatrix * pointOnLine;
        Homogeneous4 ndcs(transformedPoint.Point());

        ndcs.x = (ndcs.x + 1) / 2 * frameBuffer.width;
        ndcs.y = (ndcs.y + 1) / 2 * frameBuffer.height;

        Point3 screenCoord(ndcs.x, ndcs.y, ndcs.z);

        if (screenCoord.x >= 0 && screenCoord.x <= frameBuffer.width - 1 && screenCoord.y >= 0 && screenCoord.y <= frameBuffer.height - 1) {
            frameBuffer[(int)screenCoord.y][(int)screenCoord.x] = colour;
        }
    }
}

void BezierPatchRenderWidget::drawPoint(Point3 point) {
    return;
}

// mouse-handling
void BezierPatchRenderWidget::mousePressEvent(QMouseEvent *event)
    { // BezierPatchRenderWidget::mousePressEvent()
    // store the button for future reference
    int whichButton = int(event->button());
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;

    
    // and we want to force mouse buttons to allow shift-click to be the same as right-click
    unsigned int modifiers = event->modifiers();
    
    // shift-click (any) counts as right click
    if (modifiers & Qt::ShiftModifier)
        whichButton = Qt::RightButton;
    
    // send signal to the controller for detailed processing
    emit BeginScaledDrag(whichButton, x,y);
    } // BezierPatchRenderWidget::mousePressEvent()
    
void BezierPatchRenderWidget::mouseMoveEvent(QMouseEvent *event)
    { // BezierPatchRenderWidget::mouseMoveEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;
    
    // send signal to the controller for detailed processing
    emit ContinueScaledDrag(x,y);
    } // BezierPatchRenderWidget::mouseMoveEvent()
    
void BezierPatchRenderWidget::mouseReleaseEvent(QMouseEvent *event)
    { // BezierPatchRenderWidget::mouseReleaseEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;
    
    // send signal to the controller for detailed processing
    emit EndScaledDrag(x,y);
    } // BezierPatchRenderWidget::mouseReleaseEvent()
