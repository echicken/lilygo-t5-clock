# lilygo-t5-clock

Time, date, and weather on a LILYGO T5-4.7 ESP32 e-paper display.

Developed with Platform.IO & Arduino framework.

![A picture of the clock](/epd_clock.png)

## Components

- The "PH 2.0 holder" variant of [this EPD module](https://www.aliexpress.com/item/1005002006058892.html) ([manufacturer's page](http://www.lilygo.cn/prod_view.aspx?TypeId=50061&Id=1384&FId=t3:50061:3))
- [This battery](https://www.amazon.ca/gp/product/B095BTSMYH), 3.7V 10.5aH
- A JST PH 2.0 connector
- ~10cm of 22 AWG wire (for hanging)

### Battery Notes

On my EPD board, when viewing the board side, with the ESP32 module at the top-right, the positive pin of the JST PH 2.0 connector is on the left. Be careful to connect the battery to the correct pins on **your** board, which may be different from mine, and **do not** attempt to charge the battery until you're sure it's hooked up properly.

The wires on the battery that I'm using came unterminated. I just soldered the wires together with those of a pre-terminated pigtail. These are [easy to find online](https://www.amazon.ca/gp/product/B07NWD5NTN), but I just used what I had on hand and can't vouch for this exact product. Or do it the proper way with a connector and crimper, if you've got them.

## Configuration

Create a file at `src/config.h` and populate it with the following:

```c
#define WIFI_SSID "Your WiFi SSID goes here"
#define WIFI_PASS "Your WiFi password goes here"
#define OWM_KEY "Your OpenWeatherMap API key goes here"
#define OWM_LOCATION "Toronto,ca"        // City and country code
#define TZ_INFO "EST5EDT,M3.2.0,M11.1.0" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define VOLTAGE_INTERVAL 1800  // Seconds between battery voltage updates
#define NTP_INTERVAL	 3600  // Seconds between NTP updates
#define WEATHER_INTERVAL 1800  // Seconds between weather updates
#define REDRAW_INTERVAL  3600 // Seconds between full screen redraws
```

Customize as needed.

### Configuration Notes

Get your OpenWeatherMap API key [here](https://openweathermap.org/api).

I can't find a list of named locations (city, country code as in `OWM_LOCATION`) for use with OpenWeatherMap, but as luck would have it they know about my city. It'd be trivial to allow lookup by latitude and longitude instead, so let me know if you need this feature; I won't bother for now since I don't need it.

The NTP update interval may be unnecessarily short by default. Would be worth trying different values and checking the time against a known good clock.

The display is cleared and redrawn entirely every `REDRAW_INTERVAL` seconds. Helps to get rid of ghosting and clean up the display, as parts that should be blank (white) tend to darken over time, and garbage occasionally appears in unexpected places.

## Design Notes

### Enclosure

I'm using the variant of this display with a JST battery connector, along with [this battery](https://www.amazon.ca/gp/product/B095BTSMYH). This allows for a thinner case and longer battery life than with the 18650 variant. Case STL and OpenSCAD source are in the [enclosure/](enclosure/) directory.

### Power Consumption

The EPD is fully powered off, along with the obnoxious blue LED, except when actively drawing. (The blue LED is always on when on USB power, but not on battery.)

I initially used the Arduino [Ticker](https://www.arduino.cc/reference/en/libraries/ticker/) library, and then [TickTwo](https://github.com/sstaub/TickTwo) in order to schedule various timed events. It quickly became clear that the current consumption was unsuitable for a battery-powered device, even when turning WiFi off when it wasn't needed and limiting screen updates to a reasonable extent.

#### Deep Sleep

I'm now putting the ESP32 into deep sleep mode when it can sit idle, waking it up once per minute to update the clock. (The sleep interval is recalculated each time, so that it'll update as close to the start of the next minute as possible.) In this scenario, the `loop()` function is never called, and everything happens in `setup()`. Values are stored in RTC memory to persist between sleep/wake cycles. Some good info on this topic was found [here](https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/).

The **clock** will update every minute, on the minute (ish), but there will be some slop among the other update intervals. They'll happen as scheduled (ish) but may lag by up to a minute, causing the schedule to drift over time. Bear this in mind if you're watching and expecting to see eg. a weather update at a particular time.

##### Buttons

The leftmost button (viewing the display-side in landscape orientation), labelled S5, pulls the ESP32's RST line to ground. This restarts the device.

I've considered assigning functions to the rightmost three buttons, but the design of this device makes that a hassle when combined with deep sleep.

I've decided it's sufficient to be able to restart the device if something goes wrong. To that end I plan on leaving a hole in the enclosure so I can press the reset button with a pen or something if needed.

###### More Button Musing

The second-leftmost button is connected to the ESP32's GPIO 0, probably to put it into flash mode when pressed along with RST, but I can't remember offhand.

The remaining three buttons are active low. Their GPIOs have pull-up resistors on them. The ESP32 has two GPIO-triggered wake-up modes. The first, ext0, requires you to assign one GPIO to trigger wake-up, and will wake the device whether this goes from low to high or high to low. The second, ext1, will wake the device if one of several GPIOs goes from low to high; unfortunately all three of our available switches are already pulled up. In order to use multiple buttons, we'd need to use ext0 to wake the device, then enter an input-handling routine for some amount of time. Not difficult to do, but not worth the hassle in my opinion.

#### Clearing and redrawing

Partial updating of the screen is used for speed and, presumably, to reduce power consumption. Steps are taken to update parts of the screen only as needed. This could probably be taken a bit further by updating single characters (eg. if only the minute has changed), but I'm not sure it's worth the bother. For now, the entire hours-and-minutes field is cleared and redrawn when the time changes even by a minute, likewise the date, the temperature, and so on.

## Credits

[Dushyant Ahuja](https://github.com/dushyantahuja)'s [SimpleWeather](https://github.com/dushyantahuja/SimpleWeather) was taken and adapted for use in this project.

The [LilyGo EPD47](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47) driver from [Valentin Roland](https://github.com/vroland)'s [EPDiy](https://github.com/vroland/epdiy) project via [Xinyuan-LilyGO](https://github.com/Xinyuan-LilyGO) is used in this project.

Weather icons are from the [Meteocons](https://www.alessioatzeni.com/meteocons/) font by [Alessio Atzeni](https://www.alessioatzeni.com/meteocons/). A [font conversion script](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47/blob/master/scripts/fontconvert.py) is included in the [LilyGo EPD47](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47) project and was used here.

The NTP client is from [here](https://github.com/arduino-libraries/NTPClient) via [Arduino Libraries](https://github.com/arduino-libraries).

Probably others that I'm forgetting, and there's a whole stack of other people's hard work beneath all of that.