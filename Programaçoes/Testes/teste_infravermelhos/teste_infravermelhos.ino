// Pinos dos sensores
const int sensor1Pin = 34; //frente
const int sensor2Pin = 35;// lateral

void setup() {
  Serial.begin(9600);

  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);

  Serial.println("Leitura dos sensores E18-D80NK iniciada");
}

void loop() {
  int sensor1State = digitalRead(sensor1Pin);
  int sensor2State = digitalRead(sensor2Pin);

  // Sensor 1
  if (sensor1State == LOW) {
    Serial.println("Sensor 1: Objeto detectado (dentro do alcance ajustado)");
  } else {
    Serial.println("Sensor 1: Nenhum objeto detectado");
  }

  // Sensor 2
  if (sensor2State == LOW) {
    Serial.println("Sensor 2: Objeto detectado (dentro do alcance ajustado)");
  } else {
    Serial.println("Sensor 2: Nenhum objeto detectado");
  }

  Serial.println("-----------------------------");
  delay(500);
}
