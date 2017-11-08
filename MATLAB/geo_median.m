function x = geo_median(Z, w)
% finds the geo_median of the columns in Z, with weights w
max_iter = 200;
epsil = 10^-1;
rho = 1;
obj = zeros(max_iter,1);
res = zeros(max_iter,1);
d = size(Z,1);
N = size(Z,2);
Lambda = zeros(d,N);
Y = zeros(d,N);
%x = mean(Z,2);
x = median(Z,2);
%x = zeros(d,1);
%x = x0;

%Lambda = +(rho/2)*(repmat(x,1,N)-Z-Y);

for j=1:N
    Y(:,j) = x - Z(:,j);
end
%Y = zeros(d,N);
%G = zeros(d,N);
delta = zeros(N,1);
for it=1:max_iter
    if rem(it,50)==0
        it
    end
    X = repmat(x,1,N);
    G = X-Z;
    Lambda = Lambda + rho.*(G-Y);
    V = rho*G+Lambda;
    Yold = Y;
    for j=1:N
        delta(j) = max((norm(V(:,j))-w(j))/rho,0);
        if delta(j)>0
            Y(:,j) = V(:,j)./(w(j)/delta(j)+rho);
        end
    end
    z = mean(Z,2);
    y = mean(Y,2);
    lambda = (1/rho)*mean(Lambda,2);
    x = z+y-lambda;
    for j=1:N
        obj(it) = obj(it) + w(j)*norm(x-Z(:,j));
    end
    X = repmat(x,1,N);
    p_res = rho*norm(Y-Yold)^2;
    d_res = (1/rho)*norm(X-Z-Y)^2;
    res(it) = p_res+d_res;
    if res(it)<epsil
        break;
    end
end
obj = obj(1:it);
res = res(1:it);
figure
plot(obj)
title('obj')
figure
semilogy(res,'r')
title('res')
end