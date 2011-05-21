TEMPLATE = app
TARGET = qAnalogCommand_test
DEPENDPATH += . src
INCLUDEPATH += . src
CONFIG += \
	release \
	warn_on \
	console
HEADERS += \
	src/qAnalogCommand.h \
	src/testValue.h
SOURCES += \
	src/qAnalogCommand.cpp \
	src/testValue.cpp \
	src/main.cpp
DESTDIR =		bin
MOC_DIR =		build/moc
UI_DIR =		build/uic
OBJECTS_DIR =	build/obj
RCC_DIR =		build/rcc
