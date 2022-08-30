#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "util.h"
#include "mutil.h"
#include "world_transform.h"
#include "camera_transform.h"
#include "perspective_transform.h"
#include "projection_params.h"

static GLuint VBO;
static GLuint IBO;
static GLint gTransformLoc;

static camera_transform camera;
static projectionParams pParams;
static perspective_transform perspective(&pParams);

void createVBuffer() {
    using mutil::Vert, mutil::Vec3f;
    Vert v[8] {
	{0.5,	0.5,	0.5},
	{-0.5,	0.5,	-0.5},
	{-0.5,	0.5,	0.5},
	{0.5,	-0.5,	-0.5},
	{-0.5,	-0.5,	-0.5},
	{0.5,	0.5,	-0.5},
	{0.5,	-0.5,	0.5},
	{-0.5,	-0.5,	0.5}
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
}

void createIBuffer() {
    unsigned int i[] {
	0, 1, 2,
	1, 3, 4,
	5, 6, 3,
	7, 3, 6,
	2, 4, 7,
	0, 7, 6,
	0, 5, 1,
	1, 5, 3,
	5, 0, 6,
	7, 4, 3,
	2, 1, 4,
	0, 2, 7
    };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(i),i,GL_STATIC_DRAW);
}

void cb_display() {
    glClear(GL_COLOR_BUFFER_BIT);

    static float angle = 0.0;
    static float delta = 0.005;
    angle += delta;

    world_transform w;
    w.setRotate({0,angle,0});
    w.setTranslate({0,0,2});
    
    mutil::Mat4f WVP = perspective.get() * camera.get() * w.get();
    glUniformMatrix4fv(gTransformLoc, 1, GL_TRUE, &WVP[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, 0);
    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);


    glutSwapBuffers();
    glutPostRedisplay();
}

void cb_reshape(int w, int h) {
    pParams.width = w;
    pParams.height = h; 
    glViewport(0,0,w,h);
}

void cb_keyboard(unsigned char key, int x, int y) {
    camera.handle(key,x,y);
}

void cb_special(int key, int x, int y) {
    camera.handle(key,x,y);
}

void init(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(pParams.width, pParams.height);
    glutInitWindowPosition(pParams.width/2, pParams.height/2);
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

void registerCB() {
    glutDisplayFunc(cb_display);
    glutReshapeFunc(cb_reshape); 
    glutKeyboardFunc(cb_keyboard);
    glutSpecialFunc(cb_special);
}

int main(int argc, char **argv) {
    srandom(getpid());
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
	createIBuffer();

	registerCB();
	glutMainLoop();
	return 0;
    } catch (const std::exception &e) {
	std::cerr << e.what() << '\n';
	return 1; 
    }
}
