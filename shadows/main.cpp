//#include <windows.h>
#include <stdlib.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <math.h>
#include <SOIL/soil.h>
#include <stdio.h>
#include "shader.h"
#include "main.h"


#define RENDER_WIDTH 640.0
#define RENDER_HEIGHT 480.0
#define SHADOW_MAP_RATIO 2
//iphone class: 594CR

//Camera position
float p_camera[3] = {32,20,0};

//Camera lookAt
float l_camera[3] = {2,0,-10};

//Light position
float p_light[3] = {3,50,0};

//Light lookAt
float l_light[3] = {0,0,-5};


//Light mouvement circle radius
float light_mvnt = 30.0f;

// Hold id of the framebuffer for light POV rendering
GLuint fboId;

// Z values will be rendered to this texture when using fboId framebuffer
GLuint depthTextureId;

// Use to activate/disable shadowShader
GLhandleARB shadowShaderId;
GLuint shadowMapUniform;
GLuint shaderProgram;
GLuint toonProgram;

float lastx, lasty;
float xrot = 0, yrot = 0;
float transX = 0.0;
float transZ = -5.0;
float points[45][45][3];    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;
GLuint tex = 0;
GLfloat hold;


// Loading shader function
GLhandleARB loadShader(char* filename, unsigned int type)
{
	FILE *pfile;
	GLhandleARB handle;
	const GLcharARB* files[1];
	
	// shader Compilation variable
	GLint result;				// Compilation code result
	GLint errorLoglength ;
	char* errorLogText;
	GLsizei actualErrorLogLength;
	
	char buffer[400000] = {0,400000};
	//memset(buffer,0,400000);
	
	// This will raise a warning on MS compiler
	pfile = fopen(filename, "rb");
	if(!pfile)
	{
		printf("Sorry, can't open file: '%s'.\n", filename);
		exit(0);
	}
	
	fread(buffer,sizeof(char),400000,pfile);
	//printf("%s\n",buffer);
	
	
	fclose(pfile);
	
	handle = glCreateShaderObjectARB(type);
	if (!handle)
	{
		//We have failed creating the vertex shader object.
		printf("Failed creating vertex shader object from file: %s.",filename);
		exit(0);
	}
	
	files[0] = (const GLcharARB*)buffer;
	glShaderSourceARB(
					  handle, //The handle to our shader
					  1, //The number of files.
					  files, //An array of const char * data, which represents the source code of theshaders
					  NULL);
	
	glCompileShaderARB(handle);
	
	//Compilation checking.
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	
	// If an error was detected.
	if (!result)
	{
		//We failed to compile.
		printf("Shader '%s' failed compilation.\n",filename);
		
		//Attempt to get the length of our error log.
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);
		
		//Create a buffer to read compilation error message
		errorLogText = (char*)malloc(sizeof(char) * errorLoglength);
		
		//Used to get the final length of the log.
		glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);
		
		// Display errors.
		printf("%s\n",errorLogText);
		
		// Free the buffer malloced earlier
		free(errorLogText);
	}
	
	return handle;
}

void loadShadowShader()
{
	GLhandleARB vertexShaderHandle;
	GLhandleARB fragmentShaderHandle;
	
	vertexShaderHandle   = loadShader("shadow.vert",GL_VERTEX_SHADER);
	fragmentShaderHandle = loadShader("shadow.frag",GL_FRAGMENT_SHADER);
	
	shadowShaderId = glCreateProgramObjectARB();
	
	glAttachObjectARB(shadowShaderId,vertexShaderHandle);
	glAttachObjectARB(shadowShaderId,fragmentShaderHandle);
	glLinkProgramARB(shadowShaderId);
	
	shadowMapUniform = glGetUniformLocationARB(shadowShaderId,"ShadowMap");
}

void generateShadowFBO()
{
	glewInit();
	int shadowMapWidth = RENDER_WIDTH * SHADOW_MAP_RATIO;
	int shadowMapHeight = RENDER_HEIGHT * SHADOW_MAP_RATIO;
	
	//GLfloat borderColor[4] = {0,0,0,0};
	
	GLenum FBOstatus;
	
	// Try to use a texture depth component
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	
	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	
	//glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );
	
	
	
	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available 
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	
	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, depthTextureId, 0);
	
	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
	
	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,RENDER_WIDTH/RENDER_HEIGHT,10,40000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
}


// This update only change the position of the light.
//int elapsedTimeCounter = 0;
void update(void)
{
	
	p_light[0] = light_mvnt * cos(glutGet(GLUT_ELAPSED_TIME)/1000.0);
	p_light[2] = light_mvnt * sin(glutGet(GLUT_ELAPSED_TIME)/1000.0);
	
	//p_light[0] = light_mvnt * cos(3652/1000.0);
	//p_light[2] = light_mvnt * sin(3652/1000.0);
}


void setTextureMatrix(void)
{
	static double modelView[16];
	static double projection[16];
	
	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5 
	// y = y* 0.5 + 0.5 
	// z = z* 0.5 + 0.5 
	// Moving from unit cube [-1,1] to [0,1]  
	const GLdouble bias[16] = {	
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0};
	
	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	
	glLoadIdentity();	
	glLoadMatrixd(bias);
	
	// concatating all matrice into one.
	glMultMatrixd (projection);
	glMultMatrixd (modelView);
	
	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}

// During translation, we also have to maintain the GL_TEXTURE8, used in the shadow shader
// to determine if a vertex is in the shadow.
void startTranslate(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(x,y,z);
}

void endTranslate()
{
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void setLighting(void) {
  float DiffuseLight[] = {1.0, 0.0, 0.0}; 
  float AmbientLight[] = {0.2, 0.2, 0.2};
  float SpecularLight[] = {1.0, 1.0, 1.0};
    
  glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
  glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
  glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
    
  float LightPosition[] = {
    p_light[1], p_light[0], p_light[2], 0.0  }; 

  glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); 
}

void setMaterial(void) {
   
    float mShininess[] = {5};
    float DiffuseMaterial[] = {1.0, 0.0, 0.0}; 
    float AmbientMaterial[] = {0.0, 0.0, 0.0};
    float SpecularMaterial[] = {1.0, 1.0, 1.0}; 
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, mShininess);


}

void setLighting2(void) {

  float DiffuseLight[] = {1.0, 1.0, 1.0}; 
  float AmbientLight[] = {0.2, 0.2, 0.2};
  float SpecularLight[] = {1.0, 1.0, 1.0};
    
  glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
  glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
  glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
    
  float LightPosition[] = {
    p_light[0], p_light[1], p_light[2], 0.0  }; 

  glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); 
}

void setMaterial2(void) {
    float mShininess[] = {5};
    float DiffuseMaterial[] = {0.66, 0.66, 0.66}; 
    float AmbientMaterial[] = {0.0, 0.0, 0.0};
    float SpecularMaterial[] = {1.0, 1.0, 1.0}; 
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, mShininess);

}

void setLighting3(void) {
  float DiffuseLight[] = {1.0, 1.0, 1.0}; 
  float AmbientLight[] = {0.2, 0.2, 0.2};
  float SpecularLight[] = {1.0, 1.0, 1.0};
    
  glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
  glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
  glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
    
  float LightPosition[] = {
    p_light[0], p_light[1], p_light[2], 0.0  }; 

  glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); 
}

void setMaterial3(void) {
   
    float mShininess[] = {15};
    float DiffuseMaterial[] = {0.85, 0.83, 0.125}; 
    float AmbientMaterial[] = {0.0, 0.0, 0.0};
    float SpecularMaterial[] = {1.0, 1.0, 1.0}; 
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, mShininess);
}

void drawObjects(void)
{

	int x, y;
	float float_x, float_y, float_xb, float_yb;

	glTranslatef(transZ, 0.0, -transX);
	glRotatef(xrot, 0.0, 1.0, 0.0);
	//glRotatef(-yrot, 0.0, 1.0, 0.0);

	// Ground
	//glColor4f(0.3f,0.3f,0.3f,1);
	//glEnable( GL_TEXTURE_2D );
	//glBindTexture( GL_TEXTURE_2D, tex );
	glBegin(GL_QUADS);
	//glTexCoord2f(0, 0);
	glColor4f(0.54f,0.27f,0.07f,1);
	glVertex3f(-35,-5,-35);
	//glTexCoord2f(1, 0);
	glColor4f(0.54f,0.27f,0.07f,1);
	glVertex3f(-35,-5, 15);
	//glTexCoord2f(1, 1);
	glColor4f(0.54f,0.27f,0.07f,1);
	glVertex3f( 15,-5, 15);
	//glTexCoord2f(0, 1);
	glColor4f(0.54f,0.27f,0.07f,1);
	glVertex3f( 15,-5,-35);
	
	//glTexCoord2f(0, 0);
	glColor4f(0.5f,0.5f,0.0f,1);
	glVertex3f(-35,12,-35);
	//glTexCoord2f(1, 0);
	glColor4f(0.5f,0.5f,0.0f,1);
	glVertex3f(-35,12, 15);
	glColor4f(0.5f,0.5f,0.0f,1);
	//glTexCoord2f(1, 1);
	glColor4f(0.5f,0.5f,0.0f,1);
	glVertex3f( -35,-5, 15);
	//glTexCoord2f(0, 1);
	glColor4f(0.5f,0.5f,0.0f,1);
	glVertex3f( -35,-5,-35);

	glEnd();

	glPushMatrix();
	glTranslatef(0,-2.5,-16);
	glColor4f(0.9f,0.9f,0.9f,1);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(0,-2.5,-16);
	glutSolidCube(4);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPushMatrix();
	glUseProgram(shaderProgram);
	setLighting();
	setMaterial();
	glTranslatef(-20,-2.5,-20);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(-20,-2.5,-20);
	glutSolidCube(4);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	//endTranslate();
	glUseProgram(0);
	
	glPushMatrix();
	glUseProgram(shaderProgram);
	setLighting();
	setMaterial();
	glTranslatef(0,0,-5);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(0,0,-5);
	glutSolidSphere(2,25,25);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	//endTranslate();
	glUseProgram(0);

	
	glPushMatrix();
	glUseProgram(shaderProgram);
	setLighting();
	setMaterial();
	glTranslatef(-20,7,-10);
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(-20,7,-10);
	glBegin(GL_QUADS);
	for( x = 0; x < 44; x++ )
	{
		for( y = 0; y < 44; y++ )
		{
			float_x = float(x)/44.0f;
			float_y = float(y)/44.0f;
			float_xb = float(x+1)/44.0f;
			float_yb = float(y+1)/44.0f;

			//glTexCoord2f( float_x, float_y);
			//glColor4f(1.0, 0.0, 0.0, 1.0);
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

//			glTexCoord2f( float_x, float_yb );
			//glColor4f(0.0, 1.0, 0.0, 1.0);
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

//			glTexCoord2f( float_xb, float_yb );
			//glColor4f(0.0, 0.0, 1.0, 1.0);
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

//			glTexCoord2f( float_xb, float_y );
			//glColor4f(1.0, 1.0, 0.0, 1.0);
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

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


	// Instead of calling glTranslatef, we need a custom function that also maintain the light matrix
	//startTranslate(0,10,-16);
	glUseProgram(0);

	glPushMatrix();
	glUseProgram(shaderProgram);
	setMaterial2();
	setLighting2();
	glTranslatef(-15.5,-5,-10);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(-15.5,-5,-10);
	GLUquadric *quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.75f, 0.75f, 17.0f, 10.0f, 10.0f);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glUseProgram(0);


	glPushMatrix();
	glUseProgram(shaderProgram);
	setMaterial3();
	setLighting3();
	glTranslatef(-15.5,12,-10);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(-15.5,12,-10);
	glutSolidSphere(1,25,25);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glUseProgram(0);	


	glPushMatrix();
	glUseProgram(toonProgram);
	setMaterial3();
	setLighting3();
	glTranslatef(-10,-2.5,-30);
	//glRotatef(-90, 1.0, 0.0, 0.0);
	//glRotatef(90, 0.0, 0.0, 1.0);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(-10,-5,-30);
	glutSolidTorus(1.5, 2.0, 10.0, 50.0);
	//glutSolidTeapot(4);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glUseProgram(0);
}



void renderScene(void) 
{
	update();

	
	//First step: Render from the light POV to a FBO, story depth values only
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);	//Rendering offscreen
	
	//Using the fixed pipeline to render to the depthbuffer
	glUseProgramObjectARB(0);
	
	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT* SHADOW_MAP_RATIO);
	
	// Clear previous frame values
	glClear( GL_DEPTH_BUFFER_BIT);
	
	//Disable color rendering, we only want to write to the Z-Buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
	
	setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);
	
	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	glCullFace(GL_FRONT);
	drawObjects();
	
	//Save modelview/projection matrice into texture7, also add a biais
	setTextureMatrix();
	
	
	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	
	glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);
	
	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
	
	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the shadow shader
	glUseProgramObjectARB(shadowShaderId);
	glUniform1iARB(shadowMapUniform,7);
	glActiveTextureARB(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,depthTextureId);
	
	setupMatrices(p_camera[0],p_camera[1],p_camera[2],l_camera[0],l_camera[1],l_camera[2]);
	
	glCullFace(GL_BACK);
	
	drawObjects();
	
	
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	
	if (key == 27) 
		exit(0);
}


void keyboard(unsigned char key, int x, int y) {
  printf("key pressed %c (%d) \n", key, key);

  switch(key) {
    case 'a':
      break;

    case 'b':
      break;

    case 'q': 
    case 27: //Escape key
      exit(0); 
  }
}

void keySpecial(int key, int x, int y) {  
  printf("special key pressed:< %c > \n", key);

  switch(key) {
    case GLUT_KEY_LEFT:
		transX += 0.5;
      break;
    case GLUT_KEY_RIGHT:
		transX -= 0.5;
      break;
    case GLUT_KEY_UP:
      transZ += 0.5;
      break;
    case GLUT_KEY_DOWN:
      transZ -= 0.5;
      break;
    case GLUT_KEY_HOME:
      break;
    case GLUT_KEY_END:
      break;
    case GLUT_KEY_PAGE_UP:
      break;
    case GLUT_KEY_PAGE_DOWN:
      break;
  }
}

void mouseMoved(int x, int y) {
  printf("mouse at %i/%i\n", x, y);

    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffx/2; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx/2;    //set the xrot to yrot with the addition of the difference in the x position
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


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(RENDER_WIDTH,RENDER_HEIGHT);
	glutCreateWindow("GLSL Shadow mapping");
	
	// This call will grab openGL extension function pointers.
	// This is not necessary for macosx and linux
	generateShadowFBO();
	loadShadowShader();
	
	
	// This is important, if not here, FBO's depthbuffer won't be populated.
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);
	
	glEnable(GL_CULL_FACE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	
	
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keySpecial);
	glutPassiveMotionFunc(mouseMoved);
	//glutKeyboardFunc(processNormalKeys);
	
	shaderProgram = initializeShaders("perPixel.vert", "perPixel.frag");
	toonProgram = initializeShaders("toon.vert", "toon.frag");
	//loadTextures();
	static GLfloat angle = 0.0;
	for(int x=0; x<45; x++)
	{
		
		for(int y=0; y<45; y++)
		{
			angle = x * 2.0 * 3.14159265 / 45;
			points[x][y][0]=float((x/5.0f)-4.5f);
			points[x][y][1]=float((y/5.0f)-4.5f);
			points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
		}
	}

	glutMainLoop();
}