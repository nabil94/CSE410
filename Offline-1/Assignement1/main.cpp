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
double inc = 0;
double rr = 0;


struct point
{
	double x,y,z;
};

point pos;
point u;
point r;
point l;

void initVectors(){
    pos.x = 100.0;
    pos.y = 100.0;
    pos.z = 0.0 ;
    u.x = 0.0;
    u.y = 0.0;
    u.z = 1.0;
    r.x = (-1/sqrt(2));
    r.y = (1/sqrt(2));
    r.z = 0.0;
    l.x = (-1/sqrt(2));
    l.y = (-1/sqrt(2));
    l.z = 0.0;
    //printf("%f %f %f",r.x,r.y,r.z);


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
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquareThrZ(double a,double h)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,h);
		glVertex3f( a,-a,h);
		glVertex3f(-a,-a,h);
		glVertex3f(-a, a,h);
	}glEnd();
}

void drawSquareThrX(double a,double h)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f(h, a, a);
		glVertex3f(h, a,-a);
		glVertex3f(h,-a,-a);
		glVertex3f(h,-a, a);
	}glEnd();
}


void drawSquareThrY(double a,double h)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a,h, a);
		glVertex3f( a,h,-a);
		glVertex3f(-a,h,-a);
		glVertex3f(-a,h, a);
	}glEnd();
}



void drawCylinder(double x,double y,double z,double radius,double height,int segments){
    int i;
    struct point points[100];
    //double h = z + width;
    //glColor3f(0.7,0.6,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x = x + radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y = y + radius*sin(((double)i/(double)segments)*2*pi);
        //points[i].z = z + height;
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        //glColor3f(shade,shade,shade);
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,z);
			glVertex3f(points[i+1].x,points[i+1].y,z);
        }
        glEnd();
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,z+height);
			glVertex3f(points[i+1].x,points[i+1].y,z+height);
        }
        glEnd();
        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,z);
			glVertex3f(points[i+1].x,points[i+1].y,z);
			glVertex3f(points[i+1].x,points[i+1].y,z+height);
			glVertex3f(points[i].x,points[i].y,z+height);
        }
        glEnd();
    }

}


void keyboardListener(unsigned char key, int x,int y){
    //angle = angle + 0.5;
    angle = 15*(acos(-1.0)/180.0) ;
	switch(key){

		case '1':
            //angle = angle - 0.5; rotate left
            r.x = r.x*cos(angle) + l.x*sin(angle);
            r.y = r.y*cos(angle) + l.y*sin(angle);
            r.z = r.z*cos(angle) + l.z*sin(angle);
            l = crossProduct(u,r);
			break;

        case '2':
            //angle = angle + 0.5; rotate right
            r.x = (l.x*sin(-angle)) + (r.x*cos(-angle));
            r.y = (l.y*sin(-angle)) + (r.y*cos(-angle));
            r.z = (l.z*sin(-angle)) + (r.z*cos(-angle));
            l = crossProduct(u,r);
			break;

        case '3':
            l.x = l.x*cos(angle) + u.x*sin(angle);
            l.y = l.y*cos(angle) + u.y*sin(angle);
            l.z = l.z*cos(angle) + u.z*sin(angle);
            u = crossProduct(r,l);
            break;

        case '4':
            l.x = l.x*cos(-angle) + u.x*sin(-angle);
            l.y = l.y*cos(-angle) + u.y*sin(-angle);
            l.z = l.z*cos(-angle) + u.z*sin(-angle);
            u = crossProduct(r,l);
            break;

        case '5':
            u.x = u.x*cos(angle) + r.x*sin(angle);
            u.y = u.y*cos(angle) + r.y*sin(angle);
            u.z = u.z*cos(angle) + r.z*sin(angle);
            r = crossProduct(l,u);
            break;

        case '6':
            u.x = u.x*cos(-angle) + r.x*sin(-angle);
            u.y = u.y*cos(-angle) + r.y*sin(-angle);
            u.z = u.z*cos(-angle) + r.z*sin(-angle);
            r = crossProduct(l,u);
            break;


		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			pos.x = pos.x - 2*l.x;
			pos.y = pos.y - 2*l.y;
			pos.z = pos.z - 2*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			pos.x = pos.x + 2*l.x;
			pos.y = pos.y + 2*l.y;
			pos.z = pos.z + 2*l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x = pos.x + 2*r.x;
			pos.y = pos.y + 2*r.y;
			pos.z = pos.z + 2*r.z;
			break;
		case GLUT_KEY_LEFT:
			pos.x = pos.x - 2*r.x;
			pos.y = pos.y - 2*r.y;
			pos.z = pos.z - 2*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x = pos.x + 2*u.x;
			pos.y = pos.y + 2*u.y;
			pos.z = pos.z + 2*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x = pos.x - 2*u.x;
			pos.y = pos.y - 2*u.y;
			pos.z = pos.z - 2*u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if(inc < 20){
                inc++;
		    }
		    //rr--;
			break;
		case GLUT_KEY_END:
		    if(inc > -10){
                inc--;
		    }
		    //rr++;
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

void drawOneEighthSphere(double radius,int stacks,int slices){
    point Points[stacks+7][slices+7] ;

    for(int i=0;i<=stacks;i++){
        double a=radius*cos(((double)i/stacks)*(pi/2)) ;
        double b=radius*sin(((double)i/stacks)*(pi/2)) ;
        for(int j=0;j<=slices;j++){
                double theta = (double(j)/slices)*(pi/2) ;
                double x = a*sin(theta) ;
                double y = a*cos(theta) ;
                double z = b ;
                Points[i][j].x = x ;
                Points[i][j].y = y;
                Points[i][j].z = z;
            }
    }

    for(int i=0;i<stacks;i++){
        glColor3f(1.0,0,0);
		for(int j=0;j<slices;j++){
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(Points[i][j].x,Points[i][j].y,Points[i][j].z);
				glVertex3f(Points[i][j+1].x,Points[i][j+1].y,Points[i][j+1].z);
				glVertex3f(Points[i+1][j+1].x,Points[i+1][j+1].y,Points[i+1][j+1].z);
				glVertex3f(Points[i+1][j].x,Points[i+1][j].y,Points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSph(){
         glPushMatrix();{//2

        //drawSphere(20,20,20,10,50,50);
        glTranslated(-20+inc,-20+inc,-20+inc);
        glRotated(180,0,0,1);
        glRotated(90,0,1,0);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();

     glPushMatrix();{//1
        //lRotated(5,0,0,1);
        //glRotated(2,0,0,1);
        //drawSphere(20,20,20,10,50,50);
        glTranslated(-20+inc,-20+inc,20-inc);
        glRotated(180,0,0,1);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();

    glPushMatrix();{//3

        //drawSphere(20,20,20,10,50,50);
        glTranslated(-20+inc,20-inc,20-inc);
        glRotated(-90,0,1,0);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();

    glPushMatrix();{//3

        //drawSphere(20,20,20,10,50,50);
        glTranslated(-20+inc,20-inc,-20+inc);
        glRotated(90,0,1,0);
        glRotated(-90,1,0,0);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();

    glPushMatrix();{//3

        //drawSphere(20,20,20,10,50,50);
        glTranslated(20-inc,-20+inc,-20+inc);
        glRotated(90,0,1,0);
        glRotated(90,1,0,0);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();
    glPushMatrix();{//3

        //drawSphere(20,20,20,10,50,50);
        glTranslated(20-inc,-20+inc,20-inc);
        glRotated(90,1,0,0);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();
    glPushMatrix();{//2

        //drawSphere(20,20,20,10,50,50);
        glTranslated(20-inc,20-inc,-20+inc);
        glRotated(90,0,1,0);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();
    glPushMatrix();{//1
        //lRotated(5,0,0,1);
        //glRotated(2,0,0,1);
        //drawSphere(20,20,20,10,50,50);
        glTranslated(20-inc,20-inc,20-inc);
        //glRotated(90,0,1,0);
        drawOneEighthSphere(10+inc,100,100);
    }
    glPopMatrix();

}

void drawCyl(double a,double h){
    if(a > 0 && h > 0){
    glColor3f(0,1.0,0);

    drawCylinder(a,a,-a,h,2*a,50);//7
    glPushMatrix();{
	    glTranslated(-2*a,0,0);
	    glRotated(-90,1,0,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
    glPushMatrix();{
	    glTranslated(-2*a,0,0);
	    glRotated(90,1,0,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();

    glPushMatrix();{
	    glTranslated(0,-2*a,0);
	    glRotated(90,0,1,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
    glPushMatrix();{
	    glTranslated(0,-2*a,0);
	    glRotated(-90,0,1,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();

	glPushMatrix();{
	    //glTranslated(-40,00,0);
	    glRotated(-90,0,1,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
	glPushMatrix();{
	    //glTranslated(-40,00,0);
	    glRotated(90,0,1,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
	glPushMatrix();{
	    //glTranslated(-40,00,0);
	    glRotated(-90,1,0,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
	glPushMatrix();{
	    //glTranslated(-40,00,0);
	    glRotated(90,1,0,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
	glPushMatrix();{
	    glTranslated(-2*a,00,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
	glPushMatrix();{
	    glTranslated(-2*a,-2*a,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();
	glPushMatrix();{
	    glTranslated(0,-2*a,0);
	    drawCylinder(a,a,-a,h,2*a,50);
	}
	glPopMatrix();//1
	//drawCylinder(20,20,-20,10,40,50);
	//drawCylinder(20,20,-20,10,40,50);
    }

}

void drawSqu(double a, double b){
    glColor3f(1.0,1.0,1.0);
      drawSquareThrY(a,b);
      drawSquareThrY(a,-b);

      drawSquareThrX(a,b);
      drawSquareThrX(a,-b);

      drawSquareThrZ(a,b);
      drawSquareThrZ(a,-b);
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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	gluLookAt(pos.x,pos.y,pos.z,    pos.x+l.x,pos.y+l.y,pos.z+l.z,  u.x,u.y,u.z);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1.0,1.0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);
    //drawCylinder(20,50,24);
	//drawSphere(5,0,8,5,12,20);
	//drawSqu();
	//glColor3f(0.0,1.0,0.0);
	//drawCylinder(0,0,-30,20,60,50);
	//drawSphere(5,5,30,15,20,20);
	/*if(inc + 20 >= 0 && rr + 10 > 0){
        drawCyl(20+inc,10+rr);
        drawSqu(20+inc,30);
	}
	else if(rr + 10 <= 0){
        drawSqu(30,30);
	}*/
	drawSph();
    drawCyl(20-inc,10+inc);
    drawSqu(20-inc,30);
    //drawSqu(30,30);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.02;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=0.0;
	cameraAngle=1.0;
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
	initVectors();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

