#include <WiFi.h>
#include <WebServer.h>
#include <ThingSpeak.h>

//Thingsspeak details
unsigned long  myChannelNumber = 996659;
const char *myWriteAPIKey = "7GLQ0X86LXMFCCKL";

#define CCMP(WPA) 4
#define TKIP(WPA) 2
#define PSK(WPA) 3

int i, sel_wifi = 0;
char ch, ssid[32], password[32];
byte encryption;
bool wifi_connected = true, first = false;

char * ssid_ap = "ESP32 Server SR";
char * password_ap = "password";

float data_arr[30];
static int arr_index,send_index;
bool busy_flag;
WiFiClient client;
WebServer server(80);

hw_timer_t * timer = NULL;
bool timer_flag;
void timer_isr()
{
  timer_flag = true; 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect(true);
  WiFi.softAPdisconnect(true);

  Serial.println("|--- ESP32 Gateway Configuration:");
  Serial.println();
  Serial.println("|--- Station Configuration:");

  //Prompt to user
  Serial.print("|--- Starting the network Scan ");

  //Scanning No of WiFi Available
  int no_of_wifi = WiFi.scanNetworks();
  while (!no_of_wifi)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" Done");

  //Ptompt number of wifi available
  Serial.println("|--- Found " + String(no_of_wifi) + " Networks");
  Serial.println();

  //Printing all wifi ssid
  for (int i = 0; i < no_of_wifi; i++)
  {
    Serial.println(String(i + 1) + ". " + WiFi.SSID(i));
  }
  Serial.println();

  //Prompt user to select the required wifi
  Serial.print("Please select the required Network: ");

  //Waiting for user input
  while (!Serial.available());

  //Converting char to int value
  while ((ch = Serial.read()) != '\n')
  {
    Serial.print(ch);
    sel_wifi = sel_wifi * 10 + (ch - '0');
  }
  Serial.println();

  //Coverting String to char array
  for (i = 0; (i < WiFi.SSID(sel_wifi - 1).length()) && (i < 32); i++)
  {
    ssid[i] = WiFi.SSID(sel_wifi - 1)[i];
  }
  ssid[i] = '\0';

  //Cheching wifi is Open or note
  encryption = WiFi.encryptionType(sel_wifi - 1);

  //Serial.println(encryption);
  //If WPA wifi it will ask for password
  if (encryption == CCMP(WPA) || encryption == TKIP(WPA) || encryption == PSK(WPA))
  {
    Serial.print("Please enter authentication key:");
    while (!Serial.available());
    i = 0;
    while ((ch = Serial.read()) != '\n')
    {
      //Serial.print("*");
      Serial.print(ch);
      password[i++] = ch;
    }
    password[i] = '\0';
    WiFi.begin(ssid, password);
  }
  else
  {
    WiFi.begin(ssid);
  }

  Serial.println();

  Serial.println("|--- Attempting to connect " + String(ssid));
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);

  }
  Serial.println();

  // Print Station Information
  Serial.println("|--- Connected to " + String(ssid));
  Serial.println();
  Serial.println("|--- Station Information");
  Serial.println("Station SSID\t\t :" + String(ssid));
  Serial.print("Station IP Address\t :");
  Serial.println(WiFi.localIP());
  Serial.println("Signal Strength\t\t :" + String(WiFi.RSSI(sel_wifi - 1)) + " dBm");
  Serial.println("|---Done");
  Serial.println();

  //Access Point Configuration
  Serial.println("|--- Access Point Configuration:");
  char * ssid_ap = "ESP32 Server SR";
  char * password_ap = "password";

  

  WiFi.softAP(ssid_ap, password_ap);
  Serial.println();
  Serial.println("|--- Access Point Information");
  Serial.println("AP SSID\t\t: " + String(ssid_ap));
  Serial.print("AP IP Address\t: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Server State\t: started");
  Serial.println("|---Done");
  Serial.println();
  Serial.println("|--- Waiting for connections ...");

  first = true;
  ThingSpeak.begin(client);
  
  server.on("/Node001", handle_Node001);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &timer_isr,true);
  timerAlarmWrite(timer,20000000,true);
  timerAlarmEnable(timer);
}

void loop() {
  // put your main code here, to run repeatedly:
   server.handleClient();
  if (wifi_connected)//When wifi is
  {
    if(abs(send_index - arr_index)>0 && timer_flag)
    {
      timer_flag = false;
      ThingSpeak.setField(1,data_arr[send_index++]);
      ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
      if(send_index >= 30)
      {
        send_index = 0;
      }
    }else if(timer_flag)
    {
      timer_flag = false;
    }

  }
  else
  {
    Serial.println("Not Connect");
  }
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_Node001()
{
  busy_flag = true;
  String device = server.arg("name");
  String data_val = server.arg("data");
  Serial.println(data_val);
  if(device == "Node001")
  {
    data_arr[arr_index++] = data_val.toFloat();
    if (arr_index >= 30)
    {
      arr_index = 0;
    }
    Serial.println(device);
    server.send(200,"text/plain","OK"); 
  }
  busy_flag = false;
}
