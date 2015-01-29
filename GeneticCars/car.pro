HEADERS += main.h
HEADERS += ball.h
HEADERS += car.h
HEADERS += vector.h
HEADERS += physicsengine.h

SOURCES += main.cpp
SOURCES += physicsengine.cpp
SOURCES += ball.cpp
SOURCES += vector.cpp
SOURCES += car.cpp

target.path = car
sources.files= $$SOURCES $$HEADERS ball.pro
sources.path = .
INSTALLS += target sources

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
DEPENDPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Qt/5.3/mingw482_32/lib/ -lQt5Gui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Qt/5.3/mingw482_32/lib/ -lQt5Guid
else:unix: LIBS += -L$$PWD/../../Qt/5.3/mingw482_32/lib/ -lQt5Gui
INCLUDEPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
DEPENDPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
DEPENDPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Qt/5.3/mingw482_32/lib/ -lQt5Gui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Qt/5.3/mingw482_32/lib/ -lQt5Guid
else:unix: LIBS += -L$$PWD/../../Qt/5.3/mingw482_32/lib/ -lQt5Gui
INCLUDEPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
DEPENDPATH += $$PWD/../../Qt/5.3/mingw482_32/lib
