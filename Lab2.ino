/*
  WiFi Web Server LED Blink
 
 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.
 
 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Circuit:
 * WiFi shield attached
 * LED attached to pin 9
 
 created 25 Nov 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFi.h>
#include <Servo.h> 

//char ssid[] = "TC8715DEC";      //  your network SSID (name) 
//char pass[] = "TC8715D993DEC";   // your network password
char ssid[] = "UCLA_WEB_RES";      //  your network SSID (name) 
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

Servo myservo;
int pos = 75;
int flag = 0;

//music part
int speakerPin = 2;                  // Grove Buzzer connect to D2
int length = 37; // the number of notes

//char notes[] = "abCbCEbeagaCgefCbCDECCbab0a"; // a space represents a rest 27
//int beats[] = { 1,1,3,1,2,2,6,2,3,1,2,2,6,2,2,1,3,2,2,1,5,1,1,2,2,2,8};
//char notes[] = "eabCbCCDbabbCagageeabCbCCDbabbCagaga"; // a space represents a rest 36
//int beats[] = { 1,1,1,3,1,4,1,1,1,1,4,1,1,1,1,3,1,5,1,1,1,3,1,4,1,1,1,1,4,1,1,1,1,3,1,5};
char notes[] = "eabCDbCaba0abCbCDEEEDEFbCDEaabCCDbbCa"; // a space represents a rest 37
int beats[] = { 1,1,1,3,1,3,1,5,1,1,1,1,1,1,1,1,1,2,2,8,1,1,4,2,1,1,4,2,1,1,2,1,1,2,1,1,5};

int tempo = 100;
double duty_cycle = 0.01;

void playTone(int tone, int duration, double duty_cycle) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds((int)(duty_cycle * 2 * tone));
        digitalWrite(speakerPin, LOW);
        delayMicroseconds((int)(2 * tone - duty_cycle*2*tone));
    }
}

void playNote(char note, int duration, double duty_cycle) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', '1', '2', '3', '4', '5', '6', '7','8','0'};
    //int tones[] = { 1912, 1700, 1519, 1432, 1275, 1136, 1014, 956, 852,759,716, 638, 568, 506,478,426,379,358,319,284,253,239,1205};
    int tones[] = { 1912, 1700, 1530, 1432, 1275, 1150, 1014, 930, 820, 729,690, 620, 540, 490,458,406,349,358,319,284,253,239,1205};
    // play the tone corresponding to the note name
    for (int i = 0; i < 23; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration, duty_cycle);
        }
    }
}

//music part ends

int status = WL_IDLE_STATUS;
WiFiServer server(50);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  myservo.attach(6);
  myservo.write(pos);
  pinMode(speakerPin, OUTPUT);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true);        // don't continue
  } 

  String fv = WiFi.firmwareVersion();
  if( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    //status = WiFi.begin(ssid, pass);
    status = WL_CONNECTED;
    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}

int get_flag(WiFiClient client){
  if(client){   
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {  
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:    
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/Speed1\">here</a> for speed on<br>");
            client.print("Click <a href=\"/Speed2\">here</a> for speed two<br>");
            client.print("Click <a href=\"/Speed3\">here</a> for speed three<br>");
            client.print("Click <a href=\"/Speed4\">here</a> for speed four<br>");
            client.print("Click <a href=\"/Speed5\">here</a> for speed five<br>");
            client.print("Click <a href=\"/Speed6\">here</a> for speed six<br>");
            client.print("Click <a href=\"/Speed7\">here</a> for speed seven<br>");
            client.print("Click <a href=\"/Beats\">here</a> for speed beats playing<br>");
            client.print("Click <a href=\"/Play\">here</a> for song playing<br>");
            client.print("Click <a href=\"/Pause\">here</a> to pause<br>");
            client.print("Click <a href=\"/VolumUp\">here</a> to increase volumn<br>");
            client.print("Click <a href=\"/VolumDown\">here</a> to decrease volumn<br>");
            client.print("Click <a href=\"/SpeedUp\">here</a> to increase speed<br>");
            client.print("Click <a href=\"/SpeedDown\">here</a> to decrease speed<br>");
            client.print("Click <a href=\"/Stop\">here</a> to turn off <br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;         
          } 
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request:
        if (currentLine.endsWith("GET /Speed1")) 
          flag=1;
        if (currentLine.endsWith("GET /Speed2")) 
          flag=2;
        if (currentLine.endsWith("GET /Speed3")) 
          flag=3;
        if (currentLine.endsWith("GET /Speed4")) 
          flag=4;
        if (currentLine.endsWith("GET /Speed5")) 
          flag=5;       
        if (currentLine.endsWith("GET /Speed6")) 
          flag=6;       
        if (currentLine.endsWith("GET /Speed7")) 
          flag=7;       
        if (currentLine.endsWith("GET /Beats"))
          flag=8;        
        if (currentLine.endsWith("GET /Play")) 
          flag=-1;    
        if (currentLine.endsWith("GET /Pause")) 
          flag=-2;
        if (currentLine.endsWith("GET /VolumnUp")) 
          flag=-3;    
        if (currentLine.endsWith("GET /VolumnDown")) 
          flag=-4;
        if (currentLine.endsWith("GET /SpeedUp")) 
          flag=-5;    
        if (currentLine.endsWith("GET /SpeedDown")) 
          flag=-6;
        if (currentLine.endsWith("GET /Stop")) 
          flag=0;        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");    
  }
  return flag;
}

void get_beat(int x, int time_x){
  myservo.write(pos-x);
  delay(time_x);
  myservo.write(pos);
  delay(time_x);  
}

void beats_play1(int *time_arr){
  get_beat(60,time_arr[3]);
  get_beat(60,time_arr[3]); 
  delay(150); 
  get_beat(30,time_arr[5]);  
  get_beat(30,time_arr[5]); 
  get_beat(30,time_arr[5]);
  delay(time_arr[3]);
  get_beat(30,time_arr[5]);  
  get_beat(30,time_arr[5]); 
  get_beat(30,time_arr[5]); 
  get_beat(30,time_arr[5]);
  delay(time_arr[3]); 
  get_beat(30,time_arr[5]); 
  get_beat(30,time_arr[5]);
  delay(time_arr[3]);
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  int arr[]={75,75,75,60,40,25,10};
  int time_arr[]={1000,800,500,400,300,200,100};
  int hold1=0,hold2=0,hold3=0,hold4=0;
  flag=get_flag(client);
  while(flag==-1){
      for (int i = 0; i < length; i++) 
    {
        if (notes[i] == ' ')
        {
            delay(beats[i] * tempo); // rest
        }
        else
        {
            playNote(notes[i], beats[i] * tempo, duty_cycle);
        }

        // pause between notes
        myservo.write(50);
        delay(tempo/2);
        myservo.write(pos);
        delay(tempo/2);
        client = server.available();
        flag=get_flag(client);
        while(flag==-2){
          client = server.available();
          flag=get_flag(client);
        }
        if(flag == -3){
          if(hold1 == 0)
            duty_cycle =  duty_cycle > 0.45 ? duty_cycle+0.05 : 0.5;
          hold2 = hold3 = hold4 =0;
          hold1 = 1;
          flag=-1;
        }
        else if(flag == -4){
          if(hold2 == 0)
            duty_cycle =  duty_cycle < 0.06 ? duty_cycle-0.05 : 0.01;
          hold1 = hold3 = hold4 =0;
          hold2 = 1;
          flag=-1;
        }
        else if(flag == -5){
          if(hold3 == 0)
            tempo = tempo < 90 ? 70 : tempo-20;
          hold1 = hold2 = hold4 =0;
          hold3 = 1;
          flag=-1;
        }
        else if(flag == -6){
          if(hold4 == 0)
            tempo = tempo > 500 ? 500 : tempo+20;
          hold1 = hold2 = hold3 =0;
          hold4 = 1;
          flag=-1;
        }
        else if(flag >=0 )
          break;
        else{
          hold1 = hold2 = hold3 = hold4 = 0;
        }
    }
  }   
       while(flag>0){
          if(flag!=8){
            myservo.write(pos-arr[flag-1]);
            delay(time_arr[flag-1]);
            myservo.write(pos);
            delay(time_arr[flag-1]);             
          }
          else{
            beats_play1(time_arr);            
          }
          client = server.available();
          flag=get_flag(client);  
       }
       if(flag==0)
          myservo.write(pos);
  
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
