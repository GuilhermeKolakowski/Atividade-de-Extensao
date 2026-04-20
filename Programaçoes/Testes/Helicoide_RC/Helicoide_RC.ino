// ==========================
// PINOS BTS7960
// ==========================
#define R_EN   4
#define L_EN   5
#define RPWM   6
#define LPWM   7

// ==========================
// CANAL 3 (RECEPTOR)
// ==========================
#define CH3_PIN 2

// ==========================
// VARIÁVEIS (INTERRUPÇÃO)
// ==========================
volatile unsigned long startTime = 0;
volatile unsigned long pulseWidth = 1500;

// ==========================
// SETUP
// ==========================
void setup() {
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);

  pinMode(CH3_PIN, INPUT);

  // Habilita ponte H
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  // Motor parado
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  //reles
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  Serial.begin(9600);

  // INTERRUPÇÃO
  attachInterrupt(digitalPinToInterrupt(CH3_PIN), readCH3, CHANGE);
}

// ==========================
// LOOP PRINCIPAL
// ==========================
void loop() {
    digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  Serial.println(pulseWidth); // debug

  // ==========================
  // SEGURANÇA (sinal inválido)
  // ==========================
  if (pulseWidth < 900 || pulseWidth > 2100) {
    stopMotor();
    return;
  }

  // ==========================
  // CONTROLE USANDO TODA FAIXA
  // ==========================

  if (pulseWidth > 1200) {
    // Usa praticamente todo o curso do stick
    int pwm = map(pulseWidth, 1200, 2000, 0, 255);

    pwm = constrain(pwm, 0, 255);

    analogWrite(RPWM, pwm);
    analogWrite(LPWM, 0);

    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
  } 
  else {
    stopMotor();
  }
}

// ==========================
// INTERRUPÇÃO (LEITURA PWM)
// ==========================
void readCH3() {
  if (digitalRead(CH3_PIN) == HIGH) {
    startTime = micros();
  } else {
    pulseWidth = micros() - startTime;
  }
}

// ==========================
// FUNÇÃO PARAR MOTOR
// ==========================
void stopMotor() {
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}