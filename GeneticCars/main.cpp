//GENETIC CARS
//
//Template: Michael Black, 2014
//
//main.cpp does the following in order:
//	1) creates a population of cars
//	2) creates a racecourse
//	3) genetically improves the cars by racing them
//	4) creates the window
//	5) shows the winning cars

#include <iostream>
#include <QtWidgets>
#include <math.h>
#include "main.h"
#include "physicsengine.h"
#include "car.h"

const int GENERATIONS=20;		//how many breeding generations
const int MAXCARS=1000;			//maximum # of cars.  more than this will segfault
const int KILLMAX=20;			//kill all but this many cars
const int INITIAL_POPULATION=50;	//how many cars we start with
const int SURVIVORS=50;         //Max Population after breeding
const int BREEDCHANCE=30;     //Chance of any given two cars to breed, maybe find a nice colonial that's not two expensive, and raise a beautiful young car of their own
const int MUTATECHANCE=25;

void makeRaceCourse(int);
int WIDTH=500,HEIGHT=500;		//screen width and height
QGraphicsScene* thescene;		//window component
WindowView* view;			//window
int timeunit=1000/660;			//when we're actually showing the car, do a frame every this many milliseconds

Car* car[MAXCARS];			//cars are stored here
int CarCount=INITIAL_POPULATION;	//keeps track of how many valid cars there are
int currentCar;				//which car we're currently racing

int iterations;				//how many frames we have simulated so far

extern bool simulating;			//TRUE if actively racing a car, FALSE during setup
extern bool dontdographics;		//TRUE if no window is open, FALSE if we want to see the cars in the window

//sets up a timer for visualization
TimerHandler::TimerHandler(int t)
{
	QTimer* timer=new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	timer->start(t);
}

/*when a timer goes off, ready to show a frame
*this only happens when best "cars" are sorted (see doCars())*/
void TimerHandler::onTimer()
{
    //return if not simulating cars on track
	if(!simulating) return;

	doFrame();

    int pos=car[currentCar]->getCarPosition(2);

	iterations++;

	if(iterations>=2000 || pos>=WIDTH)
	{
        qDebug() << iterations<<" iterations, !!ALEX WAS HERE !! position="<<pos<<endl;
		car[currentCar]->score(iterations,pos);
        car[currentCar]->deconstructCar();

		currentCar++;

		if(currentCar>=CarCount)
		{
            makeRaceCourse(1);
            std::cout << "changing map" << std::endl;
            //simulating=FALSE;
            currentCar=0;
			for (int i=0; i<CarCount; i++)
			{
                qDebug()<<"Car "<<i<<": itr: "<<car[i]->iterations<<", pos:"<<car[i]->position<<endl;
			}
            car[currentCar]->constructCar();
		}
		else
		{

			car[currentCar]->constructCar();
		}
		iterations=0;
	}
}

WindowView::WindowView(QGraphicsScene *scene, QWidget* parent):QGraphicsView(scene, parent) { }

//Stops the simulation
void WindowView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton)
	{
		simulating=!simulating; return;
	}
}


//Constructs a car on the window
void WindowView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		currentCar=0;
		car[currentCar]->constructCar();
		simulating=TRUE;
	}
}

//Sorts the array and deletes low preforming cars
void kill()
{

    // Sort cars by max position via bubble sort
    for(int i=0; i<CarCount; i++)
    {
        for(int j=0; j<CarCount-1; j++)
        {

            if(car[j]->position < car[j+1]->position)
            {
                Car* temp=car[j];
                car[j]=car[j+1];
                car[j+1]=temp;
            }


        }
    }

    //delete the cars in the sorted array past KILLMAX
    for(int k=KILLMAX; k<CarCount; k++)
    {
        delete car[k];
    }
    //Number of cars equals KILLMAX
    CarCount=KILLMAX;
}

//Create new cars with traits of the old 
void breed()
{
    //Keeps track of where the next null space in the array of cars is
    int counter=0;  
    //Stores random breed attempt for each car
    int breedAttempt;
    //For each pair of cars, attempt to breed
    for(int i=0; i<KILLMAX; i++)
    {
        for(int j=0; j<KILLMAX; j++)
        {
            //Don't breed a car with itself
            if(i==j)
                continue;
            //Generates random number between 0-99
            breedAttempt=rand()%100;
            //If random numnber is less than BREEDCHANCE, breed
            if(breedAttempt<BREEDCHANCE)
            {
                //Breed two cars
                car[KILLMAX+counter]=car[i]->breed(car[j]);
                //Counter is incremented, keeping track of the first nullspace in the array of cars
                counter++;
                //# of total cars is incremented
                CarCount++;
            }
        }
    }
}

//Mutates car by changing random nodes and links then adds mutated car to the array of cars
void mutate()
{
    std::cout << "Starting to mutate" << std::endl;
    //stores random number that will be chance to mutate
    int mutateAttempt;
    //Defines temporary storage for mutated car
    Car* temp;
    /*Since CarCount will be incremented every time a car is mutated
    * a temporary count is maintained to keep the # of cars that might be mutated static
    * because we do not want to mutate cars that have already been mutated*/
    int tempCount=CarCount;
    //Go through all cars and try to mutate each car
    for(int i=0; i<tempCount;i++)
    {
        //Generates random number between 0-99
        mutateAttempt=rand()%100;
        //If random number is lees than MUTATECHANCE, mutate
        if(mutateAttempt<MUTATECHANCE)
        {
            //Mutated car is returned and stored
            temp=car[i]->mutate();
            //Add mutated car to the array of cars
            car[CarCount]=temp;
            //# of cars is incrmeented
            CarCount++;
        }
    }


    std::cout << "Mutation ended" << std::endl;
}

void doCars()
{
    //Flag for graphics, is only set to true after all generations have finished
	dontdographics=TRUE;

    //generates each generation of cars
	for(int gen=0; gen<GENERATIONS; gen++) 
    {
        //Prints out generation number for current generation
		qDebug() << "****** GENERATION "<<gen<<" **********" <<endl;
    
        //Constructs each car and then lets said car run the track
    	for (int i=0; i<CarCount; i++)
    	{
            //constructs car
    		car[i]->constructCar();

            //begins simulation after construction is finished 
    		simulating=TRUE;
            //t increments each frame
    		int t=0;
            //stores the car's furthest position on each frame
    		int pos;
            //simulates the car for 2000 frames
    		for(t=0; t<2000; t++)
            {
                //does a frame
    			doFrame();
                //gets the car's pisition
                pos=car[i]->getCarPosition(1);
                //Simulation ends if the car reaches the end of the track
    			if(pos>=WIDTH) break;
    		}
            //Calculates the car's overall score (used in determining best cars)
            car[i]->scr=car[i]->score(t,pos);
            //Stops the simulation
    		simulating=FALSE;
            //Deconstructs the car
    		car[i]->deconstructCar();
    	}

        //Sorts the cars by score, and keeps the first KILLMAX number of cars for the next generation
        kill();

        //prints out the KILLMAX cars' furthest positions
        for (int i=0; i<CarCount; i++)
        {
            qDebug()<<"Car "<<i<<": itr: "<<car[i]->iterations<<", pos:"<<car[i]->position<<endl;
        }
        //Breeds cars to repopulate car array
        breed();
        //mutates nodes on cars
        mutate();

    }
    //Final kill, gives best population
    kill();

    //Starts graphical representation of final population
	dontdographics=FALSE;
}

//Walls are defined in ball.h
extern Wall* walls[];
//Wallcount is defined in physicsengine.cpp
extern int WallCount;
//Creates one of two racecourses
void makeRaceCourse(int courseNumber)
{
    //Sets the maximum number of walls
	WallCount=9;
    //Creates the first racetrack using walls
    if(courseNumber==0)
    {
        walls[0]=new Wall(1,500,499,500);
        walls[1]=new Wall(-20,132,123,285);
        walls[2]=new Wall(104,285,203,277);
        walls[3]=new Wall(202,275,271,344);
        walls[4]=new Wall(271,344,320,344);
        walls[5]=new Wall(321,345,354,318);
        walls[6]=new Wall(354,318,394,324);
        walls[7]=new Wall(394,324,429,390);
        walls[8]=new Wall(429,391,498,401);
    }

    //Creates the second racetrack
    else if(courseNumber==1)
    {
        walls[0]=new Wall(1,500,499,600);
        walls[1]=new Wall(-20,132,123,285);
        walls[2]=new Wall(104,285,203,277);
        walls[3]=new Wall(202,275,290,350);
        walls[4]=new Wall(271,344,320,344);
        walls[5]=new Wall(321,345,354,318);
        walls[6]=new Wall(354,318,394,324);
        walls[7]=new Wall(394,324,429,390);
        walls[8]=new Wall(429,391,498,450);
    }

    //Adds the walls to the scene
	for(int i=0; i<WallCount; i++)
		thescene->addItem(walls[i]);
}

//Sets up window, starts simulation
int main(int argc, char **argv)
{

    //Handles the GUI, GUI settings, events, etc.
	QApplication app(argc,argv);

    //seeds random number geneartor
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //creates instance that handles 2D graphics items, i.e. QGraphicsItems in this case
	thescene=new QGraphicsScene();
    //sets the width and height of the scene's bounding rectangle
	thescene->setSceneRect(0,0,WIDTH,HEIGHT);


    //create first generation of cars
	for (int i=0; i<CarCount; i++)
		car[i]=new Car(10);

    //Creates the track which the cars run on
    makeRaceCourse(0);

    /*Runs for a set number of generations and
    * Kills, breeds, and mutates new cars for each
    * This is NOT visual, only the final (and suppoposedly best)
    * Generation is shown*/
    doCars();

	currentCar=0;
	car[currentCar]->constructCar();
	simulating=TRUE;

	view=new WindowView(thescene);
	view->setWindowTitle("Genetic Cars");
	view->resize(WIDTH+50,HEIGHT+50);
	view->show();
	view->setMouseTracking(true);

	TimerHandler timer(timeunit);

	return app.exec();
}
