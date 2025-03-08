  #define DelayTurn 500
  #define Delay 200
  
  int LENA = 3;
  int LIN1 = 7;
  int LIN2 = 8;
  int RENB = 5;
  int RIN3 = 6;
  int RIN4 = 4;
  int LBaseSpeed = 70;
  int RBaseSpeed = 70;


  int Sensor1 = A0;
  int Sensor2 = A1;
  int Sensor3 = A2;
  int Sensor4 = A3;
  int Sensor5 = A4;
  int Sensor6 = A5;
  int Sensor7 = 9;
  int Sensor8 = 10;

  
  int SensorValues[8] = {0,0,0,0,0,0,0,0};

  int error = 0;
  float Kp = 6;
  float Ki = 0;
  float Kd = 1;
  int x = 30;
  int y = 10;

  float  P = 0, I = 0, D = 0, PID_value = 0;
  float previous_error = 0, previous_I = 0;

  int flag = 0;
  
  
void calculate_pid(){
 
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;

  read_sensor_values();
  
  }

void motor_control(){

  // Calculating the effective motor speed:
  int left_motor_speed = LBaseSpeed + PID_value;
  int right_motor_speed = RBaseSpeed - PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 120);
  right_motor_speed = constrain(right_motor_speed, 0, 120);

  analogWrite(LENA, left_motor_speed); //Left Motor Speed
  analogWrite(RENB, right_motor_speed); //Right Motor Speed

  //following lines of code are to make the bot move forward
  forward();
  
  }



void rightturn(){

  digitalWrite(LIN1, LOW);
  digitalWrite(LIN2, HIGH);
 

  digitalWrite(RIN4, LOW);
  digitalWrite(RIN3, HIGH);
  
  }


void leftturn(){

  digitalWrite(LIN1, HIGH);
  digitalWrite(LIN2, LOW);
  

  digitalWrite(RIN4, HIGH);
  digitalWrite(RIN3, LOW);
  
  }


void stopbot(){

  digitalWrite(LIN1, LOW);
  digitalWrite(LIN2, LOW);
  

  digitalWrite(RIN4, LOW);
  digitalWrite(RIN3, LOW);
  
  }


void forward(){

  digitalWrite(LIN1, LOW);
  digitalWrite(LIN2, HIGH);
  

  digitalWrite(RIN4, HIGH);
  digitalWrite(RIN3, LOW);
  
  }


void display() {
  
  Serial.print(SensorValues[0]);
  Serial.print("\t");
  Serial.print(SensorValues[1]);
  Serial.print("\t");
  Serial.print(SensorValues[2]);
  Serial.print("\t");
  Serial.print(SensorValues[3]);
  Serial.print("\t");
  Serial.print(SensorValues[4]);
  Serial.print("\t");
  Serial.print(SensorValues[5]);
  Serial.print("\t");
  Serial.print(SensorValues[6]);
  Serial.print("\t");
  Serial.print(SensorValues[7]);
  Serial.println(); // Add a newline
  read_sensor_values();
  delay(500);
  
  }


void read_sensor_values() {
  
  SensorValues[0] = digitalRead(Sensor1);    // 0    1    2    3    4    5    6    7    
  SensorValues[1] = digitalRead(Sensor2);    // White 1 Black 0
  SensorValues[2] = digitalRead(Sensor3);
  SensorValues[3] = digitalRead(Sensor4);
  SensorValues[4] = digitalRead(Sensor5);
  SensorValues[5] = digitalRead(Sensor6);
  SensorValues[6] = digitalRead(Sensor7);
  SensorValues[7] = digitalRead(Sensor8);
  if (SensorValues[0] == 1 && SensorValues[1] == 1 && SensorValues[3] == 1 && SensorValues[4] == 1 && SensorValues[6] == 1 && SensorValues[7] == 1)
    error = 103; // Dead-end or stop
  else if (SensorValues[0] == 1 && SensorValues[7] == 0) 
    error = 100; // Sharp left
  else if (SensorValues[7] == 1 && SensorValues[1] == 0) 
    error = 101; // Sharp right
  else if (SensorValues[0] == 0 && SensorValues[1] == 0 && SensorValues[3] == 0 && SensorValues[4] == 0 && SensorValues[6] == 0 && SensorValues[7] == 0)
    error = 102; // U-turn
  else if (SensorValues[6] == 1)
    error = -5; 
  else if (SensorValues[1] == 1)
    error = 5;
  else if (SensorValues[5] == 1)
    error = -3;
  else if (SensorValues[2] == 1)
    error = 3;
  else if (SensorValues[4] == 1 && SensorValues[3] == 0)
    error = 1;
  else if (SensorValues[3] == 1 && SensorValues[4] == 0)
    error = -1;
  else
    error = 0;
    
    }


void setup(){
  
pinMode(LENA, OUTPUT);
pinMode(LIN1, OUTPUT);
pinMode(LIN2, OUTPUT);
pinMode(RENB, OUTPUT);
pinMode(RIN3, OUTPUT);
pinMode(RIN4, OUTPUT);

pinMode(Sensor1, INPUT);
pinMode(Sensor2, INPUT);
pinMode(Sensor3, INPUT);
pinMode(Sensor4, INPUT);
pinMode(Sensor5, INPUT);
pinMode(Sensor6, INPUT);
pinMode(Sensor7, INPUT);
pinMode(Sensor8, INPUT);



Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);

}

void loop(){

  read_sensor_values();
  Serial.print(error);
  Serial.print('\t');
    
  if (error >= 100){
     
      if (error == 100){               // Make left turn until it detects straight path
      stopbot();
      delay(Delay);
      analogWrite(LENA, LBaseSpeed); //Left Motor Speed
      analogWrite(RENB, RBaseSpeed); //Right Motor Speed
      forward();
      delay(Delay);
      stopbot();
      delay(Delay);
      
      do {
        analogWrite(LENA, LBaseSpeed - x); //Left Motor Speed
        analogWrite(RENB, RBaseSpeed - x + y); //Right Motor Speed
        leftturn();
        read_sensor_values();
        if (error == 0){
          stopbot();
        }
      } while (error != 0);
    } 
     
      else if (error == 101) {      // Make right turn in case it detects right path
      stopbot();
      delay(Delay);
      analogWrite(LENA, LBaseSpeed); //Left Motor Speed
      analogWrite(RENB, RBaseSpeed); //Right Motor Speed
      forward();
      delay(DelayTurn);
      stopbot();
      delay(Delay);
      do {
        analogWrite(LENA, LBaseSpeed - x); //Left Motor Speed
        analogWrite(RENB, RBaseSpeed - x + y); //Right Motor Speed
        rightturn();
        read_sensor_values();
        if (error == 0) {
          stopbot();
        }
      } while (error != 0);
    } 
    
      else if (error == 102) {      // Make left turn until it detects straight path
      analogWrite(LENA, LBaseSpeed); //Left Motor Speed
      analogWrite(RENB, RBaseSpeed); //Right Motor Speed
      forward();
      delay(DelayTurn);
      stopbot();
      read_sensor_values();
      if (error == 102) {
        do {
          analogWrite(LENA, LBaseSpeed - x); //Left Motor Speed
          analogWrite(RENB, RBaseSpeed - x + y); //Right Motor Speed
          rightturn();
          read_sensor_values();
          if (error == 0) {
            stopbot();
          }
        } while (error != 0);
      }
    } 
    
    
    else if (error == 103) {        // Go Forward if line found or stop if dead end reached.
      
      if (flag == 0) {
        analogWrite(LENA, LBaseSpeed); //Left Motor Speed
        analogWrite(RENB, RBaseSpeed); //Right Motor Speed
        forward();
        delay(Delay);
        stopbot();
        read_sensor_values();
        if (error == 103) {     // Dead End Reached
          stopbot();
          flag = 1;
        } 
      }
    } 
  }
  
  else {
   
    calculate_pid();
    motor_control();
  
  }

  display();

}