#include "cylinderstep.h"
#include <cmath>

cylinderStep::cylinderStep( float aposX, float aposY, float aposZ,
			    float adirX, float adirY, float adirZ,
			    float astepSize, float acylinderRadius,
			    float acylinderHeight, float aaccuracy ){

  setposX(aposX);
  setposY(aposY);
  setposZ(aposZ);
  setdirX(adirX);
  setdirY(adirY);
  setdirZ(adirZ);
  setstepSize(astepSize);
  setcylinderRadius(acylinderRadius);
  setcylinderHeight(acylinderHeight);
  setaccuracy(aaccuracy);

}

void cylinderStep::findIntersection(){
  
  float myRadius = sqrt( posX*posX + posY*posY );
  float myMagZ = fabs(posZ);
  float theRadius = getcylinderRadius();
  float theZ = fabs(getcylinderHeight()/2);
  float buffer = getaccuracy();
  float trigger = 1;
  float lastTrigger = 1;
  float stepsize = getstepSize();
  
  // Check that the vector will actually intersect
  

  // Outside the detector
  if( myRadius > theRadius || myMagZ > theZ ){
    while( !( myRadius <= (theRadius + buffer) &&
	      myRadius >= (theRadius - buffer) ) &&
	   !( myMagZ <= (theZ + buffer) &&
	      myMagZ >= (theZ - buffer) )
	   ){
      if ( myRadius > (theRadius + buffer) ||
	   myMagZ > (theZ + buffer) 
	   ){
	stepForwards();
	trigger = 1;
      }
    
      if ( myRadius < (theRadius - buffer) &&
	   myMagZ < (theZ - buffer) 
	   ){
	stepBackwards();
	trigger = -1;
      }
  
      if ( trigger != lastTrigger )
	setstepSize(getstepSize()/2);
      lastTrigger = trigger;
      myRadius = sqrt( posX*posX + posY*posY );
      myMagZ = fabs(posZ);
      
      if ( (posX*posX + posY*posY + posZ*posZ > 30) )
	break;
    }
  }
  
  
  // Inside the detector
  else{
    // if( myRadius < theRadius && myMagZ < theZ )
    
    while( !( myRadius >= (theRadius - buffer) &&
	      myRadius <= (theRadius + buffer) ) &&
	   !( myMagZ >= (theZ - buffer) &&
	      myMagZ <= (theZ + buffer) )
	   ){
      if ( myRadius < (theRadius - buffer) &&
	   myMagZ < (theZ - buffer) 
	   ){
	stepForwards();
	trigger = 1;
      }


      if ( myRadius > (theRadius + buffer) ||
	   myMagZ > (theZ + buffer) 
	   ){
	stepBackwards();
	trigger = -1;
      }
  
      if ( trigger != lastTrigger )
	setstepSize(getstepSize()/2);
      lastTrigger = trigger;
      myRadius = sqrt( posX*posX + posY*posY );
      myMagZ = fabs(posZ);
      if ( (posX*posX + posY*posY + posZ*posZ > 3) )
	break;
    }
  }

  setstepSize(stepsize);
}

void cylinderStep::stepForwards(){
  
  setposX( getposX() + getdirX()*getstepSize() );
  setposY( getposY() + getdirY()*getstepSize() );
  setposZ( getposZ() + getdirZ()*getstepSize() );

}

void cylinderStep::stepBackwards(){

  setposX( getposX() - getdirX()*getstepSize() );
  setposY( getposY() - getdirY()*getstepSize() );
  setposZ( getposZ() - getdirZ()*getstepSize() );

}
