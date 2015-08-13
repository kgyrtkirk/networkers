



function	scan_repos_flow()
	s=0.7;
	[x,y]=meshgrid (-1:0.1:1);
	
	
%  	[x,y]=meshgrid ([-0.1:0.005:0.1],[0.7:0.005:0.9]);
%  	newplot
%  	clear
%  	quiver (x, y, -x,-y+(1-abs(x)).*(y+s),'-r');
%  	quiver (x, y, -x,-y+(1-abs(x)).*(s./(abs(y-s)+1)),'-r');
%  	f(x)=1;
%  	quiver (x, y, y.*max(y,0),-x.*max(y,0));
	
%  	quiver (x, y, y.*max(1-2*sqrt(x.^2+(y-s).^2),0),-x.*max(1-2*sqrt(x.^2+(y-s).^2),0),'-m');
	
%  %  	z=x;
%  	z(:,2)=y;

%  	zx= y.*max(1-2*sqrt(x.^2+(y-s).^2),0)-x;
%  	zy=-x.*max(1-2*sqrt(x.^2+(y-s).^2),0)-y+(1-abs(x)).*(s./(abs(y-s)+1));
	
	mx=max(1-1*sqrt(x.^2+(y-s).^2),0);
	my=max(1-1*sqrt(x.^2+(y-s).^2),0);
	
	ux=(-x);
%  	uy=(-y+(1-abs(x)).*(s./(abs(y-s)+1)));
	uy=(-y+s);
	
	for i=[1:size(x,1)]
	for j=[1:size(x,1)]
		q=sqrt(ux(i,j)^2+uy(i,j)^2);
		ux(i,j)/=q;
		uy(i,j)/=q;
	end
	end
	
	
	zx=ux.*(1-mx);
	zy=uy.*(1-mx);
	
	zx= y.*mx	+ux.*(1-mx);
	zy=-x.*mx	+uy.*(1-mx);
	
	max(zx.^2+zy.^2)
	
	
	hold off;
	quiver (x, y, zx, zy,'k;x;');
	hold on;

%  	quiver (x, y, (1-abs(x)),0);
	c=[0:0.1:2*pi+0.1]';
	c(:,2)=1;
	polar (c(:,1),c(:,2)*s,		'-b;l;');
	polar (c(:,1),c(:,2),		'-r;R;');
	
	print('doc/fig/scan_repos.eps','-deps')

	
%  	set terminal postscript eps rounded
%  	set output '2dplot.eps'

end
