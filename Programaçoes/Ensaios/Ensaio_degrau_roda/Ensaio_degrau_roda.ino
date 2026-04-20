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

// =========================
// VARIÁVEIS ENCODER
// =========================
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

long lastEncoder1 = 0;
long lastEncoder2 = 0;

// =========================
// CONTROLE MOTOR
// =========================
int velocidade_motor1 = 64;
int velocidade_motor2 = 190;

// =========================
// ENSAIO
// =========================
#define TEMPO_ENSAIO 20000  // ms

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

    // lê linha completa
    String comando = Serial.readStringUntil('\n');
    comando.trim();  // remove \r e espaços

    int valor = comando.toInt();

    // ignora lixo / enter vazio
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

  // reset encoders
  noInterrupts();
  encoder1Count = 0;
  encoder2Count = 0;
  interrupts();

  lastEncoder1 = 0;
  lastEncoder2 = 0;

  unsigned long tempoInicio = millis();
  unsigned long lastSample = millis();

  Serial.println("tempo(ms),rpm1,rpm2");

  // ===== LOOP DO ENSAIO =====
  while (millis() - tempoInicio < TEMPO_ENSAIO) {

    // envia velocidade continuamente
    Serial3.write(velocidade_motor1);
    Serial3.write(velocidade_motor2);

    // amostragem a cada 10 ms
    if (millis() - lastSample >= 10) {

      noInterrupts();
      long enc1 = encoder1Count;
      long enc2 = encoder2Count;
      interrupts();

      long delta1 = enc1 - lastEncoder1;
      long delta2 = enc2 - lastEncoder2;

      float rpm1 = -((delta1 * 6000.0) / PPR) / 32;
      float rpm2 = -((delta2 * 6000.0) / PPR) / 32;

      unsigned long tempo = millis() - tempoInicio;

      Serial.print(tempo);
      Serial.print(",");
      Serial.print(rpm1);
      Serial.print(",");
      Serial.println(rpm2);

      lastEncoder1 = enc1;
      lastEncoder2 = enc2;

      lastSample = millis();
    }
  }

  // ===== PARA MOTORES =====
  velocidade_motor1 = 64;
  velocidade_motor2 = 190;

  Serial3.write(velocidade_motor1);
  Serial3.write(velocidade_motor2);

  Serial.println("Ensaio finalizado");
}