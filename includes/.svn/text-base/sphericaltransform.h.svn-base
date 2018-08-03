#ifndef _SPHERICAL_TRANSFORM
#define _SPHERICAL_TRANSFORM

class sphericalTransform{

 private:
  float inVecX;
  float inVecY;
  float inVecZ;
  float posX;
  float posY;
  float posZ;
  float costheta;
  float phi;
  float outVecX;
  float outVecY;
  float outVecZ;

 public:
  sphericalTransform( float ainVecX, float ainVecY, float ainVecZ,
		   float acostheta, float aphi,
		   float aposX, float aposY, float aposZ );
  void translation();
  void rotation();
  void returnVector(float &X, float &Y, float &Z);
  // Setters
  void setInVec( float ainVecX, float ainVecY, float ainVecZ );
  void setAngles( float acostheta, float aphi );
  void setPos( float aposX, float aposY, float aposZ );
  void setOutVec( float aoutVecX, float aoutVecY, float aoutVecZ );
  // Getters
  float getinVecX(){ return inVecX; }
  float getinVecY(){ return inVecY; }
  float getinVecZ(){ return inVecZ; }
  float getposX(){ return posX; }
  float getposY(){ return posY; }
  float getposZ(){ return posZ; }
  float getcostheta(){ return costheta; }
  float getphi(){ return phi; }
  float getoutVecX(){ return outVecX; }
  float getoutVecY(){ return outVecY; }
  float getoutVecZ(){ return outVecZ; }
};

#endif
