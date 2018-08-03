#ifndef _GLUISTUFF
#define _GLUISTUFF

class gluistuff {
 private:
  // Glui windows to be used
  GLUI *glui_window;
  GLUI *glui_window2;
  // All GLUI controls are declared here
  GLUI_Panel *op_panel, *transformation_panel, *cuts_panel, 
    *res_time, *panel_2D, *cherenkov_panel, *histogram_panel;
  GLUI_Rollout *transformation_rollout, *cuts_rollout, 
    *cherenkov_rollout, *histogram_rollout;
  GLUI_RadioGroup *dimensionSelect;
  GLUI_Translation *translation_xy, *translation_z;
  GLUI_Spinner *event_spinner, *zoomFunc, *pixel_spinner, *pe_min_spinner,
    *pe_max_spinner, *time_min_spinner, *time_max_spinner, *res_time_spinner,
    *cherenkov_cone_opacity, *vec_position_X_spinner, *vec_position_Y_spinner,
    *vec_position_Z_spinner, *vec_direction_costheta, *vec_direction_phi, 
    *my_theta, *ring_spinner, *ring_number_spinner, *pe_min_histo_spinner, 
    *pe_max_histo_spinner, *time_min_histo_spinner, *time_max_histo_spinner, 
    *tau_spinner;
  GLUI_Rotation *glui_arcball;
  GLUI_StaticText *detectorVolume;
  GLUI_Listbox *color_list;
  // The id of the main window
  GLuint main_window;

  // enum for glui_callback
  enum{
    QUIT = 0,
    FULLSCREEN,
    REDRAW,
    NEWDIMENSION,
    NEWRING
  };

 public:
  gluistuff();
  void setupGlui();
  void glui_callback(int arg);
  


};

#endif
