QT += opengl
TARGET = medigl
TEMPLATE = app
HEADERS += glwidget.h \
    medidialog.h \
    shaders.h \
    fastimage.h \
    dicomimagefile.h
SOURCES += glwidget.cpp \
    main.cpp \
    medidialog.cpp \
    fastimage.cpp \
    dicomimagefile.cpp
FORMS += medidialog.ui
LIBS+=-ldcmdata -lglut -lofstd -ldcmimgle -ldcmimage -ltiff