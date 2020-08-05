#include <WiFi.h>
#include<HTTPClient.h>


#define CCMP(WPA) 4
#define TKIP(WPA) 2
#define PSK(WPA) 3


int i, sel_wifi = 0;
char ch, ssid[32], password[32];
byte encryption;
bool wifi_connected, busy_flag;

int ana_old, ana_new;
bool busy;
int arr_buff[30];
static int send_index, buf_index;

WiFiClient client;
HTTPClient http;
IPAddress gateway;


void WiFiEvent(WiFiEvent_t event)
{
  //  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event)
  {
    case SYSTEM_EVENT_WIFI_READY:
      Serial.println("WiFi interface ready");
      break;
    case SYSTEM_EVENT_SCAN_DONE:
      Serial.println("Completed scan for access points");
      break;
    case SYSTEM_EVENT_STA_START:
      Serial.println("WiFi client started");
      break;
    case SYSTEM_EVENT_STA_STOP:
      Serial.println("WiFi clients stopped");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println();
      Serial.println("|--- Connected to " + String(ssid));
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      wifi_connected = false;
      Serial.println(String(WiFi.getHostname()) + "is Disconnected, Trying to reconnect");
      WiFi.reconnect();

      break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
      Serial.println("Authentication mode of access point has changed");
      break;

    case SYSTEM_EVENT_STA_GOT_IP:
      wifi_connected = true;
      Serial.println();
      Serial.println("|--- Connection Information");
      Serial.println("Server\t\t :" + String(ssid));
      Serial.print("Server IP Address :");
      gateway = WiFi.gatewayIP();
      Serial.println(gateway);
      Serial.println("Signal Strength\t : " + String(WiFi.RSSI(sel_wifi - 1)) + " dbm");
      Serial.print("Client\t\t : ");
      Serial.println(WiFi.getHostname());
      Serial.print("Client IP Address : ");
      Serial.println(WiFi.localIP());
      Serial.print("State\t\t : ");
      Serial.println((WiFi.status() == WL_CONNECTED) ? "Connected" : "Not Connected");
      Serial.println("|---Done");
      //     client.connect(gateway, 80);
      break;


    case SYSTEM_EVENT_STA_LOST_IP:
      Serial.println("Lost IP address and IP address is reset to 0");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("WiFi access point started");
      break;
    case SYSTEM_EVENT_AP_STOP:
      Serial.println("WiFi access point  stopped");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Client connected");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("Client disconnected");
      break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      Serial.println("Assigned IP address to client");
      break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
      Serial.println("Received probe request");
      break;
    case SYSTEM_EVENT_GOT_IP6:
      Serial.println("IPv6 is preferred");
      break;
    case SYSTEM_EVENT_ETH_START:
      Serial.println("Ethernet started");
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("Ethernet stopped");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("Ethernet connected");
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("Ethernet disconnected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.println("Obtained IP address");
      break;
    default: break;


  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);

  //Prompt to user
  Serial.print("|---Starting the network Scan ...");

  //Scanning No of WiFi Available
  int no_of_wifi = WiFi.scanNetworks();
  Serial.println("Done");

  //Ptompt number of wifi available
  Serial.println("|--- Found " + String(no_of_wifi) + " Networks");

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

  while ((ch = Serial.read()) != '\n')
  {
    Serial.print(ch);
    sel_wifi = sel_wifi * 10 + (ch - '0');
  }

  //Coverting String to char array
  for (i = 0; i < WiFi.SSID(sel_wifi - 1).length(); i++)
  {
    ssid[i] = WiFi.SSID(sel_wifi - 1)[i];
  }
  ssid[i] = '\0';
  //Cheching wifi is Open or note
  encryption = WiFi.encryptionType(sel_wifi - 1);
  Serial.println();


  WiFi.onEvent(WiFiEvent);

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
  WiFi.setHostname("Node001");

  Serial.println("|--- Attempting to connect " + String(ssid));
  while (!wifi_connected)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:
  ana_new = analogRead(33);
  if(abs(ana_new - ana_old)>10)
  {
    ana_old = ana_new;
    arr_buff[buf_index++] = ana_old;
    Serial.println(String(send_index) +"\n"+ String(ana_old) + "\n"+ String(buf_index) );
     if (buf_index >= 30)
    {
      buf_index = 0;
    }
  }
  

  if (wifi_connected)//When wifi is
  {
    
    if (abs(buf_index - send_index) > 0)
    {
      
      http.begin("http://" + gateway.toString() + "/" + String(WiFi.getHostname()) + "?name=" + String(WiFi.getHostname()) + "&data=" + arr_buff[send_index]);
      int responce = http.GET();
      if (responce == 200)
        send_index++;
        
        if (send_index >= 30)
        {
          send_index = 0;
        }
        
      }
    }
  else
  {
    Serial.println("Not Connect");
  }
  
  
}
