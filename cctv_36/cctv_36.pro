QT += core gui network sql multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += inc              \
               inc/model        \
               inc/view         \
               inc/controller   \
               inc/network

SOURCES += \
    main.cpp \
    $$files($$PWD/src/model/*.cpp)       \
    $$files($$PWD/src/view/*.cpp)        \
    $$files($$PWD/src/controller/*.cpp)  \
    $$files($$PWD/src/network/*.cpp) \

HEADERS += \
    $$files($$PWD/inc/model/*.h)      \
    $$files($$PWD/inc/view/*.h)       \
    $$files($$PWD/inc/controller/*.h) \
    $$files($$PWD/inc/network/*.h)    \

FORMS += \
    $$files($$PWD/src/ui/*.ui) \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
