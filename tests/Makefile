CC = g++

FILES = main.cpp \
	../BezierPatchWindowRelease/Point3.h \
	../BezierPatchWindowRelease/Point3.cpp \
	../BezierPatchWindowRelease/Vector3.h \
	../BezierPatchWindowRelease/Vector3.cpp \
	../BezierPatchWindowRelease/Homogeneous4.h \
	../BezierPatchWindowRelease/Homogeneous4.cpp \
	../BezierPatchWindowRelease/Matrix4.h \
	../BezierPatchWindowRelease/Matrix4.cpp \
	../BezierPatchWindowRelease/Quaternion.h \
	../BezierPatchWindowRelease/Quaternion.cpp

testLibrary:
	${CC} ${FILES} -o testLibrary

clean:
	rm testLibrary
