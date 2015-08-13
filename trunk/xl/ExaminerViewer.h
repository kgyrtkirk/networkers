/***********************************************************************
ExaminerViewer - Subclass of the Fltk OpenGL window class implementing
an examiner viewer.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef EXAMINERVIEWER_INCLUDED
#define EXAMINERVIEWER_INCLUDED

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

#include "HVector.h"
#include "Transformation.h"
#include "Light.h"

class ExaminerViewer:public Fl_Gl_Window
	{
	/* Embedded classes: */
	private:
	enum DragMode // Enumerated type for dragging modes
		{
		DRAG_NONE,DRAG_ROTATING,DRAG_PANNING,DRAG_ZOOMING
		};
	
	/* Elements: */
	
	/* Information about the current model: */
	double modelMin[3],modelMax[3]; // Extents of bounding box around the current model
	double modelCenter[3]; // Center position of model
	double modelRadius; // Radius of a sphere enclosing the model
	
	/* OpenGL state: */
	bool contextInitialized; // Flag if the window's OpenGL context has already been initialized
	bool viewSpecChanged; // Flag if the view specification has changed since the last call to draw()
	double frontPlaneDistance,backPlaneDistance; // Current distances of front- and backplane from camera
	bool headlightEnabled; // Flag if the head light source is enabled
	Light headlight; // Specification of the head light source
	
	/* Current view specification: */
	double windowAspect; // Aspect ratio of display window
	double windowRadius; // Approximate radius of a circle fitting the display window
	Geometry::Transformation view; // Accumulated rotation/pan matrix
	double dolly; // Distance from origin of model coordinate system to camera position
	double zoom; // Width of camera's field of view
	
	/* Interaction state: */
	int rotateButtonMask,panButtonMask,zoomButtonMask; // Mouse button masks for viewpoint manipulation
	int fullModifierMask; // Bit mask of all considered modifier states
	int rotateModifierMask,panModifierMask,zoomModifierMask; // Modifier key masks for viewpoint manipulation
	int buttonMask; // Bit mask of currently pressed mouse buttons
	DragMode dragMode; // Current dragging mode
	int dragX,dragY; // State to keep track of mouse movements inside the window
	bool isSpinning; // Flag if the display is spinning
	Geometry::Transformation spinTransformation; // Last transformation increment before mouse button was released
	
	/* Private helper methods: */
	double calcModelBoxZ(int modelBoxCornerIndex) const; // Calculates z coordinate of given corner of model bounding box
	Geometry::Transformation calcRotation(double dX,double dY) const; // Calculates incremental rotation transformation based on mouse displacement vector
	Geometry::Transformation calcPan(double dX,double dY) const; // Calculates incremental pan transformation based on mouse displacement vector
	void changeDragMode(int x,int y,int newButtonMask,int newModifierState); // Sets the drag mode according to new button and modifier states
	static void spinCB(void* cbData); // Callback to keep the model spinning
	
	/* Methods inherited from Fl_Gl_Window: */
	protected:
	virtual void draw(void);
	public:
	virtual int handle(int eventType);
	
	/* Event handling methods: */
	protected:
	virtual int push(int x,int y,int button,int modifierState);
	virtual int release(int x,int y,int button,int modifierState);
	virtual int drag(int x,int y);
	virtual int mouseWheel(int x,int y);
	virtual int enter(int x,int y);
	virtual int move(int x,int y);
	virtual int leave(void);
	virtual int focus(void);
	virtual int keyboard(int key,const char* text,int textLength,int modifierState);
	virtual int unfocus(void);
	
	/* Constructors and destructors: */
	public:
	ExaminerViewer(int w,int h,const char* label =0); // Creates examiner viewer of given size
	ExaminerViewer(int x,int y,int w,int h,const char* label =0); // Creates examiner viewer of given size at given position
	virtual ~ExaminerViewer(void); // Destroys examiner viewer
	
	/* New methods: */
	void setModelBox(const double newMin[3],const double newMax[3]);
	void resetView(void);
	void setHeadlight(bool newHeadlightEnabled); // Enables or disables the head light source
	virtual void initScene(void);
	virtual void renderScene(void);
	};

#endif
