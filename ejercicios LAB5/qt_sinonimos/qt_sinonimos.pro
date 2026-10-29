QT += core gui widgets

CONFIG += c++17

TARGET = qt_sinonimos
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    task.cpp

HEADERS += \
    mainwindow.h \
    task.h

FORMS += \
    mainwindow.ui \
    task.ui
