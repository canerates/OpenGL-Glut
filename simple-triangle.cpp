#include <glut.h>

void changeSize(int, int);
void renderScene(void);

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Simple Triangle");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMainLoop();

	return 1;
}
void changeSize(int w, int h)
{
	// use the projection matrix
	glMatrixMode(GL_PROJECTION);

	// reset matrix
	glLoadIdentity();

	// set the viewport to be the entire window
	glViewport(0, 0, w, h);

	glOrtho(-10, 10, -10, 10, -10, 10);

	// get back to the modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	// background color
	glClearColor(1.0, 1.0, 1.0, 1.0); 

	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset transformations
	glLoadIdentity(); 

	// set the camera
	gluLookAt(0.0f, 0.0f, 10.0f,
			  0.0f, 0.0f, 0.0f,
		      0.0f, 1.0f, 0.0f);

	// triangle
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0); glVertex3f(8.6603, -5, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 10, 0);
	glColor3f(0, 0, 1); glVertex3f(-8.6603, -5, 0);
	glEnd();

	glutSwapBuffers();
}