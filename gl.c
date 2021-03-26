/*
 * CPSC 424, Fall 2015, Lab 4:  Some objects in 3D.  The arrow keys
 * can be used to rotate the object.  The number keys 1 through 5
 * select the object.  The space bar toggles the use of anaglyph
 * stereo.  Compile this program with:
 *
 *           gcc -o lab4 lab4.c -lGL -lglut
 */

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/freeglut.h>
#include <stdio.h>

#include <math.h>
#define M_2PI 6.28318530718f 
 //-------------------Data for stellated dodecahedron ------------------

double dodecVertices[][3] = {
	{-0.650000, 0.000000, -0.248278},
	{0.401722, 0.401722, 0.401722}, // This array contains the coordinates
	{0.650000, 0.000000, 0.248278}, // for the vertices of the polyhedron
	{0.401722, -0.401722, 0.401722}, // known as a stellated dodecahedron
	{0.000000, -0.248278, 0.650000},
	{0.000000, 0.248278, 0.650000}, // Each row of the 2D array contains
	{0.650000, 0.000000, -0.248278}, // the xyz-coordinates for one of
	{0.401722, 0.401722, -0.401722}, // the vertices.
	{0.248278, 0.650000, 0.000000},
	{-0.248278, 0.650000, 0.000000},
	{-0.401722, 0.401722, -0.401722},
	{0.000000, 0.248278, -0.650000},
	{0.401722, -0.401722, -0.401722},
	{0.248278, -0.650000, 0.000000},
	{-0.248278, -0.650000, 0.000000},
	{-0.650000, 0.000000, 0.248278},
	{-0.401722, 0.401722, 0.401722},
	{-0.401722, -0.401722, -0.401722},
	{0.000000, -0.248278, -0.650000},
	{-0.401722, -0.401722, 0.401722},
	{0.000000, 1.051722, 0.650000},
	{-0.000000, 1.051722, -0.650000},
	{1.051722, 0.650000, -0.000000},
	{1.051722, -0.650000, -0.000000},
	{-0.000000, -1.051722, -0.650000},
	{-0.000000, -1.051722, 0.650000},
	{0.650000, 0.000000, 1.051722},
	{-0.650000, 0.000000, 1.051722},
	{0.650000, -0.000000, -1.051722},
	{-0.650000, 0.000000, -1.051722},
	{-1.051722, 0.650000, -0.000000},
	{-1.051722, -0.650000, 0.000000}
};

int dodecTriangles[][3] = {
	{16, 9, 20},
	{9, 8, 20},
	{8, 1, 20}, // This array specifies the faces of
	{1, 5, 20}, // the stellated dodecahedron.
	{5, 16, 20},
	{9, 10, 21}, // Each row in the 2D array is a list
	{10, 11, 21}, // of three integers.  The integers
	{11, 7, 21}, // are indices into the vertex array,
	{7, 8, 21}, // dodecVertices.  The vertices at
	{8, 9, 21}, // at those indices are the vertices
	{8, 7, 22}, // of one of the triangular faces of
	{7, 6, 22}, // the polyhedron.
	{6, 2, 22},
	{2, 1, 22}, // For example, the first row, {16,9,20},
	{1, 8, 22}, // means that vertices number 16, 9, and
	{6, 12, 23}, // 20 are the vertices of a face.
	{12, 13, 23},
	{13, 3, 23}, // There are 60 faces.
	{3, 2, 23},
	{2, 6, 23},
	{18, 17, 24},
	{17, 14, 24},
	{14, 13, 24},
	{13, 12, 24},
	{12, 18, 24},
	{14, 19, 25},
	{19, 4, 25},
	{4, 3, 25},
	{3, 13, 25},
	{13, 14, 25},
	{4, 5, 26},
	{5, 1, 26},
	{1, 2, 26},
	{2, 3, 26},
	{3, 4, 26},
	{15, 16, 27},
	{16, 5, 27},
	{5, 4, 27},
	{4, 19, 27},
	{19, 15, 27},
	{7, 11, 28},
	{11, 18, 28},
	{18, 12, 28},
	{12, 6, 28},
	{6, 7, 28},
	{10, 0, 29},
	{0, 17, 29},
	{17, 18, 29},
	{18, 11, 29},
	{11, 10, 29},
	{0, 10, 30},
	{10, 9, 30},
	{9, 16, 30},
	{16, 15, 30},
	{15, 0, 30},
	{17, 0, 31},
	{0, 15, 31},
	{15, 19, 31},
	{19, 14, 31},
	{14, 17, 31}
};


int objectNumber = 1; // Which object to draw (1 ,2, 3, 4, 5, or 6)?
//   (Controlled by number keys.)

int useAnaglyph = 0; // Should anaglyph stereo be used?
//    (Controlled by space bar.)

int rotateX = 0; // Rotations of the cube about the axes.
int rotateY = 0; //   (Controlled by arrow, PageUp, PageDown keys;
int rotateZ = 0; //    Home key sets all rotations to 0.)
static char buf[256];

void draw_spiral()
{
	static float angleStep = 3.f;
	static int maxSteps = 360 / 3; // max_degrees / (int)angleStep
	static float radius = 1.f;
	static float angle, ptsSize, x, y, z, r, g, b;
	static int times = 8;
	static int i;
	static int range = 8 * 360 / 3; // times * maxSteps
	static float rangeF = 8 * 360 / 3.f; // times * (float)maxSteps

	for (i = 0; i < range; i++)
	{
		angle = i * angleStep / 360 * M_2PI;

		// Max point size is 10.f, min point size is 0.1f
		// Specifying range of <1.f,9.f> here
		ptsSize = 9.f * i / rangeF + 1.f;
		glPointSize(ptsSize);

		r = 0.25f * sinf((i + rangeF / 2) / rangeF * M_2PI) + .75f;
		g = 0.25f * sinf((i + rangeF / 4) / rangeF * M_2PI) + .75f;
		b = 0.25f * sinf((i + rangeF / 8) / rangeF * M_2PI) + .75f;

		glColor3f(r, g, b);

		glBegin(GL_POINTS);
		{
			x = radius * sinf(angle);
			y = radius * cosf(angle);
			z = -radius + i * .01f;

			glVertex3f(
				x,
				y,
				z
			);
		}
		glEnd();
	}
}

void draw_pyramid_triangle3d()
{
	glVertex3f(-1.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 2.412f, 4.f);
}

void draw_pyramid_triangle2d()
{
	glVertex2f(-1.f, 0);
	glVertex2f(1.f, 0);
	glVertex2f(0.f, 2.412f);
}

void draw_pyramid()
{
	static int i;

	glColor3f(1.f, 1.f, 0.f);

	glBegin(GL_TRIANGLES);
	{
		draw_pyramid_triangle2d();
	}
	glEnd();

	for (i = 1; i < 8; ++i)
	{
		glPushMatrix();
		{
			glTranslatef(0.f, 2.412f, 0.f);
			glRotatef(i * 45, 0.f, 0.f, 1.f);
			glTranslatef(0.f, -2.412f, 0.f);

			glBegin(GL_TRIANGLES);
			{
				draw_pyramid_triangle2d();
			}
			glEnd();
		}
		glPopMatrix();
	}

	glBegin(GL_TRIANGLES);
	{
		draw_pyramid_triangle3d();
	}
	glEnd();

	for (i = 1; i < 8; ++i)
	{
		glPushMatrix();
		{
			glTranslatef(0.f, 2.412f, 0.f);
			glRotatef(i * 45, 0.f, 0.f, 1.f);
			glTranslatef(0.f, -2.412f, 0.f);

			glBegin(GL_TRIANGLES);
			{
				draw_pyramid_triangle3d();
			}
			glEnd();
		}
		glPopMatrix();
	}
}

/*
 * The function that draws the current object, with its modeling transformation.
 */
void draw()
{
	glRotatef(rotateZ, 0, 0, 1); // Apply rotations to complete object.
	glRotatef(rotateY, 0, 1, 0);
	glRotatef(rotateX, 1, 0, 0);

	if (1 == objectNumber)
	{
		draw_spiral();
	}
	else if (2 == objectNumber)
	{
		draw_pyramid();
	}
}


//-------------------- Draw the Scene  -------------------------

/*
 * The display function is called when the window needs to be drawn.
 * It's called when the window opens and it is called by the keyboard-handling
 * functions when the user hits a key that modifies the scene.
 */
void display()
{
	// Display function will draw the image.

	if (useAnaglyph)
	{
		glDisable(GL_COLOR_MATERIAL); // in anaglyph mode, everything is drawn in white
		float white[] = { 1, 1, 1, 1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
	{
		glEnable(GL_COLOR_MATERIAL); // in non-anaglyph mode, glColor* is respected
	}
	glNormal3f(0, 0, 1); // (Make sure normal vector is correct for object 1.)

	glClearColor(0, 0, 0, 1); // Background color (black).
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!useAnaglyph)
	{
		glLoadIdentity(); // Make sure we start with no transformation!
		glTranslated(0, 0, -15); // Move object away from viewer (at (0,0,0)).
		draw();
	}
	else
	{
		glLoadIdentity();
		glColorMask(1, 0, 0, 1);
		glRotatef(4, 0, 1, 0);
		glTranslated(1, 0, -15);
		draw();
		glColorMask(1, 0, 0, 1);
		glClear(GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glRotatef(-4, 0, 1, 0);
		glTranslated(-1, 0, -15);
		glColorMask(0, 1, 1, 1);
		draw();
		glColorMask(1, 1, 1, 1);
	}

	glutSwapBuffers(); // Required AT THE END to copy color buffer onto the screen.
} // end display()


/*  The initGL function is called once from main() to initialize
 *  OpenGL.  Here, it sets up a projection, turns on some lighting,
 *  and enables the depth test.
 */
void initGL()
{
	glMatrixMode(GL_PROJECTION);
	glFrustum(-3.5, 3.5, -3.5, 3.5, 5, 25);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float gray[] = { 0.7, 0.7, 0.7, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(3); // make wide lines for the stellated dodecahedron.
}

//-------------------- Key-handling functions ---------------------------

void doSpecialKey(int key, int x, int y)
{
	// called when a special key is pressed
	int redraw = 1;
	if (key == GLUT_KEY_LEFT)
		rotateY -= 15;
	else if (key == GLUT_KEY_RIGHT)
		rotateY += 15;
	else if (key == GLUT_KEY_DOWN)
		rotateX += 15;
	else if (key == GLUT_KEY_UP)
		rotateX -= 15;
	else if (key == GLUT_KEY_PAGE_UP)
		rotateZ += 15;
	else if (key == GLUT_KEY_PAGE_DOWN)
		rotateZ -= 15;
	else if (key == GLUT_KEY_HOME)
		rotateX = rotateY = rotateZ = 0;
	else
		redraw = 0;
	if (redraw)
		glutPostRedisplay(); // will repaint the window
}

void doKeyboard(unsigned char ch, int x, int y)
{
	int redraw = 1;
	if (ch == '1')
		objectNumber = 1;
	else if (ch == '2')
		objectNumber = 2;
	else if (ch == '3')
		objectNumber = 3;
	else if (ch == '4')
		objectNumber = 4;
	else if (ch == '5')
		objectNumber = 5;
	else if (ch == '6')
		objectNumber = 6;
	else if (ch == ' ')
		useAnaglyph = !useAnaglyph;
	else
		redraw = 0;
	if (redraw)
		glutPostRedisplay(); // will repaint the window
}


// --------------------------- main() -------------------------------------

int main(int argc, char** argv)
{
	// Initialize GLUT and open the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); // Use double-buffering and depth buffer.
	glutInitWindowSize(700, 700); // Size of display area, in pixels.
	glutInitWindowPosition(100, 100); // Location of window in screen coordinates.
	glutCreateWindow("Some Objects in 3D"); // Parameter is window title.
	initGL(); // Call the OpenGL initialization function, defined above; must be after glutCreateWindow.
	glutDisplayFunc(display); // display() is called when the window needs to be redrawn.
	glutKeyboardFunc(doKeyboard); // doKeyboard() is called to process normal keys.
	glutSpecialFunc(doSpecialKey); // doSpecialKey() is called to process other keys (such as arrows).
	glutMainLoop();
	return 0;
}