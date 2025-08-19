#include <Arduino.h>
void setup() {

  Serial.begin(9600);
  Serial.println("MDB Bus Sniffer - Listening on Serial1");
  Serial.println("Monitoring VMC <-> Coin Acceptor communication");
  Serial.println("---");

  UCSR1A = 0;
  UCSR1B = 0;
  UCSR1C = 0;
  
  UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11); 
  UCSR1B |= (1 << UCSZ12);
  
  UBRR1 = 103; // لـ 16MHz (16000000 / (16 * 9600) - 1)
  UCSR1B |= (1 << RXEN1);

  UCSR2A = 0;
  UCSR2B = 0;
  UCSR2C = 0;
  
  UCSR2C |= (1 << UCSZ20) | (1 << UCSZ21); 
  UCSR2B |= (1 << UCSZ22);
  
  UBRR2 = 103; // لـ 16MHz (16000000 / (16 * 9600) - 1)
  UCSR2B |= (1 << RXEN2);
}

void loop() {

    if (UCSR1A & (1 << RXC1)) { 
    uint16_t data = 0;
    data = UDR1; //
    
    if (UCSR1B & (1 << RXB81)) {
      data |= 0x100; 
    }
    bool modeBit = (data & 0x100) != 0;
    uint8_t byteValue = data & 0xFF;
    
    Serial.print(modeBit ? "ADDR VMC to arduino" : "DATA ");
    // Serial.print("0x");
    if (byteValue < 0x10) Serial.print("0"); 
    Serial.print(byteValue, HEX);
    Serial.print(" ");
    Serial.println();
    }

    /////////////
    if (UCSR2A & (1 << RXC2)) { 
    uint16_t data1 = 0;
    data1 = UDR2; //
    
    if (UCSR2B & (1 << RXB82)) {
      data1 |= 0x100; 
    }

    bool modeBit1 = (data1 & 0x100) != 0;
    uint8_t byteValue1 = data1 & 0xFF;
    
    Serial.print(modeBit1 ? "ADDR arduino to VMC" : "DATA ");
    // Serial.print("0x");
    if (byteValue1 < 0x10) Serial.print("0"); 
    Serial.print(byteValue1, HEX);
    Serial.print(" ");
    Serial.println();
    }
  delay(1);
}