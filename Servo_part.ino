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
char ssid[] = "eduroam";      //  your network SSID (name) 
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

Servo myservo;
int pos = 75;
int flag = 0;
int tempo;
int tempo_set[] = {250,200,100};
int beats_count[]={66,64,68};
int *servo_beats;
char notes1[] = "abCbCEbeagaCgefCbCDECCbab0a"; // a space represents a rest 27
int beats1[] = { 1,1,3,1,2,2,6,2,3,1,2,2,6,2,2,1,3,2,2,1,5,1,1,2,2,2,8};
int servo_beats1[] = {0,0,1,0,0, 0,1,0,0,0, 1,0,1,0,1,0, 0,0,1,0,0,0,1,0,0,0, 1,0,1,0,1,0, 0,0,1,1, 0, 1,0,0, 1,0, 1,1, 0, 1,0,0,0,1, 1,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0};
char notes2[] = "eabCbCCDbabbCagageeabCbCCDbabbCagaga"; // a space represents a rest 36
int beats2[] = { 1,1,1,3,1,4,1,1,1,1,4,1,1,1,1,3,1,5,1,1,1,3,1,4,1,1,1,1,4,1,1,1,1,3,1,5};
int servo_beats2[]={0,0,0,0,1,1,0,0,1,1,0, 0,1,0,1,0,1,1,0, 0,1,0,1,0,1,1,0, 1,0,0,0,1, 0,0,0,0,1,1,0,0,1,1,0,0,1,0,1,0,1,1,0,0,1,0,1, 0,1,1,0,1,0,0,0,1};
char notes3[] = "eabCDbCaba0abCbCDEEEDEFbCDEaabCCDbbCa"; // a space represents a rest 37
int beats3[] = { 1,1,1,3,1,3,1,5, 1,1,1,1,1,1,1,1,1,2,2,8, 1,1,4, 2,1,1,4, 2,1,1,2,1,1,2,1,1,5};
int servo_beats3[] = {0,0,0,0,1,1, 0, 0,1,1, 0, 0,1,1,1,1, 0,0,0,1,1,0,0,0,1, 1,0,1,0, 1,0,0,0,0,0,0,0, 0,0,0,1,1,1, 0,0,0,0,0,1,1,1, 0,0,1,1,0,1,0,0, 0,1,0,0, 1,0,0 ,0,0};



int status = WL_IDLE_STATUS;
WiFiServer server(50);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  myservo.attach(6);
  myservo.write(pos);
  
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
            client.print("Click <a href=\"/ServoBeats1\">here</a> for song1 beats playing<br>");
            client.print("Click <a href=\"/ServoBeats2\">here</a> for song2 beats playing<br>");
            client.print("Click <a href=\"/ServoBeats3\">here</a> for song3 beats playing<br>");
            client.print("Click <a href=\"/Pause\">here</a> to pause<br>");
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
        if (currentLine.endsWith("GET /ServoBeats1"))
          flag=9;
        if (currentLine.endsWith("GET /ServoBeats2"))
          flag=10;
        if (currentLine.endsWith("GET /ServoBeats3"))
          flag=11;
        if (currentLine.endsWith("GET /Pause"))
          flag=12;
        if (currentLine.endsWith("GET /SpeedUp"))
          flag=13;
        if (currentLine.endsWith("GET /SpeedDown"))
          flag=14;        
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
  int count = 0;
  int record_flag;
  flag=get_flag(client);
  
    while(flag>0){
      record_flag = flag;
          if(flag<8){
            myservo.write(pos-arr[flag-1]);
            delay(time_arr[flag-1]);
            myservo.write(pos);
            delay(time_arr[flag-1]);             
          }
          else if(flag == 8){
            beats_play1(time_arr);            
          }
          else{
            switch(flag){
              case 9: servo_beats = servo_beats1;
                      tempo = tempo_set[0];
                      count = beats_count[0];
                      break;
              case 10:servo_beats = servo_beats2;
                      tempo = tempo_set[1];
                      count = beats_count[1];
                      break;
              case 11:servo_beats = servo_beats3;
                      tempo = tempo_set[2];
                      count = beats_count[2];
                      break;
              default: break;
            }
            
            for(int i=0;i<count;i++){
              if(servo_beats[i]==1){
                myservo.write(60);
                delay(tempo);
                myservo.write(pos);
                delay(tempo);
              }
              else{
                delay(2*tempo);
              }
              client = server.available();
              flag = get_flag(client);
              while(flag == 12){
                client = server.available();
                flag = get_flag(client);
              }
              if(flag == 13){
                tempo = 100;
                flag = record_flag;
              }
              else if(flag == 14){
                tempo = 300;
                flag = record_flag;
              }
              else if(flag!= record_flag){
                record_flag=flag;
                break;
              }
            }
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
