
%  calculates a length based avg of the verticle group v

function	p=lavg(V,k)

	p=[];
	U1=V-shift(V,+1);
	U2=V-shift(V,-1);
	U=[];
	for i=[1:size(V,1)]
		U(i)=norm(U1(i,:))^k + norm(U2(i,:))^k;
	end
%  	V
%  	U
	p=[ V(:,1).*U'	V(:,2).*U'	];
	p/=sum(U);
	p=sum(p);

end