// Definição dos pinos
#define R_EN   4
#define L_EN   5
#define RPWM   6
#define LPWM   7

// Velocidade do motor (0 a 255)
int velocidade = 255;

void setup() {
  // Configura os pinos como saída
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

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  // Gira no sentido horário

  analogWrite(RPWM, velocidade);
  analogWrite(LPWM, 0);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  

}
