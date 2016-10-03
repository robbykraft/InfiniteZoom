#include "../headers/world.h"

// zoom stuff
#define INTERVAL 3
int LVL_LOW = 0;
int LVL_HIGH = 7;
float zoomCycle = 1.0;
float linearCycle;
int zoomLevel;

// world stuff
float transX;
float SPEED = 0.2;
GLuint texture;

char zoomReports[9][70];

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
	for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
		text(zoomReports[i], 200, 100+18*i, 0);
	}
	char transChar[50];
	sprintf(transChar, "%f", transX);
	text(transChar, 400, 60, 0);
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
// process keyboard
	float TRANSLATE_INTERVAL = 1.0/powf(INTERVAL, originY * SPEED) * 0.03;
	originDX = originDY = originDZ = 0;
	if(keyboard[UP_KEY]){
		originDY += WALK_INTERVAL;
	} if(keyboard[DOWN_KEY]){
		originDY -= WALK_INTERVAL;
	} if(keyboard[LEFT_KEY]){
		originDX += TRANSLATE_INTERVAL;
	} if(keyboard[RIGHT_KEY]){
		originDX -= TRANSLATE_INTERVAL;
	}
	originX -= originDX;
	originY -= originDY;
	originZ -= originDZ;

	float increasing = originY * SPEED;
	linearCycle = modf(increasing, &unused);
	zoomCycle = powf(INTERVAL, linearCycle);
	zoomLevel = increasing;

	transX = originX*.07 * powf(INTERVAL, zoomLevel);
}

void draw2D(){

	drawHUD();

	glPushMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
		glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
		glScalef(WIDTH, WIDTH, WIDTH);
		// NOW: dimensions are 1.0 = width of screen

		drawGround();
		// glScalef(INTERVAL*INTERVAL, INTERVAL*INTERVAL, INTERVAL*INTERVAL);

		glPushMatrix();  // SCALE: zoom cycle
			glScalef(zoomCycle, zoomCycle, zoomCycle);

			for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
				glPushMatrix();
				// with each zoom level we also calculate the fmodf of the translation

					float lvlWidth = 1.0/powf(INTERVAL, i);
					int lvlTransWhole = -transX / lvlWidth;
					float lvlTransPart = modf(-(transX)/lvlWidth, &unused);

					int lvlTransWhole_OFF = -transX / lvlWidth - 0.5;
					float lvlTransPart_OFF = modf(-(transX)/lvlWidth - 0.5, &unused);

					glTranslatef(-transX - lvlTransWhole_OFF * lvlWidth, 0, 0);
					// glTranslatef(-aTransX - aWholeTrans * aThisW, 0, 0);
					float scale = powf(INTERVAL, i);
					float color = (i-linearCycle) / (LVL_HIGH-LVL_LOW);
					glColor3f(color*0.75 + 0.25, color*0.75 + 0.25, color*0.75 + 0.25);
					glScalef(1.0/scale, 1.0/scale, 1.0/scale);

					sprintf(zoomReports[i], "%f : (%d) %f : [(%d) %f]", lvlWidth, lvlTransWhole, lvlTransPart, lvlTransWhole_OFF, lvlTransPart_OFF);

					unsigned char highlight = 0;
					if(fabs(lvlTransPart_OFF) < 1.0/INTERVAL){
						highlight = 1;
					} else if(fabs(lvlTransPart_OFF) < 2.0/INTERVAL){
						highlight = 2;
					} else if(fabs(lvlTransPart_OFF) < 1.0){
						highlight = 3;
					}

					repeating2DScene(color*0.75 + 0.25, highlight);
				glPopMatrix();
			}
		glPopMatrix();  // SCALE: zoom cycle
	glPopMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
}

void draw3D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }