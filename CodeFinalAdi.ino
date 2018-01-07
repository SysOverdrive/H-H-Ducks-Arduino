#include <SPI.h>
#include <Ethernet.h>
#include "RestClient.h"

#define IR 2  


int detection = HIGH;
unsigned long previousMillis = 0;
const long interval = 5000;

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// IPAddress ip(192,168,0,105);

//-----------Configuration for Tudor PC---------
IPAddress ip(192, 168, 1, 2);

IPAddress dnss(192, 168, 1, 1);

IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);

//-----------Configuration for Tudor PC---------


RestClient restClient = RestClient("api.backendless.com");






void setup() {
	pinMode(IR, INPUT);
	Serial.begin(9600);
	Serial.println("Start setup..");

	//Ethernet.begin(mac, ip, dnss, gateway, subnet);
	restClient.dhcp(); //Asta merge doar prin router nu si prin pc
	Serial.println("Client setup succesfull");
}

String response;

void loop() {

	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval) {
		//--------------Proximity Sensor--------------------
		detection = digitalRead(IR);
		if (detection == LOW) {
			Serial.print("Obstacle !");
		}
		else {
			Serial.print("No obstacle!\n");
		}
		//--------------Proximity Sensor--------------------

		// save the last time you blinked the LED
		previousMillis = currentMillis;
		Serial.print("\n\n\nSend to server: \n");
		//-------------Rest POST To Backendless-------------
		//restClient.setHeader("content-type:application/json");
		//restClient.setHeader("datac:eeee");
		restClient.setContentType("application/json");
		response = "";
		int statusCode;
		if (detection == LOW) {
			statusCode = restClient.post("/53218FD2-D42B-8A06-FFCE-5E59E4D12B00/1BAD9909-B097-478A-FF74-994896A11000/data/AreThereDucks", "{\"IsThereDucks\":true,\"ownerId\":\"placutaArduino\"}", &response);
		}
		else {
			 statusCode = restClient.post("/53218FD2-D42B-8A06-FFCE-5E59E4D12B00/1BAD9909-B097-478A-FF74-994896A11000/data/AreThereDucks", "{\"IsThereDucks\":false,\"ownerId\":\"placutaArduino\"}", &response);
		}
		//int statusCode = restClient.post("/data", "POSTDATA", &response);
		Serial.print("Status code from server: ");
		Serial.println(statusCode);
		Serial.print("Response body from server: ");
		Serial.println(response);
		//-------------Rest POST To Backendless-------------
		
	}
}
