#include "testValue.h"

TestValue::TestValue(QWidget *parent)
	: QLineEdit(parent)
{
	_value = 0;
	this->showValue();
}

void TestValue::showValue()
{
	this->setText(QString::number(_value));
}
	
void TestValue::increment(int val)
{
	if ((_value += val) > 100000)
		_value = 100000;
	
	this->showValue();
}

void TestValue::decrement(int val)
{
	if ((_value -= val) < 0)
		_value = 0;
	this->showValue();
}
