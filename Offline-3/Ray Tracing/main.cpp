#include<bits/stdc++.h>


#include <windows.h>
#include <glut.h>
#include "bitmap_image.hpp"
#define pi (2*acos(0.0))

#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3

#define EPSILON 0.000001



using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double inc = 0;
double rr = 0;
double W;
double H;

double window_height;
double window_width;
double fovY;//
double aspectRatio;
double near_distance;
double far_distance;
int image_height;
int image_width;
int recursion_level;

struct point
{
	double x,y,z;
};


class Vector{
public:
    double x,y,z;

    Vector(){
        x = y = z = 0;
    }

    Vector(double x,double y,double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double dot(Vector v){
        double prod = x*v.x + y*v.y + z*v.z;
        return prod;
    }

    Vector cross(Vector v){
        Vector a;
        a.x = y*v.z - z*v.y;
        a.y = z*v.x - x*v.z;
        a.z = x*v.y - y*v.x;
        return a;
    }

    Vector Plus(Vector v){
        Vector a;
        a.x = x + v.x;
        a.y = y + v.y;
        a.z = z + v.z;
        return a;
    }

    Vector Minus(Vector v){
        Vector a;
        a.x = x - v.x;
        a.y = y - v.y;
        a.z = z - v.z;
        return a;
    }

    Vector Mul(double v){
        Vector a;
        a.x = x*v;
        a.y = y*v;
        a.z = z*v;
        return a;
    }

    Vector Div(double v){
        Vector a;
        a.x = x/v;
        a.y = y/v;
        a.z = z/v;
        return a;
    }

    Vector normalize(){
        double d = sqrt(x*x + y*y + z*z);
        Vector a = Div(d);
        return a;
    }

    double getValue(){
         double d = sqrt(x*x + y*y + z*z);
         return d;
    }


    void printVector(){
        cout<<x<<"i "<<y<<"j "<<z<<"k"<<endl;
        //glutSo

    }



};

Vector getResult(Vector a, Vector b, double t){
    Vector res = b.Mul(t);
    Vector result = a.Plus(res);
    return result;
}

Vector pos;
Vector u;
Vector r;
Vector l;

void initVectors(){
    pos.x = 50.0;
    pos.y = 50.0;
    pos.z = 50.0 ;
    u.x = 0.0;
    u.y = 0.0;
    u.z = 1.0;
    r.x = (-1/sqrt(2));
    r.y = (1/sqrt(2));
    r.z = 0.0;
    l.x = (-1/sqrt(2));
    l.y = (-1/sqrt(2));
    l.z = 0.0;
    window_height = 500;
    window_width = 500;
    //fovY = 80.0;
    //aspectRatio = 1.0;
    //near = 1.0;
    //far = 1000.0;
    image_height = 768;
    image_width = 768;
    //printf("%f %f %f",r.x,r.y,r.z);


}

class Color{
public:
    double r;
    double g;
    double b;

    Color(){
    }

    Color(double r,double g,double b){
        this->r = r;
        this->g = g;
        this->b = b;
    }


    Color Plus(Color c){
        Color a;
        a.r = this->r + c.r;
        a.g = this->g + c.g;
        a.b = this->b + c.b;
        return a;
    }

    Color Minus(Color c){
        Color a;
        a.r = r - c.r;
        a.g = g - c.g;
        a.b = b - c.b;
        return a;
    }

    Color Mul(double v){
        double a = r*v;
        double b = g*v;
        double c = b*v;
        return Color(a,b,c);
    }


    Color Div(double v){
        Color a;
        a.r = r/v;
        a.g = g/v;
        a.b = b/v;
        return a;
    }

    void printColor(){
        cout<<r<<" "<<g<< " "<<b<<endl;
    }


};

Color global;

void drawTriangles(Vector a,Vector b, Vector c){
    glBegin(GL_TRIANGLES);{
        glVertex3f(a.x,a.y,a.z);
        glVertex3f(b.x,b.y,b.z);
        glVertex3f(c.x,c.y,c.z);
    }
    glEnd();
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}



class Square{
public:

    double length;
    Color c;
    Vector v;

    Square(){

    }

    Square(double length,Color c, Vector v){
        this->length = length;
        this->c = c;
        this->v = v;
    }

    void draw(){
        glPushMatrix();{
            glTranslatef(v.x,v.y,v.z);
            glColor3f(c.r,c.g,c.b);
            drawSquare(length);
        }
        glPopMatrix();
    }

};


/*

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
				glVertex3f(i*20, -330*2, 0);
				glVertex3f(i*20,  330*2, 0);

				//lines parallel to X-axis
				glVertex3f(-330*2, i*20, 0);
				glVertex3f( 330*2, i*20, 0);
			}
		}glEnd();
	}
}
*/
void drawCheckBoard(double sqSize){
    sqSize = 15;
        Color t;
    for(int i = -50; i < 50; i++){
        for(int j =-50; j < 50; j++){
            if((i%2 == 0 && j%2 == 0) || (i%2 != 0 && j%2 != 0)){
                t = Color(1.0,1.0,1.0);
            }
            else if((i%2 != 0 && j%2 == 0) || (i%2 == 0 && j%2 != 0)){
                    t = Color(0.0,0.0,0.0);
            }
            Square sq(sqSize,t,Vector(i*30,j*30,0));
            sq.draw();
            //sqr.push_back(sq);
        }

    }

}

class Light{
public:
    Vector v;

    Light(){
    }

    Light(Vector v){
        this->v = v;
    }

    Vector getLightSource(){
        return this->v;
    }

    void draw(){
        glPushMatrix();{
            glColor3f(1.0,1.0,1.0);
            glTranslatef(v.x,v.y,v.z);
            glutSolidSphere(1,10,10);
        }
        glPopMatrix();
    }


} ;


/*
vector<Sphere> sp;
vector<Triangle> tr;
vector<Light> lights;*/
vector<Light> lights;

class Ray{
public:
    Vector start;
    Vector dir;

    Ray(){}

    Ray(Vector start,Vector dir){
        this->start = start;
        this->dir = dir;
    }

    printRay(){
        cout<<start.x<<"i "<<start.y<<"j "<<start.z<<"k "<<endl;
        cout<<dir.x<<"i "<<dir.y<<"j "<<dir.z<<"k "<<endl;
        cout<<endl;
        //glutS

    }

};

class Object{
public:
    Vector ref_point;
    double shine;
    Color color;
    double coefficient[4];

    Object(){

    }

    virtual void draw(){}
    virtual Vector getNormal(Vector v){};
    virtual double getT(Ray r){}
    virtual double intersect(Ray r, Color *current_color, int level){
        return -1;
	}
	virtual Color getColor(){};

	void setShine(double shine){
        this->shine = shine;
	}

	void setCoefficients(double a,double d,double s,double r){
        this->coefficient[AMBIENT] = a;
        this->coefficient[DIFFUSE] = d;
        this->coefficient[SPECULAR] = s;
        this->coefficient[REFLECTION] = r;
	}

	Vector getReflection(Ray r, Vector v){
        double rf = -2*r.dir.dot(v);
        Vector rr = v.Mul(rf);
        Vector refl = rr.Plus(r.dir);
        return refl;
	}
 };

 vector<Object*> Obj;

class Sphere: public Object{
public:
    Vector centre;
    double radius;

    Sphere(Vector centre,double radius,Color color){
        this->centre = centre;
        this->radius = radius;
        this->color = color;
    }

    Color getColor(){
        return this->color;
    }

    void draw(){
        glPushMatrix();{
            glColor3f(color.r,color.g,color.b);
            glTranslatef(centre.x,centre.y,centre.z);
            //drawSphere(radius,20,20);
            glutSolidSphere(radius,20,20);
        }
        glPopMatrix();
    }

    Vector getNormal(Vector v){
        Vector normal;
        normal = v.Minus(centre);
        Vector n = normal.normalize();
        return n;
    }

    double getT(Ray r){
        // = Ray(Vector(0,100,0),Vector(0,1,0));
        double t;
        Vector R0 = r.start.Minus(centre);
        Vector Rd = r.dir;
        double A = Rd.dot(Rd);
        double B = 2.0*R0.dot(Rd);
        double C = R0.dot(R0) - radius*radius;
        double D = B*B - (4*A*C);
        //D = D*(-1);

        if(D >= 0){
            //cout<<"A : "<<A<<" B : "<<B<<" C : "<<C<<" D : "<<D<<endl;
            double t1 = (-B + sqrt(D))/2;
            double t2 = (-B - sqrt(D))/2;

            t = min(t1,t2);
            if(t <= 0){
                t = max(t1,t2);
            }
            //cout<<t<<endl;


            return t;
            /*pair<double,double> p;
            p.first = t1;
            p.second = t2;

            return p;*/
        }
        else return -1;


    }

    double intersect(Ray r, Color *current_color, int level){
        //current_color = this->getColor();
        //setColorAt();
        double t = getT(r);

        if(t <= 0) return -1;
        if(level == 0) return t;



        Vector intersectionPoint = getResult(r.start,r.dir,t);

        current_color->r = color.r*coefficient[AMBIENT];
        current_color->g = color.g*coefficient[AMBIENT];
        current_color->b = color.b*coefficient[AMBIENT];

        Vector normal = getNormal(intersectionPoint);
        Vector Reflection = getReflection(r,normal);

        for(int i = 0; i < lights.size(); i++){
            Vector lightDir;
            /*ldir.x = lights[i].v.x - intersectionPoint.x;
            ldir.y = lights[i].v.y - intersectionPoint.y;
            ldir.z = lights[i].v.z - intersectionPoint.z;*/
            lightDir = lights[i].getLightSource().Minus(intersectionPoint);

            double value = lightDir.getValue();
            lightDir = lightDir.normalize();

            Vector lightSource = getResult(intersectionPoint,lightDir,1.0);


            Ray L(lightSource, lightDir);

            bool flag = false;

            for(int j = 0; j < Obj.size(); j++){
                double p = Obj[j]->getT(L);
                //printf(",%f %d, ", possibleobscure, j);
                if(p > 0 && abs(p) < value){
                    flag = true;
                    break;
                }
            }
            //printf("\n");

            if(!flag){
                //printf("direct");
                //L.dir.x = -L.dir.x;
                //L.dir.y = -L.dir.y;
                //L.dir.z = -L.dir.z;
                double lambert = L.dir.dot(normal);
                double phong = Reflection.dot(r.dir);

                lambert = lambert < 0? 0:lambert;
                lambert = lambert > 1? 1:lambert;
                phong = phong < 0?0:phong;
                phong = phong < 0?0:phong;



                current_color->r += 1.0 * color.r * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);
                current_color->g += 1.0 * color.g * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);
                current_color->b += 1.0 * color.b * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);


            }
        }
        if(level < recursion_level){
                Vector res;
                res = getResult(intersectionPoint,Reflection,1);


                //reflection = normalize(reflection);

                Ray reflectionRay(res, Reflection);
                int nearest = -1;

                Color reflectColor;
                double t_min = 99999;
                for(int k = 0; k < Obj.size(); k++){
                    double t = Obj[k]->getT(reflectionRay);

                    if(t <= 0) continue;
                    else if(t < t_min) {
                        t_min = t;
                        nearest = k;
                    }
                }

                if(nearest != -1){
                    double t = Obj[nearest]->intersect(reflectionRay, &reflectColor, level + 1);
                    if(t != -1){
                        current_color->r += reflectColor.r*coefficient[REFLECTION];
                        current_color->g += reflectColor.g*coefficient[REFLECTION];
                        current_color->b += reflectColor.b*coefficient[REFLECTION];
                    }
                }
            }

            current_color->r = current_color->r < 0? 0:current_color->r;
            current_color->r = current_color->r > 1? 1:current_color->r;
            current_color->g = current_color->g < 0? 0:current_color->g;
            current_color->g = current_color->g > 1? 1:current_color->g;
            current_color->b = current_color->b < 0? 0:current_color->b;
            current_color->b = current_color->b > 1? 1:current_color->b;

            return t;
    }

    void setColorAt(){
        //color = color.Mul(this->coefficient[AMBIENT]);
    }


};

class Floor: public Object{
public:
    double FloorWidth;
    double length;
    double noOfTiles;

    Floor(){

    }

    Floor(double FloorWidth,double length){
        this->FloorWidth = FloorWidth;
        this->length = length;
        Vector v = Vector(FloorWidth,FloorWidth,0);
        this->ref_point = Vector(-FloorWidth/2.0,-FloorWidth/2,0);
        this->noOfTiles = FloorWidth/length;
        //this->color = color;


    }

    void draw(){
        //Color t;
        for(int i = 0; i < noOfTiles; i++){
            for(int j = 0; j < noOfTiles; j++){
                double a = (i+j)%2;
                color = Color(a,a,a);
                Square sq(length/2,color,Vector(ref_point.x+i*length,ref_point.y+j*length,0));
                sq.draw();
            }
        }
    }

    Color getColor(){
        return color;
    }

    Vector getNormal(Vector v){
        return Vector(0,0,1);
    }

    double getT(Ray r){
        /*struct point normal = getNormal(reference_point);
        return ((-1.0) * dotProduct(normal, r->start) / dotProduct(normal, r->dir));*/
        Vector normal = Vector(0,0,1);
        double t = ((-1.0*normal.dot(r.start)))/(normal.dot(r.dir));
        return t;

    }

    double intersect(Ray r, Color *current_color,int level){
        //current_color = getColor();

        double t = getT(r);
        if(t <= 0) return -1;
        if(level == 0) return t;



        Vector intersectionPoint;
        intersectionPoint = r.start.Plus(r.dir.Mul(t));



        int x = (intersectionPoint.x - ref_point.x) / length;
        int y = (intersectionPoint.y - ref_point.y) / length;

        double s = (x+y)%2;
        color = Color(s,s,s);

        current_color->r = color.r*coefficient[AMBIENT];
        current_color->g = color.g*coefficient[AMBIENT];
        current_color->b = color.b*coefficient[AMBIENT];

        Vector normal = getNormal(intersectionPoint);
        Vector Reflection = getReflection(r,normal);

        for(int i = 0; i < lights.size(); i++){
            Vector lightDir = lights[i].getLightSource().Minus(intersectionPoint);

            double value = lightDir.getValue();
            lightDir = lightDir.normalize();

            Vector lightSource;
            /*lstart.x = intersectionPoint.x + ldir.x * 1.0;
            lstart.y = intersectionPoint.y + ldir.y * 1.0;
            lstart.z = intersectionPoint.z + ldir.z * 1.0;*/
            lightSource = getResult(intersectionPoint,lightDir,1.0);


            Ray L(lightSource, lightDir);

            bool flag = false;

            for(int j = 0; j < Obj.size(); j++){
                double p = Obj[j]->getT(L);
                //printf(",%f %d, ", possibleobscure, j);
                if(p > 0 && abs(p) < value){
                    flag = true;
                    break;
                }
            }
            //printf("\n");

            if(!flag){
                //printf("direct");
                //L.dir.x = -L.dir.x;
                //L.dir.y = -L.dir.y;
                //L.dir.z = -L.dir.z;
                double lambert = L.dir.dot(normal);
                double phong = Reflection.dot(r.dir);

                lambert = lambert < 0 ? 0:lambert;
                lambert = lambert > 1 ? 1:lambert;
                phong = phong < 0 ? 0:phong;
                phong = phong > 1 ? 1:phong;

                Color cc = color.Mul((lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]));
                *current_color = current_color->Plus(cc.Mul(1.0));
                //current_color->r += 1.0 * color.r * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);
                //current_color->g += 1.0 * color.g * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);
                //current_color->b += 1.0 * color.b * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);

            }
        }

            if(level < recursion_level){
                Vector res;
                res = getResult(intersectionPoint,Reflection,1.0);

                //reflection = normalize(reflection);

                Ray reflectionRay(res, Reflection);
                int nearest = -1;

                Color reflectColor;
                double t_min = 9999;
                for(int k = 0; k < Obj.size(); k++){
                    double t = Obj[k]->getT(reflectionRay);

                    if(t <= 0) continue;
                    else if(t < t_min) {
                        t_min = t;
                        nearest = k;
                    }
                }

                if(nearest != -1){
                    double t = Obj[nearest]->intersect(reflectionRay, &reflectColor, level + 1);
                    if(t != -1){
                        Color rf = reflectColor.Mul(coefficient[REFLECTION]);
                        *current_color = current_color->Plus(rf);
                        //current_color->r += reflectColor.r*coefficient[REFLECTION];
                        //current_color->g += reflectColor.g*coefficient[REFLECTION];
                        //current_color->b += reflectColor.b*coefficient[REFLECTION];
                    }
                }
            }
            current_color->r = current_color->r < 0? 0:current_color->r;
            current_color->r = current_color->r > 1? 1:current_color->r;
            current_color->g = current_color->g < 0? 0:current_color->g;
            current_color->g = current_color->g > 1? 1:current_color->g;
            current_color->b = current_color->b < 0? 0:current_color->b;
            current_color->b = current_color->b > 1? 1:current_color->b;





        return t;

    }


};

class Triangle: public Object{
public:
    Vector p1,p2,p3;
    double p,q,r,s;

    Triangle(){}

    Triangle(Vector p1,Vector p2,Vector p3,Color color){
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;

        Vector AB = p2.Minus(p1);
        Vector AC = p3.Minus(p1);

        Vector V = AB.cross(AC);

        this->p = V.x;
        this->q = V.y;
        this->r = V.z;
        this->s = V.dot(p1);

        this->color = color;
    }

    void draw(){
        glPushMatrix();{
            glColor3f(color.r,color.g,color.b);
            drawTriangles(p1,p2,p3);
        }
        glPopMatrix();
    }

    Vector getNormal(Vector v){
        Vector v1 = p2.Minus(p1);
        Vector v2 = p3.Minus(p1);
        Vector n = v1.cross(v2);
        //Vector n = Vector(p, q, r);
        return n.normalize();

    }

    double getT(Ray r){
        Vector normal = getNormal(ref_point);

        double t;
        bool inside = false;
        double denom = normal.dot(r.dir);

        if (denom < 0.0)
        {
            normal = normal.Mul(-1.0);
            denom = normal.dot(r.dir);;
        }

        // checking whether the ray intersects with the triangle's plane
        if (abs(denom) < EPSILON)
            return -1;

        t = (normal.dot(p1.Minus(r.start))) / denom;
        if (t >= 0)
            inside = true; // yes, intersects the plane

        if (!inside)
            return -1; // no

        Vector intersectionPoint = getResult(r.start,r.dir,t);
        Vector n1 = p2.Minus(p1);
        Vector n2 = p3.Minus(p1);
        Vector n = n1.cross(n2);

        Vector edge1 = p2.Minus(p1);
        Vector edge2 = intersectionPoint.Minus(p1);
        Vector c = edge1.cross(edge2);

        Vector edge3 = p3.Minus(p2);
        Vector edge4 = intersectionPoint.Minus(p2);
        Vector c1 = edge3.cross(edge4);

        Vector edge5 = p1.Minus(p3);
        Vector edge6 = intersectionPoint.Minus(p3);
        Vector c2 =edge5.cross(edge2);
        if (n.dot(c) < 0 || n.dot(c2) < 0 || n.dot(c1) < 0)
            return -1;

        return t;

    }



    double intersect(Ray r, Color *current_color,int level){

        double t = getT(r);

        if(t <= 0) return -1;
        if(level == 0) return t;
        current_color->r = color.r*coefficient[AMBIENT];
        current_color->g = color.g*coefficient[AMBIENT];
        current_color->b = color.b*coefficient[AMBIENT];

        Vector intersectionPoint;
        intersectionPoint = r.start.Plus(r.dir.Mul(t));

        Vector normal = getNormal(intersectionPoint);
        Vector Reflection = getReflection(r,normal);

        for(int i = 0; i < lights.size(); i++){
            Vector lightDir = lights[i].getLightSource().Minus(intersectionPoint);

            double value = lightDir.getValue();
            lightDir = lightDir.normalize();

            Vector lightSource;
            /*lstart.x = intersectionPoint.x + ldir.x * 1.0;
            lstart.y = intersectionPoint.y + ldir.y * 1.0;
            lstart.z = intersectionPoint.z + ldir.z * 1.0;*/
            lightSource = getResult(intersectionPoint,lightDir,1.0);


            Ray L(lightSource, lightDir);

            bool flag = false;

            for(int j = 0; j < Obj.size(); j++){
                double p = Obj[j]->getT(L);
                //printf(",%f %d, ", possibleobscure, j);
                if(p > 0 && abs(p) < value){
                    flag = true;
                    break;
                }
            }
            //printf("\n");

            if(!flag){
                //printf("direct");
                //L.dir.x = -L.dir.x;
                //L.dir.y = -L.dir.y;
                //L.dir.z = -L.dir.z;
                double lambert = L.dir.dot(normal);
                double phong = Reflection.dot(r.dir);

                lambert = lambert < 0 ? 0:lambert;
                lambert = lambert > 1 ? 1:lambert;
                phong = phong < 0 ? 0:phong;
                phong = phong > 1 ? 1:phong;

                //color cc = color.Mul()
                Color cc = color.Mul((lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]));
                *current_color = current_color->Plus(cc.Mul(1.0));

                //current_color->r += 1.0 * color.r * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);
                //current_color->g += 1.0 * color.g * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);
                //current_color->b += 1.0 * color.b * (lambert * coefficient[DIFFUSE] + pow(phong, shine) * coefficient[SPECULAR]);
            }
        }
           /* if(level<recursion_level)
        start=intersectionPoint+reflection*1 //slight up to avoid own
        intersection
            Ray reflectionRay(start, reflection)
        Like capture method, find the nearest intersecting object, using
        intersect function
        If found
        objects[nearest]->intersect(reflectionRay, reflected_color,level+1);
//update curernt_color using reflected_color
            current_color+=reflected_color*co_efficient[REFLECTION];*/
            if(level < recursion_level){
                //Vector res;
                Vector res = getResult(intersectionPoint,Reflection,1.0);

                //reflection = normalize(reflection);

                Ray reflectionRay(res, Reflection);
                int nearest = -1;

                Color reflectColor;
                double t_min = 99999;
                for(int k = 0; k < Obj.size(); k++){
                    double t = Obj[k]->getT(reflectionRay);

                    if(t <= 0) continue;
                    else if(t < t_min) {
                        t_min = t;
                        nearest = k;
                    }
                }

                if(nearest != -1){
                    double t = Obj[nearest]->intersect(reflectionRay, &reflectColor, level + 1);
                    if(t != -1){
                        current_color->r += reflectColor.r*coefficient[REFLECTION];
                        current_color->g += reflectColor.g*coefficient[REFLECTION];
                        current_color->b += reflectColor.b*coefficient[REFLECTION];
                    }
                }
            }

            //Check whether all current_color pixel value is within 1 or 0 if not set it
        current_color->r = current_color->r < 0? 0:current_color->r;
            current_color->r = current_color->r > 1? 1:current_color->r;
            current_color->g = current_color->g < 0? 0:current_color->g;
            current_color->g = current_color->g > 1? 1:current_color->g;
            current_color->b = current_color->b < 0? 0:current_color->b;
            current_color->b = current_color->b > 1? 1:current_color->b;


        return t;
    }

    Color getColor(){
        return color;
    }



} ;


void loadData(){
   /* Object *temp;
    temp = new Sphere(Vector(0,0,10),10,Color(1.0,0.0,0.0));
    temp->setCoefficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    Obj.push_back(temp);

    Object *temp1;
    temp1 = new Sphere(Vector(030,60,20),20,Color(0.0,0.0,1.0));
    temp1->setCoefficients(0.4,0.2,0.2,0.2);
    temp1->setShine(1);
    Obj.push_back(temp1);

    Object *temp11;
    temp11 = new Sphere(Vector(15,15,45),15,Color(1.0,1.0,0.0));
    temp11->setCoefficients(0.4,0.2,0.2,0.2);
    temp11->setShine(1);
    Obj.push_back(temp11);

    Object *temp2;
    temp2 = new Floor(1500,30);
    temp2->setCoefficients(0.4,0.2,0.2,0.2);
    temp2->setShine(1);
    Obj.push_back(temp2);

    Light light1 = Light(Vector(-50,50,50));
    lights.push_back(light1);*/


}

void capture(){
    bitmap_image img(image_width,image_height);
    double distance = (window_height/2.0)/tan(fovY*pi/360);
    Vector topleft;
    //H = near_distance * tan(pi * fovY/(2.0 * 180));
    //W = aspectRatio * H;
    /*topleft.x = pos.x + (l.x * distance) - r.x * (window_width/2.0) + u.x * (window_height/2.0);
    topleft.y = pos.y + (l.y * distance) - r.y * (window_width/2.0) + u.y * (window_height/2.0);
    topleft.z = pos.z + (l.z * distance) - r.z * (window_width/2.0) + u.z * (window_height/2.0);*/
    //cout<<"Camera : "<<W<<"::"<<H<<endl ;
    Vector expr1 = l.Mul(distance);
    Vector expr2 = u.Mul(window_height/2.0);
    Vector expr3 = r.Mul((-1)*window_width/2.0);
    Vector expr4 = pos.Plus(expr1.Plus(expr2.Plus(expr3)));
    topleft = expr4;
    topleft.printVector();
    //Color frame[image_width][image_height];

    //topleft.printVector();
    double px = 1.0*window_width/image_width;
    double py = 1.0*window_height/image_height;
    //cout<<px<<" "<<py<<endl;
    for(int i = 0; i < image_height; i++){
        for(int j = 0; j < image_width; j++){
            Vector corner;
            corner.x = topleft.x + (-1.0)*u.x*i*px + r.x*j*py;
            corner.y = topleft.y + (-1.0)*u.y*i*px + r.y*j*py;
            corner.z = topleft.z + (-1.0)*u.z*i*px + r.z*j*py;
            //corner.printVector();
            //double px = -W/ j*width;
            //double py = -H/ i*height;
            //cout<<px<<" "<<py<<endl;
            Vector Rdir = corner.Minus(pos);
            Vector rdir = Rdir.normalize();
            /*projectionRay.dir.x = (-1)*l.x*near_distance + u.x*px + r.x*py;
            projectionRay.dir.y = (-1)*l.y*near_distance + u.y*px + r.y*py;
            projectionRay.dir.z = (-1)*l.z*near_distance + u.z*px + r.z*py;*/
           // Vector dirPro = projectionRay.dir.normalize();
            Ray ray = Ray(pos,rdir);

            int nearest = -1;
            double t_min = 999999;
            Color dummy_color;
            for(int k = 0; k < Obj.size(); k++){
                //global = Obj[k]->color;
                double t = Obj[k]->intersect(ray,&dummy_color,0);
                //cout<<t<<endl;
                if(t < 0) continue;
                if(t < t_min){
                    t_min = t;
                    nearest = k;
                    /*Color c = dummy_color;
                    if(t > 0){
                        c = Obj[k]->getColor();
                        img.set_pixel(j, i, (int)c.r*255, (int)c.g*255, (int)c.b*255);
                    }*/
                    //img.set_pixel(j, i, (int)c.r*255, (int)c.g*255, (int)c.b*255);
                }

            }
            if(nearest != -1){

                double t = Obj[nearest]->intersect(ray,&dummy_color,1);

                //Obj[0]->getColor().printColor();
                //cout<<Obj[0]->coefficient[AMBIENT]<<endl;
                //dummy_color.printColor();

                Color c;
                c.r = dummy_color.r;//Obj[nearest]->getColor();
                c.g = dummy_color.g;
                c.b = dummy_color.b;

                img.set_pixel(j, i, (double)c.r*255, (double)c.g*255, (double)c.b*255);

            }

        }

    }
    cout<<"image generated "<<endl;
        img.save_image("output.bmp");
        cout<<"image saved "<<endl;


}

void keyboardListener(unsigned char key, int x,int y){
    //angle = angle + 0.5;
    angle = 15*(acos(-1.0)/180.0) ;
	switch(key){

        case '1':
            {
                Vector a,b;
                a = r.Mul(cos(angle));
                b = l.Mul(sin(angle));
                r = a.Plus(b);
                l = u.cross(r);
                break;
            }
            //angle = angle - 0.5; rotate left

        case '2':
            {
                Vector a,b;
                a = r.Mul(cos(-angle));
                b = l.Mul(sin(-angle));
                r = a.Plus(b);
                l = u.cross(r);
                break;
            }

        case '3':
            {
                Vector a,b;
                a = l.Mul(cos(angle));
                b = u.Mul(sin(angle));
                l = a.Plus(b);
                u = r.cross(l);
                break;
            }

        case '4':
            {
                Vector a,b;
                a = l.Mul(cos(-angle));
                b = u.Mul(sin(-angle));
                l = a.Plus(b);
                u = r.cross(l);
                break;
            }

        case '5':
            {
                Vector a,b;
                a = u.Mul(cos(angle));
                b = r.Mul(sin(angle));
                u = a.Plus(b);
                r = l.cross(u);
                break;
            }

        case '6':
            {
                Vector a,b;
                a = u.Mul(cos(-angle));
                b = r.Mul(sin(-angle));
                u = a.Plus(b);
                r = l.cross(u);
                break;
            }

        case '0':
            {
                capture();
                break;
            }

        default:
            break;




	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			pos = pos.Minus(l.Mul(3));
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			pos = pos.Plus(l.Mul(3));
			break;

		case GLUT_KEY_RIGHT:
		    pos = pos.Plus(r.Mul(3));
			break;
		case GLUT_KEY_LEFT:
		    pos = pos.Minus(r.Mul(3));
			break;

		case GLUT_KEY_PAGE_UP:
		    pos = pos.Plus(u.Mul(3));
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos = pos.Minus(u.Mul(3));
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

	//drawAxes();
	//drawGrid();
	//drawCheckBoard();
	/*glColor3f(1.0,0.0,0);
	drawTriangles(Vector(0,0,60),Vector(15,15,0),Vector(15,-15,0));*/
        for(int i = 0; i < Obj.size(); i++){
            glPushMatrix();{
                Obj[i]->draw();
            }
            glPopMatrix();
        }

        for(int i = 0; i < lights.size(); i++){
            glPushMatrix();{
                lights[i].draw();
            }
            glPopMatrix();
        }


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.0001;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void takeInput(){
    int pixels,object_num,light_source;
    double x,y,z,radius,red,green,blue,ambient,diffuse,specular,reflection,specular_co,height,length;
    FILE *fp;
    fp = fopen("description.txt","r");
    fscanf(fp,"%d %d %d",&recursion_level,&pixels,&object_num) ;
    char obj[50];
    while(object_num--){
        fscanf(fp, "%s", obj);
        if (!strcmp(obj, "sphere")){
            fscanf(fp,"%lf %lf %lf",&x,&y,&z) ;
            fscanf(fp,"%lf",&radius);
            fscanf(fp,"%lf %lf %lf",&red,&green,&blue);
            fscanf(fp,"%lf %lf %lf %lf",&ambient,&diffuse,&specular,&reflection);
            fscanf(fp,"%lf",&specular_co);
            //Sphere s = Sphere(radius,Vector(x,y,z),50,50,Color(red,green,blue));
            //sp.push_back(s);
            Object *temp;
            temp = new Sphere(Vector(x,y,z),radius,Color(red,green,blue));
            temp->setCoefficients(ambient,diffuse,specular,reflection);
            temp->setShine(specular_co);
            Obj.push_back(temp);


        }

         else if (!strcmp(obj, "pyramid")){
            fscanf(fp,"%lf %lf %lf",&x,&y,&z) ;
            fscanf(fp,"%lf %lf",&length,&height);
            fscanf(fp,"%lf %lf %lf",&red,&green,&blue);
            fscanf(fp,"%lf %lf %lf %lf",&ambient,&diffuse,&specular,&reflection);
            fscanf(fp,"%lf",&specular_co);


            Vector p = Vector(x,y,z+height);
            Vector pp = Vector(x+(length/2),y+(length/2),z);
            Vector W = Vector(pp.x,pp.y,pp.z);
            Vector W1 = Vector(pp.x,-pp.y,z);
            Vector W2 = Vector(-pp.x,-pp.y,z);
            Vector W3 = Vector(-pp.x,pp.y,z);

           /* cout<<W.x<<" "<<W.y<<" "<<W.z<<endl;
            cout<<W1.x<<" "<<W1.y<<" "<<W1.z<<endl;
            cout<<p.x<<" "<<p.y<<" "<<p.z<<endl;*/



            //Vector q = Vector(x,y,z+height);
            Object *t1,*t2,*t3,*t4,*t5,*t6;
            t1 = new Triangle(p,W,W1,Color(red,green,blue));
            t1->setCoefficients(ambient,diffuse,specular,reflection);
            t1->setShine(specular_co);
            Obj.push_back(t1);
            t2 = new Triangle(p,W1,W2,Color(red,green,blue));
            t2->setCoefficients(ambient,diffuse,specular,reflection);
            t2->setShine(specular_co);
            Obj.push_back(t2);
            t3 = new Triangle(p,W2,W3,Color(red,green,blue));
            t3->setCoefficients(ambient,diffuse,specular,reflection);
            t3->setShine(specular_co);
            Obj.push_back(t3);
            t4 = new Triangle(p,W3,W,Color(red,green,blue));
            t4->setCoefficients(ambient,diffuse,specular,reflection);
            t4->setShine(specular_co);
            Obj.push_back(t4);
            t5 = new Triangle(W1,W2,W3,Color(red,green,blue));
            t5->setCoefficients(ambient,diffuse,specular,reflection);
            t5->setShine(specular_co);
            Obj.push_back(t5);
            t6 = new Triangle(W1,W,W3,Color(red,green,blue));
            t6->setCoefficients(ambient,diffuse,specular,reflection);
            t6->setShine(specular_co);
            Obj.push_back(t6);

            //cout<<tr.size()<<endl;







        }

    }
    fscanf(fp," %d",&light_source) ;
    for(int i = 0; i < light_source; i++){
        fscanf(fp,"%lf %lf %lf ",&x,&y,&z) ;
        Light l = Light(Vector(x,y,z));
        lights.push_back(l);
    }
    //cout<<lights.size()<<endl;
    Object *temp2;
    temp2 = new Floor(1500,30);
    temp2->setCoefficients(0.5, 0.1, 0.1, 0.3);
    temp2->setShine(1);
    Obj.push_back(temp2);

}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	cameraHeight=0.0;
	cameraAngle=1.0;
	angle=0;
	fovY = 90.00;
    aspectRatio = 1;
    near_distance = 1;
    far_distance = 10000.0;
    //H = near_distance * tan(pi * fovY/(2.0 * 180));
    //W = aspectRatio * H;

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
	gluPerspective(90,	1,	1,	1000.0);
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

	glutCreateWindow("Ray Tracer");

	init();

	initVectors();
	takeInput();
	//loadData();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

