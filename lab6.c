
/**
 * CPSC 424, Fall 2015, Lab 6:  Light and Material in OpenGL 1.1.
 * This program shows a square "stage" with a variety of objects
 * arranged on it.  The objects use several shapes and materials
 * and include a wireframe object that is drawn with lighting
 * turned off.  The user can rotate the scene about the y-axis by
 * dragging the mouse horizontally.
 *
 * This program use GLU as well as GLUT, and it depends on polyhedron.c,
 * which requires the math library.  It can be compiled with
 *
 *        gcc -o lab6 lab6.c polyhedron.c -lGL -lglut -lGLU -lm
 */
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/freeglut.h>
#include <stdio.h>      // (Can be used for debugging messages, with printf().)
#include "polyhedron.h" // For access to the regular polyhedra from polyhedron.c.
#include <math.h>
#define M_2PI 6.28318530718f

static double rotateY;  // rotation amount about the y-axis

// --------------------------- Data for some materials ---------------------------------------------------

enum GLMaterials
{
	GLM_EMERALD,
	GLM_JADE,
	GLM_OBSIDIAN,
	GLM_PEARL,
	GLM_RUBY,
	GLM_TURQUOISE,
	GLM_BRASS,
	GLM_BRONZE,
	GLM_CHROME,
	GLM_COPPER,
	GLM_GOLD,
	GLM_SILVER,
	GLM_CYANPL,
	GLM_GREENPL,
	GLM_REDPL,
	GLM_CYANRB,
	GLM_GREENRUB,
	GLM_REDRUB
};

/**
 * One of the rows of this array corresponds to a set of material properties.  Items 0 to 4 in a row
 * specify an ambient color; items 4 through 7, a diffuse color; items 8 through 11, a specular color;
 * and item 12, a specular exponent (shininess value).  The data is adapted from the table on the page
 * http://devernay.free.fr/cours/opengl/materials.html
 */
float materials[][13] = {
	{ /* "emerald" */   0.0215f, 0.1745f, 0.0215f, 1.0f, 0.07568f, 0.61424f, 0.07568f, 1.0f, 0.633f, 0.727811f, 0.633f, 1.0f, 0.6f * 128 },
	{ /* "jade" */   0.135f, 0.2225f, 0.1575f, 1.0f, 0.54f, 0.89f, 0.63f, 1.0f, 0.316228f, 0.316228f, 0.316228f, 1.0f, 0.1f * 128 },
	{ /* "obsidian" */   0.05375f, 0.05f, 0.06625f, 1.0f, 0.18275f, 0.17f, 0.22525f, 1.0f, 0.332741f, 0.328634f, 0.346435f, 1.0f, 0.3f * 128 },
	{ /* "pearl" */   0.25f, 0.20725f, 0.20725f, 1.0f, 1.0f, 0.829f, 0.829f, 1.0f, 0.296648f, 0.296648f, 0.296648f, 1.0f, 0.088f * 128 },
	{ /* "ruby" */   0.1745f, 0.01175f, 0.01175f, 1.0f, 0.61424f, 0.04136f, 0.04136f, 1.0f, 0.727811f, 0.626959f, 0.626959f, 1.0f, 0.6f * 128 },
	{ /* "turquoise" */   0.1f, 0.18725f, 0.1745f, 1.0f, 0.396f, 0.74151f, 0.69102f, 1.0f, 0.297254f, 0.30829f, 0.306678f, 1.0f, 0.1f * 128 },
	{ /* "brass" */   0.329412f, 0.223529f, 0.027451f, 1.0f, 0.780392f, 0.568627f, 0.113725f, 1.0f, 0.992157f, 0.941176f, 0.807843f, 1.0f, 0.21794872f * 128 },
	{ /* "bronze" */   0.2125f, 0.1275f, 0.054f, 1.0f, 0.714f, 0.4284f, 0.18144f, 1.0f, 0.393548f, 0.271906f, 0.166721f, 1.0f, 0.2f * 128 },
	{ /* "chrome" */   0.25f, 0.25f, 0.25f, 1.0f, 0.4f, 0.4f, 0.4f, 1.0f, 0.774597f, 0.774597f, 0.774597f, 1.0f, 0.6f * 128 },
	{ /* "copper" */   0.19125f, 0.0735f, 0.0225f, 1.0f, 0.7038f, 0.27048f, 0.0828f, 1.0f, 0.256777f, 0.137622f, 0.086014f, 1.0f, 0.1f * 128 },
	{ /* "gold" */   0.24725f, 0.1995f, 0.0745f, 1.0f, 0.75164f, 0.60648f, 0.22648f, 1.0f, 0.628281f, 0.555802f, 0.366065f, 1.0f, 0.4f * 128 },
	{ /* "silver" */   0.19225f, 0.19225f, 0.19225f, 1.0f, 0.50754f, 0.50754f, 0.50754f, 1.0f, 0.508273f, 0.508273f, 0.508273f, 1.0f, 0.4f * 128 },
	{ /* "cyan plastic" */   0.0f, 0.1f, 0.06f, 1.0f, 0.0f, 0.50980392f, 0.50980392f, 1.0f, 0.50196078f, 0.50196078f, 0.50196078f, 1.0f, .25f * 128 },
	{ /* "green plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.35f, 0.1f, 1.0f, 0.45f, 0.55f, 0.45f, 1.0f, .25f * 128 },
	{ /* "red plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.6f, 0.6f, 1.0f, .25f * 128 },
	{ /* "cyan rubber" */   0.0f, 0.05f, 0.05f, 1.0f, 0.4f, 0.5f, 0.5f, 1.0f, 0.04f, 0.7f, 0.7f, 1.0f, .078125f * 128 },
	{ /* "green rubber" */   0.0f, 0.05f, 0.0f, 1.0f, 0.4f, 0.5f, 0.4f, 1.0f, 0.04f, 0.7f, 0.04f, 1.0f, .078125f * 128 },
	{ /* "red rubber" */   0.05f, 0.0f, 0.0f, 1.0f, 0.5f, 0.4f, 0.4f, 1.0f, 0.7f, 0.04f, 0.04f, 1.0f, .078125f * 128 },
};

void draw_pyramid_triangle3d()
{
	glVertex3f(-1.f, -1.f, 0.f);
	glVertex3f(1.f, -1.f, 0.f);
	glVertex3f(0.f, 3.f, 2.412f);
}

void draw_pyramid()
{
	static int i;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[GLM_EMERALD]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[GLM_EMERALD] + 4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[GLM_EMERALD] + 8);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materials[GLM_EMERALD] + 12);
	
	glBegin(GL_TRIANGLES);
	{
		draw_pyramid_triangle3d();
	}
	glEnd();

	for (i = 1; i < 8; ++i)
	{	
		glPushMatrix();
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[i]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[i] + 4);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[i] + 8);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materials[i+1] + 12);
			
			glTranslatef(0.f, 0.f, 2.412f);
			glRotatef(i * 45, 0.f, 1.f, 0.f);
			glTranslatef(0.f, 0.f, -2.412f);

			glBegin(GL_TRIANGLES);
			{
				draw_pyramid_triangle3d();
			}
			glEnd();
		}
		glPopMatrix();
	}
}



// ------------------------ OpenGL rendering and  initialization -----------------------

/**
 * The display method is called when the panel needs to be drawn.
 * Here, it draws a stage and some objects on the stage.
 */
void display() {
	// called whenever the display needs to be redrawn

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 8, 40, 0, 1, 0, 0, 1, 0);  // viewing transform	

	glRotated(rotateY, 0, 1, 0);  // modeling transform: rotation of the scene about y-axis

	float gray[] = { 0.6f, 0.6f, 0.6f, 1 };
	float zero[] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	glPushMatrix();
	glTranslatef(0, -1.5, 0); // Move top of stage down to y = 0
	glScalef(1, 0.05f, 1); // Stage will be one unit thick,
	glutSolidCube(20);
	glPopMatrix();

	draw_pyramid();

	glutSwapBuffers();  // (Required for double-buffered drawing, at the end of display().)
}

/**
 * initGL() is called just once, by main(), to do initialization of OpenGL state
 * and other global state. Here, it sets up a projection, configures some lighting,
 * and enables the depth test.  It also calls createPolyhedra(), whcih is defined
 * in the included file, polyhedron.h.
 */
void initGL() {
	createPolyhedra();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20, 2, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	static float l0amb[] = {0.f, 0.f, 0.f, 1.f};
	static float l0diff[] = {1.f, 1.f, 1.f, 1.f};
	static float l0spec[] = {1.f, 1.f, 1.f, 1.f};
	static float l0pos[] = {1.f, 1.f, 0.f, 0.f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spec);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	
	static float l1amb[] = {.05f, .05f, .05f, 1.f};
	static float l1diff[] = {.95f, .9f, .5f, 1.f};
	static float l1pos[] = {0.f, 0.f, 1.f, 0.f};

	glLightfv(GL_LIGHT1, GL_AMBIENT, l1amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1diff);
	glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
	
}  // end initGL()


// ------------------------------ mouse handling functions ----------------------------------

int dragging;        // 0 or 1 to indicate whether a drag operation is in progress
int dragButton;      // which button started the drag operation
int prevX;           // previous mouse x coordinate during drag

/*  mouseUpOrDown() is set up in main() to be called when the user presses or releases
 *  a button on the mouse.  The button paramter is one of the contants GLUT_LEFT_BUTTON,
 *  GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.  The buttonState is GLUT_UP or GLUT_DOWN and
 *  tells whether this is a mouse press or a mouse release event.  x and y give the
 *  mouse position in pixel coordinates, with (0,0) at the UPPER LEFT.
 */
void mouseUpOrDown(int button, int buttonState, int x, int y) {
	// called to respond to mouse press and mouse release events
	if (buttonState == GLUT_DOWN) {  // a mouse button was pressed
		if (dragging)
			return;  // Ignore a second button press during a draw operation.
		dragging = 1;
		dragButton = button;
		prevX = x;
	}
	else {  // a mouse button was released
		if (!dragging || button != dragButton)
			return; // this mouse release does not end a drag operation.
		dragging = 0;
	}
}

/*  mouseDragged() is set up in main() to be called when the user moves the mouse,
 *  but only when one or more mouse buttons are pressed.  x and y give the position
 *  of the mouse in pixel coordinates.
 */
void mouseDragged(int x, int y) {
	// called to respond when the mouse moves during a drag
	if (!dragging)
		return;
	double dx = x - prevX;
	rotateY += dx / 7;
	glutPostRedisplay();  // make OpenGL redraw the scene
	prevX = x;
}


// ----------------- main routine -------------------------------------------------

int main(int argc, char** argv) {
	glutInit(&argc, argv); // Allows processing of certain GLUT command line options
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);  // Use double buffering and a depth buffer.
	glutInitWindowSize(1000, 500);       // size of display area, in pixels
	glutInitWindowPosition(100, 100);    // location in window coordinates
	glutCreateWindow("Stage");          // parameter is window title  
	initGL();                           // do OpenGL initialization for the window
	glutDisplayFunc(display);           // call display() to draw the scene
	glutMouseFunc(mouseUpOrDown);       // call mouseUpOrDown() for mousedown and mouseup events
	glutMotionFunc(mouseDragged);       // call mouseDragged() when mouse moves, only during a drag gesture
	glutMainLoop(); // Run the event loop!  This function does not return.
	return 0;
}
