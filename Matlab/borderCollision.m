function vOut = borderCollision(pos,r,v)
    
    vOut = v;

    if (pos(1)<=r);        pos(1)=r;         vOut(1)=-v(1); end
    if (pos(1)>=2.13-r);   pos(1)=2.13-r;    vOut(1)=-v(1); end    
    if (pos(2)<=r);        pos(2)=r;         vOut(2)=-v(2); end
    if (pos(2)>=1.065-r);  pos(2)=1.065-r;   vOut(2)=-v(2); end
    
    
end