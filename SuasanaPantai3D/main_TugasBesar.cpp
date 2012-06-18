/*Tugas Besar(UAS) Kelompok 7 IF-13, Komputer Grafika.
Anggota Kelompok:
        1. Benny Murwanto - 10108625
        2. Arief Firmansyah - 10108607
        3. Devi Eka Nainggolan - 10108622
        4. Moch. Dendi Sukandi Yahya - 10107413
*/

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <string.h>
#include <conio2.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "island.h"

char 			title[] = "Suasana Pantai 3D", 
				cmdvol[]="setaudio mySound volume to ", 
				cmdvolvar[]="";
unsigned int 	windowWidth= 640, 
				windowHeight= 480, 
				windowPosX= 50, 
				windowPosY= 50, 
				vol= 300;
bool 			fullScreenMode= true, kedepan=false;
float 			sudutputar= 0.0;
unsigned int  	texture_id, langit1, langit2, langit3, laut1, laut2, pasir;
static GLfloat lmRot = 0.5f; //rotasi lampu mercusuar
static GLfloat klRot = -5.0; //rotasi kapal laut
//layar kapal laut
GLint nNumPoints = 3;
GLfloat ctrlPoints[3][3][3]= {{{  -4.0f, 0.0f, 4.0f},	
						      { -2.0f, 4.0f, 4.0f},	
							  {  4.0f, 0.0f, 4.0f }},
							  {{  -4.0f, 0.0f, 0.0f},	
						      { -2.0f, 4.0f, 0.0f},	
							  {  4.0f, 0.0f, 0.0f }},
							  {{  -4.0f, 0.0f, -4.0f},	
						      { -2.0f, 4.0f, -4.0f},	
							  {  4.0f, 0.0f, -4.0f }}};   
							  
// Load texture
int load_texture(char *file_name, unsigned int width, unsigned int height){
	GLubyte *imgbitmap;
	FILE    *file;
	unsigned short int  depth=3;
    if ((file = fopen(file_name, "rb"))==NULL){
		printf("File tidak ditemukan: %s!\n",file_name);
		getch(); exit(1);
    } 
    imgbitmap = (GLubyte *) malloc (width * height * depth * (sizeof(GLubyte)));
    if (imgbitmap == NULL){
        printf("Alokasi textures ke Memory gagal!\n");
        fclose(file);
        getch(); exit(1);
    } 
	fread(imgbitmap, width * height * depth, 1, file);
	fclose(file);
	
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, width, height, GL_BGR, GL_UNSIGNED_BYTE, imgbitmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imgbitmap);
	free(imgbitmap);

	return texture_id;
}

// Load all image
void load_image(){
   langit1 = load_texture("bmp/sky0.bmp", 256, 256);
   langit2 = load_texture("bmp/sky1.bmp", 256, 256);
   langit3 = load_texture("bmp/sky2.bmp", 500, 500);
   laut1 = load_texture("bmp/water2.bmp", 256, 256);
   printf("Inisialisasi textures berhasil.\n");
}

// Load background theme song
void load_sound(){
   FILE *file;
   if((file = fopen("bg2.mp3", "r"))==NULL){
       printf("File tidak ditemukan: bg.mp3!\n");
       getch(); exit(1);
   }
   else printf("Inisialisasi audio berhasil.\n");
   fclose(file);
   
   strcat(cmdvol, itoa(vol, cmdvolvar, 40));
   mciSendString("open bg2.mp3 type mpegvideo alias mySound", NULL, 0, 0); 
   mciSendString("play mySound repeat", NULL, 0, 0);
   mciSendString(cmdvol, NULL, 0, 0);
}

// Initialize OpenGL
void init() {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClearDepth(100.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER, 0);
   
   load_image();
   load_sound();
   printf("\nJalankan aplikasi.\n");
}

// Draw landscape - laut 
void draw_laut(){
	float xleft=-13, y=-1.0, zfar=8.0, xright=11, znear=-20.0, xl, zf, incre=4.0;
	zf=zfar;
	
	while (zfar>=znear){
		xl=xleft;
		while (xl<=xright){
			//------------------------ Draw alas
			glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, laut1);
            if(xl==xleft){
                glPushMatrix();
                glBegin(GL_QUADS); // Left
					glTexCoord2f(1.0f, 1.0f); glVertex3f(xl, y, zfar);                  // Top Right
					glTexCoord2f(1.0f, 0.0f); glVertex3f(xl, y-(incre/4), zfar);	    // Top Left
					glTexCoord2f(0.0f, 0.0f); glVertex3f(xl, y-(incre/4), zfar-incre);  // Bottom Left
					glTexCoord2f(0.0f, 1.0f); glVertex3f(xl, y, zfar-incre);	        // Bottom Right
				glEnd();
				glPopMatrix();
			}
			if(zfar==zf){
				glPushMatrix();
                glBegin(GL_QUADS); // Top
					glTexCoord2f(1.0f, 0.0f); glVertex3f(xl, y, zfar);	                  // Top Left
					glTexCoord2f(1.0f, 1.0f); glVertex3f(xl+incre, y, zfar);	          // Top Right
					glTexCoord2f(0.0f, 1.0f); glVertex3f(xl+incre, y-(incre/4), zfar);    // Bottom Right
					glTexCoord2f(0.0f, 0.0f); glVertex3f(xl, y-(incre/4), zfar);	      // Bottom Left
				glEnd();
				glPopMatrix();
			}
			if(xl==xright){
                glPushMatrix();
                glBegin(GL_QUADS); // Right
					glTexCoord2f(1.0f, 1.0f); glVertex3f(xl+incre, y-(incre/4), zfar);       // Top Right
					glTexCoord2f(1.0f, 0.0f); glVertex3f(xl+incre, y, zfar);	             // Top Left
					glTexCoord2f(0.0f, 0.0f); glVertex3f(xl+incre, y, zfar-incre);           // Bottom Left
					glTexCoord2f(0.0f, 1.0f); glVertex3f(xl+incre, y-(incre/4), zfar-incre); // Bottom Right
				glEnd();
				glPopMatrix();
			}
			if(zfar==znear){
				glPushMatrix();
                glBegin(GL_QUADS); // Bottom
					glTexCoord2f(1.0f, 0.0f); glVertex3f(xl, y, zfar-incre);					// Top Left
					glTexCoord2f(1.0f, 1.0f); glVertex3f(xl+incre, y, zfar-incre);				// Top Right
					glTexCoord2f(0.0f, 1.0f); glVertex3f(xl+incre, y-(incre/4), zfar-incre);	// Bottom Right
					glTexCoord2f(0.0f, 0.0f); glVertex3f(xl, y-(incre/4), zfar-incre);			// Bottom Left
				glEnd();
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
			
			//------------------------ Draw laut
			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, laut1);
            glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(xl, y, zfar);	            // Top Left
				glTexCoord2f(1.0f, 1.0f); glVertex3f(xl+incre, y, zfar);	    // Top Right
				glTexCoord2f(0.0f, 1.0f); glVertex3f(xl+incre, y, zfar-incre);  // Bottom Right
				glTexCoord2f(0.0f, 0.0f); glVertex3f(xl, y, zfar-incre);	    // Bottom Left
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			xl+=incre;
		}
		zfar-=incre;
	}
}

// Draw langit
void draw_langit(){
	glPushMatrix();
	glColor3ub(125, 125, 125);
    	glEnable(GL_TEXTURE_2D);        
    	glBindTexture(GL_TEXTURE_2D, langit3);
        	GLUquadricObj* q = gluNewQuadric();							
            gluQuadricDrawStyle(q, GLU_FILL);							
            gluQuadricNormals(q, GLU_SMOOTH);
            gluQuadricTexture(q, GL_TRUE);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glScalef(16.0, 16.0, 16.0);
            gluSphere(q, 1.0, 30, 30);
        glDisable(GL_TEXTURE_2D);
	glPopMatrix();     
}

// Draw Pulau
void draw_pulau(){
   glPushMatrix();
   GLfloat pos[4]={5.0,18.0,-15.0,70.0};
   glLightfv(GL_LIGHT0, GL_POSITION, pos);  
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   //Alas 1  
   glPushMatrix();
    glTranslatef(-3.0, -0.8, 5.0);
    glScalef(6.0, 0.3, 5.0);
    glColor3ub(101, 49, 10);
    glutSolidCube(1.0);
   glPopMatrix();
   //Alas 2
   glPushMatrix();
    glTranslatef(-3.0, -0.5, 5.0);
    glScalef(5.5, 0.3, 5.0);
    glColor3ub(236, 155, 74);
    glutSolidCube(1.0);
   glPopMatrix();
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHTING);
   glPopMatrix();
}

void draw_mercusuar(){	
	GLUquadricObj *pObj; // Quadric object
	pObj =gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);
	glPushMatrix();
	glTranslatef(-3.0, 3.7, 5.0);
	glScalef(0.19, 0.19, 0.19);
	//uppest part
	glPushMatrix();
	glTranslatef(0.0f, 2.50f, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3ub(100, 100, 100);
	gluCylinder(pObj, 2.0f, 0.0f, 2.0f, 26, 1);
	gluDisk(pObj, 0.0f, 2.0f, 26, 13);
	glPopMatrix();
	//lamp
	glPushMatrix();
	glRotatef(lmRot++, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 1.50f, 0.0f);
	glColor3ub(0, 0, 0);
	glutSolidCube(2);
	glTranslatef(0.0f, 0.0f, 1.0f);
	// Specify new position and direction in rotated coords.
	glColor3ub(255,255,0);
	gluSphere(pObj, 1.0f, 26, 13);
	glPopMatrix();
	//upper cube
	glPushMatrix();
	glScalef(1.0f, 0.4f, 1.0f);
	glColor3ub(100, 100, 100);
	glutSolidCube(3.5);
	glPopMatrix();
	//cylinder part
	glPushMatrix();
	glTranslatef(0.0f, -20.0f, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3ub(0, 0, 0);
    gluCylinder(pObj, 2.50f, 1.50f, 20.0f, 26, 1);
	glPopMatrix();
	//lowest part
	glPushMatrix();
	glTranslatef(0.0f, -20.0f, 0.0f);
	glScalef(1.0f, 0.4f, 1.0f);
	glColor3ub(100, 100, 100);
    glutSolidCube(5);
	glPopMatrix();
	glPopMatrix();
}
	
void draw_kapal_laut(){   
	GLUquadricObj *pObj; // Quadric object
	pObj =gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);
	
	M3DVector3f vCorners[11] = { { 0.0f, 0.0f, 0.0f },// 0 //for the layar
                              { 3.0f, -5.0f, 0.0f },   // 1
                              { -3.0f, -5.0f, 0.0f }, // 2 
							  {20.0f, 20.0f, 0.0f}, // 3 //for the sea
							  {20.0f, -20.0f, 0.0f}, //4
							  {-20.0f, 20.0f, 0.0f}, //5
							  {-20.0f, -20.0f, 0.0f},//6
							  {10.0f, 10.0f, 2.0f},//7 //for the pic
							  {10.0f, 10.0f, -2.0f},//8
							  {10.0f, 14.0f, -2.0f},//9
							  {10.0f, 14.0f, 2.0f}//10
							  };

	glPushMatrix();
	glRotatef(90, 0.4, 12.0, 0.0);
	glTranslatef(klRot++/45, -1.0, 0.8);
    glScalef(0.15, 0.15, 0.15);
	glPushMatrix();//first
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glColor3ub(	238,59,59);
	glScalef(3.0, 1.0f, 1.0f );
	gluCylinder(pObj, 2.0f, 4.0f, 3.0f, 26, 1);
	gluCylinder(pObj, 0.0f, 2.0f, 0.10f, 26, 1);
	glColor3ub(47, 47, 79);
	glTranslatef(0.0f, 0.0f, 3.0f);
	gluCylinder(pObj, 4.0f, 5.0f, 5.0f, 26, 1);
	gluDisk(pObj, 0.0f, 4.0f, 26, 13);
	glPopMatrix();//first
	glPushMatrix();//second
	glColor3ub(139, 69, 19);
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(10.0f, 0.0f, 3.0f); //first stick
	gluCylinder(pObj, 0.30f, 0.30f, 14.0f, 26, 1);
	glTranslatef(0.0f, 0.0f, 14.0f);
	gluDisk(pObj, 0.0f, 0.30f, 26, 13);
	glTranslatef(-10.0f, 0.0f, -14.0f); //second stick
	gluCylinder(pObj, 0.30f, 0.30f, 18.0f, 26, 1);
	glTranslatef(0.0f, 0.0f, 18.0f);
	gluDisk(pObj, 0.0f, 0.30f, 26, 13);
	glTranslatef(-10.0f, 0.0f, -17.0f); //third stick
	gluCylinder(pObj, 0.30f, 0.30f, 20.0f, 26, 1);
	glTranslatef(0.0f, 0.0f, 20.0f);
	gluDisk(pObj, 0.0f, 0.30f, 26, 13);
	glPopMatrix();//second
	glPushMatrix();//third
	glColor3ub(0, 0, 0);
	glTranslatef(-0.40f, 15.0f, 0.0f);
	glScalef(1.50f, 1.30f, 1.0f);
	glRotatef(-180, 1.0f, 1.0f, 0.0f);
	// Sets up the bezier
	// This actually only needs to be called once and could go in
	// the setup function
	glMap2f(GL_MAP2_VERTEX_3,	// Type of data generated
	0.0f,						// Lower u range
	10.0f,						// Upper u range
	3,							// Distance between points in the data
	3,							// Dimension in u direction (order)
	0.0f,						// Lover v range
	10.0f,						// Upper v range
	9,							// Distance between points in the data
	3,							// Dimension in v direction (order)
	&ctrlPoints[0][0][0]);		// array of control points

	// Enable the evaluator
	// Map a grid of 10 points from 0 to 10
	glMapGrid2f(10,0.0f,10.0f,10,0.0f,10.0f);
	// Evaluate the grid, using lines
	glEvalMesh2(GL_FILL,0,10,0,10);
	glPopMatrix();//third
	glPushMatrix();//fourth
	glTranslatef(9.70f, 15.0f, 0.0f);
	glScalef(1.50f, 1.5f, 1.0f);
	glRotatef(-180, 1.0f, 1.0f, 0.0f);
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 10.0f, 3, 3, 0.0f, 10.0f, 9,3, &ctrlPoints[0][0][0]);		
	glMapGrid2f(10,0.0f,10.0f,10,0.0f,10.0f);
	glEvalMesh2(GL_FILL,0,10,0,10);
	glPopMatrix();//fourth
	glPushMatrix();//fifth
	glTranslatef(-10.50f, 13.0f, 0.0f);
	glScalef(1.30f, 1.0f, 1.0f);
	glRotatef(-180, 1.0f, 1.0f, 0.0f);
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 10.0f, 3, 3, 0.0f, 10.0f, 9,3, &ctrlPoints[0][0][0]);		
	glMapGrid2f(10,0.0f,10.0f,10,0.0f,10.0f);
	glEvalMesh2(GL_FILL,0,10,0,10);
	glPopMatrix();//fifth
	glPushMatrix();//sixth
	glTranslatef(0.30f, 10.0f, 0.0f);
	glColor3ub(255, 255, 255);
	glPopMatrix();//sixth
	glPopMatrix();
	
	if(klRot>650) klRot=-350;
}

void draw_matahari(){		
	glPushMatrix();
	//glTranslatef(-3.0, -0.8, 5.0);
	glTranslatef(7.0, 4.0, -10.0);
	glScalef(0.15, 0.15, 0.15);
	glColor3ub(255, 147, 0);
	glutSolidSphere(5.0f, 13, 26);
	glPopMatrix();
}

// Handler for window paint and re-paint event
void display(void) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   GLfloat pos0[4]        ={5.0, 5.0, 5.0, 0.0};
   GLfloat ambient[4]    = { 0.0, 0.0, 1.0, 1.0 };
   GLfloat diffuse[4]    = { 1.0, 2.0, 1.0, 1.0 };
   GLfloat specular[4]   = { 1.0, 1.0, 0.0, 1.0 };
   GLfloat shininess[1]  = { 10.0 };
   
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, pos0);
   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE, GL_ONE);
   GLfloat kabut[4]={0.93,0.95,0.92,0.88};
   glFogfv(GL_FOG_COLOR, kabut);
   glFogf(GL_FOG_DENSITY, 0.2f);
   //glFogf(GL_FOG_START, 3.0f);
   //glFogf(GL_FOG_END, -3.0f);
   glFogi(GL_FOG_MODE, GL_EXP);
   glEnable(GL_FOG);
   glDisable(GL_BLEND);
   
   glLoadIdentity();
   glRotatef(sudutputar, 0.0f, 1.0f, 0.0f); // Rotate over y-axis
   
   draw_laut();
   draw_langit();
   draw_pulau();
   draw_mercusuar();
   draw_kapal_laut();
   draw_matahari();
   
   glutSwapBuffers();
   sudutputar -= 0.1f;
   if(sudutputar<-360) sudutputar=360;
}

// Handler for window's re-size event
void reshape(GLsizei width, GLsizei height) {
   if (height == 0) height = 1;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   //glOrtho(-10,10,-10,10,-20,20);
   gluPerspective(40.0, (float)width / (float)height, 0.1, 100.0);
   gluLookAt(0.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
   
// Handler for key event
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27: exit(0); break; // ESC: exit the program
      default: break;
   }
}
   
// Handler for special-key event
void specialKey(int key, int x, int y) {
   char varvol[]="setaudio mySound volume to ";
   switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;
         if (fullScreenMode) {
            windowPosX   = glutGet(GLUT_WINDOW_X);
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
         } else {
            glutReshapeWindow(windowWidth, windowHeight);
            glutPositionWindow(windowPosX, windowPosX);
         }
         break;
      case GLUT_KEY_F2:
         if (vol>0){
             vol-=50;
             strcat(varvol, itoa(vol, cmdvolvar, 10));
             mciSendString(varvol, NULL, 0, 0);
         }
         break;
      case GLUT_KEY_F3:
         if (vol<1000){
            vol+=50;
            strcat(varvol, itoa(vol, cmdvolvar, 10));
            mciSendString(varvol, NULL, 0, 0);
         }
         break;
      case GLUT_KEY_F4:
           mciSendString("pause mySound", NULL, 0, 0);
         break;
      case GLUT_KEY_F5:
           mciSendString("resume mySound", NULL, 0, 0);
         break;
      default: break;
   }
}

// main function: GLUT runs as a console application
int main(int argc, char *argv[]) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(windowWidth, windowHeight);
   glutInitWindowPosition(windowPosX, windowPosY);
   glutCreateWindow(title);
   glutFullScreen();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKey);
   glutIdleFunc(display);
   init();
   glutMainLoop();
   return 0;
}
