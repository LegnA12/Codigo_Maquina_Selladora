#include "Variables.h"

void setup() {
  //Pines
  Serial.begin(115200);  // Iniciar la comunicación serial
  // --------------Inicializacion de los pines-------------
  pinMode(StartButton, INPUT_PULLDOWN);
  pinMode(PE, INPUT_PULLDOWN);

  pinMode(LamparaVerde, OUTPUT);

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  // ------------Declaracion en estado bajo de los pines-------------
  digitalWrite(LamparaVerde, 1);
  digitalWrite(A, 0);
  digitalWrite(B, 0);
  digitalWrite(C, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //-------------Programacion del pot analogico para el tiempo de sellado ------------
  int valorPotenciometro = analogRead(pinPotenciometro);
  TMP_PASO1 = map(valorPotenciometro, 0, 4095, 3000, 12000);
  //TMP_PASO1 = 3000;
  Serial.println("Tiempo por el POT1 : ");
  Serial.println(TMP_PASO1);
  //-------------Programacion del pot2 analogico para el tiempo de soplado------------
  int valor2Potenciometro = analogRead(pin2Potenciometro);
   TMP_PASO2 = map(valor2Potenciometro, 0, 4095, 3000, 12000);
  // TMP_PASO2 = 3000;
  // TMP_PASO2 = 4000;
  Serial.println("Tiempo por el POT2 : ");
  Serial.println(TMP_PASO2);

  //-------------------- Lectura de los botones-------------------
  // Arranque = digitalRead(StartButton);
  // Arranque2 = digitalRead(StartButton);
  // ParoEmergencia =digitalRead(PE);
  //Si hay datos disponibles en la Serial, procesarlos
  // if (Serial.available() > 0) {
  //   char input = Serial.read();  // Leer el carácter ingresado en el serial
  //   if (input == 'p') {
  //     ParoEmergencia = HIGH;  // Simular Paro de emergencia (PE)
  //     Serial.println("Botón de Paro presionado.");
  //   }
  //   if (input == 's') {
  //     Arranque = HIGH;  // Simular StartButton
  //     Serial.println("Botón de Inicio presionado.");
  //   }
  // }
  //-------------- Paro de emergencia ----------------------
  if (ParoEmergencia == false){
    parpadearLamparaVerde(400,previousMillis1);
   Serial.println("Paro de emergencia activado. Estado cambiado a 0.");
  }

  //--------------------------------------DEBOU NCE BTN START------------------------------------------------------
  if (!digitalRead(PE)) {
    if (millis() - previousPressPE > 50) {
      estado = 0;
    }
  } else {
    previousPressPE = millis();
  }

  //------------- Programacion por estados o casos ---------------------
  switch (estado) {
    case 0:  // Estado 0: Primera confirmación
      digitalWrite(A, 0);
      digitalWrite(B, 0);
      digitalWrite(C, 0);
      Serial.println("Estado 0: Esperando confirmación de Inicio...");
      Serial.print("DEBUG StartButton raw: ");
      Serial.println(digitalRead(StartButton));
      //--------------------------------------DEBOUNCE BTN START------------------------------------------------------
      if (digitalRead(StartButton) == 1) {
        if (millis() - previousPress > 50) {
          Arranque = true;
        }
      } else {
        Arranque = false;
        previousPress = millis();
      }
      //----------------------------------------------------------------------------------------------------------------------------------------
      if (Arranque == true) {  // Si se presiona el botón de inicio
        confirmacion1 = true;
        TMP_ANTERIOR = millis();
      } else {
        parpadearLamparaVerde(1400, previousMillis1);
      }
      Serial.print("ACTUAL: ");
      Serial.println(millis());

      // -------------- Si la bandera esta activa y el tiempo de a cumplido  pasa al estado 1
      if (confirmacion1 == true && millis() - TMP_ANTERIOR >= TMP_PASO10) {
        estado = 1;
        confirmacion1 = false;  // Regreso mi bandera a false
        // Arranque = false;
        TMP_ANTERIOR = millis();
        Serial.print("Botón de Inicio confirmado. Estado cambiado a 1. TMP_ANTERIOR: ");
        Serial.println(TMP_ANTERIOR);
      }
      break;

    case 1:  // Estado 1: Proceso activo tras la primera confirmació
             //Se activa las ventosas
      digitalWrite(LamparaVerde, 1);
      digitalWrite(A, 0);
      digitalWrite(B, 0);
      digitalWrite(C, 1);  // Ventosas
      Serial.println("ESTADO 1");
      Serial.print("ACTUAL: ");
      Serial.println(millis());

      if (millis() - TMP_ANTERIOR >= TMP_PASO11) {
        estado = 2;
        TMP_ANTERIOR = millis();
        Serial.print("TMP_ANTERIOR: ");
        Serial.println(TMP_ANTERIOR);
      }
      break;
    case 2:  // Se matiene las ventosas activas --- y se espera el paso de confirmacion
      digitalWrite(A, 0);
      digitalWrite(B, 0);
      digitalWrite(C, 1);
      //--------------------------------------DEBOUNCE BTN START------------------------------------------------------
      if (digitalRead(StartButton) == 1) {
        if (millis() - previousPress > 50) {
          Arranque2 = true;
        }
      } else {
        Arranque2 = false;
        previousPress = millis();
      }
      //------------------------------------------------------------------------------------------------------------------
      if (Arranque2 == true) {  // Se espera a la confirmacion
        confirmacion2 = true;
        //TMP_ANTERIOR = millis();
        Serial.println("Estado 3: Activando pistón de sellado.");
      } else {
        parpadearLamparaVerde(1400, previousMillis1);
      }
      Serial.print("ESTADO 2: ");
      Serial.print("ACTUAL: ");
      Serial.println(millis());
      // Condicion para pasar con la confirmacion al estado 3
      if (confirmacion2 == true && millis() - TMP_ANTERIOR >= TMP_PASO12) {
        estado = 3;
        confirmacion2 = false;
        TMP_ANTERIOR = millis();
        Serial.print("Ventosas activadas por suficiente tiempo. Estado cambiado a 3. TMP_ANTERIOR: ");
        Serial.println(TMP_ANTERIOR);
      }
      break;
    case 3:  // Estado 3 es cuando baja el piston
      digitalWrite(LamparaVerde, 1);
      digitalWrite(B, 1);
      digitalWrite(C, 0);
      Serial.println("ESTADO 3: ");
      Serial.print("ACTUAL: ");
      Serial.println(millis());

      // Condicion para pasar al estado 4
      if (millis() - TMP_ANTERIOR > TMP_PASO1) {  // tiempo del pot
        estado = 4;
        TMP_ANTERIOR = millis();
        Serial.print("Pistón activado por suficiente tiempo. Estado cambiado a 4. TMP_ANTERIOR: ");
        Serial.println(TMP_ANTERIOR);
      }
      break;
    case 4:  //Tiempo de espera
             //Se desactiva las ventosas y el piston
      digitalWrite(LamparaVerde, 1);
      digitalWrite(B, 0);
      digitalWrite(C, 1);
      Serial.print("ESTADO 4: ");
      Serial.print("ACTUAL: ");
      Serial.println(millis());
      if (millis() - TMP_ANTERIOR >= TMP_PASO19) {
        estado = 5;
        TMP_ANTERIOR = millis();
        Serial.print("Ventosas y pistón desactivados. Estado cambiado a 5. TMP_ANTERIOR: ");
        Serial.println(TMP_ANTERIOR);
      }
      break;
    case 5:  // Se activa el solpaldor cone el timpo del pot
      digitalWrite(LamparaVerde, 1);
      digitalWrite(A, 1);
      digitalWrite(B, 0);
      digitalWrite(C, 0);
      Serial.println("ESTADO 5: ");
      Serial.print("ACTUAL: ");
      Serial.println(millis());
      if (millis() - TMP_ANTERIOR >= TMP_PASO2) {  // Tiempo con el soplador
        estado = 0;
        confirmacion1 = false;
        confirmacion2 = false;
        TMP_ANTERIOR = millis();
        Serial.print("Ventosas y pistón desactivados. Estado cambiado a 0. TMP_ANTERIOR: ");
        Serial.println(TMP_ANTERIOR);
      }
      break;
    default:
      // estado = 0;  // En caso de error, volver al estado inicial
      break;
  }
}
