function [v1Out, v2Out] = ballCollision(pos1, pos2, v1, v2)
    
    %m=0.165;
    r=0.0286;

    if (norm (pos1-pos2)<=2*r-0.001)

        % Copy velocity vectors
        u1=v1;
        u2=v2;

        % 1 - Compute angles
        Alpha1=atan2(pos2(2)-pos1(2) , pos2(1)-pos1(1) );
        Beta1=atan2(u1(2),u1(1));
        Gamma1=Beta1-Alpha1;

        % 2 - Compute norm of vectors after decomposition
        u12=norm(u1)*cos(Gamma1);
        u11=norm(u1)*sin(Gamma1);

        % 3 - Repeat 1 and 2 for the second boody
        Alpha2=atan2(pos1(2)-pos2(2) , pos1(1)-pos2(1) );
        Beta2=atan2(u2(2),u2(1));
        Gamma2=Beta2-Alpha2;
        %u21=norm(u2)*cos(Gamma2);
        u22=norm(u2)*sin(Gamma2);

        % 4 - Compute norm of sub vectors after collision
        v21 = u12;
        v12 = -v21;

        % 5 and 6 compute velocities after collision
        v1Out=u11*[-sin(Alpha1),cos(Alpha1)] + v12*[cos(Alpha1),sin(Alpha1)];
        v2Out=u22*[-sin(Alpha2),cos(Alpha2)] - v21*[cos(Alpha2),sin(Alpha2)];
    else
        v1Out = v1;
        v2Out = v2;
        
    end

end