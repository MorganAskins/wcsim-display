#ifndef _POINTMODIFICATION
#define _POINTMODIFICATION
#include "residualtime.h"


class pointmodification {

 private:
  float iPe;
  float iTrueTime;
  float iHitX;
  float iHitY;
  float iHitZ;
  float iMaxPe;
  float iUpperTime;
  float iLowerTime;
  float iUpperPe;
  float iLowerPe;
  float iTau;
  float iEventPosition[3];
  float iEventDirection[3];
  float iReduction;
  float iResidualTimeBuffer;
  int iPixel;
  float speed_c; // cm/ns

 public:
  pointmodification( float aPe, float aTrueTime, float aHitX,
		     float aHitY, float aHitZ, float aMaxPe,
		     float aUpperTime, float aLowerTime,
		     float aUpperPe, float aLowerPe, float aEventPosition[3],
		     float aEventDirection[3], float aTau, float aReduction,
		     float aResidualTimeBuffer, int aPixel );
  bool firstCuts();
  bool timeCut();
  bool peCut();
  bool secondCuts();
  bool residualCut();
  float varyPoints();

  // Setters
  void setPe(float aPe){iPe = aPe;}
  void setTrueTime(float aTrueTime){iTrueTime = aTrueTime;}
  void setHitX(float aHitX){iHitX = aHitX;}
  void setHitY(float aHitY){iHitY = aHitY;}
  void setHitZ(float aHitZ){iHitZ = aHitZ;}
  void setMaxPe(float aMaxPe){iMaxPe = aMaxPe;}
  void setUpperTime(float aUpperTime){iUpperTime = aUpperTime;}
  void setLowerTime(float aLowerTime){iLowerTime = aLowerTime;}
  void setUpperPe(float aUpperPe){iUpperPe = aUpperPe;}
  void setLowerPe(float aLowerPe){iLowerPe = aLowerPe;}
  void setReduction(float aReduction){iReduction = aReduction;}
  void setResidualTimeBuffer(float aResidualTimeBuffer){
    iResidualTimeBuffer = aResidualTimeBuffer; }
  void setEventPosition(float aEventPosition[3]);
  void setEventDirection(float aEventDirection[3]);
  void setSpeed(){speed_c = 29.9792458;};
  void setTau(float aTau){iTau = aTau;}
  void setPixel(int aPixel){iPixel = aPixel;}

};

#endif
