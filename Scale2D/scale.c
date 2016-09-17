#include "../headers/world.h"

#define ZOOM_RADIX 3

void repeating2DScene(){
	drawRect(-WIDTH/3*1.5, -HEIGHT*.25, 0, WIDTH/3.0, HEIGHT*.25);
	drawRect(WIDTH/3*0.5, -HEIGHT*.25, 0, WIDTH/3.0, HEIGHT*.25);
}

void setup(){ }
void update(){ }
void draw3D(){ 

}
void draw2D(){
	glTranslatef(WIDTH*0.5, HEIGHT*0.5, 0.0f);
	glPushMatrix();
		char locationString[50];
		sprintf(locationString, "%d, %d, %d", (int)originX, (int)originY, (int)originZ );
		text(locationString, 4, 18, 0);
		glColor3f(1.0, 1.0, 1.0);
		drawRect(-WIDTH*0.5, 0, 0, WIDTH, HEIGHT*0.5);

		glPushMatrix();
		for(int i = 0; i <= 4; i++){
			float scale = 3;
			glScalef(1.0/scale, 1.0/scale, 1.0);
			repeating2DScene();
		}
		glPopMatrix();

		for(int i = 0; i <= 4; i++){
			glPushMatrix();
				float scale = powf(3, i);
				glScalef(1.0/scale, 1.0/scale, 1.0);
				repeating2DScene();
			glPopMatrix();
		}

	glPopMatrix();
}
void keyDown(unsigned int key){

}
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }