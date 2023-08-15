#include "glquake.h"

unsigned int post_framebuffer;

void Post_GenFramebuffer() {
	glGenFramebuffer(1, &post_framebuffer);

} 