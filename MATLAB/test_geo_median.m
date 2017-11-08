clear
clc
close all
N = 1000;
d = 10000;
N1 = round(2*N/3);
N2 = N - N1;
Z = rand(d,N1);
a = 100*randn(d,1);
Zp = repmat(a,1,N2);%-ones(d, N2);%sign(randn(d,N2));
Z = [Z, Zp];
w = ones(N,1);

%%
%tic
%x = geo_median(Z, w);
%toc
%figure;plot(x)
%x
%a = sort(Z);
%(a(floor(end/2+.5))+a(ceil(end/2+.5)))/2
%median(Z)


%% Save data to files
A = full(Z);
fileID = fopen('Z.txt','w');
fprintf(fileID,'%f\n',A);
fclose(fileID);

B = [N;d];
fileID = fopen('Nd.txt','w');
fprintf(fileID,'%d\n',B);
fclose(fileID);


C = w;
fileID = fopen('w.txt','w');
fprintf(fileID,'%f\n',C);
fclose(fileID);


% D = x;
% fileID = fopen('x.txt','w');
% fprintf(fileID,'%f\n',D);
% fclose(fileID);


