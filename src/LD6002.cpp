#include "ld6002.h"

// Funktion zum Lesen des FrameHeaders vom seriellen Port
FrameHeader FrameHeader::read()
{
    FrameHeader header;
    uint8_t headerBytes[7];

    // Header Bytes vom seriellen Port lesen
    for (int i = 0; i < 7; i++)
    {
        headerBytes[i] = Serial2.read();
    }

    // Felder extrahieren
    header.id = (headerBytes[0] << 8) | headerBytes[1];
    header.length = (headerBytes[2] << 8) | headerBytes[3];
    header.type = static_cast<MessageType>((headerBytes[4] << 8) | headerBytes[5]);
    return header;
}

// Funktion zum Lesen des Frames vom seriellen Port
Frame Frame::read()
{
    Frame frame;
    uint8_t magic = Serial2.read();

    // Prüfen, ob das Frame korrekt startet
    if (magic != 1)
    {
        // Ungültiger Frame-Start, Rückgabe eines leeren Frames
        return frame;
    }

    // Header lesen
    frame.header = FrameHeader::read();

    // check for reading errors - just return empty frame
    if (frame.header.length > 20)
        return frame;


    // Daten entsprechend der Länge im Header lesen
    for (int i = 0; i < frame.header.length; i++)
    {

        frame.data[i] = Serial2.read();
    }

    // Prüfsumme lesen und validieren
    uint8_t checksumReceived = Serial2.read();
    uint8_t calculatedChecksum = Frame::calculateChecksum(frame.data, frame.header.length);

    if (checksumReceived != calculatedChecksum)
    {
        // Prüfsummenfehler behandeln
        Serial.println("Checksum error!");
    }

    return frame;
}

// Funktion zur Berechnung der Prüfsumme
uint8_t Frame::calculateChecksum(const uint8_t *data, uint16_t length)
{
    uint8_t result = 0;
    for (uint16_t i = 0; i < length; i++)
    {
        result ^= data[i];
    }
    return ~result; // Prüfsumme invertieren
}

// Funktion zur Aktualisierung der Sensordaten basierend auf dem empfangenen Frame
void SensorData::update(Frame frame)
{
    // Nachrichtendaten basierend auf dem Nachrichtentyp interpretieren
    switch (frame.header.type)
    {
    case Respiratory:

        if (frame.data > 0)
        {
            respiratory = *((float *)frame.data);
        }
        break;
    case Distance:
        if (frame.data[0] == 1)
        {
            uint8_t byteArray[4] = {frame.data[4], frame.data[5], frame.data[6], frame.data[7]};
            distance = bytesToFloat(byteArray);
        }
        else
            distance = 0;
        break;
    case Heartbeat:
        if (frame.data > 0)
        {
            heartbeat = *((float *)frame.data);
        }
        break;
    default:
        break;
    }
}

float bytesToFloat(uint8_t byteArray[4])
{
    float result;
    memcpy(&result, byteArray, 4); 
    return result;
}