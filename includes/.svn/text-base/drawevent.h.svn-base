#ifndef _DRAWEVENT
#define _DRAWEVENT
#include <cmath>
#include "sphericaltransform.h"
#include "RGBcolorSpectrum.h"
#include "pointmodification.h"
#include "cylinderstep.h"
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <GLUI/glui.h>
// Inlcude glui.h on macs...                                                                 
#else
#include <GL/glut.h>
#include <GL/glui.h>
#endif

class drawevent {
 private:
  float PI;
  float iCylinderRadius;
  float iCylinderHeight;
  float iReduction;
  // Pointers to all of the vectors used
  vector<float> *iHitX;
  vector<float> *iHitY;
  vector<float> *iHitZ;
  vector<float> *iPe;
  vector<float> *iTrueTime;
  float iMaxPe;
  float iUpperTime;
  float iLowerTime;
  float iUpperPe;
  float iLowerPe;
  float iUpperTimeHisto;
  float iLowerTimeHisto;
  float iUpperPeHisto;
  float iLowerPeHisto;
  float *iEventPosition;
  float *iEventDirection;
  float iResidualTimeBuffer;
  float iPixel;
  int *iRingColor;
  float iTheta2D;
  float *iCosTheta;
  float *iPhi;
  float iConeAlpha;
  int iColorMethod;
  float iTau;
  int iNumberOfRings;
  float iNewDirection[3];

 public:
  drawevent(float aCylinderRadius, float aCylinderHeight,
	    float aReduction, float aMaxPe, float aUpperTime,
	    float aLowerTime, float aUpperPe, float aLowerPe,
	    float aUpperTimeHisto, float aLowerTimeHisto,
	    float aUpperPeHisto, float aLowerPeHisto,
	    float *aEventPosition, float *aEventDirection,
	    float aResidualTimeBuffer, int *aRingColor,
	    float aPixel, float aTheta2D, float *aCosTheta, 
	    float *aPhi, float aConeAlpha, int aNumberOfRings,
	    int aColorMethod, float aTau,
	    vector<float> &aHitX, vector<float> &aHitY, vector<float> &aHitZ, 
	    vector<float> &aPe, vector<float> &aTrueTime );
  void draw2DBackground();
  void draw2DEventPoints();
  void draw2DCherenkovRing();
  void draw3DDetector();
  void draw3DEventPoints();
  void draw3DCherenkovRing();
  void drawProjection();
	void drawProjectionPhiTheta();
  ~drawevent();

  // Setters
  void setCylinderRadius(float aCylinderRadius){iCylinderRadius = aCylinderRadius;}
  void setCylinderHeight(float aCylinderHeight){iCylinderHeight = aCylinderHeight;}
  void setReduction(float aReduction){iReduction = aReduction;}
  void setMaxPe(float aMaxPe){iMaxPe = aMaxPe;}
  void setUpperTime(float aUpperTime){iUpperTime = aUpperTime;}
  void setLowerTime(float aLowerTime){iLowerTime = aLowerTime;}
  void setUpperPe(float aUpperPe){iUpperPe = aUpperPe;}
  void setLowerPe(float aLowerPe){iLowerPe = aLowerPe;}
  void setUpperTimeHisto(float aUpperTimeHisto){iUpperTimeHisto = aUpperTimeHisto;}
  void setLowerTimeHisto(float aLowerTimeHisto){iLowerTimeHisto = aLowerTimeHisto;}
  void setUpperPeHisto(float aUpperPeHisto){iUpperPeHisto = aUpperPeHisto;}
  void setLowerPeHisto(float aLowerPeHisto){iLowerPeHisto = aLowerPeHisto;}
  void setTheta2D(float aTheta2D){iTheta2D = aTheta2D;}
  void setResidualTimeBuffer(float aResidualTimeBuffer){
    iResidualTimeBuffer = aResidualTimeBuffer;}
  void setPixel(float aPixel){iPixel = aPixel;}
  void setCosTheta(float *aCosTheta){iCosTheta = aCosTheta;}
  void setPhi(float *aPhi){iPhi = aPhi;}
  void setConeAlpha(float aConeAlpha){iConeAlpha = aConeAlpha;}
  void setNumberOfRings(int aNumberOfRings){iNumberOfRings = aNumberOfRings;}
  void setColorMethod(int aColorMethod){iColorMethod = aColorMethod;}
  void setEventPosition(float *aEventPosition){iEventPosition = aEventPosition;}
  void setEventDirection(float *aEventDirection){iEventDirection = aEventDirection;}
  void setHitX(vector<float> &aHitX){iHitX = &aHitX;}
  void setHitY(vector<float> &aHitY){iHitY = &aHitY;}
  void setHitZ(vector<float> &aHitZ){iHitZ = &aHitZ;}
  void setPe(vector<float> &aPe){iPe = &aPe;}
  void setTrueTime(vector<float> &aTrueTime){iTrueTime = &aTrueTime;}
  void setNewDirection(int q);
  void setTau(float aTau){iTau = aTau;}
  void setRingColor(int *aRingColor){iRingColor = aRingColor;}
};

#endif
