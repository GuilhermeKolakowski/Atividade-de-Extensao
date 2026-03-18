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
#define PPR 600   // pulsos por volta do encoder

// =========================
// VARIÁVEIS ENCODER
// =========================
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

long lastEncoder1 = 0;
long lastEncoder2 = 0;

unsigned long lastTime = 0;

// =========================
// CONTROLE MOTOR
// =========================
int velocidade_motor1 = 64;   // stop inicial
int velocidade_motor2 = 192;  // stop inicial

// =========================
// INTERRUPÇÕES
// =========================
void encoder1ISR() {
  if (digitalRead(ENC1_B) == HIGH) {
    encoder1Count++;
  } else {
    encoder1Count--;
  }
}

void encoder2ISR() {
  if (digitalRead(ENC2_B) == HIGH) {
    encoder2Count++;
  } else {
    encoder2Count--;
  }
}

// =========================
// SETUP
// =========================
void setup() {

  Serial.begin(115200);
  Serial3.begin(9600);

  delay(3000); // inicialização sabertooth

  pinMode(ENC1_A, INPUT_PULLUP);
  pinMode(ENC1_B, INPUT_PULLUP);

  pinMode(ENC2_A, INPUT_PULLUP);
  pinMode(ENC2_B, INPUT_PULLUP);

  // INTERRUPÇÃO NA BORDA DE DESCIDA
  attachInterrupt(digitalPinToInterrupt(ENC1_A), encoder1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), encoder2ISR, FALLING);

  lastTime = millis();

  Serial.println("Sistema iniciado");
}

// =========================
// LOOP
// =========================
void loop() {

  // ===== ENVIA VELOCIDADE PARA SABERTOOTH =====
  Serial3.write(velocidade_motor1);
  Serial3.write(velocidade_motor2);

  // ===== CÁLCULO RPM =====
  if (millis() - lastTime >= 100) {

    noInterrupts();
    long enc1 = encoder1Count;
    long enc2 = encoder2Count;
    interrupts();

    long delta1 = enc1 - lastEncoder1;
    long delta2 = enc2 - lastEncoder2;

    float rpm1 = -((delta1 * 600.0) / PPR) / 32;
    float rpm2 = -((delta2 * 600.0) / PPR) / 32;

    Serial.print("RPM Motor 1: ");
    Serial.print(rpm1);

    Serial.print(" | RPM Motor 2: ");
    Serial.println(rpm2);

    lastEncoder1 = enc1;
    lastEncoder2 = enc2;

    lastTime = millis();
  }

  // ===== CONTROLE POR SERIAL =====
  if (Serial.available()) {

    int valor = Serial.parseInt();

    // motor 1
    if (valor >= 1 && valor <= 127) {
      velocidade_motor1 = valor;
    }

    // motor 2
    if (valor >= 128 && valor <= 255) {
      velocidade_motor2 = valor;
    }
  }
}