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


float points[45][45][3];    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;		// Counter Used To Control How Fast Flag Waves

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
	//glRotatef(xrot,1.0f,0.0f,0.0f);
	//glRotatef(yrot,0.0f,1.0f,0.0f);  
	//glRotatef(zrot,0.0f,0.0f,1.0f);



	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, tex );

    //glRotatef(rot, 1.0, 0.0, 0.0); //rotate 20 degrees around the x axis

    /*glBegin(GL_QUADS);        //begin shape
	//glColor4f(1.0, 0.0, 0.0, 1.0);
	glTexCoord2f(0, 0);
    glVertex3f(-1.0, -1.0, 0.0);    //draw each vertex
    
	//glColor4f(0.0, 1.0, 0.0, 1.0);
	glTexCoord2f(1, 0);
	glVertex3f(1.0, -1.0, 0.0);
    
	//glColor4f(0.0, 0.0, 1.0, 1.0);
	glTexCoord2f(1, 1);
	glVertex3f(1.0, 1.0, 0.0);
    
	//glColor4f(1.0, 1.0, 0.0, 1.0);
	glTexCoord2f(0, 1);
	glVertex3f(-1.0, 1.0, 0.0);
    glEnd();  */              //end shape
    //glPopMatrix();            //end matrix

	glBegin(GL_QUADS);
	for( x = 0; x < 44; x++ )
	{
		for( y = 0; y < 44; y++ )
		{
			float_x = float(x)/44.0f;
			float_y = float(y)/44.0f;
			float_xb = float(x+1)/44.0f;
			float_yb = float(y+1)/44.0f;

			glTexCoord2f( float_x, float_y);
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

			glTexCoord2f( float_x, float_yb );
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

			glTexCoord2f( float_xb, float_yb );
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

			glTexCoord2f( float_xb, float_y );
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();

		if( wiggle_count == 2 )
	{
		for( y = 0; y < 45; y++ )
		{
			hold=points[0][y][2];
			for( x = 0; x < 44; x++)
			{
				points[x][y][2] = points[x+1][y][2];
			}
			points[44][y][2]=hold;
		}
		wiggle_count = 0;
	}

	wiggle_count++;

    glutSwapBuffers();
	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;
    //rot += .1;            //increase rotation amount
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
	
	for(int x=0; x<45; x++)
	{
		for(int y=0; y<45; y++)
		{
			points[x][y][0]=float((x/5.0f)-4.5f);
			points[x][y][1]=float((y/5.0f)-4.5f);
			points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
		}
	}

    glutMainLoop();
    return EXIT_SUCCESS;
} 