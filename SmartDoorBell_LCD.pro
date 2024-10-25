QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += \
    widgets \
    network

CONFIG += c++17
CONFIG += no_keywords

QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS += -ldl
QMAKE_CXXFLAGS += -lpthread
QMAKE_CXXFLAGS += -pthread
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS += -Wextra

LIBS += -fopenmp -ldl -lpthread -pthread -lgomp -lcurl -ljsoncpp -lwiringPi -lm

#set package support if disabled
QT_CONFIG -= no-pkgconfig

#link opencv4 package
CONFIG += link_pkgconfig
PKGCONFIG += \
    opencv4 \
    openssl

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD \
    include \
    /usr/local/include/opencv4 \
    /usr/include/libcamera \
    /usr/local/include/ncnn \
    /usr/include \
    /usr/lib/x86_64-linux-gnu \
    /usr/local/lib/cmake/websocketpp

DEPENDPATH += \
    $$PWD \
    include \
    /usr/local/include/opencv4 \
    /usr/include/libcamera \
    /usr/local/include/ncnn \
    /usr/include \
    /usr/lib/x86_64-linux-gnu \
    /usr/local/lib/cmake/websocketpp

SOURCES += \
    src/Cloud_cURL.cpp \
    src/Controls.cpp \
    src/FaceEmbedding.cpp \
    src/LED_UNIX_Socket.cpp \
    src/LibCam_Wrapper.cpp \
    src/TArcface.cpp \
    src/TBlur.cpp \
    src/TLive.cpp \
    src/TMtCNN.cpp \
    src/TRetina.cpp \
    src/TWarp.cpp \
    src/ThreadPool.cpp \
    src/base64.cpp \
    src/libcamera_app.cpp \
    src/libcamera_app_options.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/myvideocapture.cpp \
    src/requestinputwindow.cpp

HEADERS += \
    include/Cloud_cURL.h \
    include/Controls.h \
    include/FaceEmbedding.h \
    include/LED_UNIX_Socket.h \
    include/LibCam_Wrapper.h \
    include/LiveStream.h \
    include/TArcface.h \
    include/TBlur.h \
    include/TLive.h \
    include/TMtCNN.h \
    include/TRetina.h \
    include/TWarp.h \
    include/ThreadPool.h \
    include/base64.h \
    include/libcamera_app.hpp \
    include/libcamera_app_options.hpp \
    include/mainwindow.h \
    include/myvideocapture.h \
    include/requestinputwindow.h

FORMS += \
    mainwindow.ui \
    requestinputwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/ -lcamera

INCLUDEPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/ -lcamera-base

INCLUDEPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ncnn/ -lncnn

INCLUDEPATH += $$PWD/../../../../../usr/local/lib/ncnn
DEPENDPATH += $$PWD/../../../../../usr/local/lib/ncnn

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/ncnn/libncnn.a
