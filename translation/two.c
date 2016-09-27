#include "../headers/world.h"

#define INTERVAL 3

GLuint texture;

float zoomCycle = 1.0;
float linearCycle;

float SPEED = 0.2;

float transX;

static double unused;
char zoomReports[9][50];


void repeating2DScene(float brightness, unsigned char highlight){ //0 for none, 1/2/3 for left/mid/right
	// float spaceW = (float)1.0/INTERVAL;
	float barW = 1.0/INTERVAL;//(1.0-INTERVAL)*0.33;
	float barH = 0.25;
	if(highlight == 1)  glColor3f(brightness, 0, 0);
	else                glColor3f(brightness, brightness, brightness);

	drawRect(-barW*1.5, -barH, 0, barW, barH);  // left

	if(highlight == 2)  glColor3f(brightness, 0, 0);
	else                glColor3f(brightness, brightness, brightness);

	drawRect(-barW*0.5, -barH, 0, barW, barH);     // middle

	if(highlight == 3)  glColor3f(brightness, 0, 0);
	else                glColor3f(brightness, brightness, brightness);

	drawRect(+barW*0.5, -barH, 0, barW, barH);       // right
}

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	// HUD
	glColor3f(1.0, 1.0, 1.0);
	char zoomReport[50], zoomReport2[50], transReport[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomReport, "LINEAR: %.2f ", linearCycle);//, originY*SPEED);
	text(zoomReport, 4, 18, 0);
	sprintf(zoomReport2, "%d ^ X: %.2f", INTERVAL, zoomCycle);
	text(zoomReport2, 4, 37, 0);
	sprintf(transReport, "TRANSLATION: %.2f", transX);
	text(transReport, 4, 53, 0);

	glColor3f(1.0, 1.0, 1.0);
	text("1.0", thirdW*1.5 - 5, 18, 0);
	sprintf(oneMinusInterval, "%.1f", (float)INTERVAL-1.0);
	text(oneMinusInterval, thirdW*1.5 - 5, 37, 0);

	glColor3f(1.0, 1.0, 1.0);
	drawRect(thirdW, 6, 0, thirdW*linearCycle, 15);
	drawRect(thirdW, 25, 0, thirdW*(zoomCycle-1) / (INTERVAL-1), 15);

	glColor3f(0.3, 0.3, 0.3);
	drawRect(thirdW, 6, 0, thirdW, 15);
	drawRect(thirdW, 25, 0, thirdW, 15);

	glColor3f(1.0, 1.0, 1.0);
	text("0.0", thirdW - 30, 18, 0);
	text("1.0", thirdW - 30, 37, 0);
	text("1.0", thirdW*2 + 5, 18, 0);
	sprintf(intervalAsFloat, "%d.0", INTERVAL);
	text(intervalAsFloat, thirdW*2 + 5, 37, 0);
}

void setup(){ 
	// texture = loadTexture("../bin/texture.raw", 32, 32);
	texture = loadTexture("../bin/stripes512-256.raw", 512, 256);
}

void update(){ 
	float increasing = originY * SPEED;
	linearCycle = modf(increasing, &unused);
	zoomCycle = powf(INTERVAL, linearCycle);

	// originX = sinf(frameNum * 0.015)*3.5;
	// originY = cosf(frameNum * 0.03)*2.4 + 2.4 + .05;

	transX = originX*.07;
}


void draw3D(){ }

void draw2D(){
	glColor3f(1.0, 1.0, 1.0);
	for(int i = 2; i >= 0; i--){
		text(zoomReports[i], 4, 68 + i*16, 0);
	}
	drawHUD();

	// GROUND
	glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		drawLine(WIDTH*0.5, HEIGHT*0.75, 0.0, WIDTH*0.5, HEIGHT, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
		glBindTexture(GL_TEXTURE_2D, texture);
		drawRect(-WIDTH*0.5, 0, 0, WIDTH, WIDTH*0.5);
		glBindTexture (GL_TEXTURE_2D, 0);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);

	glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
	glScalef(WIDTH, WIDTH, WIDTH);

	glPushMatrix();
		glScalef(zoomCycle, zoomCycle, zoomCycle);
		glTranslatef(-transX, 0, 0);

		int iterations = 7;

		// for(int i = 6; i >= 3; i--){
		// 	glPushMatrix();
		// 	float scale = powf(INTERVAL, i);
		// 	float color = (i-linearCycle)/iterations;
		// 	glScalef(1.0/scale, 1.0/scale, 1.0/scale);
		// 	glTranslatef(WIDTH*0.33, 0, 0);
		// 	repeating2DScene(color*0.75 + 0.25, 0);
		// 	glPopMatrix();
		// }

		// subdivisions of space, zooming will repeat on the center one
		//    eg. ternary cantor set would be 3
		// for(int i = iterations-1; i >= 0; i--){
		for(int i = 2; i >= 0; i--){
			glPushMatrix();
			float scale = powf(INTERVAL, i);
			float color = (i-linearCycle)/iterations;
			glColor3f(color*0.75 + 0.25, color*0.75 + 0.25, color*0.75 + 0.25);
			glScalef(1.0/scale, 1.0/scale, 1.0/scale);
			// stuff here
			// float thisW = powf(INTERVAL, 3.5-i);
			float thisW = 1.0/powf(INTERVAL, i) * 0.5;

			unsigned char highlight = 0;
			float secW = 0;
			if(transX > -thisW && transX < thisW){
				highlight = 2;
				// secW = thisW / INTERVAL;
				// if(transX < secW && transX > -secW)
				// 	highlight = 2;
				// if(transX < -secW)// && transX > -secW*1.5)
				// 	highlight = 1;
				// if(transX > secW)// && transX > -secW*0.5)
				// 	highlight = 3;
			}

			sprintf(zoomReports[i], "(%d): SEC: %.2f  THISW: %.2f   SCALE:%.2f", i, secW, thisW, scale);

			
			repeating2DScene(color*0.75 + 0.25, highlight);
			glPopMatrix();
		}

	glPopMatrix();

}
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }