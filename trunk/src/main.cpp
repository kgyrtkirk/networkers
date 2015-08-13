#include	<stdio.h>
#include	"portability.h"
#include	"opengl.h"
#include	"radar.h"
#include	"set.h"
#include <stdarg.h>


int	log_level=4;


#define	COM_POINT_CNT	64


FILE*	debug_log=0;
void	write_log(const char*format,...)
{
#ifdef	MSG_LOG
	va_list ap;

	if(!debug_log)
	{
		debug_log=fopen("/tmp/nw_debug","w+");
		assert(debug_log);
	}
	va_start(ap,format);
	vfprintf(debug_log,	format,	ap);
	fprintf(debug_log,"\n");
	va_end(ap);
#endif
}

void	step_simulation()
{
	radar.step();
}


void	render_simulation()
{
	radar.render();
}

void	geosteiner_io();

int	main()
{
	int	i;
	
// 	xtest();
// 	set_test();
// 	return	0;
	
// 	printf("%d\n",offsetof(Q,tree_x));
// 	printf("%d\n",offsetof(xDetectable,tree_x));
// 	return	0;
	
	//	initialize drawing/load textures
	
// 	geosteiner_io();
// 	return	0;

	initGL();
	
	//	create networkers
	for(i=0;i<COM_POINT_CNT;i++)
		radar.createComPoint();
	
// 	for(i=0;i<COM_POINT_CNT;i++)
// 		radar.createComPoint();
// 	radar.createRobot(0,55);
//  	radar.createRobot(0,67);
// 	radar.createRobot(0,73);
	radar.createRobot(0,311);
//  	radar.createRobot(0,237);
 	
 	
//	radar.createRobot(0,33);
	
	//	3116

	while(loop());
	
#ifdef	MSG_LOG
	fprintf(debug_log,"leaving");
	fclose(debug_log);
#endif
	
	//	start simulation
// 	while()
// 	{
// 		
// 		//	handle events
// 	}

}
