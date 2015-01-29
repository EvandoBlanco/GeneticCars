#include <iostream>
#include <QtGui>
#include <math.h>
#include "physicsengine.h"

const int MAXBALLS=1000;
int BallCount=0;
int WallCount=0;
int LinkCount=0;
int Bound=50;
int Radius=5;
const float Restitution=0.9;
const float WallRestitution=0.2;
const float gravity=0.5;
float DT=0.1;
float FRICTION=40.0;

Ball* balls[MAXBALLS];
Wall* walls[MAXBALLS];
Link* links[MAXBALLS];

extern QGraphicsScene* thescene;

bool dontdographics=FALSE;
bool simulating=FALSE;


Ball* getBall(int i)
{
	return balls[i];
}

void handleWallCollision(Ball* ball, Wall* wall)
{
	//Ax+By+C of wall
	float A = wall->y1-wall->y2;
	float B = wall->x2-wall->x1;
	float C = wall->x1*wall->y2 - wall->x2*wall->y1;

	float dist = (A*ball->position->x + B*ball->position->y + C) / sqrt(A*A+B*B);

	if (abs(dist)>=ball->radius) return;

	float xi=(B*(B*ball->position->x - A*ball->position->y)-A*C)/(A*A+B*B);
//	float yi=(A*(-B*ball->position->x + A*ball->position->y)-A*C)/(A*A+B*B);

	if(xi<wall->x1 || xi>wall->x2) return;

	float dista = ball->radius-abs(dist);

	Vector* wallnormal = new Vector(wall->y2-wall->y1,wall->x1-wall->x2);
	Vector* wallnormalnormalized = wallnormal->normalize();
	Vector* adjust = wallnormalnormalized->mul(dista);
	Vector* newposition;
	if(dist<0)
		newposition = ball->position->add(adjust);
	else
		newposition = ball->position->sub(adjust);

	//change is based on how much velocity lies along the collision angle
	float vchange=ball->velocity->dot(wallnormalnormalized);
	//adjust for mass and restitution
	float vchange1=(1+WallRestitution)*vchange;
	//adjust velocity by adjustment magnitude along the collision vector
	Vector* velchange1=wallnormalnormalized->mul(vchange1);
	Vector* newvelocity=ball->velocity->sub(velchange1);

	Vector* wallvect = new Vector(wall->x2-wall->x1,wall->y2-wall->y1);
	Vector* walln=wallvect->normalize();
	float vfriction = FRICTION * DT;
	float vforward = ball->velocity->dot(walln);

	Vector* frictvect;
	if(vfriction>=abs(vforward))
		frictvect=walln->mul(vforward);
	else
		frictvect=walln->mul(vfriction);
	Vector* newvelocitywithfriction = newvelocity->sub(frictvect);

	delete ball->velocity;
	delete velchange1;
	delete wallnormal;
	delete wallnormalnormalized;
	delete adjust;
	delete ball->position;
	delete frictvect;
	delete newvelocity;
	delete wallvect;
	delete walln;

	ball->velocity=newvelocitywithfriction;
	ball->position=newposition;
	
}

void handleCollision(Ball* ball1, Ball* ball2)
{
	//collision vector is difference of positions
	Vector* collision=ball1->position->sub(ball2->position);
	float distance = collision->length();
	//the balls overlap.  need to move them apart.
	Vector* adjust=collision->mul((ball1->radius+ball2->radius-distance)/distance);

	float im1=1.0/ball1->mass;
	float im2=1.0/ball2->mass;

	//adjust positions according to mass
	Vector* adjust1=adjust->mul(im1/(im1+im2));
	Vector* adjust2=adjust->mul(im2/(im1+im2));
	//get new positions
	Vector* newposition1=ball1->position->add(adjust1);
	Vector* newposition2=ball2->position->sub(adjust2);

	//now all we need is the collision angle
	Vector* collision_n=collision->normalize();
	//velocity difference vector
	Vector* vdiff=ball1->velocity->sub(ball2->velocity);
	//change is based on how much velocity lies along the collision angle
	float vchange=vdiff->dot(collision_n);
	//adjust for mass and restitution
	float vchange1=(1+Restitution)*vchange*im1/(im1+im2);
	float vchange2=(1+Restitution)*vchange*im2/(im1+im2);
	//adjust velocity by adjustment magnitude along the collision vector
	Vector* velchange1=collision_n->mul(vchange1);
	Vector* velchange2=collision_n->mul(vchange2);
	Vector* newvelocity1=ball1->velocity->sub(velchange1);
	Vector* newvelocity2=ball2->velocity->add(velchange2);

	delete ball1->position;
	delete ball2->position;
	delete ball1->velocity;
	delete ball2->velocity;

	ball1->position=newposition1;
	ball2->position=newposition2;
	ball1->velocity=newvelocity1;
	ball2->velocity=newvelocity2;

	delete vdiff;
	delete collision;
	delete adjust;
	delete adjust1;
	delete adjust2;
	delete collision_n;
	delete velchange1;
	delete velchange2;

}

void handleLink(int l)
{
	Ball* ball1=balls[links[l]->ball1];
	Ball* ball2=balls[links[l]->ball2];

	//collision vector is difference of positions
	Vector* collision=ball1->position->sub(ball2->position);
	float distance = collision->length();

	if(links[l]->isRope && links[l]->length>distance)
	{
		delete collision;
		return;
	}

	//the balls need to stay a certain distance apart
	Vector* adjust=collision->mul((links[l]->length-distance)/distance);

	float im1=1.0/ball1->mass;
	float im2=1.0/ball2->mass;

	//adjust positions according to mass
	Vector* adjust1=adjust->mul(im1/(im1+im2));
	Vector* adjust2=adjust->mul(im2/(im1+im2));

	//get new positions
	Vector* newposition1;
	Vector* newposition2;

	newposition1=ball1->position->add(adjust1);
	newposition2=ball2->position->sub(adjust2);

	//now all we need is the collision angle
	Vector* collision_n=collision->normalize();
	//velocity difference vector
	Vector* vdiff=ball1->velocity->sub(ball2->velocity);
	//change is based on how much velocity lies along the collision angle
	float vchange=vdiff->dot(collision_n);
	//adjust for mass and restitution
	float vchange1=(1+1)*vchange*im1/(im1+im2);
	float vchange2=(1+1)*vchange*im2/(im1+im2);
	//adjust velocity by adjustment magnitude along the collision vector
	Vector* velchange1=collision_n->mul(vchange1);
	Vector* velchange2=collision_n->mul(vchange2);
	Vector* newvelocity1=ball1->velocity->sub(velchange1);
	Vector* newvelocity2=ball2->velocity->add(velchange2);

	delete ball1->position;
	delete ball2->position;
	delete ball1->velocity;
	delete ball2->velocity;

	ball1->position=newposition1;
	ball2->position=newposition2;
	ball1->velocity=newvelocity1;
	ball2->velocity=newvelocity2;

	delete collision;
	delete adjust;
	delete collision_n;
	delete velchange1;
	delete velchange2;
	delete vdiff;
	delete adjust1;
	delete adjust2;
}

void doFrame()
{
	if(!simulating) return;

	int i,j;

	//move each ball
	for(i=0; i<BallCount; i++)
		balls[i]->move();
	//apply gravity
	for(i=0; i<BallCount; i++)
		balls[i]->velocity->y+=gravity*DT;


	//detect collision between balls
	for(i=0; i<BallCount; i++)
	{
		for(j=i+1; j<BallCount; j++)
		{			
			//if dist(balli,ballj)<=radii^2
			if((balls[j]->position->x-balls[i]->position->x)*(balls[j]->position->x-balls[i]->position->x)+(balls[j]->position->y-balls[i]->position->y)*(balls[j]->position->y-balls[i]->position->y) <= (balls[j]->radius+balls[i]->radius)*(balls[j]->radius+balls[i]->radius))
			{
				handleCollision(balls[j],balls[i]);
			}
		}
	}

	//detect collision with a wall
	for(i=0; i<BallCount; i++)
	{
		for(j=0; j<WallCount; j++)
		{
			handleWallCollision(balls[i],walls[j]);
		}
	}

	for(i=0; i<LinkCount; i++)
		handleLink(i);

	if(!dontdographics)
	{
		int i;
		//redraw the balls
		for(i=0; i<BallCount; i++)
			balls[i]->setPos(balls[i]->position->x,balls[i]->position->y);

		for(i=0; i<LinkCount; i++)
			links[i]->update();
	}

}

int makeBall(int x, int y)
{
		balls[BallCount]=new Ball();
		balls[BallCount]->position->x=x;
		balls[BallCount]->position->y=y;
		balls[BallCount]->velocity->x=balls[BallCount]->velocity->y=0;
		balls[BallCount]->mass=1;

		if(!dontdographics)
		{
			thescene->addItem(balls[BallCount]);
			balls[BallCount]->setPos(balls[BallCount]->position->x,balls[BallCount]->position->y);
		}
		BallCount++;
		return BallCount-1;
}

int makeLink(int ball1, int ball2)
{
			links[LinkCount]=new Link(ball1,ball2,false);
			if(!dontdographics)
				thescene->addItem(links[LinkCount]);
			LinkCount++;
		return LinkCount-1;
}

void removeLink(int n)
{
	if(!dontdographics)
		thescene->removeItem(links[n]);
	delete links[n];
	for(int i=n; i<LinkCount-1; i++)
		links[i]=links[i+1];
	LinkCount--;
}

void removeBall(int n)
{
	if(!dontdographics)
		thescene->removeItem(balls[n]);
	delete balls[n];
	for(int i=n; i<BallCount-1; i++)
		balls[i]=balls[i+1];
	BallCount--;
}
