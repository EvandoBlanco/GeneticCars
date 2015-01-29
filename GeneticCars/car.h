//Car object: balls & links
class Car
{
public:
	//define the car
	int* balls_x;		//x coor of each ball
	int* balls_y;		//y coor of each ball
	int** links;		//matrix  of which ball is connected to which ball

    int scr;
	//just for car race
	int* balls;		//array of the balls that make up the car
	int* linkslist;		//array of links that make up the car

        int nodes;		//how many balls in the car
        int numlinks;		//how many links in the car
	int iterations;		//race results: iterations = # frames
	int position;		//race result: how far it got

	Car(int);		//constructor # nodes
	void constructCar();	//given ballsx ballsy and links, actually make balls and links for the car
	void deconstructCar();	//clears the car out of the racetrack

    int getCarPosition(int);	//tells us how far the car got
    int score(int,int);	//sets itr and pos at end of race

	Car* breed(Car*);	//haven't made: breeds the car with another car, returning a new car
	Car* mutate();		//haven't made: makes a new mutant car

	//destruct the car on delete car
	~Car()
	{
		delete balls_x;
		delete balls_y;
		for(int i=0; i<nodes; i++)
			delete links[i];
		delete links;
	}
};
