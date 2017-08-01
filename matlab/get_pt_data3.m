function [vx, vy, vz, t] = get_pt_data3( x, y, z, vel)
% x, y, z ���������pos��Ϣ�� ��λpulse
% vel ��ϣ�����ٶȡ� pulse/second

%�����ٶ�
vx = x;
vy = y;
vz = z;
s  = z;

subplot(2,2,2);
plot(vx);
hold on;
plot(vy, 'r');
hold on;
plot(vz, 'c');
title('now position');
legend('pos x', 'pos y', 'pos z');

vy(2:end) = (y(2:end) - y(1:end-1));
vx(2:end) = (x(2:end) - x(1:end-1));
vz(2:end) = (z(2:end) - z(1:end-1)); 
s = sqrt(vx.^2 + vy.^2 + vz.^2);

% subplot(2,2,3);
% plot(vx);
% hold on;
% plot(vy, 'r');
% hold on;
% plot(vz, 'c');
% hold on;
% plot(s, 'k');
% title('delta distance');
% legend('delta x', 'delta y', 'delta z');

t = s/vel;
%t = 4/100;
vx = vx ./t ;
vy = vy ./t;
vz = vz ./t;
s = sqrt(vx.^2 + vy.^2 + vz.^2);

subplot(2,1,2);
plot(vx);
hold on;
plot(vy, 'r');
hold on;
plot(vz, 'c');
hold on;
plot(s, 'k');
title('�����ٶȼ����ٶ�');
legend('x���ٶȣ�vx','y���ٶȣ�vy','z���ٶȣ�vz','���ٶȣ�s');