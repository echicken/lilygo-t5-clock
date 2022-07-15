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

struct tm now;
time_t waketime;
enum alignment { LEFT, RIGHT, CENTER };

RTC_DATA_ATTR bool firstRun = true;
RTC_DATA_ATTR bool redrawing = false;
RTC_DATA_ATTR int minute = -1;
RTC_DATA_ATTR int dayOfWeek = -1;
RTC_DATA_ATTR int vref = 1100;
RTC_DATA_ATTR float voltage = 0;
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

void updateClock();
void updateVoltage();
void ntpUpdate();
void updateWeather();
void redraw();

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
	if (!redrawing && !firstRun) {
		clearString(x, y, old_text, align);
	}
	drawString(x, y, text, align);
}

void drawClock() {

	setFont(NK5772B);

	if (redrawing) {

		drawString(CLOCK_X, CLOCK_Y, tod, LEFT);
		setFont(NK5724B);
		drawString(DATE_X, DATE_Y1, dow, RIGHT);
		drawString(DATE_X, DATE_Y2, mdy, RIGHT);

	} else {

		char _tod[10];
		strftime(_tod, 10, "%H:%M", &now);
		drawString(CLOCK_X, CLOCK_Y, _tod, tod, LEFT);
		strcpy(tod, _tod);

		setFont(NK5724B);

		if (dayOfWeek != now.tm_wday) {
			char _dow[20];
			strftime(_dow, 20, "%A", &now);
			drawString(DATE_X, DATE_Y1, _dow, dow, RIGHT);
			dayOfWeek = now.tm_wday;
			strcpy(dow, _dow);

			char _mdy[50];
			strftime(_mdy, 50, "%b %d %Y", &now);
			drawString(DATE_X, DATE_Y2, _mdy, mdy, RIGHT);
			strcpy(mdy, _mdy);
		}

	}

}

void drawStartTime() {
	setFont(NK5715B);
	drawString(START_TIME_X, START_TIME_Y, stime, stime, RIGHT);
}

void updateClock() {
	getLocalTime(&now, 0);
	drawClock();
}

void drawVoltage() {
	setFont(NK5715B);
	if (redrawing) {
		drawString(VOLTAGE_X, VOLTAGE_Y, volts, LEFT);
	} else {
		char _volts[10];
		sprintf(_volts, "%.2fV", voltage);
		drawString(VOLTAGE_X, VOLTAGE_Y, _volts, volts, LEFT);
		strcpy(volts, _volts);
	}
}

void updateVoltage() {
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
		voltage = _voltage;
		drawVoltage();
	}
	lastVoltageUpdate = waketime;
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
	updateClock();

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

	String icon = weatherIcon(w.icon);
	if (redrawing || wIcon != icon) {
		wIcon = icon;
		setFont(Meteocons96);
		if (redrawing) {
			drawString(WICON_X, WICON_Y, wIcon, LEFT);
		} else {
			epd_clear_area(WICON_AREA);
			drawString(WICON_X, WICON_Y, wIcon, LEFT);
		}
	}

	char _temp[10];
	sprintf(_temp, "%.1fc", w.current_Temp);
	if (redrawing || strcmp(wTemp, _temp) != 0) {
		setFont(NK5748B);
		if (redrawing) {
			drawString(CTEMP_X, CTEMP_Y, _temp, LEFT);	
		} else {
			drawString(CTEMP_X, CTEMP_Y, _temp, wTemp, LEFT);
			strcpy(wTemp, _temp);
		}
	}

	setFont(NK5715B);

	char _feels[20];
	sprintf(_feels, "Feels like %.1fc", w.feels_like);
	if (redrawing || strcmp(wFeels, _feels) != 0) {
		if (redrawing) {
			drawString(FTEMP_X, FTEMP_Y, _feels, LEFT);
		} else {
			drawString(FTEMP_X, FTEMP_Y, _feels, wFeels, LEFT);
			strcpy(wFeels, _feels);
		}
	}

	char _wind[25];
	int ws = w.wind_speed * 3.6;
	String wd = windDirection(w.wind_direction);
	sprintf(_wind, "Wind: %d km/h %s", ws, wd);
	if (redrawing || strcmp(wWind, _wind) != 0) {
		if (redrawing) {
			drawString(WIND_X, WIND_Y, _wind, RIGHT);
		} else {
			drawString(WIND_X, WIND_Y, _wind, wWind, RIGHT);
			strcpy(wWind, _wind);
		}
	}

	char _humid[20];
	sprintf(_humid, "Humidity: %d%%", w.humidity);
	if (redrawing || strcmp(wHumidity, _humid) != 0) {
		if (redrawing) {
			drawString(HUMID_X, HUMID_Y, _humid, RIGHT);
		} else {
			drawString(HUMID_X, HUMID_Y, _humid, wHumidity, RIGHT);
			strcpy(wHumidity, _humid);
		}
	}

	if (redrawing) {
		drawString(WUPDATE_X, WUPDATE_Y, wUpdated, RIGHT);	
	} else {
		struct tm now;
		getLocalTime(&now, 0);
		char _updated[20];
		strftime(_updated, 20, "Updated: %H:%M", &now);
		drawString(WUPDATE_X, WUPDATE_Y, _updated, wUpdated, RIGHT);
		strcpy(wUpdated, _updated);
	}

}

void updateWeather() {
	enableWifi();
	weather.updateStatus(&w);
	disableWifi();
	drawWeather();
	lastWeatherUpdate = waketime;
}

void redraw() {
	redrawing = true;
	epd_clear();
	drawClock();
	drawWeather();
	drawVoltage();
	drawStartTime();
	lastRedraw = waketime;
	redrawing = false;
}

void setup() {

	setenv("TZ", TZ_INFO, 1);
	tzset(); // Assign the local timezone from setenv
	disableCore0WDT(); // Network requests may block long enough to trigger watchdog

	epd_init();
	epd_poweron();

	if (firstRun) {

		epd_clear();

		ntpUpdate();
		time(&waketime);

		struct tm startTime;
		getLocalTime(&startTime, 0);
		strftime(stime, 40, "Running since %a %b %d %Y %H:%M", &startTime);

		updateVoltage();
		drawStartTime();
		updateWeather();

		firstRun = false;
		lastNtpUpdate = waketime;
		lastRedraw = waketime;

	} else {

		time(&waketime);

		if (waketime - lastNtpUpdate >= NTP_INTERVAL) {
			ntpUpdate(); // Will updateClock()
		} else {
			updateClock();
		}
		
		if (waketime - lastVoltageUpdate >= VOLTAGE_INTERVAL) {
			updateVoltage();
		}
		
		if (waketime - lastWeatherUpdate >= WEATHER_INTERVAL) {
			updateWeather();
		}

		if (waketime - lastRedraw >= REDRAW_INTERVAL) {
			redraw();
		}
	
	}

	epd_poweroff_all();

	time_t thisrun;
	time(&thisrun);
	esp_sleep_enable_timer_wakeup((60 - (thisrun % 60))  * 1000000);
	esp_deep_sleep_start();

}

void loop() {

}
