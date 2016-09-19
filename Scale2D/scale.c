#include "../headers/world.h"

#define ZOOM_RADIX 3

GLuint texture;

float zoom = 1.0;
float lvlPercent;

float SPEED = 0.2;

static double unused;

void repeating2DScene(){
	drawRect(-WIDTH/3*1.5, -HEIGHT*.25, 0, WIDTH/3.0, HEIGHT*.25);
	drawRect(WIDTH/3*0.5, -HEIGHT*.25, 0, WIDTH/3.0, HEIGHT*.25);
}

void drawHUD(){
	float thirdW = WIDTH * 0.33;
	// HUD
	glColor3f(1.0, 1.0, 1.0);
	char zoomReport2[50];
	sprintf(zoomReport2, "LINEAR (X): %.2f (%.2f)", lvlPercent, originY*SPEED);
	text(zoomReport2, 4, 18, 0);
	char zoomReport[50];
	sprintf(zoomReport, "3 ^ X: %.2f", zoom);
	text(zoomReport, 4, 37, 0);

	glColor3f(1.0, 1.0, 1.0);
	text("1.0", thirdW*1.5 - 5, 18, 0);
	text("2.0", thirdW*1.5 - 5, 37, 0);

	glColor3f(1.0, 1.0, 1.0);
	drawRect(thirdW, 6, 0, thirdW*lvlPercent, 15);
	drawRect(thirdW, 25, 0, thirdW*(zoom-1) / 2.0, 15);

	glColor3f(0.3, 0.3, 0.3);
	drawRect(thirdW, 6, 0, thirdW, 15);
	drawRect(thirdW, 25, 0, thirdW, 15);

	glColor3f(1.0, 1.0, 1.0);
	text("0.0", thirdW - 30, 18, 0);
	text("1.0", thirdW - 30, 37, 0);
	text("1.0", thirdW*2 + 5, 18, 0);
	text("3.0", thirdW*2 + 5, 37, 0);

}

void setup(){ 
	// texture = loadTexture("../bin/texture.raw", 32, 32);
	texture = loadTexture("../bin/stripes512-256.raw", 512, 256);
}
void update(){ 
	lvlPercent = modf(originY * SPEED, &unused);
	zoom = powf(3, lvlPercent);
}
void draw3D(){ 

}

void draw2D(){

	drawHUD();

	// GROUND
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(WIDTH*0.5, HEIGHT*0.75, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture);
	drawRect(-WIDTH*0.5, 0, 0, WIDTH, WIDTH*0.5);
	glBindTexture (GL_TEXTURE_2D, 0);

	glPushMatrix();

		glScalef(zoom, zoom, zoom);

		int iterations = 7;

		// subdivisions of space, zooming will repeat on the center one
		//    eg. ternary cantor set would be 3

		int INTERVAL = 3; 
		for(int i = 0; i < iterations; i++){
			glPushMatrix();
			float scale = powf(INTERVAL, i);
			float color = (i-lvlPercent)/iterations;
			glColor3f(color*0.75 + 0.25, color*0.75 + 0.25, color*0.75 + 0.25);
			glScalef(1.0/scale, 1.0/scale, 1.0/scale);
			repeating2DScene();
			glPopMatrix();
		}

	glPopMatrix();
}
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }