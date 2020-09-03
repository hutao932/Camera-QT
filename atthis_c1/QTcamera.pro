QT       += core gui

QT += multimedia
QT += multimediawidgets

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

GIT_VERSION  = $$system("scripts/gen_version.sh")

SOURCES += \
    ../thirdparty/jsoncpp/src/jsoncpp.cpp \
    com_face_deletion.cpp \
    com_face_entering.cpp \
    com_face_management.cpp \
    com_face_register.cpp \
    com_function_set.cpp \
    com_main_window.cpp \
    face_deletion.cpp \
    face_entering.cpp \
    face_management.cpp \
    face_register.cpp \
    function_set.cpp \
    main.cpp \
    main_window.cpp \
    src/camera/camera.cpp \
    src/http_client/http_client.cpp \

HEADERS += \
    ../thirdparty/curl/include/curl.h \
    ../thirdparty/jsoncpp/include/json/json.h \
    com_face_deletion.h \
    com_face_entering.h \
    com_face_management.h \
    com_face_register.h \
    com_function_set.h \
    com_main_window.h \
    face_deletion.h \
    face_entering.h \
    face_management.h \
    face_register.h \
    function_set.h \
    inc/atthis_protocal/atthis_protocal.h \
    inc/base/base.h \
    inc/base/http.h \
    inc/camera/camera.h \
    inc/helper/version.h \
    inc/http_client/http_client.h \
    main_window.h

FORMS += \
    face_deletion.ui \
    face_entering.ui \
    face_management.ui \
    face_register.ui \
    function_set.ui \
    main_window.ui

INCLUDEPATH += inc/base \
               ../thirdparty/jsoncpp/include \
               ../thirdparty/opencv/include \
               ../thirdparty \
               ../thirdparty/curl/include \
               inc/atthis_protocal \
               inc/camera \
               inc/http_client \
               inc/helper

LIBS        += ../thirdparty/opencv/lib/libopencv_*.so \
               ../thirdparty/curl/lib/libcurl.so


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
