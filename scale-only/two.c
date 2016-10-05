#include "../headers/world.h"

// space
#define INTERVAL 3
long LVL_LOW = 0;
long LVL_HIGH = 7;
// zoom
long zoomWhole;
float zoom = 0;
float zoomCycleScale;
// keyboard input
float ZOOM_SPEED = 0.01;
// graphics
GLuint texture;
unsigned char showHUD = 1;
static double unused;

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	glColor3f(1.0, 1.0, 1.0);
	// text
	char zoomString[50], zoomReport[50], zoomReport2[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomString, "ZOOM: %.2f", zoom);
	text(zoomString, thirdW*2.5-15, 27, 0);
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
	// bars
	glColor3f(0.66, 0.66, 0.66);
	drawRect(thirdW, 6, 0, thirdW*zoom, 15);
	drawRect(thirdW, 25, 0, thirdW*(zoomCycleScale-1) / (INTERVAL-1), 15);
	glColor3f(0.33, 0.33, 0.33);
	drawRect(thirdW, 6, 0, thirdW, 15);
	drawRect(thirdW, 25, 0, thirdW, 15);
}

void drawGround(){
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	drawRect(-0.5, 0, 0, 1.0, 0.5);
	glBindTexture (GL_TEXTURE_2D, 0);
}

void repeating2DScene(float brightness){
	int HALF_INTERVAL = (float)INTERVAL*0.5;
	float barH = 0.25;
	float barW = 1.0/INTERVAL * HALF_INTERVAL;

	glColor3f(brightness, brightness, brightness);
	drawRect(-0.5, -barH, 0, barW, barH);
	glColor3f(brightness + 0.02, brightness + 0.02, brightness + 0.02);
	drawRect(0.5-barW, -barH, 0, barW, barH);
}

void setup(){ 
	texture = loadTexture("../resources/stripes512-256.raw", 512, 256);
}

void update(){ 
	// keyboard input
	if(keyboard[UP_KEY] || keyboard['W'] || keyboard['w']){
		zoom -= ZOOM_SPEED;
	} if(keyboard[DOWN_KEY] || keyboard['S'] || keyboard['s']){
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
}

void draw2D(){

	if(showHUD)
		drawHUD();

	glPushMatrix(); // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
		glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
		glScalef(WIDTH, WIDTH, WIDTH);
		// NOW: dimensions are 1.0 = width of screen

		drawGround();

		glPushMatrix();  // SCALE: zoom cycle
			glScalef(zoomCycleScale, zoomCycleScale, zoomCycleScale);

///////////////////////////////////
			// Method A
			for(long i = LVL_HIGH-1; i >= LVL_LOW; i--){
				glPushMatrix();
					float scale = powf(INTERVAL, i);
					float color = (i-zoom) / (LVL_HIGH-LVL_LOW);
					glScalef(1.0/scale, 1.0/scale, 1.0/scale);
					repeating2DScene(color*0.75 + 0.25);
				glPopMatrix();
			}
///////////////////////////////////
			// Method B
			// glPushMatrix();
			// for(long i = LVL_HIGH-1; i >= LVL_LOW; i--){
			// 	float scale = INTERVAL;
			// 	float color = (i-zoom) / (LVL_HIGH-LVL_LOW);
			// 	glScalef(1.0/scale, 1.0/scale, 1.0/scale);
			// 	repeating2DScene(color*0.75 + 0.25);
			// }
			// glPopMatrix();
///////////////////////////////////
		glPopMatrix();  // SCALE: zoom cycle
	glPopMatrix(); // TRANSLATE & SCALE: (0,0) to center, screen width to 1.0
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