#include "render.h"

#include "r_local.h"

typedef struct {
	GLenum type;
	const char* s;
} gldebug_t;

static gldebug_t gldebug[] = {
	{ GL_DEBUG_TYPE_ERROR, "ERROR" },
	{ GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED BEHAVIOUR" },
	{ GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED BEHAVIOUR" },
	{ GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY" },
	{ GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE" },
	{ GL_DEBUG_TYPE_MARKER, "MASKER" },
	{ GL_DEBUG_TYPE_PUSH_GROUP, "PUSH GROUP" },
	{ GL_DEBUG_TYPE_POP_GROUP, "POP GROUP" },
	{ GL_DEBUG_TYPE_OTHER, "OTHER" },
	{ GL_DEBUG_SEVERITY_HIGH, "HIGH" },
	{ GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM" },
	{ GL_DEBUG_SEVERITY_LOW, "LOW"},
	{ GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION" },
};

gldebug_t* gl_debug_find(GLenum type) {
	for (int i = 0; i < 13; i++) {
		if (type == gldebug[i].type)
			return &gldebug[i];
	}
}

void GLAPIENTRY
r_error( GLenum source,
					GLenum type,
					GLuint id,
					GLenum severity,
					GLsizei length,
					const GLchar* message,
					const void* userParam ) {
	gldebug_t* gl_type = gl_debug_find(type);
	gldebug_t* gl_severity = gl_debug_find(severity);

	if ( type == GL_DEBUG_TYPE_ERROR  ) {
		com_printf( LOG_ERROR, "GL CALLBACK: ** GL_ERROR ** type = %s, severity = %s, message = %s",
				gl_type->s, gl_severity->s, message );
	} else {
		com_printf( LOG_DEBUG, "GL CALLBACK: type = %s, severity = %s, message = %s",
				gl_type->s, gl_severity->s, message );
	}
}

void r_init(int vertex_buffer_size, int index_buffer_size) {
	glewExperimental = 1;

	if (glewInit() != GLEW_OK)
		com_printf(LOG_ERROR, "Failed to initialize GLEW.");

	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( r_error, 0 );

	glEnable(GL_DEPTH_TEST);
	
	// glEnable(GL_CULL_FACE);
	
	// glCullFace(GL_FRONT);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	r_init_buffer(vertex_buffer_size, index_buffer_size);

}

void r_clear(int buffer) {
	glClear(buffer);
}

void r_color(float r, float g, float b) {
	glClearColor(r, g, b, 1.0);
}
