#include "qAnalogCommand.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#include <cmath>

// contructeur
QAnalogCommand::QAnalogCommand(QWidget *parent)
	: QWidget(parent)
{
	_painter = new QPainter();

	_cursor_position = 0.0;
	_increment_max = 500;
	_increment = 0;
	
	_cursorSize = 16.;
	_paddingSize = 4.;

	// reglage du rafraichissement
	_refreshTimer.setSingleShot(false);
    QObject::connect(
		&_refreshTimer, SIGNAL(timeout()),
		this, SLOT(update())
	);
    _refreshTimer.start(15);

	// reglage du repeteur de changement
	_valueTimer.setSingleShot(false);
    QObject::connect(
		&_valueTimer, SIGNAL(timeout()),
		this, SLOT(emitChangement())
	);
    _valueTimer.stop();

	// suivi des déplacement de la souris
	this->setMouseTracking(true);
}

// destructeur
QAnalogCommand::~QAnalogCommand()
{
	delete _painter;
}

// taille par defaut
QSize QAnalogCommand::sizeHint() const
{
    return QSize(220, 40);
}

// change la valeur de l'increment maximal
void QAnalogCommand::changeMaxIncrement(int newMax)
{
	if (newMax > 0)
		_increment_max = newMax;
	else
		_increment_max = -newMax;
}

void QAnalogCommand::paintEvent(QPaintEvent */*event*/)
{
	_paddingSize = this->height() / 10;
	_cursorSize = this->height() / 2 - _paddingSize;

	_painter->begin(this);
	
	_painter->setRenderHint(QPainter::TextAntialiasing);
	
	// init variables
	QRectF rectangle;
	double posX = (_cursor_position + 1.) / 2. *
		((this->width() - 1)  - 2 * (_cursorSize + _paddingSize))
		+ _cursorSize + _paddingSize;

	// dessin du fond
	_painter->setPen(Qt::NoPen);
	
	QLinearGradient gradientMin(0, 0, this->width() / 2, 0);
	gradientMin.setColorAt(0, QColor::fromRgbF(0.75, 0.125, 0.125));
	gradientMin.setColorAt(1, QColor::fromRgbF(0.5, 0.5, 0.5));
	_painter->setBrush(gradientMin);
	rectangle.setCoords(0, 0, width() / 2, height() - 1);
	_painter->drawRect(rectangle);
	
	QLinearGradient gradientMax(this->width() / 2, 0, this->width() - 1, 0);
	gradientMax.setColorAt(0, QColor::fromRgbF(0.5, 0.5, 0.5));
	gradientMax.setColorAt(1, QColor::fromRgbF(0.125, 0.75, 0.125));
	_painter->setBrush(gradientMax);
	rectangle.setCoords(width() / 2, 0, width() - 1, height() - 1);
	_painter->drawRect(rectangle);
	
	// dessin du contour
	_painter->setBrush(Qt::NoBrush);
	_painter->setPen(Qt::SolidLine);
	
	rectangle.setCoords(0, 0, width() - 1, height() -1);
	_painter->drawRect(rectangle);
	
	// dessin du curseur
	this->drawCursor(posX);

	_painter->end();
}

void QAnalogCommand::mousePressEvent(QMouseEvent *event)
{
	this->updateCursorPosition(event);
	
	if (_cursor_position > 0.2)
		emit increment(1);
	if (_cursor_position < -0.2)
		emit decrement(1);
	
	_valueTimer.start(150);
}

void QAnalogCommand::mouseReleaseEvent(QMouseEvent */*event*/)
{
	_cursor_position = 0.0;
	
	_valueTimer.stop();
}

void QAnalogCommand::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
		this->updateCursorPosition(event);
}

void QAnalogCommand::updateCursorPosition(QMouseEvent *event)
{
	_cursor_position = ((double)(event->x()) - _cursorSize - _paddingSize)
		* 2 / ((this->width() - 1)  - 2 * (_cursorSize + _paddingSize)) - 1.;
	
	if (_cursor_position > 1.)		_cursor_position = 1.;
	if (_cursor_position < -1.) 	_cursor_position = -1.;	
	
	if (_cursor_position > 0.15)
		_increment = qRound(
			pow((_cursor_position - 0.15) / 0.85 * _increment_max, 2)
			/ _increment_max);
	else if (_cursor_position < -0.15)
		_increment = qRound(
			- pow((_cursor_position + 0.15) / 0.85 * _increment_max, 2)
			/ _increment_max);
	else if (_cursor_position >= -0.15 && _cursor_position <= 0.15)
		_increment = 0;
}

void QAnalogCommand::emitChangement()
{
	if (_increment > 0)
		emit increment(_increment);
	if (_increment < 0)
		emit decrement(-_increment);
	
}

// dessin des curseurs
void QAnalogCommand::drawCursor(double pos)
{
	QColor color = this->palette().color(QPalette::Window).dark(135);
	
	QRectF rectangle;
	
	// curseur
	_painter->setBrush(Qt::SolidPattern);
	_painter->setPen(Qt::NoPen);
	
	_painter->setBrush(QBrush(color));
	rectangle.setCoords(
		pos - _cursorSize,
		_paddingSize,
		pos + _cursorSize,
		_cursorSize / 2 + _paddingSize);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos - _cursorSize,
		this->height() - _paddingSize - 1,
		pos + _cursorSize,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos - _cursorSize,
		_cursorSize / 2 + _paddingSize,
		pos - _cursorSize / 2,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos + _cursorSize,
		_cursorSize / 2 + _paddingSize,
		pos + _cursorSize / 2,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	//contour
	_painter->setBrush(Qt::NoBrush);
	_painter->setPen(Qt::SolidLine);
	
	rectangle.setCoords(
		pos - _cursorSize, _paddingSize,
		pos + _cursorSize, this->height() - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos - _cursorSize / 2,
		_cursorSize / 2 + _paddingSize,
		pos + _cursorSize / 2,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
}
