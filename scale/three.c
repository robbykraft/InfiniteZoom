#include "../headers/world.h"

#define INTERVAL 3

GLuint texture;

float zoomCycle = 1.0;
float linearCycle;

float SPEED = 0.2;

static double unused;


void initLighting(){
	// glEnable (GL_BLEND);
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat black[] = {0.0, 0.0, 0.0, 1.0};

	GLfloat light_position0[] = { 3.0, 4.0, 0.0, 1.0 };
	GLfloat light_position1[] = { -7.0, -3.0, 0.0, 1.0 };
	GLfloat light_position2[] = { -3.0, -7.0, 16.0, 1.0 };

	GLfloat spot_direction0[] = { -3.0, -4.0, -10.0 };
	GLfloat spot_direction1[] = { 7.0, 3.0, 0.0 };
	GLfloat spot_direction2[] = { 6.0, 3.0, 0.1 };


	GLfloat white[] = {0.1f, 0.1f, 0.1f, 0.2f};
	GLfloat red[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat blue[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, black);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	// glLightfv(GL_LIGHT2, GL_AMBIENT, black);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blue);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

	// glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);

	// glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.4);

	// glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 200.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);	


	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}

void drawCelestialLines(){
	float a1 = 0.33;
	float a2 = 0.166;
	glPushMatrix();
		// equator
		glColor4f(1.0, 1.0, 1.0, a1);
			drawUnitCircle(0, 0, 0);
		// latitude
		glColor4f(1.0, 1.0, 1.0, a2);
		for(float pos = 1.0/3; pos < 1.0; pos += 1.0/3){
			glPushMatrix();
				float r = cosf(pos*M_PI*0.5);
				r = sqrt(1 - powf(pos,2));
				glScalef(r, r, 1.0);
					drawUnitCircle(0, 0, -pos);
					drawUnitCircle(0, 0, pos);
			glPopMatrix();
		}
		// longitude
		glColor4f(1.0, 1.0, 1.0, a1);
			glRotatef(90, 0, 1, 0);
			drawUnitCircle(0, 0, 0);
		glColor4f(1.0, 1.0, 1.0, a2);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
		glColor4f(1.0, 1.0, 1.0, a1);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
		glColor4f(1.0, 1.0, 1.0, a2);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
	glPopMatrix();	
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

void drawZoomboard(){
	glPushMatrix();
	for(int a = -5; a < 8; a++){
		glPushMatrix();
		// glScalef(1.0/powf(ZOOM_RADIX,a), 1.0/powf(ZOOM_RADIX,a), 1.0/powf(ZOOM_RADIX,a));
		glScalef(1.0f/ZOOM_RADIX, 1.0f/ZOOM_RADIX, 1.0f/ZOOM_RADIX);
		// drawCheckerboard(0, 0, ZOOM_RADIX);
		for(int i = -1; i <= 1; i++){
			for(int j = -1; j <= 1; j++){
				int b = abs(((i+j)%2));
				if(b) glColor3f(1.0, 1.0, 1.0);
				else glColor3f(0.0, 0.0, 0.0);
				glPushMatrix();
				if(!(i == 0 && j == 0)){
					glScalef(1.0/powf(3,a), 1.0/powf(3,a), 1.0/powf(3,a));
					drawUnitSquare(i-.5, j-.5, 0.0);
				}
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void repeating3DScene(float brightness){
	GLfloat materialColor[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat mostlyBlack[] = {0.1, 0.1, 0.1, 1.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mostlyBlack);

	glColor3f(brightness, brightness, brightness);

	float W = 9.0;
	float TALL = 1.0;
	float spaceW = (float)W/INTERVAL;
	float barW = (W-INTERVAL)*0.5;
	// long sides
	drawRect(-W*0.5, -barW-spaceW*0.5, TALL, W, barW);
	drawRect(-W*0.5, spaceW*0.5, TALL, W, barW);
	// short sides
	drawRect(-barW-spaceW*0.5, -W*0.5, TALL, barW, W);
	drawRect(spaceW*0.5, -W*0.5, TALL, barW, W);
	// inner walls
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glColor3f(brightness*0.5, brightness*0.5, brightness*0.5);
		glTranslatef(-TALL, -barW*0.5, barW*0.5);
		drawRect(0, 0, 0, TALL, barW);
		glRotatef(-90, 1, 0, 0);
		glColor3f(brightness*0.3, brightness*0.3, brightness*0.3);
		drawRect(0, 0, 0, TALL, barW);
		glPushMatrix();
			glTranslatef(0, spaceW, spaceW);
			glRotatef(-90, 1, 0, 0);
			glColor3f(brightness*0.2, brightness*0.2, brightness*0.2);
			drawRect(0, 0, 0, TALL, barW);
			glRotatef(-90, 1, 0, 0);
			glColor3f(brightness*0.4, brightness*0.4, brightness*0.4);
			drawRect(0, 0, 0, TALL, barW);
		glPopMatrix();
	glPopMatrix();
}

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	// HUD
	glColor3f(1.0, 1.0, 1.0);
	char zoomReport[50], zoomReport2[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomReport, "LINEAR (X): %.2f (%.2f)", linearCycle, originY*SPEED);
	text(zoomReport, 4, 18, 0);
	sprintf(zoomReport2, "%d ^ X: %.2f", INTERVAL, zoomCycle);
	text(zoomReport2, 4, 37, 0);

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
	initLighting();
}

void update(){ 
	float increasing = originY * SPEED;
	linearCycle = modf(increasing, &unused);
	zoomCycle = powf(INTERVAL, linearCycle);
}

void draw3D(){ 

	glDisable(GL_LIGHTING);
	glTranslatef(0.0, 0.0, -0.3);
	glPushMatrix();
		glScalef(100, 100, 100);
		drawCelestialLines();
	glPopMatrix();

	// glEnable(GL_LIGHTING);

	// int whole = originX/3.0;
	// float zoom = (originX-whole);

	// glPushMatrix();
	// 	glScalef(zoom, zoom, zoom);
	// 	drawZoomboard();
	// glPopMatrix();

	// glColor3f(1.0, 1.0, 1.0);
	// text("(10, 0, 0)", 10, 0, 0);
	// text("(0, 10, 0)", 0, 10, 0);
	// text("(0, 0, 10)", 0, 0, 10);
	// text("(-10, 0, 0)", -10, 0, 0);
	// text("(0, -10, 0)", 0, -10, 0);
	// text("(0, 0, -10)", 0, 0, -10);


	glPushMatrix();
		glScalef(30, 30, 30);
		glScalef(zoomCycle, zoomCycle, zoomCycle);
		int iterations = 15;
		// subdivisions of space, zooming will repeat on the center one
		//    eg. ternary cantor set would be 3
		for(int i = 0; i < iterations; i++){
			glPushMatrix();
			float scale = powf(INTERVAL, i);
			float color = (i-linearCycle)/iterations;
			glScalef(1.0/scale, 1.0/scale, 1.0/scale);
			repeating3DScene(color*0.75+0.25);
			glPopMatrix();
		}
	glPopMatrix();
}

void draw2D(){
	drawHUD();
}
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }