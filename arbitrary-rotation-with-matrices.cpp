#include <glut.h>
#include <math.h>
#include <iostream>
#define PI 3.14159265358979323846

using namespace std;

GLfloat angleX, angleY, angleZ = 0;
GLfloat scaleX = 2.0f; GLfloat scaleY = 2.0f; GLfloat scaleZ = 2.0f;
GLfloat transX = 0; GLfloat transY = 0;
GLfloat rotatX = 0; GLfloat rotatY = 0;
GLfloat multiMatrix[16];

float arbAngle = 0;
int mouseCoordX = 0;
int mouseCoordY = 0;

void changeSize(int, int);
void renderScene(void);
void drawCoordinates();
void drawCube();
void drawArbitrary(float x, float y);
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void onMouseClick(int button, int state, int x, int y);

void MatrixReset();
void TranslateMatrix(GLfloat X, GLfloat Y, GLfloat Z);
void ScaleMatrix(GLfloat X, GLfloat Y, GLfloat Z);
void RotateMatrix(float angle, GLfloat X, GLfloat Y, GLfloat Z);

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
	glutMouseFunc(onMouseClick);

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
	gluLookAt(-1.0f, 1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
		
	drawCoordinates();
	drawArbitrary(mouseCoordX, mouseCoordY);
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

	// scale
	ScaleMatrix(scaleX, scaleY, scaleZ);

	// rotate
	RotateMatrix(angleX, 1, 0, 0);
	RotateMatrix(angleY, 0, 1, 0);
	RotateMatrix(angleZ, 0, 0, 1);

	if (rotatX != 0 || rotatY != 0) {
		RotateMatrix(arbAngle, rotatX, rotatY, 0);
	}

	// translate
	TranslateMatrix(transX, transY, 0);

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

void drawArbitrary(float x, float y) {

	if (x != 0 && y != 0) {
		float posX = x / 25;
		float posY = y / 25;
		rotatX = posX;
		rotatY = posY;
		//point
		glPointSize(5);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(posX, posY);
		glEnd();
		//line
		glColor3f(0.0, 0.0, 0.0); // black
		glBegin(GL_LINES);
		glVertex3f(posX, posY, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
	}
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
	case 'x': // arbitrary +
		arbAngle += 1.0f;
		glutPostRedisplay();
		break;
	case 'c': // arbitrary -
		arbAngle -= 1.0f;
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

void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseCoordX = x - 250;
		mouseCoordY = 250 - y;
		cout <<"("<< mouseCoordX << ", " << mouseCoordY <<")"<< endl;
	}
}

void MatrixReset() {
	for (int i = 0; i < 16; i++)
	{
		multiMatrix[i] = 0;
	}
	multiMatrix[0] = 1;
	multiMatrix[5] = 1;
	multiMatrix[10] = 1;
	multiMatrix[15] = 1;
}

void TranslateMatrix(GLfloat X, GLfloat Y, GLfloat Z) {
	MatrixReset();
	multiMatrix[12] = X;
	multiMatrix[13] = Y;
	multiMatrix[14] = Z;
	glMultMatrixf(multiMatrix);
}

void ScaleMatrix(GLfloat X, GLfloat Y, GLfloat Z) {
	MatrixReset();
	multiMatrix[0] = X;
	multiMatrix[5] = Y;
	multiMatrix[10] = Z;
	glMultMatrixf(multiMatrix);
}

void RotateMatrix(float angle, GLfloat X, GLfloat Y, GLfloat Z) {
	MatrixReset();
	GLfloat Cos = cos(angle * PI / 180);
	GLfloat Sin = sin(angle * PI / 180);

	double magnitude = sqrt(X*X + Y * Y + Z * Z);
	X /= magnitude;
	Y /= magnitude;
	Z /= magnitude;

	multiMatrix[0] = Cos + ((1 - Cos) * X * X);
	multiMatrix[1] = ((1 - Cos) * Y * X) + (Sin * Z);
	multiMatrix[2] = ((1 - Cos) * Z * X) - (Sin * Y);
	multiMatrix[4] = ((1 - Cos) * X * Y) - (Sin * Z);
	multiMatrix[5] = Cos + ((1 - Cos) * Y * Y);
	multiMatrix[6] = ((1 - Cos) * Z * Y) + (Sin * X);
	multiMatrix[8] = ((1 - Cos) * X * Z) + (Sin * Y);
	multiMatrix[9] = ((1 - Cos) * Y * Z) - (Sin * X);
	multiMatrix[10] = Cos + ((1 - Cos) * Z * Z);
	glMultMatrixf(multiMatrix);
}

