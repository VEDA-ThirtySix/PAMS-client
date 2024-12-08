<<<<<<< HEAD:cctv_36/cctv_36.pro
QT += core gui network sql#추가
=======
QT       += core gui network
# Add network tag to use #include <QSslSocket>
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87:https/client/client.pro

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += inc              \
               inc/model        \
               inc/view         \
               inc/controller   \
               inc/network

SOURCES += \
    main.cpp \
<<<<<<< HEAD:cctv_36/cctv_36.pro
    $$files($$PWD/src/model/*.cpp)       \
    $$files($$PWD/src/view/*.cpp)        \
    $$files($$PWD/src/controller/*.cpp)  \
    $$files($$PWD/src/network/*.cpp) \

HEADERS += \
    $$files($$PWD/inc/model/*.h)      \
    $$files($$PWD/inc/view/*.h)       \
    $$files($$PWD/inc/controller/*.h) \
    $$files($$PWD/inc/network/*.h)    \
    inc/view/dialog_videoclip.h
=======
    mainwindow.cpp \
    sslSocket.cpp

HEADERS += \
    mainwindow.h \
    sslSocket.h
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87:https/client/client.pro

FORMS += \
    $$files($$PWD/src/ui/*.ui) \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
