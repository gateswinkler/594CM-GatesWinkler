
#include <windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <SOIL/soil.h>
#include <stdio.h>


//variable for rotation


#define		MAP_SIZE	  1024							// Size Of Our .RAW Height Map (NEW)
#define		STEP_SIZE	  16							// Width And Height Of Each Quad (NEW)
#define		HEIGHT_RATIO  1.5f							// Ratio That The Y Is Scaled According To The X And Z (NEW)

BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];					// Holds The Height Map Data (NEW)

float scaleValue = 0.15f;		
float rot = 0.0;
GLuint tex = 0;
void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap)
{
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.
	pFile = fopen( strName, "rb" );

	// Check To See If We Found The File And Could Open It
	if ( pFile == NULL )	
	{
		// Display Error Message And Stop The Function
		//MessageBox(NULL, "Can't Find The Height Map!", "Error", MB_OK);
		return;
	}

	fread( pHeightMap, 1, nSize, pFile );

	// After We Read The Data, It's A Good Idea To Check If Everything Read Fine
	int result = ferror( pFile );

	// Check If We Received An Error
	if (result)
	{
		//MessageBox(NULL, "Failed To Get Data!", "Error", MB_OK);
	}

	// Close The File.
	fclose(pFile);
}

int Height(BYTE *pHeightMap, int X, int Y)				// This Returns The Height From A Height Map Index
{
	int x = X % MAP_SIZE;								// Error Check Our x Value
	int y = Y % MAP_SIZE;								// Error Check Our y Value

	if(!pHeightMap) return 0;							// Make Sure Our Data Is Valid

	return pHeightMap[x + (y * MAP_SIZE)];				// Index Into Our Height Array And Return The Height
}
														
void SetVertexColor(BYTE *pHeightMap, int x, int y)		// Sets The Color Value For A Particular Index, Depending On The Height Index
{
	if(!pHeightMap) return;								// Make Sure Our Height Data Is Valid

	float fColor = -0.15f + (Height(pHeightMap, x, y ) / 256.0f);

	// Assign This Blue Shade To The Current Vertex
	glColor3f(0, 0, fColor );
}


void RenderHeightMap(BYTE pHeightMap[])					// This Renders The Height Map As Quads
{
	int X = 0, Y = 0;									// Create Some Variables To Walk The Array With.
	int x, y, z;										// Create Some Variables For Readability

	if(!pHeightMap) return;								// Make Sure Our Height Data Is Valid

	glBegin( GL_QUADS );							// Render Polygons

	for ( X = 0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE )
		for ( Y = 0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE )
		{
			// Get The (X, Y, Z) Value For The Bottom Left Vertex
			x = X;							
			y = Height(pHeightMap, X, Y );	
			z = Y;							

			// Set The Color Value Of The Current Vertex
			SetVertexColor(pHeightMap, x, z);

			glVertex3i(x, y, z);						// Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster)

			// Get The (X, Y, Z) Value For The Top Left Vertex
			x = X;										
			y = Height(pHeightMap, X, Y + STEP_SIZE );  
			z = Y + STEP_SIZE ;							
			
			// Set The Color Value Of The Current Vertex
			SetVertexColor(pHeightMap, x, z);

			glVertex3i(x, y, z);						// Send This Vertex To OpenGL To Be Rendered

			// Get The (X, Y, Z) Value For The Top Right Vertex
			x = X + STEP_SIZE; 
			y = Height(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE ); 
			z = Y + STEP_SIZE ;

			// Set The Color Value Of The Current Vertex
			SetVertexColor(pHeightMap, x, z);
			
			glVertex3i(x, y, z);						// Send This Vertex To OpenGL To Be Rendered

			// Get The (X, Y, Z) Value For The Bottom Right Vertex
			x = X + STEP_SIZE; 
			y = Height(pHeightMap, X + STEP_SIZE, Y ); 
			z = Y;

			// Set The Color Value Of The Current Vertex
			SetVertexColor(pHeightMap, x, z);

			glVertex3i(x, y, z);						// Send This Vertex To OpenGL To Be Rendered
		}
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);					// Reset The Color
}

/////////////
//DisplayLoop
/////////////
void display(void)
{
    //glPushMatrix();            //start matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glColor3f(1.0, 1.0, 1.0);    //set color of rectangle


	//glEnable( GL_TEXTURE_2D );
	//glBindTexture( GL_TEXTURE_2D, tex );
    //glTranslatef(0.0, 0.0, -5.0);    //set origin of shape (5 units into the distance)
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The Matrix
	
	// 			 Position	      View		Up Vector
	gluLookAt(212, 60, 194,  186, 55, 171,  0, 1, 0);	// This Determines Where The Camera's Position And View Is

	glRotatef(rot, 0.0, 1.0, 0.0); //rotate 20 degrees around the x axis
	glTranslatef(-65.0, 00.0, -50.0);	
	
	glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);

		RenderHeightMap(g_HeightMap);						// Render The Height Map


   /* glBegin(GL_QUADS);        //begin shape
	//glColor4f(1.0, 0.0, 0.0, 1.0);
	//glTexCoord2f(0, 0);
    glVertex3f(-1.0, -1.0, 0.0);    //draw each vertex
    
	//glColor4f(0.0, 1.0, 0.0, 1.0);
	//glTexCoord2f(1, 0);
	glVertex3f(1.0, -1.0, 0.0);
    
	//glColor4f(0.0, 0.0, 1.0, 1.0);
	//glTexCoord2f(1, 1);
	glVertex3f(1.0, 1.0, 0.0);
    
	//glColor4f(1.0, 1.0, 0.0, 1.0);
	//glTexCoord2f(0, 1);
	glVertex3f(-1.0, 1.0, 0.0);
    glEnd();  */              //end shape
    //glPopMatrix();            //end matrix

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
    gluPerspective(45.0, (float)width/(float)height, .1, 500.0); //far plane is set to 100 units

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
	//printf("test1 \n");
	//loadTextures();
	//printf("test2 \n");

	LoadRawFile("Data/Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);	// (NEW)

    glutMainLoop();
    return EXIT_SUCCESS;
} 