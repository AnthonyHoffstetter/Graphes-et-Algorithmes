QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ajouterarcsdialog.cpp \
    ajoutergraphedialog.cpp \
    algorithms.cpp \
    dialogordonnancement.cpp \
    dialogtachesprecedentes.cpp \
    graphe.cpp \
    graphevalue.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ajouterarcsdialog.h \
    ajoutergraphedialog.h \
    algorithms.h \
    dialogordonnancement.h \
    dialogtachesprecedentes.h \
    graphe.h \
    graphevalue.h \
    mainwindow.h

FORMS += \
    ajouterarcsdialog.ui \
    ajoutergraphedialog.ui \
    dialogordonnancement.ui \
    dialogtachesprecedentes.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
