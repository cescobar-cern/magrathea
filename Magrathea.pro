#-------------------------------------------------
#
# Project created by QtCreator 2017-10-07T07:29:37
#
#-------------------------------------------------

QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Magrathea
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
        magrathea.cpp \
        calibrator.cpp \
        MotionHandler.cpp

HEADERS += \
        magrathea.h \
        calibrator.h \
        MotionHandler.h

FORMS += \
        magrathea.ui

INCLUDEPATH += C:\opencv\build\include
LIBS += C:\opencv-build\bin\libopencv_flann331.dll
LIBS += C:\opencv-build\bin\libopencv_core331.dll
LIBS += C:\opencv-build\bin\libopencv_highgui331.dll
LIBS += C:\opencv-build\bin\libopencv_videoio331.dll
LIBS += C:\opencv-build\bin\libopencv_imgcodecs331.dll
LIBS += C:\opencv-build\bin\libopencv_imgproc331.dll
LIBS += C:\opencv-build\bin\libopencv_features2d331.dll
LIBS += C:\opencv-build\bin\libopencv_calib3d331.dll

#Vancouver
win32 : exists(C:/Program Files (x86)/Aerotech/A3200/CLibrary/Include/A3200.h) {
        message("Vancouver, Aerotech A3200 gantry")
        DEFINES += VANCOUVER
        DEFINES += AEROTECH
        SOURCES += AerotechMotionHandler.cpp
        HEADERS += AerotechMotionHandler.h
        INCLUDEPATH += "C:/Program Files (x86)/Aerotech/A3200/CLibrary/Include/"
        INCLUDEPATH += "C:/Program Files (x86)/Aerotech/A3200/CLibrary/Bin/"
        LIBS += -L'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Lib/' -lA3200C
}

#Valencia
win32 : exists(ACSC/C_CPP/ACSC.h) {
        SOURCES += ACSCMotionHandler.cpp
        HEADERS += ACSCMotionHandler.h \
        ACSC\C_CPP\ACSC.h \
        DEFINES += ACSC
        DEFINES += VALENCIA
        LIBS += C:\Users\Silicio\WORK\Opencv_Qt_proj\Loader\ACSC\C_CPP\ACSCL_x86.LIB
}
