% Kevin Moffatt u0987649
% ME 3230 Motor Torque Curves
% 2/3/18
clc, clear, close all

%% Motor Torque Curves
% matrix for each motor with columns gear ratio (x:1), no load speed (RPM), and torque (oz-in) 
MotorMatrix = [1 11000 5; 19 500 84; 30 350 110; 50 200 170; 70 150 200; 100 100 220; 131 80 250];
% plot torque vs motor speed
figure;
for i = 1:length(MotorMatrix)
    plot([0,MotorMatrix(i,2)], [MotorMatrix(i,3),0]);
    hold on;
end
xlabel('No Load Speed @ 12V (RPM) [\omega]');
ylabel('Torque (oz-in) [\tau]');
xlim([0 600]);
ylim([0 300]);
grid minor;
% create legend
legendCell = cellstr(num2str(MotorMatrix(:,1), '%d:1'));
legend(legendCell);
hold off;

% create modified matrix for torque (Nm) vs linear speed plot (assuming 100mm diameter tires)
MotorMatrixMod = MotorMatrix;   % columns gear ratio (x:1), no load linear speed (m/s), and torque (Nm)
MotorMatrixMod(:,3) = 0.00706155*MotorMatrixMod(:,3);  % convert oz-in to Nm
MotorMatrixMod(:,2) = pi*.05/60*MotorMatrixMod(:,2);    % convert RPM to linear m/s
figure;
for j = 1:length(MotorMatrixMod)
    plot([0,MotorMatrixMod(j,2)], [MotorMatrixMod(j,3),0]);
    hold on;
end
xlabel('No Load Linear Speed @ 12V (m/s) [v] (100mm wheels)');
ylabel('Torque (Nm) [\tau]');
xlim([0 1.5]);
ylim([0 2]);
grid minor;
legend(legendCell);
hold off;