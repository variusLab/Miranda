QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Miranda # nom du fichier .exe

VERSION = 1.0
QMAKE_TARGET_COMPANY = Varius Lab
QMAKE_TARGET_PRODUCT = Miranda
QMAKE_TARGET_DESCRIPTION = Random password generator
QMAKE_TARGET_COPYRIGHT = Varvara Petrova 2023
RC_ICONS += key_icon.ico


SOURCES += \
    main.cpp \
    pwdgenerator.cpp \
    wordinputdialog.cpp

HEADERS += \
    alphabetconstants.h \
    pwdgenerator.h \
    wordinputdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
