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
#define DIAMETRO_RODA 0.32  // metros

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
int velocidade_motor1 = 64;   // neutro
int velocidade_motor2 = 192;  // neutro

// =========================
// TEMPO
// =========================
#define SAMPLE_TIME 10  // ms

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

  pinMode(ENC1_A, INPUT_PULLUP);
  pinMode(ENC1_B, INPUT_PULLUP);
  pinMode(ENC2_A, INPUT_PULLUP);
  pinMode(ENC2_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC1_A), encoder1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), encoder2ISR, FALLING);

  Serial.println("Digite um valor (0–255):");
}

// =========================
// LOOP
// =========================
void loop() {

  static unsigned long lastSample = millis();

  // ===== RECEBE COMANDO =====
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    int valor = comando.toInt();

    if (valor >= 0 && valor <= 127) {
      velocidade_motor1 = valor;
      velocidade_motor2 = 190; // mantém outro parado
    }
    else if (valor >= 128 && valor <= 255) {
      velocidade_motor2 = valor;
      velocidade_motor1 = 64;  // mantém outro parado
    }
  }

  // envia comando continuamente
  Serial3.write(velocidade_motor1);
  Serial3.write(velocidade_motor2);

  // ===== AMOSTRAGEM =====
  if (millis() - lastSample >= SAMPLE_TIME) {

    noInterrupts();
    long enc1 = encoder1Count;
    long enc2 = encoder2Count;
    interrupts();

    long delta1 = enc1 - lastEncoder1;
    long delta2 = enc2 - lastEncoder2;

    // ===== RPM =====
    float fator = 60.0 / (SAMPLE_TIME / 1000.0);

    float rpm1 = ((delta1 * fator) / PPR) / REDUCAO;
    float rpm2 = -((delta2 * fator) / PPR) / REDUCAO;

    // ===== FILTRO =====
    rpm1_filtrado = ALPHA * rpm1 + (1 - ALPHA) * rpm1_filtrado;
    rpm2_filtrado = ALPHA * rpm2 + (1 - ALPHA) * rpm2_filtrado;

    // ===== VELOCIDADE (m/s) =====
    float v1 = (rpm1_filtrado * PI * DIAMETRO_RODA) / 60.0;
    float v2 = (rpm2_filtrado * PI * DIAMETRO_RODA) / 60.0;

    // ===== SAÍDA =====
    Serial.print("PWM1: ");
    Serial.print(velocidade_motor1);
    Serial.print(" | RPM1: ");
    Serial.print(rpm1_filtrado);
    Serial.print(" | v1: ");
    Serial.print(v1);

    Serial.print(" || PWM2: ");
    Serial.print(velocidade_motor2);
    Serial.print(" | RPM2: ");
    Serial.print(rpm2_filtrado);
    Serial.print(" | v2: ");
    Serial.println(v2);

    lastEncoder1 = enc1;
    lastEncoder2 = enc2;

    lastSample += SAMPLE_TIME;
  }
}