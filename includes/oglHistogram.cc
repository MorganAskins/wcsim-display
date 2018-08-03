#include "oglHistogram.h"
#include "RGBcolorSpectrum.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

oglHistogram::oglHistogram(vector<float> &aVariable, int aNumBins,
			   vector<float> &aPosition, float aSize){

  setVariable(aVariable);
  setNumBins(aNumBins);
  setPosition(aPosition);
  setSize(aSize);
  determineMaxValue();
  determineMinValue();
  fillBins();
  determineTopBin();

} 

oglHistogram::oglHistogram(vector<float> &aVariable, int aNumBins,
			   vector<float> &aPosition, float aSize,
			   float aMinValue, float aMaxValue, 
			   float aMinCut, float aMaxCut, bool aLog ){

  setVariable(aVariable);
  setLog(aLog);
  setNumBins(aNumBins);
  setPosition(aPosition);
  setSize(aSize);
  setMaxValue(aMaxValue);
  setMinValue(aMinValue);
  fillBins();
  determineTopBin();
  setMaxCut(aMaxCut);
  setMinCut(aMinCut);


} 

void oglHistogram::drawAxes(){

  vector<float> *aPosition;
  getPosition(aPosition);
  float aSize = getSize();

  glLineWidth(3.0);
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(aPosition->at(0), aPosition->at(1), aPosition->at(2));
  glVertex3f(aPosition->at(0) + aSize, aPosition->at(1), 
	     aPosition->at(2));
  glVertex3f(aPosition->at(0), aPosition->at(1), aPosition->at(2));
  glVertex3f(aPosition->at(0), aPosition->at(1) + 0.8 * aSize, 
	     aPosition->at(2));
  glEnd();

}
 
void oglHistogram::drawLabels(const char *name){

  vector<float> *aPosition;
  getPosition(aPosition);
  float aSize = getSize();
  float aMaxValue = getMaxValue();
  float aMinValue = getMinValue();
  
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glRasterPos3f(aPosition->at(0), 
  		aPosition->at(1) + aSize*0.9,
  		aPosition->at(2));
  for (int i=0; i < static_cast<int>(strlen(name)); i++){
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[i]);
  }

  float numberofmarkers = 4;
  
  glLineWidth(3.0);
  for (int i = 0; i <= numberofmarkers; i++){
    float theValue = aMaxValue - (aMaxValue-aMinValue)*(i/numberofmarkers);
    stringstream valuename;
    valuename << fixed << setprecision(0) << theValue;
    string valuestring = valuename.str();
    char *valuechar = (char*)valuestring.c_str();
    glRasterPos3f( aPosition->at(0) + aSize * 
    		   (numberofmarkers - i) / numberofmarkers, 
    		   aPosition->at(1) - aSize * 0.1, aPosition->at(2) - 0.2);
    for (int j=0; j < static_cast<int>(strlen(valuechar)); j++){
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, valuechar[j]);
    }
    glBegin(GL_LINES);    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(aPosition->at(0) + aSize * 
	       (numberofmarkers - i) / numberofmarkers, 
	       aPosition->at(1) + aSize * 0.05, aPosition->at(2) - 0.1);
    glVertex3f(aPosition->at(0) + aSize * 
	       (numberofmarkers - i) / numberofmarkers, 
	       aPosition->at(1) - aSize * 0.05, aPosition->at(2) - 0.1);
    glEnd();
  }

}

void oglHistogram::fillBins(){

  vector<float> *aVariable, *aBins;
  float aNumBins = getNumBins();
  float aMaxValue = getMaxValue();
  float aMinValue = getMinValue();
  getVariable(aVariable);
  getBins(aBins);
  aBins->resize(aNumBins);
  
  for(unsigned int i=0; i < aVariable->size(); i++){
    if(aVariable->at(i) < aMaxValue && aVariable->at(i) > aMinValue ){
      int theBin = static_cast<int>( (aVariable->at(i) - aMinValue) 
				     * ( aNumBins / (aMaxValue - aMinValue)) );
      aBins->at(theBin) += 1;
    }
    else if (aVariable->at(i) >= aMaxValue)
      aBins->at(aNumBins-1) += 1;
    else
      aBins->at(0) += 1;

  }


  if(iLog){
    for(int i = 0; i < aBins->size(); i++)
      if(aBins->at(i) != 0)
    	aBins->at(i) = log10(aBins->at(i));
  }

  
}

void oglHistogram::drawBins(){

  vector<float> *aBins, *aPosition;
  float aNumBins = getNumBins();
  float aSize = getSize();
  float distance = aSize / aNumBins;
  float aTopBin = getTopBin();
  getBins(aBins);
  getPosition(aPosition);
  // Add colors
  float R, G, B;
  RGBcolorSpectrum rainbow(aNumBins);


  for(int i=0; i < aNumBins; i++){
    rainbow.returnSpectrum(float(i),R,G,B);
    glColor4f(R, G, B, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(aPosition->at(0) + i * distance, aPosition->at(1), 
	       aPosition->at(2));
    glVertex3f(aPosition->at(0) + i * distance + distance, 
	       aPosition->at(1), aPosition->at(2));
    glVertex3f(aPosition->at(0) + i * distance + distance, 
	       aPosition->at(1) + 0.8 * aSize * (aBins->at(i) / aTopBin), 
	       aPosition->at(2));
    glVertex3f(aPosition->at(0) + i * distance, aPosition->at(1) + 0.8 
	       * aSize * (aBins->at(i) / aTopBin), aPosition->at(2));
    glEnd();
  }
}

void oglHistogram::drawCutLines(){

  vector<float> *aPosition;
  float aSize = getSize();
  getPosition(aPosition);

  glLineWidth(2.0);
  glBegin(GL_LINES);
  glColor4f(1.0, 1.0, 1.0, 1.0);
  // Min Cut
  if( iMinCut > iMinValue && iMinCut < iMaxValue){
    glVertex3f(aPosition->at(0) + ( (iMinCut - iMinValue) / 
				    (iMaxValue - iMinValue) ) * aSize,
	       aPosition->at(1), aPosition->at(2) );
    glVertex3f(aPosition->at(0) + ( (iMinCut - iMinValue) / 
				    (iMaxValue - iMinValue) ) * aSize,
	       aPosition->at(1) + 0.8 * aSize, aPosition->at(2) );
  }
  // Max Cut
  if( iMaxCut < iMaxValue && iMaxCut > iMinValue){
    glVertex3f(aPosition->at(0) + ( (iMaxCut - iMinValue) / 
				    (iMaxValue - iMinValue) ) * aSize,
	       aPosition->at(1), aPosition->at(2) );
    glVertex3f(aPosition->at(0) + ( (iMaxCut - iMinValue) / 
				    (iMaxValue - iMinValue) ) * aSize,
	       aPosition->at(1) + 0.8 * aSize, aPosition->at(2) );
  }
  glEnd();

}

void oglHistogram::determineMaxValue(){

  vector<float> *aVariable;
  getVariable(aVariable);
  float aMaxValue = 0;
  for(unsigned int i = 0; i < aVariable->size(); i++){
    if( aVariable->at(i) > aMaxValue )
      aMaxValue = aVariable->at(i);
  }

  setMaxValue(aMaxValue);

}
void oglHistogram::determineMinValue(){

  vector<float> *aVariable;
  getVariable(aVariable);
  float aMinValue = 0;
  for(unsigned int i = 0; i < aVariable->size(); i++){
    if( aVariable->at(i) < aMinValue )
      aMinValue = aVariable->at(i);
  }

  setMinValue(aMinValue);

}
void oglHistogram::determineTopBin(){

  vector<float> *aBins;
  getBins(aBins);

  float aTopBin = 0;
  for(unsigned int i=0; i < aBins->size(); i++)
    if( aBins->at(i) > aTopBin )
      aTopBin = aBins->at(i);

  setTopBin(aTopBin);

}
