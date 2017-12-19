// Cubica

#include <stdlib.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// the global Assimp scene object
const aiScene* scene = NULL;
GLuint scene_list = 0;
aiVector3D scene_min, scene_max, scene_center;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

GLfloat activar;

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0


#include "glm.h"

static GLfloat plano;
static GLfloat rPlano;
static GLfloat gPlano;
static GLfloat bPlano;
static GLfloat rBunny;
static GLfloat gBunny;
static GLfloat bBunny;

GLuint SkyboxTexture[6];	
static GLuint texNamePlano;
int iheightPlano, iwidthPlano;
unsigned char* imagePlano = NULL;

static GLuint texNameBunny;
int iheightBunny, iwidthBunny;
unsigned char* imageBunny = NULL;

static GLuint texPosX;
int iheighskypx, iwidthPX;
unsigned char* imgPositiveX = NULL;

static GLuint texNegX;
int iheighskynx, iwidthNX;
unsigned char* imgNegativeX = NULL;

static GLuint texPosY;
int iheighskypy, iwidthPY;
unsigned char* imgPositiveY = NULL;

static GLuint texNegY;
int iheighskyny, iwidthNY;
unsigned char* imgNegativeY = NULL;

static GLuint texPosZ;
int iheighskypz, iwidthPZ;
unsigned char* imgPositiveZ = NULL;

static GLuint texNegZ;
int iheighskynz, iwidthNZ;
unsigned char* imgNegativeZ = NULL;

//****************LINEAS DEL SKYBOX************************//
static GLuint SkyPosX;
int iheighskypx2, iwidthPX2;
unsigned char* img2PositiveX = NULL;
	
static GLuint SkyNegX;
int iheighskynx2, iwidthNX2;
unsigned char* img2NegativeX = NULL;

static GLuint SkyPosY;
int iheighskypy2, iwidthPY2;
unsigned char* img2PositiveY = NULL;
	
static GLuint SkyNegY;
int iheighskyny2, iwidthNY2;
unsigned char* img2NegativeY = NULL;

static GLuint SkyPosZ;
int iheighskypz2, iwidthPZ2;
unsigned char* img2PositiveZ = NULL;
	
static GLuint SkyNegZ;
int iheighskynz2, iwidthNZ2;
unsigned char* img2NegativeZ = NULL;

GLfloat angle = 0.0;

static GLuint skypx, skynx, skypy, skyny, skypz, skynz;
int iwidthskypx, iheightskypx, iwidthskypy, iheightskypy, iwidthskypz, iheightskypz, iwidthskynx, iheightskynx, iwidthskyny, iheightskyny, iwidthskynz, iheightskynz;
unsigned char* imgskypx = NULL;
unsigned char* imgskynx = NULL;
unsigned char* imgskypy = NULL;
unsigned char* imgskyny = NULL;
unsigned char* imgskypz = NULL;
unsigned char* imgskynz = NULL;

void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 3000.0);
   glMatrixMode (GL_MODELVIEW);

}

void drawPlane(float size)
{
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(1.0f * size,0.0f,1.0f * size);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(1.0f * size,0.0f,-1.0f * size);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-1.0f * size,0.0f,-1.0f * size);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(-1.0f * size,0.0f,1.0f * size);
	glEnd();
}

void create_cube_map(){
	
	//glEnable(GL_TEXTURE_CUBE_MAP);
	// Create and load the 6 textures of the cube map
	imgPositiveX = glmReadPPM("x_pos.ppm",&iwidthPX,&iheighskypx);
	imgNegativeX = glmReadPPM("x_neg.ppm",&iwidthNX,&iheighskynx);
	imgPositiveY = glmReadPPM("y_pos.ppm",&iwidthPY,&iheighskypy);
	imgNegativeY = glmReadPPM("y_neg.ppm",&iwidthNY,&iheighskyny);
	imgPositiveZ = glmReadPPM("z_pos.ppm",&iwidthPZ,&iheighskypz);
	imgNegativeZ = glmReadPPM("z_neg.ppm",&iwidthNZ,&iheighskynz);

	// Create the cube map textures, positive X
	glGenTextures(1,&texPosX);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X,texPosX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_RGB,iwidthPX,iheighskypx,1,GL_RGB,GL_UNSIGNED_BYTE,imgPositiveX);
	// Create the cube map textures, negative X
	glGenTextures(1,&texNegX);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,texNegX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,GL_RGB,iwidthNX,iheighskynx,1,GL_RGB,GL_UNSIGNED_BYTE,imgNegativeX);
	// Create the cube map textures, positive Y
	glGenTextures(1,&texPosY);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,texPosY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,GL_RGB,iwidthPY,iheighskypy,1,GL_RGB,GL_UNSIGNED_BYTE,imgPositiveY);
	// Create the cube map textures, negative Y
	glGenTextures(1,&texNegY);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,texNegY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,GL_RGB,iwidthNY,iheighskyny,1,GL_RGB,GL_UNSIGNED_BYTE,imgNegativeY);
	// Create the cube map textures, positive Z
	glGenTextures(1,&texPosZ);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,texPosZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,GL_RGB,iwidthPZ,iheighskypz,1,GL_RGB,GL_UNSIGNED_BYTE,imgPositiveZ);
	// Create the cube map textures, negative Z
	glGenTextures(1,&texNegZ);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,texNegZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,GL_RGB,iwidthNZ,iheighskynz,1,GL_RGB,GL_UNSIGNED_BYTE,imgNegativeZ);
	
	// Sets the texture's behavior for wrapping (optional)
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
	// Sets the texture's max/min filters
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

}


void skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	glColor4f(1.0,1.0,1.0,1.0);
	

	img2PositiveX = glmReadPPM("x_pos.ppm",&iwidthPX2,&iheighskypx2);
	img2NegativeX = glmReadPPM("x_neg.ppm",&iwidthNX2,&iheighskynx2);
	img2PositiveY = glmReadPPM("y_pos.ppm",&iwidthPY2,&iheighskypy2);
	img2NegativeY = glmReadPPM("y_neg.ppm",&iwidthNY2,&iheighskyny2);
	img2PositiveZ = glmReadPPM("z_pos.ppm",&iwidthPZ2,&iheighskypz2);
	img2NegativeZ = glmReadPPM("z_neg.ppm",&iwidthNZ2,&iheighskynz2);
	
	//SKYBOX

	glGenTextures(1,&SkyboxTexture[SkyPosX]);
	glBindTexture(GL_TEXTURE_2D,SkyPosX);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iwidthPX2,iheighskypx2,1,GL_RGB,GL_UNSIGNED_BYTE,img2PositiveX);

	glGenTextures(1,&SkyboxTexture[SkyNegX]);
	glBindTexture(GL_TEXTURE_2D,SkyNegX);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iwidthNX2,iheighskynx2,1,GL_RGB,GL_UNSIGNED_BYTE,img2NegativeX);

	glGenTextures(1,&SkyboxTexture[SkyPosY]);
	glBindTexture(GL_TEXTURE_2D,SkyPosY);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iwidthPY2,iheighskypy2,1,GL_RGB,GL_UNSIGNED_BYTE,img2PositiveY);

	glGenTextures(1,&SkyboxTexture[SkyNegY]);
	glBindTexture(GL_TEXTURE_2D,SkyNegY);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iwidthNY2,iheighskyny2,1,GL_RGB,GL_UNSIGNED_BYTE,img2NegativeY);

	glGenTextures(1,&SkyboxTexture[SkyPosZ]);
	glBindTexture(GL_TEXTURE_2D,SkyPosZ);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iwidthPZ2,iheighskypz2,1,GL_RGB,GL_UNSIGNED_BYTE,img2PositiveZ);

	glGenTextures(1,&SkyboxTexture[SkyNegZ]);
	glBindTexture(GL_TEXTURE_2D, SkyNegZ);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iwidthNZ2,iheighskynz2,1,GL_RGB,GL_UNSIGNED_BYTE,img2NegativeZ);


/////////////////////////////////////////CIERRA SKYBOX//////////////////////////////////////////////////////////
	
	
	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SkyPosX]);
	glBegin(GL_QUADS);	
		glTexCoord3f(1.0, 0.0f,0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord3f(1.0, 1.0f,0.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord3f(0.0, 1.0f,0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord3f(0.0f, 0.0f,0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SkyNegX]);
	glBegin(GL_QUADS);		
		glTexCoord3f(1.0f, 0.0f,0.0f); glVertex3f(x+width, y,		z);
		glTexCoord3f(1.0f, 1.0f,0.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord3f(0.0f, 1.0f,0.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(x,		  y,		z);

	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SkyPosY]);
	glBegin(GL_QUADS);		
		glTexCoord3f(1.0f, 0.0f,0.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord3f(1.0f, 1.0f,0.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord3f(0.0f, 1.0f,0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord3f(0.0f, 0.0f,0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SkyNegY]);
	glBegin(GL_QUADS);		
		glTexCoord3f(0.0f, 1.0f,0.0f); glVertex3f(x+width, y,		z);
		glTexCoord3f(0.0f, 0.0f,0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord3f(1.0f, 0.0f,0.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord3f(1.0f, 1.0f,0.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SkyPosZ]);
	glBegin(GL_QUADS);		
		glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SkyNegZ]);
	glBegin(GL_QUADS);		
		glTexCoord3f(0.0f, 0.0f,0.0f); glVertex3f(x,		  y,		z);
		glTexCoord3f(1.0f, 0.0f,0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord3f(1.0f, 1.0f,0.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord3f(0.0f, 1.0f,0.0f); glVertex3f(x+width, y,		z);
	glEnd();

}



void init_surface() {

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);


   glGenTextures(1, &texNamePlano);
   glBindTexture(GL_TEXTURE_2D, texNamePlano);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   imagePlano = glmReadPPM("piso.ppm", &iwidthPlano, &iheightPlano);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthPlano, iheightPlano, 0, GL_RGB, GL_UNSIGNED_BYTE, imagePlano);
	

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);


   glGenTextures(1, &texNameBunny);
   glBindTexture(GL_TEXTURE_2D, texNameBunny);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   imageBunny = glmReadPPM("bunny.ppm", &iwidthBunny, &iheightBunny);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthBunny, iheightBunny, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBunny);

 glGenTextures(1, &skypx);
   glBindTexture(GL_TEXTURE_2D, skypx);

   imgskypx = glmReadPPM("x_pos.ppm", &iwidthskypx, &iheightskypx);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthskypx, iheightskypx, 0, GL_RGB, GL_UNSIGNED_BYTE, imgskypx);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glGenTextures(1, &skynx);
   glBindTexture(GL_TEXTURE_2D, skynx);

   imgskynx = glmReadPPM("x_neg.ppm", &iwidthskynx, &iheightskynx);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthskynx, iheightskynx, 0, GL_RGB, GL_UNSIGNED_BYTE, imgskynx);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glGenTextures(1, &skypy);
   glBindTexture(GL_TEXTURE_2D, skypy);

   imgskypy = glmReadPPM("y_pos.ppm", &iwidthskypy, &iheightskypy);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthskypy, iheightskypy, 0, GL_RGB, GL_UNSIGNED_BYTE, imgskypy);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glGenTextures(1, &skyny);
   glBindTexture(GL_TEXTURE_2D, skyny);

   imgskyny = glmReadPPM("y_neg.ppm", &iwidthskyny, &iheightskyny);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthskyny, iheightskyny, 0, GL_RGB, GL_UNSIGNED_BYTE, imgskyny);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glGenTextures(1, &skypz);
   glBindTexture(GL_TEXTURE_2D, skypz);

   imgskypz = glmReadPPM("z_pos.ppm", &iwidthskypz, &iheightskypz);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthskypz, iheightskypz, 0, GL_RGB, GL_UNSIGNED_BYTE, imgskypz);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glGenTextures(1, &skynz);
   glBindTexture(GL_TEXTURE_2D, skynz);

   imgskynz = glmReadPPM("z_neg.ppm", &iwidthskynz, &iheightskynz);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthskynz, iheightskynz, 0, GL_RGB, GL_UNSIGNED_BYTE, imgskynz);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	

}

void init(){

	  
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   rBunny = 1.0;
   gBunny = 1.0;
   bBunny = 1.0;
   rPlano = 1.0;
   gPlano = 1.0;
   bPlano = 1.0;
   activar = 0;
   
   create_cube_map();
 
   glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
   glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
   glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);

   init_surface();
}



void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
	case 27:             
		exit (0);
		break;

	case 'Q':
    case 'q':
		rPlano += 0.1;
		printf("Color Rojo del Plano %f (+) \n", rPlano);
		break;

	case 'W':
	case 'w':
		rPlano -= 0.1;
		printf("Color Rojo del Plano %f (-) \n", rPlano);
		break;

    case 'A':
    case 'a':
		gPlano += 0.1;
		printf("Color Verde del Plano %f (+)\n",gPlano);
		break;

	case 'S':
    case 's':
		gPlano -= 0.1;
		printf("Color Verde del Plano (-) %f \n",gPlano);
		break;

	case 'Z':
	case 'z':
		bPlano += 0.1;
		printf("Color Azul del Plano %f (+)\n", bPlano);
		break;

	case 'X':
	case 'x':
		bPlano -= 0.1;
		printf("Color Azul del Plano %f (-)\n", bPlano);
		break;

	case 'R':
    case 'r':
		rBunny += 0.1;
		printf("Color Rojo del conejo %f (+)\n", rBunny);
		break;

	case 'T':
	case 't':
	rBunny -= 0.1;
		printf("Color Rojo del conejo %f (-)\n", rBunny);
		break;

	case 'F':
	case 'f':
		gBunny += 0.1;
		printf("Color Verde del conejo %f (+)\n", gBunny);
		break;

	case 'G':
	case 'g':
		gBunny -= 0.1;
		printf("Decrementa el Color Azul del conejo %f \n", gBunny);
		break;

	case 'V':
	case 'v':
		bBunny += 0.1;
		printf("Aumenta el Color Azul del conejo %f \n", bBunny);
		break;

	case 'B':
	case 'b':
		bBunny -= 0.1;
		printf("Decrementa el Color Azul del conejo %f  \n", bBunny);
		break;

	case 'E':
	case 'e':
		activar = 1;
        break;

	case 'D':
	case 'd':
		activar = 0;
		break;

  }

  glutPostRedisplay();
}

void recursive_render (const aiScene *sc, const aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		//apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			//glDisable(GL_LIGHTING);
		} else {
			//glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				
				if (mesh->HasTextureCoords(0)) 
					glTexCoord2f(mesh->mTextureCoords[0][index].x, 1-mesh->mTextureCoords[0][index].y);
				
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}



void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (42, 60, 148, 0.0, 25.0, 0.0, 0.0, 1.0, 0.0);

	
	// Render Grid 
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glPushMatrix();
	glRotatef(90,1.0,0.0,0.0);
    glColor3f( 0.0, 0.7, 0.7 );
    glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	{
	xLocal = DEF_floorGridScale * loopX;
	glVertex3f( xLocal, -zExtent, 0.0 );
	glVertex3f( xLocal, zExtent,  0.0 );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	{
	zLocal = DEF_floorGridScale * loopZ;
	glVertex3f( -xExtent, zLocal, 0.0 );
	glVertex3f(  xExtent, zLocal, 0.0 );
	}
    glEnd();
	ejesCoordenada();
    glPopMatrix();
	
	// Fin Grid
	/*Para el Skybox*/

	/*
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
	*/

	

	/*
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	glPushMatrix;

	//skybox(0,0,0,200,200,200);
	
	glPopMatrix;
	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
	*/
	
	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );


   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, texNamePlano);

   //Colores del plano
   glColor4f(rPlano,gPlano,bPlano,1.0);

	glPushMatrix();
	glBegin(GL_QUADS);
		glNormal3f(0.0,1.0,0.0);glTexCoord2f(1.0,0.0);glVertex3f(337.494,0.0,337.494);
		glNormal3f(0.0,1.0,0.0);glTexCoord2f(1.0,1.0);glVertex3f(337.494,0.0,-337.494);
		glNormal3f(0.0,1.0,0.0);glTexCoord2f(0.0,1.0);glVertex3f(-337.494,0.0,-337.494);
		glNormal3f(0.0,1.0,0.0);glTexCoord2f(0.0,0.0);glVertex3f(-337.494,0.0,337.494);
	glEnd();
	glPopMatrix();

	glPushMatrix();

	if (activar==1){
	
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_CUBE_MAP);
	
	}else{
	
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, texNameBunny);
   
	}

   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, texNameBunny);

   //Colores del Conejo
	glColor4f(rBunny,gBunny,bBunny,1.0);
	glEnable(GL_NORMALIZE);
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    recursive_render(scene, scene->mRootNode);
	    glEndList();
	}
	glCallList(scene_list);
	
	
	glPopMatrix();
	//Desactivar a la funcion display y el se desactiva


	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);


	//Aqui se llama el skybox
	//Lo escalo
	// glScalef(0.15, 0.2, 0.19);
	glScalef(5.0, 1.0, 5.0);
	//glScalef(5.0, 5.0, 5.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, skynx);
	glPushMatrix();
		glTranslatef(-300.0f,0.0f,0.0f);
		glRotatef(180.0f,0.0f,1.0f,0.0f);
		glRotatef(90.0f,0.0f,0.0f,1.0f);
		drawPlane(300);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, skynz);
	glPushMatrix();
		glTranslatef(0.0f,0.0f,-300.0f);
		glRotatef(90.0f,0.0f,0.0f,1.0f);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		drawPlane(300);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void animacion(int value) {
	
	glutTimerFunc(10,animacion,1);
    glutPostRedisplay();
	
}

void get_bounding_box_for_node (const aiNode* nd, 
	aiVector3D* min, 
	aiVector3D* max, 
	aiMatrix4x4* trafo
){
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	
		prev = *trafo;
		aiMultiplyMatrix4(trafo,&nd->mTransformation);

		for (; n < nd->mNumMeshes; ++n) {
			const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
				for (t = 0; t < mesh->mNumVertices; ++t) {

				aiVector3D tmp = mesh->mVertices[t];
				aiTransformVecByMatrix4(&tmp,trafo);

				min->x = aisgl_min(min->x,tmp.x);
				min->y = aisgl_min(min->y,tmp.y);
				min->z = aisgl_min(min->z,tmp.z);

				max->x = aisgl_max(max->x,tmp.x);
				max->y = aisgl_max(max->y,tmp.y);
				max->z = aisgl_max(max->z,tmp.z);
			}
		}

		for (n = 0; n < nd->mNumChildren; ++n) {
			get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
		}
		*trafo = prev;
	

}

void get_bounding_box (aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;

	
		aiIdentityMatrix4(&trafo);
	
		min->x = min->y = min->z =  1e10f;
		max->x = max->y = max->z = -1e10f;
		get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);


}

int loadasset (const char* path)
{
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	
		
		scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene) {
			get_bounding_box(&scene_min,&scene_max);
			scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
			scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
			scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
			return 0;
		}


	return 1;
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Test Opengl");


	aiLogStream stream;
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&stream);

	// the model name can be specified on the command line. If none
	// is specified, we try to locate one of the more expressive test 
	// models from the repository (/models-nonbsd may be missing in 
	// some distributions so we need a fallback from /models!).
	//if( 0 != loadasset( argc >= 2 ? argv[1] : "dragon_vrip_res2.ply")) {
	//	if( argc != 1 || (0 != loadasset( "dragon_vrip_res2.ply") && 0 != loadasset( "dragon_vrip_res2.ply"))) { 
	//		return -1;
	//	}
	//}

	if (loadasset( "bunnyusb.obj") != 0) {
		return -1;
	}




	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc (Keyboard);


	
	/*
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << "\n";
	
	if (GLEW_VERSION_1_5){
		std::cout << "Core extensions of OpenGL 1.1 to 1.5 are available!\n";
	}

	if (glewIsSupported("GL_ARB_shading_language_100")) {  
		int major, minor, revision;
		const GLubyte* sVersion = glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
		if (glGetError() == GL_INVALID_ENUM){
			major = 1; minor = 0; revision=51;
			std::cout << "No Shader\n";
		}else{
			// parse string sVersion to get major, minor, revision
			std::cout << "Shader habilitado\n";
		}
	}
	*/


	glutMainLoop();
	return 0;

}
