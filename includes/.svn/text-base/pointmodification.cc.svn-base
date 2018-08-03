#include "pointmodification.h"


pointmodification::pointmodification( float aPe, float aTrueTime, float aHitX,
				      float aHitY, float aHitZ, float aMaxPe,
				      float aUpperTime, float aLowerTime,
				      float aUpperPe, float aLowerPe,
				      float aEventPosition[3], float aEventDirection[3], float aTau, 
				      float aReduction, float aResidualTimeBuffer, int aPixel ){

  setPe(aPe);
  setTau(aTau);
  setTrueTime(aTrueTime);
  setHitX(aHitX);
  setHitY(aHitY);
  setHitZ(aHitZ);
  setMaxPe(aMaxPe);
  setUpperTime(aUpperTime);
  setLowerTime(aLowerTime);
  setUpperPe(aUpperPe);
  setLowerPe(aLowerPe);
  setEventPosition(aEventPosition);
  setEventDirection(aEventDirection);
  setReduction(aReduction);
  setResidualTimeBuffer(aResidualTimeBuffer);
  setPixel(aPixel);
  setSpeed();

}
bool pointmodification::firstCuts(){

  if ( timeCut() && peCut() )
    return true;
  else
    return false;

}
bool pointmodification::secondCuts(){

  if ( residualCut() )
    return true;
  else
    return false;

}
float pointmodification::varyPoints(){

  return float(iPixel + (iPe / iMaxPe) * 6.0 + 0.5);

}
bool pointmodification::timeCut(){

  if ( iTrueTime < iUpperTime && iTrueTime > iLowerTime )
    return true;
  else
    return false;

}
bool pointmodification::peCut(){

  if ( (iPe <= iUpperPe && iPe >= iLowerPe) )
    return true;
  else
    return false;

}
bool pointmodification::residualCut(){

  float pmt[3] = {iHitX, iHitY, iHitZ};
  float realPosition[3] = { iEventPosition[0] * iReduction,
			    iEventPosition[1] * iReduction,
			    iEventPosition[2] * iReduction };
  iTau = iTau * speed_c;
  residualTime rt( iEventDirection, realPosition, pmt, iTau );
  float resTime = rt.findResidualTime() / speed_c;
  
  if ( iTrueTime < (resTime + iResidualTimeBuffer) &&
       iTrueTime > (resTime - iResidualTimeBuffer) )
    return true;
  else if ( iResidualTimeBuffer == 0 )
    return true;
  else
    return false;

}
void pointmodification::setEventPosition(float aEventPosition[3]){

  iEventPosition[0] = aEventPosition[0];
  iEventPosition[1] = aEventPosition[1];
  iEventPosition[2] = aEventPosition[2];

}
void pointmodification::setEventDirection(float aEventDirection[3]){

  iEventDirection[0] = aEventDirection[0];
  iEventDirection[1] = aEventDirection[1];
  iEventDirection[2] = aEventDirection[2];

}
