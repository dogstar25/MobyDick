#pragma once

#include <iostream>
#include <glad/glad.h>

void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar* msg, const void* data);
