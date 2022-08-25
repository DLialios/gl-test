#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "util.h"
#include "mutil.h"

static int WIDTH = 800;
static int HEIGHT = 600;
static GLuint VBO;
static GLuint IBO;
static GLint gTransformLoc;

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);


    static float angle = 0.0;
    static float delta = 0.005;
    mutil::Mat4f roty {
	cosf(angle),	0,  sinf(angle),   0,
	0,		1,  0,		   0,
	-sinf(angle),	0,  cos(angle),	   0,
	0,		0,  0,	           1
    };
    mutil::Mat4f rotz {
	cosf(angle),	-sinf(angle),	0,  0,
	sinf(angle),	cos(angle),	0,  0,
	0,		0,		1,  0,
	0,		0,		0,  1

    };
    angle += delta;

    mutil::Mat4f translate {
	1,  0,	0,  0,
	0,  1,	0,  0,
	0,  0,	1,  2,
	0,  0,	0,  1
    };

    float fov = M_PI / 2;
    float tanHalfFOV = tanf(fov / 2);
    float d = 1 / tanHalfFOV;
    float ar = (float) WIDTH / HEIGHT;
    float nz = 1;
    float fz = 10;
    static const float A = (-nz-fz)/(nz-fz);
    static const float B = (2*fz*nz)/(nz-fz);
    mutil::Mat4f projection {
	d/ar,0,0,0,
	0,d,0,0,
	0,0,A,B,
	0,0,1,0
    };

    mutil::Mat4f transform = projection * translate * rotz * roty;
    glUniformMatrix4fv(gTransformLoc, 1, GL_TRUE, &transform[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(mutil::Vec3f), 0);
    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(mutil::Vec3f), (void*)sizeof(mutil::Vec3f));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);


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

void reshape(int w, int h) {
    WIDTH = w;
    HEIGHT = h; 
    glViewport(0,0,w,h);
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

	glutDisplayFunc(display);
	glutReshapeFunc(reshape); 
	glutMainLoop();
	return 0;
    } catch (const std::exception &e) {
	std::cerr << e.what() << '\n';
	return 1; 
    }
}
