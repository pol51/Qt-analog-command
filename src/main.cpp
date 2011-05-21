#include <QApplication>

#include "qAnalogCommand.h"
#include "testValue.h"

#include <QWidget>
#include <QVBoxLayout>

int main (int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QAnalogCommand command;
	QWidget mainWidget;
	TestValue test;
	QVBoxLayout mainLayout(&mainWidget);
	
	mainLayout.addWidget(&command);
	mainLayout.addWidget(&test);
	
	QObject::connect(
		&command, SIGNAL(increment(int)),
		&test, SLOT(increment(int))
	);	
	QObject::connect(
		&command, SIGNAL(decrement(int)),
		&test, SLOT(decrement(int))
	);	
	
	mainWidget.show();	
	
	return app.exec();
}
