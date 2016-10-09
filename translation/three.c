#include "../headers/world.h"

// space
#define INTERVAL 3
long LVL_LOW = 0;
long LVL_HIGH = 9;
int HALF_INTERVAL; // lower bounds of INTERVAL / 2
unsigned char INTERVAL_IS_ODD;
// zoom
long zoomWhole;
float zoom = 0;
float zoomCycleScale;
// translation
float transXLinear, transYLinear;
float transXExp, transYExp;
// keyboard input
float ZOOM_SPEED = 0.02;
float TRANSLATE_SPEED = 0.001;
float TRANSLATE_INTERVAL;
// graphics
char zoomReports[9][70];
GLuint texture;
unsigned char showHUD = 1;
static double unused;

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	// bars
	glColor3f(0.33, 0.33, 0.33);
	drawRect(thirdW, 6, 0, thirdW, 15);
	drawRect(thirdW, 25, 0, thirdW, 15);
	glColor3f(0.66, 0.66, 0.66);
	drawRect(thirdW, 6, 0, thirdW*zoom, 15);
	drawRect(thirdW, 25, 0, thirdW*(zoomCycleScale-1) / (INTERVAL-1), 15);
	// text
	glColor3f(1.0, 1.0, 1.0);
	char zoomString[50], zoomReport[50], zoomReport2[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomReport, "ZOOM (%ld): %.2f", zoomWhole, zoom);
	text(zoomReport, 4, 18, 0);
	sprintf(zoomReport2, "%d ^ X: %.2f", INTERVAL, zoomCycleScale);
	text(zoomReport2, 4, 37, 0);
	text("1.0", thirdW*1.5 - 5, 18, 0);
	sprintf(oneMinusInterval, "%.1f", (float)INTERVAL-1.0);
	text(oneMinusInterval, thirdW*1.5 - 5, 37, 0);
	text("0.0", thirdW - 30, 18, 0);
	text("1.0", thirdW - 30, 37, 0);
	text("1.0", thirdW*2 + 5, 18, 0);
	sprintf(intervalAsFloat, "%d.0", INTERVAL);
	text(intervalAsFloat, thirdW*2 + 5, 37, 0);
	// translation data
	glColor3f(1.0, 1.0, 1.0);
	char transString[50], transReport1[50], transReportX[50], transReportY[50];
	sprintf(transString, "TRANS: (%.3f, %.3f", transXLinear, transYLinear);
	text(transString, thirdW*2.5-50, 18, 0);
	sprintf(transReport1, "+/-dT: %.9f", TRANSLATE_INTERVAL);
	text(transReport1, thirdW*2.5-50, 37, 0);
	sprintf(transReportX, "%.2f*%d^%ld: %.4f", transXLinear, INTERVAL, zoomWhole, transXExp);
	text(transReportX, thirdW*2.5-50, 56, 0);
	sprintf(transReportY, "%.2f*%d^%ld: %.4f", transYLinear, INTERVAL, zoomWhole, transYExp);
	text(transReportY, thirdW*2.5-50, 75, 0);
	for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
		text(zoomReports[i], WIDTH*0.5 - 180, 80+18*i, 0);
	}
}

void repeating3DScene(float brightness, unsigned char highlight){ //0 for none, 1/2/3.. to color segments
	glColor3f(brightness, brightness, brightness);

	float W = 2.0;
	float TALL = 0.1;
	float spaceW = (float)W/INTERVAL;
	float barW = (W*2/3.0)*0.5;

	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-TALL, -W*0.5, barW*0.5);
		glColor3f(brightness*0.5, brightness*0.5, brightness*0.5);
		drawRect(0, 0, 0, TALL, W);
		glTranslatef(0, 0, -barW);
		glColor3f(brightness*0.3, brightness*0.3, brightness*0.3);
		drawRect(0, 0, 0, TALL, W);
	glPopMatrix();
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		glTranslatef(-TALL, -W*0.5, barW*0.5);
		glColor3f(brightness*0.2, brightness*0.2, brightness*0.2);
		drawRect(0, 0, 0, TALL, W);
		glTranslatef(0, 0, -barW);
		glColor3f(brightness*0.4, brightness*0.4, brightness*0.4);
		drawRect(0, 0, 0, TALL, W);
	glPopMatrix();
}

void drawGround(){
	glColor3f(0.25, 0.25, 0.25);
	drawRect(-10.0, -10.0, 0, 20.0, 20.0);

	// glColor3f(1.0, 1.0, 1.0);
	// glBindTexture(GL_TEXTURE_2D, texture);
	// drawRect(-1.0, -0.5, 0, 2.0, 1.0);
	// glBindTexture (GL_TEXTURE_2D, 0);
}

void setup(){ 
	texture = loadTexture("../resources/stripes512-256.raw", 512, 256);
	HALF_INTERVAL = (float)INTERVAL*0.5;
	INTERVAL_IS_ODD = INTERVAL - HALF_INTERVAL*2;
}

void update(){ 
// process keyboard
	if(keyboard[UP_KEY]){
		zoom -= ZOOM_SPEED;
	}
	if(keyboard[DOWN_KEY]){
		zoom += ZOOM_SPEED;
	}
	if(keyboard[LEFT_KEY]){ }
	if(keyboard[RIGHT_KEY]){ }
	// zoomWhole = zoom;
	// zoom = modf(zoom, &unused);
	while(zoom >= 1.0){
		zoomWhole += 1;
		zoom -= 1.0;
	}
	while(zoom < 0.0){
		zoomWhole -= 1;
		zoom += 1.0;
	}
	zoomCycleScale = powf(INTERVAL, zoom);

	TRANSLATE_INTERVAL = 1.0/powf(INTERVAL, zoom+zoomWhole) * TRANSLATE_SPEED;
	if(keyboard['W'] || keyboard['w']){
		transYLinear += TRANSLATE_INTERVAL;
	}
	if(keyboard['S'] || keyboard['s']){
		transYLinear -= TRANSLATE_INTERVAL;
	}
	if(keyboard['A'] || keyboard['a']){
		transXLinear -= TRANSLATE_INTERVAL;
	}
	if(keyboard['D'] || keyboard['d']){
		transXLinear += TRANSLATE_INTERVAL;
	}
	transXExp = transXLinear * powf(INTERVAL, zoomWhole);
	transYExp = transYLinear * powf(INTERVAL, zoomWhole);
}

void draw2D(){
	if(showHUD)
		drawHUD();
}

void draw3D(){

	glTranslatef(0.0, 0.0, -0.3);  // move eyes above zoom plane

	glPushMatrix();
		glScalef(100, 100, 100);
		drawUVSphereLines();
	glPopMatrix();

	glPushMatrix();
		drawGround();
	glPopMatrix();

	glPushMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
		// glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
		// glScalef(WIDTH, WIDTH, WIDTH);
		// NOW: dimensions are 1.0 = width of screen

		glScalef(30, 30, 30);

		glPushMatrix();  // SCALE: zoom cycle
			glScalef(zoomCycleScale, zoomCycleScale, zoomCycleScale);

			for(int i = LVL_LOW; i < LVL_HIGH; i++){
				glPushMatrix();
				// with each zoom level we also calculate the fmodf of the translation

					float lvlWidth = 1.0/powf(INTERVAL, i);
					int lvlTransXWhole = -transXExp / lvlWidth;
					int lvlTransYWhole = -transYExp / lvlWidth;
					float lvlTransXPart = modf(-(transXExp)/lvlWidth, &unused);
					float lvlTransYPart = modf(-(transYExp)/lvlWidth, &unused);

					// int lvlTransWhole_OFF = -transXExp / lvlWidth - 0.5;
					// float lvlTransPart_OFF = modf(-(transXExp)/lvlWidth - 0.5, &unused);
					// glTranslatef(-transXExp - lvlTransWhole_OFF * lvlWidth, 0, 0);

					int lvlTransXWhole_OFF = -transXExp / lvlWidth - 0.5*(INTERVAL_IS_ODD);
					int lvlTransYWhole_OFF = -transYExp / lvlWidth - 0.5*(INTERVAL_IS_ODD);
					float lvlTransXPart_OFF = modf(-(transXExp)/lvlWidth - 0.5*(INTERVAL_IS_ODD), &unused);
					float lvlTransYPart_OFF = modf(-(transYExp)/lvlWidth - 0.5*(INTERVAL_IS_ODD), &unused);
					glTranslatef(-transXExp - (lvlTransXWhole_OFF - 0.5*(!INTERVAL_IS_ODD)) * lvlWidth, 0, 0);
					glTranslatef(-transYExp - (lvlTransYWhole_OFF - 0.5*(!INTERVAL_IS_ODD)) * lvlWidth, 0, 0);

					float scale = powf(INTERVAL, i);
					float color = (i-zoom) / (LVL_HIGH-LVL_LOW);
					glScalef(1.0/scale, 1.0/scale, 1.0/scale);

					sprintf(zoomReports[i], "%f : (%d) %f : [(%d) %f]", lvlWidth, lvlTransXWhole, lvlTransXPart, lvlTransXWhole_OFF, lvlTransXPart_OFF);

					unsigned char highlight = 0;
					// for(int i = 1; i <= INTERVAL; i++){
					// 	float seg = (float)i / INTERVAL;
					// 	if(fabs(lvlTransXPart_OFF) < seg){
					// 		highlight = i;
					// 		break;
					// 	}
					// }

					repeating3DScene(color, highlight);
				glPopMatrix();
			}
		glPopMatrix();  // SCALE: zoom cycle
	glPopMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0

}

void keyDown(unsigned int key){ 
	if(key == ' ')
		showHUD = !showHUD;
}

void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }