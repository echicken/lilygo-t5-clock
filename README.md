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

### Configuration Notes

Get your OpenWeatherMap API key [here](https://openweathermap.org/api).

The NTP update interval may be unnecessarily short by default. Would be worth trying different values and checking the time against a known good clock.

The display is cleared and redrawn entirely every `REDRAW_INTERVAL` seconds. Helps to get rid of ghosting and clean up the display, but may not be necessary - it's a bit of a holdover from an earlier stage of this project. Would be worth increasing this and seeing if the display suffers for it over time.

## Design Notes

### Enclosure

I'm using the variant of this display with a JST battery connector rather than the 18650 version. The intention is to keep it thin and flat, to be used as a wall clock. Every case I've seen so far is thicc or has a hump on the back for a big round battery, so I'll design my own and add it here once I've settled on a suitable battery.

### Power Consumption

I initially used the Arduino [Ticker](https://www.arduino.cc/reference/en/libraries/ticker/) library, and then [TickTwo](https://github.com/sstaub/TickTwo) in order to schedule various timed events. It quickly became clear that the current consumption was unsuitable for a battery-powered device, even when turning WiFi off when it wasn't needed and limiting screen updates to a reasonable extent.

The EPD is fully powered off between updates, including the obnoxious blue LED when on battery power.

#### Deep Sleep

I'm now putting the ESP32 into deep sleep mode when it can sit idle, waking it up once per minute to update the clock. (The sleep interval is recalculated each time, so that it'll update as close to the start of the next minute as possible.) In this scenario, the `loop()` function is never called, and everything happens in `setup()`. Values are stored in RTC memory to persist between sleep/wake cycles. Some good info on this topic was found [here](https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/).

The **clock** will update every minute, on the minute (ish), but there will be some slop among the other update intervals. They'll happen as scheduled (ish) but may lag by up to a minute, causing the schedule to drift over time. Bear this in mind if you're watching and expecting to see eg. a weather update at a particular time.

#### Clearing and redrawing

Partial updating of the screen is used for speed and, presumably, to reduce power consumption. Steps are taken to update parts of the screen only as needed. This could probably be taken a bit further by updating single characters (eg. if only the minute has changed), but I'm not sure it's worth the bother, and it'd probably limit us to monospaced fonts. For now, the entire hours-and-minutes field is cleared and redrawn when the time changes even by a minute, likewise the date, the temperature, and so on.

## Credits

[Dushyant Ahuja](https://github.com/dushyantahuja)'s [SimpleWeather](https://github.com/dushyantahuja/SimpleWeather) was taken and adapted for use in this project.

The [LilyGo EPD47](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47) driver from [Valentin Roland](https://github.com/vroland)'s [EPDiy](https://github.com/vroland/epdiy) project via [Xinyuan-LilyGO](https://github.com/Xinyuan-LilyGO) is used in this project.

Weather icons are from the [Meteocons](https://www.alessioatzeni.com/meteocons/) font by [Alessio Atzeni](https://www.alessioatzeni.com/meteocons/). A [font conversion script](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47/blob/master/scripts/fontconvert.py) is included in the [LilyGo EPD47](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47) project and was used here.

The NTP client is from [here](https://github.com/arduino-libraries/NTPClient) via [Arduino Libraries](https://github.com/arduino-libraries).

Probably others that I'm forgetting, and there's a whole stack of other people's hard work beneath all of that.