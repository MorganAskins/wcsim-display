## INSTALL ##

In order to compile and run the 3D event display the following must
be installed first. ROOT, WCSim, OpenGL (glut.h, glui.h).
Note for Mac users: the GLUI Framework for Mac OS X installs into /Library/Frameworks by default. You should copy GLUI.framework from there into /System/Library/Frameworks for a successful installation.

1. create an environmental variable WCSIM pointing to your WCSim directory.
2. use "make" to compile the application
3. run ./runme file.root to display the events within file.root


## USAGE ##

This 3D event display allows for both viewing of the detector in
3 dimensions as well as a two dimensional projection. Through the
manipulation of live variables in the toolbars the display will
change in real time to display the changing features. Currently
the display is designed to accept events stored in root files
produced by WCSim.

*All controls in the toolbars can be sped up by holding the ctrl
key and slowed by holding the shift key

For each event information can be print to the terminal through
std::cout by pressing the "Print Out" button. This allows for
only the information for the current event to be printed. For
analysis the information can then easily be sent to a text file.

Movie Mode: Movie mode plays the event within the bounds set by
the histogram time limits. The movie plays by continuously increasing
the timing cut showing the event in a frame-by-frame sequence. The
"MOVIE 1" button auto sets the cuts for the movie to play as well
as starts the movie. This needs to be done before play and rewind
can work.
MOVIE 1: Increases only the max time cut therefore you see all events
as they arrive as well as all past events.
MOVIE 2: Increases both the max and min time cut with a gap between them
(currently defined as 4ns by the program) so that you see only current events
in the time scale.

*ex: ./runme event.root > data.dat


## KNOWN BUGS ##

1. There is currently a bug with the mac version of glui that causes
the fullscreen option on mac to not display the event in the correct
proportions.