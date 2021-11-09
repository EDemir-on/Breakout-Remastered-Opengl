#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include <time.h>
# define M_PI 3.14159265358979323846
using namespace std;

#define FPS 90
#define KEY_ESC 27

#define BRICKWIDTH 70
#define BRICKHEIGHT 30
#define BALLSPEED 3

class Brick {
public:
	int x;
	int y;
	bool isBroken;
	Brick() {}
	Brick(int x1, int y1, bool isBroken) {
		this->x = x1;
		this->y = y1;
		this->isBroken = isBroken;
	}
};

class Ball {
public:
	int x;
	int y;
	int radius;
	int direction;
	Ball() {}
	Ball(int x1, int y1, int r) {
		this->x = x1;
		this->y = y1;
		this->radius = r;
		this->direction = 45;
	}
	
};

class Bar {
public:
	int x1;
	int y1;	
	int x2;
	int y2;
	Bar() {}
	Bar(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->y1 = y1;	
		this->x2 = x2;
		this->y2 = y2;
	}
};

Brick bricks[100];
Ball theBall;
Bar theBar;

bool isBallNearBar() {
	int mX = (theBar.x1 + theBar.x2) / 2;
	int mY = (theBar.y1 + theBar.y2) / 2;
	
	int ballDistance = sqrt((mX - theBall.x) * (mX - theBall.x) + (mY - theBall.y) * (mY - theBall.y));	
	if (ballDistance < 100) {
		return true;
	}	
	return false;
}
int distance(Brick b) {
	int dx = max(b.x - theBall.x, theBall.x - b.x+BRICKWIDTH);
	int dy = max(b.y - theBall.y, theBall.y - b.y + BRICKHEIGHT);
	return sqrt(dx * dx + dy * dy);
}

bool CheckCollision(Brick b) 
{
	int theBallDistanceX = abs(theBall.x - b.x);
	int theBallDistanceY = abs(theBall.y - b.y);

	if (theBallDistanceX > (BRICKWIDTH / 2 + theBall.radius)) { return false; }
	if (theBallDistanceY > (BRICKHEIGHT / 2 + theBall.radius)) { return false; }

	if (theBallDistanceX <= (BRICKWIDTH / 2)) { return true; }
	if (theBallDistanceY <= (BRICKHEIGHT / 2)) { return true; }

	int cornerDistance_sq = (theBallDistanceX - BRICKWIDTH / 2)* (theBallDistanceX - BRICKWIDTH / 2) +	(theBallDistanceY - BRICKHEIGHT / 2) * (theBallDistanceY - BRICKHEIGHT / 2);

	return (cornerDistance_sq <= (theBall.radius * theBall.radius));
}

void SetNewBallPos() {
	int newX;
	int newY;
	float radians = (theBall.direction * M_PI) / 180;
	newX = BALLSPEED *sin(radians);
	newY = BALLSPEED *cos(radians);
	theBall.x = theBall.x + newX;
	theBall.y = theBall.y + newY;

	//checking borders
	if (abs(theBall.x - 0) < theBall.radius) {
		//calculate angle
		cout << "left border" << endl;
		theBall.direction = (theBall.direction + 90) % 360;
	}
	else if (abs(theBall.x - 1000) < theBall.radius) {
		cout << "right border" << endl;
		theBall.direction = (theBall.direction + 90) % 360;
	}
	if (abs(theBall.y - 900) < theBall.radius) {
		theBall.direction = (theBall.direction + 90) % 360;
		cout << "top border" << endl;
	}
	else if (abs(theBall.y - 0) < theBall.radius) {
		cout << "bottom border GAME OVER" << endl;
		exit(1);
	}

	//if hits the bar
	if (theBall.y <= 36 && theBall.x >= theBar.x1 && theBall.x <= theBar.x2) {
		cout << "Ball near bar" << endl;
		theBall.direction = (theBall.direction + 90) % 360;
	}

	bool isGameWon = true;
	for (int i = 0; i < 100; i++) {
		if (!bricks[i].isBroken) {
			isGameWon = false;
			if (CheckCollision(bricks[i])) {
				//cout << distance(bricks[i]) << endl;
				bricks[i].isBroken = true;
			}			
		}
	}

	if (isGameWon) {
		cout << "Congratulations you won the game!!" << endl;
		exit(1);
	}
}

void DrawBrick(int x1, int y1) {
	glColor3f(0.5f, 0.5f, 0.5f);
	glRectf(x1, y1, x1 + BRICKWIDTH, y1 + BRICKHEIGHT);
}

void DrawBall(int cx, int cy, float radius, bool outline = false) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(outline ? GL_LINE_LOOP : GL_TRIANGLE_FAN);
	for (int i = 0; i <= 32; i++) {
		float angle = 2 * 3.14159 * i / 32.0f;
		float x = radius * cosf(angle);
		float y = radius * sinf(angle);
		glVertex2f(x + cx, y + cy);
	}
	glEnd();
	
}

void DrawInit() {
	glClearColor(0.0, 0.0, 0.0 , 0 ); 
	glClear(GL_COLOR_BUFFER_BIT);

	int i = 150;
	int u = 839;
	int j = 0;

	while (u > 540)
	{		
		while (i < 800) //to draw the bricks
		{
			DrawBrick(i, u);
			bricks[j] = Brick(i, u, false);
			j++;
			i = i + 71;
		}
		i = 150;
		u = u - 31;
	}

	//to draw the ball	
	DrawBall(110, 150, 10);
	theBall = Ball(110, 150, 10);
	//to draw the bar
	glRectf(30, 25, 120, 10);
	theBar = Bar(30, 25, 120, 10);
	
	glutSwapBuffers(); 
}

void Draw() {
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 100; i++) {
		if (bricks[i].isBroken == false) {
			DrawBrick(bricks[i].x, bricks[i].y);
		}
	}

	DrawBall(theBall.x, theBall.y, theBall.radius);
	//to draw the bar
	glRectf(theBar.x1, theBar.y1, theBar.x2, theBar.y2);

	glutSwapBuffers(); // do not modify this line..
}

void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
}


void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
	// Key a to go left
	if (key == 97) {
		// what to do when left key is pressed...
		if (theBar.x1 != 0) {
			theBar.x1 -= 10;;
			theBar.x2 -= 10;
		}
	}
	// Key a to go right
	else if (key == 100) {
		if (theBar.x2 != 1000) {
			theBar.x1 += 10;
			theBar.x2 += 10;
		}

	}
}

void Timer(int m) {
	
	SetNewBallPos();
	Draw();
	glutTimerFunc(1000.0 / FPS, Timer, 0);
}


int main(int argc, char* argv[]) {
	int width = 1000, height = 900; 
	srand((unsigned int)time(0)); 
	glutInit(&argc, argv); 

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(width, height); 
	glutCreateWindow("Breakout Remastered"); 
	SetCanvasSize(width, height); 

	glutDisplayFunc(DrawInit); 
	glutKeyboardFunc(PrintableKeys); 
	
	glutTimerFunc(1000.0 / FPS, Timer, 0);

	glutMainLoop();
	return 1;
}




























//#include <GL/glut.h>
//#include <stdlib.h>      // LIBRARIES
//#include <stdio.h>
//#include <math.h>
//
//#define BALL_SIDE 12        //SIDE PIXES PARA D MAG LAPAW
//#define BALL_SPEED 0.8      // BALL SPEED
//#define PLAYER_SIDE 45      //PLAYER SIDE
//#define PLAYER_SPEED 12     //PLAYER SPEED
//
//#define ANGLE 45;       //ANGLE
//int angles[2] = { 45,-45 };   // ANGLE
//
//#define RED 0
//#define GREEN 1
//#define BLUE 2
//#define YELLOW 3
//#define BLACK 4
//#define WHITE 5
//#define GRAY 6
//#define PINK 7
//#define PURPLE 8
//#define DARKGREEN 9
//GLfloat colors[10][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, // Red, Green
//                        {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, // Blue, Yellow
//                        {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, // Black, White 
//                        {0.5, 0.5, 0.5}, {1.0, 0.0, 1.0}, // Gray, Pink
//                        {0.29, 0.0, 0.5}, {0.0, 0.39, 0.0} };
//
//
//GLfloat pixel_color[3];     //for pixel comparison
//
//typedef struct {        // STRUCTURE FOR COORDINATES
//    double x;
//    double y;
//} coords;           // coords datatype
//
//int width, height;     // WIDTH & HEIGHT
//int cw, ch;         //current
//
//
//coords ball;            // we have 1 ball having x and y values
//coords ball_direction;      // ball x and y directions
//
//coords bar;          //bar
//coords bar_direction;    //bar only X direction will be used
//bool bar_moving;      //?
//
//coords brick;           //bricks
//
//int numlines = 4;
//int bricksperline = 10;
//int brickspace = 50;
//
//int showbrick[40];          // show brick 1 - true , 0 - false
//
///*---------------------------------------------------------------------------------*/
//
///*START OF BRICKS*/
//
//void DrawBricks() {
//
//    int i, j;
//    int x1, y1, x2, y2;
//    for (j = 0; j < numlines; j++)
//    {
//        for (i = 0; i < bricksperline; i++)
//        {
//
//            if (showbrick[j * bricksperline + i]) {
//                x1 = brick.x;
//                y1 = brick.y;
//                x2 = x1 + PLAYER_SIDE;
//                y2 = y1 + PLAYER_SIDE;
//                glColor3fv(colors[GRAY]);
//                //glRectf(x1 + brickspace*i,y1,x2 + brickspace*i ,y2);              
//                glRectf(x1 + brickspace * i, y1 - brickspace * j, x2 + brickspace * i, y2 - brickspace * j);
//            }
//
//        }
//    }
//
//}
//
//void InitBricks()
//{
//    int i;
//    for (i = 0; i < numlines * bricksperline; i++)
//        showbrick[i] = 1;   // 1 as true
//}
//
//
//void CheckBricks()
//{
//    int i, j, x, y;
//    for (j = 0; j < numlines; j++)
//    {
//        for (i = 0; i < bricksperline; i++)
//        {
//            if (showbrick[j * bricksperline + i])
//            {
//                y = 5 + j * (brickspace + PLAYER_SIDE);
//                x = 5 + i * (brickspace + PLAYER_SIDE);
//                if (ball.y >= (y - BALL_SIDE) && ball.y < (y + PLAYER_SIDE) &&
//                    ball.x >= (x - BALL_SIDE) && ball.x < (x + PLAYER_SIDE))
//                {
//                    showbrick[j * bricksperline + i] = 0;
//                }
//            }
//        }
//    }
//}
//
//
///*END OF BRICKS*/
//
//
///* START OF BALL*/
//int reverse(int i) {
//    return i * -1;
//}
//
//double radian(double deg) {
//    return deg * (3.14159265 / 180);
//}
//
//void bounce_x() {
//
//    int angle = angles[rand() % 2];
//
//    //  int angle = ANGLE;
//    float x = sqrt(2) * sin(radian(90 - angle));
//    float y = sqrt(2) * sin(radian(angle));
//    if (ball_direction.x > 0) {
//        x *= -1;
//    }
//    /*if(ball_direction.y > 0){
//        y *= -1;
//    }*/
//    ball_direction.x = x;
//    ball_direction.y = y;
//}
//void bounce_y() {
//
//
//    int angle = angles[rand() % 2];
//
//    //int angle = ANGLE;
//    float x = sqrt(2) * sin(radian(90 - angle));
//    float y = sqrt(2) * sin(radian(angle));
//    /*if(ball_direction.x > 0){
//        x *= -1;
//    }*/
//    if (ball_direction.y > 0) {
//        y *= -1;
//    }
//    ball_direction.x = x;
//    ball_direction.y = y;
//}
//
//void bounce() { //BOUNCES BALL BACK?
//
//    int angle = angles[rand() % 2];
//
//    // int angle = ANGLE;
//    float x = sqrt(2) * sin(radian(90 - angle));
//    float y = sqrt(2) * sin(radian(angle));
//    if (ball_direction.x > 0) {
//        x *= -1;
//    }
//    if (ball_direction.y > 0) {
//        y *= -1;
//    }
//    ball_direction.x = x;
//    ball_direction.y = y;
//}
//
//void updateBallPosition() {
//    coords bd = ball_direction;
//    ball.x += BALL_SPEED * bd.x;
//    ball.y += BALL_SPEED * bd.y;
//    if (ball.x < 0) {
//        ball.x = 0;
//        //ball_direction.x = reverse(bd.x);
//        bounce_x();
//    }
//    if (ball.x + BALL_SIDE > width) {
//        ball.x = width - BALL_SIDE;
//        //ball_direction.x = reverse(bd.x);
//        bounce_x();
//    }
//    if (ball.y < 0) {
//        ball.y = 0;
//        //ball_direction.y = reverse(bd.y);
//        bounce_y();
//    }
//    if (ball.y + BALL_SIDE > height) {
//        ball.y = height - BALL_SIDE;
//        //ball_direction.y = reverse(bd.y);
//        bounce_y();
//    }
//    glReadPixels(ball.x - 1, ball.y - 1, 1, 1, GL_RGB, GL_FLOAT, pixel_color);
//    //if(pixel_color[0] != colors[WHITE][0] && pixel_color[1] != colors[WHITE][1] && pixel_color[2] != colors[WHITE][2])        //{
//
//    if (pixel_color[0] != colors[WHITE][0]) {
//        if (pixel_color != colors[WHITE]) {
//            bounce();
//            //showbrick[j*bricksperline+i]= 0;
//            //int s = ball.x;       
//            //showbrick[s] = 0;
//            CheckBricks();
//            glutPostRedisplay();
//        }
//    }
//
//}
//
//void displayBall() {
//    int i;
//    int x1, y1, x2, y2;
//    x1 = ball.x;    //struct
//    y1 = ball.y;
//    x2 = x1 + BALL_SIDE;
//    y2 = y1 + BALL_SIDE;
//    glColor3fv(colors[RED]);
//    //glColor3f(0.0,0.0,0.0);
//    glRectf(x1, y1, x2, y2);
//}
///* END OF BALL*/
//
//
///*START OF PLAYER*/
//void movePlayer() {
//
//    coords pd = bar_direction;
//    bar.x += BALL_SPEED * pd.x; //used to be PLAYER_SPEED, but too fast
//
//    if (bar.x <= 0) {
//        bar.x = 0;
//        bar_moving = 0;
//    }
//    if (bar.x + PLAYER_SIDE >= width) {
//        bar.x = width - PLAYER_SIDE;
//        bar_moving = 0;
//    }
//
//    if (bar_moving == 0) {
//        bar_direction.x = 0;     //para kun ga stack up sa gilid pwede pa ka balik
//    }
//}
//
//void displayPlayer() {
//    int x1, y1, x2, y2;
//    x1 = bar.x;
//    y1 = bar.y;
//    x2 = x1 + PLAYER_SIDE;
//    y2 = y1 + BALL_SIDE;
//    glColor3fv(colors[BLACK]);
//    glRectf(x1, y1, x2, y2);
//}
///*END OF PLAYER*/
//
//
//
//void myDisplay() {   //DISPLAY FUNC
//    glClear(GL_COLOR_BUFFER_BIT);
//    glColor3f(1.0, 1.0, 1.0);
//
//    glRectf(0, 0, width, height);
//
//    displayBall();//display ball
//
//    displayPlayer(); //display Player
//
//    DrawBricks();
//
//    glFlush();
//}
//
//void myReshape(int w, int h) {    // RESHAPE FUNC
//    cw = w;
//    ch = h;
//    glViewport((cw - width) / 2, (ch - height) / 2, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(0, width, 0, height);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//void myIdle() {  //IDLE
//
//    updateBallPosition();
//
//    /*if(bar_moving){
//        movePlayer();
//    }*/
//
//    if (bar_moving) {
//        movePlayer();
//    }
//
//    glutPostRedisplay();
//}
//
//void myKeyboard(unsigned char key, int x, int y) {   //KEYBOARD
//    coords move_direction;
//    int change_direction = 1;
//
//    if (key == 'a' || key == 'A') {
//        //left
//        bar.x += PLAYER_SPEED * -1;
//        move_direction.x = -1;
//        if (bar_direction.x == 1) {
//            change_direction = 1;
//        }
//    }
//    else if (key == 'd' || key == 'D') {
//        //right
//        bar.x += PLAYER_SPEED * 1;
//        move_direction.x = 1;
//        if (bar_direction.x == -1) {
//            change_direction = 1;
//        }
//    }
//
//    if ((bar.x != 0)) {
//        bar_moving = 1;
//
//        if ((bar.x != width - PLAYER_SIDE)) {
//            bar_moving = 1;
//        }
//
//        if (change_direction == 1) {
//            bar_direction = move_direction;
//        }
//    }
//    glutPostRedisplay();
//}
//
//void init() {    // INITIALIZE
//
//    glClearColor(0.0, 0.0, 0.0, 1.0);
//
//
//    //bar.x = (width/2)-(BALL_SIDE/2);
//    bar.x = 250;
//    bar.y = 0;
//
//    bar_direction.x = 0;
//    bar_direction.y = 0;
//    bar_moving = 0;
//
//
//    ball.x = 250;   //set ball to center    // not yet so much centered
//    ball.y = 30;
//
//    ball_direction.x = 1;
//    ball_direction.y = -1;
//
//    InitBricks();       //INITIALIZE BRICKS SHOW
//    brick.x = 5;
//    brick.y = 450;
//
//}
//
//int main(int argc, char** argv) {    //MAIN
//    //srand(time(0));
//    glutInit(&argc, argv);
//    width = 500;
//    height = 500;
//    glutInitWindowSize(width, height);
//    glutInitWindowPosition(100, 100);
//    glutCreateWindow("BRicK-OUT");
//    glutDisplayFunc(myDisplay);
//    glutReshapeFunc(myReshape);
//    glutKeyboardFunc(myKeyboard);
//    glutIdleFunc(myIdle);
//    init();//initBricks();  
//
//    glutMainLoop();
//}