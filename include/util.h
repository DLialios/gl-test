#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <GL/glew.h>

extern GLuint compileShader(GLenum type, const char *file);
extern GLuint linkProg(const std::vector<GLuint> &shaders);
extern void validateProg(GLuint program);

#endif
