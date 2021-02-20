# Arduino library for DSM501 dust sensor


### Example
```
#define DUST_SENSOR_PIN_PM10  3  //Must be the pins that
#define DUST_SENSOR_PIN_PM25  2  //support interrupts

#define INTERVAL_COUNTDOWN 1000
#define INTERVAL_READ 30000

#include <KarserDSM501.h>
// ISRs forward declaration
void pm10_handleInterrupt();
void pm25_handleInterrupt();
// init pm10 and pm25 instances
KarserDSM501 pm10(DUST_SENSOR_PIN_PM10, pm10_handleInterrupt);
KarserDSM501 pm25(DUST_SENSOR_PIN_PM25, pm25_handleInterrupt);
// handle ISRs
void pm10_handleInterrupt() { pm10.handleInterrupt(); }
void pm25_handleInterrupt() { pm25.handleInterrupt(); }


unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (!pm10.isReady() && (millis() >= timer + INTERVAL_COUNTDOWN)) {
    Serial.println("DSM501 warm up: " + String(pm10.getReadyCountdown()));
    timer += INTERVAL_COUNTDOWN;
  } else if (millis() >= timer + INTERVAL_READ) {
    timer += INTERVAL_READ;
    Serial.println("pm10: "+String(pm10.readPM())+" pm2.5: "+String(pm25.readPM()));    
  }
}
```

Here is the console output
```
18:53:40.791 -> DSM501 warm up: 59
...
18:54:36.845 -> DSM501 warm up: 3
18:54:37.841 -> DSM501 warm up: 2
18:54:38.837 -> DSM501 warm up: 1
18:54:39.832 -> DSM501 warm up: 0
18:55:09.855 -> pm10: 21.39 pm2.5: 6.65
18:55:39.872 -> pm10: 20.31 pm2.5: 5.57
18:56:09.897 -> pm10: 28.64 pm2.5: 7.07
18:56:39.920 -> pm10: 17.89 pm2.5: 6.92
18:57:09.940 -> pm10: 25.84 pm2.5: 7.86
18:57:39.937 -> pm10: 23.35 pm2.5: 6.83
18:58:09.959 -> pm10: 29.80 pm2.5: 6.84
18:58:39.980 -> pm10: 18.68 pm2.5: 6.98
18:59:10.003 -> pm10: 27.84 pm2.5: 7.54
```

