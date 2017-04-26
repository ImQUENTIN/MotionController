close all
clear;
% 电机/机械结构参数（暂时未使用）
step_x=(2*6400)/100;   %步进电机步长，单位mm
step_y=step_x;
step_z=step_x;

step_v = 6400;   % pulse/second

% 绘图尺寸范围限制（暂时未使用）
start_x=0;      %坐标下限（相对于系统坐标原点，单位mm）
start_y=10;
start_z=0;
end_x=100;      %坐标上限（相对于系统坐标原点，单位mm）
end_y=70;
end_z=0;

% 正弦函数y=A*sin(B*x+C)+D参数及其原点坐标、x取值范围
A=40;
B=2*pi/80;
C=0;
D=0;
Ori_x=0;
Ori_y=0;
Ori_z=0;
xmin=0;
xmax=100;

% 数据密化参数
delta_x=1;            %密化步长，单位mm

% 初始化；
L_x=end_x-start_x;
L_y=end_y-start_y;
L_z=end_z-start_z;
N=L_x/delta_x;

% 密化
x=xmin:delta_x:xmax;
y=A*sin(B*x+C)+D;
z=zeros(1,N+1);

% 转换到机械系统坐标
x=(x+Ori_x)*step_x;
y=(y+Ori_y)*step_x;
z=(z+Ori_z)*step_x;

%计算速度
vx = x;
vx(2:end) = (x(2:end) - x(1:end-1));
vy = y;
vy(2:end) = (y(2:end) - y(1:end-1));

vy = vy/max(vy)*step_v/1.4142;
vx = vx/max(vx)*step_v/1.4142;
v  = ((vx.^2+vy.^2).^0.5);

%整形
x_int = int32(x);
y_int = int32(y);
z_int = int32(z);
vx_int = int32(vx);
vy_int = int32(vy);

% 存数据
SineData=[x_int',y_int',vx_int',vy_int'];
save('D:/data.txt','SineData','-ascii','-double');
plot(x_int',y_int');
