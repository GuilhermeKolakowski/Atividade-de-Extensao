#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// Pinos:
const int HX711_dout = 50; 
const int HX711_sck = 49; 

// HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

// --- CONFIGURAÇÃO DO FILTRO ---
const int numLeituras = 10;      // Aumente para mais estabilidade, diminua para mais rapidez
float leituras[numLeituras];      
int indiceLeitura = 0;            
float totalSoma = 0;              
float mediaFiltrada = 0;          
// ------------------------------

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println("\nIniciando...");

  // Inicializa o array do filtro
  for (int i = 0; i < numLeituras; i++) leituras[i] = 0;

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; 
  boolean _tare = true; 
  LoadCell.start(stabilizingtime, _tare);
  
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Erro: Verifique a fiação do HX711");
    while (1);
  } else {
    LoadCell.setCalFactor(1.0); 
    Serial.println("Inicialização concluída");
  }
  while (!LoadCell.update());
  calibrate(); // Chama a calibração inicial
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 200; 

  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {
    // --- LÓGICA DO FILTRO DE MÉDIA MÓVEL ---
    totalSoma = totalSoma - leituras[indiceLeitura];    
    leituras[indiceLeitura] = LoadCell.getData();       
    totalSoma = totalSoma + leituras[indiceLeitura];    
    indiceLeitura = (indiceLeitura + 1) % numLeituras;  
    mediaFiltrada = totalSoma / numLeituras;            
    // ---------------------------------------

    if (millis() > t + serialPrintInterval) {
      Serial.print("Valor Filtrado: ");
      Serial.println(mediaFiltrada);         
      newDataReady = 0;
      t = millis();
    }
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
    else if (inByte == 'r') calibrate();
    else if (inByte == 'c') changeSavedCalFactor();
  }

  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}

// --- FUNÇÃO DE CALIBRAÇÃO ---
void calibrate() {
  Serial.println("***");
  Serial.println("Iniciando Calibração:");
  Serial.println("Remova qualquer peso da balança.");
  Serial.println("Envie 't' via serial para definir o TARE (Zero).");

  boolean _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 't') LoadCell.tareNoDelay();
    }
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare completo.");
      _resume = true;
    }
  }

  Serial.println("Agora, coloque um peso conhecido.");
  Serial.println("Envie o valor desse peso (ex: 100.0) via serial.");

  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print("Peso conhecido: ");
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  LoadCell.refreshDataSet(); 
  float newCalibrationValue = LoadCell.getNewCalibration(known_mass); 

  Serial.print("Novo fator de calibração: ");
  Serial.println(newCalibrationValue);
  Serial.println("Salvar no EEPROM? y/n");

  _resume = false;
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        Serial.println("Salvo com sucesso!");
        _resume = true;
      }
      else if (inByte == 'n') {
        Serial.println("Valor não salvo.");
        _resume = true;
      }
    }
  }
  Serial.println("Fim da calibração. Use 'r' para repetir ou 'c' para ajuste manual.");
  Serial.println("***");
}

// --- FUNÇÃO DE AJUSTE MANUAL ---
void changeSavedCalFactor() {
  float oldCalibrationValue = LoadCell.getCalFactor();
  boolean _resume = false;
  Serial.println("***");
  Serial.print("Fator atual: ");
  Serial.println(oldCalibrationValue);
  Serial.println("Envie o novo fator de calibração manual:");
  
  float newCalibrationValue;
  while (_resume == false) {
    if (Serial.available() > 0) {
      newCalibrationValue = Serial.parseFloat();
      if (newCalibrationValue != 0) {
        Serial.print("Novo fator definido: ");
        Serial.println(newCalibrationValue);
        LoadCell.setCalFactor(newCalibrationValue);
        _resume = true;
      }
    }
  }
  
  Serial.println("Salvar este novo fator no EEPROM? y/n");
  _resume = false;
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        Serial.println("Salvo!");
        _resume = true;
      }
      else if (inByte == 'n') {
        _resume = true;
      }
    }
  }
}