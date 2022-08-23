#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>
#include <algorithm>
#include "util.h"

GLuint compileShader(GLenum type, const char *file) {
    std::ifstream in(file);

    if (!in.is_open()) {
	std::stringstream msg;
	msg << "compileShader: file '" << file << "' failed to open";
	throw std::runtime_error(msg.str());
    }

    std::stringstream ss;
    ss << in.rdbuf();

    std::string text = ss.str();
    const char *p = text.c_str();
    GLint size = text.size(); 

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &p, &size);
    glCompileShader(shader);

    GLint status, logLen;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

    GLchar *log = new GLchar[logLen];
    glGetShaderInfoLog(shader, logLen, nullptr, log);

    if (status == GL_FALSE) {
	std::stringstream msg;
	msg << "compileShader: file '" << file << "' failed to compile\n";
	msg << log;
	throw std::runtime_error(msg.str());
    }

    std::cerr << "compiled (" << shader << ") '" << file << "'\n";

    delete[] log;
    return shader;
}

GLuint linkProg(const std::vector<GLuint> &shaders) {
    GLuint program = glCreateProgram();

    for (const auto &e : shaders) {
	glAttachShader(program, e);
    }

    glLinkProgram(program);

    GLint status, logLen;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

    GLchar *log = new GLchar[logLen];
    glGetProgramInfoLog(program, logLen, nullptr, log);

    if (status == GL_FALSE) {
	std::stringstream msg;
	msg << "linkProg: failure to link shaders ";
	for (const auto &e : shaders) {
	    msg << e << ' ';
	}
	msg << '\n' << log;
	throw std::runtime_error(msg.str());
    }

    std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

    std::stringstream msg;
    msg << "linked (" << program << ") shaders ";
    for (const auto &e : shaders) {
	msg << e << ' ';
    }
    std::cerr << msg.str() << '\n';

    delete[] log;
    return program;
}

void validateProg(GLuint program) {
    glValidateProgram(program);

    GLint status, logLen;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

    GLchar *log = new GLchar[logLen];
    glGetProgramInfoLog(program, logLen, nullptr, log);

    if (status == GL_FALSE) {
	std::stringstream msg;
	msg << "validateProg: failure to validate program ";
	msg << program << '\n' << log;
	throw std::runtime_error(msg.str());
    }

    std::cerr << "validated program " << program << '\n';

    delete[] log;
}
