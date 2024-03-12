QT       += core gui
QT       += network
QT       += sql
QT       += axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    exceloperation.cpp \
    http.cpp \
    imageconvert.cpp \
    inputdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mysqloperation.cpp \
    tablewindow.cpp

HEADERS += \
    exceloperation.h \
    http.h \
    imageconvert.h \
    inputdialog.h \
    mainwindow.h \
    mysqloperation.h \
    tablewindow.h

FORMS += \
    inputdialog.ui \
    mainwindow.ui \
    tablewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
