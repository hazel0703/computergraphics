/***************************/
/* Adriana Devera 09-11286 */
/***************************/


// Cubica

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>



using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0
#define pi 3.14159265358979323846

GLfloat ctlpoints[21][21][3];
GLfloat knots[25]; //defino el vector de knots
GLUnurbsObj *theNurb;

void animacion(int);

GLenum animar = true;
int opcion;
//Normalizando los vectores

//Defino las variables utilizadas en la funcion
//Ola 1
GLfloat L= 8.0;
GLfloat A = 0.4;
GLfloat S = 2.0;
GLfloat Dx = 0.0;
GLfloat Dy = -1.0;
GLint modulo1 = sqrt(sqrt(Dx) + sqrt(Dy));
//GLfloat modulo2 = sqrt(sqrt(Dx2) + sqrt(Dy2));
GLfloat W = 2 * pi/L;
GLfloat t = 0.1;
GLfloat phi= S * 2 * pi/L;
GLfloat mod1= (Dx/modulo1);
//GLfloat mod2= (Dy/modulo1);


//Ola 2
GLfloat L2= 4.0;
GLfloat A2 = 0.0;
GLfloat S2 = 0.0;
GLfloat Dx2 = 1.0;
GLfloat Dy2 = 1.0;
GLfloat W2 = 2 * pi/L2;
GLfloat t2 = 0.1;
GLfloat phi2= S2 * 2 * pi/L2;



void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
   glMatrixMode (GL_MODELVIEW);

}

void init_surface() {
		/*inicializacion del vector knot, las primeras 4 se inicializan en 0 y las ultimas 4 se inicializan en 1*/
	knots[0]=0;
	knots[1]=0;
	knots[2]=0;
	knots[3]=0;
	knots[21]=1;
	knots[22]=1;
	knots[23]=1;
	knots[24]=1;

	for(int i=4; i<21; i++){
	knots[i]=((float)i-3)/18;

	}
}

void init(){

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);

   init_surface();

   theNurb = gluNewNurbsRenderer();
   gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 15.0);
   gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	

}

void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {

    case 'r':
		if(animar){
			animar==true;
			std::cout  << "*****Valores Iniciales Ola 1:*****" << "\n";
			std::cout  << "**********************************" << "\n";
			std::cout  << "Distancia:" << L <<  "\n";
			std::cout  << "Amplitud:" << A <<  "\n";
			std::cout  << "Velocidad (Speed): " << S <<  "\n";
			std::cout  << "Vector direccion eje X: " << Dx <<  "\n";
			std::cout  << "Vector direccion eje Y: " << Dy <<  "\n";
			std::cout  <<  "\n" <<  "\n";
			std::cout  << "*****Valores Iniciales Ola 2:*****" << "\n";
			std::cout  << "**********************************" << "\n";
			std::cout  << "Distancia:" << L2 <<  "\n";
			std::cout  << "Amplitud:" << A2 <<  "\n";
			std::cout  << "Velocidad (Speed):" << S2 <<  "\n";
			std::cout  << "Vector direccion eje X: " << Dx2 <<  "\n";
			std::cout  << "Vector direccion eje Y: " << Dy2 <<  "\n";
			std::cout  <<  "\n";
					glutTimerFunc(10,animacion,1);
		}else{
		   animar=false;
		}	
		break;
		if(animar==true){
    case '1': 
		    
			opcion = 1;
			std::cout  << "Opcion 1,Se encuentra en la Ola 1:" << "\n";
			std::cout  << "\n";
			animar=true;
			
			break;
	case '2': 
			opcion = 2;
			std::cout  << "Opcion 2,Se encuentra en la Ola 2:" << "\n";
			std::cout  <<  "\n";
			animar=true;
			break;
		}
    case 'A':
	case 'a':
			if (opcion == 1){
				std::cout  << "Decremento de la Distancia de la ola 1:" << L <<  "\n";
				L = L - 0.1;
			}else{
				
			   (opcion == 2);
			   std::cout  << "Decremento de la Distancia de la ola 2:" << L2 <<  "\n";
				L2 = L2 - 0.1;
			}
		break;

    case 'Z':
	case 'z':	
	    	if (opcion == 1){
				std::cout  << "Incremento de la Distancia de la ola 1:" << L <<  "\n";
				L = L + 0.1;
			}else{
			   (opcion == 2);
			   std::cout  << "Incremento de la Distancia de la ola 2:" << L2 <<  "\n";
				L2 = L2 + 0.1;
		}
	    break;

	case 'S':
	case 's':	
		if (opcion == 1){
        std::cout  << "Decremento de la Altura de la ola 1:" << A <<  "\n";
		A = A - 0.1;
		}else{
	    (opcion == 2);
		std::cout  << "Decremento de la Altura de la ola 2:" << A2 <<  "\n";
		A2 = A2 - 0.1;
		}
		break;

	case 'X':
	case 'x':	
		if (opcion == 1){
        std::cout  << "Incremento de la Altura de la ola 1:" << A <<  "\n";
		A = A + 0.1;
		}else{
	    (opcion == 2);
		std::cout  << "Incremento de la Altura de la ola 2:" << A2 <<  "\n";
		A2 = A2 + 0.1;
		}
		break;

	case 'D':
	case 'd':
		if (opcion == 1){
        std::cout  << "Decremento de Speed ola 1:" << S <<  "\n";
		S = S - 0.1;
		}else{
        (opcion == 2);
		std::cout  << "Decremento de Speed ola 2:" << S2 <<  "\n";
		S2 = S2 - 0.1;
		}
		break;
	case 'C':
	case 'c':	
		if (opcion == 1){
		std::cout  << "Incremento de Speed ola 1:" << S <<  "\n";
		S = S + 0.1;
		}else{
		(opcion == 2);
		std::cout  << "Incremento de Speed ola 2:" << S2 <<  "\n";

		S2 = S2 + 0.1;
		}
		break;

    case 'F':
	case 'f':	
		if (opcion == 1){
       std::cout  << "Cambio de direccion Eje X's (-) ola 1:" << Dx <<  "\n";
		Dx = Dx - 0.1;
		}else{
		(opcion == 2);
		std::cout  << "Cambio de direccion Eje X's (-) ola 2:" << Dx2 <<  "\n";
		Dx2 = Dx2 - 0.1;
		}
		break;

    case 'V':	
	case 'v':
		if (opcion == 1){
			std::cout  << "Cambio de direccion Eje X's (+) ola 1:" << Dx <<  "\n";
		Dx = Dx + 0.1;
		}else{
        (opcion == 2);
		std::cout  << "Cambio de direccion Eje X's (+) ola 2:" << Dx2 <<  "\n";
        Dx2 = Dx2 + 0.1;
		}
		break;

    case 'G':
	case 'g':	
		if (opcion == 1){
			std::cout  << "Cambio de direccion Eje Y (-) ola 1:" << Dy <<  "\n";
		Dy = Dy - 0.1;
		}else{
		(opcion == 2);
		std::cout  << "Cambio de direccion Eje Y (-) ola 2:" << Dy2 <<  "\n";
		Dy2 = Dy2 - 0.1;
		}
		break;
    case 'B':
	case 'b':
		if (opcion == 1){
			std::cout  << "Cambio de direccion Eje Y (+) ola 1:" << Dy <<  "\n";
		Dy = Dy + 0.1;
		}else{
		(opcion == 2);
		std::cout  << "Cambio de direccion Eje Y (+) ola 2:" << Dy <<  "\n";
		Dy2 = Dy2 + 0.1;
		}
		break;

		case 27:             
		exit (0);
		break;
 }
}




void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	// Luz y material

	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   

	//Suaviza las lineas
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

	
	

	glPushMatrix();
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 25, knots, 25, knots, 21 * 3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
 
	gluEndSurface(theNurb);
	glPopMatrix();
	
	

	/* Muestra los puntos de control */
	/*
		int i,j;
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		for (i = 0; i <21; i++) {
			for (j = 0; j < 21; j++) {
	            glVertex3f(ctlpoints[i][j][0], 	ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();
		*/
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}

void waves(){
GLint i, j;
	for (i = 0; i <21; i++) 
	{
	  for (j = 0; j < 21; j++) 
	  {
		  //Ecuacion utilizada para las olas 1 y 2
		  ctlpoints[i][j][1]= (A* sin((Dx * (ctlpoints[i][j][0]) + (Dy*(ctlpoints[i][j][2])) * W + t * (S * 2 * pi/L)))) + (A2* sin((Dx2 * (ctlpoints[i][j][0]) + (Dy2*(ctlpoints[i][j][2])) * W2 + t * (S2 * 2 * pi/L2))));
		//  ctlpoints[i][j][1]= ((A* sin(((Dx)* (ctlpoints[i][j][0]) + (Dy*(ctlpoints[i][j][2])) * W + t * (S * 2 * pi/L))))+ (A2* sin((Dx2 * (ctlpoints[i][j][0]) + (Dy2*(ctlpoints[i][j][2])) * W2 + t * (S2 * 2 * pi/L2)))));
		    
	  }
	}

}

void animacion(int value) {
	t += 0.1;
	waves();
	glutPostRedisplay();
   	glutTimerFunc(10,animacion,1);

}

int main (int argc, char** argv) {
	 	

	for(int i=0; i<21; i++)
	{
		for(int j=0; j<21; j++)
		{
		ctlpoints[i][j][0]=(j - 10);
		ctlpoints[i][j][1]=0;
		ctlpoints[i][j][2]=(i-10);

		}
	}

	/*Inicializacion de la variable nurbs*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(960,540);
	glutCreateWindow("Efecto Ola en Opengl Proyecto III Adriana Devera");

	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc (Keyboard);
		
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 0;

}
