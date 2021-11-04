QT += testlib
QT -= gui

CONFIG += qt console warn_on testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../Source

SOURCES +=  \
    main.cpp \
    Test_AES.cpp \
    Test_State.cpp \
    Test_Word.cpp \
    ../Source/AES/AES.cpp \
    ../Source/AES/State.cpp \
    ../Source/AES/Word.cpp \
    ../Source/Utility/AESUtility.cpp \
    ../Source/Utility/GFUtility.cpp

HEADERS += \
    Test_AES.h \
    Test_State.h \
    Test_Word.h \
    ../Source/AES/AES.h \
    ../Source/AES/State.h \
    ../Source/AES/Word.h \
    ../Source/Utility/AESUtility.h \
    ../Source/Utility/GFUtility.h
