#ifndef __TESTVALUE_H
#define __TESTVALUE_H

#include <QLineEdit>

class TestValue : public QLineEdit
{
	Q_OBJECT
	
	protected:
		int _value;
	
	public:
		TestValue(QWidget *parent = NULL);
	
	protected:
		void showValue();
	
	public slots:
		void increment(int val = 1);
		void decrement(int val = 1);
};

#endif
