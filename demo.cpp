
/*
 * GL01GettingStarted.cpp
 * Drawing Simple 2D Shapes: quad, triangle, polygon.
 */
#include <gl/glut.h>   // also included gl.h, glu.h
   
void display() {
   glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
   
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(-0.7f, -0.1f);    // x, y
      glVertex2f(-0.1f, -0.1f);
      glVertex2f(-0.1f,  0.5f);
      glVertex2f(-0.7f,  0.5f);
   glEnd();
   
   glBegin(GL_TRIANGLES);          // Each set of 3 vertices form a triangle
      glColor3f(0.0f, 1.0f, 0.0f); // Green
      glVertex2f(0.2f, -0.3f);
      glVertex2f(0.8f, -0.3f);
      glVertex2f(0.5f,  0.2f);
   glEnd();
   
   glBegin(GL_POLYGON);            // The vertices form one closed polygon
      glColor3f(0.0f, 0.0f, 1.0f); // Blue
      glVertex2f(0.2f, 0.3f);
      glVertex2f(0.4f, 0.3f);
      glVertex2f(0.5f, 0.5f);
      glVertex2f(0.4f, 0.7f);
      glVertex2f(0.2f, 0.7f);
      glVertex2f(0.1f, 0.5f);
   glEnd();
   
   glFlush();  // Render now
}
   
// GLUT runs as a Console Application
int main(int argc, char** argv) {
   glutInit(&argc, argv);          // Initialize GLUT
   glutCreateWindow("2D Shapes");  // Create a window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint
   glutMainLoop();                 // Enter infinitely event-processing loop
   return 0;
}