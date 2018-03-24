% Group 6.2 - Dorien Butter, Kevin Moffatt
% Brett Reeder and Kohl Schoensee
% ME 3230 PM 10 Code
% 3/30/18
clc, clear, close all

%% Gear Calculation Table

% Motor info
gearRatios = [50; 70; 100; 131];    % gear ratios in format n:1
wNL = [200; 150; 100; 80];     % no load angular speed (RPM)
Tstall = [170; 200; 220; 250];  % stall torque (oz-in)

% matrix for each motor with columns gear ratio (x:1), no load speed (RPM), and stall torque (oz-in) 
MotorMatrix = [gearRatios, Tstall, wNL];
MotorMatrix_9V = 9/12*MotorMatrix;  % motor characteristics at 9V instead of 12V
MotorMatrix_9V(:,1) = gearRatios;   % gear ratios remain constant
wNL9 = wNL*9/12;
Tstall9 = Tstall*9/12;

% Max force and speed at 9V
wheelDiameter = 84;     % wheel diameter (mm)
wDiameterIN = wheelDiameter/25.4; % wheel diameter (in)
wheelRadius = wheelDiameter/2;  % wheel radius (mm)
wRadiusIN = wheelRadius/25.4;   % wheel radius (in)
maxForce9 = 2*Tstall9/wRadiusIN;     % max robot force at 9V (2 wheels) (oz)
maxSpeed9 = wNL9*wDiameterIN*pi/60;   % max speed at 9V (in/s) (RPM*Circumference/60)

% Force and speed at max power at 9V
wMaxP_9V = .5*wNL9; % angular velocity at max power (RPM)
TmaxP_9V = .5*Tstall9;   % stall torque at max power (oz-in)
forceMaxP = 2*TmaxP_9V/wRadiusIN;   % max robot force at max power (2 wheels) (oz)
speedMaxP = wMaxP_9V.*wDiameterIN*pi/60;   % speed at 9V max power in (in/s)

% gear calculations table (see lab 10 handout)
columnStrings = {'Gear_Ratios','Tstall_12V_oz_in','wNL_12V_RPM','Tstall_9V_oz_in','wNL_9V_RPM','MaxForce_9V_oz','MaxSpeed_9V_in_per_s','ForceMaxPower_oz','SpeedMaxPower_in_per_s'};
gearRatiosStrings = {'50:1';'70:1';'100:1';'131:1'};
MotorCalcs = table(gearRatiosStrings,Tstall,wNL, Tstall9, wNL9, maxForce9, maxSpeed9, forceMaxP, speedMaxP,'VariableNames',columnStrings); 
display(MotorCalcs);
% writetable(MotorCalcs,'MotorCalcs.xlsx','Range','A2','WriteVariableNames',false);

%% Torque Curves (for visualization)

% torque (oz-in) vs w (RPM)
figure;
for i = 1:length(MotorMatrix_9V)
    plot([0,MotorMatrix_9V(i,3)], [MotorMatrix_9V(i,2),0]);
    hold on;
end
title('Torque vs Speed Curves @ 9V');
xlabel('Angular Speed (RPM) [\omega]');
ylabel('Torque (oz-in) [\tau]');
grid minor;
% create legend
legendCell = cellstr(num2str(MotorMatrix_9V(:,1), '%d:1'));
legend(legendCell);
hold off;

% create modified matrix for Force (oz) vs linear speed (in/s) plot (assuming 84mm diameter wheels)
MotorMatrixMod = MotorMatrix_9V;   % columns gear ratio (x:1), no load linear speed (in/s), and force (oz)
MotorMatrixMod(:,2) = forceMaxP;    % torque (oz-in) converted to force (oz) with 84mm diametter wheels
MotorMatrixMod(:,3) = speedMaxP;    % w_nl converted to in/s
figure;
for j = 1:length(MotorMatrixMod)
    plot([0,MotorMatrixMod(j,3)], [MotorMatrixMod(j,2),0]);
    hold on;
end
title('Force vs Linear Speed Curves @ 9V');
xlabel('Linear Speed (in/s) [v] (84mm wheels)');
ylabel('Force (oz) [F]');
grid minor;
legend(legendCell);
hold off;