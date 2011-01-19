#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>

//variable for rotation
float roty = 0.0;

/////////////
//DisplayLoop
/////////////
void display(void)
{
    //glPushMatrix();            //start matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);    //set color of rectangle


    glTranslatef(1.0, 0.0, -5.0);    //set origin of shape (5 units into the distance)
    //glRotatef(roty, 0.0, 1.0, 0.0); //rotate 20 degrees around the x axis

    glBegin(GL_QUADS);        //begin shape
    glVertex3f(-0.5, -0.5, 0.5);    //draw each vertex
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

	glVertex3f(-0.5, -0.5, 0.5);    
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);

	glVertex3f(-0.5, -0.5, 0.5);    
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);

	glVertex3f(-0.5, 0.5, 0.5);    
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);

	glVertex3f(0.5, -0.5, 0.5);    
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);

    glVertex3f(-0.5, -0.5, -0.5);    //draw each vertex
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
	glEnd();                //end shape
    //glPopMatrix();            //end matrix

	glRotatef(roty, 0.0, 1.0, 0.0); //rotate 20 degrees around the x axis
	glTranslatef(1.0, 0.0, 0.0);    //set origin of shape (5 units into the distance)
    
    glBegin(GL_QUADS);        //begin shape
    glVertex3f(-0.1, -0.1, 0.1);    //draw each vertex
    glVertex3f(0.1, -0.1, 0.1);
    glVertex3f(0.1, 0.1, 0.1);
    glVertex3f(-0.1, 0.1, 0.1);

	glVertex3f(-0.1, -0.1, 0.1);    
	glVertex3f(-0.1, -0.1, -0.1);
	glVertex3f(0.1, -0.1, -0.1);
	glVertex3f(0.1, -0.1, 0.1);

	glVertex3f(-0.1, -0.1, 0.1);    
	glVertex3f(-0.1, -0.1, -0.1);
	glVertex3f(-0.1, 0.1, -0.1);
	glVertex3f(-0.1, 0.1, 0.1);

	glVertex3f(-0.1, 0.1, 0.1);    
	glVertex3f(-0.1, 0.1, -0.1);
	glVertex3f(0.1, 0.1, -0.1);
	glVertex3f(0.1, 0.1, 0.1);

	glVertex3f(0.1, -0.1, 0.1);    
	glVertex3f(0.1, -0.1, -0.1);
	glVertex3f(0.1, 0.1, -0.1);
	glVertex3f(0.1, 0.1, 0.1);

    glVertex3f(-0.1, -0.1, -0.1);    //draw each vertex
    glVertex3f(0.1, -0.1, -0.1);
    glVertex3f(0.1, 0.1, -0.1);
    glVertex3f(-0.1, 0.1, -0.1);
	glEnd(); 
	
	
	glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);    //set color of rectangle
	glTranslatef(-1.0, 0.0, -5.0);    //set origin of shape (5 units into the distance)
    //glRotatef(roty, 1.0, 0.0, 0.0); //rotate 20 degrees around the x axis

    glBegin(GL_QUADS);        //begin shape
    glVertex3f(-0.5, -0.5, 0.5);    //draw each vertex
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

	glVertex3f(-0.5, -0.5, 0.5);    
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);

	glVertex3f(-0.5, -0.5, 0.5);    
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);

	glVertex3f(-0.5, 0.5, 0.5);    
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);

	glVertex3f(0.5, -0.5, 0.5);    
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);

    glVertex3f(-0.5, -0.5, -0.5);    //draw each vertex
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
	glEnd();  
	//glPopMatrix();            //end matrix

	glRotatef(roty, 0.0, 0.0, 1.0); //rotate 20 degrees around the x axis
	glTranslatef(0.0, 1.0, 0.0);    //set origin of shape (5 units into the distance)
	glRotatef(roty, 1.0, 0.0, 0.0); //rotate 20 degrees around the x axis
	
    glBegin(GL_QUADS);        //begin shape
    glVertex3f(-0.1, -0.1, 0.1);    //draw each vertex
    glVertex3f(0.1, -0.1, 0.1);
    glVertex3f(0.1, 0.1, 0.1);
    glVertex3f(-0.1, 0.1, 0.1);

	glVertex3f(-0.1, -0.1, 0.1);    
	glVertex3f(-0.1, -0.1, -0.1);
	glVertex3f(0.1, -0.1, -0.1);
	glVertex3f(0.1, -0.1, 0.1);

	glVertex3f(-0.1, -0.1, 0.1);    
	glVertex3f(-0.1, -0.1, -0.1);
	glVertex3f(-0.1, 0.1, -0.1);
	glVertex3f(-0.1, 0.1, 0.1);

	glVertex3f(-0.1, 0.1, 0.1);    
	glVertex3f(-0.1, 0.1, -0.1);
	glVertex3f(0.1, 0.1, -0.1);
	glVertex3f(0.1, 0.1, 0.1);

	glVertex3f(0.1, -0.1, 0.1);    
	glVertex3f(0.1, -0.1, -0.1);
	glVertex3f(0.1, 0.1, -0.1);
	glVertex3f(0.1, 0.1, 0.1);

    glVertex3f(-0.1, -0.1, -0.1);    //draw each vertex
    glVertex3f(0.1, -0.1, -0.1);
    glVertex3f(0.1, 0.1, -0.1);
    glVertex3f(-0.1, 0.1, -0.1);
	glEnd();

    glutSwapBuffers();

    roty += .1;            //increase rotation amount
}


//////////////////
//Reshape Function
//////////////////
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);    //
    glMatrixMode(GL_PROJECTION);        //
    glLoadIdentity();                    //
    gluPerspective(45.0, (float)width/(float)height, .1, 100.0); //far plane is set to 100 units

}


///////////////
//Idle Function
///////////////
void idle(void)
{
    glutPostRedisplay();
}


///////////////
//Init Function
///////////////
int main(int argc, char** argv) //THIS IS THE BARE MINIMUM OF A GLUT PROJECT
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);

    glutCreateWindow("GLUT Program");

    glutDisplayFunc(display); //REGISTERS CALLBACK FUNCTIONS
    glutReshapeFunc(reshape); //RESIZING/VIEWPORT
    glutIdleFunc(idle);
    glClearColor(0.0,0.0,0.0,0.0);


    glutMainLoop();
    return EXIT_SUCCESS;
} 