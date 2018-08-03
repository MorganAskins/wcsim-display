#include <vector>
#include <cmath>
#include "residualtime.h"
using namespace std;

residualTime::residualTime( float aDir[3], float aEpos[3],
	      float aPMTpos[3], float &act0 ){

  direction[0] = aDir[0];
  direction[1] = aDir[1];
  direction[2] = aDir[2];
  eventPos[0] = aEpos[0];
  eventPos[1] = aEpos[1];
  eventPos[2] = aEpos[2];
  pmtPos[0] = aPMTpos[0];
  pmtPos[1] = aPMTpos[1];
  pmtPos[2] = aPMTpos[2];
  index_n = 1.33;
  rho[0] = pmtPos[0] - eventPos[0];
  rho[1] = pmtPos[1] - eventPos[1];
  rho[2] = pmtPos[2] - eventPos[2];
  ct0 = act0;
  makeUnitVector();

  return;  
}

float residualTime::MagCrossProduct( float vec1[3], float vec2[3] ){

  return ( sqrt( pow(vec1[1]*vec2[2]-vec1[2]*vec2[1],2) +		 
		 pow(vec1[0]*vec2[2]-vec1[2]*vec2[0],2) +
		 pow(vec1[0]*vec2[1]-vec1[1]*vec2[0],2) ) );
}

float residualTime::dotProduct( float vec1[3], float vec2[3] ){

  return ( vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2] );

}

float residualTime::findResidualTime(){
  
  return ( ct0 + dotProduct(rho, direction)
	   + sqrt(index_n*index_n-1) * MagCrossProduct(rho, direction) );

}

void residualTime::makeUnitVector(){
  
  float normal = sqrt( direction[0] * direction[0] +
		       direction[1] * direction[1] +
		       direction[2] * direction[2] );
  direction[0] = direction[0] / normal;
  direction[1] = direction[1] / normal;
  direction[2] = direction[2] / normal;

}
