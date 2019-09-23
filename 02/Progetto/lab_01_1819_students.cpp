/*
 * Lab-01-1819_students.c
 *
 *     This program draws straight lines connecting dots placed with mouse clicks.
 *
 * Usage:  
 *   Left click to place a control point.  
 *		Maximum number of control points allowed is currently set at 64.
 *	 Press "f" to remove the first control point
 *	 Press "l" to remove the last control point.
 *	 Press escape to exit.
 */

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#define MaxNumPts 64
float PointArray[MaxNumPts][3];
int NumPts = 0;
int mouseOverIndex = -1;

//Settings
int switchMode = 0; //OpenGL - deCasteljau - adaptive Subdivision
float t = 100;
float tol = 0.2;

bool isMovingPoint = false;
int movingPoint;

// Window size in pixels
int WindowHeight;
int WindowWidth;

/* Prototypes */ 
void addNewPoint(float x, float y);
void removeFirstPoint();
void removeLastPoint();


void myKeyboardFunc (unsigned char key, int x, int y)
{
	switch (key) {
	case 'f':
		removeFirstPoint();
		glutPostRedisplay();
		break;
	case 'l':
		removeLastPoint();
		glutPostRedisplay();
		break;
	case '1':
		printf("OPENGL\n");
		switchMode = 0;
		glutPostRedisplay();
		break;
	case '2':
		printf("deCasteljau\n");
		switchMode = 1;
		glutPostRedisplay();
		break;
	case '3':
		printf("adaptive\n");
		switchMode = 2;
		glutPostRedisplay();
		break;
	case '+':
		if (switchMode == 2) {
			if (tol > 0.03f) {
				tol -= 0.02;
			}
			else if (tol > 0.003f) {
				tol -= 0.002;
			}
			else if (tol > 0.0003f) {
				tol -= 0.0002;
			}
			else if (tol > 0.00003f) {
				tol -= 0.00002;
			}
			else if (tol > 0.00003f) {
				tol -= 0.00002;
			}
			else if (tol > 0.000003f) {
				tol -= 0.000002;
			}
			else if (tol > 0.0000003f) {
				tol -= 0.0000002;
			}
		}
		else {
			t += 5;
		}
		glutPostRedisplay();
		break;
	case '-':
		if (switchMode == 2) {
			if (tol < 0.000003) {
				tol += 0.0000002;
			}
			else if (tol < 0.00003) {
				tol += 0.000002;
			}
			else if (tol < 0.0003) {
				tol += 0.00002;
			}
			else if (tol < 0.003) {
				tol += 0.0002;
			}
			else if (tol < 0.003) {
				tol += 0.0002;
			}
			else if (tol < 0.03) {
				tol += 0.002;
			}
			else {
				tol += 0.02;
			}
		}
		else {
			t = (int)std::fmax(0, t-5);
		}
		glutPostRedisplay();
		break;
	case 27:			// Escape key
		exit(0);
		break;
	}
}

void removeFirstPoint() {
	int i;
	if ( NumPts>0 ) {
		// Remove the first point, slide the rest down
		NumPts--;
		for ( i=0; i<NumPts; i++ ) {
			PointArray[i][0] = PointArray[i+1][0];
			PointArray[i][1] = PointArray[i+1][1];
			PointArray[i][2] = 0;
		}
	}
}

// Left button presses place a control point.
void myMouseFunc( int button, int state, int x, int y ) {
	if ( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN ) 
	{
		float xPos = ((float)x)/((float)(WindowWidth-1));
		float yPos = ((float)y) / ((float)(WindowHeight - 1));

		if (mouseOverIndex != -1) {
			isMovingPoint = true;
			movingPoint = mouseOverIndex;

			return;
		}

		isMovingPoint = false;
		printf("PROVA");

		yPos = 1.0f - yPos;			// Flip value since y position is from top row.

		addNewPoint( xPos, yPos );
		glutPostRedisplay();
	}
}

void passiveMouse(int x, int y) {

	float xPos = ((float)x) / ((float)(WindowWidth - 1));
	float yPos = ((float)(WindowHeight - 1) - y) / ((float)(WindowHeight - 1));

	for (int i = 0; i < NumPts; i++) {
		float dist = sqrt(pow(PointArray[i][0] - xPos, 2) + pow(PointArray[i][1] - yPos, 2));

		if (dist < 0.02) {
			mouseOverIndex = i;
			glutPostRedisplay();
			return;
		}
		else {
			mouseOverIndex = -1;
			glutPostRedisplay();
		}
	}
}

void motionMouse(int x, int y) {
	float xPos = ((float)x) / ((float)(WindowWidth - 1));
	float yPos = ((float)(WindowHeight - 1) - y) / ((float)(WindowHeight - 1));

	if (isMovingPoint) {
		PointArray[movingPoint][0] = xPos;
		PointArray[movingPoint][1] = yPos;
	}

	glutPostRedisplay();
}

// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void removeLastPoint() {
	if ( NumPts>0 ) {
		NumPts--;
	}
}

// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void addNewPoint( float x, float y ) {
	if ( NumPts>=MaxNumPts ) {
		removeFirstPoint();
	}
	PointArray[NumPts][0] = x;
	PointArray[NumPts][1] = y;
	PointArray[NumPts][2] = 0;
	NumPts++;
	
}

void deCasteljau(float t) {

	float tempPoint[MaxNumPts][3];
	
	for (int i = 0; i < NumPts; i++) {
		tempPoint[i][0] = PointArray[i][0];
		tempPoint[i][1] = PointArray[i][1];
		tempPoint[i][2] = PointArray[i][2];
	}

	for (int i = 1; i < NumPts; i++) {
		for (int j = 0; j < NumPts - i; j++) {
			tempPoint[j][0] = ((1 - t) * tempPoint[j][0]) + ((t) * tempPoint[j + 1][0]);
			tempPoint[j][1] = ((1 - t) * tempPoint[j][1]) + ((t) * tempPoint[j + 1][1]);
			tempPoint[j][2] = ((1 - t) * tempPoint[j][2]) + ((t) * tempPoint[j + 1][2]);
		}
	}
	glVertex3f(tempPoint[0][0], tempPoint[0][1], tempPoint[0][2]);
}

float pointDistance(std::vector<float> firstPoint, std::vector<float> controlPoint, std::vector<float> lastPoint) {
	float distanceFirstLast = sqrtf(powf(firstPoint[0] - lastPoint[0], 2) + powf(firstPoint[1] - lastPoint[1], 2));
	
	//Formula di nerone
	return fabsf(
		((lastPoint[0] - firstPoint[0]) * (firstPoint[1] - controlPoint[1])) // (x2 - x1) * (y1 - y0)
		- ((firstPoint[0] - controlPoint[0]) * (lastPoint[1] - firstPoint[1])) // - (x1 - x0) * (p2 - p1)
	) / distanceFirstLast;
}

void adaptiveSubdivision(std::vector<std::vector<float>> points, float tol) {
	bool drawLine = true;

	for (int i = 1; i < NumPts - 1; i++) {
		if (pointDistance(points[i], points[0], points[NumPts - 1]) > tol) {
			drawLine = false;
		}
	}

	if (drawLine) {
		glVertex3f(points[0][0], points[0][1], points[0][2]);
		glVertex3f(points[NumPts-1][0], points[NumPts-1][1], points[NumPts-1][2]);
	}
	else {
		std::vector<std::vector<float>> curve1(NumPts, std::vector<float>(3));
		std::vector<std::vector<float>> curve2(NumPts, std::vector<float>(3));

		curve1[0][0] = points[0][0];
		curve1[0][1] = points[0][1];
		curve1[0][2] = points[0][2];

		curve2[NumPts-1][0] = points[NumPts-1][0];
		curve2[NumPts-1][1] = points[NumPts-1][1];
		curve2[NumPts-1][2] = points[NumPts-1][2];
		
		for (int i = 1; i < NumPts; i++) {
			for (int j = 0; j < NumPts - i; j++) {
				points[j][0] = ((1 - .5f) * points[j][0]) + ((.5f)* points[j + 1][0]);
				points[j][1] = ((1 - .5f) * points[j][1]) + ((.5f)* points[j + 1][1]);
				points[j][2] = ((1 - .5f) * points[j][2]) + ((.5f)* points[j + 1][2]);
			}
			
			curve1[i][0] = points[0][0];
			curve1[i][1] = points[0][1];
			curve1[i][2] = points[0][2];

			curve2[NumPts - i - 1][0] = points[NumPts - i - 1][0];
			curve2[NumPts - i - 1][1] = points[NumPts - i - 1][1];
			curve2[NumPts - i - 1][2] = points[NumPts - i - 1][2];
		}

		adaptiveSubdivision(curve1, tol);
		adaptiveSubdivision(curve2, tol);
	
	}
}

void evaluateBezier() {
	if (NumPts>1) {
		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, NumPts, &PointArray[0][0]);

		glColor3f(0.0f, 0.5f, 0.8f);			// Reddish/purple lines
		
		glBegin(GL_LINE_STRIP);

		switch (switchMode) {
		case(0):
		case(1):
			//For di valutazione della curva
			//100 sono i punti di valutazione e non di controllo
			for (int i = 0; i < t; i++) {
				if (switchMode == 0) {
					//Valutazione di openGL
					glEvalCoord1f((GLfloat)i / t);
				}
				else {
					//Valutazione tramite deCasteljau
					deCasteljau((float) i/t);
				}
			}
			break;
		case(2):
			std::vector<std::vector<float>> suppPoint(NumPts, std::vector<float>(3));
			for (int i = 0; i < NumPts; i++) {
				suppPoint[i][0] = PointArray[i][0];
				suppPoint[i][1] = PointArray[i][1];
				suppPoint[i][2] = PointArray[i][2];
			}

			adaptiveSubdivision(suppPoint, tol);
			break;
		}
		glEnd();
	}
}

void display(void)
{
	int i;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(3);
	
	// Draw the line segments
	
	if ( NumPts>1 ) {
		glColor3f(0.0f, 0.5f, 0.8f);			// Reddish/purple lines
		glBegin( GL_LINE_STRIP );
		for ( i=0; i<NumPts; i++ ) {
			//Disegno con la glVertex i vari segmenti tra i punti di controllo
			glVertex2f( PointArray[i][0], PointArray[i][1] );
		}
		glEnd();
	}

	// Draw the interpolated points second.
	glColor3f( 0.0f, 0.0f, 0.0f);			// Draw points in black
	glBegin( GL_POINTS );

	for ( i=0; i<NumPts; i++ ) {
		//Disegno i vari punti di controllo
		glVertex2f( PointArray[i][0], PointArray[i][1] );
	}
	glEnd();

	evaluateBezier();

	if (mouseOverIndex != -1) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(PointArray[mouseOverIndex][0], PointArray[mouseOverIndex][1]);
		glEnd();
	}

	//Evade il comando per ottenere il risultato
	glFlush();
}

void initRendering() {
	glEnable(GL_MAP1_VERTEX_3);
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

	// Make big points and wide lines.  (This may be commented out if desired.)
	glPointSize(8);
	

	// The following commands should induce OpenGL to create round points and 
	//	antialias points and lines.  (This is implementation dependent unfortunately, and
	//  may slow down rendering considerably.)
	//  You may comment these out if you wish.
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void resizeWindow(int w, int h)
{
	WindowHeight = (h>1) ? h : 2;
	WindowWidth = (w>1) ? w : 2;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);  // Always view [0,1]x[0,1].
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB ); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	
	initRendering();

	glutDisplayFunc(display);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(myKeyboardFunc);
	glutMouseFunc(myMouseFunc);
	glutPassiveMotionFunc(passiveMouse);
	glutMotionFunc(motionMouse);
	
	glutMainLoop();

	return 0;					// This line is never reached
}
