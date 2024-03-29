QT       += gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Board.cpp \
    BoardBuilder.cpp \
    Cell.cpp \
    Document.cpp \
    PuzzleSolver.cpp \
    SQLBase.cpp \
    Utility.cpp \
    createcrossworddlg.cpp \
    lengthdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    waitdialog.cpp

HEADERS += \
    Board.h \
    BoardBuilder.h \
    Cell.h \
    DirectionArrow.h \
    Document.h \
    PuzzleSolver.h \
    SQLBase.h \
    Utility.h \
    createcrossworddlg.h \
    lengthdialog.h \
    mainwindow.h \
    waitdialog.h

FORMS += \
    createcrossworddlg.ui \
    lengthdialog.ui \
    mainwindow.ui \
    waitdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    GenCross.pro.user \
    img/rightDown.png
