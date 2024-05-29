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
#include "batt_100.h"
#include "batt_75.h"
#include "batt_50.h"
#include "batt_25.h"
#include "batt_0.h"

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
const uint BATT_X = EPD_WIDTH - H_MARGIN - batt_100_width;
const uint BATT_Y = EPD_HEIGHT - V_MARGIN - batt_100_height;
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
	.x = H_MARGIN,
	.y = (int32_t)(EPD_HEIGHT / 2) - 10,
	.width = (int32_t)(CTEMP_X - H_MARGIN - 5),
	.height = (int32_t)(BATT_Y - (EPD_HEIGHT / 2) + 10),
};

const Rect_t BATT_AREA = {
	.x = BATT_X,
	.y = BATT_Y,
	.width = batt_100_width,
	.height = batt_100_height,
};

bool _drawDate = false;
bool _drawWeather = false;
bool _drawWicon = false;
bool _drawTemp = false;
bool _drawFtemp = false;
bool _drawWind = false;
bool _drawHumidity = false;
bool _drawVoltage = false;
char _tod[10];
char _dow[20];
char _mdy[50];
char _wIcon[5];
char _wTemp[10];
char _wFeels[20];
char _wWind[25];
char _wHumidity[20];
char _wUpdated[20];
uint8_t _batt = 0;
time_t waketime;
enum alignment { LEFT, RIGHT, CENTER };

RTC_DATA_ATTR bool firstRun = true;
RTC_DATA_ATTR int minute = -1;
RTC_DATA_ATTR int dayOfWeek = -1;
RTC_DATA_ATTR int vref = 1100;
RTC_DATA_ATTR int batt = 5;
RTC_DATA_ATTR float voltage = -1;
RTC_DATA_ATTR char tod[10];
RTC_DATA_ATTR char dow[20];
RTC_DATA_ATTR char mdy[50];
RTC_DATA_ATTR char wIcon[5];
RTC_DATA_ATTR char wTemp[10];
RTC_DATA_ATTR char wFeels[20];
RTC_DATA_ATTR char wWind[25];
RTC_DATA_ATTR char wHumidity[20];
RTC_DATA_ATTR char wUpdated[20];
RTC_DATA_ATTR time_t lastNtpUpdate;
RTC_DATA_ATTR time_t lastVoltageUpdate;
RTC_DATA_ATTR time_t lastWeatherUpdate;
RTC_DATA_ATTR time_t lastRedraw;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
GFXfont currentFont;
weatherData w;
OpenWeather weather(OWM_KEY, OWM_LAT, OWM_LON);

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

void redrawClock() {
	setFont(NK5772B);	
	drawString(CLOCK_X, CLOCK_Y, tod, LEFT);
	setFont(NK5724B);
	drawString(DATE_X, DATE_Y1, dow, RIGHT);
	drawString(DATE_X, DATE_Y2, mdy, RIGHT);
}

void drawClock() {
	setFont(NK5772B);
	drawString(CLOCK_X, CLOCK_Y, _tod, tod, LEFT);
	if (_drawDate) {
		setFont(NK5724B);
		drawString(DATE_X, DATE_Y1, _dow, dow, RIGHT);
		drawString(DATE_X, DATE_Y2, _mdy, mdy, RIGHT);
	}
}

void getClock() {
	struct tm now;
	getLocalTime(&now);
	strftime(_tod, 10, "%H:%M", &now);
	if (dayOfWeek != now.tm_wday) {
		_drawDate = true;
		dayOfWeek = now.tm_wday;
		strftime(_dow, 20, "%A", &now);
		strftime(_mdy, 50, "%b %d %Y", &now);
	}
}

void setClock() {
	strcpy(tod, _tod);
	if (_drawDate) {
		strcpy(dow, _dow);
		strcpy(mdy, _mdy);
	}
}

void getVoltage() {
	// Correct the ADC reference voltage
	esp_adc_cal_characteristics_t adc_chars;
	esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
	if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) vref = adc_chars.vref;
	delay(10); // Make adc measurement more accurate
	uint16_t v = analogRead(BATT_PIN);
	float _voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
	if (_voltage != voltage) {
		voltage = _voltage;
		// These values were arrived at via careful reasoning and due consideration, I assure you. I absolutely did not just make them up.
		if (voltage < 3.7) {
			_batt = 0;
		} else if (voltage < 3.8) {
			_batt = 1;
		} else if (voltage < 3.9) {
			_batt = 2;
		} else if (voltage < 4) {
			_batt = 3;
		} else {
			_batt = 4;
		}
		if (batt != _batt) {
			batt = _batt;
			_drawVoltage = true;
		}
	}
	time(&lastVoltageUpdate);
}

void redrawVoltage() {
	if (batt == 0) {
		epd_draw_grayscale_image(BATT_AREA, (uint8_t *)batt_0_data);
	} else if (batt == 1) {
		epd_draw_grayscale_image(BATT_AREA, (uint8_t *)batt_25_data);
	} else if (batt == 2) {
		epd_draw_grayscale_image(BATT_AREA, (uint8_t *)batt_50_data);
	} else if (batt == 3) {
		epd_draw_grayscale_image(BATT_AREA, (uint8_t *)batt_75_data);
	} else {
		epd_draw_grayscale_image(BATT_AREA, (uint8_t *)batt_100_data);
	}
}

void drawVoltage() {
	epd_clear_area(BATT_AREA);
	redrawVoltage();
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
	bool updated = timeClient.update();
	timeClient.end();
	disableWifi();

	if (updated) setUnixtime(timeClient.getEpochTime());

	time(&lastNtpUpdate);

}

void redrawWeather() {
	setFont(Meteocons96);
	drawString(WICON_X, WICON_Y, wIcon, LEFT);
	setFont(NK5748B);
	drawString(CTEMP_X, CTEMP_Y, wTemp, LEFT);
	setFont(NK5715B);
	drawString(FTEMP_X, FTEMP_Y, wFeels, LEFT);
	drawString(WIND_X, WIND_Y, wWind, RIGHT);
	drawString(HUMID_X, HUMID_Y, wHumidity, RIGHT);
	drawString(WUPDATE_X, WUPDATE_Y, wUpdated, RIGHT);
}

void drawWeather() {

	setFont(Meteocons96);
	if (_drawWicon) {
		epd_clear_area(WICON_AREA);
		drawString(WICON_X, WICON_Y, wIcon, LEFT);
	}

	setFont(NK5748B);

	if (_drawTemp) {
		drawString(CTEMP_X, CTEMP_Y, _wTemp, wTemp, LEFT);
	}

	setFont(NK5715B);

	if (_drawFtemp) {
		drawString(FTEMP_X, FTEMP_Y, _wFeels, wFeels, LEFT);
	}

	if (_drawWind) {
		drawString(WIND_X, WIND_Y, _wWind, wWind, RIGHT);
	}

	if (_drawHumidity) {
		drawString(HUMID_X, HUMID_Y, _wHumidity, wHumidity, RIGHT);
	}

	drawString(WUPDATE_X, WUPDATE_Y, _wUpdated, wUpdated, RIGHT);

}

void getWeather() {

	enableWifi();
	bool updated = weather.updateStatus(&w);
	disableWifi();

	time(&lastWeatherUpdate);
	struct tm now;
	getLocalTime(&now);

	if (updated) {

		_drawWeather = true;

		sprintf(_wIcon, "%s", weather.getIcon(w.icon));
		if (wIcon != _wIcon) _drawWicon = true;

		sprintf(_wTemp, "%.1fc", w.current_Temp);
		if (strcmp(wTemp, _wTemp) != 0) _drawTemp = true;

		sprintf(_wFeels, "Feels like %.1fc", w.feels_like);
		if (strcmp(wFeels, _wFeels) != 0) _drawFtemp = true;

		int ws = w.wind_speed * 3.6;
		String wd = weather.getWindDirection(w.wind_direction);
		sprintf(_wWind, "Wind: %d km/h %s", ws, wd);
		if (strcmp(wWind, _wWind) != 0) _drawWind = true;

		sprintf(_wHumidity, "Humidity: %d%%", w.humidity);
		if (strcmp(wHumidity, _wHumidity) != 0) _drawHumidity = true;

		strftime(_wUpdated, 20, "Updated: %H:%M", &now);

	} else {

		strftime(_wUpdated, 20, "! Updated: %H:%M", &now);

	}
	
}

void setWeather() {
	if (_drawWicon) strcpy(wIcon, _wIcon);
	if (_drawTemp) strcpy(wTemp, _wTemp);
	if (_drawFtemp) strcpy(wFeels, _wFeels);
	if (_drawWind) strcpy(wWind, _wWind);
	if (_drawHumidity) strcpy(wHumidity, _wHumidity);
	strcpy(wUpdated, _wUpdated);
}

void redraw() {
	epd_init();
	epd_poweron();
	epd_clear();
	redrawClock();
	redrawWeather();
	if (firstRun) getVoltage();
	redrawVoltage();
	epd_poweroff_all();
	time(&lastRedraw);
}

void partialRedraw() {
	epd_init();
	epd_poweron();
	drawClock();
	if (_drawWeather) drawWeather();
	if (waketime - lastVoltageUpdate >= VOLTAGE_INTERVAL) {
		getVoltage();
		if (_drawVoltage) drawVoltage();
	}
	epd_poweroff_all();
}

void setup() {

	setenv("TZ", TZ_INFO, 1);
	tzset(); // Assign the local timezone from setenv
	disableCore0WDT(); // Network requests may block long enough to trigger watchdog

	if (firstRun) {
		ntpUpdate();
		time(&waketime);
		getClock();
		setClock();
		getWeather();
		setWeather();
		redraw();
		firstRun = false;
	} else {
		time(&waketime);
		bool r = waketime - lastRedraw >= REDRAW_INTERVAL;
		if (waketime - lastNtpUpdate >= NTP_INTERVAL) {
			ntpUpdate();
			time(&waketime);
		}
		if (waketime - lastWeatherUpdate >= WEATHER_INTERVAL) getWeather();
		getClock();
		if (!r) partialRedraw();
		setClock();
		if (_drawWeather) setWeather();
		if (r) redraw();
	}

	int nextRun = (60 - (waketime % 60));
	if (nextRun < 5) nextRun += 5;

	esp_sleep_enable_timer_wakeup(nextRun  * 1000000);
	esp_deep_sleep_start();

}

void loop() {

}
