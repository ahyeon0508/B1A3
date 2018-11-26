#include "SPI.h"
#include "WiFi.h"

char ssid[] = "쓰지마세욤";       //와이파이 SSID
char pass[] = "heeyeon0512";   //와이파이 password 

//인스턴스 변수 초기화
WiFiServer server(80);
WiFiClient client;

IPAddress hostIp;
uint8_t ret;

int temp = 0;

String weather_str="";
String wt_temp="";
String wt_wfKor="";
String wt_wfEn="";
String wt_reh="";
String wt_pop="";

void setup() {
  //각 변수에 정해진 공간 할당
  Serial.begin(115200);    
  
  delay(10);
  //WiFi연결 시도
  Serial.println("Connecting to WiFi....");  
  WiFi.begin(ssid, pass);  //WiFi가 패스워드를 사용한다면 매개변수에 password도 작성
  
  server.begin();
  Serial.println("Connect success!");
  Serial.println("Waiting for DHCP address");
  //DHCP주소를 기다린다
  while(WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\n");
  printWifiData();
  connectToServer();
}

void loop() {
  if (client.connected()) {
    while (client.available()) {
      //라인을 기준으로 문자열을 저장한다.
      String line = client.readStringUntil('\n');
      Serial.println(line);
      
      //시간
      int temp11= line.indexOf("</hour>");
      if(temp11>0) {
        String tmp_str="<hour>";
        String wt_hour = line.substring(line.indexOf(tmp_str)+tmp_str.length(),temp11);
        Serial.print("hour is "); 
        Serial.println(wt_hour);  
      }
      
      //온도
      int temp= line.indexOf("</temp>");
      if(temp>0) {
        String tmp_str="<temp>";
        String wt_temp = line.substring(line.indexOf(tmp_str)+tmp_str.length(),temp); // wt_temp :  온도값
        Serial.print("temperature is "); 
        Serial.println(wt_temp);  
      }
      
      //날씨 정보
      int wfEn= line.indexOf("</wfEn>");
      if(wfEn>0) {
        String tmp_str="<wfEn>";
        String wt_twfEn = line.substring(line.indexOf(tmp_str)+tmp_str.length(),wfEn); // wt_twfEn : 날씨정보
        Serial.print("weather is ");
        Serial.println(wt_twfEn);  
      }
      
      //습도
      int reh= line.indexOf("</reh>");
      if(reh>0) {
        String tmp_str="<reh>";
        String wt_reh = line.substring(line.indexOf(tmp_str)+tmp_str.length(),reh);
        Serial.print("Humidity is ");
        Serial.println(wt_reh);  

       //강수확률
       int pop= line.indexOf("</pop>");
       if(pop>0){
        String tmp_str = "<pop>";
        String wt_pop = line.substring(line.indexOf(tmp_str)+tmp_str.length(), pop); // wt_pop : 강수확률
        Serial.print("a rainfall probability is ");
        Serial.println(wt_pop);

       //하늘 상태
       int sky= line.indexOf("</sky>");
       if(sky>0) {
        String tmp_str="<sky>";
        String wt_sky = line.substring(line.indexOf(tmp_str)+tmp_str.length(), sky); // wt_sky : 하늘상태
        Serial.print("sky condition is ");
        Serial.println(wt_sky);
       }
       }
      }
    }   
  }
}

//서버와 연결
void connectToServer() {
  Serial.println("connecting to server...");
  String content = "";
  if (client.connect(hostIp, 80)) {
    Serial.println("Connected! Making HTTP request to www.kma.go.kr");
    //Serial.println("GET /data/2.5/weather?q="+location+"&mode=xml");
    client.println("GET /wid/queryDFSRSS.jsp?zone=1162058500 HTTP/1.1"); 

    /*{"code":"11","value":"서울특별시"},{"code":"26","value":"부산광역시"},
    {"code":"27","value":"대구광역시"},{"code":"28","value":"인천광역시"},
    {"code":"29","value":"광주광역시"},{"code":"30","value":"대전광역시"},
    {"code":"31","value":"울산광역시"},{"code":"41","value":"경기도"},
    {"code":"42","value":"강원도"},{"code":"43","value":"충청북도"},
    {"code":"44","value":"충청남도"},{"code":"45","value":"전라북도"},
    {"code":"46","value":"전라남도"},{"code":"47","value":"경상북도"},
    {"code":"48","value":"경상남도"},{"code":"50","value":"제주특별자치도"}*/

    // 서울특별시 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=11
    // 부산광역시 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=26
    // 대구광역시 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=27
    // 인천광역시 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=28
    // 광주광역시 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=29
    // 대전광역시 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=30
    // 울산광역시 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=31
    // 경기도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=41
    // 강원도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=42
    // 충청북도  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=43
    // 충청남도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=44
    // 전라북도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=45
    // 전라남도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=46
    // 경상북도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=47
    // 경상남도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=48
    // 제주특별자치도 :  http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=50

    //위에 지정된 주소와 연결한다.
    client.print("HOST: www.kma.go.kr\n");
    client.println("User-Agent: launchpad-wifi");
    client.println("Connection: close");
    
    client.println();
    Serial.println("Weather information for ");
  }
  //마지막으로 연결에 성공한 시간을 기록
}


void printHex(int num, int precision) {
  char tmp[16];
  char format[128];
  
  sprintf(format, "%%.%dX", precision);
  
  sprintf(tmp, format, num);
  Serial.print(tmp);
}

void printWifiData() {
  // Wifi쉴드의 IP주소를 출력
  Serial.println();
  Serial.println("IP Address Information:");  
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  //MAC address출력
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printHex(mac[5], 2);
  Serial.print(":");
  printHex(mac[4], 2);
  Serial.print(":");
  printHex(mac[3], 2);
  Serial.print(":");
  printHex(mac[2], 2);
  Serial.print(":");
  printHex(mac[1], 2);
  Serial.print(":");
  printHex(mac[0], 2);
  Serial.println();
  //서브넷 마스크 출력
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);
  
  //게이트웨이 주소 출력
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  ret = WiFi.hostByName("www.kma.go.kr", hostIp);
  
  Serial.print("ret: ");
  Serial.println(ret);
  
  Serial.print("Host IP: ");
  Serial.println(hostIp);
  Serial.println("");
}

int getInt(String input){
  int i = 2;
  
  while(input[i] != '"'){
    i++;
  }
  input = input.substring(2,i);
  char carray[20];
  //Serial.println(input);
  input.toCharArray(carray, sizeof(carray));
  //Serial.println(carray);
  temp = atoi(carray);
  //Serial.println(temp);
  return temp;
}
