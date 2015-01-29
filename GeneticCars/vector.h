#ifndef VECTOR
#define VECTOR

class Vector
{
public:
	float x,y;
	Vector(float,float);
	Vector* add(Vector*);
	Vector* sub(Vector*);
	Vector* mul(float);
	float dot(Vector*);
	Vector* normalize();
	float length();
	float lengthsquared();
	float angle();	
};

#endif
