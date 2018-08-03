#ifndef _CYLINDERSTEP
#define _CYLINDERSTEP

class cylinderStep {
 private:
  float posX;
  float posY;
  float posZ;
  float dirX;
  float dirY;
  float dirZ;
  float stepSize;
  float cylinderRadius;
  float cylinderHeight;
  float accuracy;

 public:
  cylinderStep(float, float, float, float, float,
	       float, float, float, float, float );
  void findIntersection();
  void stepForwards();
  void stepBackwards();

  // Setters
  void setposX(float aposX){posX = aposX;}
  void setposY(float aposY){posY = aposY;}
  void setposZ(float aposZ){posZ = aposZ;}
  void setdirX(float adirX){dirX = adirX;}
  void setdirY(float adirY){dirY = adirY;}
  void setdirZ(float adirZ){dirZ = adirZ;}
  void setstepSize(float astepSize){stepSize = astepSize;}
  void setcylinderRadius(float acylinderRadius){cylinderRadius = acylinderRadius;}
  void setcylinderHeight(float acylinderHeight){cylinderHeight = acylinderHeight;}
  void setaccuracy(float aaccuracy){accuracy = aaccuracy;}
  // Getters
  float getposX(){return posX;}
  float getposY(){return posY;}
  float getposZ(){return posZ;}
  float getdirX(){return dirX;}
  float getdirY(){return dirY;}
  float getdirZ(){return dirZ;}
  float getstepSize(){return stepSize;}
  float getcylinderRadius(){return cylinderRadius;}
  float getcylinderHeight(){return cylinderHeight;}
  float getaccuracy(){return accuracy;}

};

#endif
