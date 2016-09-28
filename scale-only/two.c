#include "../headers/world.h"

// zoom stuff
#define INTERVAL 3
unsigned int LVL_LOW = -1;
unsigned int LVL_HIGH = 7;
float zoomCycle = 1.0;
float linearCycle;

// world stuff
float SPEED = 0.2;
GLuint texture;

void repeating2DScene(float brightness){
	float barW = 1.0/INTERVAL;//(1.0-INTERVAL)*0.33;
	float barH = 0.25;

	glColor3f(brightness, brightness, brightness);
	drawRect(-barW*1.5, -barH, 0, barW, barH);  // left
	// drawRect(-barW*0.5, -barH, 0, barW, barH);     // middle
	drawRect(+barW*0.5, -barH, 0, barW, barH);       // right
}

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	glColor3f(1.0, 1.0, 1.0);
	// text
	char zoomReport[50], zoomReport2[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomReport, "LINEAR (X): %.2f (%.2f)", linearCycle, originY*SPEED);
	text(zoomReport, 4, 18, 0);
	sprintf(zoomReport2, "%d ^ X: %.2f", INTERVAL, zoomCycle);
	text(zoomReport2, 4, 37, 0);
	text("1.0", thirdW*1.5 - 5, 18, 0);
	sprintf(oneMinusInterval, "%.1f", (float)INTERVAL-1.0);
	text(oneMinusInterval, thirdW*1.5 - 5, 37, 0);
	text("0.0", thirdW - 30, 18, 0);
	text("1.0", thirdW - 30, 37, 0);
	text("1.0", thirdW*2 + 5, 18, 0);
	sprintf(intervalAsFloat, "%d.0", INTERVAL);
	text(intervalAsFloat, thirdW*2 + 5, 37, 0);
	// bars
	drawRect(thirdW, 6, 0, thirdW*linearCycle, 15);
	drawRect(thirdW, 25, 0, thirdW*(zoomCycle-1) / (INTERVAL-1), 15);
	glColor3f(0.3, 0.3, 0.3);
	drawRect(thirdW, 6, 0, thirdW, 15);
	drawRect(thirdW, 25, 0, thirdW, 15);
}

void drawGround(){
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	drawRect(-0.5, 0, 0, 1.0, 0.5);
	glBindTexture (GL_TEXTURE_2D, 0);
}

void setup(){ 
	texture = loadTexture("../resources/stripes512-256.raw", 512, 256);
}

void update(){ 
	static double unused;
	float increasing = originY * SPEED;
	linearCycle = modf(increasing, &unused);
	zoomCycle = powf(INTERVAL, linearCycle);
}

void draw2D(){

	drawHUD();

	glPushMatrix(); // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
		glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
		glScalef(WIDTH, WIDTH, WIDTH);
		// NOW: dimensions are 1.0 = width of screen

		drawGround();

		glPushMatrix();  // SCALE: zoom cycle
			glScalef(zoomCycle, zoomCycle, zoomCycle);

///////////////////////////////////
			// Method A
			// for(int i = LVL_LOW; i < LVL_HIGH; i++){
			// 	glPushMatrix();
			// 		float scale = powf(INTERVAL, i);
			// 		float color = (i-linearCycle) / (LVL_HIGH-LVL_LOW);
			// 		glScalef(1.0/scale, 1.0/scale, 1.0/scale);
			// 		repeating2DScene(color*0.75 + 0.25);
			// 	glPopMatrix();
			// }
///////////////////////////////////
			// Method B
			glPushMatrix();
			for(int i = LVL_LOW; i < LVL_HIGH; i++){
				float scale = INTERVAL;
				float color = (i-linearCycle) / (LVL_HIGH-LVL_LOW);
				glScalef(1.0/scale, 1.0/scale, 1.0/scale);
				repeating2DScene(color*0.75 + 0.25);
			}
			glPopMatrix();
///////////////////////////////////
		glPopMatrix();  // SCALE: zoom cycle
	glPopMatrix(); // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
}

void draw3D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }