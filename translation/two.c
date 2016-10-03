#include "../headers/world.h"

// zoom stuff
#define INTERVAL 3
int LVL_LOW = 0;
int LVL_HIGH = 3;
float zoomCycle = 1.0;
float linearCycle;
int zoomLevel;

float offsets[20];

// world stuff
float transX;
float SPEED = 0.2;
GLuint texture;

char zoomReports[9][50];
char shiftString[50];

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	glColor3f(1.0, 1.0, 1.0);
	// text
	char zoomReport[50], zoomReport2[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomReport, "LINEAR (%d): %.2f (%.2f)", zoomLevel, linearCycle, originY*SPEED);
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
	glColor3f(1.0, 1.0, 1.0);
}

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

void drawGround(){
	glColor3f(1.0, 0.2, 0.2);
	drawLine(0.0, 0.0, 0.0, 0.0, 0.5, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	drawRect(-0.5, 0, 0, 1.0, 0.5);
	glBindTexture (GL_TEXTURE_2D, 0);
}

void setup(){ 
	texture = loadTexture("../resources/stripes512-256.raw", 512, 256);
}

static double unused;
void update(){ 
	float increasing = originY * SPEED;
	linearCycle = modf(increasing, &unused);
	zoomCycle = powf(INTERVAL, linearCycle);
	zoomLevel = increasing;

	// originX = sinf(frameNum * 0.015)*3.5;
	// originY = cosf(frameNum * 0.03)*2.4 + 2.4 + .05;

	transX = originX*.07;
}

void draw2D(){
	// glColor3f(1.0, 1.0, 1.0);
	// for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
	// 	text(zoomReports[i], 10, 68 + i*16, 0);
	// }

	drawHUD();

	for(int i = 0; i < 9; i++){
		text(zoomReports[i], 300, 100+18*i, 0);
	}

	glPushMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
		glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
		glScalef(WIDTH, WIDTH, WIDTH);
		// NOW: dimensions are 1.0 = width of screen

		drawGround();

		// sprintf(shiftString, "");

		glPushMatrix();  // SCALE: zoom cycle
			glScalef(zoomCycle, zoomCycle, zoomCycle);

			for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
				glPushMatrix();
				// with each zoom level we also calculate the fmodf of the translation

					float lvlWidth = 1.0/powf(INTERVAL, i);
					int lvlTransWhole = -transX / lvlWidth;
					float lvlTransPart = modf(-(transX)/lvlWidth, &unused);
					// 
					int lvlTransWhole_OFF = -transX / lvlWidth - 0.5;
					float lvlTransPart_OFF = modf(-(transX)/lvlWidth - 0.5, &unused);

					// float aTransX = transX;
					// float aThisW = 1.0/powf(INTERVAL, i + zoomLevel);
					// int aWholeTrans = -aTransX / aThisW - 0.5 ;

					glTranslatef(-transX - lvlTransWhole_OFF * lvlWidth, 0, 0);
					// glTranslatef(-aTransX - aWholeTrans * aThisW, 0, 0);
					float scale = powf(INTERVAL, i);
					float color = (i-linearCycle) / (LVL_HIGH-LVL_LOW);
					glColor3f(color*0.75 + 0.25, color*0.75 + 0.25, color*0.75 + 0.25);
					glScalef(1.0/scale, 1.0/scale, 1.0/scale);


					sprintf(zoomReports[i], "%f : (%d) %f : [(%d) %f]", lvlWidth, lvlTransWhole, lvlTransPart, lvlTransWhole_OFF, lvlTransPart_OFF);
					// sprintf(zoomReports[i], "%f : (%d) %f", lvlWidth, lvlTransWhole, lvlTransPart);


					unsigned char highlight = 0;
					// float secW = 0;
				// if(transWOff > -lvlWidth*0.5 && transWOff < lvlWidth*0.5){
				// 	// highlight = 2;
				// 	secW = lvlWidth / INTERVAL;
				// 	if(transWOff < secW*0.5 && transWOff > -secW*0.5){
				// 		offsets[i+1] = 0;
				// 		highlight = 2;
				// 	} else if(transWOff < -secW*0.5){
				// 		offsets[i+1] = -1.0;
				// 		highlight = 1;
				// 	} else if(transWOff > secW*0.5){
				// 		offsets[i+1] = 1.0;
				// 		highlight = 3;
				// 	}
				// }

					repeating2DScene(color*0.75 + 0.25, highlight);
				glPopMatrix();
			}

		glPopMatrix();  // SCALE: zoom cycle

	glPopMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0

	glColor3f(1.0, 1.0, 1.0);

	char transChar[50];
	sprintf(transChar, "%f", transX);
	text(transChar, 400, 60, 0);

	// text(shiftString, 550, 70, 0);
	// for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
	// 	char stream[10];
	// 	sprintf(stream, "%.4f", offsets[i]);
	// 	text(stream, 460, 68 + i*16, 0);
	// }
	// memset(offsets, 0, sizeof(float)*20);
}

void draw3D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }