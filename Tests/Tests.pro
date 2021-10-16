QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    main.cpp \
    Test_AES128.cpp \
    Test_AES192.cpp \
    Test_AES256.cpp

HEADERS += \
    Test_AES128.h \
    Test_AES192.h \
    Test_AES256.h
