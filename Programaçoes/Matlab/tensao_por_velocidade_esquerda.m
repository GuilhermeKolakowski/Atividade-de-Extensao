%% Dados - Motor Direita
valor_serial = [0, 156, 160, 164, 168, 172, 176, 180, 184]';
tensao       = [0, 9.2, 8.2, 7.2, 6.1, 5.1, 4.0, 2.9, 1.8]';
rpm_real     = [0, 51.4, 45.5, 37.6, 34.8, 27.5, 21.2, 15.0, 8.3]';
rpm_lido     = [0, 52.1, 43.9, 38.1, 32.8, 26.8, 20.8, 14.8, 7.9]';

%% ============================================================
%% PARTE 1: RPM Real vs RPM Lido (validação do encoder)
%% ============================================================
figure(1);
hold on;
scatter(rpm_real, rpm_lido, 60, 'b', 'filled', 'DisplayName', 'Dados medidos');

p1 = polyfit(rpm_real, rpm_lido, 1);
x_fit = linspace(min(rpm_real), max(rpm_real), 100);
y_fit = polyval(p1, x_fit);
plot(x_fit, y_fit, 'r-', 'LineWidth', 2, 'DisplayName', ...
    sprintf('Regressão: y = %.4fx + %.4f', p1(1), p1(2)));

plot(x_fit, x_fit, 'k--', 'LineWidth', 1.5, 'DisplayName', 'Ideal (y = x)');

rpm_lido_pred = polyval(p1, rpm_real);
ss_res = sum((rpm_lido - rpm_lido_pred).^2);
ss_tot = sum((rpm_lido - mean(rpm_lido)).^2);
R2_encoder = 1 - ss_res/ss_tot;

xlabel('RPM Real'); ylabel('RPM Lido (Encoder)');
title(sprintf('RPM Real vs RPM Lido | R² = %.5f', R2_encoder));
legend('Location', 'northwest');
grid on; hold off;

fprintf('=== PARTE 1: Encoder ===\n');
fprintf('Equação: RPM_Lido = %.4f * RPM_Real + %.4f\n', p1(1), p1(2));
fprintf('Fator de correção sugerido: %.4f\n', 1/p1(1));
fprintf('R² = %.6f\n\n', R2_encoder);

%% ============================================================
%% PARTE 2: Tensão vs RPM Real e RPM Lido
%% ============================================================
figure(2);
hold on;

scatter(tensao, rpm_real, 60, 'b', 'filled', 'DisplayName', 'RPM Real');
scatter(tensao, rpm_lido, 60, 'r', 'filled', 'DisplayName', 'RPM Lido');

p2_real = polyfit(tensao, rpm_real, 1);
x_fit2  = linspace(min(tensao), max(tensao), 100);
plot(x_fit2, polyval(p2_real, x_fit2), 'b-', 'LineWidth', 2, ...
    'DisplayName', sprintf('Reg. Real: y = %.4fx + %.4f', p2_real(1), p2_real(2)));

p2_lido = polyfit(tensao, rpm_lido, 1);
plot(x_fit2, polyval(p2_lido, x_fit2), 'r-', 'LineWidth', 2, ...
    'DisplayName', sprintf('Reg. Lido: y = %.4fx + %.4f', p2_lido(1), p2_lido(2)));

pred_real = polyval(p2_real, tensao);
pred_lido = polyval(p2_lido, tensao);
R2_real = 1 - sum((rpm_real - pred_real).^2) / sum((rpm_real - mean(rpm_real)).^2);
R2_lido = 1 - sum((rpm_lido - pred_lido).^2) / sum((rpm_lido - mean(rpm_lido)).^2);

xlabel('Tensão (V)'); ylabel('RPM');
title('Tensão vs RPM Real e RPM Lido');
legend('Location', 'northwest');
grid on; hold off;

fprintf('=== PARTE 2: Tensão → RPM ===\n');
fprintf('RPM_Real = %.4f * Tensão + %.4f  |  R² = %.6f\n', p2_real(1), p2_real(2), R2_real);
fprintf('RPM_Lido = %.4f * Tensão + %.4f  |  R² = %.6f\n', p2_lido(1), p2_lido(2), R2_lido);

%% ============================================================
%% PARTE 3: Erro percentual encoder ponto a ponto
%% ============================================================
figure(3);
erro_pct = zeros(size(rpm_real));
idx = rpm_real ~= 0;
erro_pct(idx) = ((rpm_lido(idx) - rpm_real(idx)) ./ rpm_real(idx)) * 100;

bar(erro_pct, 'FaceColor', [0.9 0.4 0.2]);
set(gca, 'XTick', 1:length(valor_serial));
set(gca, 'XTickLabel', arrayfun(@num2str, valor_serial, 'UniformOutput', false));

xlabel('Valor Serial'); ylabel('Erro (%)');
title('Erro percentual do Encoder em relação ao RPM Real');
grid on;
yline(0, 'k--', 'LineWidth', 1.5);

fprintf('\n=== PARTE 3: Erro do Encoder ===\n');
fprintf('Erro médio absoluto: %.4f%%\n', mean(abs(erro_pct(idx))));
fprintf('Erro máximo: %.4f%%\n', max(abs(erro_pct(idx))));