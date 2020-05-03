#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <cmath>

using namespace std;

typedef enum {
	ARRIVAL = 0,
	DEPARTURE = 1
} EVENT_TYPE;

/* Below is just a skeleton and template. Feel free to propose changes
 * or ask any questions. */

struct Event {
    /* We can rename the variables later to be shorter if needed */

    /* Event time is when the packet arrives (ARRIVAL event).
     * Event time is when packet the server is done transmitting the packet (DEPARTURE event). */
    unsigned int event_time;
    EVENT_TYPE event_type;

    /* As stated in the prompt it should have pointers to the next and previous events */
    /* Temporarily removed since we don't think we need a use for it at the moment. */
    //Event* next;
    //Event* prev;

    /* Constructor */
    Event(int a, EVENT_TYPE b) : event_time(a), event_type(b) {}
};

/* Essentially a double linked list consisting of 'Events'
 * It will be sorted from least to greatest in terms of 'event_time' */
list<Event*> GEL;


/* To be FIFO. 'Events' get enqueued when the timer reaches the event's
 * specified 'event_time' and(?) if there's another 'Event' already being processed */
/* Using the queue library provides us with a DS that comes with pop_front and push_back
 * functions to quickly meet the requirements for a FIFO queue */
queue<Event*> buffer;

int main() {
    /* Initializing */
    unsigned int length = 0, time = 0;

    /* Lambda will be a different value with every run so that we can gather data to plot our graph */
    /* Lambda and mu values are found in the 3.7 Experiment section of the project. */
    unsigned int lambda = 0.1, mu = 1;

    /* Generate the random events all as arrival at first. */
    /* We will handle process + departure time in the main loop. */
    int arrival_time;
    for (int i = 0; i < 1000; ++i) {
        arrival_time = negative_exponentially_distributed_time(lambda);

        /* Construct a new object and push it into the GEL */
        GEL.emplace_back(arrival_time, ARRIVAL);
    }
    /* #### We need to sort the GEL before doing anything else!!!! ####*/


    /* Base template provided in the prompt

    Initialize;
    for (i = 0; i < 100000; i++){
        1.  get the first event from the GEL;  
        2.  If the event is an arrival then process-arrival-event; 
        3.  Otherwise it must be a departure event and hence process-service-completion;
    }
    output-statistics;

    */
   return 0;
}

double negative_exponentially_distributed_time (double rate)  
{
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u)); 
}
