#include "../headers/world.h"

#define ZOOM_RADIX 3
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

// void drawZoomboard(float zoom){
// 	glPushMatrix();
// 	glScalef(zoom, zoom, zoom);
// 	// for(int a = -5; a < 8; a++){
// 	int a = 1;
// 		glPushMatrix();
// 		// glScalef(1.0/powf(ZOOM_RADIX,a), 1.0/powf(ZOOM_RADIX,a), 1.0/powf(ZOOM_RADIX,a));
// 		glScalef(1.0f/ZOOM_RADIX, 1.0f/ZOOM_RADIX, 1.0f/ZOOM_RADIX);
// 		drawCheckerboard(0, 0, ZOOM_RADIX);
// 		glPopMatrix();
		// for(int i = -1; i <= 1; i++){
		// 	for(int j = -1; j <= 1; j++){
		// 		int b = abs(((i+j)%2));
		// 		if(b) glColor3f(1.0, 1.0, 1.0);
		// 		else glColor3f(0.0, 0.0, 0.0);
		// 		if(!(i == 0 && j == 0)){
		// 			glScalef(1.0/powf(3,a), 1.0/powf(3,a), 1.0/powf(3,a));
		// 			drawUnitSquare(i-.5, j-.5, 0.0);
		// 		}
		// 	}
		// }
// 	// }
// 	glPopMatrix();
// }

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

void setup(){ }
void update(){ }
void draw3D(){ 

	glTranslatef(0.0, 0.0, -0.3);
	glPushMatrix();
		glScalef(10, 10, 10);
		drawCelestialLines();
	glPopMatrix();

	int whole = originX/3.0;
	float zoom = (originX-whole);

	glPushMatrix();
		glScalef(zoom, zoom, zoom);
		drawZoomboard();
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	text("(10, 0, 0)", 10, 0, 0);
	text("(0, 10, 0)", 0, 10, 0);
	text("(0, 0, 10)", 0, 0, 10);
	text("(-10, 0, 0)", -10, 0, 0);
	text("(0, -10, 0)", 0, -10, 0);
	text("(0, 0, -10)", 0, 0, -10);

}
void draw2D(){
	char locationString[50];
	sprintf(locationString, "%d, %d, %d", (int)originX, (int)originY, (int)originZ );
	text(locationString, 4, 18, 0);

}
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }