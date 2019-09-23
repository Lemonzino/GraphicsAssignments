// Lab-00.cpp
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>


enum AppMode
{
	MODE_SIMPLEDRAW,
	MODE_SIMPLEANIM,
	MODE_CUBEVIEW,
};

enum AppMode appMode = MODE_SIMPLEDRAW;

int runMode = 1;		// Used as a boolean (1 or 0) for turning "on" and "off" the animation in MODE_SIMPLEANIM

// Controls the animation's state and speed in MODE_SIMPLEANIM
float currentAngle = 0.0f;			// Angle in degrees
float animateStep = 0.5f;			// Rotation step per update

// These variables control the current draw mode in MODE_SIMPLEDRAW
int drawMode = 0;
const int numDrawModes = 5;
int 	listname;

// Vertices, normals and colors for the cube in MODE_CUBEVIEW
//Immediate mode
GLfloat vertices[][3] = { {-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0},
{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0} };

GLfloat colors[][3] = { {0.0,0.0,0.0},{1.0,0.0,0.0},
{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0},
{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0} };

// DRAW ELEMENTS mode
// We need an array containing indices referring to vertices, normals and colors in their arrays
GLuint indexes[] = {
	0, 3, 2, 1, // face #1
	2, 3, 7, 6, // face #2
	0, 4, 7, 3, // face #3
	1, 2, 6, 5, // face #4
	4, 5, 6, 7, // face #5
	0, 1, 5, 4  // face #6
};
//Opengl pointers to the data structures for glDrawElements
GLuint vao_element_mod_ID, vbo_element_mod_ID, cbo_element_mod_ID, ibo_element_mod_ID;

//vertex array mode GLOBALS:
/* Arrays for use with glDrawArrays.  The coordinate array contains four sets of vertex
 * coordinates for each face.  The color array must have a color for each vertex.  Since
 * the color of each face is solid, there is a lot of redundancy in the color array.
 * There is also redundancy in the coordinate array, compared to using glDrawElements.
 * But note that it is impossible to use a single call to glDrawElements to draw a cube 
 * with six faces where each face has a different solid color, since with glDrawElements, 
 * the colors are associated with the vertices, not the faces.
 */
 //Opengl pointers to the data structures for glDrawArrays
GLuint vao_array_mod_ID, vbo_array_mod_ID, cbo_array_mod_ID; 
GLfloat cubeCoords[72] = {
       1,1,1,    -1,1,1,   -1,-1,1,   1,-1,1,      // face #1
       1,1,1,     1,-1,1,   1,-1,-1,  1,1,-1,      // face #2
       1,1,1,     1,1,-1,  -1,1,-1,  -1,1,1,       // face #3
       -1,-1,-1, -1,1,-1,   1,1,-1,   1,-1,-1,     // face #4
       -1,-1,-1, -1,-1,1,  -1,1,1,   -1,1,-1,      // face #5
       -1,-1,-1,  1,-1,-1,  1,-1,1,   -1,-1,1  };  // face #6

GLfloat cubeFaceColors[72] = {
        1,0,0,  1,0,0,  1,0,0,  1,0,0,      // face #1 is red
        0,1,0,  0,1,0,  0,1,0,  0,1,0,      // face #2 is green
        0,0,1,  0,0,1,  0,0,1,  0,0,1,      // face #3 is blue
        1,1,0,  1,1,0,  1,1,0,  1,1,0,      // face #4 is yellow
        0,1,1,  0,1,1,  0,1,1,  0,1,1,      // face #5 is cyan
        1,0,1,  1,0,1,  1,0,1,  1,0,1,   }; // face #6 is red


// View parameters for MODE_CUBEVIEW
float theta[] = {0.0,0.0,0.0};
int axis = 2;
double viewer[]= {0.0, 0.0, 5.0}; /* initial viewer location */

void keyboard( unsigned char key, int x, int y )
{
	switch ( key ) 
	{
	case '1':
		appMode = MODE_SIMPLEDRAW;
	break;
	case '2':
		appMode = MODE_SIMPLEANIM;
	break;
	case '3':
		appMode = MODE_CUBEVIEW;
	break;
	case 27:	// Escape key
		exit(1);
	break;
	case ' ': // Space bar
		drawMode = (drawMode+1) % numDrawModes;
	break;
	case 'r':
		runMode = 1-runMode;
	break;
	break;
	}

	/* Use x, X, y, Y, z, and Z keys to move viewer in MODE_CUBEVIEW */

	if(key == 'x') viewer[0]-= 1.0;
	if(key == 'X') viewer[0]+= 1.0;
	if(key == 'y') viewer[1]-= 1.0;
	if(key == 'Y') viewer[1]+= 1.0;
	if(key == 'z') viewer[2]-= 1.0;
	if(key == 'Z') viewer[2]+= 1.0;


	glutPostRedisplay();
}

void special( int key, int x, int y )
{
	switch ( key ) 
	{
	case GLUT_KEY_UP:		
		if ( animateStep < 1.0e3) 			// Avoid overflow problems
			animateStep *= sqrt(2.0);		// Increase the angle increment
	break;
	case GLUT_KEY_DOWN:
		if (animateStep>1.0e-6) 		// Avoid underflow problems.
			animateStep /= sqrt(2.0);	// Decrease the angle increment
	break;
	}
}

void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;

	theta[axis] += 10.0;

	if( theta[axis] > 360.0 ) theta[axis] -= 360.0;

	glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////
// draw 1: immediate mode
// 72 calls = 6x6 glVertex*() calls + 6x6 glColor*() calls 
void polygon(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);
	glColor3fv(colors[b]);
	glVertex3fv(vertices[b]);
	glColor3fv(colors[c]);
	glVertex3fv(vertices[c]);
	glColor3fv(colors[d]);
	glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube()
{
	polygon(0,3,2,1);
	polygon(2,3,7,6);
	polygon(0,4,7,3);
	polygon(1,2,6,5);
	polygon(4,5,6,7);
	polygon(0,1,5,4);
}
///////////////////////////////////////////////////////////////////////////////
// draw 2: vertex array direct (no VBO)
void colorcube_vertex_array()
{
	// SetUp Vertex Array
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	//CON ARRAYS
	//glVertexPointer(3, GL_FLOAT, 0, cubeCoords);
	//glColorPointer(3, GL_FLOAT, 0, cubeFaceColors);
	//glDrawArrays(GL_QUADS, 0, 24);
	

	//CON ELEMENTS
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indexes);

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
}
///////////////////////////////////////////////////////////////////////////////
// draw 3: vertex array with VBO
void colorcube_ARRAY_MODE() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glBindVertexArray(vao_array_mod_ID);
	glDrawArrays(
		GL_QUADS,									// drawing mode	
		0,											//starting index in the array
		sizeof(cubeCoords) / (sizeof(GLfloat) * 3)	// number of indices to be rendered in the array
	);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}
///////////////////////////////////////////////////////////////////////////////
// draw 3: Element array with VBO
void colorcube_ELEMENTS_MODE() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glBindVertexArray(vao_element_mod_ID);
	glDrawElements(
		GL_QUADS,									// drawing mode	
		sizeof(indexes) / sizeof(GLuint),			// number of indices to be rendered in the array
		GL_UNSIGNED_INT,							// type of data					
		0											//starting index in the array
	);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}
///////////////////////////////////////////////////////////////////////////////

void display()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if( appMode != MODE_CUBEVIEW )
	{
		gluOrtho2D(0.0f, 4.0f, 0.0f, 4.0f);
	}
	else
	{
		gluPerspective(45.0, 1.0, 0.1, 100.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	switch(appMode) {
	case MODE_SIMPLEDRAW:
		glColor3f( 0.0, 0.0, 0.0 );

		switch (drawMode)
		{
		case 0:
			// Draw three points
			glBegin(GL_POINTS);
			glVertex2f( 1.0, 1.0 );
			glVertex2f( 2.0, 1.0 );
			glVertex2f( 2.0, 2.0 );
			glEnd();
			break;

		case 1: 
		case 2: 
		case 3: 
			if ( drawMode==1 ) glBegin( GL_LINES ); 			  // Draw lines in GL_LINES mode
			else if ( drawMode==2 ) glBegin( GL_LINE_STRIP );  // Draw lines in GL_LINE_STRIP mode
			else glBegin( GL_LINE_LOOP ); 						  // Draw lines in GL_LINE_LOOP mode

			glVertex2f( 0.5, 1.0 );
			glVertex2f( 2.0, 2.0 );
			glVertex2f( 1.8, 2.6 );
			glVertex2f( 0.7, 2.2 );
			glVertex2f( 1.6, 1.2 );
			glVertex2f( 1.0, 0.5 );
			glEnd();
			break;

		case 4:
			glBegin( GL_TRIANGLES ); // Overlapping triangles
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.3, 1.0, 0.5 );
			glVertex3f( 2.7, 0.85, 0.0 );
			glVertex3f( 2.7, 1.15, 0.0 );

			glColor3f( 0.0, 1.0, 0.0 );
			glVertex3f(2.53, 0.71, 0.5 );
			glVertex3f(1.46, 2.86, 0.0 );
			glVertex3f(1.2, 2.71, 0.0 );

			glColor3f( 0.0, 0.0, 1.0 );
			glVertex3f(1.667, 2.79, 0.5);
			glVertex3f(0.337, 0.786, 0.0);
			glVertex3f(0.597, 0.636, 0.0);
			glEnd();
		}

		glutSwapBuffers();

	break;
	case MODE_SIMPLEANIM:
		if (runMode==1) 
		{   // Calculate animation parameters
			currentAngle += animateStep;
			if ( currentAngle > 360.0 ) 
			{
				currentAngle -= 360.0 * floor( currentAngle / 360.0 );	// Don't allow overflow
			}
		}
		
		// Rotate the image
		glMatrixMode( GL_MODELVIEW );			// Current matrix affects objects positions
		glLoadIdentity();						// Initialize to the identity

		// v' = [T(c)][R][T(-c)] v
		
		glTranslatef( 1.5, 1.5, 0.0 );					// Translate rotation center from origin
		glRotatef( currentAngle, 0.0, 0.0, 1.0 );		// Rotate through animation angle
		glTranslatef( -1.5, -1.5, 0.0 );				// Translate rotation center to origin
		
		// Draw three overlapping triangles of different colors
		glBegin( GL_TRIANGLES );

		glColor3f( 1.0, 0.0, 0.0 );
		glVertex3f( 0.3, 1.0, 0.5 );
		glVertex3f( 2.7, 0.85, 0.0 );
		glVertex3f( 2.7, 1.15, 0.0 );

		glColor3f( 0.0, 1.0, 0.0 );
		glVertex3f(2.53, 0.71, 0.5 );
		glVertex3f(1.46, 2.86, 0.0 );
		glVertex3f(1.2, 2.71, 0.0 );

		glColor3f( 0.0, 0.0, 1.0 );
		glVertex3f(1.667, 2.79, 0.5);
		glVertex3f(0.337, 0.786, 0.0);
		glVertex3f(0.597, 0.636, 0.0);
		glEnd();


		// Flush the pipeline, swap the buffers
		glutSwapBuffers();

		if ( runMode==1 )
			glutPostRedisplay();	// Trigger an automatic redraw for animation
	break;
	case MODE_CUBEVIEW:

		/* Update viewer position in modelview matrix */
		glLoadIdentity();
		gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		/* rotate cube */

		glRotatef(theta[0], 1.0, 0.0, 0.0);
		glRotatef(theta[1], 0.0, 1.0, 0.0);
		glRotatef(theta[2], 0.0, 0.0, 1.0);

		//Immediate mode 
		//colorcube();

		//Vertex Array direct (no VBO)
		//Nota!! se usate questa modalità si devono commentare le chiamate 
		// alle funzioni che generano i VBO in initRendering()
		//colorcube_vertex_array();

		//Draw Arrays mode with VBO
		//colorcube_ARRAY_MODE();

		//Draw Elements mode with VBO
		colorcube_ELEMENTS_MODE();

		glutSwapBuffers();
	break;
	}
}

void build_cube_VAO_for_ARRAY_draw() {
	// Genero 1 Vertex Array Object
	glGenVertexArrays(1, &vao_array_mod_ID);
	glBindVertexArray(vao_array_mod_ID);
	
	// Genero 1 Vertex Buffer Object per i vertici
	glGenBuffers(1, &vbo_array_mod_ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_array_mod_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeCoords), &cubeCoords[0], GL_STATIC_DRAW);
	glVertexPointer(
		3,                  // size
		GL_FLOAT,           // type
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Genero 1 Color Buffer Object per i colori
	glGenBuffers(1, &cbo_array_mod_ID);
	glBindBuffer(GL_ARRAY_BUFFER, cbo_array_mod_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeFaceColors), &cubeFaceColors[0], GL_STATIC_DRAW);
	glColorPointer(
		3,                  // size
		GL_FLOAT,           // type
		0,                  // stride
		(void*)0            // array buffer offset
	);
}

void build_cube_VAO_for_ELEMENTS_draw() {
	// Genero 1 Vertex Array Object
	glGenVertexArrays(1, &vao_element_mod_ID);
	glBindVertexArray(vao_element_mod_ID);
	
	// Genero 1 Vertex Buffer Object per i vertici
	glGenBuffers(1, &vbo_element_mod_ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_element_mod_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glVertexPointer(
		3,                  // size
		GL_FLOAT,           // type
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Genero 1 Color Buffer Object per i colori
	glGenBuffers(1, &cbo_element_mod_ID);
	glBindBuffer(GL_ARRAY_BUFFER, cbo_element_mod_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors[0], GL_STATIC_DRAW);
	glColorPointer(
		3,                  // size
		GL_FLOAT,           // type
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Genero 1 Index Buffer Object per gli indici
	glGenBuffers(1, &ibo_element_mod_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_element_mod_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), &indexes[0], GL_STATIC_DRAW);
}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable( GL_DEPTH_TEST );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );

	// Uncomment out the first block of code below, and then the second block, to see how they affect line and point drawing.
	// The following commands should cause points and line to be drawn larger than a single pixel width.
	
	glPointSize(3);
/*
	glLineWidth(5);
*/

/*
	// The following commands should induce OpenGL to create round points and antialias points and lines.
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/

	
	// Build the VAO & VBOs for glDrawArrays (redundant)
	//build_cube_VAO_for_ARRAY_draw();
	// Build the VAO & VBOs for glDrawElements (optimized)
	build_cube_VAO_for_ELEMENTS_draw();
}


// Called when the window is resized. parameters 'w' and 'h' are the width and height of the window in pixels.
void resizeWindow(int w, int h)
{
	w = h; //force viewport aspect ratio

	// Define the portion of the window used for OpenGL rendering.
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

}

int main(int argc, char **argv)
{
	GLboolean GlewInitResult;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition( 10, 60 );
	glutInitWindowSize(500, 500);
	glutCreateWindow("lab 0");

	GlewInitResult = glewInit();
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);
	
	// Initialize OpenGL as we like it..
    initRendering();

	//glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special); // Handles "special" keyboard keys
	//glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

