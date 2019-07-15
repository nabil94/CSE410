#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;
#define pi (2 * acos(0.0))

#define dim 4

struct point
{
    double x, y, z;
};

void initpoint(point a)
{
    a.x = a.y = a.z = 0;
}

class Matrix
{
public:
    int row, col;
    double **mat;

    Matrix(){
        mat = new double *[row];
        for (int i = 0; i < row; i++)
        {
            mat[i] = new double[col];
            for (int j = 0; j < col; j++)
            {
                mat[i][j] = 0;
            }
        }
    }

    Matrix(int row, int col)
    {
        this->row = row;
        this->col = col;
        mat = new double *[row];
        for (int i = 0; i < row; i++)
        {
            mat[i] = new double[col];
            for (int j = 0; j < col; j++)
            {
                mat[i][j] = 0;
            }
        }
    }

    void PrintMatrix()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void PrintTriangle()
    {
        for(int i = 0; i < 3; i++){
            mat[i][0] = mat[i][0]/mat[3][0];
            mat[i][1] = mat[i][1]/mat[3][1];
            mat[i][2] = mat[i][2]/mat[3][2];
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                cout << mat[j][i] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void makeIdentity()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (i == j)
                {
                    mat[i][j] = 1;
                }
                else
                    mat[i][j] = 0;
            }
        }
    }
};

double dotProduct(point a, point b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

point crossProduct(point a, point b)
{
    point c;
    c.x = (a.y * b.z) - (a.z * b.y);
    c.y = (a.z * b.x) - (a.x * b.z);
    c.z = (a.x * b.y) - (b.x * a.y);
    return c;
}

point normalize(point p)
{
    point q;
    double d = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    q.x = p.x / d;
    q.y = p.y / d;
    q.z = p.z / d;
    return q;
}

point mulPoint(point p, double d)
{
    point q;
    q.x = p.x * d;
    q.y = p.y * d;
    q.z = p.z * d;
    return q;
}

point addition(point p, point q)
{
    p.x = p.x + q.x;
    p.y = p.y + q.y;
    p.z = p.z + q.z;
    return p;
}

Matrix mulMatrix(Matrix a, double b)
{
    Matrix c(a.row, a.col);
    for (int i = 0; i < a.row; i++)
    {
        for (int j = 0; j < a.col; j++)
        {
            //c.mat[i][j] = 0;
            c.mat[i][j] = a.mat[i][j] / b;
        }
    }
    return c;
}

Matrix MatrixMultiply(Matrix a, Matrix b)
{
    Matrix c(a.row, b.col);
    for (int i = 0; i < a.row; i++)
    {
        for (int j = 0; j < b.col; j++)
        {
            c.mat[i][j] = 0;
            for (int k = 0; k < c.col; k++)
                c.mat[i][j] += a.mat[i][k] * b.mat[k][j];
        }
    }
    return c;
}

point RodriguesFormula(double angle, point a, point x)
{
    /*cout << fixed << showpoint;
    cout << setprecision(2);*/
    double theta = angle * pi / 180;
    //point c = mulPoint(x, cos(theta));
    point c = mulPoint(a, cos(theta));
    double w = dotProduct(a, x);
    //point w1 = mulPoint(a, w);
    point w1 = mulPoint(x, w);
    point cc = mulPoint(w1, 1 - cos(theta));
    point w2 = addition(c, cc);
    point pp = crossProduct(a, x);
    point ppp = mulPoint(pp, sin(theta));
    point c1 = addition(w2, ppp);
    return c1;
}

Matrix MatrixAddition(Matrix a, Matrix b)
{
    Matrix c(a.row, b.col);
    for (int i = 0; i < a.row; i++)
    {
        for (int j = 0; j < b.col; j++)
        {
            //c.mat[i][j] = 0;
            c.mat[i][j] = a.mat[i][j] + b.mat[i][j];
        }
    }
    return c;
}

int main()
{
    cout << fixed << showpoint;
    cout.precision(7);
    //point look, up, eye;
    double fovY, aspectRatio, near, far;
    FILE *inputFile, *outputFile;
    Matrix I(dim, dim);
    I.makeIdentity();
    stack<int> pushTrace;
    stack<Matrix> st;
    vector<Matrix> vec;
    st.push(I);
    double eye[3];
    double look[3];
    double up[3];
    //st.top().PrintMatrix();
    inputFile = fopen("scene.txt", "r");
    //inputFile = fopen("scene3.txt", "r");
    //inputFile = fopen("scene2.txt", "r");
    for (int i = 0; i < 3; i++)
    {
        fscanf(inputFile, "%lf", &eye[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        fscanf(inputFile, "%lf", &look[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        fscanf(inputFile, "%lf", &up[i]);
    }
    /* fscanf(inputFile, "%lf %lf %lf", &eye.x, &eye.y, &eye.z);
    fscanf(inputFile, "%lf %lf %lf", &look.x, &look.y, &look.z);
    fscanf(inputFile, "%lf %lf %lf", &up.x, &up.y, &up.z);*/
    fscanf(inputFile, "%lf %lf %lf %lf", &fovY, &aspectRatio, &near, &far);
    freopen("stage1.txt","w",stdout);
    char command[30];
    while (fscanf(inputFile, "%s", command) != EOF)
    {
        if (!strcmp(command, "triangle"))
        {
            //cout<<command<<endl;
            //break;
            Matrix CoOrd1(4, 4);
            Matrix CoOrd2(4, 4);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    fscanf(inputFile, "%lf", &CoOrd1.mat[j][i]);
                }
            }
            for (int p = 0; p < 4; p++)
            {
                CoOrd1.mat[3][p] = 1;
            }
            Matrix t = st.top();
            CoOrd2 = MatrixMultiply(t, CoOrd1);
            vec.push_back(CoOrd2);
            //CoOrd2.PrintMatrix();
            CoOrd2.PrintTriangle();
            /*for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cout << CoOrd2.mat[j][i] << " ";
                }
                cout << endl;
            }
            cout << endl;*/
            //break;
        }
        else if (!strcmp(command, "scale"))
        {
            Matrix sc(4, 4);
            Matrix t = st.top();
            st.pop();//new
            for (int i = 0; i < 3; i++)
            {
                fscanf(inputFile, "%lf", &sc.mat[i][i]);
            }
            sc.mat[3][3] = 1;
            t = MatrixMultiply(t, sc);
            st.push(t);
            //t.PrintMatrix();
            cout << endl;
        }
        else if (!strcmp(command, "translate"))
        {
            Matrix tr(4, 4);
            tr.makeIdentity();
            Matrix t = st.top();
            st.pop();
            for (int i = 0; i < 3; i++)
            {
                fscanf(inputFile, "%lf", &tr.mat[i][3]);
            }
            //tr.mat[3][3] = 1;
            //tr.PrintMatrix();
            Matrix xx(4, 4);
            xx = MatrixMultiply(t, tr);
            st.push(xx);
            //t.PrintMatrix();
            //break;
        }
        else if (!strcmp(command, "rotate"))
        {
            double alpha, ax, ay, az;
            point i;
            point j;
            point k;
            initpoint(i);
            initpoint(j);
            initpoint(k);
            i.x = j.y = k.z = 1;
            //alpha = 15*(acos(-1.0)/180.0) ;

            fscanf(inputFile, "%lf %lf %lf %lf", &alpha, &ax, &ay, &az);
            point a;
            a.x = ax;
            a.y = ay;
            a.z = az;
            point aa = normalize(a);
            point c[3];
            c[0] = RodriguesFormula(alpha, i, aa);
            c[1] = RodriguesFormula(alpha, j, aa);
            c[2] = RodriguesFormula(alpha, k, aa);
            /* cout << c[0].x << " " << c[0].y << " " << c[0].z << endl;
            cout << c[1].x << " " << c[1].y << " " << c[1].z << endl;
            cout << c[2].x << " " << c[2].y << " " << c[2].z << endl;*/
            Matrix mm(4, 4);
            for (int i = 0; i < 3; i++)
            {
                mm.mat[i][0] = c[i].x;
                mm.mat[i][1] = c[i].y;
                mm.mat[i][2] = c[i].z;
            }
            mm.mat[3][3] = 1;
            //mm.PrintMatrix();
            Matrix tt(4, 4);
            //cout<<cos(pi/2)<<endl;
            Matrix t = st.top();
            st.pop();
            tt = MatrixMultiply(t, mm);
            st.push(tt);
            //tt.PrintMatrix();
            //break;
        }
        else if (!strcmp(command, "push"))
        {
            //pushTrace.push(st.size());
            st.push(st.top());
        }
        else if (!strcmp(command, "pop"))
        {
            /*int index = pushTrace.top();
            pushTrace.pop();
            for(int i = 0; i < (int)st.size()-index; i++){
                st.pop();
            }*/
            st.pop();
        }
        else if (!strcmp(command, "end"))
        {
            break;
        }
    }
    fclose(inputFile);
    //fclose(outputFile);
    freopen("stage2.txt","w",stdout);
    double l[3];
    for (int i = 0; i < 3; i++)
    {
        l[i] = look[i] - eye[i];
    }
    point ll;
    ll.x = l[0];
    ll.y = l[1];
    ll.z = l[2];
    ll = normalize(ll);
    point up1;
    up1.x = up[0];
    up1.y = up[1];
    up1.z = up[2];
    point r = crossProduct(ll, up1);
    r = normalize(r);
    point u = crossProduct(r, ll);
    Matrix T(4, 4);
    T.makeIdentity();
    for (int i = 0; i < 3; i++)
    {
        T.mat[i][3] = -eye[i];
    }

    //T.PrintMatrix();

    Matrix R(4, 4);
    //R.makeIdentity();
    R.mat[0][0] = r.x;
    R.mat[0][1] = r.y;
    R.mat[0][2] = r.z;

    R.mat[1][0] = u.x;
    R.mat[1][1] = u.y;
    R.mat[1][2] = u.z;

    R.mat[2][0] = -ll.x;
    R.mat[2][1] = -ll.y;
    R.mat[2][2] = -ll.z;

    R.mat[3][3] = 1;
    //R.PrintMatrix();

    Matrix view(4, 4);
    view = MatrixMultiply(R, T);

    //    view.PrintMatrix();
    vector<Matrix> resView;
    for (int i = 0; i < vec.size(); i++)
    {
        Matrix res(4, 4);
        res = MatrixMultiply(view, vec[i]);
        res.PrintTriangle();
        resView.push_back(res);
    }
    //fclose(outputFile);
    freopen("stage3.txt","w",stdout);
    double t0, r0, fovX;
    fovX = fovY * aspectRatio;
    fovX = fovX*pi /180;
    fovY = fovY*pi /180;
    t0 = near * tan(fovY / 2);
    r0 = near * tan(fovX / 2);

    Matrix Proj(4, 4);
    //Proj.makeIdentity();
    Proj.mat[0][0] = near / r0;
    Proj.mat[1][1] = near / t0;
    Proj.mat[2][2] = -(far + near) / (far - near);
    Proj.mat[2][3] = -(2 * far * near) / (far - near);
    Proj.mat[3][2] = -1;
    //Proj.mat[3][3] = 1;
    //Proj.PrintMatrix();

    for (int i = 0; i < resView.size(); i++)
    {
        Matrix result(4, 4);
        result = MatrixMultiply(Proj, resView[i]);
        result.PrintTriangle();
    }
    fclose(outputFile);
    return 0;
}
