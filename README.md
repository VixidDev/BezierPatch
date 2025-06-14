# Bézier Patch Visualiser

Bézier patch visualiser which calculates multiple bézier curves to form a patch from a control net given in an input file. The left side is my manual implementation of the bézier patch and on the right is using inbuilt legacy OpenGL calls to render a patch for comparison.

All rendering is done from scratch including the projection matrices, lines and axis, and sorting the pixels back to front using Painter's Algorithm, along with the full transformation from object space to screen space. Rendering is done very simply by setting the pixel color for each pixel on the screen and using `glDrawPixels` to render to the screen.

Since this manual method is very computationally heavy and done soley on the CPU, OpenMP is utilised along with compiler flags such as `-O3` and `-D_GLIBCXX_PARALLEL` to speed up standard library methods such as `std::sort` and general compiler optimisations.

![image](https://github.com/user-attachments/assets/d1b9c2ea-d301-4f2d-a417-94df63a57c27)

## Usage

To compile on linux:

Qt version should be 6.0+. QMake version should be 3.1+

Create Makefiles with:
```bash
qmake -project "QT += core gui widgets opengl openglwidgets" "QMAKE_CXXFLAGS+= -fopenmp -Wall" "LIBS += -lGL -lGLU -fopenmp"
qmake
make
```

To compile on Windows:

Install QT 5.13.0 or newer (6.0+)

Create .pro file with:
```bash
qmake -project "QT += core gui widgets opengl openglwidgets" "LIBS += -lOpengl32 -lglu32 -fopenmp"
```

Either open the .pro file in QtCreator and set additional flags to: `QMAKE_CXXFLAGS+= -fopenmp -Wall -O3 -D_GLIBCXX_PARALLEL`

Or use Qt Visual Studio Tools plugin in Visual Studio and import the .pro file to convert it to a Visual Studio solution. If you use this solution it is probably best to add the `QMAKE_CXXFLAGS+= -fopenmp -Wall -O3 -D_GLIBCXX_PARALLEL` flags to the .pro file beforehand.

Once the project is either imported in QtCreator or Visual Studio, run with the program argument `../input/patch.txt` with the run directory being `BezierPatchWindowRelease`.
