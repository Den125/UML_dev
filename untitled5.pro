QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    centralwidget.cpp \
    diagram.cpp \
    main.cpp \
    picturewidget.cpp \
    project.cpp \
    running.cpp \
    toolbar.cpp \
    treewidget.cpp \
    umltabwidget/uml_highlighter.cpp \
    umltabwidget/umltabwidget.cpp \
    umltabwidget/umltexteditwidget.cpp

HEADERS += \
    centralwidget.h \
    diagram.h \
    globaldata.h \
    picturewidget.h \
    project.h \
    running.h \
    singleton.h \
    toolbar.h \
    treewidget.h \
    umltabwidget/uml_highlighter.hpp \
    umltabwidget/umltabwidget.h \
    umltabwidget/umltexteditwidget.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-analyzer-Desktop_Qt_5_9_1_MinGW_32bit-Release/release/ -lanalyzer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-analyzer-Desktop_Qt_5_9_1_MinGW_32bit-Debug/debug/ -lanalyzer
else:unix: LIBS += -L$$PWD/../build-analyzer-Desktop_Qt_5_9_1_MinGW_32bit-Release/ -lanalyzer

INCLUDEPATH += $$PWD/../analyzer
DEPENDPATH += $$PWD/../analyzer
