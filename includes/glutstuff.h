#ifndef _GLUTSTUFF
#define _GLUTSTUFF

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <GLUI/glui.h>
// Inlcude glui.h on macs...
#else
#include <GL/glut.h>
#include <GL/glui.h>
#endif

class glutstuff {
  
 public:
  void initRendering();

};

#endif
