#include <GL/gl.h>
#include <GL/glu.h>
#include	"xl/GLGetMatrix.h"

using namespace	Geometry;


#define	MAX_QUADPARTS		2048

int	initGL();

void	drawQuad(	GLuint tx_id,
			float z,
			float x0,	float y0,
			float x1,	float y1);

void	drawQuad(	GLuint tx_id,
			float z,
			float x0,	float y0,
			float x1,	float y1,
			float x_repeat,
			float y_repeat);

void	render_simulation();
void	step_simulation();
bool	loop();

extern	double	diff;	//	in secs
