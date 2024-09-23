#include "ld6002.h"

SensorData sensorData;

unsigned long last_timestamp = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up...");
  // Seriellen Monitor starten
  Serial2.begin(1382400); // Baudrate auf 1.382.400 setzen
  while (!Serial2)
  {
    ; // Warte auf die serielle Verbindung
  }

  Serial.println("HLK-LD6002 Sensor gestartet...");
  }

void loop()
{
  // Überprüfen, ob Daten vom seriellen Port verfügbar sind

  if (Serial2.available() > 0)
  {
    // Frame vom Sensor lesen
    Frame frame = Frame::read();

    // Sensor-Daten basierend auf dem Frame aktualisieren
    sensorData.update(frame);
  }

  if (millis() - last_timestamp > 1000)
  { // Sensordaten ausgeben
    Serial.print("Respiratory: ");
    Serial.println(sensorData.respiratory);
    Serial.print("Distance: ");
    Serial.println(sensorData.distance);
    Serial.print("Heartbeat: ");
    Serial.println(sensorData.heartbeat);
    last_timestamp = millis();
  }
}
