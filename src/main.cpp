#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "util.h"
#include "mutil.h"

#define WIDTH  800
#define HEIGHT 600

static GLuint VBO;
static GLint gTransformLoc;

void createVBuffer() {
    float v[3][3] {
	{-1,-1,0},
	{0,1,0},
	{1,-1,0}
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);


    static float scale = 0.0;
    static float delta = 0.01;
    static float scale2 = 0.5;
    static float delta2 = 0.01;
    static float scale3 = 0.0;
    static float delta3 = 0.01;

    mutil::Mat4f mattrans {
	1,0,0,scale,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
    };
    scale += delta;
    if (scale >= 1.0 || scale <= -1.0)
	delta *= -1;

    mutil::Mat4f matrot {
	cosf(scale3), -sinf(scale3),0,0,
	sinf(scale3), cosf(scale3), 0,0,
	0,0,1,0,
	0,0,0,1
    };
    scale3 += delta3;
    if (scale3 >= 2*M_PI || scale3 <= -2*M_PI)
	delta3 *= -1;

    mutil::Mat4f matscale {
	scale2,0,0,0,
	0,scale2,0,0,
	0,0,1,0,
	0,0,0,1
    };
    scale2 += delta2;
    if (scale2 >= 1.0 || scale2 <= 0.5)
	delta2 *= -1;

    mutil::Mat4f transform = mattrans * matrot * matscale;
    glUniformMatrix4fv(gTransformLoc, 1, GL_TRUE, &transform[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);


    glutSwapBuffers();
    glutPostRedisplay();
}

void init(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(WIDTH/2, HEIGHT/2);
    glutCreateWindow("a.out");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
	fprintf(stderr, "error: %s\n", glewGetErrorString(err));
	std::exit(1);
    }

    GLclampf r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
    glClearColor(r,g,b,a);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
}

GLuint initProg() {
    std::vector<GLuint> shaders;
    shaders.push_back(compileShader(GL_VERTEX_SHADER, "data/vert.glsl"));
    shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "data/frag.glsl"));

    GLuint program = linkProg(shaders);
    validateProg(program);

    return program;
}

int main(int argc, char **argv) {
    try {
	init(argc, argv);
	GLuint program = initProg();

	glUseProgram(program);
	gTransformLoc = glGetUniformLocation(program, "gTransform");
	if (gTransformLoc == -1) {
	    const char *msg = "glGetUniformLocation: invalid uniform 'gTransform'";
	    throw std::runtime_error(msg);
	}
	createVBuffer();


	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
    } catch (const std::exception &e) {
	std::cerr << e.what() << '\n';
	return 1; 
    }
}
