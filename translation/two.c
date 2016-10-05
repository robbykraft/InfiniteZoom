#include "../headers/world.h"

// space
#define INTERVAL 2
long LVL_LOW = 0;
long LVL_HIGH = 9;
int HALF_INTERVAL; // lower bounds of INTERVAL / 2
unsigned char INTERVAL_IS_ODD;
// zoom
long zoomWhole;
float zoom = 0;
float zoomCycleScale;
// translation
float transX;
// keyboard input
float ZOOM_SPEED = 0.02;
float TRANSLATE_SPEED = 0.01;
// graphics
char zoomReports[9][70];
GLuint texture;
unsigned char showHUD = 1;
static double unused;

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	glColor3f(1.0, 1.0, 1.0);
	// text
	char zoomString[50], transString[50];
	char zoomReport[50], zoomReport2[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomString, "ZOOM: %.2f", zoom);
	text(zoomString, thirdW*2.5-15, 18, 0);
	sprintf(transString, "TRANS: %.2f", transX);
	text(transString, thirdW*2.5-15, 37, 0);
	sprintf(zoomReport, "LINEAR (%ld): %.2f", zoomWhole, zoom);
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
	for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
		text(zoomReports[i], WIDTH*0.5 - 180, 80+18*i, 0);
	}
	// bars
	glColor3f(0.66, 0.66, 0.66);
	drawRect(thirdW, 6, 0, thirdW*zoom, 15);
	drawRect(thirdW, 25, 0, thirdW*(zoomCycleScale-1) / (INTERVAL-1), 15);
	glColor3f(0.33, 0.33, 0.33);
	drawRect(thirdW, 6, 0, thirdW, 15);
	drawRect(thirdW, 25, 0, thirdW, 15);
}

void repeating2DScene(float brightness, unsigned char highlight){ //0 for none, 1/2/3.. to color segments
	float barW = 1.0/INTERVAL;
	float barH = 0.25;
	for(int i = 1; i <= INTERVAL; i++){
		if(highlight == i)  glColor3f(brightness, 0, 0);
		else                glColor3f(brightness+(i%2)*.03, brightness+(i%2)*.03, brightness+(i%2)*.03);
		drawRect(-barW*(1.0 + HALF_INTERVAL + 0.5*INTERVAL_IS_ODD) + barW*i, -barH, 0, barW, barH);
	}
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
	HALF_INTERVAL = (float)INTERVAL*0.5;
	INTERVAL_IS_ODD = INTERVAL - HALF_INTERVAL*2;
}

void update(){ 
// process keyboard
	if(keyboard[UP_KEY] || keyboard['W'] || keyboard['w']){
		zoom -= ZOOM_SPEED;
	}
	if(keyboard[DOWN_KEY] || keyboard['S'] || keyboard['s']){
		zoom += ZOOM_SPEED;
	}

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

	float TRANSLATE_INTERVAL = 1.0/powf(INTERVAL, zoom+zoomWhole) * TRANSLATE_SPEED;
	if(keyboard[LEFT_KEY]){
		originX -= TRANSLATE_INTERVAL;
	}
	if(keyboard[RIGHT_KEY]){
		originX += TRANSLATE_INTERVAL;
	}

	transX = originX*.07 * powf(INTERVAL, zoomWhole);
}

void draw2D(){

	if(showHUD)
		drawHUD();

	glPushMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
		glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
		glScalef(WIDTH, WIDTH, WIDTH);
		// NOW: dimensions are 1.0 = width of screen

		drawGround();
		glScalef(INTERVAL*INTERVAL, INTERVAL*INTERVAL, INTERVAL*INTERVAL);

		glPushMatrix();  // SCALE: zoom cycle
			glScalef(zoomCycleScale, zoomCycleScale, zoomCycleScale);

			for(int i = LVL_HIGH-1; i >= LVL_LOW; i--){
				glPushMatrix();
				// with each zoom level we also calculate the fmodf of the translation

					float lvlWidth = 1.0/powf(INTERVAL, i);
					int lvlTransWhole = -transX / lvlWidth;
					float lvlTransPart = modf(-(transX)/lvlWidth, &unused);

					// int lvlTransWhole_OFF = -transX / lvlWidth - 0.5;
					// float lvlTransPart_OFF = modf(-(transX)/lvlWidth - 0.5, &unused);
					// glTranslatef(-transX - lvlTransWhole_OFF * lvlWidth, 0, 0);

					int lvlTransWhole_OFF = -transX / lvlWidth - 0.5*(INTERVAL_IS_ODD);
					float lvlTransPart_OFF = modf(-(transX)/lvlWidth - 0.5*(INTERVAL_IS_ODD), &unused);
					glTranslatef(-transX - (lvlTransWhole_OFF - 0.5*(!INTERVAL_IS_ODD)) * lvlWidth, 0, 0);

					float scale = powf(INTERVAL, i);
					float color = (i-zoom) / (LVL_HIGH-LVL_LOW);
					glScalef(1.0/scale, 1.0/scale, 1.0/scale);

					sprintf(zoomReports[i], "%f : (%d) %f : [(%d) %f]", lvlWidth, lvlTransWhole, lvlTransPart, lvlTransWhole_OFF, lvlTransPart_OFF);

					unsigned char highlight = 0;
					for(int i = 1; i <= INTERVAL; i++){
						float seg = (float)i / INTERVAL;
						if(fabs(lvlTransPart_OFF) < seg){
							highlight = i;
							break;
						}
					}

					repeating2DScene(color, highlight);
				glPopMatrix();
			}
		glPopMatrix();  // SCALE: zoom cycle
	glPopMatrix();  // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
}

void draw3D(){ }
void keyDown(unsigned int key){ 
	if(key == ' ')
		showHUD = !showHUD;
}
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }