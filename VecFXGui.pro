QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += link_pkgconfig
LIBS+=-lpugixml




# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SVGObject.cpp \
    csscolorparser.cpp \
    kfTableModel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ResvgQt.h \
    SVGObject.hpp \
    csscolorparser.hpp \
    kfTableModel.h \
    mainwindow.h \
    resvg.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lresvg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lresvg
else:unix: LIBS += -L$$PWD/./ -lresvg

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/libresvg.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/libresvg.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/resvg.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/resvg.lib
else:unix: PRE_TARGETDEPS += $$PWD/./libresvg.a
