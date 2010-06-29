QT += opengl
TARGET = medigl
TEMPLATE = app
HEADERS += glwidget.h \
    medidialog.h \
    shaders.h \
    fastimage.h
SOURCES += glwidget.cpp \
    main.cpp \
    medidialog.cpp \
    fastimage.cpp
FORMS += medidialog.ui
