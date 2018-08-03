/* 
   This software is intended to reconstruct and display
   an event from WCSim root output files in 2D and 3D.
   This software should further be able to analyze an event
   by coloring coding based upon energy as well as making
   various cuts. All of this should be packaged neatly into
   a very simple GUI so that the user doesn't need to
   know the source code to navigate.

   NOTE: While this software should be able to compile on
   any OS, it is only tested to work on Ubuntu 10.10
   with all of the proper root, WCSim, OpenGL, and GLUI
   libraries installed.

   Author: Morgan Askins
   Institution: University of California Davis
   Contact: maaskins@ucdavis.edu
*/

#include "handleroot.h"
#include "residualtime.h"
#include "cylinderstep.h"
#include "sphericaltransform.h"
#include "oglHistogram.h"
#include "RGBcolorSpectrum.h"
#include "glutstuff.h"
#include "pointmodification.h"
#include "drawevent.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <GLUI/glui.h>
// Inlcude glui.h on macs...
#else
#include <GL/glut.h>
#include <GL/glui.h>
#endif
// OS dependent sleep
#ifdef _WIN32
#include <windows.h>
inline void mySleep(unsigned seconds)
{
  ::Sleep(seconds);
}
#else
#include <unistd.h>
inline void mySleep(unsigned seconds)
{
  ::usleep(seconds * 1000);
}
#endif
using namespace std;

/* In order to draw we need global vectors */
vector<float> pmtID, pmtX, pmtY, pmtZ;
vector<float> hitX, hitY, hitZ, Pe, trueTime;
double vertexTrue[3], vertexReco[3], dirTrue[3], dirReco[3];
float CylinderHeight, CylinderRadius;
float scale = 0.8;
float resize_x = 1, resize_y = 1;
float maxpe = 0, minpe=0, upperPe = 0, lowerPe = 0;
float lowerPeHisto = 0, upperPeHisto = 0;
float maxtime = 0, mintime=0, upperTime = 0, lowerTime = 0,
  upperTimeHisto = 0, lowerTimeHisto = 0;
int isLogPe = 0;
int totalEvents = 1;
int pixel = 0;
float detectorAlpha = 0.1;
float coneAlpha = 0.0;
int bins = 20;
const float PI = 4.0*atan(1);
float tau = 0;
float moviespeed = 1;
int isforward = 1;
int movietype = 0;
// Translation and rotation variables
float xypos[2] = {0, 0}; 
float zpos = 0;
float rotation_matrix[16] = { 1.0, 0.0, 0.0, 0.0,
			      0.0, 1.0, 0.0, 0.0,
			      0.0, 0.0, 1.0, 0.0,
			      0.0, 0.0, 0.0, 1.0 };
float theMatrix[16] = { 1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0 };
// Direction and position of pseudo event used
// for residual timing cuts and event projection
const int totalrings = 6;
int numberofrings = 1;
int currentring = 1;
float eventdirection[totalrings * 3];
float eventposition[totalrings * 3];
float costheta[totalrings];
float phi[totalrings];
int ringcolor[totalrings];
float eventdirection_now[3];
float eventposition_now[3];
float costheta_now;
float phi_now;
int ringcolor_now;
int clarityvar = 0, ringcountvar = 0;
// Color chooser
int colorMethod = 0;
// Time before and after residual time to cut at
float residualTimeBuffer = 0;
// Speed of light in units modified to the detector size
const float speed_c = 29.9792458; // cm/ns
float mod_c = speed_c;
float reduction; // Normalize picture to fit screen
// Window title
const char *window_title = "3D Event Display";
const char *window2D_title = "2D Event Display";
// Window Position on screen
int window_x=0;
int window_y=0;
// GLUI variables
GLUI *glui_window2D;
GLUI *glui_window;
GLUI *glui_window2;
// All GLUI controls are declared here
GLUI_Panel *op_panel, *transformation_panel, *cuts_panel, *res_time, *panel_2D,
  *cherenkov_panel, *histogram_panel, *output_panel, *movie_panel;
GLUI_Rollout *transformation_rollout, *cuts_rollout, *cherenkov_rollout, 
  *histogram_rollout;
GLUI_RadioGroup *dimensionSelect;
GLUI_Translation *translation_xy, *translation_z;
GLUI_Spinner *event_spinner, *zoomFunc, *pixel_spinner, *pe_min_spinner,
  *pe_max_spinner, *time_min_spinner, *time_max_spinner, *res_time_spinner,
  *cherenkov_cone_opacity, *vec_position_X_spinner, *vec_position_Y_spinner,
  *vec_position_Z_spinner, *vec_direction_costheta, *vec_direction_phi, 
  *my_theta, *ring_spinner, *ring_number_spinner, *pe_min_histo_spinner, 
  *pe_max_histo_spinner, *time_min_histo_spinner, *time_max_histo_spinner, 
  *tau_spinner, *bins_spinner, *moviespeed_spinner;
GLUI_Rotation *glui_arcball;
GLUI_StaticText *detectorVolume;
GLUI_Listbox *color_list, *ring_color;
GLUI_EditText *ringcount, *clarity;
GLUI_Button *movieone, *movietwo, *stopmovie, *forwardmovie, *backmovie;
// The id ofthe main window
GLuint main_window;
GLuint newWindow;
// Toggle variables
int displayEvent = 1;		// Display the event in the window
int displayDetector = 1;	// Display the detector in the window
int drawMode = 0;			// Draw the 3D event display
float myTheta2D = PI;
int EventNum = 0;
int thisEvent = 0;
int lastEvent = -1;
int argc_globe;
char **argv_globe;
int isFullScreen = 0;
bool movieOn = false;


// Function declarations
void camera();
float normalize();
void SetMaxMinPe();
void SetMaxMinTime();
void renderevent();
void update(int value);
void handleKeypress(unsigned char key, int x, int y);
void handleResize(int w, int h);
void drawGL(int argc, char **argv);
void setupGLUI();
void updateGLUI();
void idle();
float findResidualTime(float trueTime, float hitX, 
		       float hitY, float hitZ );
void draw2DEvent();
void setDirectionFromAngles();
void getRootInfo(int argc, char **argv);
void setArgumentsGlobal(int argc, char **argv);
void myTestDrawing();
// void myTestGLUI();
void InitializeVariables();
void printdetectorinfo();
void printtoterminal();
float totalPe();
float rmsTime(float mean);
float meanTime();
void playmovie();

// Setup glui callback functions
void glui_callback(int arg);
// Using the following callback IDs
enum{
  QUIT = 0,
  FULLSCREEN,
  REDRAW,
  NEWDIMENSION,
  NEWRING,
  RESET,
  PRINTOUT,
  MOVIEONE,
  MOVIETWO,
  MOVIE_PAUSE,
  MOVIE_BACK,
  TRUERING,
  FITRING
};

int main(int argc, char **argv){
  
  // Check that a file is specified
  if (argc != 2){
    cerr << "Format is $./runme datafile.root" << endl;
    exit (0);
  }
  
  // Display in OpenGL
  setArgumentsGlobal(argc, argv);
  drawGL(argc, argv);
  
  return 0;
}

// Mouse control over environment
void camera(){
  if(drawMode == 1){
    glScalef(resize_x * scale, resize_y * scale, scale);
    glMultMatrixf( rotation_matrix );
  }
  else if(drawMode ==0){
    glScalef(resize_x * scale/2.5, resize_y * scale/2.5, scale/2.5);
    glTranslated(-xypos[0], -xypos[1], -zpos);
  }
  else if(drawMode == 2){
    glScalef(resize_x * scale, resize_y * scale, scale);
    glMultMatrixf( rotation_matrix );
  }
  glMultMatrixf( theMatrix );
}

float normalize(){
  // This will normalize the event so that
  // It fits within one window                          
  float maxValue = 1;
  for (unsigned int i=0; i < pmtX.size(); i++){
    if ( pmtX[i] > maxValue )
      maxValue = pmtX[i];
    if ( pmtY[i] > maxValue )
      maxValue = pmtY[i];
    if ( pmtZ[i] > maxValue )
      maxValue = pmtZ[i];
  }
  return maxValue;
}

void SetMaxMinPe(){

  maxpe = -1;
  for(unsigned int i=0; i<Pe.size(); i++){
    if (Pe[i] > maxpe)
      maxpe = Pe[i];
  }
  upperPe = maxpe;
  upperPeHisto = maxpe;
}

void SetMaxMinTime(){

  maxtime = -1;
  mintime = 1000000;
  
  for(unsigned int i=0; i<trueTime.size(); i++){
    if (trueTime[i] > maxtime)
      maxtime = trueTime[i];
    if (trueTime[i] < mintime)
      mintime = trueTime[i];
  }

  upperTime = maxtime;
  lowerTime = mintime;

}


void renderevent(){

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  camera();
  
  thisEvent = EventNum;
  if ( thisEvent != lastEvent ){
    getRootInfo( argc_globe, argv_globe );
    updateGLUI();
    lastEvent = thisEvent;
  }
  
  //** Put all of this somewhere **//
  reduction = normalize(); //This must normalize to fit the screen
  mod_c = speed_c / reduction;
  if(currentring != 0){
    costheta[currentring-1] = costheta_now;
    phi[currentring-1] = phi_now;
    eventposition[(currentring-1)*3 + 0] = eventposition_now[0]*cos(eventposition_now[1]);
    eventposition[(currentring-1)*3 + 1] = eventposition_now[0]*sin(eventposition_now[1]);
    eventposition[(currentring-1)*3 + 2] = eventposition_now[2];
    ringcolor[currentring-1] = ringcolor_now;
  }
  setDirectionFromAngles();
  //******************************//

  // Allow for the drawevent class to handle displaying both 2D and 3D
  // events including coloring, cuts on points and cone intersections
  drawevent thisdisplay( CylinderRadius, CylinderHeight, reduction, 
			 maxpe, upperTime, lowerTime, upperPe, lowerPe, 
			 upperTimeHisto, lowerTimeHisto, upperPeHisto, 
			 lowerPeHisto, eventposition, eventdirection, 
			 residualTimeBuffer, ringcolor, pixel, 
			 myTheta2D, costheta, phi, coneAlpha, 
			 numberofrings, colorMethod, tau, hitX, hitY, hitZ, 
			 Pe, trueTime );

  // Draw the event in 3D
  if(drawMode==1){
    if (displayEvent)
      thisdisplay.draw3DEventPoints();
    thisdisplay.draw3DCherenkovRing();
    if(displayDetector)
      thisdisplay.draw3DDetector();
  }
  else if(drawMode==0){
    if(displayEvent)
      thisdisplay.draw2DEventPoints();
    if(displayDetector)
      thisdisplay.draw2DBackground();
    thisdisplay.draw2DCherenkovRing();
    draw2DEvent();
  }
  else if(drawMode == 2){
    if (displayEvent)
      thisdisplay.drawProjection();
  }else if(drawMode == 3){
	  if (displayEvent)
		  thisdisplay.drawProjectionPhiTheta();
  }
	
  
  glutSwapBuffers();
  
}

void handleKeypress(unsigned char key, int x, int y) {
  switch (key) {
  case 27: //Escape key                                           
    glutDestroyWindow(glutGetWindow());
  }
  // Testing **
  // if(key=='p'){
  //   glutstuff myglut;
  //   glutInitWindowSize(400, 400);
  //   glutInitWindowPosition(window_x, window_y);    
  //   newWindow = glutCreateWindow("Hello World");
  //   myglut.initRendering();
  //   myTestGLUI();
  //   glutDisplayFunc(myTestDrawing);
  //   glutKeyboardFunc(handleKeypress);
  //   glutReshapeFunc(handleResize);

  // }
  // if(key=='o'){
  //   glutDestroyWindow(glutGetWindow());
  // }

}


void handleResize(int w, int h) {

  int x, y;
  GLUI_Master.get_viewport_area(&x, &y, &w, &h);

  glViewport(x, y, w, h);
  if ( w > h ){
    resize_x = static_cast<float>(h)/w;
    resize_y = 1;
  }
  else{
    resize_x = 1;
    resize_y = static_cast<float>(w)/h;
  }
  
}

void drawGL(int argc, char **argv){
  // This is where all of the drawing takes place

  glutstuff myglut;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(1200, 800);
  glutInitWindowPosition(window_x, window_y);
  main_window = glutCreateWindow(window_title);
  myglut.initRendering();
  setupGLUI();
  glutDisplayFunc(renderevent);

  //glutIdleFunc(idle);
  GLUI_Master.set_glutIdleFunc (idle);
  // GLUI_Master.set_glutKeyboardFunc(handleKeypress);
  // GLUI_Master.set_glutMouseFunc(mouseButton);
  // glutMotionFunc(mouseMove);
  glutKeyboardFunc(handleKeypress);
  glutReshapeFunc(handleResize);
  GLUI_Master.set_glutReshapeFunc(handleResize);
  glutMainLoop();
  
  return;
}


void setupGLUI(){
 
  // This function will draw all of the buttons, radio buttons,
  // check boxes, spinners, arcballs for manipulation of the
  // image. This function will be called multiple times in order
  // to redraw the panels when certain variables are changed
  // ie: min and max values for spinners when events change
  
  glui_window2 = GLUI_Master.create_glui_subwindow 
    (main_window, GLUI_SUBWINDOW_RIGHT);
  glui_window = GLUI_Master.create_glui_subwindow 
    (main_window, GLUI_SUBWINDOW_RIGHT);
  // Sync all live variables

  GLUI_Master.sync_live_all();

  // Add GLUI objects (Checkboxes)
  op_panel = glui_window->add_panel("Object Properties");
  event_spinner = glui_window->add_spinner_to_panel
    (op_panel, "Event Number", GLUI_SPINNER_INT, &EventNum);
  event_spinner->set_int_limits(0, totalEvents-1);
  event_spinner->set_speed(0.1);
  dimensionSelect = glui_window->add_radiogroup_to_panel
    (op_panel, &drawMode, NEWDIMENSION, glui_callback);
  glui_window->add_radiobutton_to_group( dimensionSelect, "2D display" );
  glui_window->add_radiobutton_to_group( dimensionSelect, "3D display" );
  glui_window->add_radiobutton_to_group( dimensionSelect, "Vertex Spherical Projection" );
  glui_window->add_radiobutton_to_group( dimensionSelect, "Vertex Polar Projection" );
  glui_window->add_separator_to_panel (op_panel);
  detectorVolume = glui_window->add_statictext_to_panel
    (op_panel, "helloWorld");
  glui_window->add_checkbox_to_panel 
    (op_panel, "Display Event", &displayEvent);
  glui_window->add_checkbox_to_panel 
    (op_panel, "Display Detector", &displayDetector);
  color_list = glui_window->add_listbox_to_panel 
    (op_panel, "Color Method", &colorMethod);
  color_list->add_item(0, "Pe");
  color_list->add_item(1, "Time");
  
  // Add GLUI Transformations
  // transformation_panel = glui_window->add_panel("Transformation");
  transformation_rollout = glui_window->add_rollout("Transformation");
  GLUI_Panel *trans_holder = glui_window->add_panel_to_panel
    (transformation_rollout, "", GLUI_PANEL_NONE);
  translation_xy = glui_window->add_translation_to_panel
    (trans_holder, "Translation XY", GLUI_TRANSLATION_XY, xypos);
  translation_xy->set_speed( 0.005 );
  glui_window->add_column_to_panel(trans_holder, false);
  glui_arcball = glui_window->add_rotation_to_panel
    (trans_holder, "Rotation", rotation_matrix);
  my_theta = glui_window2->add_spinner_to_panel
    (transformation_rollout, "Theta Rotation", GLUI_SPINNER_FLOAT, &myTheta2D);
  my_theta->set_float_limits(-4*PI,4*PI, GLUI_LIMIT_WRAP);
  glui_window->add_separator_to_panel(transformation_rollout);
  
  // Add GLUI Zoom function
  zoomFunc = glui_window->add_spinner_to_panel
    (transformation_rollout, "Scale", GLUI_SPINNER_FLOAT, &scale);
  zoomFunc->set_float_limits(0.0, 4.0);
  // Add GLUI Pixel Size
  pixel_spinner = glui_window->add_spinner_to_panel
    (transformation_rollout, "Pixel Size", GLUI_SPINNER_INT, &pixel);
  pixel_spinner->set_int_limits(0, 10);

  // Add GLUI Cut spinners
  cuts_rollout = glui_window->add_rollout("Cuts");
  pe_min_spinner = glui_window->add_spinner_to_panel
    (cuts_rollout, "Min Pe", GLUI_SPINNER_FLOAT, &lowerPe);
  pe_min_spinner->set_float_limits(0, maxpe);
  pe_max_spinner = glui_window->add_spinner_to_panel
    (cuts_rollout, "Max Pe", GLUI_SPINNER_FLOAT, &upperPe);
  pe_max_spinner->set_float_limits(0, maxpe);
  time_min_spinner = glui_window->add_spinner_to_panel
    (cuts_rollout, "Min Time", GLUI_SPINNER_FLOAT, &lowerTime);
  time_min_spinner->set_float_limits(0.0, maxtime);
  time_max_spinner = glui_window->add_spinner_to_panel
    (cuts_rollout, "Max Time", GLUI_SPINNER_FLOAT, &upperTime);
  time_max_spinner->set_float_limits(0.0, maxtime);

  // Histogram Modifiers
  histogram_rollout = glui_window->add_rollout("Histograms");
  pe_min_histo_spinner = glui_window->add_spinner_to_panel
    (histogram_rollout, "Min Pe", GLUI_SPINNER_FLOAT, &lowerPeHisto);
  pe_min_histo_spinner->set_float_limits(0, maxpe);
  pe_max_histo_spinner = glui_window->add_spinner_to_panel
    (histogram_rollout, "Max Pe", GLUI_SPINNER_FLOAT, &upperPeHisto);
  pe_max_histo_spinner->set_float_limits(0, maxpe);
  time_min_histo_spinner = glui_window->add_spinner_to_panel
    (histogram_rollout, "Min Time", GLUI_SPINNER_FLOAT, &lowerTimeHisto);
  time_min_histo_spinner->set_float_limits(0.0, maxtime);
  time_max_histo_spinner = glui_window->add_spinner_to_panel
    (histogram_rollout, "Max Time", GLUI_SPINNER_FLOAT, &upperTimeHisto);
  time_max_histo_spinner->set_float_limits(0.0, maxtime);
  bins_spinner = glui_window->add_spinner_to_panel
    (histogram_rollout, "Bins", GLUI_SPINNER_INT, &bins);
  bins_spinner->set_int_limits(1,100);
  glui_window->add_checkbox_to_panel 
    (histogram_rollout, "Pe Logscale", &isLogPe);

  // Cherenkov Rings
  cherenkov_rollout = glui_window2->add_rollout("Cherenkov Rings");
  ring_number_spinner = glui_window2->add_spinner_to_panel
    (cherenkov_rollout, "Total Rings", GLUI_SPINNER_INT, 
     &numberofrings, NEWRING, glui_callback);
  ring_number_spinner->set_int_limits(0, totalrings);
  ring_spinner = glui_window2->add_spinner_to_panel
    (cherenkov_rollout, "Ring", GLUI_SPINNER_INT, 
     &currentring, NEWRING, glui_callback);
  vec_position_X_spinner = glui_window->add_spinner_to_panel
    (cherenkov_rollout, "R Position", GLUI_SPINNER_FLOAT, 
     &eventposition_now[0]);
  vec_position_X_spinner->set_float_limits(-1,1);
  vec_position_Y_spinner = glui_window->add_spinner_to_panel
    (cherenkov_rollout, "Phi Position", GLUI_SPINNER_FLOAT, 
     &eventposition_now[1]);
  vec_position_Y_spinner->set_float_limits(-2*PI,2*PI);
  vec_position_Z_spinner = glui_window->add_spinner_to_panel
    (cherenkov_rollout, "Z Position", GLUI_SPINNER_FLOAT, 
     &eventposition_now[2]);
  vec_position_Z_spinner->set_float_limits(-1,1);
  vec_direction_costheta = glui_window->add_spinner_to_panel
    (cherenkov_rollout, "Cos Theta", GLUI_SPINNER_FLOAT, &costheta_now);
  vec_direction_costheta->set_float_limits(-1, 1, GLUI_LIMIT_WRAP);
  vec_direction_costheta->set_speed(1.5);
  vec_direction_phi = glui_window->add_spinner_to_panel
    (cherenkov_rollout, "Phi", GLUI_SPINNER_FLOAT, &phi_now);
  vec_direction_phi->set_float_limits(-2*PI, 2*PI, GLUI_LIMIT_WRAP);
  vec_direction_phi->set_speed(1.5);
  ring_color = glui_window->add_listbox_to_panel 
    (cherenkov_rollout, "Color", &ringcolor_now);
  ring_color->add_item(0, "Clear");
  ring_color->add_item(1, "Black");
  ring_color->add_item(2, "White");
  ring_color->add_item(3, "Red");
  ring_color->add_item(4, "Green");
  ring_color->add_item(5, "Blue");
  

  // Add GLUI Residual Time Spinner & vector modifiers
  res_time = glui_window2->add_panel("Timing Residual");
  res_time_spinner = glui_window2->add_spinner_to_panel
    (res_time, "Time Buffer", GLUI_SPINNER_FLOAT, &residualTimeBuffer);
  res_time_spinner->set_float_limits(0.0, maxtime);
  tau_spinner = glui_window2->add_spinner_to_panel
    (res_time, "Start Time", GLUI_SPINNER_FLOAT, &tau);
  tau_spinner->set_float_limits(0.0, maxtime);
  cherenkov_cone_opacity = glui_window2->add_spinner_to_panel
    (res_time, "Cone Opacity", GLUI_SPINNER_FLOAT, &coneAlpha);
  cherenkov_cone_opacity->set_float_limits(0,1);

  movie_panel = glui_window2->add_rollout("Movie Mode");
  moviespeed_spinner = glui_window2->add_spinner_to_panel
    (movie_panel, "Speed", GLUI_SPINNER_FLOAT, &moviespeed);
  moviespeed_spinner->set_float_limits(0,10);
  GLUI_Panel *movie_mode_panel = glui_window2->add_panel_to_panel
    (movie_panel, "", GLUI_PANEL_NONE);
  movieone = glui_window2->add_button_to_panel
    (movie_mode_panel,"MOVIE 1", MOVIEONE, glui_callback);
  glui_window2->add_column_to_panel(movie_mode_panel, false);
  movietwo = glui_window2->add_button_to_panel
    (movie_mode_panel,"MOVIE 2", MOVIETWO, glui_callback);
  GLUI_Panel *movie_controls_panel = glui_window2->add_panel_to_panel
    (movie_panel, "", GLUI_PANEL_NONE);
  stopmovie = glui_window2->add_button_to_panel
    (movie_controls_panel, "PAUSE", MOVIE_PAUSE, glui_callback);
  glui_window2->add_column_to_panel(movie_controls_panel, false);
  backmovie = glui_window2->add_button_to_panel
    (movie_controls_panel, "REWIND", MOVIE_BACK, glui_callback);
  stopmovie->set_w(10);
  movieone->set_w(10);
  movietwo->set_w(10);
  backmovie->set_w(10);
  
  output_panel = glui_window2->add_panel("Output");
  clarity = glui_window2->add_edittext_to_panel
    (output_panel, "Clarity", GLUI_EDITTEXT_INT, &clarityvar);
  ringcount = glui_window2->add_edittext_to_panel
    (output_panel, "Ring Count", GLUI_EDITTEXT_INT, &ringcountvar);

  glui_window2->add_button("Full Screen", FULLSCREEN, glui_callback);
  glui_window2->add_button("True Ring", TRUERING, glui_callback);
  glui_window2->add_button("Fitted Ring", FITRING, glui_callback);
  glui_window2->add_button("Reset", RESET, glui_callback);
  glui_window2->add_button("Print Out", PRINTOUT, glui_callback);


  // Let the GLUI window know where the glut window is
  glui_window->set_main_gfx_window( main_window );
  glui_window2->set_main_gfx_window( main_window );


  glui_callback(NEWDIMENSION);
  glui_callback(RESET);

  return;
}

void updateGLUI(){

  pe_min_spinner->set_float_limits(0, maxpe);
  pe_max_spinner->set_float_limits(0, maxpe);
  time_min_spinner->set_float_limits(0.0, maxtime);
  time_max_spinner->set_float_limits(0.0, maxtime);
  time_min_spinner->set_speed(3000 / (maxtime));
  time_max_spinner->set_speed(3000 / (maxtime));
  pe_min_histo_spinner->set_float_limits(0, maxpe);
  pe_max_histo_spinner->set_float_limits(0, maxpe);
  time_min_histo_spinner->set_float_limits(0.0, maxtime);
  time_max_histo_spinner->set_float_limits(0.0, maxtime);
  time_min_histo_spinner->set_speed(3000 / (maxtime));
  time_max_histo_spinner->set_speed(3000 / (maxtime));

  res_time_spinner->set_float_limits(0.0, maxtime);
  tau_spinner->set_float_limits(0.0, maxtime);

  event_spinner->set_int_limits(0, totalEvents-1);

  lowerPe = 0;
  lowerPeHisto = 0;
  upperPe = maxpe;
  upperPeHisto = maxpe;
  lowerTime = 0;
  lowerTimeHisto = mintime;
  upperTime = maxtime;
  lowerTime = mintime;
  upperTimeHisto = maxtime;
  residualTimeBuffer=0;
  tau=0;

  GLUI_Master.sync_live_all();

  return;
}

void idle()
{
  glutSetWindow (main_window);
  glutPostRedisplay ();
  playmovie();
  mySleep(50);
}
 
float findResidualTime(float trueTime, float hitX, 
		       float hitY, float hitZ ){

  float pmt[3] = {hitX, hitY, hitZ};
  float tau = -trueTime;
  float realPosition[3] = { eventposition[0] * reduction,
			    eventposition[1] * reduction,
			    eventposition[2] * reduction };
  residualTime rt( eventdirection, realPosition, pmt, tau );
  float resTime = -rt.findResidualTime() / speed_c;
  
  return resTime;
}


void draw2DEvent(){

  
  float peposValues[] = {-2.5, 1.5, 0};
  vector<float> peposition(peposValues, 
			   peposValues + sizeof(peposValues) / sizeof(float));
  float timeposValues[] = {1.2, 1.5, 0};
  vector<float> timeposition(timeposValues, 
			     timeposValues + sizeof(timeposValues) 
			     / sizeof(float));
  float size = 1.5;
  vector<float> logPe;
  
  oglHistogram pehisto(Pe, bins, peposition, size, 
  		       lowerPeHisto, upperPeHisto, 
		       lowerPe, upperPe, isLogPe );
  pehisto.drawAxes();
  pehisto.drawBins();
  pehisto.drawCutLines();
  pehisto.drawLabels("Pe");

  oglHistogram timehisto(trueTime, bins, timeposition, size, 
			 lowerTimeHisto, upperTimeHisto, lowerTime, upperTime);
  timehisto.drawAxes();
  timehisto.drawBins();
  timehisto.drawCutLines();
  timehisto.drawLabels("Time");


}


void setDirectionFromAngles(){

  // Given the costheta and phi coordinates from
  // the user, properly set the eventdirection[3]
  // coordinates used by the rest of the program
  // which is in cartesian
  for(int i=0; i < numberofrings; i++){
    eventdirection[i*3 + 0] = cos(phi[i])*sqrt(1-costheta[i]*costheta[i]);
    eventdirection[i*3 + 1] = sin(phi[i])*sqrt(1-costheta[i]*costheta[i]);
    eventdirection[i*3 + 2] = costheta[i];
  }

}

void getRootInfo(int argc, char **argv){

    // Get vectors from root
    TString filename = argv[1];
    handleRoot root(filename, argc, argv, EventNum);
    // Clear all the vectors being passed to root
    pmtID.clear();
    pmtX.clear();
    pmtY.clear();
    pmtZ.clear();
    hitX.clear();
    hitY.clear();
    hitZ.clear();
    Pe.clear();
    trueTime.clear();
    root.returnVectors(pmtID, pmtX, pmtY, pmtZ,
		       hitX, hitY, hitZ, Pe, trueTime, 
		       CylinderHeight, CylinderRadius, totalEvents,
		       vertexTrue, vertexReco, dirTrue, dirReco);
    if(lastEvent == -1)
      printdetectorinfo();
    // Set Global Variables
    SetMaxMinPe();
    SetMaxMinTime();

    return;
}

void setArgumentsGlobal(int argc, char **argv){

  argc_globe = argc;
  argv_globe = argv;

}


void myTestDrawing(){

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  const char* theWords = "Hello World!";
  glRasterPos2i(0, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  for (int i = 0; i < (int)strlen(theWords); i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, theWords[i]);
  
  glutSwapBuffers();
  
}

// void myTestGLUI(){

//   GLUI *buttonWindow;
//   buttonWindow = GLUI_Master.create_glui_subwindow 
//     (newWindow, GLUI_SUBWINDOW_TOP);

//   GLUI_Button *close = buttonWindow->add_button("CLOSE", QUIT, glui_callback);
  
// }

void glui_callback(int arg){

  switch (arg)
    {
    case QUIT:
      glutDestroyWindow(newWindow);
      break;
    case FULLSCREEN:
      if(isFullScreen){
	glutReshapeWindow( 1200, 800 );
	isFullScreen = 0;
      }
      else{
	glutFullScreen();
	isFullScreen = 1;
      }
      break;
    case REDRAW:
      updateGLUI();
      break;
    case NEWDIMENSION:
      if(drawMode==1){
	my_theta->disable();
	glui_arcball->enable();
	translation_xy->disable();
      }
      else if(drawMode==0){
	my_theta->enable();
	glui_arcball->disable();
	translation_xy->enable();
      }
      else if(drawMode == 2){
	my_theta->disable();
	glui_arcball->enable();
	translation_xy->disable();
      }
      break;
    case NEWRING:
      ring_spinner->set_int_limits(1, numberofrings); 
      if(currentring != 0){
	costheta_now = costheta[currentring - 1];
	phi_now = phi[currentring - 1];
	eventposition_now[0] = sqrt(pow(eventposition[(currentring-1)*3 + 0],2)+
				    pow(eventposition[(currentring-1)*3 + 1],2));
	eventposition_now[1] = atan2(eventposition[(currentring-1)*3 + 1],
				     eventposition[(currentring-1)*3 + 0]);
	eventposition_now[2] = eventposition[(currentring-1)*3 + 2];
	ringcolor_now = ringcolor[currentring - 1];
      }
      else {
	costheta_now = 0;
	phi_now = 0;
	ringcolor_now=0;
	eventposition_now[0] = 0;
	eventposition_now[1] = 0;
	eventposition_now[2] = 0;
      }
      break;
    case RESET:
      InitializeVariables();
      break;
    case PRINTOUT:
      printtoterminal();
      break;
    case MOVIEONE:
      movieOn = true;
      movietype = 0;
      stopmovie->set_name("PAUSE");
      isforward = 1;
      upperTime = lowerTimeHisto;
      lowerTime = lowerTimeHisto;
      break;
    case MOVIETWO:
      movieOn = true;
      movietype = 1;
      stopmovie->set_name("PAUSE");
      isforward = 1;
      upperTime = lowerTimeHisto + 4;
      lowerTime = lowerTimeHisto;
      break;
    case MOVIE_PAUSE:
      isforward = 1;
      if(movieOn){
	stopmovie->set_name("PLAY");
	movieOn = false;
      }
      else{
	stopmovie->set_name("PAUSE");
	movieOn = true;
      }
      break;
    case MOVIE_BACK:
      isforward = -1;
      stopmovie->set_name("PAUSE");
      movieOn = true;
      break;
    case TRUERING:
      if(vertexTrue[0] != 9 &&
	 (dirTrue[1]*dirTrue[1]+dirTrue[2]*dirTrue[2]+
	  dirTrue[0]*dirTrue[0] >= 0.1)){
	eventposition_now[0] = sqrt(pow(vertexTrue[0]/reduction,2)+
				    pow(vertexTrue[1]/reduction,2));
	eventposition_now[1] = atan2(vertexTrue[1]/reduction,
				     vertexTrue[0]/reduction);
	eventposition_now[2] = vertexTrue[2]/reduction;
  
	costheta_now = dirTrue[2]/(sqrt(dirTrue[2]*dirTrue[2]
					+dirTrue[1]*dirTrue[1]
					+dirTrue[0]*dirTrue[0]));
	phi_now = atan2(dirTrue[1], dirTrue[0]);
      }
      break;
    case FITRING:
      if(vertexTrue[0] != 9 && 
	 (dirReco[1]*dirReco[1]+dirReco[2]*dirReco[2]+
	  dirReco[0]*dirReco[0] >= 0.1)){
	eventposition_now[0] = sqrt(pow(vertexReco[0]/reduction,2)+
				    pow(vertexReco[1]/reduction,2));
	eventposition_now[1] = atan2(vertexReco[1]/reduction,
				     vertexReco[0]/reduction);
	eventposition_now[2] = vertexReco[2]/reduction;
  
	costheta_now = dirReco[2]/(sqrt(dirReco[2]*dirReco[2]
					+dirReco[1]*dirReco[1]
					+dirReco[0]*dirReco[0]));
	phi_now = atan2(dirReco[1], dirReco[0]);
      }
      break;
    }
  GLUI_Master.sync_live_all();

  return;
}

void InitializeVariables(){

  scale = 0.8;
  lowerPe = 0;
  lowerPeHisto = 0;
  upperPe = maxpe;
  upperPeHisto = maxpe;
  lowerTime = 0;
  lowerTimeHisto = mintime;
  upperTime = maxtime;
  upperTimeHisto = maxtime;
  pixel = 0;
  coneAlpha = 0.0;  
  tau = 0;
  xypos[0] = 0;
  xypos[1] = 0;
  zpos = 0;
  bins = 20;
  for( int i = 0; i < 16; i++ ){
    if( i%5 == 0 )
      rotation_matrix[i] = 1.0;
    else
      rotation_matrix[i] = 0.0;
  }  
  numberofrings = 1;
  currentring = 1;
  for( int i = 0; i < totalrings; i++ ){
    costheta[i] = 0;
    phi[i] = 0;
  }
  eventposition_now[0] = 0;
  eventposition_now[1] = 0;
  eventposition_now[2] = 0;
  costheta_now = 0;
  phi_now = 0;
  colorMethod = 0;
  ringcolor_now = 1;
  residualTimeBuffer = 0;
  myTheta2D = PI;

}

void printdetectorinfo(){
  
  cout << "**********************************" << endl;
  cout << "Detector height: " << CylinderHeight << " cm" << endl;
  cout << "Detector radius: " << CylinderRadius << " cm" << endl;
  cout << "Detector volume: "
       << (PI*(CylinderRadius)*
	   (CylinderRadius)*
	   (CylinderHeight))/(1000*1000000)
       << " kton" << endl;
  cout << "Number of pmt's: " << pmtID.size() << endl;
  cout << "**********************************" << endl;
  // Print headings for variables spit out by printtoterminal()
  cout << setw(12) << left << "Event"
       << setw(12) << "TotalPe"
       << setw(12) << "Mean Time"
       << setw(12) << "RMS Time"
       << setw(12) << "Rings"
       << setw(12) << "Clarity"
       // << setw(12) << "Vertex (X,Y,Z)"
       // << setw(12) << "Direction (X,Y,Z)"
       << endl;

}

void printtoterminal(){

  // This function will print relevant information to standard out
  // This can be piped to a text file for data analysis
  cout << setw(12) << left << EventNum
       << setw(12) << totalPe()
       << setw(12) << meanTime()
       << setw(12) << rmsTime(meanTime())
       << setw(12) << ringcountvar
       << setw(12) << clarityvar
       // << setw(12) << eventposition[0]*reduction << "," 
       // << eventposition[1]*reduction 
       // << ","<< eventposition[2]*reduction
       // << setw(12) << eventdirection[0] << "," << eventdirection[1] 
       // << ","<< eventdirection[2]
       << endl;

}

float totalPe(){

  float total = 0;
  for(unsigned int i = 0; i < Pe.size(); i++)
    total += Pe[i];

  return total;

}

float meanTime(){

  float total = 0;
  float size = 0;
  for(unsigned int i = 0; i < trueTime.size(); i++){
    if( trueTime[i] < upperTime && trueTime[i] > lowerTime ){
      total += trueTime[i];
      size++;
    }
  }

  return total / size;

}

float rmsTime(float mean){

  unsigned int number = trueTime.size();
  float size = 0;
  float sumofsquares = 0;
  for( unsigned int i = 0; i < number; i++ )
    if( trueTime[i] < upperTime && trueTime[i] > lowerTime ){
    sumofsquares += trueTime[i] * trueTime[i];
    size++;
    }
  return sqrt( sumofsquares / size );

}

void playmovie(){

  if(movieOn){
    upperTime += moviespeed * isforward;
    lowerTime += moviespeed * isforward * movietype;
    GLUI_Master.sync_live_all();
  }

  if( upperTime >= upperTimeHisto || upperTime <= lowerTimeHisto){
    movieOn = false;
    stopmovie->set_name("PLAY");
  }

}
