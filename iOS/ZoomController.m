//
//  GameViewController.m
//  InfiniteZoom
//
//  Created by Robby on 9/10/14.
//  Copyright (c) 2014 Robby. All rights reserved.
//

#import "ZoomController.h"
#import <OpenGLES/ES1/gl.h>

#define Z_NEAR .01f
#define Z_FAR 1000.0f

GLfloat quadVertexData[12] = {
    -0.1f, 0.1f, 0.0f,
    -0.1f, -0.1f, 0.0f,
    0.1f, 0.1f, 0.0f,
    0.1f, -0.1f, 0.0f
};


@interface ZoomController () {
    GLKMatrix4 _modelViewProjectionMatrix;
    GLKMatrix4 _projectionMatrix;
    
    NSDate *start;
    NSTimeInterval elapsedSeconds;
    
    float position;
    float velocity;
    BOOL leftButton;
    BOOL rightButton;
    
    int zoomLevel;
    float levelProgress;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ZoomController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    start = [NSDate date];
    
    [self setupGL];
    
    [self setupGame];
    
    [self.view setUserInteractionEnabled:YES];
}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    self.effect = [[GLKBaseEffect alloc] init];
    self.preferredFramesPerSecond = 60;
//    self.effect.light0.enabled = GL_TRUE;
//    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 0.4f, 0.4f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    [self rebuildProjectionMatrix];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    self.effect = nil;
}

-(void)rebuildProjectionMatrix{
    float _fieldOfView = 75;
    float _aspectRatio = fabsf(self.view.bounds.size.width / self.view.bounds.size.height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat frustum = Z_NEAR * tanf(_fieldOfView*0.00872664625997);  // pi/180/2
    _projectionMatrix = GLKMatrix4MakeFrustum(-frustum, frustum, -frustum/_aspectRatio, frustum/_aspectRatio, Z_NEAR, Z_FAR);
    glMultMatrixf(_projectionMatrix.m);
    glViewport(0, 0, self.view.frame.size.width, self.view.frame.size.height);
    glMatrixMode(GL_MODELVIEW);
}

-(void) setupGame{
    position = 0.0f;
    velocity = 0.0f;
}


-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    for(UITouch *touch in [event allTouches]){
        if([touch locationInView:self.view].y < self.view.bounds.size.height*.5){
            leftButton = true;
        }
        else if ([touch locationInView:self.view].y >= self.view.bounds.size.height*.5){
            rightButton = true;
        }
    }
//    NSLog(@"(%d) L:%d  R:%d", [[event allTouches] count], leftButton, rightButton);
}

-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
//    NSLog(@"(%d) L:%d  R:%d", [[event allTouches] count], leftButton, rightButton);
}

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{

    NSLog(@"%d, %d", [touches count], [[event allTouches] count]);
    
    BOOL checkLeft = false;
    BOOL checkRight = false;
    for(UITouch *touch in [event allTouches]){
        if(![[touches anyObject] isEqual:touch]){
            if([touch locationInView:self.view].y < self.view.bounds.size.height*.5){
                checkLeft = true;
            }
            else if([touch locationInView:self.view].y >= self.view.bounds.size.height*.5){
                checkRight = true;
            }
        }
    }
    leftButton = checkLeft;
    rightButton = checkRight;

//    for(UITouch *touch in [event allTouches]){
//        if([touch locationInView:self.view].y < self.view.bounds.size.height*.5){
//            leftButton = false;
//        }
//        else if([touch locationInView:self.view].y >= self.view.bounds.size.height*.5){
//            rightButton = false;
//        }
//    }
//    NSLog(@"(%d) L:%d  R:%d", touches.count, leftButton, rightButton);
}


#pragma mark - GLKView and GLKViewController delegate methods

- (void)update{

    elapsedSeconds = -[start timeIntervalSinceNow];
    
    float zoom = elapsedSeconds / 5.0;
    zoomLevel = (int)zoom;
    levelProgress = zoom - zoomLevel;
    
    if(leftButton) velocity = -.005f;
    else if(rightButton) velocity = .005f;
    else velocity = 0.0f;
    
    position += velocity;

//    float aspect = fabsf(self.view.bounds.size.width / self.view.bounds.size.height);
//    float aspect = fabsf(self.view.bounds.size.height / self.view.bounds.size.width);
//    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
//    
//    self.effect.transform.projectionMatrix = projectionMatrix;
//    
//    GLKMatrix4 baseModelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -4.0f);
//    
//    // Compute the model view matrix for the object rendered with GLKit
//    GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -1.5f);
//    modelViewMatrix = GLKMatrix4Translate(modelViewMatrix, 0.0f, 0.0f, 0.0f);
//    modelViewMatrix = GLKMatrix4Multiply(baseModelViewMatrix, modelViewMatrix);
//    
//    self.effect.transform.modelviewMatrix = modelViewMatrix;
//    
//    _modelViewProjectionMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
    
}

#define RADIX 5

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the object with GLKit
    [self.effect prepareToDraw];

    glPushMatrix();
    
    for(int levels = -1; levels < 3; levels++){

        float altitude = pow(RADIX,-(levelProgress+levels));
        float scale = pow(RADIX,(levelProgress+levels));
        
        glPushMatrix();
        glTranslatef(0.0f, 0.0, -2);
        glScalef(scale*.5, scale*.5, scale*.5);

        glTranslatef(0.0f, RADIX*position*altitude, 0.0f);
        
        for(int i = 0; i <= RADIX; i++){
            float gap = (float)i/(float)RADIX;
            glPushMatrix();
            glTranslatef(0, 0, .1);
            glTranslatef(0, gap-.5, 0);
            glRotatef(90, 1, 0, 0);
            glColor4f(i/(float)RADIX, 0.5f-i/(RADIX*2.0), 1.0f, 1.0f);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, quadVertexData);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

@end
