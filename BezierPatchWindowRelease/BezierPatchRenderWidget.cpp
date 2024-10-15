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


    // Get start time of frame
    auto start = std::chrono::steady_clock::now();

    fragments.reserve(1000000);

    // clear the (non-OpenGL) buffer where we will set pixels to:
    frameBuffer.clear(renderParameters->theClearColor);

    // now clear the OpenGL buffer:
    glClearColor(0.8, 0.8, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    Matrix4 identity_matrix;
    identity_matrix.SetIdentity();

    float w = (float) frameBuffer.width;
    float h = (float) frameBuffer.height;
    float aspectRatio = w / h; // Calculate aspect ratio based on width and height

    // Set near and far planes
    float near = 0.01f;
    float far = 200.0f;
    float left, right, bottom, top;

    // Projection matrix
    projectionMatrix.SetIdentity();

    // View matrix
    viewMatrix.SetIdentity();

    if (renderParameters->orthoProjection) {
        // Set view matrix translation and rotation for orthographic projection
        viewMatrix.SetTranslation(Vector3(renderParameters->xTranslate, renderParameters->yTranslate, renderParameters->zTranslate-1));
        viewMatrix = viewMatrix * renderParameters->rotationMatrix;

        // Set different left, right, bottom and top variables based on aspect ratio, in line with RenderWidget.cpp
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

        // glOrtho projection matrix
        projectionMatrix[0][0] = 2.0f / (right - left);
        projectionMatrix[1][1] = 2.0f / (top - bottom);
        projectionMatrix[2][2] = -2.0f / (far - near);
        projectionMatrix[0][3] = -(right + left) / (right - left);
        projectionMatrix[1][3] = -(top + bottom) / (top - bottom);
        projectionMatrix[2][3] = -(far * near) / (far - near);
    } else {
        // Set view matrix translation and rotation for perspective projection
        viewMatrix.SetTranslation(Vector3(renderParameters->xTranslate, renderParameters->yTranslate, -(9.0f - renderParameters->zTranslate)));
        viewMatrix = viewMatrix * renderParameters->rotationMatrix;

        // Again, set different projection matrix parameters based on current aspect ratio
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

        // glFrustum projection matrix
        projectionMatrix[0][0] = (2.0f * near) / (right - left);
        projectionMatrix[1][1] = (2.0f * near) / (top - bottom);
        projectionMatrix[2][2] = -(far + near) / (far - near);
        projectionMatrix[3][3] = 0.0f;
        projectionMatrix[0][2] = (right + left) / (right / left);
        projectionMatrix[1][2] = (top + bottom) / (top - bottom);
        projectionMatrix[3][2] = -1.0f;
        projectionMatrix[2][3] = -(2.0f * far * near) / (far - near);
    }

    // Model-view-projection matrix
    mvpMatrix.SetIdentity();
    mvpMatrix = projectionMatrix * viewMatrix; // Combine projection and view matrix for transforming to clip space
    
    if(renderParameters->verticesEnabled)
    {// UI control for showing vertices
        #pragma omp parallel for
        for(int i = 0; i < (*patchControlPoints).vertices.size(); i++)
        {
            // draw each vertex as a point
            // (paint the active vertex in red, ...
            //  ... keep the others in white)

            // consider ways to make the rendered points bigger than just 1x1 pixel on the screen
            RGBAValue colour;
            if (i == renderParameters->activeVertex) { // Set colour of active vertex to red
                colour = RGBAValue(255.0f, 0.0f, 0.0f, 255.0f);
            } else {                                   // keep others as an off white
                colour = RGBAValue(255.0f * 0.75, 255.0f * 0.75, 255.0f * 0.75, 255.0f);
            }

            // Draw the vertices at the given patch control points
            drawPoint(Point3(
                (*patchControlPoints).vertices[(i/4)*4+(i%4)][0],
                (*patchControlPoints).vertices[(i/4)*4+(i%4)][1],
                (*patchControlPoints).vertices[(i/4)*4+(i%4)][2]),
                colour);
        }
    }// UI control for showing vertices

    if(renderParameters->planesEnabled)
    {// UI control for showing axis-aligned planes

        // Planes are axis aligned grids made up of lines
        #pragma omp parallel for
        for (int i = -5; i <= 5; i += 2) {
            drawLine(Point3(-5, 0, i), Point3(5, 0, i), RGBAValue(255.0f / 4, 0.0f, 255.0f / 4, 255.0f)); // x plane horizontal
            drawLine(Point3(i, 0, -5), Point3(i, 0, 5), RGBAValue(255.0f / 4, 0.0f, 255.0f / 4, 255.0f)); // x plane vertical
            drawLine(Point3(0, i, -5), Point3(0, i, 5), RGBAValue(0.0f, 255.0f / 4, 255.0f / 4, 255.0f)); // z plane horizontal
            drawLine(Point3(0, -5, i), Point3(0, 5, i), RGBAValue(0.0f, 255.0f / 4, 255.0f / 4, 255.0f)); // z plane vertical
        }
        #pragma omp parallel for
        for (int i = -5; i <= 5; i++) {
            drawLine(Point3(-5, i, 0), Point3(5, i, 0), RGBAValue(255.0f / 4, 255.0f / 4, 0.0f, 255.0f)); // y plane horizontal
            drawLine(Point3(i, -5, 0), Point3(i, 5, 0), RGBAValue(255.0f / 4, 255.0f / 4, 0.0f, 255.0f)); // y plane vertical
        }

        // Refer to RenderWidget.cpp for the precise colours.

    }// UI control for showing axis-aligned planes

    

    if(renderParameters->netEnabled)
    {// UI control for showing the Bezier control net
     // (control points connected with lines)

        // Draw horizontal lines between control points
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                Point3 controlPoint1(                           // First control point
                    (*patchControlPoints).vertices[i*4+j][0],
                    (*patchControlPoints).vertices[i*4+j][1],
                    (*patchControlPoints).vertices[i*4+j][2]);
                Point3 controlPoint2(                           // Second control point
                    (*patchControlPoints).vertices[i*4+j+1][0],
                    (*patchControlPoints).vertices[i*4+j+1][1],
                    (*patchControlPoints).vertices[i*4+j+1][2]);
                
                // Draw line between
                drawLine(controlPoint1, controlPoint2, RGBAValue(0.0f, 255.0f, 0.0f, 255.0f));
            }
        }

        // Draw the vertical lines between control points
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                Point3 controlPoint1(                            // First control point
                    (*patchControlPoints).vertices[i+j*4][0],
                    (*patchControlPoints).vertices[i+j*4][1],
                    (*patchControlPoints).vertices[i+j*4][2]);
                Point3 controlPoint2(                            // Second control point
                    (*patchControlPoints).vertices[i+4+j*4][0],
                    (*patchControlPoints).vertices[i+4+j*4][1],
                    (*patchControlPoints).vertices[i+4+j*4][2]);
                
                // Draw line between
                drawLine(controlPoint1, controlPoint2, RGBAValue(0.0f, 255.0f, 0.0f, 255.0f));
            }
        }
    }// UI control for showing the Bezier control net

    if(renderParameters->bezierEnabled)
    {// UI control for showing the Bezier curve
        // Get the control points in a variable with shorter name for ease of reading
        std::vector<Point3> controlPoints = renderParameters->patchControlPoints->vertices;

        #pragma omp parallel for
        for (int s = 0; s <= 1000; s++)
        {// s parameter loop
            for (float t = 0.0; t <= 1.0; t += 0.001)
            { // t parameter loop

            // Find coordinates from each bezier curve at t parameters
            Homogeneous4 bezier1 = bezier(t, controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
            Homogeneous4 bezier2 = bezier(t, controlPoints[4], controlPoints[5], controlPoints[6], controlPoints[7]);
            Homogeneous4 bezier3 = bezier(t, controlPoints[8], controlPoints[9], controlPoints[10], controlPoints[11]);
            Homogeneous4 bezier4 = bezier(t, controlPoints[12], controlPoints[13], controlPoints[14], controlPoints[15]);

            // Find final point using the previous 4 points as points for a final bezier curve with s parameter
            Homogeneous4 finalPoint = bezier((float)s / 1000.0f, bezier1, bezier2, bezier3, bezier4);

            #pragma omp critical 
            {
                fragments.emplace_back(Fragment{finalPoint, mvpMatrix, RGBAValue(255.0f * (float)s / 1000.0f, 255.0f / 2, 255.0f * t, 255.0f)});
            }

            // Set pixel at final point
            //frameBuffer.setPixel(finalPoint, mvpMatrix, RGBAValue(255.0f * (float)s / 1000.0f, 255.0f / 2, 255.0f * t, 255.0f));
                // set the pixel for this parameter value using s, t for colour
            } // t parameter loop
        } // s parameter loop
    }


    struct {
        bool operator()(Fragment left, Fragment right) const {
            if (left.point.y < right.point.y) return true;
            if (left.point.y > right.point.y) return false;
            if (left.point.x < right.point.x) return true;
            if (left.point.x > right.point.x) return false;
            if (left.point.z > right.point.z) return true;
            if (left.point.z < right.point.z) return false;
            return false; 
        }
    } lessFunctor;

    
    auto sortStart = std::chrono::steady_clock::now();

    std::sort(fragments.begin(), fragments.end(), lessFunctor);

    auto sortEnd = std::chrono::steady_clock::now();
    auto sortTimeTaken = std::chrono::duration_cast<std::chrono::microseconds>(sortEnd - sortStart);
    std::cout << "Sort time taken: " << sortTimeTaken.count() / 1000000.0f << " seconds." << std::endl;


    for (int i = 0; i < fragments.size() - 1; i++) {
        frameBuffer.setPixel(fragments.at(i).point, fragments.at(i).transformationMatrix, fragments.at(i).colour);
    }

    fragments.clear();

    auto end = std::chrono::steady_clock::now();
    auto timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken: " << timeTaken.count() / 1000000.0f << " seconds." << std::endl;

    // Put the custom framebufer on the screen to display the image
    glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.block);

} // BezierPatchRenderWidget::paintGL()

Homogeneous4 BezierPatchRenderWidget::bezier(float parameter, Homogeneous4 controlPoint1, Homogeneous4 controlPoint2, Homogeneous4 controlPoint3, Homogeneous4 controlPoint4) {
    // Precompute t coefficients for terms
    float oneMinust = 1 - parameter;
    float oneMinustSquared = oneMinust * oneMinust;
    float oneMinustCubed = oneMinust * oneMinust * oneMinust;
    float tSquared = parameter * parameter;
    float tCubed = parameter * parameter * parameter;

    // Calculate each term of the bezier for each control point
    Homogeneous4 term1 = oneMinustCubed * controlPoint1;
    Homogeneous4 term2 = 3 * parameter * oneMinustSquared * controlPoint2;
    Homogeneous4 term3 = 3 * tSquared * oneMinust * controlPoint3;
    Homogeneous4 term4 = tCubed * controlPoint4;

    // Return sum of terms
    return term1 + term2 + term3 + term4;
}

void BezierPatchRenderWidget::drawLine(Point3 start, Point3 end, RGBAValue colour) {
    // Find difference between end and start point of line
    Vector3 difference = end - start;
    
    // Loop over parameter t
    for (float t = 0.0f; t < 1.0f; t += 0.001f) {
        Homogeneous4 pointOnLine(Point3(start + difference * t)); // Find point travelled along line based on t and convert to Homogeneous 4 for transformation
        #pragma omp critical 
        {
            fragments.emplace_back(Fragment{pointOnLine, mvpMatrix, colour});
        }
        //frameBuffer.setPixel(pointOnLine, mvpMatrix, colour); // Set pixel at given point along line
    }
}

void BezierPatchRenderWidget::drawPoint(Point3 point, RGBAValue colour) {
    // Form a translation matrix with the translation being the point we want to draw the vertex at
    Matrix4 translationMatrix = Matrix4();
    translationMatrix.SetTranslation(Vector3(point.x, point.y, point.z));
    
    // Calculate the transformation matrix to convert world space coordinate to clip space
    Matrix4 pointMatrix = projectionMatrix * viewMatrix * translationMatrix;
    
    float radius = 0.1f;

    // Loop over phi and theta to construct points in a spherical shape
    for (float phi = 0.0f; phi < 2.0f * PI; phi += PI / 30.0f) {
        for (float theta = 0.0f; theta < 2.0f * PI; theta += PI / 30.0f) {
            // Homogeneous point of a single point of the sphere based near the origin
            // (will be translated to the point on the net when transformed by pointMatrix)
            Homogeneous4 point(Point3(
                radius * std::cos(phi) * std::cos(theta),  
                radius * std::cos(phi) * std::sin(theta),
                radius * std::sin(phi)));

            #pragma omp critical 
            {
                fragments.emplace_back(Fragment{point, pointMatrix, colour});
            }
            
            // Set pixel of sphere points
            //frameBuffer.setPixel(point, pointMatrix, colour);
        }
    }
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
