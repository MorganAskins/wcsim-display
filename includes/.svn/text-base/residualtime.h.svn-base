#ifndef _RESIDUALTIME
#define _RESIDUALTIME

#include <vector>
#include <cmath>
using namespace std;

class residualTime {

 private:
  float direction[3];		/* Direction unit vector */
  float eventPos[3];		/* Position of the event */
  float pmtPos[3];		/* Position of the pmt */
  float ct0;			/* initial time of the event */
  float index_n;		/* index of refraction */
  float ct;			/* residual time */
  float rho[3];			/* pmtPos - eventPos */

 public:
  residualTime( float aDir[3], float aEpos[3],
		float aPMTpos[3], float &act0 );
  float MagCrossProduct( float vec1[3], float vec2[3] );
  float dotProduct( float vec1[3], float vec2[3] );
  float findResidualTime();
  void makeUnitVector();
};

#endif
