#ifndef _OGLHISTOGRAM
#define _OGLHISTOGRAM

#include <vector>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>    
#include <GLUI/glui.h>    
// Inlcude glui.h on macs...
#else
#include <GL/glut.h>
#include <GL/glui.h>
#endif

class oglHistogram {

 private:
  std::vector<float> *iVariable;
  std::vector<float> *iPosition;
  std::vector<float> iBins;
  int iNumBins;
  float iSize;
  float iMaxValue;
  float iTopBin;
  float iMinValue;
  float iMaxCut;
  float iMinCut;
  bool iLog;

 public:
  oglHistogram(std::vector<float> &aVariable, int aNumBins,
	       std::vector<float> &aPosition, float aSize);
  oglHistogram(std::vector<float> &aVariable, int aNumBins,
	       std::vector<float> &aPosition, float aSize, 
	       float aMinValue, float aMaxValue, float aMinCut,
	       float aMaxCut, bool aLog = false );
  void drawAxes();
  void drawLabels(const char *name);
  void fillBins();
  void drawBins();
  void drawCutLines();
  void determineMaxValue();
  void determineMinValue();
  void determineTopBin();

  // Getters
  void getVariable(std::vector<float> *&aVariable){aVariable = iVariable;}
  void getPosition(std::vector<float> *&aPosition){aPosition = iPosition;}
  void getBins(std::vector<float> *&aBins){aBins = &iBins;}
  int getNumBins(){return iNumBins;}
  float getSize(){return iSize;}
  float getMaxValue(){return iMaxValue;}
  float getMinValue(){return iMinValue;}
  float getMaxCut(){return iMaxCut;}
  float getMinCut(){return iMinCut;}
  float getTopBin(){return iTopBin;}
  // Setters
  void setVariable(std::vector<float> &aVariable){iVariable = &aVariable;}
  void setPosition(std::vector<float> &aPosition){iPosition = &aPosition;}
  void setNumBins(int aNumBins){iNumBins = aNumBins;}
  void setSize(float aSize){iSize = aSize;} 
  void setMaxValue(float aMaxValue){iMaxValue = aMaxValue;}
  void setMinValue(float aMinValue){iMinValue = aMinValue;}
  void setMaxCut(float aMaxCut){iMaxCut = aMaxCut;}
  void setMinCut(float aMinCut){iMinCut = aMinCut;}
  void setTopBin(float aTopBin){iTopBin = aTopBin;}
  void setLog(bool aLog){iLog = aLog;}

};

#endif
