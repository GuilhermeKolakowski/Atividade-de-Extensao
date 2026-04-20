clc
clear
close all

% Tensao aplicada (V)
V = [5.6 7.7 11.7];

% Massa media dosada em 30s (g) - valores corrigidos
m30 = [590.87 862.93 1338.27];

% Conversao para (g/s)
mdot = m30 / 30;

%% ================================
%  AJUSTE LINEAR
%  mdot = aV + b
%  ================================

p1 = polyfit(V, mdot, 1);
mdot_lin = polyval(p1, V);

% Calculo R² linear
R2_lin = 1 - sum((mdot - mdot_lin).^2) / sum((mdot - mean(mdot)).^2);

%% ================================
%  AJUSTE QUADRATICO
%  mdot = aV² + bV + c
%  ================================

p2 = polyfit(V, mdot, 2);
mdot_quad = polyval(p2, V);

% Calculo R² quadratico
R2_quad = 1 - sum((mdot - mdot_quad).^2) / sum((mdot - mean(mdot)).^2);

%% ================================
%  GRAFICO
%  ================================

Vfit = linspace(5, 12, 200);

mdot_lin_fit  = polyval(p1, Vfit);
mdot_quad_fit = polyval(p2, Vfit);

figure
plot(V, mdot, 'o', 'LineWidth', 2)
hold on
plot(Vfit, mdot_lin_fit, '--', 'LineWidth', 2)


grid on
xlabel('Tensão (V)')
ylabel('Vazão (g/s)')
title('Caracterização da Vazão em função da Tensão')
legend('Dados coletados','Ajuste Linear','Location','northwest')

%% ================================
%  RESULTADOS NO CONSOLE
%  ================================

disp('===== MODELO LINEAR =====')
fprintf('mdot = %.4f*V + %.4f\n', p1(1), p1(2))
fprintf('R² linear = %.4f\n\n', R2_lin)

