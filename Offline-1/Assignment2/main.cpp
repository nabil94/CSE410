#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double theta;
double axisAngle ;
double distance = 0.0;
double dist;

struct point
{
	double x,y,z;
};

struct twoDVec{
    double a,b;
};

point wheelCentre,d,f;
twoDVec pos,v;

void initWheel(){
    axisAngle = 0.0;
    theta = 0.0;
    wheelCentre.x=wheelCentre.y=wheelCentre.z = 0;

}

twoDVec perpVec(twoDVec aa){
    twoDVec bb;
    bb.a = -aa.b;
    bb.b = aa.a;
    return bb;

}

point crossProduct(point a,point b){
    point c;
    c.x = (a.y*b.z) - (a.z*b.y);
    c.y = (a.z*b.x) - (a.x*b.z);
    c.z = (a.x*b.y) - (b.x*a.y);
    return c;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-32;i<=32;i++){

					//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -330, 0);
				glVertex3f(i*10,  330, 0);

				//lines parallel to X-axis
				glVertex3f(-330, i*10, 0);
				glVertex3f( 330, i*10, 0);
			}
		}glEnd();
	}
}

void drawWheel(double radius, double width, double x, double y, double z,int segments){
   // glPushMatrix();{
        //glTranslated(0,-width/2,0);
        //glRotated(90,1,0,0);
    //}
    //glPopMatrix();
    int i;
    struct point points[segments+1];
    double h = width;
    double shade;
    /*glRotatef(rotateAngle,0,1,0);
    glRotated(90,1,0,0);*/
    //glColor3f(0.7,0.6,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x = radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y = radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);
        //glTranslatef(radius,0,0);
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,z);
			glVertex3f(points[i+1].x,points[i+1].y,z);
        }
        glEnd();
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,z+h);
			glVertex3f(points[i+1].x,points[i+1].y,z+h);
        }
        glEnd();

            glBegin(GL_QUADS);
            {
                glVertex3f(points[i].x,points[i].y,z);
                glVertex3f(points[i+1].x,points[i+1].y,z);
                glVertex3f(points[i+1].x,points[i+1].y,z+h);
                glVertex3f(points[i].x,points[i].y,z+h);
            }
            glEnd();

    }

    int uu = segments/4;
    glColor3f(0.7,0.7,0.7);
    glBegin(GL_QUADS);
    {
        glVertex3f(points[uu-1].x,points[uu-1].y,z+(h/4));
        glVertex3f(points[uu-1].x,points[uu-1].y,z+(3*h)/4);
        glVertex3f(points[(3*uu)-1].x,points[(3*uu)-1].y,z+(3*h)/4);
        glVertex3f(points[(3*uu)-1].x,points[(3*uu)-1].y,z+(h/4));
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        glVertex3f(points[segments-1].x,points[segments-1].y,z+(h/4));
        glVertex3f(points[segments-1].x,points[segments-1].y,z+(3*h)/4);
        glVertex3f(points[(2*uu)-1].x,points[(2*uu)-1].y,z+(3*h)/4);
        glVertex3f(points[(2*uu)-1].x,points[(2*uu)-1].y,z+(h/4));
    }
    glEnd();




}

point qq(point p,double r){
    point q;
    q.x = r*p.x;
    q.y = r*p.y;
    q.z = r*p.z;
    return q;
}

void keyboardListener(unsigned char key, int x,int y){
    //double cc =
    //theta = 15*(acos(-1.0)/180.0) ;
    //twoDVec q = perpVec(v);
   /* double ff = sqrt(f.x*f.x + f.y*f.y + f.z*f.z);
    double dd = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
    point p,q;
    p = qq(f,ff);
    q = qq(d,dd);
    point perp = crossProduct(p,q);*/
    //twoDVec q = perpVec(v);
    double wheelAngle = theta*pi/180;
    double f = 2*pi*25*axisAngle/360;
    //double dist = (2*pi*25*axisAngle)/360;
	switch(key){

    case 'w':
        //axisAngle++;
        wheelCentre.x = wheelCentre.x + 3*cos(wheelAngle);
        wheelCentre.y = wheelCentre.y + 3*sin(wheelAngle);
        distance = distance + 3;
        //pos.b = pos.b + 2*v.b;
        break;

    case 's':
        //axisAngle--;
        wheelCentre.x = wheelCentre.x - 3*cos(wheelAngle);
        wheelCentre.y = wheelCentre.y - 3*sin(wheelAngle);
        distance = distance - 3;
        //pos.b = pos.b - 2*v.b;
        break;


    case 'a':
        theta++;

        break;

    case 'd':
        theta--;
        break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(-200*cos(cameraAngle), -200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    //glTranslatef(-wheelCentre.x,-wheelCentre.y,-wheelCentre.z);
    //glTranslated(1,1,0);

    /*glTranslatef(wheelCentre.x,wheelCentre.y,0);
    glRotated(theta,0,0,1);
    glTranslatef(-wheelCentre.x,-wheelCentre.y,0);*/
    double rotateAngle = 360*distance/(2*pi*25);
    //glTranslated(0,-5,0);
    glPushMatrix();{
    glTranslated(0,0,25);
    glTranslatef(wheelCentre.x,wheelCentre.y,wheelCentre.z);
    glRotatef(theta,0,0,1);
    //glTranslatef(-wheelCentre.x,-wheelCentre.y,-wheelCentre.z);
    glRotatef(rotateAngle,0,1,0);
    glRotated(90,1,0,0);
    drawWheel(25,10,wheelCentre.x,wheelCentre.y,wheelCentre.z,40);
    }
    glPopMatrix();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}



void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=100.0;
	cameraAngle=45.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();
	initWheel();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}


