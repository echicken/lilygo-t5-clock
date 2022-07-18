const uint32_t batt_50_width = 73;
const uint32_t batt_50_height = 40;
const uint8_t batt_50_data[(74*40)/2] = {
0xDF, 0x16, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x41, 0xFB, 0xFF, 0xFF, 0xFF, 0x0F, 
	0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0xFF, 0xFF, 0xFF, 0x0F, 
	0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xB7, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAB, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xEE, 0xEE, 0xEE, 0xFE, 0xFF, 0xFF, 0xEF, 0xEE, 0xEE, 0xEE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xEC, 0xEE, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0x2F, 0x00, 0x00, 0x00, 0x90, 0xFF, 0xFF, 0x06, 0x00, 0x00, 0x00, 0xF5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
	0x01, 0x00, 0xFB, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 
	0x01, 0x00, 0xFB, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x90, 0xFF, 0xFF, 0x06, 0x00, 0x00, 0x00, 0xF5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xEE, 0xEE, 0xEE, 0xFE, 0xFF, 0xFF, 0xEF, 0xEE, 0xEE, 0xEE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xEC, 0xFE, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0xB7, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xAB, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0x0F, 
	0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 
	0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xDF, 0x27, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x52, 0xFB, 0xFF, 0xFF, 0xFF, 0x0F, 
	};