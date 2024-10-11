//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////

// system libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// QT
#include <QApplication>

// local includes
#include "RenderWindow.h"
#include "ControlPoints.h"
#include "RenderParameters.h"
#include "RenderController.h"


// main routine
int main(int argc, char **argv)
    { // main()

    // initialize QT
    QApplication renderApp(argc, argv);

    // check the args to make sure there's an input file
    if (argc != 2)
        { // bad arg count
        // print an error message
        std::cout << "Usage: " << argv[0] << " textfile (.txt) containing the control points" << std::endl;
        // and leave
        return 0;
        } // bad arg count


    ControlPoints bezierPatch;

    // open the input files for the geometry & texture
    std::ifstream geometryFile(argv[1]);

    // try reading it
    if (!(geometryFile.good()))
    {
        std::cout << "Read failed for object " << argv[1] << std::endl;
        return 0;
    } // object read failed

    bezierPatch = ControlPoints::ReadPointStream(geometryFile);

    if(bezierPatch.vertices.size() == 0){
        std::cout << "Read failed for control points " << argv[1] << std::endl;
        return 0;
    } // object read failed

    // create some default render parameters
    RenderParameters renderParameters(&bezierPatch);

    // use the object & parameters to create a window
    RenderWindow renderWindow(&bezierPatch, &renderParameters, argv[1]);

    // create a controller for the window
    RenderController renderController(&bezierPatch, &renderParameters, &renderWindow);

    //  set the initial size
    renderWindow.resize(1600, 720);

    // show the window
    renderWindow.show();

    // set QT running
    return renderApp.exec();
    } // main()
