/* shutterAutomation                                                    */
/*                                                                      */
/* controls my dormroom shutter opening and a blower via webpage,       */
/* an extra photoresistor automate open and close by the sunlight       */
/*                                                                      */
/* Releases:                                                            */
/*      -26/05/24 v0.1 early commit, only wifi command. servo need      */
/*      turned off. slow 50ms timeout                                   */
/*                                                                      */
/* @ 2024 Ivan Mella. ivan.m48@gmail.com                                */

#include <WiFi.h>
#include <ESP32Servo.h>

/* create servo object to control a servo                               */
Servo myservo;
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
int servoPin = 17;
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
int value = 0;
int pos1 = 0;
int pos2 = 0;



/* Current time                                                         */
unsigned long currentTime = millis();

/* Previous time                                                        */
unsigned long previousTime = 0; 

/* Define timeout time in milliseconds (example: 2000ms = 2s)           */
const long timeoutTime = 2000;

/* Servo timeout                                                        */
unsigned long servoTargetTime = 0;
/* 50ms within every servo update                                       */
const unsigned long servoTmeout = 50;

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
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

	/* standard 50 hz servo                                         */
	myservo.setPeriodHertz(50); 

	/* attaches the servo on pin  18  to  the  servo  object  using */
        /* default min/max of 1000us and 2000us  different  servos  may */
        /* require different min/max settings for an accurate 0 to  180 */
        /* sweep                                                        */
	myservo.attach(servoPin, 0, 20000);
        Serial.print("Attached: "); 
        Serial.println(servoPin); 
}

void loop() 
{
        /* Listen for incoming clients                                  */
	WiFiClient client = server.available();
        
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
						/* HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)        */ 
						/* and a content-type so the client knows what's coming, then a blank line:     */
						client.println("HTTP/1.1 200 OK");
						client.println("Content-type:text/html");
						client.println("Connection: close");
						client.println();

						/* Display the HTML web page                            */
						client.println("<!DOCTYPE html><html>");
						client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
						client.println("<link rel=\"icon\" href=\"data:,\">");
						/* CSS to style the on/off buttons                                                              */
						/* Feel free to change the background-color and font-size attributes to fit your preferences    */
						client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
						client.println(".slider { width: 300px; }</style>");
						client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
										 
						/* Web Page                                                                                     */
						client.println("</head><body><h1>Tapparella Sesame</h1>");
						client.println("<p>Position: <span id=\"servoPos\"></span></p>");          
						client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
						
						client.println("<script>var slider = document.getElementById(\"servoSlider\");");
						client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
						client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
						client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
						client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
					 
						client.println("</body></html>");     
						
						/* GET /?value=180& HTTP/1.1                                                            */
						if(header.indexOf("GET /?value=")>=0) {
							pos1 = header.indexOf('=');
							pos2 = header.indexOf('&');
                                                        /* Set the servo value                                          */
							valueString = header.substring(pos1+1, pos2);
                                                        value = valueString.toInt();
				
							Serial.println(valueString); 
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
		/* Clear the header variable    */
		header = "";
		/* Close the connection         */
		client.stop();
		Serial.println("Client disconnected.");
		Serial.println("");
	}
        /* Rotate servo slowly                                          */ 
        while(pos != value && millis() > servoTargetTime)
        {
                if(pos < value)
                {
                        pos++;
                }
                else if (pos > value)
                {
                        pos--;        
                }

                Serial.println(pos);
                myservo.write(pos);
                /* Delay 50ms                                           */
                servoTargetTime = millis() + servoTimeout;
        }
}        



