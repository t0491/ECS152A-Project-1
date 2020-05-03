#define ARRIVAL     "ARRIVAL"
#define DERPARTURE  "DEPARTURE"

#include <iostream>
#include <string>


/* Below is just a skeleton and template. Feel free to propose changes
 * or ask any questions. */

/* An idea I have is to use the vector library instead of manually creating
 * a linked list since vectors already have a built in class/struct with
 * functions. Also from what I've read in piazza, it seems like we are able
 * to use structures provided by the libraries as long as they aren't "exotic" */

struct Event
{
    /* We can rename the variables later to be shorter if needed */
    int event_time;
    string event_type;

    /* Stated in the prompt, the events should be linked to one another
     * I'm assuming it is also going to be a linked list of sorts */
    Event* next;
    Event* prev;
};

struct GEL {
    /* Essentially a double linked list consisting of 'Events'
     * It will be sorted from least to greatest in terms of 'event_time' */
};

struct Queue {
    /* To be FIFO. 'Events' get enqueued when the timer reaches the event's
     * specified 'event_time' and(?) if there's another 'Event' already being processed */
};

int main() {
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