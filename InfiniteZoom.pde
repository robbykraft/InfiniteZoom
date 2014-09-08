float zoom = 1;
float logZoom;
int zoomLevel;
float levelProgress;
float altitude;
float levelAltitude;

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
  // log10(altitude) = zoom
}

void draw(){
  update();

  background(255);
  stroke(0);
  fill(0);
  strokeWeight(15);
  
  float room = 50;
  
  for(int i = -2; i <= 2; i++){
    float scale = ((i+(1-levelProgress))*room);
    println(scale);
    ellipse(width*.33, height*.5 + scale, 30, 30);
  }
  
  for(int i = -2; i <= 2; i++){
    float thisAltitude = pow(10,-(zoom+i-zoomLevel-2));
    float scale = thisAltitude*room;
    println(scale);
    ellipse(width*.66, height*.5 + scale, 30, 30);
  }

  line(0, 0, width*levelProgress, 0);
  text(zoomLevel, 20, 60);
  ellipse(1,height - altitude*height,30,30);
  ellipse(width-1,height - levelAltitude*height,30,30);
}


