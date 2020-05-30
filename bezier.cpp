/*
Bezier curve

//curve - parts that make the bezier curve with lines

//points - ...with points

*/

#include <iostream>//includes the standard library header file
#include<vector>
#include<GL/Glut.h> //includes the opengl, glu, and glut header files
using namespace std;
//global variables
float r, g, b, x, y;
///current point
float cx, cy;
///checks if a point has been dragged
int drag = 0;
///to know the point being dragged (it treats overlapping points as separate otherwise)
vector<int> overlaps;

float bx0=0, by0=0;//b*0 variables have the last point so as to create the line in bezier curve //curve

vector<vector<float>> points;
int c = 0;///counter for number of points index
int counter = 0;///actual counter
bool check = true;
///returns factorial
double factorial(double n)
{
    if (n <= 1)
        return(1);
    else
        n = n * factorial(n - 1);
    return n;
}

///returns nCk binomial coefficient
double binomial_coff(double n, double k)
{
    double ans;
    ans = factorial(n) / (factorial(k) * factorial(n-k));
    return ans;
}

///tracks mouse actions
void mouse(int button, int state, int mousex, int mousey)
{
    
    //cout <<endl<<" this "<< button << ' ' << state << ' ' << mousex << ' ' << mousey << ' ' << drag<<endl;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        check = true;
        //cout << mousex << endl << mousey << endl;
        x = (mousex / 15) * 15 + 7.5;      //so that the points snap to a grid-like position
        y = 480 - (mousey / 15) * 15 - 7.5;        //prevents overlapping points
        if (c == 0) {
            points[c].push_back(x);
            points[c].push_back(y);
            c++;
            counter++;
        }
        int diff = 1;//to avoid loops by adding the same point multiple times
        for (int i = 0; i < c; i++) {
            if (points[i][0] == x && points[i][1] == y) {
                diff = 0;
                break;
            }
        }

        if (diff==1){
            points[c].push_back(x);
            points[c].push_back(y);
            c++;
            counter++;
        }

        

        r = 0;
        g = 0;
        b = 0;
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        x = (mousex / 15) * 15 + 7.5;      
        y = 480 - (mousey / 15) * 15 - 7.5;  
        overlaps.clear();//the overlap had to cleared or the previous overlap gets added to.
        for (int i = 0; i < c; i++) {
            if (points[i][0] == x && points[i][1] == y) {//checking if the click is on a point.
                drag = 1;
                overlaps.push_back(i);
                //cout << drag <<"middle down"<<endl;
            }
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP && drag != 0) {
        cout << endl << drag<<" "<<cx<<cy<<' '<<x<<y<<endl ;
        if (x != (mousex/15)*15+7.5 || y != 480-(mousey / 15)*15-7.5) {//if you try to move the point but to the same place : x here is from middle down and is compared to middle up
            for (int i = 0; i < overlaps.size(); i++) {
                points[overlaps[i]][0] = cx;//instead of shifting the whole array, we overlap the deleted point with the previous one
                points[overlaps[i]][1] = cy;
            }
        }
        drag = 0;
        
           
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//undo(clear)the drawing
    {
        
        x = (mousex / 15) * 15 + 7.5;       //so that the points snap to a grid-like position
        y = 480 - (mousey / 15) * 15 - 7.5; //prevents overlapping points
        int flag = 0;
        for(int i = 1; i < c; i++) {
            if (points[i][0] == x && points[i][1] == y) {//checking if the right click is on a point.
                points[i][0] = points[i - 1][0];//instead of shifting the whole array, we overlap the deleted point with the previous one
                points[i][1] = points[i - 1][1];
                flag = 1;
            }
        }
        if (counter > 1 && flag==1&& (points[0][0] != x || points[0][1] != y)) {//flag checks if the waas a point removed and counter contion makes sure nu. of points is > 0, another condition for point 0
            counter--;
        }
        r = 0;
        g = 0;
        b = 0;
        
    }
    
    glutPostRedisplay();
}



///cursor position when pressed(down)
void cursorPos(int x, int y) {//get postion of curson when any of the mouse buttons are pressed(in action eg. draging)
    //cout << x <<" "<< y << endl;
    cx = (x / 15) * 15 + 7.5;
    cy = 480-(y / 15) * 15 - 7.5; 
}

///draws all the line at once 
void drawLine() {
    glColor3f(1, 0, 0);
    glPointSize(25);
    
    glBegin(GL_LINES);
    for (int i = 1; i < c; i++) {
        glVertex2f(points[i-1][0], points[i-1][1]);
        glVertex2f(points[i][0], points[i][1]);
    }
    glEnd();

    glFlush();

    cout<<"hello"<<endl;
}

///Bezier curve for n(counter) points
void drawBezierGeneralized(vector<vector<float>> p, double t) {
    float bx1=0, by1=0;
    if (counter > 0) {
        bx1 = binomial_coff((float)(counter - 1), 0.0) * pow(t, 0.0) * pow((1 - t), (counter - 1)) * p[0][0];
        by1 = binomial_coff((float)(counter - 1), 0.0) * pow(t, 0.0) * pow((1 - t), (counter - 1)) * p[0][1];
    }
    if (bx0==0) {//line from 0,0 to first point
        bx0 = bx1;
        by0 = by1; 
    }

    int j=1;//overlap is the root of all problems
    for (int i = 1; i < c; i++)//have to go till i<c not counter , [[j : couter :: i : c]]
    {
        if(p[i-1][0]!=p[i][0] || p[i - 1][1] != p[i][1]){
        bx1 = bx1 + binomial_coff((float)(counter - 1), (float)j) * pow(t, (double)j) * pow((1 - t), (counter - 1 - j)) * p[i][0];
        by1 = by1 + binomial_coff((float)(counter - 1), (float)j) * pow(t, (double)j) * pow((1 - t), (counter - 1 - j)) * p[i][1];
        j++;
        }
    }
    //cout << c/*bx1 << " " << by1 << endl << x << y */<<counter<< endl;

    //curve
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex2f(bx0, by0);
    glVertex2f(bx1, by1);
    glEnd();

    /* //points
    glColor3f(0, t, 0);
    glPointSize(5);
    cout << j << "*" << bx0 << "*" << bx1 << endl;
    glBegin(GL_POINTS);
    glVertex2f(bx1, by1);
    glEnd();*/

    glFlush();
    bx0 = bx1;//makes sure b*0 variables have the last point so as to create the line //curve
    by0 = by1;//curve
}

///initiates display
void display(void)
{
    glClearColor(1,1,1,0);//clering screen every time to avoid ghost lines
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(r, g, b); // sets the current drawing (foreground) color to blue 
    glPointSize(15); // sets the size of points to be drawn (in pixels)

    glMatrixMode(GL_PROJECTION);// sets the current matrix to projection
    glLoadIdentity();//multiply the current matrix by identity matrix
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);//sets the parallel(orthographic) projection of the full frame buffer 

    for (int i = 0; i < c; i++) {
        glBegin(GL_POINTS); // writes pixels on the frame buffer with the current drawing color
        glVertex2f(points[i][0], points[i][1]);   // sets vertex
        glEnd();
    }
    glFlush();     // flushes the frame buffer to the screen
    drawLine();
    for (int i = 0; i < c; i++) {
        cout << points[i][0]<<"b";
    }
    bx0 = 0;//this is done so as to not start with bx0 as the last point and bx1 as the first creating a circle
    by0 = 0;//curve
    for (float t = 0.0; t <= 1.0; t += 0.02){
        drawBezierGeneralized(points, t);
    }

    
}


int main(int argc, char** argv)
{
    points.resize(100);
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);   //sets the width and height of the window in pixels
    glutInitWindowPosition(10, 10);//sets the position of the window in pixels from top left corner 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//creates a single frame buffer of RGB color capacity.
    glutCreateWindow("DDA Line Drawing");//creates the window as specified by the user as above.
   
    glClearColor(1, 1, 1, 0); // sets the backgraound color to white light
    glClear(GL_COLOR_BUFFER_BIT); // clears the frame buffer and set values defined in glClearColor() function call 
    glutDisplayFunc(display);//links the display event with the display event handler(display) 
    glutMouseFunc(mouse);//keyboard event handler
    glutMotionFunc(cursorPos);
    glutMainLoop();//loops the current event
}