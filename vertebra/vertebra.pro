QT += opengl
QMAKE_CXXFLAGS = -O3 -march=native
TARGET = vertebra
TEMPLATE = app
HEADERS += glwidget.h \
    vertebradialog.h \
    shaders.h \
    fastimage.h \
    dicomimagefile.h
SOURCES += glwidget.cpp \
    main.cpp \
    vertebradialog.cpp \
    fastimage.cpp \
    dicomimagefile.cpp
FORMS += vertebradialog.ui
LIBS+=-ldcmdata -lglut -lofstd -ldcmimgle -ldcmimage -ltiff