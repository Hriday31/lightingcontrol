                // --------------------------------   Initalzing Code ------------------------------ //
                
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>

AsyncWebServer server(80);

const char* ssid = "A.T.O.M_LABS";
const char* password = "Atom281121";

String AUTHoUSER = "ATOM"  ;
String AUTHoPASS = "12345" ;

int state = 0 ;

                // --------------------------------   WEB PAGES  ------------------------------ //

const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE HTML>
<html>

<head>
  <title>ADMIN LOGIN PAGE</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

 <style>
        body {
      background-image: url('https://cdna.artstation.com/p/assets/images/images/025/752/604/large/joshua-golde-waben03web.jpg?1586815618');
      background-repeat: no-repeat;
      background-color: black;
      background-size: 330%;
    }

    .text {
      text-align: center;
      margin-top: 50px;
      background-color: rgba(193, 176, 176, 0.7);
      border-radius: 40px;
      border: 2px solid rgb(0, 0, 0);
      padding: 20px;
      color: rgb(0, 0, 0);
      font-size: medium;
    }

    .text label {
      color: rgb(0, 0, 0);
      font-family: 'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif;
      font-size: medium;
    }

    .text input {
      border-color: black;
      color: rgb(0, 0, 0);
      font-family: 'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif;
      font-size: medium;
    }

    .login {
      text-align: center;
      color: rgb(196, 189, 189);
      width: fit-content;
      padding: 20px;
      font-family: 'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif;
      font-size: medium;
    }
  </style>
</head>

<body>
  <div class="login">
    <h1>ADVANCE POWER MANAGEMENT PORTAL</h1>
  </div>

  <div class="text">
    <form action="/login">
      <label for="username"><b>Username :&nbsp;</b></label>
      <input type="text" id="username" name="username" autocomplete="off" /><br /><br />
      <label for="password"><b>Password &nbsp;:&nbsp;</b></label>
      <input type="password" id="password" name="password" /><br /><br />
      <input type="submit" value="submit" style="font-weight: bold" />
    </form>
  </div>

</body>
</html>
)rawliteral";


const char main_html[] PROGMEM = R"rawliteral(

<!DOCTYPE HTML>
<html>

<head>
  <title>ADMIN LOGIN PAGE</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

 <link
      href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css"
      rel="stylesheet"
      integrity="sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi"
      crossorigin="anonymous"
    />

    <style>

      * {
        margin: 0;
        padding: 0;
      }

      .btn {
        margin-top: 10px;
      }

      .table {
        color: azure;
      }

      #main {
        margin-top: 10px;
        width: auto;
        background-color: rgba(79, 75, 75, 0.7);
        border-radius: 50px;
        margin-left: 30px;
        margin-right: 30px;
        border: 1px solid black;
        padding: 40px;
        max-height: fit-content;
        color: azure;
      }

      body {
        background-image: url(https://cdna.artstation.com/p/assets/images/images/025/752/604/large/joshua-golde-waben03web.jpg?1586815618);
        background-size: cover;
        overflow-x: hidden;
        height: 100vh;
        /* background-repeat: no-repeat; */
      }

      a{text-decoration : none ;
      color:rgb(0,0,0)}

    </style>

    <title>People Counter Details</title>
  </head>
  <body>
    <div
      class="row text-center"
      id=""
      style="
        margin-top: 50px;
        font-size: 40px;
        color: #cadeeb;
        font-weight: 900;
      "
    >
      <div class="col">AUTONOMOUS POWER MANAGEMENT SYSTEM</div>
    </div>
    <div class="row align-items-center text-center" id="main">
      <table class="table">
        <thead class="thead-dark">
          <tr>
            <th scope="col">Total Consumption</th>
            <th scope="col">Live Person Count</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><span id="TotalConsumption"> 0  units</td>
            <td><span id="LivePersonCounter">5</td>
          </tr>
        </tbody>
      </table>
       <div class="btn btn-danger"><a href="/updatePower">Power off</a></div>
       <div class="btn btn-primary"><a href="/Automatic">Automatic Mode</a></div>
       <div class="btn btn-warning"><a href="/Manual">Manual Mode </a></div>

    </div>
    <div
      class="row text-center"
      id=""
      style="
        margin-top: 20px;
        font-size: 20px;
        color: #cadeeb;
        font-weight: 900;
      "
    >
      <div class="col">SUB CONSUMPTION LEVELS</div>
    </div>
    <div class="row align-items-center text-center" id="main">
      <h3>AIR CONDITIONER</h3>
      <table class="table">
        <thead class="thead-dark">
          <tr>
            <th scope="col">Consumption</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>10 units</td>
          </tr>
        </tbody>
      </table>
      <div class="row">
        <div class="col"><button type = "button" class="btn btn-success"><a href="/Increase">Increase</a></button></div>
        <div class="col"><button type = "button" class="btn btn-danger"><a href="/Decrease">Decrease</a></button></div>
      </div>
    </div>
    <div class="row align-items-center text-center" id="main">
      <h3>FAN</h3>
      <table class="table">
        <thead class="thead-dark">
          <tr>
            <th scope="col">Consumption</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>10 units</td>
          </tr>
        </tbody>
      </table>
      <div class="row">
        <div class="col"><div class="btn btn-success"><a href="updateFan">Switch ON</a></div></div>
        <div class="col"><div class="btn btn-danger"><a href="updateFan">Switch OFF</a></div></div>
      </div>
    </div>
    <div class="row align-items-center text-center" id="main">
      <h3>LIGHTING</h3>
      <table class="table">
        <thead class="thead-dark">
          <tr>
            <th scope="col">Consumption</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>10 units</td>
          </tr>
        </tbody>
      </table>
      <div class="row">
        <div class="col"><div class="btn btn-success"><a href="updateLight">Switch ON</a></div></div>
        <div class="col"><div class="btn btn-danger"><a href="updateLight">Switch OFF</a></div></div>
      </div>
    </div>

    <script>

      setInterval(function() {
      getData();
        }, 500); 

    function getData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("LivePersonCounter").innerHTML =
            this.responseText;
         
        }
        };
        xhttp.open("GET", "update", true);
        xhttp.send();
        }
</script>

</body>
</html>
)rawliteral";

                        // --------------------------------   WEB PAGES  ------------------------------ //


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

int AutomaticFlag = 0 ;
int PowerFlag = 1 ;
int FanSwitch = 1 ;
int Light = 1 ;
int numberofIndividual = 0 ;
String LiveCount = "0" ;


void setup() {


  pinMode(14 , OUTPUT) ;
  pinMode(12 , OUTPUT) ;
  pinMode(13 , OUTPUT) ;
  pinMode(15 , OUTPUT) ;
  pinMode(A0 , INPUT) ;
  pinMode(16 , OUTPUT) ;
  pinMode(5  , OUTPUT) ;

  digitalWrite(14 , LOW) ;
  digitalWrite(12 , LOW) ;
  digitalWrite(13 , LOW) ;
  digitalWrite(15 , LOW) ;

  
          // --------------------------------   WIFI SETUP   ------------------------------ //
          
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
          
          // --------------------------------   ROUTES   ------------------------------ //

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/main_html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", main_html);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){

    request->send_P(200, "text/text", LiveCount.c_str());
    Serial.println(digitalRead(5));

if (AutomaticFlag){
     if( digitalRead(A0) > 1 ){  
      digitalWrite(16,LOW);}

if ((LiveCount) == "1" ) {digitalWrite(14 , HIGH);digitalWrite(12 , LOW);digitalWrite(13 , LOW);digitalWrite(15 , LOW);}
else if ((LiveCount) = "2" ) {digitalWrite(14 , HIGH);digitalWrite(12 , HIGH);digitalWrite(13 , LOW);digitalWrite(15 , LOW);}
else if ((LiveCount) = "3" ) {digitalWrite(14 , HIGH);digitalWrite(12 , HIGH);digitalWrite(13 , HIGH);digitalWrite(15 , LOW);}
else if ((LiveCount) = "4" ) {digitalWrite(14 , HIGH);digitalWrite(12 , HIGH);digitalWrite(13 , HIGH);digitalWrite(15 , HIGH);}
else {digitalWrite(14 , LOW);digitalWrite(12 , LOW);digitalWrite(13 , LOW);digitalWrite(15 , LOW);}
}

  });

  server.on("/update_Info", HTTP_GET, [](AsyncWebServerRequest *request){

    if (request->hasParam("LiveCount")) {
      LiveCount = request->getParam("LiveCount")->value();
      Serial.println(LiveCount);
      request->send_P(200, "text/text", "oK");
    }

  });

   server.on("/Automatic", HTTP_GET, [](AsyncWebServerRequest *request){

    AutomaticFlag = 1 ;
    request->send_P(200, "text/html", main_html);
  });

  server.on("/Manual", HTTP_GET, [](AsyncWebServerRequest *request){

    AutomaticFlag = 0 ;
    request->send_P(200, "text/html", main_html);
  });

   
   server.on("/updateFan", HTTP_GET, [](AsyncWebServerRequest *request){
   if (FanSwitch) FanSwitch = 0 ;
   else FanSwitch = 1 ;
    request->send_P(200, "text/html", main_html); 
  });

  server.on("/updateLight", HTTP_GET, [](AsyncWebServerRequest *request){
    if (Light) Light = 0 ;
   else Light = 1 ;
   request->send_P(200, "text/html", main_html); 
  });

  server.on("/updatePower", HTTP_GET, [](AsyncWebServerRequest *request){
       
    digitalWrite(14 , LOW) ;
    digitalWrite(12 , LOW) ;
    digitalWrite(13 , LOW) ;
    digitalWrite(15 , LOW) ;  
    PowerFlag = 0 ;
    digitalWrite(14 , LOW);digitalWrite(12 , LOW);digitalWrite(13 , LOW);digitalWrite(15 , LOW) ;
    request->send_P(200, "text/html", main_html);
    
  
  });

   server.on("/Increase", HTTP_GET, [](AsyncWebServerRequest *request){
    ++ state ;
    Serial.println(state) ;
    if ( state >= 1) digitalWrite(14,HIGH) ;
    if ( state >= 2) digitalWrite(12,HIGH) ;
    if ( state >= 3) digitalWrite(13,HIGH) ;
    if ( state >= 4) {digitalWrite(15,HIGH);state=4 ;}
    
    request->send_P(200, "text/html", main_html);
  });

  server.on("/Decrease", HTTP_GET, [](AsyncWebServerRequest *request){
    -- state ;
    Serial.println(state) ;

    if ( state < 1) {digitalWrite(14,LOW); state = 0;}
    if ( state < 2) digitalWrite(12,LOW) ;
    if ( state < 3) digitalWrite(13,LOW) ;
    if ( state < 4) digitalWrite(15,LOW) ;
    
    request->send_P(200, "text/html", main_html);
  });
  
  server.on("/login", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String Username;
    String Password ;


    if (request->hasParam("username")) {
      Username = request->getParam("username")->value();
      Serial.println(Username);
    }

    if (request->hasParam("password")) {
      Password = request->getParam("password")->value();
      Serial.println(Password);
    }

    if (AUTHoUSER == Username  && AUTHoPASS == Password ){
      request->send_P(200, "text/html", main_html);
    }

    
  });
  
  server.onNotFound(notFound);
  
  // --------------------------------   XXXXXXXXX  ------------------------------ //

  server.begin();
}

void loop() {
  
}
