%Week6
clc
clear all
close all

%%
r=5;
s=3;
A=zeros(r,s);
B=eye(2,2);
v1=rand(r,1);
v2=rand(s,1);

v3= v1*v2';
A(2:3,2:3)= v3(1:2,1:2)
% A(2,:)=v1(1:3)    
%%
r=abs(sin(linspace(0,2*pi,720)));
g= abs(sin(linspace(0,2*pi,720)+(2/3)*pi));
b= abs(sin(linspace(0,2*pi,720)+(4/3)*pi));

a=720;
bb=360;
c=3; %RGB

A=ones(a,bb,c);
A(:,:,1)=A(:,:,1).*r'; %R
A(:,:,2)=A(:,:,2).*g'; %G
A(:,:,3)=A(:,:,3).*b'; %B

figure
imshow(A)
%%

syms a b c x
fcn=(a*x^2 + b*x + c ==0);
y=solve(fcn,x);
subs(y,[a b c],[2 5 7])
%%

fcn1= exp(x)+1;

yf1=int(fcn1,[1 5]);
yf2=diff(fcn1);




