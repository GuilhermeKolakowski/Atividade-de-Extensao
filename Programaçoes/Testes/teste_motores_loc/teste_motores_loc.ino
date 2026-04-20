/*
  Sabertooth 2x25 V2
  Simplified Serial
  Arduino Mega - Serial3 (TX pino 14)
  motor esquerda
*/

#define R_EN   4
#define L_EN   5
#define RPWM   6
#define LPWM   7

int velocidade = 255;
void setup() {
  Serial3.begin(9600);
  delay(3000); // tempo de inicialização da Sabertooth

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);

  // Habilita a ponte H
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  // Garante motor parado na inicialização
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
}

void loop() {

  // ===== FRENTE =====
  /* . Sending a character between 1 and 127 will
  control motor 1. 1 is full reverse, 64 is stop and 127 is full forward. Sending a character between
  128 and 255 will control motor 2. 128 is full reverse, 192 is stop and 255 is full forward*/
  Serial3.write(1); // Motor 1 frente esquerda
  Serial3.write(130);

  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  delay(5000);

  delay(3000000);

  /*// ===== STOP =====
  Serial3.write(0);  // Motor 1 stop esquerda
  Serial3.write(0); // Motor 2 stop direita
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  delay(2000);

  // ===== RÉ =====
  Serial3.write(38);   // Motor 1 ré esquerda
  Serial3.write(176); // Motor 2 ré direita
  analogWrite(RPWM, velocidade);
  analogWrite(LPWM, 0);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  delay(1500);

  // ===== STOP =====
  Serial3.write(0);
  Serial3.write(0);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  delay(2000);*/
}
