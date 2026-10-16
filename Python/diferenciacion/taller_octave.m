% =========================================================================
% Métodos Computacionales en Ingeniería - Taller Diferenciación Numérica
% Parte 1: GNU Octave
% =========================================================================

clear; clc; close all;

% 1. Carga de datos desde el CSV (saltando el encabezado)
data = dlmread('trayectoria_robot.csv', ',', 1, 0);
t = data(:, 1);
x = data(:, 2);
y = data(:, 3);

N = length(t);
h = t(2) - t(1); % Paso temporal h = 0.2 s

% 2. Cálculo de derivadas de posición (vx, vy) usando diferencias centrales
vx = zeros(N, 1);
vy = zeros(N, 1);

% Puntos interiores (diferencia centrada)
vx(2:N-1) = (x(3:N) - x(1:N-2)) / (2 * h);
vy(2:N-1) = (y(3:N) - y(1:N-2)) / (2 * h);

% Extremos (diferencia hacia adelante/atrás)
vx(1) = (x(2) - x(1)) / h;
vx(N) = (x(N) - x(N-1)) / h;
vy(1) = (y(2) - y(1)) / h;
vy(N) = (y(N) - y(N-1)) / h;

% 3. Velocidad lineal v y Orientación theta
v = sqrt(vx.^2 + vy.^2);
theta = atan2(vy, vx);

% Desenvolvimiento angular para evitar saltos discontinuos
theta = unwrap(theta);

% 4. Velocidad angular omega con diferencias centrales sobre theta
omega = zeros(N, 1);
omega(2:N-1) = (theta(3:N) - theta(1:N-2)) / (2 * h);
omega(1) = (theta(2) - theta(1)) / h;
omega(N) = (theta(N) - theta(N-1)) / h;

% 5. Generación de Gráficas
figure('Name', 'Resultados Cinemática en GNU Octave', 'NumberTitle', 'off');

subplot(2, 2, 1);
plot(x, y, 'b-', 'LineWidth', 1.8);
title('Trayectoria del Robot (y vs x)');
xlabel('x [m]'); ylabel('y [m]'); grid on;

subplot(2, 2, 2);
plot(t, v, 'r-', 'LineWidth', 1.8);
title('Velocidad lineal v(t)');
xlabel('t [s]'); ylabel('v [m/s]'); grid on;

subplot(2, 2, 3);
plot(t, theta, 'g-', 'LineWidth', 1.8);
title('Orientación \theta(t)');
xlabel('t [s]'); ylabel('\theta [rad]'); grid on;

subplot(2, 2, 4);
plot(t, omega, 'm-', 'LineWidth', 1.8);
title('Velocidad angular \omega(t)');
xlabel('t [s]'); ylabel('\omega [rad/s]'); grid on;
