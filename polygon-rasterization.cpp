#include <glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;

struct Color {
	float R;
	float G;
	float B;
};

struct Vertex {
	float mx;
	float my;
	Color vColor;
};

float winWidth = 700;
float winHeight = 700;
float gridCount = 50; // default
bool isPolygon = false;
bool isExist = false;

int mainMenu, sizeMenu;

vector<Vertex> vList;

void changeSize(int, int);
void renderScene(void);
void init(void);

void drawGrid();
void drawSquare(float vx, float vy, Color color);
void printSquare(float x, float y);
void drawClickedPoints();
void rasterLine();
void rasterPolygon();
float randomColorGenerator();
vector<Color> differenceRGB(Color c1, Color c2, int max);
void resetRendering();

void crow(vector<Vertex> vList, int n);
void scanY(vector<Vertex> vList, int n, int i);
void differenceY(Vertex *v1, Vertex *v2, Vertex *e, Vertex *de, int y);
void difference(Vertex *v1, Vertex *v2, Vertex *e, Vertex *de, float d, float f);
void differenceX(Vertex *v1, Vertex *v2, Vertex *e, Vertex *de, int x);
void increment(Vertex *edge, Vertex *delta);
void scanX(Vertex *l, Vertex *r, int y);

void onMouseClick(int button, int state, int x, int y);

void processMainMenu(int option);
void processSizeMenu(int option);
void createPopupMenus();

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Polygon Rasterization");
	init();

	srand(time(NULL));

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMouseFunc(onMouseClick);

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
	drawClickedPoints();
	rasterLine();
	rasterPolygon();
	glutSwapBuffers();
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void midPoint(Vertex v1, Vertex v2)
{
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	int x1 = v1.mx;
	int y1 = v1.my;

	int x2 = v2.mx;
	int y2 = v2.my;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1) incx = -1;
	incy = 1;
	if (y2 < y1) incy = -1;
	x = x1; y = y1;

	int maxD = max(dy, dx);

	vector<Color> rgbVec = differenceRGB(v1.vColor, v2.vColor, maxD);

	if (dx > dy) {
		drawSquare(x, y, rgbVec[0]);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++) {
			if (e >= 0) {
				y += incy;
				e += inc1;
			}
			else
				e += inc2;
			x += incx;

			drawSquare(x, y, rgbVec[i]);
		}
	}
	else {
		drawSquare(x, y, rgbVec[0]);
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++) {
			if (e >= 0) {
				x += incx;
				e += inc1;
			}
			else
				e += inc2;
			y += incy;

			drawSquare(x, y, rgbVec[i]);
		}
	}
}

void drawGrid() {

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);

	for (int i = 1; i < gridCount; i++) {
		glVertex2f(0.0, i / gridCount);
		glVertex2f(1.0, i / gridCount);
		glVertex2f(i / gridCount, 0.0);
		glVertex2f(i / gridCount, 1.0);
	}
	glEnd();
}

void drawSquare(float vx, float vy, Color color)
{
	float x = floor(vx);
	float y = floor(vy);
	glBegin(GL_QUADS);
	glColor3f(color.R, color.G, color.B);
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
	if (vList.size() != 0 && !(vList.size() > 1))
	{
		drawSquare(vList[0].mx, vList[0].my, vList[0].vColor);
	}
}

void rasterLine()
{
	if (vList.size() > 1)
	{
		for (int i = 0; i < vList.size() - 1; i++)
		{
			midPoint(vList[i], vList[i + 1]);
		}
	}
}

void rasterPolygon()
{
	if (isPolygon)
	{
		crow(vList, vList.size());
	}
}

void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (isPolygon)
		{
			resetRendering();
		}

		float locx = (x / winWidth) * gridCount;
		float locy = gridCount - ((y / winHeight) * gridCount);

		float lx = floor(locx);
		float ly = floor(locy);

		for (int i = 0; i < vList.size(); i++)
		{
			if (lx == floor(vList[i].mx) && ly == floor(vList[i].my))
			{
				isExist = true;
				break;
			}
			else isExist = false;
		}

		if (vList.size() > 1 && lx == floor(vList[0].mx) && ly == floor(vList[0].my))
		{
			isPolygon = true;
			Vertex v;
			v.mx = locx;
			v.my = locy;
			v.vColor.R = vList[0].vColor.R;
			v.vColor.G = vList[0].vColor.G;
			v.vColor.B = vList[0].vColor.B;

			vList.push_back(v);
		}
		else if (!isExist)
		{
			Vertex v;
			v.mx = locx;
			v.my = locy;
			v.vColor.R = randomColorGenerator();
			v.vColor.G = randomColorGenerator();
			v.vColor.B = randomColorGenerator();

			vList.push_back(v);
		}
	}
}

float randomColorGenerator()
{
	float colorIndex = (rand() % 256);
	return colorIndex / 256;
}

vector<Color> differenceRGB(Color c1, Color c2, int max) {

	vector<Color> rgbVec;

	float diffR = fabs(c1.R - c2.R) / max;
	float diffG = fabs(c1.G - c2.G) / max;
	float diffB = fabs(c1.B - c2.B) / max;

	Color color;
	for (int i = 1; i <= max; i++)
	{
		if (c1.R > c2.R) color.R = c1.R - (i * diffR);
		else color.R = c1.R + (i * diffR);

		if (c1.G > c2.G) color.G = c1.G - (i * diffG);
		else color.G = c1.G + (i * diffG);

		if (c1.B > c2.B) color.B = c1.B - (i * diffB);
		else color.B = c1.B + (i * diffB);

		rgbVec.push_back(color);
	}
	return rgbVec;
}

void processMainMenu(int option) {

	switch (option) {
	case 1: //reset
		resetRendering();
		glutPostRedisplay();
		break;
	}
}

void processSizeMenu(int option) {
	switch (option) {
	case 1: //10x10
		gridCount = 10;
		resetRendering();
		glutPostRedisplay();
		break;
	case 2: //20x20
		gridCount = 20;
		resetRendering();
		glutPostRedisplay();
		break;
	case 3: //30x30
		gridCount = 30;
		resetRendering();
		glutPostRedisplay();
		break;
	case 4: //50x50
		gridCount = 50;
		resetRendering();
		glutPostRedisplay();
		break;
	case 5: //100x100
		gridCount = 100;
		resetRendering();
		glutPostRedisplay();
		break;
	}
}

void createPopupMenus() {

	sizeMenu = glutCreateMenu(processSizeMenu);
	glutAddMenuEntry("10", 1);
	glutAddMenuEntry("20", 2);
	glutAddMenuEntry("30", 3);
	glutAddMenuEntry("50", 4);
	glutAddMenuEntry("100", 5);

	mainMenu = glutCreateMenu(processMainMenu);

	glutAddSubMenu("Size", sizeMenu);
	glutAddMenuEntry("Reset", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void resetRendering()
{
	vList.clear();
	isPolygon = false;
	isExist = false;
}

void crow(vector<Vertex> vList, int n)
{
	int iMin = 0;
	for (int i = 1; i < n; i++)
	{
		if (vList[i].my < vList[iMin].my)
			iMin = i;
	}
	scanY(vList, n, iMin);
}

void scanY(vector<Vertex> vList, int n, int i)
{
	int li, ri; // left & right upper endpoint indices
	int ly, ry; // left & right upper endpoint y values
	Vertex l, dl; // current left edge and delta
	Vertex r, dr; // current right edge and delta
	int rem; // number of remaining vertices
	int y; // current scanline

	li = ri = i;
	ly = ry = y = vList[i].my;

	for (rem = n; rem > 0;)
	{
		while (ly <= y && rem > 0)
		{
			rem--;
			i = li - 1;
			if (i < 0)
				i = n - 1;
			ly = vList[i].my;
			if (ly > y)
				differenceY(&vList[li], &vList[i], &l, &dl, y);
			li = i;
		}

		while (ry <= y && rem > 0)
		{
			rem--;
			i = ri + 1;
			if (i >= n)
				i = 0;
			ry = vList[i].my;
			if (ry > y)
				differenceY(&vList[ri], &vList[i], &r, &dr, y);
			ri = i;
		}

		for (; y < ly && y < ry; y++)
		{
			scanX(&l, &r, y);
			increment(&l, &dl);
			increment(&r, &dr);
		}
	}
}

void differenceY(Vertex *v1, Vertex *v2, Vertex *e, Vertex *de, int y)
{
	difference(v1, v2, e, de, (v2->my - v1->my), y - v1->my);
}

void difference(Vertex *v1, Vertex *v2, Vertex *e, Vertex *de, float d, float f)
{
	de->mx = (v2->mx - v1->mx) / d;
	e->mx = v1->mx + f * de->mx;
	de->vColor.R = (v2->vColor.R - v1->vColor.R) / d;
	e->vColor.R = v1->vColor.R + f * de->vColor.R;
	de->vColor.G = (v2->vColor.G - v1->vColor.G) / d;
	e->vColor.G = v1->vColor.G + f * de->vColor.G;
	de->vColor.B = (v2->vColor.B - v1->vColor.B) / d;
	e->vColor.B = v1->vColor.B + f * de->vColor.B;
}

void differenceX(Vertex *v1, Vertex *v2, Vertex *e, Vertex *de, int x)
{
	difference(v1, v2, e, de, (v2->mx - v1->mx), x - v1->mx);
}

void increment(Vertex *edge, Vertex *delta)
{
	edge->mx += delta->mx;
	edge->vColor.R += delta->vColor.R;
	edge->vColor.G += delta->vColor.G;
	edge->vColor.B += delta->vColor.B;
}

void scanX(Vertex *l, Vertex *r, int y)
{
	int x, lx, rx;
	Vertex s, ds;

	lx = l->mx;
	rx = r->mx;
	if (lx < rx)
	{
		differenceX(l, r, &s, &ds, lx);
		for (x = lx; x < rx; x++)
		{
			drawSquare(x, y, s.vColor);
			increment(&s, &ds);
		}
	}
}