TEMPLATE = app
TARGET   = raytracer
CONFIG  += qt opengl xml warn_on console release thread
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS +=  -fopenmp
QT *= opengl xml
HEADERS = Window.h \
          GLViewer.h \
          QTUtils.h \
          Vertex.h \
          Triangle.h \
          Mesh.h \
          BoundingBox.h \
          Material.h \
          Object.h \
          Light.h \
          Scene.h \
          RayTracer.h \
          Ray.h \
    KdTree.h \
    Leaf.h \
    SimpleNode.h \
    Node.h \
    DirectionGenerator.h

SOURCES = Window.cpp \
          GLViewer.cpp \
          QTUtils.cpp \
          Vertex.cpp \
          Triangle.cpp \
          Mesh.cpp \
          BoundingBox.cpp \
          Material.cpp \
          Object.cpp \
          Light.cpp \
          Scene.cpp \ 
          RayTracer.cpp \
          Ray.cpp \
          Main.cpp \   
    KdTree.cpp \
    Leaf.cpp \
    SimpleNode.cpp \
    Node.cpp \
    DirectionGenerator.cpp
          
    DESTDIR=.

win32 {
    INCLUDEPATH += 'C:\Users\boubek\projects\cg\extern\libQGLViewer-2.3.4'
    LIBS += -L"c:\Users\boubek\projects\cg\extern\libQGLViewer-2.3.4\QGLViewer\release" \
            -lQGLViewer2 \
            -lglew32
}
unix {
    LIBS += -lGLEW \
            -lQGLViewer \
            -lGLU
}

MOC_DIR = tmp
OBJECTS_DIR = tmp

