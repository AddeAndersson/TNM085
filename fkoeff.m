function [u] = fkoeff(v0,t)

%Funktionen tar in vO och tiden, den returnerar friktionskoefficienten för
%den aktuella tidpunkten.

ur = 0.01;
ug = 0.2;
g = 9.82;

tswich = (2/7)*(v0/(g*ug));


    if t < tswich
    u = (t/tswich)*ur + (1-(t/tswich))*ug;

    else
        u = ur;
    end

end
