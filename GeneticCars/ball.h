#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTimer>
#include <QWidget>
#include "vector.h"

#ifndef BALL
#define BALL

class Ball : public QGraphicsItem
{
public:
	Ball();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	float mass;
	float radius;
	Vector* velocity;
	Vector* position;
	void move();
	bool contains(float,float);

	~Ball()
	{
		delete velocity;
		delete position;
	}

private:
	QColor color;

};

class Wall : public QGraphicsItem
{
public:
	Wall(float,float,float,float);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	float x1,y1,x2,y2;
};

class Link : public QGraphicsItem
{
public:
	Link(int,int,bool);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	int ball1,ball2;
	float length;
	bool isRope;
};

#endif
