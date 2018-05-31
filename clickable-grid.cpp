#include <glut.h>
#include <math.h>
#include <iostream>
using namespace std;

float winWidth = 700;
float winHeight = 700;
float gridCount = 10; // default
float mx;
float my;
bool isMouseClicked;

int gridMenu;

void changeSize(int, int);
void renderScene(void);
void init(void);
void drawGrid();
void drawSquare();

void processMainMenu(int option);
void processGridMenu(int option);
void createPopupMenus();

void onMouseClick(int button, int state, int x, int y);

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("2D Grid");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMouseFunc(onMouseClick);

	if (argc == 2)
	{
		gridCount = atof(argv[1]);
	}
	else if (argc != 1)
		cout << "Usage: " << argv[0] << " GRID COUNT\n";

	createPopupMenus();

	glutMainLoop();

	return 1;
}
void changeSize(int w, int h)
{
	winWidth = w;
	winHeight = h;

	// use the projection matrix
	glMatrixMode(GL_PROJECTION);

	// reset matrix
	glLoadIdentity();

	// set the viewport to be the entire window
	glViewport(0, 0, w, h);

	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	// get back to the modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	// clear color
	glClear(GL_COLOR_BUFFER_BIT);

	drawGrid();
	drawSquare();
	
	glutSwapBuffers();
}

void init(void) 
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void drawGrid() {

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);

	for (int i = 1; i<gridCount; i++) {
		glVertex2f(0.0, i / gridCount);
		glVertex2f(1.0, i / gridCount);
		glVertex2f(i / gridCount, 0.0);
		glVertex2f(i / gridCount, 1.0);
	}
	glEnd();
}

void drawSquare()
{
	if (isMouseClicked)
	{
		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);

		glVertex2f((mx / gridCount), (my / gridCount));
		glVertex2f((mx / gridCount), ((my + 1) / gridCount));
		glVertex2f(((mx + 1) / gridCount), ((my + 1) / gridCount));
		glVertex2f(((mx + 1) / gridCount), (my / gridCount));

		glEnd();
	}
}

void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mx = floor((x / winWidth) * gridCount);
		my = floor(gridCount - ((y / winHeight) * gridCount));
		cout << "(" << mx << ", " << my << ")" << endl;
		isMouseClicked = true;
	}
}

void processMainMenu(int option) {

	// nothing to do in here
	// all actions are for submenus
}

void processGridMenu(int option) {
	switch (option) {
	case 1: //10x10
		gridCount = 10;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	case 2: //20x20
		gridCount = 20;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	case 3: //30x30
		gridCount = 30;
		isMouseClicked = false;
		glutPostRedisplay();
		break;

	case 4: //40x40
		gridCount = 40;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	case 5: //50x50
		gridCount = 50;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	}
}


void createPopupMenus() {

	gridMenu = glutCreateMenu(processGridMenu);
	glutAddMenuEntry("10x10", 1);
	glutAddMenuEntry("20x20", 2);
	glutAddMenuEntry("30x30", 3);
	glutAddMenuEntry("40x40", 4);
	glutAddMenuEntry("50x50", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}