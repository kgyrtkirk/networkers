
#include "port/portability.h"
#include "util/log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "geosteiner_io.h"
#include "textures.h"

#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

void set_async(int fd);
void set_blocking(int fd);
void set_nonblocking(int fd);



int	setup_emu_channel(int*io_fds,const char*program)
//int main(int argc, char **argv)
{
//    int	i;
    pid_t	pid;
    int	to_child_pipe[2];
    int	from_child_pipe[2];
//    int	f_in, f_out;
//    int	n;
//    char buffer[512];

    if(	socketpair(AF_UNIX, SOCK_STREAM, 0, to_child_pipe)	||
		socketpair(AF_UNIX, SOCK_STREAM, 0, from_child_pipe)	)
	{
		fprintf(stderr, "socketpair error (%d)\n", errno);
		exit(1);
    }

    pid = fork();
    if (pid == -1) {
        fprintf(stderr, "fork error (%d)\n", errno);
        exit(1);
    }

	if (pid == 0)
	{ /* CHILD */
		if (	dup2(to_child_pipe[0], STDIN_FILENO) < 0	||
			close(to_child_pipe[1]) < 0				||
			close(from_child_pipe[0]) < 0				||
			dup2(from_child_pipe[1], STDOUT_FILENO) < 0 )
		{
			fprintf(stderr, "dup2/close error (%d)\n", errno);
			exit(1);
		}
        if (to_child_pipe[0] != STDIN_FILENO)
                close(to_child_pipe[0]);
        if (from_child_pipe[1] != STDOUT_FILENO)
                close(from_child_pipe[1]);

        set_blocking(STDIN_FILENO);
//        set_blocking(STDOUT_FILENO);
//        set_nonblocking(STDIN_FILENO);
//        set_blocking(STDIN_FILENO);
//        set_nonblocking(STDOUT_FILENO);

		const char	*argv[2];
		argv[0]=program;
		argv[1]=0;
		execvp(program, (char*const*)argv);
		perror("execvp:");
        fprintf(stderr, "execvp error (%d) on '%s'", errno, program);

        exit(1);
    }

    /* PARENT */

    if (close(from_child_pipe[1]) < 0 || close(to_child_pipe[0]) < 0) {
        fprintf(stderr, "close error (%d) on child handles", errno);
        exit(1);
    }

	io_fds[0]=from_child_pipe[0];
	io_fds[1]=to_child_pipe[1];

//	set_async(io_fds[0]);
//	set_async(io_fds[1]);
	
//	set_blocking(io_fds[1]);
//	set_blocking(io_fds[0]);
	
//    f_in  = from_child_pipe[0];
//    f_out = to_child_pipe[1];
//
//    /* read and display data from child process */
//	while (1)
//	{	
//        n = read(f_in, buffer, sizeof(buffer) - 1);
//        if (n == 0)
//            continue;
//        if (n == -1) {
//            if (errno != ECONNABORTED && errno != ECONNRESET)
//                fprintf(stderr, "read error (%d)\n", errno);
//            break;
//        }
//
//        buffer[n] = '\0';
//
//        printf("%s", buffer);
//        fflush(NULL);
////        write(f_out,"asd\r\n",5);
//        
//    }
//	printf("xxxxx\n");
//    exit(1);
	return	0;
}


void	geosteiner_io()
{
	int     fds[2];
	int		c;
	char		chr;
	LOG(MSG,"geo-io start");
	setup_emu_channel(fds,"./etsp_pipeline.bash");
	const	char*q="0 0\n2 2\n3 3\n10 10\n";
	write(fds[1],q,strlen(q));
	close(fds[1]);
	
	while(1)
	{
		c=read(fds[0],&chr,1);
		if(c>0)
		{
			printf("%c",chr);
		}
		else
			break;
		usleep(500);
	}
	close(fds[1]);
	LOG(MSG,"geo-io exit");
//	exit(0);
}


void	cGeoSteinerIO::clear()
{
	cListCursor<GPoint>		cr(&points);
	GPoint				*pos;
	while((pos=cr.next()))
		delete pos;
//	checksum=0.0f;
	points.empty();
}

void	cGeoSteinerIO::render()
{
	cListCursor<GeoEdge	>		cr(&edges);
	GeoEdge					*edge;

	
	while((edge=cr.next()))
	{
	//	LOG(MSG,"steiner-edge	%f,%f	%f,%f",edge->a[0],edge->a[1],edge->b[0],edge->b[1]);
		glPushMatrix();
		glTranslatef(edge->a[0],edge->a[1],8.0f);

		drawMessage(TX_STEINER,Vec2D(edge->a[0],edge->a[1]),Vec2D(edge->b[0],edge->b[1]));
// 		drawMessage(TX_STEINER,edge->a,edge->b);
		glPopMatrix();
	}
		
//	checksum=0.0f;
//	points.empty();
}

void	cGeoSteinerIO::addPoint(HVector p)
{
// 	LOG(MSG,"AP	%f	%f",p[0],p[1]);
	points.add(new GPoint(p));
}

void	cGeoSteinerIO::calculate()
{
	static	int     fds[2];
	static	char		line[512];
	static	int		i;
	cListCursor<GeoEdge	>	edge_cr(&edges);
	GeoEdge				*edge;
	
	int		c;
	char		chr;
	if(!running)
	{
		cListCursor<GPoint>		cr(&points);
		GPoint				*pos;
		float	new_checksum;
		new_checksum=0.0f;
		while((pos=cr.next()))
		{
	// 		LOG(MSG,"X	%f	%f",pos->p[0],pos->p[1]);
			new_checksum+=pos->p[0]+pos->p[1];
		}
// 		LOG(MSG,"checksums:	%f	%f",checksum,new_checksum);
		if(checksum==new_checksum)
			return;
		
		checksum=new_checksum;
		//	if there isn't enough points..
		if(points.getsize()<3)
			return;
		
		LOG(MSG,"geosteiner update!");
		
		while((edge=edge_cr.next()))
			delete	edge;
		edges.empty();
		LOG(MSG,"geo-io start");
		setup_emu_channel(fds,"./etsp_pipeline.bash");
		cr.reset();
		while((pos=cr.next()))
		{
			dprintf(fds[1],"%.6f %.6f\n",pos->p[0],pos->p[1]);
// 			printf("%f %f\n",pos->p[0],pos->p[1]);
		}
		close(fds[1]);
		
		running=true;
		i=0;
	}

//	else
//	{

	//	write all points to pipe
	
	set_nonblocking(fds[0]);
	
//	i=0;
	while(1)
	{
		c=read(fds[0],&chr,1);
		if(c>0)
		{
			switch(chr)
			{
				case	'\n':
						line[i]=0;
						{
							edge=new GeoEdge();
							float	x1,x2,y1,y2;
							sscanf(line,"%f %f %f %f",&x1,&y1,&x2,&y2);
							x1/=1000000.0;
							x2/=1000000.0;
							y1/=1000000.0;
							y2/=1000000.0;
							edge->a=HVector(x1,y1,0,0);
							edge->b=HVector(x2,y2,0,0);
// 							LOG(MSG,"steiner-edge	%f,%f	%f,%f",x1,y1,x2,y2);
							edges.add(edge);
						}
						i=0;
					break;
				default:
					if(i<510)
						line[i++]=chr;
			}
// 			printf("%c",chr);
		}
		else
		{
			if(c==0)
			{
				LOG(MSG,"geoio-close	%d",c);
				close(fds[0]);
				close(fds[1]);
				running=false;
//				LOG(MSG,"geoio-close	cnt:%d",c);
				break;
			}
			else
			{
				break;
			}
		}
//		usleep(500);
	}
// 	close(fds[0]);
	
}


void set_blocking(int fd)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) == -1)
        return;
    if (val & O_NONBLOCK) {
        val &= ~O_NONBLOCK;
        fcntl(fd, F_SETFL, val);
    }
}

void set_nonblocking(int fd)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) == -1)
        return;
    if (!(val & O_NONBLOCK)) {
        val |= O_NONBLOCK;
        fcntl(fd, F_SETFL, val);
    }
}

//void set_async(int fd)
//{
//    int val;
//
//    if ((val = fcntl(fd, F_GETFL, 0)) == -1)
//        return;
//    if (!(val & O_DIRECT)) {
//        val |= O_DIRECT;
//        fcntl(fd, F_SETFL, val);
//    }
//}

