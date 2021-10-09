#include<iostream>
using namespace std;

enum Planestatus {null, arriving, departing};
class Plane {
public:
Plane( );
Plane(int flt, int time, Planestatus status);
void refuse( ) const;
void land(int time) const;
void fly(int time) const;
int started( ) const;
private:
int fltnum;
int clockstart;
Planestatus state;
};

enum Runwayactivity {idle, land, takeoff};
class Runway {
public:
Runway(int limit);
Error_code canland(const Plane &current);
Error_code candepart(const Plane &current);
Runwayactivity activity(int time, Plane &moving);
void shutdown(int time) const;
private:
Extended_queue landing;
Extended_queue takeoff;
int queuelimit;
int numlandrequests; // number of planes asking to land
int numtakeoffrequests; // number of planes asking to take off
int numlandings; // number of planes that have landed
int numtakeoffs; // number of planes that have taken off
int numlandaccepted; // number of planes queued to land
int numtakeoffaccepted; // number of planes queued to take off
int numlandrefused; // number of landing planes refused
int numtakeoffrefused; // number of departing planes refused
int landwait; // total time of planes waiting to land
int takeoffwait; // total time of planes waiting to take off
int idletime; // total time runway is idle
};



void initialize(int &endtime, int &queuelimit,double &arrivalrate, double &departurerate)
/* Pre: The user specifies the number of time units in the simulation, the maximal queue
sizes permitted, and the expected arrival and departure rates for the airport.
Post: The program prints instructions and initializes the parameters end time,
queue limit, arrival rate, and departure rate to the specified values.
Uses: utility function user says yes */
{ cout << "This program simulates an airport with only one runway."
<< endl
<< "One plane can land or depart in each unit of time." << endl;
cout << "Up to what number of planes can be waiting to land "
<< "or take off at any time? " << flush;
cin >> queuelimit;
cout << "How many units of time will the simulation run?" << flush;
cin >> endtime;
bool acceptable;
do {
cout << "Expected number of arrivals per unit time?" << flush;
cin >> arrivalrate;
cout << "Expected number of departures per unit time?" << flush;
cin >> departurerate;
if (arrivalrate < 0.0 || departurerate < 0.0)
cerr << "These rates must be nonnegative." << endl;
else acceptable = true;
if (acceptable && arrivalrate + departurerate > 1.0)
cerr<< "Safety Warning: This airport will become saturated. "
<< endl;
} while (!acceptable);
}

Runway :: Runway(int limit)
/* Post: The Runway data members are initialized to record no prior Runway use
and to record the limit on queue sizes. */
{
queuelimit = limit;
numlandrequests = numtakeoffrequests = 0;
numlandings = numtakeoffs = 0;
numlandrefused = numtakeoffrefused = 0;
numlandaccepted = numtakeoffaccepted = 0;
landwait = takeoffwait = idletime = 0;
}
Error_code Runway :: canland(const Plane &current)
/* Post: If possible, the Plane current is added to the landing Queue; otherwise, an
Error code of overflow is returned. The Runway statistics are updated.
Uses: class Extended queue. */
{
Error_code result;
if (landing.size( ) < queuelimit)
result = landing.append(current);
else
result = fail;
numland requests++;
if (result != success)
numlandrefused++;
else
numlandaccepted++;
return result;
}

Runwayactivity Runway :: activity(int time, Plane &moving)
/* Post: If the landing Queue has entries, its front Plane is copied to the parameter
moving and a result land is returned. Otherwise, if the takeoff Queue has
entries, its front Plane is copied to the parameter moving and a result takeoff
is returned. Otherwise, idle is returned. Runway statistics are updated.
Uses: class Extended queue. */
{
Runwayactivityinprogress;
if (!landing.empty( )) {
landing.retrieve(moving);
landwait =landwait+ time − moving.started( );
numlandings++;
inprogress = land;
landing.serve( );
}
else if (!takeoff.empty( )) {
takeoff.retrieve(moving);
takeoffwait += time − moving.started( );
numtakeoffs++;
inprogress = takeoff;
takeoff.serve( );
}
else {
idletime++;
inprogress = idle;
}
return inprogress;
}

Plane :: Plane(int flt, int time, Planestatus status)
/* Post: The Plane data members flt num, clock start, and state are set to the
values of the parameters flt, time and status, respectively. */
{
fltnum = flt;
clockstart = time;
state = status;
cout << "Plane number " << flt << " ready to ";
if (status == arriving)
cout << "land." << endl;
else
cout << "take off." << endl;
}
Plane :: Plane( )
/* Post: The Plane data members fltnum, clockstart, state are set to illegal default
values. */
{
fltnum = −1;
clockstart = −1;
state = null;
}

void Plane :: refuse( ) const
/* Post: Processes a Plane wanting to use Runway, when the Queue is full. */
{
cout << "Plane number " << fltnum;
if (state == arriving)
cout << " directed to another airport" << endl;
else
cout << " told to try to takeoff again later" << endl;
}
void Plane :: land(int time) const
/* Post: Processes a Plane that is landing at the specified time. */
{
int wait = time − clockstart;
cout << time << ": Plane number " << fltnum << " landed after "
<< wait << " time unit" << ((wait == 1) ? "" : "s")
<< " in the takeoff queue." << endl;
}
void Plane :: fly(int time) const
/* Post: Process a Plane that is taking off at the specified time. */
{
int wait = time − clockstart;
cout << time << ": Plane number " << fltnum << " took off after "
<< wait << " time unit" << ((wait == 1) ? "" : "s")
<< " in the takeoff queue." << endl;
}

void Runway :: shut down(int time) const
/* Post: Runway usage statistics are summarized and printed. */
{
cout << "Simulation has concluded after " << time << " time units."
<< endl
<< "Total number of planes processed "
<< (numlandrequests + numtakeoffrequests) << endl
<< "Total number of planes asking to land "
<< numlandrequests << endl
<< "Total number of planes asking to take off "
<< numtakeoffrequests << endl
<< "Total number of planes accepted for landing "
<< numlandaccepted << endl
<< "Total number of planes accepted for takeoff "
<< numtakeoffaccepted << endl
<< "Total number of planes refused for landing "
<< numlandrefused << endl
<< "Total number of planes refused for takeoff "
<< numtakeoffrefused << endl
<< "Total number of planes that landed "
<< numlandings << endl
<< "Total number of planes that took off "
<< numtakeoffs << endl
<< "Total number of planes left in landing queue "
<< landing.size( ) << endl
<< "Total number of planes left in takeoff queue "
<< takeoff.size( ) << endl;
cout<< "Percentage of time runway idle "
<< 100.0 * ((float) idletime)/((float) time) << "%" << endl;
cout<< "Average wait in landing queue "
<< ((float) landwait)/((float) numlandings) << " time units";
cout<< endl << "Average wait in takeoff queue "
<< ((float) takeoffwait)/((float) numtakeoffs)
<< " time units" << endl;
cout<< "Average observed rate of planes wanting to land "
<< ((float) numlandrequests)/((float) time)
<< " per time unit" << endl;
cout<< "Average observed rate of planes wanting to take off "
<< ((float) numtakeoffrequests)/((float) time)
<< " per time unit" << endl;
}

int main( ) // Airport simulation program
/* Pre: The user must supply the number of time intervals the simulation is to run, the
expected number of planes arriving, the expected number of planes departing
per time interval, and the maximum allowed size for runway queues.
Post: The program performs a random simulation of the airport, showing the status of
the runway at each time interval, and prints out a summary of airport operation
at the conclusion.
Uses: Classes Runway, Plane, Random and functions run idle, initialize. */
{ int endtime; // time to run simulation
int queuelimit; // size of Runway queues
int flightnumber = 0;
double arrivalrate, departurerate;
initialize(endtime, queuelimit, arrivalrate, departurerate);
Random variable; Runway smallairport(queuelimit);
for (int currenttime = 0; currenttime < endtime; currenttime++) {
int numberarrivals = variable.poisson(arrivalrate);
for (int i = 0; i < numberarrivals; i++) {
Plane currentplane(flightnumber++, currenttime, arriving);
if (smallairport.canland(currentplane) != success)
currentplane.refuse( ); }
int numberdepartures = variable.poisson(departurerate);
for (int j = 0; j < numberdepartures; j++) {
Plane currentplane(flightnumber++, currenttime, departing);
if (smallairport.candepart(currentplane) != success)
currentplane.refuse( ); }
Plane movingplane;
switch (smallairport.activity(currenttime, movingplane)) {
case land: movingplane.land(currenttime); break;
case takeoff: movingplane.fly(currenttime); break;
case idle: runidle(currenttime); }
}
smallairport.shutdown(endtime);
return 0;
}


