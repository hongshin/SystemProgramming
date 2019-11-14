#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>

int DHTPIN = 7 ;

void 
read_dht11 () 
{
  int d[5] = {0, 0, 0, 0, 0} ;
  uint8_t i, j = 0 ;
  uint8_t counter = 0 ;

  pinMode(DHTPIN, OUTPUT) ;
  digitalWrite(DHTPIN, LOW ) ;
  delay(18) ;
	  //digitalWrite(DHTPIN, HIGH) ;
	  //delayMicroseconds(40) ;

  digitalWrite(DHTPIN, HIGH) ;
  uint8_t lastState = HIGH ;

  delayMicroseconds(20) ;

  pinMode(DHTPIN, INPUT) ;

  for (i = 0 ; i < 85 ; i++) {
    counter = 0 ;
    while (digitalRead(DHTPIN) == lastState) {
      counter++ ;
      delayMicroseconds(1) ;
      if (counter == 255)
        break ;
    }
    lastState = digitalRead(DHTPIN) ;

    if (counter == 255) {
       printf(".") ;
       break ;
    }
    if (i >= 4 && i % 2 == 0) {
      d[j/8] <<= 1 ;
      if (counter > 30) 
	d[j/8] |= 1 ;
      j++ ;
    }
  }

  if (j >= 40 && (d[4] == ((d[0] + d[1] + d[2] + d[3]) & 0xFF))) {
    float f ;
    f = d[2] * 9.0 / 5.0 + 32 ;
    printf("Humidity = %d.%d\t", d[0], d[1]) ;
    printf("Temperat = %d.%d C\n", d[2], d[3]) ;
    //printf("Farenhei = %f\n", f) ;
  }
  else {
    printf("data not good\n") ;
    printf("Humidity = %d.%d\t", d[0], d[1]) ;
    printf("Temperat = %d.%d C\n", d[2], d[3]) ;
  }
}

int  
main () 
{
  printf("DHT11 temperature\n") ;
  if (wiringPiSetup() == -1)
    exit(-1) ;

  while (1) {
    read_dht11() ;
    delay(2000) ;
  }

  return 0 ;
}
