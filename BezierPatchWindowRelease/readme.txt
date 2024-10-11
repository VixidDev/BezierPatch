To compile on feng-linux / feng-gps / lab machines:

check the current qt version with:
qmake --version

qt version should be 6.0+:
QMake version 3.1
Using Qt version 6.6.2 in /usr/lib64

then create the Makefile with:
qmake -project "QT+=core gui widgets opengl openglwidgets" "QMAKE_CXXFLAGS+= -fopenmp -Wall" "LIBS += -lGL -lGLU -fopenmp"
qmake
make
--------------------------------------

To compile on Windows:
Install QT 5.13.0 or newer (6.0+)
It can be found here: 
	https://download.qt.io/official_releases/qt/6.6/
	https://download.qt.io/official_releases/qtcreator/14.0/14.0.1/
	
	or older versions in the qt archive:
	https://download.qt.io/archive/qt/5.13/5.13.0/qt-opensource-windows-x86-5.13.0.exe.mirrorlist

Update graphics card driver
Double click BezierPatchWindow_win.pro to open in QTCreator (or import the .pro via the "File" menu)
Running with Qt Creator, make sure the qmake variables are set to:
QT+= core gui opengl
QMAKE_CXXFLAGS+= -fopenmp -Wall
LIBS += -lOpengl32 -lfreeglut -lglu32 -lglew32 -fopenmp
TEMPLATE = app
TARGET = BezierPatchWindowRelease
INCLUDEPATH += .

Alternativelly, compile manually on Windows via:
qmake -project "QT+= core gui opengl" "LIBS += -lOpengl32 -lfreeglut -lglu32 -lglew32 -fopenmp"


Select the platform to compile to (32 or 64 bits)
Click details to select the build folder
Click Configure Project (the spanner icon titled "Projects"
In the "Run" of either the 32-bit or 64-bit configuration, set "Command line arguments" to:
../input/patch.txt
--------------------------------------

To run on feng-linux / feng-gps:
./BezierPatchWindowRelease ../path_to/model.txt
for this assignment should be:
./BezierPatchWindowRelease ../input/patch.txt

---------------------------------------------
To run on Windows
./BezierPatchWindowRelease.exe ../path_to/model.txt
for this assignment should be:
./BezierPatchWindowRelease ../input/patch.txt

or
Click projects
Select "Run" on the left side menu under the active configuration
Add "../input/patch.txt to command line arguments
Click Run/Debug on the bottom left side.
