#include <iostream>
#include <QtGui>
#include <math.h>
#include "ball.h"

extern int Bound,Radius,WIDTH,HEIGHT;
extern float DT;
extern Ball* balls[];

Ball::Ball()
{
	setPos(qrand()%(WIDTH-Bound*2)+Bound,qrand()%(HEIGHT-Bound*2)+Bound);
	position=new Vector(x(),y());
	mass=(qrand()%10000)/1000.0;
	color.setBlue(0); color.setRed(0); color.setGreen(0);
	switch(qrand()%3)
	{
		case 0:color.setBlue(255-(int)(25*mass)); break;
		case 1:color.setRed(255-(int)(25*mass)); break;
		case 2:color.setGreen(255-(int)(25*mass)); break;
	}

	float speed=qrand()%5+1;
	float angle=(qrand()%360)*2*3.14/360.0;
	velocity=new Vector(speed*cos(angle),speed*sin(angle));

	radius=Radius;
}


QRectF Ball::boundingRect() const
{
	return QRectF(-radius,-radius,radius*2,radius*2);
}

Wall::Wall(float p1, float p2, float p3, float p4)
{
	setPos(0,0);
	x1=p1; y1=p2; x2=p3; y2=p4;
}

QRectF Wall::boundingRect() const
{
//	return QRectF(x1,y1,x2-x1,y2-y1);
	return QRectF(0,0,WIDTH,HEIGHT);
}

Link::Link(int b1, int b2, bool isRpe)
{
	setPos(0,0);
	ball1=b1; ball2=b2; isRope=isRpe;
	Vector* v=balls[ball1]->position->sub(balls[ball2]->position);
	length=v->length();
	delete v;
}

QRectF Link::boundingRect() const
{
	return QRectF(0,0,WIDTH,HEIGHT);
}


void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setBrush(color);
	painter->drawEllipse(-radius,-radius,radius*2,radius*2);
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QColor color;
	color.setBlue(0); color.setRed(0); color.setGreen(0);
	painter->setBrush(color);
	painter->drawLine(x1,y1,x2,y2);
}

void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QColor color;
	color.setBlue(0); color.setRed(255); color.setGreen(0);
	painter->setBrush(color);
	painter->drawLine(balls[ball1]->position->x,balls[ball1]->position->y,balls[ball2]->position->x,balls[ball2]->position->y);
}

bool Ball::contains(float x, float y)
{
	return (position->x-x)*(position->x-x)+(position->y-y)*(position->y-y)<=radius*radius;
}

void Ball::move()
{
	position->x+=velocity->x*DT;
	position->y+=velocity->y*DT;

}
