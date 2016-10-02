#include "../headers/world.h"

// zoom stuff
#define INTERVAL 3
int LVL_LOW = 0;
int LVL_HIGH = 15;
float zoomCycle = 1.0;
float linearCycle;

// world stuff
float SPEED = 0.2;
GLuint texture;

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
	glColor3f(1.0, 1.0, 1.0);
	// text
	char zoomReport[50], zoomReport2[50], oneMinusInterval[50], intervalAsFloat[50];
	sprintf(zoomReport, "LINEAR (X): %.2f (%.2f)", linearCycle, originY*SPEED);
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
}

void setup(){ 
	texture = loadTexture("../resources/stripes512-256.raw", 512, 256);
}

void update(){ 
	static double unused;
	float increasing = originY * SPEED;
	linearCycle = modf(increasing, &unused);
	zoomCycle = powf(INTERVAL, linearCycle);
}

void draw3D(){ 

	glTranslatef(0.0, 0.0, -0.3);  // move eyes above zoom plane

	glPushMatrix();
		glScalef(100, 100, 100);
		drawUVSphereLines();
	glPopMatrix();

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

		for(int i = LVL_LOW; i < LVL_HIGH; i++){
			glPushMatrix();
				float scale = powf(INTERVAL, i);
				float color = (i-linearCycle) / (LVL_HIGH-LVL_LOW);
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