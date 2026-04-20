% =========================
% DADOS DO WORKSPACE
% =========================
% tempo -> vetor de tempo
% velocidade -> vetor de velocidade

t = tempo;
v = velocidade;

% =========================
% 1. VALOR INICIAL
% =========================
vi = mean(v(1:50));   % média dos primeiros pontos

% =========================
% 2. VALOR FINAL
% =========================
vf = mean(v(end-200:end)); % média dos últimos pontos

% =========================
% 3. VALOR DE 63.2%
% =========================
v_tau = vi + 0.632*(vf - vi);

% =========================
% 4. ENCONTRAR TAU
% =========================
% pega o ponto mais próximo de 63.2%
[~, idx] = min(abs(v - v_tau));

tau = t(idx);

fprintf('Tau = %.4f segundos\n', tau);

% =========================
% 5. PLOT
% =========================
figure
plot(t, v, 'b'); hold on

yline(v_tau, '--r', '63.2%');
xline(tau, '--g', 'Tau');

xlabel('Tempo (s)')
ylabel('Velocidade')
legend('Sinal original', '63.2%', 'Tau')

grid on