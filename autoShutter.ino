/* shutterAutomation                                                    */
/*                                                                      */
/* Controls my bedroom shutter by a servomotor using an ESP32           */
/* wifi controller and light sensing. It also switch an optional 240ac  */
/* fan via a SSR                                                        */
/*                                                                      */
/* Releases:                                                            */
/*      -26/05/24 v0.1 early commit, only wifi command. servo need      */
/*      turned off. slow 50ms timeout                                   */
/*                                                                      */
/* @ 2024 Ivan Mella. ivan.m48@gmail.com                                */

#include <WiFi.h>
#include "mg995.h"
/* The html file.                                                       */
#include "html.h"

/* create servo object to control a servo                               */
mg995 myservo;
/* 16 servo objects can be created on the ESP32                         */

/* variable to store the servo position                                 */
int pos = 0;

/* Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,      */
/* 25-27,32-33                                                          */
/* Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),  */
/* 1-17,18(used by on-board LED),19-21,26,33-42                         */
/* Possible PWM GPIO pins on the ESP32-S3: 0(used by on-board button),  */
/* 1-21,35-45,47,48(used by on-board LED)                               */
/* Possible PWM GPIO pins on the ESP32-C3: 0(used by on-board button),  */
/* 1-7,8(used by on-board LED),9-10,18-21                               */

#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
int servoPin = 18;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
int servoPin = 7;
#else
int servoPin = 18;
#endif

/* Network configuration                                                */
const char* ssid     = "GL-MT300N-V2-10e";
const char* password = "goodlife";

/* Set web server port number to 80                                     */
WiFiServer server(80);

/* Variable to store the HTTP request                                   */
String header;

/* Decode HTTP GET value                                                */
String valueString = String(5);
int value = -1;                 // boot default
int pos1 = 0;
int pos2 = 0;

// analog light variables 
constexpr int pin_A0 = 36;
// automa variables 
long automaTimeout = 0;
bool automaTrigger = 0;

// fan variables
constexpr int fanPin = 23;
bool fanSwitch = 0;

/* Current time                                                         */
unsigned long currentTime = millis();

/* Previous time                                                        */
unsigned long previousTime = 0; 

/* Define timeout time in milliseconds (example: 2000ms = 2s)           */
const long timeoutTime = 2000;

/* Servo timeout                                                        */
unsigned long servoTargetTime = 0;
/* 50ms within every servo update                                       */
const unsigned long servoTimeout = 30;

void setup()
{
        /*                      **** WIFI *****                         */
	Serial.begin(115200);
  
 	/* Connect to Wi-Fi network with SSID and password              */
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	/* Print local IP address and start web server                  */
	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	server.begin();

        /*                      **** SERVO *****                        */                                  
	/* Allow allocation of all timers                               */
	// ESP32PWM::allocateTimer(0);
	// ESP32PWM::allocateTimer(1);
	// ESP32PWM::allocateTimer(2);
	// ESP32PWM::allocateTimer(3);
        pinMode(fanPin, OUTPUT);
	/* standard 50 hz servo                                         */
	// myservo.setPeriodHertz(50); 

        /* attaches the servo on pin  18  to  the  servo  object  using */
        /* default min/max of 1000us and 2000us  different  servos  may */
        /* require different min/max settings for an accurate 0 to  180 */
        /* sweep                                                        */
        myservo.attach(servoPin, 10);
        Serial.print("Attached: "); 
        Serial.println(servoPin); 
}

void loop() 
{
        /* Listen for incoming clients                                  */
	WiFiClient client = server.accept();
        
        /* If a new client connects,                                    */
	if (client)
        {                             
		currentTime = millis();
		previousTime = currentTime;
                /* print a message out in the serial port               */
		Serial.println("New Client.");
                /* make a String to hold incoming data from the client  */
		String currentLine = "";
                /* loop while the client's connected                    */
		while (client.connected() && currentTime - previousTime <= timeoutTime) 
                {
			currentTime = millis();
                        /* if there's bytes to read from the client,    */
			if (client.available()) {
                                /* read a byte, then                    */
				char c = client.read();
                                /* print it out the serial monitor      */
				Serial.write(c);
				header += c;
                                /* if the byte is a newline character   */
				if (c == '\n') 
                                {                                        
					/* if the current line is blank, you got two newline characters in a row.       */
					/* that's the end of the client HTTP request, so send a response:               */
					if (currentLine.length() == 0) {
                                                /* Send whole html text file */        
						client.print(html);
						
						/* GET /?value=180& HTTP/1.1                                            */
                                                if(header.indexOf("GET /?value=") >= 0) {
							pos1 = header.indexOf('=');
							pos2 = header.indexOf('&');
                                                        /* Set the servo value                                          */
							valueString = header.substring(pos1+1, pos2);
                                                        Serial.println(valueString); 
                                                        if(valueString.length() <= 3)
                                                                value = valueString.toInt();
						}         
						/* The HTTP response ends with another blank line                       */
						client.println();
						/* Break out of the while loop                                          */
						break;
					} else { 
                                                /* if you got a newline, then clear currentLine                         */
						currentLine = "";
					}
                                /* if you got anything else but a carriage return character,                            */
				} else if (c != '\r') {
                                        /* add it to the end of the currentLine                                         */
					currentLine += c;
				}
			}
		}

                //triggers the fan frow webpage
                if(valueString == "switch")
                {
                        fanSwitch = !fanSwitch;
                        Serial.print("Fan :");
                        Serial.println(fanSwitch);
                        digitalWrite(fanPin, fanSwitch);
                }

		/* Clear the header variable    */
		header = "";
		/* Close the connection         */
		client.stop();
		Serial.println("Client disconnected.");
		Serial.println("");
	}
        
        // updates servo position
        myservo.write(value);


        // automata every 1.5 sec. opens and turns on fan on dusk
        if ((millis() - automaTimeout) >= 1500)
        {
                automaTimeout = millis();
                //reads light emission
                uint16_t BitsA0 = analogRead(pin_A0);
                Serial.println(BitsA0);

                if (!automaTrigger && BitsA0 > 1700)
                {
                        value = 0;
                        automaTrigger = 1;
                        digitalWrite(fanPin, 0);
                }
                else if (automaTrigger && BitsA0 < 1200)
                {
                        value = 90;
                        automaTrigger = 0;
                        digitalWrite(fanPin, 1);
                }
               
        }
       

}        



