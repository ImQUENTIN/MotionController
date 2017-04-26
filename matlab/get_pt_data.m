function [vx, vy, vz] = get_pt_data3( x, y, z, vel)
% x, y, z 是三个轴的pos信息， 单位pulse
% vel 是希望的速度。 pulse/second

%计算速度
vx = x;
vx(2:end) = (x(2:end) - x(1:end-1));
vy = y;
vy(2:end) = (y(2:end) - y(1:end-1));
vz = z;
vz(2:end) = (z(2:end) - z(1:end-1)); 
s = sqrt(vx.^2 + vy.^2 + vz.^2);
t = s/vel;

vx = vx ./t;
vy = vy ./t;
vz = vz ./t;