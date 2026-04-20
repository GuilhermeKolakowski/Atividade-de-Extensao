clc
clear
close all

%% Dados
V    = [5.6, 7.7, 11.7];
m30  = [590.87, 862.93, 1338.27];
mdot = m30 / 30;

%% ================================
%  AJUSTE LINEAR:  mdot = a*V + b
%% ================================
p1 = polyfit(V, mdot, 1);
mdot_lin = polyval(p1, V);
R2_lin = 1 - sum((mdot - mdot_lin).^2) / sum((mdot - mean(mdot)).^2);

%% ================================
%  INVERSÃO DO MODELO LINEAR
%  V = (mdot - b) / a
%% ================================
V_de_mdot = @(md) (md - p1(2)) / p1(1);

%% ================================
%  GRÁFICO
%% ================================
Vfit         = linspace(4, 13, 200);
mdot_lin_fit = polyval(p1, Vfit);

figure;
hold on;
plot(V, mdot, 'ko', 'MarkerSize', 10, 'LineWidth', 2, 'DisplayName', 'Dados coletados');
plot(Vfit, mdot_lin_fit, 'b--', 'LineWidth', 2, ...
    'DisplayName', sprintf('Linear: y=%.3fV%+.3f  |  R²=%.4f', p1(1), p1(2), R2_lin));
grid on;
xlabel('Tensão (V)');
ylabel('Vazão (g/s)');
title('Caracterização da Vazão em função da Tensão');
legend('Location', 'northwest');
hold off;

%% ================================
%  TABELA V → Vazão
%% ================================
V_tabela    = (5:0.5:12)';
mdot_tabela = polyval(p1, V_tabela);

fprintf('===== TABELA LINEAR (referência) =====\n');
fprintf('%-12s %-12s\n', 'Tensão (V)', 'Vazão (g/s)');
fprintf('%-12.1f %-12.2f\n', [V_tabela, mdot_tabela]');

%% ================================
%  RESULTADOS NO CONSOLE
%% ================================
fprintf('\n===== MODELO LINEAR =====\n');
fprintf('mdot = %.4f*V + %.4f\n', p1(1), p1(2));
fprintf('R² = %.4f\n\n', R2_lin);

fprintf('===== INVERSÃO DO MODELO =====\n');
fprintf('V = (mdot - %.4f) / %.4f\n', p1(2), p1(1));