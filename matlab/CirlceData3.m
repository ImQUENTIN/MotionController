close all;
clear;

% 电机/机械结构参数（暂时未使用）
step_x = (2.1*6400)/100;   %步进电机步长，单位mm
step_y = step_x;
step_z = step_x;
step_v = 6400;   % pulse/second

% 绘图尺寸范围限制（暂时未使用）
start_x=0;      %坐标下限（相对于系统坐标原点，单位mm）
start_y=0;
start_z=0;
end_x=80;      %坐标上限（相对于系统坐标原点，单位mm）
end_y=80;
end_z=80;

% 数据密化参数
min_step = 2*pi/100;            %密化步长，单位mm

% 圆参数
R = 40;
pitch = pi/4;
theta = 0: min_step :2*pi-min_step;

Ori_x= 0;
Ori_y= 0;
Ori_z= 0;

% 密化
x = R*cos(pitch)*cos(1*pi - theta) + Ori_x;
y = R*cos(pitch)*sin(1*pi - theta) + Ori_y;
z = x*tan(pitch) + Ori_z;
plot3(x,y,z);

% 转成脉冲形式：
px = x*step_x;
py = y*step_y;
pz = z*step_z;
[vx vy vz t] = get_pt_data3( px, py, pz, step_v); % get pulse_vel

%整形
x_int = int32(px);
y_int = int32(py);
z_int = int32(pz);
vx_int = int32(vx);
vy_int = int32(vy);
vz_int = int32(vz);
t_int = int32(t);

% 存数据
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








