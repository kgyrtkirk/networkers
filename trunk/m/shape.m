
function	shape(a,l)
	hold off;
	
	axis("square");

	plot([a(:,1);a(1,1)],[a(:,2);a(1,2)]);
	hold on;
	plot([a(:,1);a(1,1)],[a(:,2);a(1,2)],'xg');
	
	
	%  	axis([1 1 1 1]);
	m=[	min(a(:,1)) min(a(:,2))	];
	M=[	max(a(:,1)) max(a(:,2))	];
	d=M-m;
	d=max(d)/10;
	axis([ m(1)-d M(1)+d m(2)-d M(2)+d ],"square");

%  	axis([min(a(:,1)-1 max(a(:,1)+1 min(a(:,2)-1 max(a(:,2)+1]);
%  	axis("square");

	q=z=[];
	for i=l
		q=[	q;
			gavg(a,i)	];
		z=[	z;
			lavg(a,i)	];
	end

	plot(q(:,1),q(:,2),'xr');
	plot(z(:,1),z(:,2),'+y');


end