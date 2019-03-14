%   
% Authors: 
% Joel Paulsson: joepa811@student.liu.se
% Johan Fr?berg: johfr198@student.liu.se
% Adrian Andersson: adran117@student.liu.se
% Gustaf Wallstr?m: gusan112@student.liu.se
% 
% This program simulates a billiard break-shot.
% 
% Parameters:
% N = number of balls
% L = table length
% W = table width
% dt = timestep
% k = separation constant
% X = vector with positions
% V = vector with velocities
% R = radius of the balls
% M = mass of the balls
% nstep = number of steps
% pit = number of iterations between two plots
% colors = vector with colors
%
%
%%

clc 
clear
close all

%------------------------------------------------
N=16;                 
L=2.13;               
W = 1.065;            
dt=0.0003;
k = 0.005;


X = [0.5325, 1.5975-2*k, 1.6470-k, 1.6470-k, 1.6965, 1.6965, 1.6965, 1.7460+k ,1.7460+k ,1.7460+k ,1.7460+k, 1.7955+2*k , 1.7955+2*k, 1.7955+2*k, 1.7955+2*k, 1.7955+2*k;
       0.5325, 0.5325, 0.5039-k, 0.5611+k, 0.4753-k, 0.5325, 0.5897+k, 0.4467-2*k, 0.5039-k, 0.5611+k, 0.6183+2*k,0.4181-2*k, 0.4753-k, 0.532, 0.5897+k, 0.6469+2*k];

V = zeros(2,N);
V(1,1) = 17; % 
R=0.0286;   
M=0.165; 

theta=0:2*pi/20:2*pi;
nstep=2000;               
pit=80;                  
counter = 0;
colors =['w','m','c','r','y','k','b','m','c','r','y','g','b','y','g','b'];
 
%auxiliary variables  
  Ax=zeros(N,N);
  Ay=zeros(N,N);
  Ar=zeros(N,N);
  Ar(1:N,1:N) = 2*R;
 
  
 % plot background and "balls
 hold on
 axis equal
 plotbackground();
 for i=1:N       
  x=X(1,i)+R*cos(theta);
  y=X(2,i)+R*sin(theta);
  patch(x,y,colors(i))
 end
 drawnow
 pause(1)

  
timer = tic;

% Motion engine
for T=0:nstep
    
    %update positions
    X=X+dt*V;
    
    %check if particules collided with eachother
    for i=1:N
        Ax(i,:) = X(1,i) - X(1,:);
        Ay(i,:) = X(2,i) - X(2,:);
    end
    
    % the upper triangular part of the matrices
    Ax = triu(Ax);
    Ay = triu(Ay);
    
    % calculate the distance between each two particles
    Nrm = (Ax.^2+Ay.^2).^(0.5)-Ar-10^-3; 
    Nrm = triu(Nrm(1:N-1,2:N));
    
    % find the particles that collided 
    [row,col] = find(Nrm < 0); 
    l = length(row);
    
    % Ball to ball colision and calculating new velocities
    if l~=0
        col=col+1;       
        for t=1:l
            i=row(t,1);
            j=col(t,1);
            
            C1=(2*M/(M+M))*dot(V(:,i)-V(:,j),X(:,i)-X(:,j))/(norm(X(:,i)-X(:,j))^2);
            C2=(2*M/(M+M))*dot(V(:,j)-V(:,i),X(:,j)-X(:,i))/(norm(X(:,i)-X(:,j))^2);
            V(:,i) = V(:,i) - C1*(X(:,i)-X(:,j));
            V(:,j) = V(:,j) - C2*(X(:,j)-X(:,i));
             
            if(counter == 0)
                tFirstColl = toc(timer);
            end
            counter = 1;
            
        end
    end
    
    
    % Calculate friction
    if T==pit*ceil(T/pit)
        
       
    fric = abs(V) * 0.1 * 9.82* T*dt;
    
     %fric = 0.02 * 9.82 * T*dt;
    
    %fricX = abs(V(i)) * 0.2 * 9.82* T*dt;
    %frixY
    
    for m = 1:2
     for n = 1:16
         
         s = sign(V(m,n));
         V(m,n) = V(m,n) - s*fric(m,n);
        
     end
    end
    end
       
   
    % ball to border collision
     for i=1:N
        %check if particules collided with the walls horizontally
        if X(1,i)+ R >= L-10^-2 || X(1,i)-R <= 0+10^-2
            
            V(1,i) = sqrt(0.75*V(1,i)^2);
            V(1,i)=-V(1,i);
        end
        %check if particules collided with the walls vertically
        if X(2,i)+R >= W-10^-3 || X(2,i)-R<=0+10^-3
            
            V(2,i) = sqrt(0.75*V(2,i)^2);
            V(2,i) = -V(2,i);
        end 
     end  
     
     
     % plot
     if T==pit*ceil(T/pit)
         
         clf
         hold on
         axis equal
         plotbackground();
         
         for i=1:N    
             
              x=X(1,i)+R*cos(theta);
              y=X(2,i)+R*sin(theta);
              patch(x,y,colors(i));
              
         end
         
        drawnow
        pause(0.01)
        
     end
end


 
 