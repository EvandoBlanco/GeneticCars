#include "car.h"
#include "physicsengine.h"
#include "ball.h"
#include <QtGui>
#include <iostream>

const int BALLMUTATION=30;
const int LINKMUTATION=30;

//random number [a,b]
int randint(int a, int b)
{
	return (qrand()%(b-a+1))+a;
}

//Mutates the nodes and links of a car
Car* Car::mutate()
{
	Car* newcar=new Car(nodes);

    //make newcar a clone of its parent
    for(int i=0; i<nodes; i++)
    {
        //Copy x and y values for each node
        newcar->balls_x[i]=balls_x[i];
        newcar->balls_y[i]=balls_y[i];
    }
    //stores random number that is mutation chance for each node
    int randomMutation;

    //loops trhough nodes and mutates
    for(int j=0; j<nodes; j++)
    {
        // Stores random number between 0-99
        randomMutation=rand()%100;
        //Mutates ball positions with values from 5-50
        if(randomMutation<BALLMUTATION)
        {
            newcar->balls_x[j]=randint(5,50);
            newcar->balls_y[j]=randint(5,50);

        }
    }

    //Loops through links and mutates
    for(int i=0; i<nodes; i++)
    {
        for(int j=0; j<nodes; j++)
        {
            //Stores random number between 0-99
            randomMutation=rand()%100;

            if(i<=j)
            {
                continue;
            }

            //If randomMutation is less than LINKMUTATION, mutate links
            if(randomMutation<LINKMUTATION)
            {
                //Switch link existance between nodes on or off
                if(newcar->links[i][j]==1)
                {
                    newcar->links[i][j]=0;
                }

                else
                {
                    newcar->links[i][j]=1;
                }
            }
        }
    }

	return newcar;
}

Car* Car::breed(Car* c)
{
	Car* newcar=new Car(nodes);
    int crossoverPoint=rand() % nodes;
    for(int i=0; i<crossoverPoint; i++)
    {
        //daughter gets the father's balls before crossover point
        newcar->balls_x[i]=balls_x[i];
        newcar->balls_y[i]=balls_y[i];

        for(int k=0; k<crossoverPoint;k++)
        {
            if(k<=i)
            {
                continue;
            }
            newcar->links[i][k]=links[i][k];
        }
    }

    for(int j=crossoverPoint; j<nodes; j++)
    {
        //daughter get the mother's balls after crossover point
        newcar->balls_x[j]=c->balls_x[j];
        newcar->balls_y[j]=c->balls_y[j];

        for(int l=0; l<crossoverPoint; l++)
        {
            if(l<=j)
            {
                continue;
            }
            newcar->links[j][l]=links[j][l];
        }
    }

    crossoverPoint=rand() % nodes;



	return newcar;
}

//makes a random car
Car::Car(int n)
{
	//make all of the arrays I need
	nodes=n;
	balls_x=new int[nodes];
	balls_y=new int[nodes];
	balls=new int[nodes];
	links=new int*[nodes];
	for(int i=0; i<nodes; i++) links[i]=new int[nodes];
	numlinks=0;
	linkslist=new int[nodes*nodes];
	
	//make random ball locations x=5-50, y=5-50
	for(int i=0; i<nodes; i++)
	{
		balls_x[i]=randint(5,50);
		balls_y[i]=randint(5,50);
	}
	//making links, 1/3 probability link between any two balls
	for(int i=0; i<nodes; i++)
	{
		for(int j=0; j<nodes; j++)
		{
			links[i][j]=0;
			if(i<=j) continue;
			if(randint(1,3)==1)
			{
				links[i][j]=1;
			}
		}
	}
}

//graphically make the car so we can race it
void Car::constructCar()
{
	balls=new int[nodes];
	numlinks=0;
	linkslist=new int[nodes*nodes];
	
	for(int i=0; i<nodes; i++)
	{
		balls[i]=makeBall(balls_x[i],balls_y[i]);
	}
	for(int i=0; i<nodes; i++)
	{
		for(int j=0; j<nodes; j++)
		{
			if(links[i][j])
			{
				linkslist[numlinks++]=makeLink(i,j);
			}
		}
	}
}

//graphically destroy the car so we can race somebody else
void Car::deconstructCar()
{
	for(int i=0; i<numlinks; i++)
		removeLink(linkslist[i]-i);
	for(int i=0; i<nodes; i++)
		removeBall(balls[i]-i);

	delete balls;
	delete linkslist;
}

//determine where car is at end of race: average
int Car::getCarPosition(int n)
{
    //n is for debugging purposes
	int sum=0;
    for (int i=0; i<nodes; i++){
        sum+=getBall(balls[i])->position->x;
    }

    /* Debugging
    if(n==1){}
        std::cout << "do Cars get pos:" << sum/nodes << std::endl;
    else if(n==2)
    {
        std::cout << "timehandler get pos:" << sum/nodes << std::endl;
    } */
	return sum / nodes;
}

//save the score
int Car::score(int itr, int pos)
{
	iterations=itr;
	position=pos;

    return pos-iterations;
}
