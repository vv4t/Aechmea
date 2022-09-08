#include "input.h"

int			event_head = 0;
in_event_t	event_queue[32];

void in_queue_event(in_event_t event) {
	event_queue[event_head++] = event;
}

int in_poll_event(in_event_t** event) {
	int remaining = event_head;
	
	if (event_head > 0)
		*event = &event_queue[--event_head];
	
	return remaining;
}
