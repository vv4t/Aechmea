#include "window.h"

#include "input.h"
#include "common.h"

#include <GLFW/glfw3.h>

GLFWwindow* window;

float mouse_pos[2];

void keyboard_callback(GLFWwindow* window, int key, int scan, int action, int mods) {
	
	in_event_t event;
	
	switch(action) {
		case GLFW_PRESS:
			event.type		= IN_KEYDOWN;
			event.kbutton	= key;
			
			in_queue_event(event);
			
			break;
		
		case GLFW_RELEASE:
			event.type		= IN_KEYUP;
			event.kbutton	= key;
			
			in_queue_event(event);
			
			break;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	in_event_t event;
	
	switch(action) {
		case GLFW_PRESS:
			event.type		= IN_MOUSEDOWN;
			event.mbutton	= button;
			
			in_queue_event(event);
			
			break;
		
		case GLFW_RELEASE:
			event.type		= IN_MOUSEUP;
			event.mbutton	= button;
			
			in_queue_event(event);
			
			break;
	}
}

void mousemove_callback(GLFWwindow* window, double xpos, double ypos) {
	float dx = xpos - mouse_pos[0];
	float dy = ypos - mouse_pos[1];
	
	mouse_pos[0] = xpos;
	mouse_pos[1] = ypos;
	
	in_event_t event;
		event.type		= IN_MOUSEMOVE;
		event.mpos[0]	= dx;
		event.mpos[1]	= dy;
	
	in_queue_event(event);
}

void win_init(int width, int height, const char* title) {
	if (!glfwInit())
		com_printf(LOG_ERROR, "Failed to initialize GLFW.");

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	glfwMakeContextCurrent(window);
}

void win_cursor_lock(void) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void win_poll(void) {
	glfwPollEvents();
}

void win_input_init(void) {
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetCursorPosCallback(window, mousemove_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void win_exit() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

int win_loop() {
	glfwSwapBuffers(window);

	return !glfwWindowShouldClose(window);
}

float win_time() {
	return glfwGetTime();
}
