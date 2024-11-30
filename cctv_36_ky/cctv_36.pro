QT       += core gui network sql#추가

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbManager.cpp \
    dialog_edit.cpp \
    dialog_enroll.cpp \
    main.cpp \
    mainwindow.cpp \
    metadata.cpp \
    parseJson.cpp \
    search.cpp \
    socket.cpp \
    streaming.cpp \
    userManager.cpp

HEADERS += \
    dbManager.h \
    dialog_edit.h \
    dialog_enroll.h \
    mainwindow.h \
    metadata.h \
    parseJson.h \
    search.h \
    socket.h \
    streaming.h \
    userManager.h

FORMS += \
    edit.ui \
    enroll.ui \
    mainwindow.ui \
    search.ui \
    streaming.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
