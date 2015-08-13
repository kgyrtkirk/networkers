#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include	"textures.h"
#include	"opengl.h"
#include	"util/log.h"
#include	"glprintf.h"
#include	"radar.h"
#include	"nw_info.h"
#include	"nw_stat.h"

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 1024
#define SCREEN_BPP     16

/* Set up some booleans */
#define TRUE  1
#define FALSE 0

#define	N_MENU	4
const	char*	menu_str[N_MENU]=	{	"pause",
									"run",
									"graph",
									"?"	};

/* Number of textures to load */
#define NUM_TEXTURES 6

/* This is our SDL surface */
SDL_Surface *surface;

int	window_width=1;
int	window_height=1;

/* Whether or not lighting is on */
int light = FALSE;

GLfloat	xrot;
GLfloat	yrot;
GLfloat	xspeed;
GLfloat	yspeed;
GLfloat	z = -5.0f;

GLfloat LightAmbient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };

GLUquadricObj *quadratic;     /* Storage For Our Quadratic Objects */
GLuint object = 3;            /* Which Object To Draw              */

GLuint filter = 0;            /* Which Filter To Use               */
GLuint texture[NUM_TEXTURES]; /* Storage for 3 textures            */

int	videoFlags;


#define	CAM_SLOW					0.2
#define	CAM_SPRING_NEAR_DIST		200.0f
#define	CAM_SPRING_FAR_DIST			2500.0f
#define	CAM_SPRING_NEAR_FORCE		5.83
#define	CAM_SPRING_FAR_FORCE		5.83

#define	CAM_SPRING_SIDE_DIST		1000.0f

#define V_X_V(A,B,M,C)  A[0]=B[0] M C[0];       A[1]=B[1] M C[1];       A[2]=B[2] M C[2];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];

int	mode=0;

void	glCamProject( HVector*_pos,HVector*_dir,double x,double y)
{
	int		viewport[4];
	double	u[3];
	double	v[3];
	
	y=window_height-y;
	
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluUnProject(	x,y,	0.0f,
				modelMatrix,
				projMatrix,	
				viewport,
				&u[0],	&u[1],	&u[2]      );
				
	gluUnProject(	x,y,	1.0f,
				modelMatrix,
				projMatrix,	
				viewport,
				&v[0],	&v[1],	&v[2]      );


	HVector	a(u[0],u[1],u[2],1.0f);
	HVector	b(v[0],v[1],v[2],1.0f);
	
	HVector	p=a+(b-a)*(a[2]/(a[2]-b[2]));
	
	if(_pos)
		*_pos=p;
	else
		LOG(MSG,	"click@	%f	%f	%f",p[0],p[1],p[2]);
	if(_dir)
		*_dir=(a-b).normalize();
}

HVector	cursor_pos(0,0,0,0);
float	spring_force(float x,float power)
{
	if(x>0.0)
		return	x*power;
	return	0.0f;
}

double	diff=0.0;	//	in secs
class	Camera
{
public:
	HVector	pos;
	HVector	dir;
	float	power;
	public:
	Camera()	:	pos(0.0f,0.0f,25000.0f,0.0f),dir(0.0f,0.0f,1.0f,0.0f),power(0.0f)	{}
	void	applyPower()
	{
		double	diffx=fmax(diff,1.0f/100);
// 		if(	pos[2] < CAM_SPRING_NEAR_DIST)
// 			pos[2]+=( CAM_SPRING_NEAR_DIST-pos[2])*CAM_SPRING_NEAR_FORCE*diff;
// 		if(	pos[2] > CAM_SPRING_FAR_DIST)
// 			pos[2]-=(-CAM_SPRING_FAR_DIST+pos[2])*CAM_SPRING_FAR_FORCE*diff;

		pos[2]+=spring_force( CAM_SPRING_NEAR_DIST-pos[2],	CAM_SPRING_NEAR_FORCE)*diff;
		pos[2]-=spring_force(-CAM_SPRING_FAR_DIST +pos[2],	CAM_SPRING_FAR_FORCE)*diff;
		
		power*=pow(0.97,spring_force( CAM_SPRING_NEAR_DIST-pos[2],	CAM_SPRING_NEAR_FORCE)*diff+
		spring_force(-CAM_SPRING_FAR_DIST +pos[2],	CAM_SPRING_FAR_FORCE)*diff);
		
// 		if(power<CAM_SPRING_NEAR_DIST)
// 			power*=pow(0.01,diff);
// 		power*=
		
		
// 		if(pos[2] < CAM_SPRING_FAR_DIST)
		{
			HVector	v(0,0,0,0);
			glCamProject(&v,0,	0,0);

// 			LOG(MSG,"po:%f	diff:	%f	%f",v[0],v[1]);
			pos[0]+=spring_force(	-CAM_SPRING_SIDE_DIST-v[0],	CAM_SPRING_FAR_FORCE)*diffx;
			pos[1]-=spring_force(	-CAM_SPRING_SIDE_DIST+v[1],	CAM_SPRING_FAR_FORCE)*diffx;
				
			glCamProject(&v,0,	window_width,window_height);
			
// 			LOG(MSG,"po:%f	diff:	%f	%f",v[0],v[1]);
			pos[0]-=spring_force(	-CAM_SPRING_SIDE_DIST+v[0],	CAM_SPRING_FAR_FORCE)*diffx;
			pos[1]+=spring_force(	-CAM_SPRING_SIDE_DIST-v[1],	CAM_SPRING_FAR_FORCE)*diffx;


		}
// 		{
// 			float	mx=pos*HVector(;
// 			float	my=fabsf(pos[1])+pos[2]/2;
// 			
// 			pos[0]+=spring_force(+CAM_SPRING_FAR_DIST +mx,	CAM_SPRING_FAR_FORCE)*diff;
// 			pos[0]-=spring_force(-CAM_SPRING_FAR_DIST +mx,	CAM_SPRING_FAR_FORCE)*diff;
// 		}
		
// 			LOG(MSG,"po:%f	diff:	%f	%f",mx,my,pos[2]);
// 		pos[1]+=spring_force(+CAM_SPRING_FAR_DIST -my,	CAM_SPRING_FAR_FORCE)*diff;
// 		pos[1]-=spring_force(-CAM_SPRING_FAR_DIST +my,	CAM_SPRING_FAR_FORCE)*diff;


		
			
// 		LOG(MSG,"po:%f	diff:	%f	%f",power,diff,pos[2]);

		pos+=dir*power*diff*1000.0;
		power*=pow(CAM_SLOW,diff);
// 		LOG(MSG,"po:%f	diff:	%f",power,diff);
		
	}
	void	look()
	{
		gluLookAt(	pos[0],	pos[1],	pos[2],
					pos[0],	pos[1],	0.0f,
					0.0f,	1.0f,	0.0f	);
		
	}
	void	perturbate(float _power,HVector _dir)
	{
// 		dir=(dir*power+_dir*_power).normalize();
// 		power=(dir*power+_dir*_power).mag();
		if(	(((power > 0.0f) && (_power <0.0f)) ||
			((power < 0.0f) && (_power >0.0f)))
			 && fabsf(power) > fabsf(_power) )
		{
			dir=_dir;
			power=_power;
		}
		else
		{
			dir=(dir+_dir)/2;
			power+=_power;
		}
// 		dir=_dir;
// // 		dir=dir
// 		power+=_power;
	}
} cam;

/* function to release/destroy our resources and restoring the old desktop */
void Quit( int returnCode )
{

    /* Clean up our quadratic */
    gluDeleteQuadric( quadratic );

    /* Clean up our textures */
	glDeleteTextures( NUM_TEXTURES,	&texture[0] );
	glDeleteTextures( TEXTURES_CNT,	&texture_cache[0] );

    /* clean up the window */
    SDL_Quit( );

    /* and exit appropriately */
//     freeTextures();
    exit( returnCode );
}

/* function to load in bitmap as a GL texture */
int	loadTX()
{
	return	-1;
}
int LoadGLTextures( )
{
	/* Status indicator */
	int Status = FALSE;

	int loop;

    /* Create storage space for the texture */
    SDL_Surface *TextureImage[2];

    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ( ( TextureImage[0] = SDL_LoadBMP( "data/bg.bmp" ) ) &&
         ( TextureImage[1] = SDL_LoadBMP( "data/reflect.bmp" ) ) )
    {

        /* Set the status to true */
        Status = TRUE;

        /* Create The Texture */
        glGenTextures( NUM_TEXTURES, &texture[0] );
        for( loop = 0; loop <= 1; loop++ )
        {
            /* Create Nearest Filtered Texture */
            glBindTexture( GL_TEXTURE_2D, texture[loop] ); /* Gen Tex 0 And 1 */
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
            glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[loop]->w, TextureImage[loop]->h,
                0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[loop]->pixels );

            /* Create Linear Filtered Texture */
            glBindTexture( GL_TEXTURE_2D, texture[loop+2] ); /* Gen Tex 2 And 3 */
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[loop]->w, TextureImage[loop]->h,
                0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[loop]->pixels );

            /* Create MipMapped Texture */
            glBindTexture( GL_TEXTURE_2D, texture[loop+4] ); /* Gen Tex 4 and 5 */
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
            gluBuild2DMipmaps( GL_TEXTURE_2D, 3, TextureImage[loop]->w, TextureImage[loop]->h,
                GL_BGR, GL_UNSIGNED_BYTE, TextureImage[loop]->pixels );
        }
    }
    /* Free up any memory we may have used */
    for( loop = 0; loop <= 1; loop++ )
    {
        if ( TextureImage[loop] )
            SDL_FreeSurface( TextureImage[loop] );
    }

    return Status;
}
GLfloat ratio;

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{
	if ( height == 0 )
		height = 1;
	ratio = ( GLfloat )width / ( GLfloat )height;
	glViewport( 0, 0, ( GLint )width, ( GLint )height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( 45.0f, ratio, 0.1f, 10000.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	window_width=width;
	window_height=height;
	
	return( TRUE );
}

int done=0;

void handleKeyPress( SDL_keysym *keysym , HVector dir)
{
	switch ( keysym->sym )
	{
		case SDLK_ESCAPE:	done=true;						break;
		case SDLK_f:	filter = ( filter + 1 ) % 3;				break;
		case SDLK_l:	light = !light;
		if ( !light )
			glDisable( GL_LIGHTING );
		else
			glEnable( GL_LIGHTING );
		break;
		case SDLK_SPACE:	object = ( object + 1 ) % 4;			break;
		case SDLK_PAGEUP:	z -= 0.02f;						break;
		case SDLK_PAGEDOWN:	z += 0.02f;						break;
		case SDLK_UP:		xspeed -= 0.01f;					break;
		case SDLK_DOWN:	xspeed += 0.01f;					break;
		case SDLK_RIGHT:	yspeed += 0.01f;					break;
		case SDLK_LEFT:	yspeed -= 0.01f;					break;
		case SDLK_F1:		SDL_WM_ToggleFullScreen( surface );	break;
		case	SDLK_a:		cam.perturbate(-0.5f, dir);	break;
		case	SDLK_z:		cam.perturbate( 0.5f, dir);	break;
		default:
		break;
	}
}

/* general OpenGL initialization function */
int initOpenGL( void )
{

    /* Load in the texture */
// 	if ( !LoadGLTextures( ) )
// 		return FALSE;
	loadTextures();

    /* Enable Texture Mapping ( NEW ) */
	glEnable( GL_TEXTURE_2D );

    /* Enable smooth shading */
	glShadeModel( GL_SMOOTH );

    /* Set the background black */
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
	glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

    /* The Type Of Depth Test To Do */
	glDepthFunc( GL_LESS );	//	GL_LEQUAL 

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    /* Setup The Ambient Light */
    glLightfv( GL_LIGHT1, GL_AMBIENT, LightAmbient );

    /* Setup The Diffuse Light */
    glLightfv( GL_LIGHT1, GL_DIFFUSE, LightDiffuse );

    /* Position The Light */
    glLightfv( GL_LIGHT1, GL_POSITION, LightPosition );

    /* Enable Light One */
    glEnable( GL_LIGHT1 );

    /* Set The Texture Generation Mode For S To Sphere Mapping */
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    /* Set The Texture Generation Mode For T To Sphere Mapping */
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    /* Create A Pointer To The Quadric Object */
    quadratic = gluNewQuadric( );
    /* Create Smooth Normals */
    gluQuadricNormals( quadratic, GLU_SMOOTH );
    /* Create Texture Coords */
    gluQuadricTexture( quadratic, GL_TRUE );

    return( TRUE );
}

void drawGLCube( void )
{
    /* Start Drawing Quads */
    glBegin( GL_QUADS );
      /* Front Face */
      glNormal3f( 0.0f, 0.0f, 0.5f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  1.0f, -1.0f,  1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.0f,  1.0f,  1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );

      /* Back Face */
      glNormal3f( 0.0f, 0.0f, -0.5f);
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );

      /* Top Face */
      glNormal3f( 0.0f, 0.5f, 0.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  1.0f,  1.0f,  1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );

      /* Bottom Face */
      glNormal3f( 0.0f, -0.5f, 0.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f,  1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );

      /* Right face */
      glNormal3f( 0.5f, 0.0f, 0.0f);
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 1.0f,  1.0f,  1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f,  1.0f );

      /* Left Face*/
      glNormal3f( -0.5f, 0.0f, 0.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
    glEnd();

    return;
}

// template<size_t N,size_t K>
// class	QDrawer
// {
// 		struct	QuadPart
// 		{
// 			struct	quadFragment{
// 			
// 				float	points[4][3];
// 				float	uv[4][3];
// 			} parts[K];
// 			uint32	nparts;
// 		}	quads[N];
// public:		
// 		QDrawer()
// 		{
// 			uint32	i;
// 			for(i=0;i<N;i++)
// 				quads[i].nparts=0;
// 		}
// 		void	addQuad()
// 	
// };
// QDrawer	<TEXTURES_CNT,MAX_QUADPARTS>	qdrawer;


void	drawQuad(	GLuint tx_id,
			float z,
			float x0,	float y0,
			float x1,	float y1,
			float x_repeat,
			float y_repeat)
{
// 	if(!dq_initialized)
// 	{
// 		TEXTURES_CNT
// 	}
	glBindTexture( GL_TEXTURE_2D, TEXTURE(tx_id) );

// 	glTranslatef( 0.0f, 0.0f, -29.0f );
	glBegin( GL_QUADS );
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f( 0.0f,		0.0f );		glVertex3f(  x0, y0,  z );
		glTexCoord2f( x_repeat,	0.0f	);		glVertex3f(  x1, y0,  z );
		glTexCoord2f( x_repeat,	y_repeat );	glVertex3f(  x1, y1,  z );
		glTexCoord2f( 0.0f,		y_repeat );	glVertex3f(  x0, y1,  z );
	glEnd();
}


void	drawQuad(	GLuint tx_id,
			float z,
			float x0,	float y0,
			float x1,	float y1
			)
{
	drawQuad(tx_id,z,x0,y0,x1,y1,1.0f,1.0f);
}





/* Here goes our drawing code */
int drawGLScene( void )
{

    /* These are to calculate our fps */
	static GLint T0     = 0;
	static GLint Frames = 0;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity( );
	glPushMatrix();
	
// 	gluLookAt(	10.0f,	10.0f,	10.0f,
// 				0.0f,	0.0f,	0.0f,
// 				0.0f,	1.0f,	0.0f	);

// 	glScalef(10.0f,10.0f,10.0f);
	
// 	glRasterPos3f( -1,
// 				0,1);


	cam.look();
	cam.applyPower();
	
	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);

// 	glprintf("hello world %d",1);

// 	gluProject(
// 	gluUnProject(		)
	
// 	GLfloat viewmatrix[16];
// 	glGetFloatv(GL_PROJECTION_MATRIX,viewmatrix);
	
//	float	pos[2];
// 	glCamProject(pos,0.1f,0.1f);
	

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glBindTexture( GL_TEXTURE_2D, TEXTURE(TX_BG) );
	


	glPushMatrix();
	
	glTranslatef(1.0f,1.0f,1.0f);
// 	glRasterPos2f( -1,
// 
// 				1);


	glRotatef( xrot, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis By xrot */
	glRotatef( yrot, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis By yrot */

	glTranslatef(cursor_pos[0],cursor_pos[1],10.0f);
	glScalef(10.0f,10.0f,10.0f);
	switch( object )
	{
	case 0:
	    /* Draw our cube */
//	    drawGLCube( );
	    break;
	case 1:
	    /* Draw a cylinder */
	    glTranslatef( 0.0f, 0.0f, -1.5f );
//	    gluCylinder( quadratic, 1.0f, 1.0f, 3.0f, 32, 32 );
	    break;
	case 2:
	    /* Draw a sphere */
//	    gluSphere( quadratic, 1.3f, 32, 32 );
	    break;
	case 3:
	    /* Draw a cone */
	    glTranslatef( 0.0f, 0.0f, -1.5f );
//	    gluCylinder( quadratic, 1.0f, 0.0f, 3.0f, 32, 32 );
	    break;
	};
// 	    glEnable(GL_TEXTURE_2D);
 	


    glPopMatrix();

    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_T );

	

	drawQuad(TX_BG,		-0.05,	-1000.0f,	-1000.0f,
								 1000.0f,	 1000.0f,
								 	4.0f,4.0f	);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	render_simulation();
	nwi.render();

	glPopMatrix();
	glPushMatrix();
	
		glTranslatef( 0.25*ratio, 0.25f, -1.0f );
		
		
 		glScalef(0.25f,0.25f,0.25f);
		
// 		glTranslatef( -0.0f,1.0f,0.0f);
		
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
// 		glprintf(0.0f,0.0f,.05f,"hello world %d",1);

		if(mode!=2)
			nwi.text_render();
			
		else	
		{
			glTranslatef( -1,-0.5,0 );
			
// 			glScalef(1.75,1,1);

			nws.render();
			

		}
		

	glPopMatrix();
	glPushMatrix();
		
		glTranslatef( -0.40*ratio, 0.35f, -1.0f );
		
		
		
 		glScalef(0.25f,0.25f,0.25f);
		
// 		glTranslatef( -0.0f,1.0f,0.0f);
		
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		
		glLog.render();
// 		glprintf(0.0f,0.0f,.05f,"hello world %d x\n asd\nasd",1);
		
		
	
	glPopMatrix();
 		glScalef(ratio,1,1);
		glTranslatef( -1, 1-2.0f/16, -2.4f );
		
		
		
		for(int i=0;i<N_MENU;i++)
		{
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glprintf(0.05f,+2.0f/64,0.05f,menu_str[i]);
			
			
			glColor4f(1.0f,1.0f,1.0f,0.25f);
			drawQuad(i==mode?TX_BUTTON_1:TX_BUTTON_0,		-0.0f,	-0.0f,0,
			
			
										2.0f/N_MENU,	 2.0f/16
									
										);
			
			glTranslatef(		2.0f/N_MENU,0,0);
			
			
			
		}
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		
		


	SDL_GL_SwapBuffers( );

	static	int	last_t;
	
	GLint t = SDL_GetTicks();
	if(Frames)
		diff=(t-last_t)/1000.0;
// 	LOG(MSG,"time:	%d",	last_t);
	last_t=t;
	
	Frames++;
	{
	GLint t = SDL_GetTicks();
	if (t - T0 >= 5000) {
	    GLfloat seconds = (t - T0) / 1000.0;
	    GLfloat fps = Frames / seconds;
	    printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
	    T0 = t;
	    Frames = 0;
	}
    }

    xrot += xspeed; /* Add xspeed To xrot */
    yrot += yspeed; /* Add yspeed To yrot */




    return( TRUE );
}


bool	loop()
{
	/* handle the events in the queue */
	int				isActive = TRUE;
	SDL_Event			event;

	
	while ( SDL_PollEvent( &event ) )
	{
		HVector	pos;
		HVector	dir;
		switch( event.type )
		{
		case SDL_ACTIVEEVENT:
			/* Something's happend with our focus
			* If we lost focus or we are iconified, we
			* shouldn't draw the screen
			*/
			if ( event.active.gain == 0 )
			isActive = FALSE;
			else
			isActive = TRUE;
			break;			    
		case SDL_VIDEORESIZE:
			/* handle resize event */
			surface = SDL_SetVideoMode( event.resize.w,
						event.resize.h,
						16, videoFlags );
			if ( !surface )
			{
				fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				Quit( 1 );
			}
			resizeWindow( event.resize.w, event.resize.h );
			break;
		case SDL_KEYDOWN:
			/* handle key presses */
			handleKeyPress( &event.key.keysym ,dir);
			break;
		case SDL_QUIT:
			/* handle quit requests */
			done = TRUE;
			break;
		case	SDL_MOUSEBUTTONDOWN:
// 			printf("Current mouse position is: (%d, %d)", event.motion.x, event.motion.y);
			
			glCamProject(&pos,&dir,event.button.x, event.button.y);
			switch(event.button.button)
			{
				case	SDL_BUTTON_LEFT:
// 						LOG(MSG,"%f	%f,	",pos[0],pos[1]);
						{
							Vec2D	s(pos[0],pos[1]);
						nwi.select(radar.look(s));
						}
// 						glCamProject(0,event.button.x, event.button.y);
					break;
				case	SDL_BUTTON_RIGHT:
// 						glCamProject(0,event.motion.x, event.motion.y);
					break;
				case	SDL_BUTTON_MIDDLE:
						cam.pos[0]=(cam.pos[0]+pos[0])/2;
						cam.pos[1]=(cam.pos[1]+pos[1])/2;
// 						LOG(MSG,"%f	%f,	",pos[0],pos[1]);
					break;
				
				case	SDL_BUTTON_WHEELUP:		cam.perturbate(-0.5f, dir);	break;
				case	SDL_BUTTON_WHEELDOWN:	cam.perturbate( 0.5f, dir);	break;
				default:
					LOG(MSG,"mode:	%d",event.button.button);
					
					break;
 			}
			break;
		case	SDL_MOUSEMOTION:
			glCamProject(&pos,&dir,event.button.x, event.button.y);
			cursor_pos=pos;
			if(event.button.y<window_height/16)
			{
				int	new_mode=event.button.x*N_MENU/window_width;
				if(new_mode!=mode)
					LOG(MSG,"mode:	%d",new_mode);
				mode=new_mode;
			}
			break;
		default:
			break;
		}
	}

	    /* draw the scene */
	if ( isActive )
		drawGLScene( );
// 	mode=1;

	if(mode!=0)
		step_simulation();
// 	if(radar.timestamp>3200)
// 		done=1;

	return	!done;
}

// int main( int argc, char **argv )
int	initGL()
{
	const SDL_VideoInfo	*videoInfo;
	
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Video initialization failed: %s\n",
			SDL_GetError( ) );
		Quit( 1 );
	}
	
	videoInfo = SDL_GetVideoInfo( );
	
	if ( !videoInfo )
	{
		fprintf( stderr, "Video query failed: %s\n",
			SDL_GetError( ) );
		Quit( 1 );
	}
	
	videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL          */
	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering       */
	videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	videoFlags |= SDL_RESIZABLE;       /* Enable window resizing        */
	
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else	videoFlags |= SDL_SWSURFACE;

	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags );

	if ( !surface )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    Quit( 1 );
	}

	if ( ( SDL_EnableKeyRepeat( 100, SDL_DEFAULT_REPEAT_INTERVAL ) ) )
	{
	    fprintf( stderr, "Setting keyboard repeat failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

	if ( initOpenGL( ) == FALSE )
	{
	    fprintf( stderr, "Could not initialize OpenGL.\n" );
	    Quit( 1 );
	}

	resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

	LOG(MSG,"loading font");
	buildFont();


	return	0;
}
