#include<stdio.h>
#include<GL/glut.h>
#include<stdlib.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include<time.h>

#include<math.h>
#include<string>

#define PI 3.1416
#define ESCAPE 27

using namespace std;

void rock();
void display1();
void display2();
void display3();
void ship();
void broken_ship();
void water();
void smoke();
void drawCloud(float x, float y, float scale);
void DrawCloud(float x, float y, float scale);
void drawSky();
void DrawSky();
void DrawCircle();
void drawCircle();


struct point {
    GLfloat x,y,z;
};
void drawCloud(float x, float y, float scale);
void drawSky();
void DrawCircle(float cx, float cy, float r, int num_segments);
void floatingWater();
int factorial(int);
void computeNcR(int, int*);
void computeBezier(float, point *, int, point *, int *);
void bezier(point *, int, int);

void text(int, int, string, int);
void myMouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void menu();
void initialiseValues();
GLfloat getX(GLfloat, GLfloat, GLfloat, GLfloat);
//BOOLEAN nanosleep(LONGLONG);

const int NUM_CLOUDS = 5;
float cloudPositions[NUM_CLOUDS][2];
struct timespec jmp,jmp2;

GLfloat a,b,c,d,e,f,g,h,x,i,velocity;
int compartments;
bool main_menu, start, right, crashed;

GLfloat mid_height[] = {0.0f, 0.0f, 0.0f};
GLfloat mid_hor[] = {0.0f, 0.0f, 0.0f};

GLfloat mid_ring_height[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat mid_ring_hor[] = {0.0f, 0.0f, 0.0f, 0.0f};

float theta;



void calculate_mid_vals() {
    for (int i=1; i<=3; i++) {
        mid_height[i-1] = 3.2*i + 0.6*(i-1) + 5.0;
        mid_hor[i-1] = getX(mid_height[i-1], 5.0, 20.0, -1.16667);
    }
}



void calculate_mid_ring_vals() {
    for (int i=0; i<2; i++) {
        mid_ring_height[2*i] = (i+1)*(3.2) + 5.0 + i*0.6;
        mid_ring_height[2*i + 1] = mid_ring_height[2*i] + 0.6;

        mid_ring_hor[2*i] = getX(mid_ring_height[2*i], 5.0, 20.0, -1.16667);
        mid_ring_hor[2*i + 1] = getX(mid_ring_height[2*i + 1], 5.0, 20.0, -1.16667);
    }
}



void initialiseValues() {
    compartments=2;
    a=0;    b=-250;    c=0;    d=0;    e=0;    f=0;
    g=500;  h=600;
    x=0;    i=0;
    velocity=1;
    main_menu = true;
    start = false;
    right = true;
    crashed = false;
    theta = 0;

    calculate_mid_vals();   calculate_mid_ring_vals();
}
void initializeClouds() {
    for (int i = 0; i < NUM_CLOUDS; i++) {
        cloudPositions[i][0] = rand() % 1000;  // Random x position
        cloudPositions[i][1] = 600 + rand() % 150;  // Random y position between 600 and 750
    }
}

void update(int value)
{
    if (start) {
        if(right || crashed) a+=velocity;
        else if(a>0) a-=velocity;
        else a=0;
    }

    // Move clouds
    for (int i = 0; i < NUM_CLOUDS; i++) {
        cloudPositions[i][0] -= 0.5;  // Move clouds to the left
        if (cloudPositions[i][0] < -50) {
            cloudPositions[i][0] = 1050;  // Reset cloud position when it goes off-screen
        }
    }

    glutPostRedisplay();
    glutTimerFunc(10,update,0);
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();  // Always draw the sky

    if (main_menu) {
        menu();
    }
    else if ((compartments==1 && x<-80) || (compartments==2 && x<-90) || (compartments==3 && x<-105)) {
        printf("The ship has sunk completely\n");
        initialiseValues();
    }
    else {
        if(a<900) {
            display1();
            b = -250;
        }
        if(a>900 && b<180) {
            if(start && right) b+=velocity;
            else if(start) b-=velocity;
            display2();
        }
        if(b==180) {
            crashed = true;
            start = true;
            b = 181;
            jmp.tv_sec = 0;
            jmp.tv_nsec = 25000000L;
            nanosleep(&jmp , &jmp2);
            display3();
        }
        if(b>180) {
            crashed = true;
            b+=(velocity/4);
            c+=(3*velocity/4);
            x-=(velocity/20);
            velocity += 0.001;
            display3();
        }
    }

    glFlush();
    glutSwapBuffers();
}


void display1()
{
    // Remove glClear(GL_COLOR_BUFFER_BIT) if it exists here
    glPushMatrix();
    glTranslated(a,75,0.0);
    ship();
    glPopMatrix();
    floatingWater();
}

void display2()
{
    // Remove glClear(GL_COLOR_BUFFER_BIT) if it exists here
    rock();
    glPushMatrix();
    glTranslated(b,75,0.0);
    ship();
    glPopMatrix();
    floatingWater();
}

void display3()
{
    // Remove glClear(GL_COLOR_BUFFER_BIT) if it exists here
    rock();
    glPushMatrix();
    glTranslated(b,100+x*5,0.0);
    glRotated(-0.5,0,0,1);
    broken_ship();
    glPopMatrix();
    floatingWater();
}



void text(int x, int y, string s, int font) {
	int i=0;
	glColor3f(0.0,0.0,0.8);                  //instruction text color
	glRasterPos2f(x,y);
	for(i=0;i<s.length();i++) {
		if(font==1)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,s[i]);
		else if(font==2)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[i]);
		else if(font==3) {
			glColor3f(1.0,0.0,0.0);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[i]);
		}
	}
}



int factorial(int n) {
    if(n<=1) return 1;
    return n*factorial(n-1);
}



void computeNcR(int n, int *hold_ncr_values) {
    int r;
    for(r=0; r<=n; r++) hold_ncr_values[r] = factorial(n) / (factorial(n-r) * factorial(r));
}



void computeBezier(float t, point *actual_bezier_point, int n_main, point *control_points_array, int *hold_ncr_values) {
    int i, n = n_main-1;
    float bernstein_polynomial;
    actual_bezier_point -> x = 0;
    actual_bezier_point -> y = 0;
    actual_bezier_point -> z = 0;
    for(i=0; i<n_main; i++) {
        bernstein_polynomial = hold_ncr_values[i] * pow(t,i) * pow(1-t,n-i);
        actual_bezier_point -> x += bernstein_polynomial * control_points_array[i].x;
        actual_bezier_point -> y += bernstein_polynomial * control_points_array[i].y;
        actual_bezier_point -> z += bernstein_polynomial * control_points_array[i].z;
    }
}



void bezier(point *control_points_array, int number_of_control_points, int number_of_bezier_points) {
    point actual_bezier_point;
    float t;
    int *hold_ncr_values, i;
    hold_ncr_values = new int[number_of_control_points];
    computeNcR(number_of_control_points-1, hold_ncr_values);

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<=number_of_bezier_points; i++) {
        t = float(i)/float(number_of_bezier_points);
        computeBezier(t, &actual_bezier_point, number_of_control_points, control_points_array, hold_ncr_values);
        glVertex2f(actual_bezier_point.x, actual_bezier_point.y);
    }
    glEnd();
    delete[] hold_ncr_values;
}



void floatingWater() {
    int number_of_control_points=4, number_of_bezier_points=20;
    float water_height = 125;
    point control_points_array[number_of_control_points] =
                {{-250,water_height,0},{250,water_height+25,0},{750,water_height-25,0},{1250,water_height,0}};
    control_points_array[1].x += 50*sin(theta*PI/180.0);
    control_points_array[1].y += 25*sin(theta*PI/180.0);
    control_points_array[2].x -= 50*sin((theta+30)*PI/180.0);
    control_points_array[2].y -= 50*sin((theta+30)*PI/180.0);
    control_points_array[3].x -= 25*sin((theta-30)*PI/180.0);
    control_points_array[3].y += sin((theta-30)*PI/180.0);
    theta += 2;
    glPushMatrix();
    glPointSize(5);
    glEnable(GL_BLEND);
    glBlendFunc(1, 0.5);
    glColor3f(0.196078,0.6,0.8);         //water color
    for(int i=0; i<200; i++) {
        glTranslated(0,-0.8,0);
        bezier(control_points_array, number_of_control_points, number_of_bezier_points);
    }
    glPopMatrix();
}



void keyboard(unsigned char key, int x, int y) {

	printf("Keyboard Log : --%c--\n", key);

    if(key==ESCAPE) exit(1);
    if(crashed) return;

	switch(key) {

		case ' ':
			if (!main_menu) start = !start;
			break;

        case 'd':
            right = true;
            start = true;
            break;

        case 'a':
            right = false;
            start = true;
            break;

        case '+':
            if(velocity<3 && start) velocity += 1;
            break;

        case '-':
            if(velocity>1) velocity -= 1;
            else if (velocity==1) start = false;
            break;

        case ',':
            if(compartments>1) compartments--;
            break;

        case '.':
            if(compartments<3) compartments++;
            break;
	}
}



void myMouse(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_UP && main_menu) {
        main_menu = false;
	}
	glutPostRedisplay();
}



void menu() {
	//glClearColor(1.0,1.0,0.6,1.0);
	text(410,700,"SINKING SHIP",1);
	text(430,660,"Using OpenGL",2);
	text(450,600,"Made By:",2);
	text(270,550,"Shivaprasad      Sumanth        RinuPriya        Sahana ",1);
	text(270,520,"4CA21CS090      4CA21CS100      4CA21CS080      4CA21CS082",2);
    text(530,440,"SHIP MOVEMENT :",3);
	text(530,400,"Press SpaceBar to Start / Stop the ship",2);
	text(530,360,"Press '+' to increase ship's speed",2);
    text(530,320,"Press '-' to decrease ship's speed",2);
    text(530,280,"Press 'd' to move the ship Forward",2);
    text(530,240,"Press 'a' to Reverse the ship",2);
    text(530,200,"Press ESC to exit at any time",3);

text(90, 440, "SHIP COMPARTMENT :", 3);
    text(90,400,"Press ',' to Increase the compartments of the ship",2);
    text(90,360,"Press '.' to Decrease the compartments of the ship",2);
	text(250,80,"LEFT CLICK ON YOUR MOUSE TO CONTINUE",1);
	glutPostRedisplay();
}



GLfloat getX(GLfloat y2, GLfloat y1, GLfloat x1, GLfloat m) {
    return (y2-y1+(m*x1))/m;
}



void ship()
{
	glScaled(24,24,0);

	glColor3f(0.329412,0.329412,0.329412); // main base of ship
	glBegin(GL_POLYGON);
		glVertex2f(0.5,5.0);
		glVertex2f(3,1);
		glVertex2f(22,1);
		glVertex2f(24.0,5.0);
	glEnd();

	glColor3f(1.0,1.0,1.0); // base ring(ship middle line)
	glBegin(GL_POLYGON);
		glVertex2f(1.35,3.5);
		glVertex2f(1.6,3.2);
		glVertex2f(23.1,3.2);
		glVertex2f(23.25,3.5);
	glEnd();

	glColor3f(0.329412,0.329412,0.329412); // base extra
	glBegin(GL_POLYGON);
		glVertex2f(21.0,5.0);
		glVertex2f(21.0,6.0);
		glVertex2f(24.5,6.0);
		glVertex2f(24.0,5.0);
	glEnd();

	glColor3f(0.74902,0.847059,0.847059); // mid containing windows
	glBegin(GL_POLYGON);
		glVertex2f(2.0,5.0);
		glVertex2f(2.0,mid_height[compartments-1]);
		glVertex2f(mid_hor[compartments-1],mid_height[compartments-1]);
		glVertex2f(20.0,5.0);
	glEnd();

	glColor3f(0.137255,0.137255,0.556863); // ring between windows
	for (int x=0; x<compartments-1; x++) {
        glBegin(GL_POLYGON);
		    glVertex2f(2.0,mid_ring_height[2*x]);
		    glVertex2f(2.0,mid_ring_height[2*x + 1]);
		    glVertex2f(mid_ring_hor[2*x + 1],mid_ring_height[2*x + 1]);
		    glVertex2f(mid_ring_hor[2*x],mid_ring_height[2*x]);
	    glEnd();
    }

	glColor3f(0.90,0.91,0.98); // windows
    int no_of_windows = 9;
    GLfloat window_x = 3.0, window_y = 6.0;
    for (int x=0; x<compartments; x++) {
        for (int y=0; y<no_of_windows; y++) {
            glBegin(GL_POLYGON);
                glVertex2f(window_x, window_y);
                glVertex2f(window_x, window_y+1.2);
                glVertex2f(window_x+1.0, window_y+1.2);
                glVertex2f(window_x+1.0, window_y);
            glEnd();
            window_x += 1.7;
        }
        no_of_windows -= 2;
        window_x = 3.0;
        window_y += 3.8;
    }

	glColor3f(0.329412,0.329412,0.329412); //top-cover
	glBegin(GL_POLYGON);
		glVertex2f(1.5, mid_height[compartments-1]);
		glVertex2f(1.5, mid_height[compartments-1]+0.5);
		glVertex2f(mid_hor[compartments-1] + 0.5, mid_height[compartments-1]+0.5);
		glVertex2f(mid_hor[compartments-1] + 0.5, mid_height[compartments-1]);
	glEnd();


    GLfloat chim_start = 2.5;
    for (int i=0; i<2; i++) {      //number of exhaust(i<2)
        glColor3f(0.0,0.0,0.0); // chim
	    glBegin(GL_POLYGON);
		    glVertex2f(chim_start, mid_height[compartments-1] + 0.5);
		    glVertex2f(chim_start, mid_height[compartments-1] + 4.0);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 4.0);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 0.5);
	    glEnd();

        glColor3f(1.0,0.25,0.0); // main ring
	    glBegin(GL_POLYGON);
		    glVertex2f(chim_start, mid_height[compartments-1] + 0.5);
		    glVertex2f(chim_start, mid_height[compartments-1] + 1.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 1.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 0.5);
	    glEnd();

        glColor3f(1.0,0.25,0.0); // another ring
	    glBegin(GL_POLYGON);
		    glVertex2f(chim_start, mid_height[compartments-1] + 2.5);
		    glVertex2f(chim_start, mid_height[compartments-1] + 3.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 3.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 2.5);
	    glEnd();

        chim_start += (3.5);
    }

}



void broken_ship()
{
	glScaled(24,24,0);

    if(x>-90) {
        glTranslated(12,3,0.0);
	    glRotated(x/2,0,0,1);
	    glTranslated(-12,-3,0.0);
    }

	glColor3f(0.329412,0.329412,0.329412); // main base (while sinking)
	glBegin(GL_POLYGON);
		glVertex2f(0.5,5.0);
		glVertex2f(3,1);
		glVertex2f(21,1);
        glVertex2f(20.5,3.2);
        glVertex2f(20.6,3.5);
		glVertex2f(21,5.0);
	glEnd();

	glColor3f(1.0,1.0,1.0); // base ring
	glBegin(GL_POLYGON);
		glVertex2f(1.35,3.5);
		glVertex2f(1.6,3.2);
		glVertex2f(20.49,3.2);
        glVertex2f(20.5,3.3);
		glVertex2f(20.535,3.5);
	glEnd();

	glColor3f(0.74902,0.847059,0.847059); // mid containing windows
	glBegin(GL_POLYGON);
		glVertex2f(2.0,5.0);
		glVertex2f(2.0,mid_height[compartments-1]);
		glVertex2f(mid_hor[compartments-1],mid_height[compartments-1]);
		glVertex2f(20.0,5.0);
	glEnd();

	glColor3f(0.137255,0.137255,0.556863); // ring
	for (int x=0; x<compartments-1; x++) {
        glBegin(GL_POLYGON);
		    glVertex2f(2.0,mid_ring_height[2*x]);
		    glVertex2f(2.0,mid_ring_height[2*x + 1]);
		    glVertex2f(mid_ring_hor[2*x + 1],mid_ring_height[2*x + 1]);
		    glVertex2f(mid_ring_hor[2*x],mid_ring_height[2*x]);
	    glEnd();
    }

	glColor3f(0.90,0.91,0.98); // windows
    int no_of_windows = 9;
    GLfloat window_x = 3.0, window_y = 6.0;
    for (int x=0; x<compartments; x++) {
        for (int y=0; y<no_of_windows; y++) {
            glBegin(GL_POLYGON);
                glVertex2f(window_x, window_y);
                glVertex2f(window_x, window_y+1.2);
                glVertex2f(window_x+1.0, window_y+1.2);
                glVertex2f(window_x+1.0, window_y);
            glEnd();
            window_x += 1.7;
        }
        no_of_windows -= 2;
        window_x = 3.0;
        window_y += 3.8;
    }

	glColor3f(0.329412,0.329412,0.329412); //top-cover
	glBegin(GL_POLYGON);
		glVertex2f(1.5, mid_height[compartments-1]);
		glVertex2f(1.5, mid_height[compartments-1]+0.5);
		glVertex2f(mid_hor[compartments-1] + 0.5, mid_height[compartments-1]+0.5);
		glVertex2f(mid_hor[compartments-1] + 0.5, mid_height[compartments-1]);
	glEnd();


    GLfloat chim_start = 2.5;
    for (int i=0; i<2; i++) {    //number of exhaust(i<2)
        glColor3f(0.0,0.0,0.0); // chim(while sinking)
	    glBegin(GL_POLYGON);
		    glVertex2f(chim_start, mid_height[compartments-1] + 0.5);
		    glVertex2f(chim_start, mid_height[compartments-1] + 4.0);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 4.0);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 0.5);
	    glEnd();

        glColor3f(1.0,0.25,0.0); // main ring
	    glBegin(GL_POLYGON);
		    glVertex2f(chim_start, mid_height[compartments-1] + 0.5);
		    glVertex2f(chim_start, mid_height[compartments-1] + 1.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 1.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 0.5);
	    glEnd();

        glColor3f(1.0,0.25,0.0); // another ring
	    glBegin(GL_POLYGON);
		    glVertex2f(chim_start, mid_height[compartments-1] + 2.5);
		    glVertex2f(chim_start, mid_height[compartments-1] + 3.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 3.5);
		    glVertex2f(chim_start + 2.5, mid_height[compartments-1] + 2.5);
	    glEnd();

        chim_start += (3.5);
    }


    glTranslated(12,3,0.0);
	glRotated(-x/10,0,0,1);
	glTranslated(-12,-3,0.0);

	glColor3f(0.329412,0.329412,0.329412); // other part of main base
	glBegin(GL_POLYGON);
		glVertex2f(21,1);
        glVertex2f(20.5,3.2);
        glVertex2f(20.6,3.5);
		glVertex2f(21,5.0);
		glVertex2f(24.0,5.0);
		glVertex2f(22,1);
	glEnd();

	glColor3f(1.0,1.0,1.0); // other part of base ring
	glBegin(GL_POLYGON);
        glVertex2f(20.6,3.5);
		glVertex2f(20.5,3.2);
		glVertex2f(23.1,3.2);
		glVertex2f(23.25,3.5);
	glEnd();

	glColor3f(0.329412,0.329412,0.329412); // base extra
	glBegin(GL_POLYGON);
		glVertex2f(21.0,5.0);
		glVertex2f(21.0,6.0);
		glVertex2f(24.5,6.0);
		glVertex2f(24.0,5.0);
	glEnd();

}

void drawCloud(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    glColor3f(1.0, 1.0, 1.0);  // White color for clouds
    DrawCircle(0, 0, 20, 32);
    DrawCircle(-15, -5, 15, 32);
    DrawCircle(15, -5, 15, 32);
    DrawCircle(-25, 5, 10, 32);
    DrawCircle(25, 5, 10, 32);

    glPopMatrix();
}

// Add this function to draw the sky with clouds
void drawSky() {
    // Draw sky background
    glColor3f(0.529, 0.808, 0.922);  // Light blue color
    glBegin(GL_QUADS);
    glVertex2f(0, 799);
    glVertex2f(999, 799);
    glVertex2f(999, 125);
    glVertex2f(0, 125);
    glEnd();

    // Draw clouds
    for (int i = 0; i < NUM_CLOUDS; i++) {
        drawCloud(cloudPositions[i][0], cloudPositions[i][1], 1.0);
    }
}

void DrawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
        for (int i=0; i<num_segments; i++) {
            float theta = 2.0f * PI * float(i)/float(num_segments);

            float x = r * cosf(theta);
            float y = r * sinf(theta);

            glVertex2f(x + cx, y + cy);
        }
    glEnd();
}

void DrawCloud(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    glColor3f(1.0, 1.0, 1.0);  // White color for clouds
    DrawCircle(0, 0, 20, 32);
    DrawCircle(-15, -5, 15, 32);
    DrawCircle(15, -5, 15, 32);
    DrawCircle(-25, 5, 10, 32);
    DrawCircle(25, 5, 10, 32);

    glPopMatrix();
}

// Add this function to draw the sky with clouds
void DrawSky() {
    // Draw sky background
    glColor3f(0.529, 0.808, 0.922);  // Light blue color
    glBegin(GL_QUADS);
    glVertex2f(0, 799);
    glVertex2f(999, 799);
    glVertex2f(999, 125);
    glVertex2f(0, 125);
    glEnd();

    // Draw clouds
    for (int i = 0; i < NUM_CLOUDS; i++) {
        drawCloud(cloudPositions[i][0], cloudPositions[i][1], 1.0);
    }
}

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
        for (int i=0; i<num_segments; i++) {
            float theta = 2.0f * PI * float(i)/float(num_segments);

            float x = r * cosf(theta);
            float y = r * sinf(theta);

            glVertex2f(x + cx, y + cy);
        }
    glEnd();
}

void rock()
{
	glPushMatrix();
	glTranslated(600,50,0.0);
	glScaled(10,10,0);
	glColor3f(0.36,0.25,0.20);  //rock color

	if(c>0) {
		glPushMatrix();
		glTranslated(0,x,0);
		glPushMatrix();
		glTranslated(7,2,0.0);
		glRotated(-x,0,0,1);
		glTranslated(-7,-2,0.0);

		glBegin(GL_POLYGON);
		glVertex2f(8.1,1);
		glVertex2f(7.89,1.23);
		glVertex2f(11.26,18);
		glVertex2f(13.23,21.33);
		glVertex2f(14.23,2.53);
		glEnd();

		glPopMatrix();
		glPushMatrix();

		glTranslated(12,3,0.0);
		glRotated(x,0,0,1);
		glTranslated(-12,-3,0.0);

		glBegin(GL_POLYGON);
		glVertex2f(11.1,3.5);
		glVertex2f(13.2,18.7);
		glVertex2f(16.4,18.5);
		glVertex2f(18.12,3.65);
		glVertex2f(19.21,1.25);
		glEnd();

		glPopMatrix();
		glPopMatrix();
	}
	else {
		glBegin(GL_POLYGON);
		glVertex2f(8.1,1);
		glVertex2f(7.89,1.23);
		glVertex2f(11.26,18);
		glVertex2f(13.23,21.33);
		glVertex2f(14.23,2.53);
		glEnd();
			glBegin(GL_POLYGON);
			glVertex2f(11.1,3.5);
			glVertex2f(13.2,18.7);
			glVertex2f(16.4,18.5);
			glVertex2f(18.12,3.65);
			glVertex2f(19.21,1.25);
			glEnd();
	}

	glPopMatrix();

}

void myinit()
{
    glClearColor(0.529, 0.808, 0.922, 1.0);  // Light blue color for sky
    glColor3f(1.0,0.0,0.0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,999.0,0.0,799.0);
}



/*
BOOLEAN nanosleep(LONGLONG ns){
	HANDLE timer;
	LARGE_INTEGER li;
	if(!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return FALSE;
	li.QuadPart = -ns;
	if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)){
		CloseHandle(timer);
		return FALSE;
	}
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	return TRUE;
}
*/



int main(int argc, char* argv[]) {
    int chs;
    initialiseValues();
    initializeClouds();  // Add this line to initialize cloud positions
    printf("keys pressed are\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024.0,768.0);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Sinking Ship");
    glutDisplayFunc(display);
    myinit();
    glutKeyboardFunc(keyboard);
    glutMouseFunc(myMouse);
    glutTimerFunc(250,update,0);
    glutMainLoop();
    return 0;
}
