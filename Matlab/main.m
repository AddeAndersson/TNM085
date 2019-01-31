

% Array with ball positions
% row 1 is x-coordinates and row 2 is y coordinates
allPos = [0.5325, 1.5975, 1.6470, 1.6470, 1.6965, 1.6965 1.6965, 1.7460 ,1.7460 ,1.7460 ,1.7460, 1.7955 , 1.7955, 1.7955, 1.7955, 1.7955;
       0.5325, 0.5325, 0.5039, 0.5611, 0.4753, 0.5325, 0.5897, 0.4467, 0.5039, 0.5611, 0.6183,0.4181, 0.4753, 0.532, 0.5897, 0.6469];

% row 1 is x-velocity and row 2 is y-velocity   
allVel = zeros(2,16);
allVel(1,1) = 0.001;

%General properties of the balls
m=0.165;
r=0.0286;

% Background
patch([0,0,2.13,2.13],[0,1.065,1.065,0],'g','FaceAlpha',0.5);
grid on;
axis square equal;
hold on;

% Create the circles (graphical items)
a = 0:0.1:2*pi;
xCircle=cos(a);
yCircle=sin(a);

% Color array
colors =['w','m','c','r','y','k','b','m','c','r','y','g','b','y','g','b'];

hold on
axis equal

patches = zeros(1,16);

% Store patches with circles
for i=1:16  
    
  x=allPos(1,i)+r*xCircle;
  y=allPos(2,i)+r*yCircle;
  color = colors(i); 
  patches(1,i) = patch(x,y,color,'FaceAlpha',1);

end


% Motion engin
for i = 1: 0.01 : 5000
    
    % Update position
    allPos = allPos+allVel;

    % Check for collision with playground boarder
    for n = 1:16

        pos = [allPos(1,n), allPos(2,n)];
        vel = [allVel(1,n), allVel(2,n)];

        vOut = borderCollision(pos,r,vel);

        allVel(1,n) = vOut(1);
        allVel(2,n) = vOut(2);

    end
    
    % check for collision with other ball
    for m = 1:15
        for n = m+1:16
            
            pos1 = [allPos(1,m), allPos(2,m)];
            vel1 = [allVel(1,m), allVel(2,m)];
        
            pos2 = [allPos(1,n), allPos(2,n)];
            vel2 = [allVel(1,n), allVel(2,n)];
            
            [v1Out, v2Out] = ballCollision(pos1,pos2,vel1,vel2);
            
            allVel(1,m) = v1Out(1);
            allVel(2,m) = v1Out(2);
            
            allVel(1,n) = v2Out(1);
            allVel(2,n) = v2Out(2);            
            

        end
    end


    % Update display    
    for n = 1:16 
         set(patches(1,n),'XData',allPos(1,n)+r*xCircle, 'YData', allPos(2,n)+r*yCircle);
    end
   
    
    drawnow;
    
end
 