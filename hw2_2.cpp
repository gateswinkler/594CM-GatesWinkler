#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>

float roty = 0.0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void display(void)
{
   GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   glTranslatef (0.0, 0.0, -5.0);

   glPushMatrix ();
   glRotated (roty, 1.0, 0.0, 0.0);
   glLightfv (GL_LIGHT0, GL_POSITION, position);

   glTranslated (0.0, 0.0, 1.5);
   glPopMatrix ();

    glBegin(GL_QUADS);        //begin shape
	glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-2.0, -2.0, 0.0);    //draw each vertex
    glVertex3f(-2.0, -1.5, 0.0);
    glVertex3f(2.0, -1.5, 0.0);
    glVertex3f(2.0, -2.0, 0.0);
    glEnd(); 
   
    glBegin(GL_QUADS);        //begin shape
	
    glVertex3f(1.0, -2.0, 0.0);    //draw each vertex
    glVertex3f(1.2, 0.5, 0.3);
    glVertex3f(1.5, -0.3, -1.0);
    glVertex3f(2.0, -2.0, 0.0);
    glEnd(); 

	glBegin(GL_QUADS);        //begin shape
	
    glVertex3f(-.5, -2.0, 0.0);    //draw each vertex
    glVertex3f(1.0, 0.5, 0.5);
    glVertex3f(1.2, 1.0, -0.1);
    glVertex3f(1.4, -2.0, 0.0);
    glEnd(); 
	
	glBegin(GL_QUADS);        //begin shape
	
    glVertex3f(-1.5, -2.0, 0.0);    //draw each vertex
    glVertex3f(-1.0, 0.0, 1.0);
    glVertex3f(0.0, -1.0, 0.2);
    glVertex3f(0.5, -2.0, 0.0);
    glEnd(); 
	
	glBegin(GL_QUADS);        //begin shape
	
    glVertex3f(-2.0, -2.0, 0.0);    //draw each vertex
    glVertex3f(-2.0, -1.0, -0.7);
    glVertex3f(-1.0, -1.5, -0.2);
    glVertex3f(-1.0, -2.0, 0.0);
    glEnd(); 
   //glutSolidTorus (0.275, 0.85, 8, 15);
   glPopMatrix ();
   glFlush ();
   roty += .1; 
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void idle(void)
{
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   //glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}