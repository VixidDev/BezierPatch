//////////////////////////////////////////////////////////////////////
//
//  Since the render code is in the Bezier patch control points class
//  this widget primarily sets up the transformation matrices
//
//  It implements the UI for an arcball controller, but in an
//  abstractable way that allows it to share an arcball with another
//  widget (the arcball controller) - thus you can manipulate the
//  object either directly in the widget or indirectly through the
//  arcball displayed visually.
//
//  It also supports translation by visual dragging.  This results in
//  needing three mouse buttons, and not all systems have them, so it
//  will be up to the user to make sure that they can use it on their
//  own machine (especially on OSX)
//
//  Since the controls are (potentially) shared with other widgets,
//  this widget is only responsible for scaling the x,y of mouse events
//  then passing them to the controller
//
////////////////////////////////////////////////////////////////////////

#include <math.h>

// include the header file
#include "RenderWidget.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

//	Ken Shoemake's ArcBall
//#include "BallAux.h"
#include "ArcBall.h"

#ifndef PI
#define PI 3.14159265358979
#endif

// width and height of window, plus initial value
int windowSize = 640;
int windowWidth, windowHeight;


// constructor
RenderWidget::RenderWidget
        (
        // the Bezier patch control points to show
        ControlPoints       *newPatchControlPoints,
        // the render parameters to use
        RenderParameters    *newRenderParameters,
        // parent widget in visual hierarchy
        QWidget             *parent
        )
    // the : indicates variable instantiation rather than arbitrary code
    // it is considered good style to use it where possible
    :
    // start by calling inherited constructor with parent widget's pointer
    QOpenGLWidget(parent),
    // then store the pointers that were passed in
    patchControlPoints(newPatchControlPoints),
    renderParameters(newRenderParameters)
    { // constructor
      // set the strong focus policy for enabling the widget to accept keyboard input
        this->setFocusPolicy(Qt::StrongFocus);
    } // constructor

// destructor
RenderWidget::~RenderWidget()
    { // destructor
    // empty (for now)
    // all of our pointers are to data owned by another class
    // so we have no responsibility for destruction
    // and OpenGL cleanup is taken care of by Qt
    } // destructor

// called when OpenGL context is set up
void RenderWidget::initializeGL()
    { // RenderWidget::initializeGL()

    glEnable(GL_DEPTH_TEST);

    // set shading parameters (may be reset later)
    glShadeModel(GL_SMOOTH);

    // background is black!
    glClearColor(0, 0, 0, 1.0);

    // rotate slightly to make grid visible
    glRotatef(7.0, 1.0, 1.0, 0.0);

    // // initialize the arcball
    ArcBall arcballCamera();

    // set up evaluators
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

    } // RenderWidget::initializeGL()

// called every time the widget is resized
void RenderWidget::resizeGL(int w, int h)
    { // RenderWidget::resizeGL()

    renderParameters->windowWidth = w;
    renderParameters->windowHeight = h;
    // reset the viewport
    glViewport(0, 0, w, h);

    // set projection matrix to be glOrtho based on zoom & window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // compute the aspect ratio of the widget
    double aspectRatio = double(w) / double(h);

    // we want to capture a sphere of radius 1.0 without distortion
    // so we set the ortho projection based on whether the window is portrait (> 1.0) or landscape
    // portrait ratio is wider, so make bottom & top -1.0 & 1.0
    if (aspectRatio > 1.0)
    {
        if(renderParameters->orthoProjection)
        {
            glOrtho(-aspectRatio * 10/renderParameters->zTranslate, aspectRatio * 10/renderParameters->zTranslate,
                    -10/renderParameters->zTranslate, 10/renderParameters->zTranslate,
                    0.01, 200.0);
        }
        else
        {
            glFrustum (-aspectRatio * 0.01, aspectRatio * 0.01,
                      -0.01, 0.01,
                      0.01, 200.0);
        }
    }
    // otherwise, make left & right -1.0 & 1.0
    else
    {
        if(renderParameters->orthoProjection)
        {
            glOrtho(-10/renderParameters->zTranslate, 10/renderParameters->zTranslate,
                    -aspectRatio * 10/renderParameters->zTranslate, aspectRatio * 10/renderParameters->zTranslate,
                    0.01, 200.0);
        }
        else
        {
            glFrustum ( -0.01, 0.01,
                      -aspectRatio * 0.01, aspectRatio * 0.01,
                      0.01, 200.0);
        }
    }

    } // RenderWidget::resizeGL()

// called every time the widget needs painting

void RenderWidget::paintGL()
 { // RenderWidget::paintGL()
    int i, j, k;

    // if the projection/ortho matrix checkbox has been toggled:
    if(renderParameters->triggerResize)
    {
        this->resizeGL(renderParameters->windowWidth, renderParameters->windowHeight);
        renderParameters->triggerResize = false;
    }

    // set the clear colour and clear the frame buffer
    glClearColor(0.0, 0.0, 0.0, 1.25);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the modelview
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // apply camera view - sets up gluLookAt:
    gluLookAt(0.0, 0.0, 8.0, // 8.0 is the starting zoom level
              0.0, 0,0,
              0.0, 1.0, 0.0);

    // translate from the horizontal and vertical QT slider interaction
    // Let us center the scene on -1 so we can have a camera with near 0 and far 2 ...
    // ... without having to do many changes
    glTranslatef(renderParameters->xTranslate, renderParameters->yTranslate, renderParameters->zTranslate-1);

    // apply rotation matrix from arcball
    glMultMatrixf(renderParameters->rotationMatrix.columnMajor().coordinates);

    if(renderParameters->planesEnabled)
    { // showPlanes
        // set the drawing colour
        glColor3f(0.25, 0.0, 0.25);

        // draw the ground planes
        glBegin(GL_LINES);
        for (i = -5; i <= 5; i+=2)
        {
            glVertex3f(i, 0, -5);
            glVertex3f(i, 0, 5);
            glVertex3f(-5, 0, i);
            glVertex3f(5, 0, i);
        }
        glEnd();

        glColor3f(0.0, 0.25, 0.25);
        glBegin(GL_LINES);
        for (i = -5; i <= 5; i+=2)
        {
            glVertex3f(0, i, -5);
            glVertex3f(0, i, 5);
            glVertex3f(0, -5, i);
            glVertex3f(0, 5, i);
        }
        glEnd();

        glColor3f(0.25, 0.25, 0.0);
        glBegin(GL_LINES);
        for (i = -5; i <= 5; i++)
        {
            glVertex3f(-5, i, 0);
            glVertex3f(5, i, 0);
            glVertex3f(i, -5, 0);
            glVertex3f(i, 5, 0);
        }
        glEnd();
    } // showPlanes


    // now render the control vertices
    if(renderParameters->verticesEnabled)
    { //  showVertices
        glMatrixMode(GL_MODELVIEW);
        // for each control vertex
        for (int id = 0; id < 16; id++)
        { // for id
            if (id == renderParameters->activeVertex)
                glColor3f(1.0, 0.0, 0.0);
            else
                glColor3f(0.75, 0.75, 0.75);
            glPushMatrix();
            glTranslatef((*patchControlPoints).vertices[(id/4)*4+(id%4)][0],
                         (*patchControlPoints).vertices[(id/4)*4+(id%4)][1],
                         (*patchControlPoints).vertices[(id/4)*4+(id%4)][2]);

            // draw spheres for where the control point vertices area
            // (for simplicity, draw the spheres from small points close together)
            double radius = 0.1;
            glBegin(GL_POINTS);
            for (float phi = 0.0; phi < 2.f*PI; phi += PI / 30.0)
                for (float theta = 0.0; theta < 2.f*PI; theta += PI / 30.0)
                    glVertex3f(radius * cos(phi) * cos(theta), radius * cos(phi) * sin(theta), radius * sin(phi));
            glEnd();

            glPopMatrix();
        } // for id
    } // showVertices

    if(renderParameters->netEnabled)
    { // showNet
        // now draw control net
        glColor3f(0.0, 1.0, 0.0);
        for (int i = 0; i < 4; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < 4; j++)
                glVertex3f((*patchControlPoints).vertices[i*4+j][0],
                           (*patchControlPoints).vertices[i*4+j][1],
                           (*patchControlPoints).vertices[i*4+j][2]);
            glEnd();
        }
        for (int j = 0; j < 4; j++)
        {
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < 4; i++)
                glVertex3f((*patchControlPoints).vertices[i*4+j][0],
                           (*patchControlPoints).vertices[i*4+j][1],
                           (*patchControlPoints).vertices[i*4+j][2]);
            glEnd();
        }

    } // showNet

    if(renderParameters->bezierEnabled)
    { // showBezier
        // now we want to draw the Bezier patch with evaluators
        // pre-compute the colours for the patch (4x4) mesh:
        GLfloat bezierPatchCols[4][4][4];
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                bezierPatchCols[j][i][0]=0.25f * (float)j; // red = s
                bezierPatchCols[j][i][1]=0.5f; // leave as constant green
                bezierPatchCols[j][i][2]=0.25f * (float)i; // blue = t
                bezierPatchCols[j][i][3]=1.0f;
            }
        }

        // enable evaluator colours (4 channels RGBA)
        glEnable(GL_MAP2_COLOR_4);
        // enable vertices (stride 3 for x, y, z)
        glEnable(GL_MAP2_VERTEX_3);

        glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

        glMap2f(GL_MAP2_COLOR_4,			//	2 manifold in 3D
                0, 1, 4, 4,						//	0 .. 1 u, step by 4, deg. 4
                0, 1, 16, 4,					//  0 .. 1 v, step by 16, deg. 4
                &bezierPatchCols[0][0][0]);

        glMap2f(GL_MAP2_VERTEX_3,			//	2 manifold in 3D
                0, 1, 3, 4,						//	0 .. 1 u, step by 3, deg. 4
                0, 1, 12, 4,					//  0 .. 1 v, step by 12, deg. 4
                &((*patchControlPoints).vertices[0][0]));		//	input data

        glEvalMesh2(GL_FILL, 0, 20, 0, 20);

        glEnable(GL_AUTO_NORMAL);
        glEnable(GL_NORMALIZE);

    } // showBezier

    // make sure we pop our matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}


void RenderWidget::forceRepaint(){
    update();
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Less:
            // iterate active vertex (which control point is allowed to be moved)
            renderParameters->activeVertex = (renderParameters->activeVertex-1) % renderParameters->patchControlPoints->vertices.size();
        break;

    case Qt::Key_Greater:
            // iterate active vertex (which control point is allowed to be moved)
            renderParameters->activeVertex = (renderParameters->activeVertex+1) % renderParameters->patchControlPoints->vertices.size();
        break;

    case Qt::Key_Left:
            // move the active vertex
            renderParameters->patchControlPoints->vertices[renderParameters->activeVertex].x -= 0.1f;
        break;

    case Qt::Key_Right:
            // move the active vertex
            renderParameters->patchControlPoints->vertices[renderParameters->activeVertex].x += 0.1f;
        break;

    case Qt::Key_Up:
            // move the active vertex
            renderParameters->patchControlPoints->vertices[renderParameters->activeVertex].y += 0.1f;
        break;

    case Qt::Key_Down:
            // move the active vertex
            renderParameters->patchControlPoints->vertices[renderParameters->activeVertex].y -= 0.1f;
        break;

    case Qt::Key_Plus:
            // move the active vertex
            renderParameters->patchControlPoints->vertices[renderParameters->activeVertex].z += 0.1f;
        break;

    case Qt::Key_Minus:
            // move the active vertex
            renderParameters->patchControlPoints->vertices[renderParameters->activeVertex].z -= 0.1f;
        break;

    case Qt::Key_P:
            // show axis-aligned planes
        renderParameters->planesEnabled = !renderParameters->planesEnabled;
        break;

    case Qt::Key_V:
            // toggle vertices (control points)
        renderParameters->verticesEnabled = !renderParameters->verticesEnabled;
        break;

    case Qt::Key_N:
            // toggle the control point net
        renderParameters->netEnabled = !renderParameters->netEnabled;
        break;

    case Qt::Key_B:
            // toggle the bezier patch
        renderParameters->bezierEnabled = !renderParameters->bezierEnabled;
        break;

    }

    this->forceRepaint();

}

// mouse-handling
void RenderWidget::mousePressEvent(QMouseEvent *event)
    { // RenderWidget::mousePressEvent()
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
    } // RenderWidget::mousePressEvent()

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
    { // RenderWidget::mouseMoveEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;

    // send signal to the controller for detailed processing
    emit ContinueScaledDrag(x,y);
    } // RenderWidget::mouseMoveEvent()

void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
    { // RenderWidget::mouseReleaseEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y() ) / size;

    // send signal to the controller for detailed processing
    emit EndScaledDrag(x,y);
    } // RenderWidget::mouseReleaseEvent()
