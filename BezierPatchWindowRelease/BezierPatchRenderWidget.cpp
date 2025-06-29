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
#include <windows.h>
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

    // Get start time of frame
    auto start = std::chrono::steady_clock::now();

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

    // Set _near and _far planes
    float _near = 0.01f;
    float _far = 200.0f;
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
        projectionMatrix[2][2] = -2.0f / (_far - _near);
        projectionMatrix[0][3] = -(right + left) / (right - left);
        projectionMatrix[1][3] = -(top + bottom) / (top - bottom);
        projectionMatrix[2][3] = -(_far * _near) / (_far - _near);
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
        projectionMatrix[0][0] = (2.0f * _near) / (right - left);
        projectionMatrix[1][1] = (2.0f * _near) / (top - bottom);
        projectionMatrix[2][2] = -(_far + _near) / (_far - _near);
        projectionMatrix[3][3] = 0.0f;
        projectionMatrix[0][2] = (right + left) / (right / left);
        projectionMatrix[1][2] = (top + bottom) / (top - bottom);
        projectionMatrix[3][2] = -1.0f;
        projectionMatrix[2][3] = -(2.0f * _far * _near) / (_far - _near);
    }

    // Model-view-projection matrix
    mvpMatrix.SetIdentity();
    mvpMatrix = projectionMatrix * viewMatrix; // Combine projection and view matrix for transforming to clip space
    
    if(renderParameters->verticesEnabled)
    {// UI control for showing vertices

        // If vertices are enabled, reserve memory to fragments to the number
        // of fragments we would generate to reduce automatic memory reallocation.
        fragments.reserve(1048);

        // In the same vein as the reasoning stated for why the drawLine loops are not
        // parallelised, is the same for this one. Since we are only iterating over 16 vertices,
        // each of which call drawPoint which iterates over 100 pixels, only 1600 pixels
        // are being computed, which is not in region in which the cost of invoking #pragma omp parallel for
        // would be worth the performance gained by parallelising this loop.
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

        // If planes are enabled reserve memory to fragments to current size plus the number
        // of fragments we would generate to reduce automatic memory reallocation
        fragments.reserve(fragments.size() + 46046);

        // Planes are axis aligned grids made up of lines

        // I don't parallelise the loops that call drawLine, since the point at which
        // line calculation starts being the bottleneck (i.e. greater cost than the overhead of openmp threads)
        // is at 1 million loop iterations, however we are only doing 1000, as such it is counter intuitive to
        // parallelise the loops when the cost of overhead + parallel calculation is higher than just serial calculation in this case. 
        for (int i = -5; i <= 5; i+=2) {
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

        // If net is enabled reserve memory to fragments to current size plus the number
        // of fragments we would generate to reduce automatic memory reallocation
        fragments.reserve(fragments.size() + 24024);

        // Reasoning for not parallelising these loops is as stated previously in the planes loop,
        // more so with these since even fewer points are being calculated
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                // Draw horizontal lines between control points
                Point3 horzControlPoint1(                           // First control point
                    (*patchControlPoints).vertices[i*4+j][0],
                    (*patchControlPoints).vertices[i*4+j][1],
                    (*patchControlPoints).vertices[i*4+j][2]);
                Point3 horzControlPoint2(                           // Second control point
                    (*patchControlPoints).vertices[i*4+j+1][0],
                    (*patchControlPoints).vertices[i*4+j+1][1],
                    (*patchControlPoints).vertices[i*4+j+1][2]);
                
                // Draw net line between
                drawLine(horzControlPoint1, horzControlPoint2, RGBAValue(0.0f, 255.0f, 0.0f, 255.0f));

                // Draw the vertical lines between control points
                Point3 vertControlPoint1(                            // First control point
                    (*patchControlPoints).vertices[i+j*4][0],
                    (*patchControlPoints).vertices[i+j*4][1],
                    (*patchControlPoints).vertices[i+j*4][2]);
                Point3 vertControlPoint2(                            // Second control point
                    (*patchControlPoints).vertices[i+4+j*4][0],
                    (*patchControlPoints).vertices[i+4+j*4][1],
                    (*patchControlPoints).vertices[i+4+j*4][2]);
                
                // Draw net line between
                drawLine(vertControlPoint1, vertControlPoint2, RGBAValue(0.0f, 255.0f, 0.0f, 255.0f));
            }
        }

    }// UI control for showing the Bezier control net

    if(renderParameters->bezierEnabled)
    {// UI control for showing the Bezier curve
        // Get the control points in a variable with shorter name for ease of reading
        std::vector<Point3> controlPoints = renderParameters->patchControlPoints->vertices;

        head = fragments.size();
        // If bezier is enabled resize fragments to current size plus the number
        // of fragments we would generate to reduce automatic memory reallocation.
        // I use resize specifically here as resize not only reserves memory but also
        // default constructs elements in the new space so accessing positions with the
        // [] operator is valid
        fragments.resize(fragments.size() + 1002001);

        #pragma omp parallel for
        for (int s = 0; s <= 1000; s++) // for loop parameter needs to be int for omp (remember to float cast and divide later)
        {// s parameter loop
            for (float t = 0.0; t <= 1.0; t += 0.001)
            { // t parameter loop

            // Find coordinates from each bezier curve at t parameter
            Homogeneous4 bezier1 = bezier(t, controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
            Homogeneous4 bezier2 = bezier(t, controlPoints[4], controlPoints[5], controlPoints[6], controlPoints[7]);
            Homogeneous4 bezier3 = bezier(t, controlPoints[8], controlPoints[9], controlPoints[10], controlPoints[11]);
            Homogeneous4 bezier4 = bezier(t, controlPoints[12], controlPoints[13], controlPoints[14], controlPoints[15]);

            // Find final point using the previous 4 points as points for a final bezier curve with s parameter
            Homogeneous4 finalPoint = bezier((float)s / 1000.0f, bezier1, bezier2, bezier3, bezier4);

            // Transform the point to screen space
            Point3 screenPoint = transformPoint(finalPoint);

            // Calculate index for each fragment to get a unique memory location
            // so no two threads try to write to the same index and cause a write collision
            int index = head + (s * 1001 + (t / 0.001f));
            fragments[index] = Fragment{screenPoint, RGBAValue(255.0f * (float)s / 1000.0f, 255.0f / 2, 255.0f * t, 255.0f)};

            } // t parameter loop
        } // s parameter loop
    }
    
    auto sortStart = std::chrono::steady_clock::now();

    if (!fragments.empty()) // Fragments will be empty if all toggles are turned off
        std::sort(fragments.begin(), fragments.end(), lessFunctor); // Sort fragments based on lessFunctor sorting (Painter's algorithm)

    // Draw every fragment, will be ordered so multiple pixels at same location will be drawn back to front
    if (!fragments.empty()) { // fragments will be empty when all toggles are off, so we need to check before iterating over
        for (int i = 1; i < fragments.size() - 1; i++) {
            // When either the x or y changes from the previous fragment, the previous fragment will be the front most fragment for that pixel
            if (fragments[i].point.x != fragments[i - 1].point.x || fragments[i].point.y != fragments[i - 1].point.y)
                frameBuffer.setPixel(fragments[i - 1].point, fragments[i - 1].colour);
        }
    }

    fragments.clear(); // Clear fragments so we don't get artifacts next frame

    auto end = std::chrono::steady_clock::now();
    auto timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken: " << timeTaken.count() / 1000000.0f << " seconds." << std::endl << std::endl;

    // Put the custom framebufer on the screen to display the image
    glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.block);

} // BezierPatchRenderWidget::paintGL()

// Function to transform a point from world space to clip space, and to do the necessary clipping check
// so vertices that are behind the camera don't reappear back in front of it.
Point3 BezierPatchRenderWidget::transformPoint(Homogeneous4 point) {
    // Transform the point from world space to clip space
    Homogeneous4 transformedPoint = mvpMatrix * point;

    // Clipping
    if (-transformedPoint.w > transformedPoint.x || transformedPoint.x > transformedPoint.w ||
        -transformedPoint.w > transformedPoint.y || transformedPoint.y > transformedPoint.w ||
        -transformedPoint.w > transformedPoint.z || transformedPoint.z > transformedPoint.w ||
        transformedPoint.w < 0.0f)
        return Point3(-1, -1, -1); // return an invalid point so when it gets to setPixel it will be discarded

    // Perspective divide (clip space to normalised device space)
    Point3 ndcs(transformedPoint.Point());

    // Viewport transformation (normalised device space to screen space)
    float screenCoordx = (ndcs.x + 1) / 2 * frameBuffer.width;
    float screenCoordy = (ndcs.y + 1) / 2 * frameBuffer.height;
    float screenCoordz = ndcs.z; // Keep z so we can do Painter's algorithm later

    return Point3(screenCoordx, screenCoordy, screenCoordz); // Return the screen point
}

// Function to calculate a bezier point based on a input parameter and 4 control points
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

// Function to draw a line given a start and end point.
void BezierPatchRenderWidget::drawLine(Point3 start, Point3 end, RGBAValue colour) {
    // Find difference between end and start point of line
    Vector3 difference = end - start;
    
    // Loop over parameter t
    // I don't parallelise this loop, since this function gets called many times a frame, 
    // the overhead of creating and destroying threads if this loop was parallelised slows 
    // down the run time drastically, and for the amount we iterate over this loop, doing this loop in serial is quicker.
    for (float t = 0.0f; t < 1.0f; t += 0.001f) {
        // Find point travelled along line based on t and convert to Homogeneous4 for transformation
        Homogeneous4 pointOnLine(Point3(start + difference * t));

        // Transform the point to screen space
        Point3 screenPoint = transformPoint(pointOnLine);

        // Put the screenPoint and colour in fragments to be sorted later
        fragments.emplace_back(Fragment{screenPoint, colour});
    }
}

// Function to draw a vertex as a point as a circle
// (Has side effect of staying as a set size regardless of zoom factor)
void BezierPatchRenderWidget::drawPoint(Point3 point, RGBAValue colour) {
    Point3 screenPoint = transformPoint(Homogeneous4(point)); // Transform point to screen space

    int radius = 5; // Radius of point in pixels
    // Loop over a square of side lengths 2 * radius around the point
    for (int x = screenPoint.x - radius; x < screenPoint.x + radius; x++) {
        for (int y = screenPoint.y - radius; y < screenPoint.y + radius; y++) {
            int nX = x - screenPoint.x; // x distance from point
            int nY = y - screenPoint.y; // y distance from point
            // Check distance from point center to get points in a circle
            // use square values to avoid doing sqrt
            if ((nX * nX + nY * nY) < radius * radius) {
                // Put new calculated point in fragments (whilst preserving the z value)
                fragments.emplace_back(Fragment{Point3(x, y, screenPoint.z), colour});
            }
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
