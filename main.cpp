#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <algorithm>
#include <math.h>

typedef enum {
	ARRIVAL = 0,
	DEPARTURE = 1
} EVENT_TYPE;

typedef struct {
	double service_time;
} Packet;

typedef struct {
    /* ARRIVAL: time is when the packet arrives
     * DEPARTURE: time is when the server is done transmitting the packet */
    double time; 
    EVENT_TYPE type;
	Packet *packet; /* pointer to packet the event refers to */
} Event;

/* double linked list of all events */
std::list<Event *> EVENTS;

/* double linked list of all packets */
std::list<Packet *> PACKETS;

/* double linked list of queued events sorted by event time */
std::list<Event *> GEL;

/* FIFO queue of waiting packets */
std::queue<Packet *> PACKET_BUFFER;

/* flag indicating if the server is processing a packet or not */
/* from the spec, we can get length as PACKET_BUFFER.size() + PROCESSING */
unsigned int PROCESSING = 0;

/* maximum size of PACKET_BUFFER queue */
unsigned int MAX_BUFFER = 10;

/* maximum steps to run the simulation for */
unsigned int MAX_STEPS = 500000;

/* UTILIZATION */
double UTILIZATION = 0;

/* global clock */
double CURRENT_TIME = 0;

/* reference to previous event */
double PREVIOUS_TIME = 0;

/* Previous queue length */
unsigned int PREV_QUEUE_LENGTH = 0;

/* Sum of the area under the curve for every event/iteration we do */
double SUM_OF_AREAS = 0;

/* Packets dropped */
double PACKETS_DROPPED = 0;

/* Mean length */
double NQ = 0;

/* Keeps track of how many packets were queued in the queue at any moment and sums it all up */
unsigned int total_queued = 0;

/* return negative exponentially distributed time with parameter rate */
double neg_exp_dist_time(double rate) {
    double u = drand48();
    return ((-1.0 / rate) * log(1.0 - u));
}

/* return the next time relative to the current time */
double get_next_time(double rate) {
	double next_time = neg_exp_dist_time(rate);
	return CURRENT_TIME + next_time;
}

/* create a new packet */
Packet *create_packet(double mu) {
	Packet *packet = new Packet;
	packet->service_time = neg_exp_dist_time(mu);

	PACKETS.push_back(packet);

    /* Update server busy time when a new packet w/ processing time is generated */
    UTILIZATION += packet->service_time;
	return packet;
}

void print_packet(Packet *packet) {
	std::cout << "[packet " << packet << "] ";
	std::cout << "service_time=" << packet->service_time << "\n";
}

/* print the content of a queue of packets */
void print_queue(std::queue<Packet *> queue) {
	std::queue<Packet *> copy = queue;

	while (!copy.empty()) {
		Packet *packet = copy.front();
		print_packet(packet);
				copy.pop();
	}
}

double mean_length(double p) {
	double NQ = 0;
	double Numerator = p*p;
	double Denominator = 1-p;

	NQ = Numerator/Denominator;
	return NQ;  
}

/* create a new event pointing to given packet */
Event *create_event(double lambda, EVENT_TYPE type, Packet *packet) {
	Event *event = new Event;
	event->type = type;
	event->time = get_next_time(lambda);
	event->packet = packet;

	EVENTS.push_back(event);

	return event;
}

/* sort by event time */
bool sort_events(const Event *lhs, const Event *rhs) {
	return lhs->time < rhs->time;
}

/* print the address, type, time and packet pointer of an event */
void print_event(Event *event) {	
	std::string type;
	switch(event->type) {
		case ARRIVAL: type = "A"; break;
		case DEPARTURE: type = "D"; break;
	}

	std::cout << "[event " << event << "] ";
	std::cout << "type=" << type << "\t";
	std::cout << "time=" << event->time << "\t";
	std::cout << "packet=" << event->packet << "\n";
}

/* print the content of GEL and PACKET_BUFFER */
void debug(std::string message) {
	std::cout << message << "\n";

	/* print GEL */
	std::cout << "GEL:\n"; 
	for (auto event : GEL) {
		print_event(event);
	}

	/* print PACKET_BUFFER */
	std::cout << "PACKET_BUFFER:\n";
	print_queue(PACKET_BUFFER);
	
	std::cout << "\n";
}

/* process an arrival event:
 * 	if the server is free (no packets in queue and not processing):
 * 		create a departure event for the packet pointed by the arrival event
 *	else:
 *		if queue is not full:
 *			push packet pointed by arrival event into queue
 */
void process_arrival_event(Event *event, double lambda, double mu) {
	std::cout << "PROCESS ARRIVAL EVENT\n";
	/* create next packet */
	Packet *next_packet = create_packet(mu);

	/* create next arrival event */
	Event *next_event = create_event(lambda, ARRIVAL, next_packet);
	GEL.push_back(next_event);

	/* process arrival event for departure */
	if (PACKET_BUFFER.empty() && !PROCESSING) { /* server is free */
		/* indicate that the server is processing */
		PROCESSING = 1;

		/* create a departure event for the current packet */
		double departure_time = CURRENT_TIME + event->packet->service_time;
		Event *departure_event = create_event(lambda, DEPARTURE, event->packet);
		departure_event->time = departure_time;
		GEL.push_back(departure_event);
	} else { /* server is busy; queue has packets or server is processing */
		if (PACKET_BUFFER.empty() ||  PACKET_BUFFER.size() - 1 < MAX_BUFFER) {
			/* queue is not full, push packet in queue */
			PACKET_BUFFER.push(event->packet);
			std::cout << PACKET_BUFFER.size() << "\n";
			std::cout << MAX_BUFFER << "\n";

			total_queued += PACKET_BUFFER.size();
		} else { /* queue is full */
			/* TODO record packet drop */
			PACKETS_DROPPED += 1;
			/* TODO update mean queue length */
		}
	}
}

/* process departure event:
 * 	if queue has packets:
 * 		dequeue first packet from queue
 * 		create departure event for that packet
 */
void process_departure_event(Event *event, double mu) {
	std::cout << "PROCESS DEPARTURE EVENT\n";
	/* indicate that the server is not processing */
	PROCESSING = 0;

	/* TODO update mean queue length */
	/* TODO update server busy time */

	if (!PACKET_BUFFER.empty()) { /* queue has packets */
		/* dequeue first packet from packet buffer */
		Packet *packet = PACKET_BUFFER.front();
		PACKET_BUFFER.pop();

		/* create departure event for dequeued packet */
		Event *departure_event = create_event(mu, DEPARTURE, packet);
		GEL.push_back(departure_event);

		total_queued += PACKET_BUFFER.size();
	}
}

int main(int argc, char **argv) {
	/* initialize variables */
    double lambda = 0.9; /* arrival rate */
	double mu = 1; /* service rate */

	/* create first packet */
	Packet *first_packet = create_packet(mu);

	/* create first arrival event */
	Event *first_event = create_event(lambda, ARRIVAL, first_packet);

	/* push first event into GEL */
	GEL.push_back(first_event);
	debug("first");

	int numA = 0, numD = 0;
	/* start simulation loop */
	for (unsigned int i = 0; i <= MAX_STEPS; i++) {
		/* get first element in GEL */
		Event *event = GEL.front();
		GEL.pop_front();
		
        /* Copy the current time into previous before its updated for the next event */
        PREVIOUS_TIME = CURRENT_TIME;

		/* set current time to event time */
		CURRENT_TIME = event->time;

        SUM_OF_AREAS += (CURRENT_TIME - PREVIOUS_TIME) * PREV_QUEUE_LENGTH;

		/* process first element in GEL depending on its type */
		if (event->type == ARRIVAL) {
			process_arrival_event(event, lambda, mu);
			++numA;
		} else if (event->type == DEPARTURE) {
			process_departure_event(event, mu);
			++numD;
		}

        /* Update the prev queue length so that it will be used at the start of next iteration */
        PREV_QUEUE_LENGTH = PACKET_BUFFER.size();

		/* sort GEL by event time */
		GEL.sort(sort_events);
		debug("");
	}

	/* print list of all packets, not sorted by time */
	std::cout << "TOTAL PACKETS: " << PACKETS.size() << "\n";
	for (auto packet : PACKETS) {
		print_packet(packet);	
	}

	/* print list of all events, not sorted by time */
	std::cout << "TOTAL EVENTS: " << EVENTS.size() << "\n";
	for (auto event : EVENTS) {
		print_event(event);
	}
	/* How many packets have been dropped so far */
	std::cout << "PACKETS_DROPPED: " <<  PACKETS_DROPPED << "\n";

	/* Server busy time (UTILIZATION) */
	std::cout << "TIME SPENT BUSY: " << UTILIZATION << "\n";	
	std::cout << "TOTAL TIME ELAPSED: " << CURRENT_TIME << "\n";
    std::cout << "THEORETICAL UTILIZATION: " << lambda/mu << "\n";
	std::cout << "SIMULATED (MEAN) UTILIZATION: " << UTILIZATION / CURRENT_TIME << "\n";
	
	NQ = mean_length(lambda/mu);
	/* Packet Mean Length */
	std::cout << "THEORETICAL NQ: " << NQ << "\n"; 	

    /* Current time right now should be the "total time" since the simulation is now fully over once we
     * have exited the loop. So we divide the sum of areas under the curve by current_time to get 
     * our simulated average # of packets in queue */
    double simulated_nq = SUM_OF_AREAS / CURRENT_TIME;
    std::cout << "SIMULATED NQ: " << simulated_nq << "\n";
	std::cout << "Sum of queued: " << total_queued << "\n"; 
	std::cout << "OTHER NQ: " << ((double) total_queued/ (double) MAX_STEPS) << "\n";
	std::cout << "Num Arrival Events: " << numA << "\n";
	std::cout << "Num Departure Events: " << numD << "\n";

	/* free the allocated memory for each packet */
	for (auto packet : PACKETS) {
		delete packet;
	}

	/* free the allocated memory for each event */
	for (auto event : EVENTS) {
		delete event;
	}

   	return 0;
}