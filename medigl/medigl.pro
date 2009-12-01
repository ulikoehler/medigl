QT += opengl
TARGET = medigl
TEMPLATE = app
HEADERS += glwidget.h \
    medidialog.h \
    shaders.h
SOURCES += glwidget.cpp \
    main.cpp \
    medidialog.cpp
FORMS += medidialog.ui
