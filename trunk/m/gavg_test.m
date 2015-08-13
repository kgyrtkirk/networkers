
function	gavg_test()
	t0=	[	0	0	;
			3	0	;
			0	4	];
			
	t1=	[	5	-1	;
			6	0	;
			5	4	];

	t2=	[	0	5	;
			-1	7	;
			-2	8	;
			1	12	];

	t3=	[	3	5	;
			5	7	;
			6	8	;
			9	12	];

	t5=	[	1	5	;
			1.1	4.9	;
			1.2	5	;
			1.3	4.9	;
			1.4	5	;
			1.5	4.9	;
			1.5	11	];


	r=32;
	l=4;
	t= 2*pi*[1:r]'.^l/r^l;
	t4=[	sin(t)	cos(t)	];
	t4*=3;
	t4(:,1)+=10;

	r=32;
	r1=0.1;
	l=4;
	t= 2*pi*[1:r]'.^l/r^l;
	
	t6=[	sin(t).*t/r	cos(t).*t/r	];
	t6*=20;
	t6(:,1)+=11;
	t6(:,2)+=6;

%  	t4

	
%  	t4=	[	8	1	;
%  			5	7	;
%  			6	8	;
%  			9	12	];

%  	plot_border("east");
	
	plot_w=2;
	plot_h=2;
	plot_x=1;
	
	targets=[0 1 2 4 8 16 32 64 128]
	hold off;
%  	subplot(plot_w,plot_h,plot_x++);
%  	shape(t0,targets);
%  	subplot(plot_w,plot_h,plot_x++);
%  	shape(t1,targets);
	subplot(plot_w,plot_h,plot_x++);
	shape(t2,targets);
	subplot(plot_w,plot_h,plot_x++);
%  	shape(t3,targets);
%  	subplot(plot_w,plot_h,plot_x++);
	shape(t4,targets);
	subplot(plot_w,plot_h,plot_x++);
	shape(t5,targets);
	subplot(plot_w,plot_h,plot_x++);
	shape(t6,targets);
	
%  	q=[];
%  	z=[];
%  	for i=[0:5]
%  		q=[	q;
%  			gavg(t0,i);
%  			gavg(t1,i);
%  			gavg(t2,i);
%  			gavg(t3,i);
%  			gavg(t4,i);
%  			gavg(t5,i);
%  			gavg(t6,i)
%  						];
%  
%  		z=[	z;
%  			lavg(t0,i);
%  			lavg(t1,i);
%  			lavg(t2,i);
%  			lavg(t3,i);
%  			lavg(t4,i);
%  			lavg(t5,i);	
%  			lavg(t6,i)	
%  						];
%  	end
%  	plot(q(:,1),q(:,2),'xr');
%  	plot(z(:,1),z(:,2),'+y');

end