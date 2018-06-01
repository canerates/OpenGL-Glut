#include <glut.h>
#include <math.h>
#include <iostream>
using namespace std;

float winWidth = 700;
float winHeight = 700;
float gridCount = 30; // default
float m1x, m2x, m1y, m2y;
bool isMouseClicked;
bool isFirst = false;

void changeSize(int, int);
void renderScene(void);
void init(void);

void drawGrid();
void midPoint(float x1, float x2, float y1, float y2);
void drawSquare(float x, float y);
void printSquare(float x, float y);
void drawClickedPoints();

void onMouseClick(int button, int state, int x, int y);

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Mid-point Algorithm");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMouseFunc(onMouseClick);

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
	drawClickedPoints();
	midPoint(m1x, m2x, m1y, m2y);

	glutSwapBuffers();
}

void init(void) 
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void midPoint(float x1, float x2, float y1, float y2)
{
	if (isMouseClicked && !isFirst)
	{
		int dx, dy, i, e;
		int incx, incy, inc1, inc2;
		int x, y;

		dx = x2 - x1;
		dy = y2 - y1;

		if (dx < 0) dx = -dx;
		if (dy < 0) dy = -dy;
		incx = 1;
		if (x2 < x1) incx = -1;
		incy = 1;
		if (y2 < y1) incy = -1;
		x = x1; y = y1;
		if (dx > dy) {
			drawSquare(x, y);
			printSquare(x, y);
			e = 2 * dy - dx;
			inc1 = 2 * (dy - dx);
			inc2 = 2 * dy;
			for (i = 0; i<dx; i++) {
				if (e >= 0) {
					y += incy;
					e += inc1;
				}
				else
					e += inc2;
				x += incx;
				drawSquare(x, y);
				printSquare(x, y);
			}

		}
		else {
			drawSquare(x, y);
			printSquare(x, y);
			e = 2 * dx - dy;
			inc1 = 2 * (dx - dy);
			inc2 = 2 * dx;
			for (i = 0; i<dy; i++) {
				if (e >= 0) {
					x += incx;
					e += inc1;
				}
				else
					e += inc2;
				y += incy;
				drawSquare(x, y);
				printSquare(x, y);
			}
		}
	}
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

void drawSquare(float x, float y)
{
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f((x / gridCount), (y / gridCount));
	glVertex2f((x / gridCount), ((y + 1) / gridCount));
	glVertex2f(((x + 1) / gridCount), ((y + 1) / gridCount));
	glVertex2f(((x + 1) / gridCount), (y / gridCount));
	glEnd();
}

void printSquare(float x, float y)
{
	cout << "(" << x << ", " << y << ")" << endl;
}

void drawClickedPoints()
{
	if (isMouseClicked)
	{
		drawSquare(m1x, m1y);

		if (!isFirst)
			drawSquare(m2x, m2y);
	}
}

void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (!isFirst)
		{
			m1x = floor((x / winWidth) * gridCount);
			m1y = floor(gridCount - ((y / winHeight) * gridCount));
			cout << "Start point: (" << m1x << ", " << m1y << ")" << endl;
			isFirst = true;
		}
		else
		{
			m2x = floor((x / winWidth) * gridCount);
			m2y = floor(gridCount - ((y / winHeight) * gridCount));
			cout << "End point: (" << m2x << ", " << m2y << ")" << endl;
			isFirst = false;
		}
		isMouseClicked = true;
	}
}
