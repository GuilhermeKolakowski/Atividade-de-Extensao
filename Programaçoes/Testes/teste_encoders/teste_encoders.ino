// =========================
// Definição dos pinos
// =========================
#define ENC1_A 21
#define ENC1_B 20

#define ENC2_A 19
#define ENC2_B 18

// =========================
// Variáveis globais
// =========================
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

// =========================
// Rotinas de interrupção
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
// Setup
// =========================
void setup() {
  Serial.begin(115200);

  pinMode(ENC1_A, INPUT_PULLUP);
  pinMode(ENC1_B, INPUT_PULLUP);

  pinMode(ENC2_A, INPUT_PULLUP);
  pinMode(ENC2_B, INPUT_PULLUP);

  // Interrupções nas fases A
  attachInterrupt(digitalPinToInterrupt(ENC1_A), encoder1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), encoder2ISR, RISING);

  Serial.println("Leitura de encoders iniciada");
}

// =========================
// Loop principal
// =========================
void loop() {
  static long lastEnc1 = 0;
  static long lastEnc2 = 0;

  noInterrupts();
  long enc1 = encoder1Count;
  long enc2 = encoder2Count;
  interrupts();

  if (enc1 != lastEnc1 || enc2 != lastEnc2) {
    Serial.print("Encoder 1: ");
    Serial.print(enc1);
    Serial.print(" | Encoder 2: ");
    Serial.println(enc2);

    lastEnc1 = enc1;
    lastEnc2 = enc2;
  }

  delay(100);
}
