/*
	SimpleWeather.cpp - Library to get weather from Openweather
	Created by Dushyant Ahuja, September 2019.
	Released into the public domain.
*/

#ifndef SimpleWeather_h
#define SimpleWeather_h

#include "Arduino.h"
#include <WiFi.h>

struct weatherData {
	String icon;
	int humidity;
	int wind_direction;
	float current_Temp;
	float feels_like;
	float wind_speed;
};

class OpenWeather {
	public:
		OpenWeather(String Key, String City); 
		OpenWeather(String Key, float lat, float longi);
		bool updateStatus(weatherData *w);
		String getResponse();
		String getWindDirection(int deg);
		String getIcon(String i);
	private:
		String _Response;
		String _url;
};

#endif
