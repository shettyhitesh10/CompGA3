#include <iostream>//includes the standard library header file
#include<vector>
#include<GL/Glut.h> //includes the opengl, glu, and glut header files
using namespace std;
//global variables
float r, g, b, x, y;
float cx, cy;//current point
int drag = 0;//to know the point being dragged (it treats overlapping points as separate otherwise)
vector<int> overlaps;

vector<vector<float>> points;
int c = 0;//counter for number of points
bool check = true;

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

        for(int i = 1; i < c; i++) {
            if (points[i][0] == x && points[i][1] == y) {//checking if the right click is on a point.
                points[i][0] = points[i - 1][0];//instead of shifting the whole array, we overlap the deleted point with the previous one
                points[i][1] = points[i - 1][1];
            }
        }
        r = 0;
        g = 0;
        b = 0;
        
    }
    
    glutPostRedisplay();
}

void cursorPos(int x, int y) {//get postion of curson when any of the mouse buttons are pressed(in action eg. draging)
    //cout << x <<" "<< y << endl;
    cx = (x / 15) * 15 + 7.5;
    cy = 480-(y / 15) * 15 - 7.5; 
}

void drawLine(void) {
    glColor3f(1, 0, 0);
    glPointSize(25);
    
    glMatrixMode(GL_PROJECTION);// sets the current matrix to projection
    glLoadIdentity();//multiply the current matrix by identity matrix
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);

    
    glBegin(GL_LINES);
    for (int i = 1; i < c; i++) {
        glVertex2f(points[i-1][0], points[i-1][1]);
        glVertex2f(points[i][0], points[i][1]);
    //glVertex2i(50,20);
    //glVertex2i(150,60);
    }
    glEnd();
    glFlush();
    cout<<"hello"<<endl;
}

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
    
}


int main(int argc, char** argv)
{
    points.resize(1000);
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