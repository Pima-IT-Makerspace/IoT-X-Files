#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid     = "testlab-2";
const char* password = "welcomewelcome";

// Create an instance of the WebServer on port 80
WebServer server(80);

// Motor 1
int motor1Pin1 = 2;
int motor1Pin2 = 4;
int enable1Pin = 13; 

// Motor 2
int motor2Pin1 = 25;
int motor2Pin2 = 33;
int enable2Pin = 32;

// Motor 3
int motor3Pin1 = 18;
int motor3Pin2 = 5;
int enable3Pin = 19;

// Motor 4
int motor4Pin1 = 21;
int motor4Pin2 = 22;
int enable4Pin = 23;

// Setting PWM properties
const int freq = 30000;
const int resolution = 8;
int dutyCycle = 0;
 
String valueString = String(0);

void handleHTML() {
  const char html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <link rel="icon" href="data:,">
      <style>
        html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }
        .button { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; background-color: #4CAF50; border: none; color: white; padding: 12px 28px; text-decoration: none; font-size: 26px; margin: 1px; cursor: pointer; }
        .button2 {background-color: #555555;}
      </style>
    </head>
    
    <body>
      <h1>Speedy v1</h1>
      <p id="gamepad-info">Waiting for Gamepad.</p>
      <p id="button-info">Button Pressed:.</p>
      
      <p><button class="button" onclick="moveForward()">FORWARD</button></p>
      <div style="clear: both;">
        <p>
          <button class="button" onclick="moveLeft()">LEFT</button>
          <button class="button button2" onclick="stopRobot()">STOP</button>
          <button class="button" onclick="moveRight()">RIGHT</button>
        </p>
      </div>
      <p><button class="button" onclick="moveReverse()">REVERSE</button></p>
      <p>Motor Speed: <span id="motorSpeed">0</span></p>
      <input type="range" min="0" max="100" step="25" id="motorSlider" oninput="updateMotorSpeed(this.value)" value="0"/>
    </body>

    <script>
      var CurrentSpeed = 0;
      
      // Allows our functions to be called through our buttons
      function moveForward() { fetch('/forward'); }
      function moveLeft() { fetch('/left'); }
      function stopRobot() { fetch('/stop'); }
      function moveRight() { fetch('/right'); }
      function moveReverse() { fetch('/reverse'); }

      buttonInfo = document.getElementById("button-info");

      function updateMotorSpeed(pos) {
        document.getElementById('motorSpeed').innerHTML = pos;
        fetch(`/speed?value=${pos}`);
      }

      var gamepadInfo;
      
      // A function to handle button presses and send commands
      function handleGamepadInput() {
        
        var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
        if (!gamepads) return;

        var gp = gamepads[0];
        if (!gp) return; // Make sure a gamepad is actually connected

        

        if (gp.buttons[0].pressed) { // A button
          moveReverse();
          buttonInfo.innerHTML = "Reverse ";
        } else if (gp.buttons[1].pressed) { // B button
          moveRight();
          buttonInfo.innerHTML = "Right ";
        } else if (gp.buttons[2].pressed) { // X button
          moveForward();
          buttonInfo.innerHTML = "Forward ";
        } else if (gp.buttons[3].pressed) { // Y button
          moveLeft();
          buttonInfo.innerHTML = "Left";
        } 
        else if (gp.buttons[5].touched) { //RT
          if(CurrentSpeed <= 475)
          {
            CurrentSpeed += 25;
            fetch(`/speed?value=${CurrentSpeed}`);
            buttonInfo.innerHTML = "Speed at " + CurrentSpeed;
          }
        }
        else if (gp.buttons[4].touched){
          if(CurrentSpeed >= 25)
          {
            CurrentSpeed -= 25;
            fetch(`/speed?value=${CurrentSpeed}`);
            buttonInfo.innerHTML = "Speed at " + CurrentSpeed;
          }        
        }
      }
      
      // Make sure the DOM is loaded before accessing elements
      window.onload = function() {
        gamepadInfo = document.getElementById("gamepad-info");

        window.addEventListener("gamepadconnected", function(e) {
          var gp = e.gamepad;
          gamepadInfo.innerHTML = "Gamepad connected at index " + gp.index + ": " + gp.id + ". It has " + gp.buttons.length + " buttons and " + gp.axes.length + " axes.";
          handleGamepadInput();
        });
        
        window.addEventListener("gamepaddisconnected", function(e) {
          gamepadInfo.innerHTML = "disconnected.";
        });

        if (!('GamepadEvent' in window)) {
          var interval = setInterval(pollGamepads, 500);
        }

        function pollGamepads() {
          var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
          for (var i = 0; i < gamepads.length; i++) {
            
            var gp = gamepads[i];
            if (gp) {
              gamepadInfo.innerHTML = "Gamepad connected at index " + gp.index + ": " + gp.id + ". It has " + gp.buttons.length + " buttons and " + gp.axes.length + " axes.";
              handleGamepadInput();
              clearInterval(interval);
            }
          }
        }

        //Modifying this will change the amount of get requests used for checking if the button is pressed or not. 
        //If an on hold effect is wanted, we'd want this value to be very low and constantly sending requests, however this will fluff network traffic
        //Maybe use an if statement to limit this function from sending multiple requests
        var Controllerinterval = setInterval(handleGamepadInput, 100);
      };
    </script>
  </html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleForward() {
  Serial.println("Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
  server.send(200);
}

void handleLeft() {
  Serial.println("Left");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
  server.send(200);
}

void handleStop() {
  Serial.println("Stop");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, LOW);
  server.send(200);
}

void handleRight() {
  Serial.println("Right");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, LOW);
  server.send(200);
}

void handleReverse() {
  Serial.println("Reverse");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, HIGH);
  digitalWrite(motor4Pin2, LOW);
  server.send(200);
}

void handleSpeed() {
  if (server.hasArg("value")) {
    valueString = server.arg("value");
    int value = valueString.toInt();
    if (value == 0) {
      ledcWrite(enable1Pin, 0);
      ledcWrite(enable2Pin, 0);
      ledcWrite(enable3Pin, 0);
      ledcWrite(enable4Pin, 0);

      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);
      digitalWrite(motor3Pin1, LOW);
      digitalWrite(motor3Pin2, LOW);
      digitalWrite(motor4Pin1, LOW);
      digitalWrite(motor4Pin2, LOW);
    } else {
      dutyCycle = map(value, 25, 500, 150, 255);
      ledcWrite(enable1Pin, dutyCycle);
      ledcWrite(enable2Pin, dutyCycle);
      ledcWrite(enable3Pin, dutyCycle);
      ledcWrite(enable4Pin, dutyCycle);
      Serial.println("Motor speed set to " + String(value));
    }
  }
  server.send(200);
}

void setup() {
  Serial.begin(115200);

  // Set the Motor pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);

  // Configure PWM Pins
  ledcAttach(enable1Pin, freq, resolution);
  ledcAttach(enable2Pin, freq, resolution);
  ledcAttach(enable3Pin, freq, resolution);
  ledcAttach(enable4Pin, freq, resolution);

  // Initialize PWM with 0 duty cycle
  ledcWrite(enable1Pin, 0);
  ledcWrite(enable2Pin, 0);
  ledcWrite(enable3Pin, 0);
  ledcWrite(enable4Pin, 0);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/", handleHTML);
  server.on("/forward", handleForward);
  server.on("/left", handleLeft);
  server.on("/stop", handleStop);
  server.on("/right", handleRight);
  server.on("/reverse", handleReverse);
  server.on("/speed", handleSpeed);

  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
}