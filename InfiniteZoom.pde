float zoom = 1;
float logZoom;
int zoomLevel;
float levelProgress;
float altitude;
float levelAltitude;

int RADIX = 9;

float log10 (float x) {
  return (log(x) / log(10));
}

void setup(){
  size(640,480);
  textSize(50);
}

void update(){
  zoom = millis() / 4000.0;
  zoomLevel = (int)zoom;

  levelProgress = zoom - zoomLevel;
  altitude = pow(10,-zoom);
  levelAltitude = pow(10,-levelProgress);

}

void draw(){
  update();

  background(255);
  fill(0);
  
  strokeWeight(5);
  for(int i = 2; i >= -2; i--){
    stroke(255*(i+2)/5.0);
    float thisAltitude = pow(RADIX,-(zoom+i-zoomLevel-1));
    float size = pow(RADIX,(zoom+i-zoomLevel-1));
    line(width*.5 - 10*size, height*.95 - thisAltitude, width*.5 + 10*size, height*.95 - thisAltitude); 
  } 
  
  strokeWeight(1);
  for(int levels = 3; levels >= 0; levels--){
    stroke(255*levels/4.0);
    float scale = pow(RADIX,(levelProgress+levels));
    for(float f = -.5; f <= .5; f+=1.0/RADIX){
      line(width*.5 + f*(scale), height*.15-.1*scale, width*.5 + f*(scale), height*.15+.1*scale);
    }
  }

/////////////////////
/////////////////////

  stroke(0);
  strokeWeight(15);

  //  circle groups, linear and logarithmic
  float room = 50;  
  for(int i = -2; i <= 2; i++){
    float scale = ((i+(1-levelProgress))*room);
    ellipse(60, height*.5 + scale, 30, 30);
  }
  
  for(int i = -2; i <= 2; i++){
    float thisAltitude = pow(10,-(zoom+i-zoomLevel-1));
    float scale = thisAltitude;
    ellipse(width - 60, height*.5 + scale, 30, 30);
  }
  
  // top,bottom lines, side circles

  line(0, 0, width*levelProgress, 0);
//  text(zoomLevel, 20, 60);
  ellipse(1,height - altitude*height,30,30);
  ellipse(width-1,height - levelAltitude*height,30,30);
  line(0, height, width*(1-levelAltitude)/.9, height);

}


