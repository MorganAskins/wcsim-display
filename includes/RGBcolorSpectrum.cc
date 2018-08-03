#include "RGBcolorSpectrum.h"

RGBcolorSpectrum::RGBcolorSpectrum(float aRange){

  iRange = aRange;
  iSectionSize = aRange / 4.0;

}
void RGBcolorSpectrum::returnSpectrum(float aValue, float &aR, 
				      float &aG, float &aB){

  iRGB[0] = &aR;
  iRGB[1] = &aG;
  iRGB[2] = &aB;
  iValue = aValue;
  setSpectrum();
  
  return;
}

void RGBcolorSpectrum::setSpectrum(){

  // If the value is out of range still give it a color
  if ( iValue < 0 ){ // below range
    *iRGB[2] = 1;
    *iRGB[1] = 0;
    *iRGB[0] = 0;
  }
  else if ( iValue > iRange ){ // above range
    *iRGB[2] = 0;
    *iRGB[1] = 0;
    *iRGB[0] = 1;
  }
  else if ( iValue <= iSectionSize * 1 ){
    *iRGB[2] = 1;
    *iRGB[1] = 1 - (iSectionSize * 1 - iValue) / iSectionSize;
    *iRGB[0] = 0;
  }
  else if ( iValue <= iSectionSize * 2 ){
    *iRGB[2] = (iSectionSize * 2 - iValue) / iSectionSize;
    *iRGB[1] = 1;
    *iRGB[0] = 0;
  }
  else if ( iValue <= iSectionSize * 3 ){
    *iRGB[2] = 0;
    *iRGB[1] = 1;
    *iRGB[0] = 1 - (iSectionSize * 3 - iValue) / iSectionSize;
  }
  else if ( iValue <= iSectionSize * 4 ){
    *iRGB[2] = 0;
    *iRGB[1] = (iSectionSize * 4 - iValue) / iSectionSize;
    *iRGB[0] = 1;
  }


  return;
}
