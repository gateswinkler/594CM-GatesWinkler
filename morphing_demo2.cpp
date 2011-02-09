#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <SOIL/soil.h>
#include <stdio.h>


//variable for rotation
float rot = 0.0;
GLuint tex = 0;


float Plane_points[45][3];
float temp_points[45][3];
float Cyl_points[45][3];// The Array For The Points On The Grid Of Our "Wave"
int count = 0;		// Counter Used To Control How Fast Flag Waves

GLfloat	xrot;				// X Rotation ( NEW )
GLfloat	yrot;				// Y Rotation ( NEW )
GLfloat	zrot;				// Z Rotation ( NEW )
GLfloat hold;				// Temporarily Holds A Floating Point Value
/////////////
//DisplayLoop
/////////////
void display(void)
{
	
	int x, y;
	float float_x, float_y, float_xb, float_yb;
    //glPushMatrix();            //start matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glColor3f(1.0, 1.0, 1.0);    //set color of rectangle
	glTranslatef(0.0, 0.0, -15.0);    //set origin of shape (5 units into the distance)
	glRotatef(90,1.0f,0.0f,0.0f);
	//glRotatef(yrot,0.0f,1.0f,0.0f);  
	//glRotatef(zrot,0.0f,0.0f,1.0f);



	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, tex );


	glBegin(GL_QUAD_STRIP);
		for( x = 0; x < 45; x++ )
		{				
			Plane_points[x][0]-=(Plane_points[x][0] - Cyl_points[x][0])/200;
			Plane_points[x][1]-=(Plane_points[x][1] - Cyl_points[x][1])/200;
			Plane_points[x][2]-=(Plane_points[x][2] - Cyl_points[x][2])/200;
		
			//glTexCoord2f( float_x, float_y);
			glColor4f(1.0, 0.0, 0.0, 1.0);
			glVertex3f( Plane_points[x][0], Plane_points[x][1], Plane_points[x][2] );

			//glTexCoord2f( float_x, float_yb );
			glColor4f(0.0, 1.0, 0.0, 1.0);
			glVertex3f( Plane_points[x][0], -Plane_points[x][1], Plane_points[x][2] );

		}
	glEnd();

    glutSwapBuffers();
	rot += .1;            //increase rotation amount
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
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();	

}

void loadTextures()
{
	tex = SOIL_load_OGL_texture(
		"img/profile.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
		);

	if(tex == 0)
	{
		printf("failed to load texture");
	}
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
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0,0.0,0.0,0.0);
	//printf("test1 \n");
	//loadTextures();
	//printf("test2 \n");
	
	static GLfloat angle = 0.0;
	
		for(int x=0; x<45; x++)
		{
			angle = x * 2.0 * 3.14159265 / 44;
			Cyl_points[x][0]=5.0f * cos(angle);
			Cyl_points[x][1]=-0.5;
			Cyl_points[x][2]=5.0f * sin(angle);
		}
		
		for(int x=0; x<45; x++)
		{
			//angle = x * 2.0 * 3.14159265 / 44;
			temp_points[x][3] = Plane_points[x][0]=x/9;
			temp_points[x][3] = Plane_points[x][1]=-0.5;
			temp_points[x][3] = Plane_points[x][2]=0;
		}
	
	/*for (i = 0; i <= 50; i++) {
    angle = i * 2.0 * 3.14159265 / 50;

    //glNormal3f(-cos(angle), -sin(angle), 0.0);
    glVertex3f(1.0f * cos(angle), 1.0f * sin(angle), -1.0f * 0.5);
    glVertex3f(1.0f * cos(angle), 1.0f * sin(angle), 1.0f * 0.5);
  }
*/
    glutMainLoop();
    return EXIT_SUCCESS;
} 