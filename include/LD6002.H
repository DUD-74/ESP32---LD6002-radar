#ifndef LD6002_H
#define LD6002_H

#include <Arduino.h>

float bytesToFloat(uint8_t byteArray[4]);

// Enum für Nachrichtentypen, wie sie vom Sensor verwendet werden
enum MessageType
{
    Phase = 0x0A13,
    Respiratory = 0x0A14,
    Heartbeat = 0x0A15,
    Distance = 0x0A16
};

// Struktur für den Nachrichten-Header (FrameHeader)
struct FrameHeader
{
    uint16_t id;
    uint16_t length;
    MessageType type;

    // Funktion zum Lesen des Headers vom seriellen Port
    static FrameHeader read();
};

// Struktur für den Frame, welcher Header und Daten enthält
struct Frame
{
    FrameHeader header;
    uint8_t data[16];
    uint16_t len;

    // Funktion zum Lesen des Frames vom seriellen Port
    static Frame read();

    // Funktion zur Berechnung der Prüfsumme
    static uint8_t calculateChecksum(const uint8_t *data, uint16_t length);
};

// Struktur zur Speicherung der Sensor-Daten
struct SensorData
{
    float respiratory;
    float distance;
    float heartbeat;

    // Funktion zur Aktualisierung der Daten basierend auf dem Frame
    void update(Frame frame);
};

#endif // LD6002_H
