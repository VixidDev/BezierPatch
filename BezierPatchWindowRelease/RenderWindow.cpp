/////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  October, 2024
//
//  -----------------------------
//  Render Window
//  -----------------------------
//
//  The render window class is really just a container
//  for tracking the visual hierarchy.  While it is
//  entirely possible to use Qt Creator, I try to avoid
//  over-commitment to it because I need to write code in
//  multiple environments, some of which are not well-suited
//  to IDEs in general, let alone Qt Creator
//
//  Also, by providing sample code, the didactic purpose of
//  showing how things fit together is better served.
//
/////////////////////////////////////////////////////////////////

#include "RenderWindow.h"
#include "RenderParameters.h"

// constructor
RenderWindow::RenderWindow
        (
        // the object to be rendered
        ControlPoints          *newPatchControlPoints,
        // the model object storing render parameters
        RenderParameters        *newRenderParameters,
        // the title for the window (with default value)
        const char              *windowName
        )
    // call the inherited constructor
    // NULL indicates that this widget has no parent
    // i.e. that it is a top-level window
    :
    // member instantiation
    QWidget(NULL),
    patchControlPoints(newPatchControlPoints),
    renderParameters(newRenderParameters)
    { // RenderWindow::RenderWindow()
    // set the window's title
    setWindowTitle(QString(windowName));

    // initialise the grid layout
    windowLayout = new QGridLayout(this);

    // create all of the widgets, starting with the custom render widgets
    renderWidget                = new RenderWidget              (newPatchControlPoints,     newRenderParameters,        this);
    bezierPatchRenderWidget        = new BezierPatchRenderWidget      (newPatchControlPoints,     newRenderParameters,        this);

    // construct custom arcball Widgets
    modelRotator                = new ArcBallWidget             (                       this);

    // construct standard QT widgets
    // check boxes
    showPlanesBox        = new QCheckBox                 ("Show Planes",            this);
    showNetBox           = new QCheckBox                 ("Show Net",            this);
    showVerticesBox      = new QCheckBox                 ("Show Vertices",            this);
    showBezierBox        = new QCheckBox                 ("Show Bezier",            this);
    orthoBox             = new QCheckBox                 ("Orthographic Projection",            this);


    // spatial sliders
    xTranslateSlider            = new QSlider                   (Qt::Horizontal,        this);
    secondXTranslateSlider      = new QSlider                   (Qt::Horizontal,        this);
    yTranslateSlider            = new QSlider                   (Qt::Vertical,          this);
    zTranslateSlider            = new QSlider                   (Qt::Vertical,          this);



    // labels for sliders and arcballs
    modelRotatorLabel           = new QLabel                    ("Model",               this);
    yTranslateLabel             = new QLabel                    ("Y",                   this);
    zoomLabel                   = new QLabel                    ("Zm",                  this);


    // the top two widgets have to fit to the widgets stack between them
    int nStacked = 13;


    windowLayout->addWidget(bezierPatchRenderWidget,               0,          1,          nStacked,   1           );
    windowLayout->addWidget(yTranslateSlider,           0,          2,          nStacked,   1           );
    windowLayout->addWidget(zTranslateSlider,                 0,          4,          nStacked,   1           );
    windowLayout->addWidget(renderWidget,       0,          5,          nStacked,   1           );

    // the stack in the middle
    windowLayout->addWidget(modelRotator,               0,          3,          1,          1           );
    windowLayout->addWidget(modelRotatorLabel,          1,          3,          1,          1           );
    windowLayout->addWidget(showPlanesBox,           2,         3,          1,          1           );
    windowLayout->addWidget(showNetBox,            3,         3,          1,          1           );
    windowLayout->addWidget(showVerticesBox,                  4,         3,          1,          1           );
    windowLayout->addWidget(showBezierBox,              5,         3,          1,          1           );
    windowLayout->addWidget(orthoBox,              6,         3,          1,          1           );

    // Translate Slider Row
    windowLayout->addWidget(xTranslateSlider,           nStacked,   1,          1,          1           );
    windowLayout->addWidget(yTranslateLabel,            nStacked,   2,          1,          1           );
    // nothing in column 3
    windowLayout->addWidget(zoomLabel,                  nStacked,   4,          1,          1           );
    windowLayout->addWidget(secondXTranslateSlider,     nStacked,   5,          1,          1           );


    // now reset all of the control elements to match the render parameters passed in
    ResetInterface();
    } // RenderWindow::RenderWindow()

// routine to reset interface
// sets every visual control to match the model
// gets called by the controller after each change in the model
void RenderWindow::ResetInterface()
    { // RenderWindow::ResetInterface()
    // set check boxes
    showPlanesBox       ->setChecked        (renderParameters   ->  planesEnabled);
    showVerticesBox     ->setChecked        (renderParameters   ->  verticesEnabled);
    showNetBox          ->setChecked        (renderParameters   ->  netEnabled);
    showBezierBox       ->setChecked        (renderParameters   ->  bezierEnabled);
    orthoBox            ->setChecked        (renderParameters   ->  orthoProjection);


    // set sliders
    // x & y translate are scaled to notional unit sphere in render widgets
    // but because the slider is defined as integer, we multiply by a 100 for all sliders
    xTranslateSlider        ->setMinimum        (int(TRANSLATE_MIN                               * PARAMETER_SCALING));
    xTranslateSlider        ->setMaximum        (int(TRANSLATE_MAX                               * PARAMETER_SCALING));
    xTranslateSlider        ->setValue          (int(renderParameters -> xTranslate              * PARAMETER_SCALING));

    secondXTranslateSlider  ->setMinimum        (int(TRANSLATE_MIN                               * PARAMETER_SCALING));
    secondXTranslateSlider  ->setMaximum        (int(TRANSLATE_MAX                               * PARAMETER_SCALING));
    secondXTranslateSlider  ->setValue          (int(renderParameters -> xTranslate              * PARAMETER_SCALING));

    yTranslateSlider        ->setMinimum        (int(TRANSLATE_MIN                               * PARAMETER_SCALING));
    yTranslateSlider        ->setMaximum        (int(TRANSLATE_MAX                               * PARAMETER_SCALING));
    yTranslateSlider        ->setValue          (int(renderParameters -> yTranslate              * PARAMETER_SCALING));

    // zoom slider is a logarithmic scale, so we want a narrow range
    zTranslateSlider  ->setMinimum        (int(TRANSLATE_MIN                               * PARAMETER_SCALING));
    zTranslateSlider  ->setMaximum        (int(TRANSLATE_MAX                               * PARAMETER_SCALING));
    zTranslateSlider  ->setValue          (int(renderParameters -> zTranslate              * PARAMETER_SCALING));

    // now flag them all for update
    renderWidget            ->update();
    bezierPatchRenderWidget    ->update();
    modelRotator            ->update();
    xTranslateSlider        ->update();
    secondXTranslateSlider  ->update();
    yTranslateSlider        ->update();
    zTranslateSlider        ->update();
    showPlanesBox        ->update();
    showNetBox         ->update();
    showPlanesBox        ->update();
    showVerticesBox               ->update();
    showBezierBox           ->update();
    orthoBox           ->update();

    } // RenderWindow::ResetInterface()

