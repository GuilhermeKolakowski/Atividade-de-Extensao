%% =========================
% DADOS (já devem existir no workspace)
% Tempo (s), rpm, tensao
%% =========================

% Garantir vetores coluna
Tempo = Tempo(:);
rpm = rpm(:);

%% =========================
% 1. Remover outliers
%% =========================
rpm_clean = filloutliers(rpm, 'linear');

%% =========================
% 2. Suavização (média móvel)
%% =========================
janela = 30; % ajuste entre 20 e 100
rpm_mov = movmean(rpm_clean, janela);

%% =========================
% 3. Filtro passa-baixa (Butterworth)
%% =========================
Ts = mean(diff(Tempo));   % tempo de amostragem
fs = 1/Ts;                % frequência de amostragem

fc = 1.5; % frequência de corte (Hz) -> ajuste conforme sistema

[b,a] = butter(2, fc/(fs/2));
rpm_filt = filtfilt(b,a,rpm_mov);

%% =========================
% 4. (Opcional) Downsample
%% =========================
fator = 2; % pode testar 2, 5, etc.
rpm_final = downsample(rpm_filt, fator);
Tempo_final = downsample(Tempo, fator);

%% =========================
% 5. Plot comparação
%% =========================
figure
plot(Tempo, rpm, ':', 'DisplayName', 'Original')
hold on
plot(Tempo, rpm_clean, '--', 'DisplayName', 'Sem outlier')
plot(Tempo, rpm_mov, 'DisplayName', 'Média móvel')
plot(Tempo_final, rpm_final, 'LineWidth', 2, 'DisplayName', 'Final filtrado')

xlabel('Tempo (s)')
ylabel('RPM')
title('Processamento do sinal de RPM')
legend
grid on
set(gca, 'FontSize', 12)

%% =========================
% 6. (Opcional) Recorte útil
%% =========================
% Exemplo: ignorar primeiros segundos
idx = Tempo_final > 0.5;

Tempo_id = Tempo_final(idx);
rpm_id = rpm_final(idx);

%% =========================
% 7. Preparar para identificação
%% =========================
% Se tiver tensao:
% tensao = tensao(:);
% tensao_final = downsample(tensao, fator);
% tensao_id = tensao_final(idx);

% Ts novo após downsample
Ts_id = mean(diff(Tempo_id));

% Criar objeto iddata
% data = iddata(rpm_id, tensao_id, Ts_id);

%% =========================
% FIM
%% =========================