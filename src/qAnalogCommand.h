#ifndef __QANALOGCOMMAND_H
#define __QANALOGCOMMAND_H

#include <QWidget>
#include <QTimer>

class QMouseEvent;
class QPainter;

class QAnalogCommand : public QWidget
{
	Q_OBJECT
	
	private:
		QTimer		_refreshTimer;		// timer de maj affichage
		QTimer		_valueTimer;		// timer de maj des valeurs envoyées
		double		_cursor_position;	// position du curseur
		int 		_increment;			// increment/decrement
		int 		_increment_max;		// valeur increment maximal
		QPainter	*_painter;			// qpainter pour redessiner le widget
		double		_cursorSize;		// taille du curseur
		double		_paddingSize;		// taille de la "marge-curseur"
		
	public:
		QAnalogCommand(QWidget *parent = NULL);	// constructeur
		~QAnalogCommand();						// destructeur
		
		QSize sizeHint() const;			// taille par defaut
		
		void changeMaxIncrement(int newMax);
		
	protected:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void updateCursorPosition(QMouseEvent *event);
		void drawCursor(double pos);
	
	protected slots:
		void emitChangement();
	
	signals:
		void increment(int);
		void decrement(int);
};

#endif
