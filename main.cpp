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
    unsigned int event_time;
    EVENT_TYPE event_type;

    /* As stated in the prompt it should have pointers to the next and previous events */
    Event* next;
    Event* prev;
};

struct GEL {
    /* Essentially a double linked list consisting of 'Events'
     * It will be sorted from least to greatest in terms of 'event_time' */
    list<Event> list;
};

struct Queue {
    /* To be FIFO. 'Events' get enqueued when the timer reaches the event's
     * specified 'event_time' and(?) if there's another 'Event' already being processed */

    /* Using the queue library provides us with a DS that comes with pop_front and push_back
     * functions to quickly meet the requirements for a FIFO queue */
    queue<Event> buffer;
};

int main() {
    /* Initializing */
    unsigned int length = 0, time = 0;

    /* Lambda will be a different value with every run so that we can gather data to plot our graph */
    /* Lambda and mu values are found in the 3.7 Experiment section of the project. */
    unsigned int lambda = 0.1, mu = 1;

    /* Generate the random events and add them to our GEL. */
    int arrival_time, process_time;
    for (int i = 0; i < 1000; ++i) {
        negative_exponentially_distributed_time(lambda);
        negative_exponentially_distributed_time(mu);
    }
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
