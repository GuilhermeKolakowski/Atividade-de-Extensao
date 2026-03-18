clc
clear
close all

%% ================================
% DADOS EXPERIMENTAIS
% ================================

rpm_real = [63.1 84.6 105.8 129.9 60.4 83.2 111.3 135.5];
rpm_lido = [64.2 86.1 106.4 132.6 65.2 85.7 114.6 141.1];

%% ================================
% AJUSTE LINEAR
% rpm_lido = a*rpm_real + b
% ================================

p = polyfit(rpm_real, rpm_lido, 1);
rpm_fit = polyval(p, rpm_real);

% R²
R2 = 1 - sum((rpm_lido - rpm_fit).^2) / sum((rpm_lido - mean(rpm_lido)).^2);

%% ================================
% ERRO
% ================================

erro = rpm_lido - rpm_real;
erro_percentual = (erro ./ rpm_real) * 100;

erro_medio = mean(abs(erro_percentual));

%% ================================
% GRAFICO
% ================================

x = linspace(min(rpm_real)-5, max(rpm_real)+5, 200);
y_fit = polyval(p, x);

figure
plot(rpm_real, rpm_lido,'o','LineWidth',2)
hold on

% linha de ajuste
plot(x, y_fit,'--','LineWidth',2)

% linha ideal
plot(x, x,':','LineWidth',2)

grid on
xlabel('RPM Real')
ylabel('RPM Lido (Encoder)')
title('Comparação RPM Real vs RPM Encoder')

legend('Dados experimentais','Ajuste linear','Linha ideal','Location','northwest')

%% ================================
% RESULTADOS
% ================================

disp('===== MODELO DE CORREÇÃO ESQUERDA=====')
fprintf('RPM_lido = %.4f * RPM_real + %.4f\n', p(1), p(2))
fprintf('R² = %.4f\n\n', R2)

disp('===== ERRO =====')
fprintf('Erro médio percentual = %.2f %%\n', erro_medio)