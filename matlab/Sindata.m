close all
clear;
% ���/��е�ṹ��������ʱδʹ�ã�
step_x=(2*6400)/100;   %���������������λmm
step_y=step_x;
step_z=step_x;

step_v = 6400;   % pulse/second

% ��ͼ�ߴ緶Χ���ƣ���ʱδʹ�ã�
start_x=0;      %�������ޣ������ϵͳ����ԭ�㣬��λmm��
start_y=10;
start_z=0;
end_x=100;      %�������ޣ������ϵͳ����ԭ�㣬��λmm��
end_y=70;
end_z=0;

% ���Һ���y=A*sin(B*x+C)+D��������ԭ�����ꡢxȡֵ��Χ
A=40;
B=2*pi/80;
C=0;
D=0;
Ori_x=0;
Ori_y=0;
Ori_z=0;
xmin=0;
xmax=100;

% �����ܻ�����
delta_x=1;            %�ܻ���������λmm

% ��ʼ����
L_x=end_x-start_x;
L_y=end_y-start_y;
L_z=end_z-start_z;
N=L_x/delta_x;

% �ܻ�
x=xmin:delta_x:xmax;
y=A*sin(B*x+C)+D;
z=zeros(1,N+1);

% ת������еϵͳ����
x=(x+Ori_x)*step_x;
y=(y+Ori_y)*step_x;
z=(z+Ori_z)*step_x;

%�����ٶ�
vx = x;
vx(2:end) = (x(2:end) - x(1:end-1));
vy = y;
vy(2:end) = (y(2:end) - y(1:end-1));

vy = vy/max(vy)*step_v/1.4142;
vx = vx/max(vx)*step_v/1.4142;
v  = ((vx.^2+vy.^2).^0.5);

%����
x_int = int32(x);
y_int = int32(y);
z_int = int32(z);
vx_int = int32(vx);
vy_int = int32(vy);

% ������
SineData=[x_int',y_int',vx_int',vy_int'];
save('D:/data.txt','SineData','-ascii','-double');
plot(x_int',y_int');
