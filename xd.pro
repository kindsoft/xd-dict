
TEMPLATE = app
TARGET = XD
DEPENDPATH += . Debug lang Release
INCLUDEPATH += .
HEADERS += combobox.h dict.h mainwindow.h optiondialog.h textedit.h
SOURCES += combobox.cpp \
           dict.cpp \
           main.cpp \
           mainwindow.cpp \
           optiondialog.cpp \
           textedit.cpp
RESOURCES += xd.qrc
TRANSLATIONS += lang/en.ts lang/zh_CN.ts
ICON = xd.icns
