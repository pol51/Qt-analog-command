#ifndef __QANALOGCOMMAND_H
#define __QANALOGCOMMAND_H

#include <QGLWidget>
#include <QTimer>

class QMouseEvent ;

class QAnalogCommand : public QGLWidget
{
	Q_OBJECT
	
	private:
		QTimer		_refreshTimer;		// timer de maj affichage
		QTimer		_valueTimer;		// timer de maj des valeurs envoyées
		double		_cursor_position;	// position du curseur
		int 		_increment;			// increment/decrement
		int 		_increment_max;		// valeur increment maximal
		
	public:
		QAnalogCommand(QWidget *parent = NULL);	// constructeur
		~QAnalogCommand();						// destructeur
		
		QSize sizeHint() const;			// taille par defaut
	
		void changeMaxIncrement(int newMax);
		
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void updateCursorPosition(QMouseEvent *event);
	
	protected slots:
		void emitChangement();
	
	signals:
		void increment(int);
		void decrement(int);
};

#endif
