close all;
clear;
clc

% ���/��е�ṹ��������ʱδʹ�ã�
step_x = (2.1*6400)/100;   %���������������λmm
step_y = step_x;
step_z = 1.5*step_x;
step_v = 6400;   % pulse/second

% ��ͼ�ߴ緶Χ���ƣ���ʱδʹ�ã�
start_x=0;      %�������ޣ������ϵͳ����ԭ�㣬��λmm��
start_y=0;
start_z=0;
end_x=80;      %�������ޣ������ϵͳ����ԭ�㣬��λmm��
end_y=80;
end_z=80;

% �����ܻ�����
min_step = 2*pi/100;            %�ܻ���������λmm

% Բ����
R = 40;
pitch = pi/4;
theta = 0: min_step :1.75*pi;

Ori_x= 0;
Ori_y= 0;
Ori_z= 0;

% �ܻ�
x = R*cos(pitch)*cos(0*pi - theta) + Ori_x;
y = R*cos(pitch)*sin(0*pi - theta) + Ori_y;
z = 1*x*tan(pitch) + Ori_z;
figure;
subplot(2,2,1);
plot3(x,y,z);
title('Բ�ι켣');
hold on;

% ת��������ʽ��
px = x*step_x;
py = y*step_y;
pz = z*step_z;
[vx vy vz t] = get_pt_data3( px, py, pz, step_v); % get pulse_vel

%����
x_int = int32(px);
y_int = int32(py);
z_int = int32(pz);
vx_int = int32(vx);
vy_int = int32(vy);
vz_int = int32(vz);
t_int = int32(t);

% ������
SineData=[x_int', y_int', z_int', vx_int', vy_int', vz_int', t_int'];

disp( ['max px:' max(px) 'pulse']);
disp( ['max py:' max(py) 'pulse']);
disp( ['max pz:' max(pz) 'pulse']);

disp( ['max vx:' max(vx) 'pulse/s']);
disp( ['max vy:' max(vy) 'pulse/s']);
disp( ['max vz:' max(vz) 'pulse/s']);

save('D:/CircleData3.txt','SineData','-ascii','-double')
% plot3(x_int',y_int', z_int');
% figure;
% plot(vx_int);
% hold on;
% plot(vy_int, 'r');
% figure;
% plot(x_int);
% hold on;
% plot(y_int, 'r');







