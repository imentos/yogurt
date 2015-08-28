// OneWire DS18B20 Temperature Switch on ATtiny85
//
// Check temperature every second.
#define ROOM_TEMP 90.0
#define YOGURT_TEMP 110.0
 
#include <OneWire.h> 
#include <SoftwareSerial.h>
 
// pins
#define SERIAL_RX 3
#define SERIAL_TX 4
#define ONEWIRE_BUSS 0
#define BUZZ 2
#define LED 1
 
SoftwareSerial TinySerial(SERIAL_RX, SERIAL_TX); // RX, TX
 
OneWire TemperatureSensor(ONEWIRE_BUSS);  // Dallas one wire data buss pin, a 4.7K resistor pullup is needed
 
void setup(void) {
    pinMode(BUZZ, OUTPUT);
    pinMode(LED, OUTPUT);
    TinySerial.begin(9600);
    TinySerial.println(F("DS18x20 Temperature Switch"));
}
 
void loop(void) {
    byte i;
    byte data[12];
    int16_t raw;
    float celsius, fahrenheit;
 
    TemperatureSensor.reset();       // reset one wire buss
    TemperatureSensor.skip();        // select only device
    TemperatureSensor.write(0x44);   // start conversion
 
    delay(1000);                     // wait for the conversion
 
    TemperatureSensor.reset();
    TemperatureSensor.skip();
    TemperatureSensor.write(0xBE);   // Read Scratchpad
    for ( i = 0; i < 9; i++) {       // 9 bytes
      data[i] = TemperatureSensor.read();
    }
 
    // Convert the data to actual temperature
    raw = (data[1] << 8) | data[0];
    celsius = (float)raw / 16.0;
    fahrenheit = celsius * 1.8 + 32.0;
    TinySerial.print(F("  Temperature = "));
    TinySerial.print(fahrenheit);
    TinySerial.print(F(" 'F"));
    TinySerial.print(F("\n"));

    // ignore the room temperature
    if (fahrenheit < ROOM_TEMP) {
      TinySerial.print(F("test"));
      TinySerial.print(F("\n"));
      tone(BUZZ, frequency('c'), 150);
      delay(150);
      return;
    }

    // when milk temperature from boil point goes below best yogurt, buzz
    if (fahrenheit < YOGURT_TEMP) {
      tone(BUZZ, frequency('c'), 150);
      delay(150);
    }
}

int frequency(char note) 
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.
  
  int i;
  const int numNotes = 8;  // number of notes we're storing
  
  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};
  
  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.
  
  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return(frequencies[i]);     // Yes! Return the frequency
    }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}
