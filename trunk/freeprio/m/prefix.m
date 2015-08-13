
function	m=prefix(m)
	for i=[0:floor(log2(size(m,2)))]
		d=2^i;
		r=[1:size(m,2)-d ];
%  		r1=[1:(d-1)];
%  		m(:,r1)+=m(:,r1+size(m,2)-d+1);
%  		m(:,r1)+=m(:,r1+size(m,2)-d+1);
		m(:,r+d)+=m(:,r);

	end
end
