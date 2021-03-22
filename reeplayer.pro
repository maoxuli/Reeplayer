#-------------------------------------------------
#
# Project created by QtCreator 2021-03-12T20:59:50
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += c++11
QT_CONFIG -= no-pkg-config

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += jsoncpp hiredis libjsonrpccpp-common libjsonrpccpp-client

unix:!macx {
    PKGCONFIG += gstreamer-1.0 gstreamer-video-1.0 gstreamer-app-1.0
    DEFINES += GST_USE_UNSTABLE_API
}

macx: {
    INCLUDEPATH += "/Library/Frameworks/GStreamer.framework/Headers"
    LIBS += -F/Library/Frameworks/ -framework GStreamer
}


TARGET = reeplayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    imageview.cpp \
    GstPipelineWrapper.cpp \
    GstAppSinkPipeline.cpp \
    camerasform.cpp \
    filesform.cpp \
    videosform.cpp \
    videoview.cpp \
    camerasmanager.cpp \
    camera.cpp \
    camerastate.cpp \
    filestate.cpp \
    addcameraform.cpp \
    removecameraform.cpp \
    zoomvideoform.cpp \
    switchbutton.cpp \
    systemform.cpp \
    clickablelabel.cpp

HEADERS += \
    mainwindow.h \
    imageview.h \
    GstPipelineWrapper.h \
    GstAppSinkPipeline.h \
    GstreamerPipelines.h \
    camerasform.h \
    filesform.h \
    videosform.h \
    videoview.h \
    camerasmanager.h \
    camera.h \
    camerastate.h \
    filestate.h \
    addcameraform.h \
    formindex.h \
    removecameraform.h \
    zoomvideoform.h \
    switchbutton.h \
    cameraclient.h \
    systemform.h \
    clickablelabel.h

FORMS +=

DISTFILES +=

RESOURCES += \
    resource.qrc
