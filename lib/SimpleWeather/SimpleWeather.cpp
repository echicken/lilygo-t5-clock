#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "SimpleWeather.h"

OpenWeather::OpenWeather(String Key, String City){
	_forecast = false;
	_url = "/data/2.5/weather?q=" + City + "&appid=" + Key +"&units=metric";
}

OpenWeather::OpenWeather(String Key, float lat, float longi){
	_forecast = false;
	_url = "/data/2.5/weather?lat=" + String(lat) + "&long=" + String(longi) + "&appid=" + Key +"&units=metric";
}

OpenWeather::OpenWeather(String Key, String City, boolean forecast){
	_forecast = forecast;
	if(!forecast) {
		_url = "/data/2.5/weather?q=" + City + "&appid=" + Key +"&units=metric&cnt=1";
	} else {
		_url = "/data/2.5/forecast?q=" + City + "&appid=" + Key +"&units=metric&cnt=2";
	}
}

void OpenWeather::updateStatus(weatherData *w){

	const char *openweather = "api.openweathermap.org";
	const int httpsPort = 443;
	WiFiClientSecure httpsClient;
	const size_t capacity = 2*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 6*JSON_OBJECT_SIZE(1) + 3*JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 5*JSON_OBJECT_SIZE(8) + 570;
	httpsClient.setInsecure();
	httpsClient.setTimeout(15000);

	int r = 0;
	while ((!httpsClient.connect(openweather, httpsPort)) && (r < 30)) {
		delay(100);
		r++;
	}
	if (r > 29) {
		return;
	}

	httpsClient.print(String("GET ") + _url + " HTTP/1.1\r\n" + "Host: " + openweather + "\r\n" + "Connection: close\r\n\r\n");

	while (httpsClient.connected()) {
		_Response = httpsClient.readStringUntil('\n');
		if (_Response == "\r") {
			break;
		}
	}

	while (httpsClient.connected()) {
		_Response = httpsClient.readString();
	}
	DynamicJsonDocument doc(capacity);
	DeserializationError err = deserializeJson(doc,_Response);
	if (err.code() != DeserializationError::Ok) {
		return;
	}

	if(!_forecast){
		w->description = doc["weather"][0]["description"].as<String>();
		w->weather = doc["weather"][0]["main"].as<String>();
		w->icon = doc["weather"][0]["icon"].as<String>();
		w->id = doc["weather"][0]["id"].as<int>();
		w->current_Temp = doc["main"]["temp"].as<float>();
		w->feels_like = doc["main"]["feels_like"].as<float>();
		w->min_temp = doc["main"]["temp_min"].as<float>();
		w->max_temp = doc["main"]["temp_max"].as<float>();
		w->humidity = doc["main"]["humidity"].as<int>();
		w->wind_speed = doc["wind"]["speed"].as<float>();
		w->wind_direction = doc["wind"]["deg"].as<int>();
		if(w->id <700) {
			w->rain = doc["rain"]["1h"].as<float>();
		} else {
			w->rain = 0;
		}
	} else {
		// Currently set to get forecast 3 hours from now
		w->description = doc["list"][1]["weather"][0]["description"].as<String>(); 
		w->weather = doc["list"][1]["weather"][0]["main"].as<String>();
		w->id = doc["list"][1]["weather"]["id"].as<int>();
		w->current_Temp = doc["list"][1]["main"]["temp"].as<float>();
		w->min_temp = doc["list"][1]["main"]["temp_min"].as<float>();
		w->max_temp = doc["list"][1]["main"]["temp_max"].as<float>();
		w->humidity = doc["list"][1]["main"]["temp_max"].as<float>();
		if(w->id <700) {
			w->rain = doc["list"][1]["rain"]["3h"].as<float>();
		} else {
			w->rain = 0;
		}
	}
}

String OpenWeather::getResponse() {
	return _Response;
}
