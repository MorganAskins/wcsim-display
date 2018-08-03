#include "sphericaltransform.h"
#include <iostream>
#include <cmath>
using namespace std;

sphericalTransform::sphericalTransform( float ainVecX, float ainVecY, 
				  float ainVecZ, float acostheta,
				  float aphi,
				  float aposX, float aposY, float aposZ ){

  setInVec( ainVecX, ainVecY, ainVecZ );
  setAngles( acostheta, aphi );
  setPos( aposX, aposY, aposZ );
  setOutVec( 0, 0, 0 );

}

void sphericalTransform::translation(){
  
  setOutVec( getoutVecX() + getposX(),
	     getoutVecY() + getposY(),
	     getoutVecZ() + getposZ() );
  
}

void sphericalTransform::rotation(){

  float X = getinVecX();
  float Y = getinVecY();
  float Z = getinVecZ();
  float Costheta = getcostheta();
  float Sintheta = sqrt(1-Costheta*Costheta);
  float Cosphi = cos(getphi());
  float Sinphi = sin(getphi());

  float Xprime = X;
  float Yprime = Y*Costheta - Z*Sintheta;
  float Zprime = Y*Sintheta + Z*Costheta;

  setOutVec( -Xprime*Sinphi - Yprime*Cosphi,
	     Xprime*Cosphi -Yprime*Sinphi,
	     Zprime );

}

void sphericalTransform::returnVector(float &X, float &Y, float &Z){

  rotation();
  translation();
  X = getoutVecX();
  Y = getoutVecY();
  Z = getoutVecZ();
  
}

// Setters
void sphericalTransform::setInVec( float ainVecX, float ainVecY, 
				float ainVecZ ){
  inVecX = ainVecX;
  inVecY = ainVecY;
  inVecZ = ainVecZ;
}
void sphericalTransform::setAngles( float acostheta, float aphi ){
  costheta = acostheta;
  phi = aphi;
}
void sphericalTransform::setPos( float aposX, float aposY, float aposZ ){
  posX = aposX;
  posY = aposY;
  posZ = aposZ;
}
void sphericalTransform::setOutVec( float aoutVecX, float aoutVecY, 
				 float aoutVecZ ){
  outVecX = aoutVecX;
  outVecY = aoutVecY;
  outVecZ = aoutVecZ;
}
