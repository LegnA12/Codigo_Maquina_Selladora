// #include "Variables.h"

//------------------Funcion parpadeo ----------------------

void parpadearLamparaVerde(long intervalo, unsigned long &previousMillis) {

  // Comprueba si ha pasado el tiempo deseado
  if (millis() - previousMillis >= intervalo) {
    previousMillis = millis();   // Actualiza el tiempo de la última vez que cambió el LED

    // Lee el estado actual del LED y cambia el estado con if-else
    if (digitalRead(LamparaVerde) == HIGH) {
      digitalWrite(LamparaVerde, LOW);   // Si está encendido, lo apaga
    } else {
      digitalWrite(LamparaVerde, HIGH);  // Si está apagado, lo enciende
    }
  }
}
