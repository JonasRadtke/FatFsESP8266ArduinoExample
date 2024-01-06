
#include <SPI.h>
#include "ESP8266TimerInterrupt.h"             //https://github.com/khoih-prog/ESP8266TimerInterrupt
#include "ESP8266_ISR_Timer.hpp"               //https://github.com/khoih-prog/ESP8266TimerInterrupt
#include "ff.h"
#include "diskio.h"

// Select a Timer Clock
#define USING_TIM_DIV1                true           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              false            // for longest timer but least accurate. Default
#define TIMER_INTERVAL_MS             1
volatile uint32_t lastMillis = 0;
// Init ESP8266 only and only Timer 1
ESP8266Timer ITimer;
volatile uint32_t systick;

// interval (in microseconds)
bool attachInterruptInterval(unsigned long interval, timer_callback callback);

// 1ms Handler
void IRAM_ATTR TimerHandler()
{
  systick++;
  disk_timerproc(); // Timer for FatFS
  return;
}

uint32_t test;

FATFS FatFs;				/* File system object for each logical drive */
FIL datei[2];				/* File objects */
FRESULT res;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(lastMillis);
  }
  else {
    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));
  }

  res = f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
  Serial.print("Status:");
  Serial.println(res);

}

  char bufferarr[256];				// Variable für FileHeader
	char *buffptr = &bufferarr[0];	// Pointer darauf
	UINT readBytes;
  uint32_t i;

void loop()
{
  if (systick > test + 2000){
    test = systick;
    f_open(&datei[0], "test.txt", FA_READ);	/* Create a file */
    f_read(&datei[0], buffptr, 256, &readBytes );	/* Read data to from file */
    f_close(&datei[0]); /* Close File */
   
    // Print File
    i = 0;
    while (i < readBytes){
      Serial.print(bufferarr[i]);
      i++;
    }
    
  }
}