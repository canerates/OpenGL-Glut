#include <glut.h>
#include <math.h>

GLfloat angleX, angleY, angleZ = 0;
GLfloat scaleX = 1.5f; GLfloat scaleY = 1.5f; GLfloat scaleZ = 1.5f;
GLfloat transX = 0; GLfloat transY = 0;

void changeSize(int, int);
void renderScene(void);
void drawCoordinates();
void drawCube();
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Cube Transformations");
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

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
	gluLookAt(0.0f, 0.0f, 2.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
		
	drawCoordinates();
	drawCube();
	
	glutSwapBuffers();
}

void drawCoordinates() {

	// x-axis
	glColor3f(1.0, 0.0, 0.0); // red
	glBegin(GL_LINES);
	glVertex3f(-40.0f, 0.0f, 0.0f);
	glVertex3f(40.0f, 0.0f, 0.0f);
	glEnd();

	// y-axis
	glColor3f(0.0, 10.0, 0.0); // green
	glBegin(GL_LINES);
	glVertex3f(0.0, -40.0f, 0.0f);
	glVertex3f(0.0, 40.0f, 0.0f);
	glEnd();

	// z-axis
	glColor3f(0.0, 0.0, 10.0); // blue
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0f, -40.0f);
	glVertex3f(0.0, 0.0f, 40.0f);
	glEnd();
}

void drawCube()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glScalef(scaleX, scaleY, scaleZ); // scale

	glRotatef(angleX, 1, 0, 0); //rotate alpha around the x axis
	glRotatef(angleY, 0, 1, 0); //rotate beta around the y axis
	glRotatef(angleZ, 0, 0, 1); //rotate gamma around the z axis

	glTranslatef(transX, transY, 0); // translate

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);    // Color RED - Top

	glVertex3f(1.0, 1.0, 1.0); // TOP-RIGHT-NEAR
	glVertex3f(-1.0, 1.0, 1.0); // TOP-LEFT-NEAR
	glVertex3f(-1.0, 1.0, -1.0); //TOP-LEFT-FAR
	glVertex3f(1.0, 1.0, -1.0); // TOP-RIGHT-FAR

	glColor3f(1.0, 0.0, 0.0); // Color RED - Bottom

	glVertex3f(1.0, -1.0, 1.0); //BOTTOM-RIGHT-NEAR
	glVertex3f(-1.0, -1.0, 1.0); //BOTTOM-LEFT-NEAR
	glVertex3f(-1.0, -1.0, -1.0); //BOTTOM-LEFT-FAR
	glVertex3f(1.0, -1.0, -1.0); //BOTTOM-RIGHT-FAR

	glColor3f(1.0, 1.0, 0.0); // Color YELLOW - Back

	glVertex3f(1.0, 1.0, -1.0); //TOP-RIGHT-FAR
	glVertex3f(-1.0, 1.0, -1.0); //TOP-LEFT-FAR
	glVertex3f(-1.0, -1.0, -1.0); //BOTTOM-LEFT-FAR
	glVertex3f(1.0, -1.0, -1.0); //BOTTOM-RIGHT-FAR

	glColor3f(0.0, 0.0, 1.0); //Color BLUE - Right

	glVertex3f(1.0, 1.0, 1.0); //TOP-FRONT-NEAR
	glVertex3f(1.0, 1.0, -1.0); //TOP-BACK-FAR
	glVertex3f(1.0, -1.0, -1.0); //BOTTOM-BACK-FAR
	glVertex3f(1.0, -1.0, 1.0); //BOTTOM-FRONT-NEAR

	glColor3f(0.0, 0.0, 1.0); //Color BLUE - Left

	glVertex3f(-1.0, 1.0, 1.0); //TOP-FRONT-NEAR
	glVertex3f(-1.0, 1.0, -1.0); //TOP-BACK-FAR
	glVertex3f(-1.0, -1.0, -1.0);//BOTTOM-BACK-FAR
	glVertex3f(-1.0, -1.0, 1.0); //BOTTOM-FRONT-NEAR

	glColor3f(1.0, 1.0, 0.0); //Color YELLOW- Front

	glVertex3f(-1.0, -1.0, 1.0); //TOP-FRONT-NEAR
	glVertex3f(1.0, -1.0, 1.0); //TOP-BACK-FAR
	glVertex3f(1.0, 1.0, 1.0);//BOTTOM-BACK-FAR
	glVertex3f(-1.0, 1.0, 1.0); //BOTTOM-FRONT-NEAR

	glEnd();
	glPopMatrix();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 's':
		angleX += 10.0f;
		glutPostRedisplay();
		break;
	case 'w':
		angleX -= 10.0f;
		glutPostRedisplay();
		break;
	case 'd':
		angleY += 10.0f;
		glutPostRedisplay();
		break;
	case 'a':
		angleY -= 10.0f;
		glutPostRedisplay();
		break;
	case 'e':
		angleZ += 10.0f;
		glutPostRedisplay();
		break;
	case 'r':
		angleZ -= 10.0f;
		glutPostRedisplay();
		break;
	case '+':
		scaleX += 0.5f;
		scaleY += 0.5f;
		scaleZ += 0.5f;
		glutPostRedisplay();
		break;
	case '-':
		scaleX -= 0.5f;
		scaleY -= 0.5f;
		scaleZ -= 0.5f;
		glutPostRedisplay();
		break;
	}
}

void processSpecialKeys(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		transX -= 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		transX += 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		transY += 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		transY -= 1.0f;
		glutPostRedisplay();
		break;
	}
}
