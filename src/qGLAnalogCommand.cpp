#include "qAnalogCommand.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#include <cmath>

// contructeur
QAnalogCommand::QAnalogCommand(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent)
{
	_cursor_position = 0.0;
	_increment_max = 500;
	_increment = 0;
	
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

void QAnalogCommand::initializeGL()
{
}

void QAnalogCommand::paintGL()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	
	glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	
	resizeGL(width(), height());
	
	glClearColor(0.75, 0.75, 0.75, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
	// fond
	glBegin(GL_QUADS);
		glColor3f(0.75, 0.125, 0.125);
		glVertex3f(-1.25, -0.5, -0.5);
		glVertex3f(-1.25, 0.5, -0.5);
		glColor3f(0.50, 0.50, 0.50);
		glVertex3f(0.0, 0.5, -0.5);
		glVertex3f(0.0, -0.5, -0.5);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.125, 0.75, 0.125);
		glVertex3f(1.25, 0.5, -0.5);
		glVertex3f(1.25, -0.5, -0.5);
		glColor3f(0.50, 0.50, 0.50);
		glVertex3f(0.0, -0.5, -0.5);
		glVertex3f(0.0, 0.5, -0.5);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-1.245, -0.495, -0.49);
		glVertex3f(-1.245, 0.495, -0.49);
		glVertex3f(1.245, 0.495, -0.49);
		glVertex3f(1.245, -0.495, -0.49);
	glEnd();
	
	// dessin du curseur
	QColor color = this->palette().color(QPalette::Window).dark(155);
	glColor3f(color.redF(), color.greenF(), color.blueF());
	glBegin(GL_QUADS);
		glVertex3f(_cursor_position - 0.2, 0.4, -0.4);
		glVertex3f(_cursor_position + 0.2, 0.4, -0.4);
		glVertex3f(_cursor_position + 0.2, 0.2, -0.4);
		glVertex3f(_cursor_position - 0.2, 0.2, -0.4);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(_cursor_position - 0.2, -0.2, -0.4);
		glVertex3f(_cursor_position + 0.2, -0.2, -0.4);
		glVertex3f(_cursor_position + 0.2, -0.4, -0.4);
		glVertex3f(_cursor_position - 0.2, -0.4, -0.4);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(_cursor_position + 0.1, 0.2, -0.4);
		glVertex3f(_cursor_position + 0.2, 0.2, -0.4);
		glVertex3f(_cursor_position + 0.2, -0.2, -0.4);
		glVertex3f(_cursor_position + 0.1, -0.2, -0.4);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(_cursor_position - 0.2, 0.2, -0.4);
		glVertex3f(_cursor_position - 0.1, 0.2, -0.4);
		glVertex3f(_cursor_position - 0.1, -0.2, -0.4);
		glVertex3f(_cursor_position - 0.2, -0.2, -0.4);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(_cursor_position - 0.2, 0.4, -0.3);
		glVertex3f(_cursor_position + 0.2, 0.4, -0.3);
		glVertex3f(_cursor_position + 0.2, -0.4, -0.3);
		glVertex3f(_cursor_position - 0.2, -0.4, -0.3);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f(_cursor_position - 0.1, 0.2, -0.3);
		glVertex3f(_cursor_position + 0.1, 0.2, -0.3);
		glVertex3f(_cursor_position + 0.1, -0.2, -0.3);
		glVertex3f(_cursor_position - 0.1, -0.2, -0.3);
	glEnd();
	color = this->palette().color(QPalette::Window);
	glColor3f(color.redF(), color.greenF(), color.blueF());
	glBegin(GL_LINES);
		glVertex3f(_cursor_position - 0.10, -0.23, -0.3);
		glVertex3f(_cursor_position - 0.10, -0.37, -0.3);
		glVertex3f(_cursor_position - 0.10, +0.23, -0.3);
		glVertex3f(_cursor_position - 0.10, +0.37, -0.3);
		glVertex3f(_cursor_position - 0.05, -0.23, -0.3);
		glVertex3f(_cursor_position - 0.05, -0.37, -0.3);
		glVertex3f(_cursor_position - 0.05, +0.23, -0.3);
		glVertex3f(_cursor_position - 0.05, +0.37, -0.3);
		glVertex3f(_cursor_position 	  , -0.23, -0.3);
		glVertex3f(_cursor_position 	  , -0.37, -0.3);
		glVertex3f(_cursor_position		  , +0.23, -0.3);
		glVertex3f(_cursor_position 	  , +0.37, -0.3);
		glVertex3f(_cursor_position + 0.05, -0.23, -0.3);
		glVertex3f(_cursor_position + 0.05, -0.37, -0.3);
		glVertex3f(_cursor_position + 0.05, +0.23, -0.3);
		glVertex3f(_cursor_position + 0.05, +0.37, -0.3);
		glVertex3f(_cursor_position + 0.10, -0.23, -0.3);
		glVertex3f(_cursor_position + 0.10, -0.37, -0.3);
		glVertex3f(_cursor_position + 0.10, +0.23, -0.3);
		glVertex3f(_cursor_position + 0.10, +0.37, -0.3);
	glEnd();
	

	glFlush();
	
	glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void QAnalogCommand::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.25, 1.25, 0.5, -0.5, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void QAnalogCommand::paintEvent(QPaintEvent */*event*/)
{
	QPainter painter;
	painter.begin(this);
	
	this->paintGL();
	painter.setRenderHint(QPainter::Antialiasing);
	
	painter.end();
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
	_cursor_position = ((double)(event->x()) / (double)this->width() * 2.) - 1.;
	
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
