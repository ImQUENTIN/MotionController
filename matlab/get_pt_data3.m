function [vx, vy, vz, t] = get_pt_data3( x, y, z, vel)
% x, y, z 是三个轴的pos信息， 单位pulse
% vel 是希望的速度。 pulse/second

%计算速度
vx = x;
vy = y;
vz = z;
s  = z;
figure;
plot(vx);
hold on;
plot(vy, 'r');
hold on;
plot(vz, 'c');

vy(2:end) = (y(2:end) - y(1:end-1));
vx(2:end) = (x(2:end) - x(1:end-1));
vz(2:end) = (z(2:end) - z(1:end-1)); 
s = sqrt(vx.^2 + vy.^2 + vz.^2);

figure;
plot(vx);
hold on;
plot(vy, 'r');
hold on;
plot(vz, 'c');
hold on;
plot(s, 'k');

t = s/vel;
%t = 4/100;
vx = vx ./t ;
vy = vy ./t;
vz = vz ./t;
s = sqrt(vx.^2 + vy.^2 + vz.^2);

figure;
plot(vx);
hold on;
plot(vy, 'r');
hold on;
plot(vz, 'c');
hold on;
plot(s, 'k');
