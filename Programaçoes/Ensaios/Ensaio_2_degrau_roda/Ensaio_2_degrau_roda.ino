// =========================
// PINOS ENCODER
// =========================
#define ENC1_A 21
#define ENC1_B 20

#define ENC2_A 19
#define ENC2_B 18

// =========================
// PARÂMETROS
// =========================
#define PPR 600
#define REDUCAO 32

// =========================
// VARIÁVEIS ENCODER
// =========================
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

long lastEncoder1 = 0;
long lastEncoder2 = 0;

// =========================
// FILTRO
// =========================
float rpm1_filtrado = 0;
float rpm2_filtrado = 0;

#define ALPHA 0.286

// =========================
// CONTROLE MOTOR
// =========================
int velocidade_motor1 = 64;
int velocidade_motor2 = 192;

// =========================
// ENSAIO
// =========================
#define TEMPO_ENSAIO 20000
#define SAMPLE_TIME 10      

// =========================
// INTERRUPÇÕES
// =========================
void encoder1ISR() {
  if (digitalRead(ENC1_B)) encoder1Count++;
  else encoder1Count--;
}

void encoder2ISR() {
  if (digitalRead(ENC2_B)) encoder2Count++;
  else encoder2Count--;
}

// =========================
// SETUP
// =========================
void setup() {

  Serial.begin(115200);
  Serial3.begin(9600);

  delay(3000);

  pinMode(ENC1_A, INPUT_PULLUP);
  pinMode(ENC1_B, INPUT_PULLUP);
  pinMode(ENC2_A, INPUT_PULLUP);
  pinMode(ENC2_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC1_A), encoder1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), encoder2ISR, FALLING);

  Serial.println("Digite valor para iniciar ensaio:");
}

// =========================
// LOOP PRINCIPAL
// =========================
void loop() {

  if (Serial.available()) {

    String comando = Serial.readStringUntil('\n');
    comando.trim();

    int valor = comando.toInt();

    if (valor == 0) return;

    if (valor >= 1 && valor <= 127) {
      velocidade_motor1 = valor;
    }

    if (valor >= 128 && valor <= 255) {
      velocidade_motor2 = valor;
    }

    executarEnsaio();

    Serial.println("Aguardando novo valor...");
  }
}

// =========================
// FUNÇÃO DE ENSAIO
// =========================
void executarEnsaio() {

  noInterrupts();
  encoder1Count = 0;
  encoder2Count = 0;
  interrupts();

  lastEncoder1 = 0;
  lastEncoder2 = 0;

  rpm1_filtrado = 0;
  rpm2_filtrado = 0;

  unsigned long tempoInicio = millis();
  unsigned long lastSample = millis();

  Serial.println("tempo(ms),rpm1,rpm2");

  while (millis() - tempoInicio < TEMPO_ENSAIO) {

    // mantém comando do motor
    Serial3.write(velocidade_motor1);
    Serial3.write(velocidade_motor2);

    if (millis() - lastSample >= SAMPLE_TIME) {

      // ===== LEITURA SEGURA =====
      noInterrupts();
      long enc1 = encoder1Count;
      long enc2 = encoder2Count;
      interrupts();

      long delta1 = enc1 - lastEncoder1;
      long delta2 = enc2 - lastEncoder2;

      // ===== CÁLCULO RPM =====
      float fator = 60.0 / (SAMPLE_TIME / 1000.0);

      float rpm1 = ((delta1 * fator) / PPR) / REDUCAO;
      float rpm2 = -((delta2 * fator) / PPR) / REDUCAO;

      // ===== FILTRO =====
      rpm1_filtrado = ALPHA * rpm1 + (1 - ALPHA) * rpm1_filtrado;
      rpm2_filtrado = ALPHA * rpm2 + (1 - ALPHA) * rpm2_filtrado;

      // ===== SAÍDA =====
      unsigned long tempo = millis() - tempoInicio;

      Serial.print(tempo);
      Serial.print(",");
      Serial.print(rpm1_filtrado);
      Serial.print(",");
      Serial.println(rpm2_filtrado);

      lastEncoder1 = enc1;
      lastEncoder2 = enc2;

      // ===== ATUALIZA TEMPO =====
      lastSample += SAMPLE_TIME;
    }
  }

  // ===== PARA MOTORES =====
  velocidade_motor1 = 64;
  velocidade_motor2 = 192;

  Serial3.write(velocidade_motor1);
  Serial3.write(velocidade_motor2);

  Serial.println("Ensaio finalizado");
}