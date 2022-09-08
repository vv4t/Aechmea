#include "c_local.h"

void c_input_event(cinput_t* input, in_event_t* event) {
	switch (event->type) {
		case IN_KEYUP:
		case IN_KEYDOWN: {
			float d = 2 * (event->type == IN_KEYDOWN) - 1;
			
			switch (event->kbutton) {
				case KEY_W:
					input->axis[2] += d;
					break;
				
				case KEY_S:
					input->axis[2] -= d;
					break;
				
				case KEY_D:
					input->axis[0] += d;
					break;
				
				case KEY_A:
					input->axis[0] -= d;
					break;
				
				case KEY_SPACE:
					input->axis[1]  = event->type == IN_KEYDOWN;
			}
		}; break;

		case IN_MOUSEUP:
		case IN_MOUSEDOWN:
			input->attack[event->mbutton] = event->type == IN_MOUSEDOWN;

			break;
		
		case IN_MOUSEMOVE: {
			input->yaw		= event->mpos[0];
			input->pitch	= event->mpos[1];	
		}; break;
	}
}
