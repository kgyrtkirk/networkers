## -*- texinfo -*-
## @deftypefn {Function File} {} [p]=gavg (@var{input_cnt})
## calculates the g weighted point of the given V verices
##
## @example
## @group
## s = [ "ab"; "cde" ];
## strcat (s, s, s)
##      @result{} "ab ab ab "
##         "cdecdecde"
## @end group
## @end example
## @end deftypefn

## Author: kirk


function	a=gavg(V,k)

	W=0;
	a=V(1,:);
	for j = [1:size(a,2)]
		a(j)=0;
	end
	
	for i = [1:size(V,1)]
		p=V(i,:);
		w=0;
		if(k>0)
			for u = [1:size(V,1)]
				if u != i
					w+=pnorm(V(u,:) - p,k);
				end
			end
		else
			w=1;
		end
		w=w^1;
		a+=p*w;
		W+=w;
	end
	a/=W;

end