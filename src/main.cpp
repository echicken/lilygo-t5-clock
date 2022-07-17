#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

#include <Arduino.h>
#include "config.h"
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "epd_driver.h"
#include "NK5715b.h"
#include "NK5724b.h"
#include "NK5748b.h"
#include "NK5772b.h"
#include "Meteocons96.h"
#include "esp_adc_cal.h"
#include <Wire.h>
#include <sys/time.h>
#include "WiFi.h"
#include "WiFiUdp.h"
#include <NTPClient.h>
#include "SimpleWeather.h"

#define BATT_PIN 36

#define EPD_WIDTH 960
#define EPD_HEIGHT 540
#define H_MARGIN 20
#define V_MARGIN 20

const uint CLOCK_X = H_MARGIN;
const uint CLOCK_Y = 175;
const uint DATE_X = EPD_WIDTH - H_MARGIN;
const uint DATE_Y1 = 105;
const uint DATE_Y2 = CLOCK_Y;
const uint VOLTAGE_X = H_MARGIN;
const uint VOLTAGE_Y = EPD_HEIGHT - V_MARGIN;
const uint START_TIME_X = EPD_WIDTH - H_MARGIN;
const uint START_TIME_Y = EPD_HEIGHT - V_MARGIN;
const uint WICON_X = H_MARGIN;
const uint WICON_Y = 450;
const uint CTEMP_X = 235;
const uint CTEMP_Y = 450;
const uint FTEMP_X = 235;
const uint FTEMP_Y = 350;
const uint WIND_X = EPD_WIDTH - H_MARGIN;
const uint WIND_Y = 350;
const uint HUMID_X = WIND_X;
const uint HUMID_Y = 400;
const uint WUPDATE_X = WIND_X;
const uint WUPDATE_Y = 450;

/**
 * WICON_AREA is used when erasing the weather icon prior to redrawing.
 * I may have messed up when generating the Meteocons font, or it may just be
 * that it's an unusual font. Either way, the clearString function wasn't
 * consistently erasing the entire previous icon. Here we'll just define a
 * large area that definitely eoncompasses the whole thing.
 */
const Rect_t WICON_AREA = {
	.x = 1,
	.y = (int32_t)(CLOCK_Y + 1),
	.width = (int32_t)(CTEMP_X - 1),
	.height = (int32_t)(VOLTAGE_Y - 30 - (CLOCK_Y + 1)),
};

bool DRAW_DATE = false;
bool DRAW_WEATHER = false;
bool DRAW_WICON = false;
bool DRAW_TEMP = false;
bool DRAW_FTEMP = false;
bool DRAW_WIND = false;
bool DRAW_HUMIDITY = false;
bool DRAW_VOLTAGE = false;

struct tm now;
time_t waketime;
enum alignment { LEFT, RIGHT, CENTER };

RTC_DATA_ATTR bool firstRun = true;
RTC_DATA_ATTR int minute = -1;
RTC_DATA_ATTR int dayOfWeek = -1;
RTC_DATA_ATTR int vref = 1100;
RTC_DATA_ATTR float voltage = -1;
RTC_DATA_ATTR char dow[20];
RTC_DATA_ATTR char mdy[50];
RTC_DATA_ATTR char tod[10];
RTC_DATA_ATTR char volts[10];
RTC_DATA_ATTR char wTemp[10];
RTC_DATA_ATTR char wFeels[20];
RTC_DATA_ATTR char wWind[25];
RTC_DATA_ATTR char wHumidity[20];
RTC_DATA_ATTR char wUpdated[20];
RTC_DATA_ATTR char stime[40];
RTC_DATA_ATTR time_t lastNtpUpdate;
RTC_DATA_ATTR time_t lastVoltageUpdate;
RTC_DATA_ATTR time_t lastWeatherUpdate;
RTC_DATA_ATTR time_t lastRedraw;
RTC_DATA_ATTR String wIcon = ")";

char _tod[10];
char _dow[20];
char _mdy[50];
char _volts[10];
char _wTemp[10];
char _wFeels[20];
char _wWind[25];
char _wHumidity[20];
char _wUpdated[20];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
GFXfont currentFont;
weatherData w;
OpenWeather weather(OWM_KEY, OWM_LOCATION);

int setUnixtime(int32_t unixtime) {
	timeval epoch = {unixtime, 0};
	return settimeofday((const timeval*)&epoch, 0);
}

void setFont(GFXfont const &font) {
	currentFont = font;
}

void clearString(int x, int y, String text, alignment align) {
	char *data  = const_cast<char*>(text.c_str());
	int x1, y1;
	int w, h;
	int xx = x, yy = y;
	get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
	if (align == RIGHT)  x = x - w;
	if (align == CENTER) x = x - w / 2;
	Rect_t area = {
		.x = x,
		.y = y - h,
		.width = w,
		.height = h,
	};
	epd_clear_area(area);
}

void drawString(int x, int y, String text, alignment align) {
	char *data  = const_cast<char*>(text.c_str());
	int x1, y1;
	int w, h;
	int xx = x, yy = y;
	get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
	if (align == RIGHT)  x = x - w;
	if (align == CENTER) x = x - w / 2;
	writeln((GFXfont *)&currentFont, data, &x, &y, NULL);
}

void drawString(int x, int y, String text, String old_text, alignment align) {
	if (!firstRun) {
		clearString(x, y, old_text, align);
	}
	drawString(x, y, text, align);
}

void drawStartTime() {
	setFont(NK5715B);
	drawString(START_TIME_X, START_TIME_Y, stime, stime, RIGHT);
}

void drawClock() {
	setFont(NK5772B);
	drawString(CLOCK_X, CLOCK_Y, _tod, tod, LEFT);
	if (DRAW_DATE) {
		setFont(NK5724B);
		drawString(DATE_X, DATE_Y1, _dow, dow, RIGHT);
		drawString(DATE_X, DATE_Y2, _mdy, mdy, RIGHT);
	}
}

void getClock() {
	getLocalTime(&now, 0);
	strftime(_tod, 10, "%H:%M", &now);
	if (dayOfWeek != now.tm_wday) {
		DRAW_DATE = true;
		dayOfWeek = now.tm_wday;
		strftime(_dow, 20, "%A", &now);
		strftime(_mdy, 50, "%b %d %Y", &now);
	}
}

void setClock() {
	strcpy(tod, _tod);
	if (DRAW_DATE) {
		strcpy(dow, _dow);
		strcpy(mdy, _mdy);
	}
}

void drawVoltage() {
	setFont(NK5715B);
	drawString(VOLTAGE_X, VOLTAGE_Y, _volts, volts, LEFT);
}

void getVoltage() {
	// Correct the ADC reference voltage
	esp_adc_cal_characteristics_t adc_chars;
	esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
	if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
		vref = adc_chars.vref;
	}
	delay(10); // Make adc measurement more accurate
	uint16_t v = analogRead(BATT_PIN);
	float _voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
	if (_voltage != voltage) {
		DRAW_VOLTAGE = true;
		voltage = _voltage;
		sprintf(_volts, "%.2fV", voltage);
	}
	lastVoltageUpdate = waketime;
}

void setVoltage() {
	strcpy(volts, _volts);
}

void enableWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
    }
}

void disableWifi() {
	WiFi.disconnect();
	WiFi.mode(WIFI_OFF);
}

void ntpUpdate() {

	enableWifi();
	timeClient.begin();
	timeClient.update();
	timeClient.end();
	disableWifi();

	setUnixtime(timeClient.getEpochTime());

	lastNtpUpdate = waketime;

}

String windDirection(int deg) {
    String ret = "";
    if (deg >= 337.5 || deg < 22.5) {
        ret = "N";
    } else if (deg < 67.5) {
        ret = "NE";
    } else if (deg < 112.5) {
        ret = "E";
    } else if (deg < 157.5) {
        ret = "SE";
    } else if (deg < 202.5) {
        ret = "S";
    } else if (deg < 247.5) {
        ret = "SW";
    } else if (deg < 292.5) {
        ret = "W";
    } else if (deg < 337.5) {
        ret = "NW";
    }
    return ret;
}

String weatherIcon(String i) {
	String icon = "B";
	if (i == "01d") {
		icon = "B";
	} else if (i == "01n") {
		icon = "C";
	} else if (i == "02d") {
		icon = "H";
	} else if (i == "02n") {
		icon = "4";
	} else if (i == "03d") {
		icon = "N";
	} else if (i == "03n") {
		icon = "5";
	} else if (i == "04d") {
		icon = "Y";
	} else if (i == "04n") {
		icon = "Y";
	} else if (i == "09d") {
		icon = "Q";
	} else if (i == "09n") {
		icon = "7";
	} else if (i == "10d") {
		icon = "R";
	} else if (i == "10n") {
		icon = "8";
	} else if (i == "11d") {
		icon = "O";
	} else if (i == "11n") {
		icon = "6";
	} else if (i == "13d") {
		icon = "W";
	} else if (i == "13n") {
		icon = "#";
	} else if (i == "50d") {
		icon = "M";
	} else if (i == "50n") {
		icon = "M";
	}
	return icon;
}

void drawWeather() {

	setFont(Meteocons96);
	String icon = weatherIcon(w.icon);
	if (DRAW_WICON) {
		epd_clear_area(WICON_AREA);
		drawString(WICON_X, WICON_Y, wIcon, LEFT);
	}

	setFont(NK5748B);

	if (DRAW_TEMP) {
		drawString(CTEMP_X, CTEMP_Y, _wTemp, wTemp, LEFT);
	}

	setFont(NK5715B);

	if (DRAW_FTEMP) {
		drawString(FTEMP_X, FTEMP_Y, _wFeels, wFeels, LEFT);
	}

	if (DRAW_WIND) {
		drawString(WIND_X, WIND_Y, _wWind, wWind, RIGHT);
	}

	if (DRAW_HUMIDITY) {
		drawString(HUMID_X, HUMID_Y, _wHumidity, wHumidity, RIGHT);
	}

	drawString(WUPDATE_X, WUPDATE_Y, _wUpdated, wUpdated, RIGHT);

}

void getWeather() {

	enableWifi();
	weather.updateStatus(&w);
	disableWifi();

	lastWeatherUpdate = waketime;

	String icon = weatherIcon(w.icon);
	if (wIcon != icon) {
		wIcon = icon;
		DRAW_WEATHER = true;
		DRAW_WICON = true;
	}

	sprintf(_wTemp, "%.1fc", w.current_Temp);
	if (strcmp(wTemp, _wTemp) != 0) {
		DRAW_WEATHER = true;
		DRAW_TEMP = true;
	}

	sprintf(_wFeels, "Feels like %.1fc", w.feels_like);
	if (strcmp(wFeels, _wFeels) != 0) {
		DRAW_WEATHER = true;
		DRAW_FTEMP = true;
	}

	int ws = w.wind_speed * 3.6;
	String wd = windDirection(w.wind_direction);
	sprintf(_wWind, "Wind: %d km/h %s", ws, wd);
	if (strcmp(wWind, _wWind) != 0) {
		DRAW_WEATHER = true;
		DRAW_WIND = true;
	}

	sprintf(_wHumidity, "Humidity: %d%%", w.humidity);
	if (strcmp(wHumidity, _wHumidity) != 0) {
		DRAW_WEATHER = true;
		DRAW_HUMIDITY = true;
	}

	struct tm now;
	getLocalTime(&now, 0);
	strftime(_wUpdated, 20, "Updated: %H:%M", &now);

}

void setWeather() {

	if (DRAW_TEMP) {
		strcpy(wTemp, _wTemp);
	}

	if (DRAW_FTEMP) {
		strcpy(wFeels, _wFeels);
	}
	
	if (DRAW_WIND) {
		strcpy(wWind, _wWind);
	}
	
	if (DRAW_HUMIDITY) {
		strcpy(wHumidity, _wHumidity);
	}
	
	strcpy(wUpdated, _wUpdated);

}

void setup() {

	setenv("TZ", TZ_INFO, 1);
	tzset(); // Assign the local timezone from setenv
	disableCore0WDT(); // Network requests may block long enough to trigger watchdog

	if (firstRun) {

		firstRun = false;

		ntpUpdate();
		time(&waketime);
		lastNtpUpdate = waketime;

		getClock();
		getWeather();
		getVoltage();
		struct tm startTime;
		getLocalTime(&startTime, 0);
		strftime(stime, 40, "Running since %a %b %d %Y %H:%M", &startTime);

		epd_init();
		epd_poweron();
		epd_clear();
		drawClock();
		drawWeather();
		drawVoltage();
		drawStartTime();
		epd_poweroff_all();

		setClock();
		setWeather();
		setVoltage();

	} else {

		time(&waketime);

		if (waketime - lastNtpUpdate >= NTP_INTERVAL) {
			ntpUpdate();
		}
		getClock();
		
		if (waketime - lastVoltageUpdate >= VOLTAGE_INTERVAL) {
			getVoltage();
		}
		
		if (waketime - lastWeatherUpdate >= WEATHER_INTERVAL) {
			getWeather();
		}

		epd_init();
		epd_poweron();
		drawClock();
		if (DRAW_WEATHER) {
			drawWeather();
		}
		if (DRAW_VOLTAGE) {
			drawVoltage();
		}
		epd_poweroff_all();

		setClock();
		if (DRAW_WEATHER) {
			setWeather();
		}
		if (DRAW_VOLTAGE) {
			setVoltage();
		}

	}

	time_t thisrun;
	time(&thisrun);
	esp_sleep_enable_timer_wakeup((60 - (thisrun % 60))  * 1000000);
	esp_deep_sleep_start();

}

void loop() {

}
