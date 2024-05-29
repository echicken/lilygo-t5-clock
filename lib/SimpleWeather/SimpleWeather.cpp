#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "SimpleWeather.h"

OpenWeather::OpenWeather(String Key, float lat, float longi){
	_url = "/data/3.0/onecall?lat=" + String(lat) + "&lon=" + String(longi) + "&appid=" + Key +"&units=metric&exclude=minutely,hourly,daily,alerts";
}

bool OpenWeather::updateStatus(weatherData *w){

	const char *openweather = "api.openweathermap.org";
	const int httpsPort = 443;
	WiFiClientSecure httpsClient;
	const size_t capacity = 1024;
	httpsClient.setInsecure();
	httpsClient.setTimeout(15000);

	int r = 0;
	while ((!httpsClient.connect(openweather, httpsPort)) && (r < 4)) {
		delay(100);
		r++;
	}
	if (r > 3) {
		return false;
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
		return false;
	}

	if (!doc.containsKey("current")) return false;
	if (!doc["current"].containsKey("weather") && doc["current"]["weather"].size() > 0) {
		w->icon = doc["weather"][0]["icon"].as<String>();
	}
	if (doc["current"].containsKey("temp")) {
		w->current_Temp = doc["current"]["temp"].as<float>();
	}
	if (doc["current"].containsKey("feels_like")) {
		w->feels_like = doc["current"]["feels_like"].as<float>();
	}
	if (doc["current"].containsKey("humidity")) {
		w->humidity = doc["current"]["humidity"].as<int>();
	}
	if (doc["current"].containsKey("wind_speed")) {
		w->wind_speed = doc["current"]["wind_speed"].as<float>();
	}
	if (doc["current"].containsKey("wind_deg")) {
		w->wind_direction = doc["current"]["wind_deg"].as<int>();
	}

	return true;

}

String OpenWeather::getResponse() {
	return _Response;
}

// I'm not hitting the high seas in my schooner anytime soon, so this is precise enough
String OpenWeather::getWindDirection(int deg) {
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

// Map OWM's suggested icons to Meteocons
String OpenWeather::getIcon(String i) {
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