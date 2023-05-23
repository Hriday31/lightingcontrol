#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//const char *ssid = "A.T.O.M_LABS";
//const char *password = "Atom281121";
//const char *ssid = "Hriday";
//const char *password = "9953777876";
const char *ssid = "HRHK";
const char *password = "Ha9868598102@";

int ENA_pin = 14;
int IN1 = 27;
int IN2 = 26;
int p=0;
const int frequency = 500;
const int pwm_channel = 0;
const int resolution = 8;

const char *input_parameter = "value";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Document</title>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet"
    integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous" />
  <link rel="icon" type="image/x-icon"
    src="https://drive.google.com/file/d/1dX-5-Pdz4BMSn4itBAyrH7ERuY5MDCVt/view?usp=sharing">


  <style>
    * {
      font-size: 17px;
      font-family: "Lucida Sans", "Lucida Sans Regular", "Lucida Grande",
        "Lucida Sans Unicode", Geneva, Verdana, sans-serif;
      font-weight: 600;
      color: #1a1a1a;
    }

    html {
      padding: 20px;
      background-color: #095376;
    }

    body {
      background-color: #dceef8;
      border-radius: 20px;
      height: 95vh;
    }

    .a {
      /* on off*/
      margin: 7px;
    }

    .b {
      /* on off plus minus */
      background-color: #098bc9;
      border: none;
      border-radius: 50%;
      height: 3rem;
      width: 3rem;
      /* box-shadow: inset -10px -10px 15px rgba(228, 223, 223, 0.5),
          inset 10px 10px 15px rgba(70, 70, 70, 0.12); */
    }

    .c {
      /* plus minus 100 */
      background-color: #2eb0ee;
      height: 100px;
      padding: 10px;
    }

    .center {
      display: flex;
      align-items: center;
      justify-content: center;
    }

    .d {
      /* 100 only */
      /* background-color: #E7FDFF; */
      /* border-radius: 0.4rem; */
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 10px;
      margin-top: 10px;
      font-size: x-large;
    }

    .e {
      display: flex;
      justify-content: end;
      align-items: center;
    }

    .topp {
      background-color: #2eb0ee;
      height: 60px;
      width: 130px;
      padding: 2px;
      margin-top: 2rem;
      margin-right: 1rem;
    }

    .navbtn {
      background-color: transparent;
      font-size: x-large;
      border: none;
      border-radius: 50%;
      height: 3rem;
      width: 3rem;
      margin: 0.1rem;
    }

    .normal {
      font-size: x-large;
    }

    button.navbtn:hover,
    button.navbtn:active {
      background-color: #dceef8;
      box-shadow: inset -10px -10px 15px rgba(228, 223, 223, 0.5),
        inset 10px 10px 15px rgba(70, 70, 70, 0.12);
    }

    button.a:hover {
      background-color: #dceef8;
    }
  </style>
</head>

<body>
  <div class="row align-content-center" style="height: 10vh">
    <div class="col-6"></div>
    <div class="col-6 d-inline-flex justify-content-end">
      <div class="rounded-pill topp d-inline-flex justify-content-around">
        <button class="navbtn normal">⚙️</button>
        <button class="navbtn">🔥</button>
      </div>
    </div>
  </div>
  <div class="row align-content-center " style="height: 17vh">
    <div class="row">
      <div class="col-12 col-xl-12 text-center ">
        <h2 style="font-weight: 700;margin-bottom: 0px;font-size: 30px;">Water Management Portal</h2>
      </div>
    </div>
    <div class="row">
      <div class="col-8 col-xl-12 text-center">
        <h4 style="font-weight: 1000;">Valve Control Unit</h4>
      </div>
    </div>
  </div>
  <div class="row align-content-center" style="height: 10vh">
    <div class="col-3 col-sm-4 e">
      <button type="button" class="button b center a" onclick="updateSliderPWMon()">ON</button>
    </div>
    <div class="col-6 col-sm-4 d-flex justify-content-around rounded-pill c">
      <div class="col-4 center">
        <button type="button" class="b" onclick="updateSliderPWMlow()" style="background-color:#dceef8 ;">➖</button>
      </div>
      <div class="col-4">
        <div class="d" id="textslider_value">100%</div>
      </div>
      <div class="col-4 center">
        <button type="button" class="b" onclick="updateSliderPWM()" style="background-color:#dceef8">➕</button>
      </div>
    </div>
    <div class="col-3 col-sm-4 e d-flex justify-content-start">
      <button type="button" class="button b center a" onclick="updateSliderPWMoff()">OFF</button>
    </div>
  </div>
  <div class="row align-content-end" style="height: 60vh">
    <div class="col-12 d-flex justify-content-center"><img
        src="https://media.istockphoto.com/id/872714390/vector/water-pouring-from-pipe-flow-of-clean-water-vector-illustration.jpg?s=170667a&w=0&k=20&c=g_NOjGQfV40Hp1d5FUXDUJsWhKfqj2I52xW-TJD633o="
        style="height: 45vh; border-radius: 30px 30px 0px 0px;" class="align-bottom"></div>
  </div>
  <script>
    src = "https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"
    integrity = "sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4"
    crossorigin = "anonymous"
    function updateSliderPWM() {
      var slider_value = document.getElementById("textslider_value").innerText;
      if (slider_value != 100) {
        slider_value = Number(slider_value) + 20;
      }
      document.getElementById("textslider_value").innerHTML = slider_value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider", true);
      xhr.send();
    }
    function updateSliderPWMlow() {
      var slider_value = document.getElementById("textslider_value").innerText;
      if (slider_value != 0) {
        slider_value = Number(slider_value) - 20;
      }
      document.getElementById("textslider_value").innerHTML = slider_value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/sliderlow", true);
      xhr.send();
    }
    function updateSliderPWMon() {
      d = new Date();
      t = d.getTime();
      console.log(t);
      v = document.getElementById('textslider_value')
      v.innerHTML = 100
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slideron", true);
      xhr.send();
    }
    function updateSliderPWMoff() {
      v = document.getElementById('textslider_value')
      v.innerHTML = 0
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slideroff", true);
      xhr.send();
    }
    function updateSliderPWMoff2() {
      updateSliderPWMoff();
      d2 = new Date();
      t2 = d2.getTime();
      console.log(t2);
      t3 = (t2 - t) / 1000;
      console.log(t3 * 25);
      document.getElementById("time").innerHTML = t3 * 25;

    }
  </script>
</body>

</html>
)rawliteral";

void setup()
{
  Serial.begin(115200);
  delay(1000);
  pinMode(ENA_pin, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  ledcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(ENA_pin, pwm_channel);
  ledcWrite(pwm_channel, p);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (p<255){
                p=p+51;
                ledcWrite(pwm_channel, p);
              }
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.on("/sliderlow", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (p>50){
                p=p-51;
                ledcWrite(pwm_channel, p);
              }
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.on("/slideron", HTTP_GET, [](AsyncWebServerRequest *request)
            {p=255;
              ledcWrite(pwm_channel, p);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.on("/slideroff", HTTP_GET, [](AsyncWebServerRequest *request)
            {p=0;
              ledcWrite(pwm_channel, p);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.begin();
}

void loop()
{
}
