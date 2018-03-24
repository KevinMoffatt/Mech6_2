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
wNL9 = wNL*9/12;
Tstall9 = Tstall*9/12;

% Max force and speed at 9V
wheelDiameter = 84;     % wheel diameter (mm)
wDiameterIN = wheelDiameter/25.; % wheel diameter (in)
wheelRadius = wheelDiameter/2;  % wheel radius (mm)
wRadiusIN = wheelRadius/25.4;   % wheel radius (in)
maxForce9 = 2*Tstall9/wRadiusIN;     % max robot force at 9V (2 wheels) (oz)
maxSpeed9 = wNL9*wDiameterIN*pi/60;   % max speed at 9V (in/s) (RPM*Circumference/60)

% Force and speed at max power at 9V
wMaxP_9V = .5*wNL9; % angular velocity at max power (RPM)
TmaxP_9V = .5*Tstall;   % stall torque at max power (oz-in)
forceMaxP = 2*TmaxP_9V/wRadiusIN;   % max robot force at max power (2 wheels) (oz)
speedMaxP = wMaxP_9V.*wDiameterIN*pi/60;   % speed at 9V max power in (in/s)

% gear calculations table (see lab 10 handout)
columnStrings = {'Gear_Ratios','Tstall_12V_oz_in','wNL_12V_RPM','Tstall_9V_oz_in','wNL_9V_RPM','MaxForce_9V_oz','MaxSpeed_9V_in_per_s','ForceMaxPower_oz','SpeedMaxPower_in_per_s'};
gearRatiosStrings = {'50:1';'70:1';'100:1';'131:1'};
GearCalcs = table(gearRatiosStrings,Tstall,wNL, Tstall9, wNL9, maxForce9, maxSpeed9, forceMaxP, speedMaxP,'VariableNames',columnStrings); 
display(GearCalcs);
