# lilygo-t5-clock
Time, date, and weather on a LILYGO T5-4.7 ESP32 e-paper display. Developed with Platform.IO & Arduino framework.

![A picture of the clock](/epd_clock.png)

## Configuration

Create a file at `src/config.h` and populate it with the following:

```c
#define WIFI_SSID "Your WiFi SSID goes here"
#define WIFI_PASS "Your WiFi password goes here"
#define OWM_KEY "Your OpenWeatherMap API key goes here"
#define OWM_LOCATION "Toronto,ca"        // City and country code
#define TZ_INFO "EST5EDT,M3.2.0,M11.1.0" // "America/Toronto" https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define VOLTAGE_INTERVAL 900  // Seconds between battery voltage updates
#define NTP_INTERVAL	 3600 // Seconds between NTP updates
#define WEATHER_INTERVAL 1800 // Seconds between weather updates
#define REDRAW_INTERVAL  3600 // Seconds between full screen redraws
```

Customize as needed.