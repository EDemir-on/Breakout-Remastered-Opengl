//#include "Main.h"
//#include <cmath>
//#include <iostream>
//#include <random>
//#include <chrono>
//#include <gl\glut.h>
//
//enum { MINE = 9 };
//enum { TILE_SIZE = 20 };
//enum { MARGIN = 40 };
//enum { PADDING = 10 };
//enum { BOARD_SIZE = 30 };
//enum { MINE_COUNT = 100 };
//
//// Create a list of colors
//enum Color {
//	RED,
//	DARKRED,
//	BLUE,
//	DARKBLUE,
//	DARKGREEN,
//	CYAN,
//	DARKCYAN,
//	YELLOW,
//	DARKYELLOW,
//	WHITE,
//	BLACK,
//	DARKGRAY,
//	LIGHTGRAY,
//	VERYLIGHTGRAY
//};
//
//// Fill list above with RGB values
//static const struct { float r, g, b; } colors[] = {
//	{1, 0, 0},		// red
//	{0.5f, 0, 0},		// dark red
//
//	{0, 0, 1},		// blue
//	{0, 0, 0.5f},		// dark blue
//
//	{0, 0.5f, 0},		// dark green
//
//	{0, 1, 1},		// cyan
//	{0, 0.5f, 0.5f},	// dark  cyan
//
//	{1, 1, 0},		// yellow
//	{0.5f, 0.5f, 0},	// dark yellow
//
//	{1, 1, 1},		// White
//	{0, 0, 0},          	// black
//
//	{0.25, 0.25, 0.25}, 	// dark gray
//	{0.5, 0.5, 0.5},    	// light gray
//	{0.75, 0.75, 0.75}  	// very-light gray
//};
//
//struct cell {
//	int type;
//	bool flag;
//	bool open;
//};
//
//cell board[BOARD_SIZE * BOARD_SIZE];
//int death;
//int width;
//int height;
//int num_opened;
//
//// Random int between min and max
//int rand_int(int min, int max) {
//	static std::default_random_engine re{ std::random_device{}() };
//	using Dist = std::uniform_int_distribution<int>;
//	static Dist uid{};
//	return uid(re, Dist::param_type{ min, max });
//}
//
//// https://en.wikibooks.org/wiki/OpenGL_Programming/GLStart/Tut3
//void drawRect(int x, int y, float width, float height, const Color& color = DARKGRAY, bool outline = true) {
//	glColor3f(colors[color].r, colors[color].g, colors[color].b);
//	glBegin(outline ? GL_LINE_STRIP : GL_TRIANGLE_FAN);
//	{
//		glVertex2i(x, y);
//		glVertex2i(x + width, y);
//		glVertex2i(x + width, y + height);
//		glVertex2i(x, y + height);
//	}
//	glEnd();
//}
//
//void drawCircle(int cx, int cy, float radius, const Color& color = LIGHTGRAY, bool outline = true) {
//	glColor3f(colors[color].r, colors[color].g, colors[color].b);
//	glBegin(outline ? GL_LINE_LOOP : GL_TRIANGLE_FAN);
//	for (int i = 0; i <= 32; i++) {
//		float angle = 2 * 3.14159 * i / 32.0f;
//		float x = radius * cosf(angle);
//		float y = radius * sinf(angle);
//		glVertex2f(x + cx, y + cy);
//	}
//	glEnd();
//}
//
//void drawFrame(float x, float y, float width, float height, bool border = true) {
//	glColor3f(colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
//
//	// Left and top side - white frame border
//	glBegin(GL_LINE_STRIP);
//	{
//		glVertex2i(x, y);
//		glVertex2i(x, y + height - 1);
//		glVertex2i(x + width - 1, y + height - 1);
//		glVertex2i(x + width - 2, y + height - 2);
//		glVertex2i(x + 1, y + height - 2);
//		glVertex2i(x + 1, y + 1);
//	}
//	glEnd();
//
//	glColor3f(colors[LIGHTGRAY].r, colors[LIGHTGRAY].g, colors[LIGHTGRAY].b);
//
//	// Right and bottom side - gray frame border
//	glBegin(GL_LINE_STRIP);
//	{
//		glVertex2f(x + width - 2, y + height - 2);
//		glVertex2f(x + width - 2, y + 1);
//		glVertex2f(x + 1, y + 1);
//		glVertex2f(x, y);
//		glVertex2f(x + width - 1, y);
//		glVertex2f(x + width - 1, y + height - 1);
//	}
//	glEnd();
//
//	// Creates the thick "border" look of the outside frame by drawing a new smaller frame
//	if (!border) return;
//	width = width - 2 * PADDING;
//	height = height - 2 * PADDING;
//
//	// Gray highlight on left and top side
//	glBegin(GL_LINE_STRIP);
//	{
//		glVertex2i(x + PADDING, y + PADDING);
//		glVertex2i(x + PADDING, y + PADDING + height);
//		glVertex2i(x + PADDING + width, y + PADDING + height);
//	}
//	glEnd();
//
//	// White highlight on bottom and right side
//	glColor3f(colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
//	glBegin(GL_LINE_STRIP);
//	{
//		glVertex2i(x + width + PADDING, y + height + PADDING);
//		glVertex2i(x + width + PADDING, y + PADDING);
//		glVertex2i(x + PADDING, y + PADDING);
//	}
//	glEnd();
//}
//
//
//// Main board frame
//void drawLowerFrame(int x = 0, int y = 0) {
//	static const float lower_frame_size = width;
//	drawFrame(0, 0, lower_frame_size, lower_frame_size);
//}
//
//
//
//int index(int x, int y) { return x + (y * BOARD_SIZE); }
//
//
//
//
//bool gameOver() { return death != -1; }
//
//bool isDead(int x, int y) { return death == index(x, y); }
//
//bool hasWon() { return num_opened == MINE_COUNT; }
//
//
//
//void init() {
//	// Initialize empty board
//	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
//		board[i].type = 0;
//		board[i].flag = false;
//		board[i].open = false;
//	}
//
//	
//	death = -1;
//	num_opened = BOARD_SIZE * BOARD_SIZE;
//
//	// GL Options found on StackOverflow to enable antialieasing etc.
//	glClearColor(0.8f, 0.8f, 0.8f, 1.f);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0, width, 0, height, -1.f, 1.f);
//	glPointSize(5.0);
//	glEnable(GL_LINE_SMOOTH);
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	glEnable(GL_POINT_SMOOTH);
//	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//// glut callbacks
//void display() {
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	drawRect(3, 3, 2, 2, DARKBLUE);
//
//	glutSwapBuffers();
//}
//
//// Handle ESC press - Found on StackOverflow
//void key(unsigned char key, int x, int y) {
//	switch (key) {
//	case 27:
//		exit(0);
//		break;
//	}
//}
//
//// Roughly the screen position of the emoji
//bool requestRestart(int x, int y) {
//	return (x >= BOARD_SIZE / 2 - 1 && x <= BOARD_SIZE / 2 + 1 &&
//		y >= BOARD_SIZE + 1 && y <= BOARD_SIZE + 3);
//}
//void mouse(int button, int state, int x, int y) {
//	x = (x + PADDING) / TILE_SIZE - 1;
//	y = (height - y + PADDING) / TILE_SIZE - 1;
//
//	switch (button) {
//	case GLUT_LEFT_BUTTON:
//		if (state == GLUT_DOWN) {
//			if (requestRestart(x, y)) {
//				std::cout << "Restarting..." << std::endl;
//				init();
//			}
//			else if (!gameOver() && !hasWon()) {
//				
//			}
//		}
//		break;
//	case GLUT_RIGHT_BUTTON:
//		if (state == GLUT_DOWN) {
//			if (gameOver() || hasWon()) break;
//			
//		}
//		break;
//	}
//}
///*
//int main(int argc, char** argv) {
//	width = BOARD_SIZE * TILE_SIZE + 2 * PADDING;
//	height = BOARD_SIZE * TILE_SIZE + 2 * PADDING + 2 * MARGIN;
//
//	glutInit(&argc, argv);
//	// Double buffered, RGB mode, multisampling/anti-aliasing enabled
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
//	glutInitWindowSize(width, height);
//	// Center Window to screen
//	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2,
//		(glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
//	glutCreateWindow("Breakout Remastered");
//	glutIdleFunc(display);
//	glutDisplayFunc(display);
//	glutKeyboardFunc(key);
//	glutMouseFunc(mouse);
//
//	init();
//
//	glutMainLoop();
//	return 0;
//}
//
//*/