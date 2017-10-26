
// Alarm System by Konstantinos Chatzis



#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int sensNum = 3, btnLockSt = A5, redLed1 = 6, buzz1 = 7;
int val[3] = {0, 0}, i, j, k = 0, lockSt = 0;
const String sensName[3] = {"MAIN", "KITC", "MOTI"};
int timesAlarmed = 0;

const int sens[3] = {A1, A2, A4};
int sens1Val = 0;

byte c0_00[8] = {
  0b00111,
  0b01111,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100
};

byte c0_11[8] = {
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b11110,
  0b11100
};

byte c0_10[8] = {
  0b11100,
  0b11110,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111
};

byte c0_01[8] = {
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b01111,
  0b00111
};

byte k1[8] = {
  0b11100,
  0b11100,
  0b11100,
  0b11110,
  0b11110,
  0b11111,
  0b11101,
  0b11101
};

byte k2[8] = {
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00111
};


byte k3[8] = {
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100
};


byte k4[8] = {
  0b10111,
  0b10111,
  0b11111,
  0b01111,
  0b01111,
  0b00111,
  0b00111,
  0b00111
};

void setup() {
  Serial.begin(9600);
  Serial.println("SETUP;");
  
  for (i = 0; i < sensNum; i++) {
    pinMode(sens[i], INPUT);
  }
  pinMode(btnLockSt, INPUT);
  pinMode(redLed1, OUTPUT);
  pinMode(buzz1, OUTPUT);

  digitalWrite(redLed1, LOW);

  lcd.createChar(0, c0_00);
  lcd.createChar(1, c0_01);
  lcd.createChar(2, c0_10);
  lcd.createChar(3, c0_11);
  lcd.createChar(4, k1);
  lcd.createChar(5, k2);
  lcd.createChar(6, k3);
  lcd.createChar(7, k4);


  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HomeSafe");
  lcd.setCursor(0, 1);
  lcd.print("Loading...");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lockSt = 0;
}

void loop() {
  lcd.clear();
  readLockSt();
  checkSensors();
  disableLimit();
  writeLcd();
  writeTime();
  writeLeds();
  Serial.println(";E;");
  delay(500);
}

void disableLimit() {
  if(timesAlarmed > 5){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LIMIT REACHED!");
    while(1){
      delay(10000);
    }
    
  }
}

void readLockSt() {
    Serial.print("btnLockStARead:");
    Serial.print(analogRead(btnLockSt));
  if (analogRead(btnLockSt) > 1020) {
    if (lockSt > 0) {
      lockSt = 0;
      unlockScreen();
    } else {
      lockSt = 1;
      lockScreen();
    }
  }
}

void checkSensors() {
  k = 0;
  for (i = 0; i < sensNum - 1; i++) {
    j = analogRead(sens[i]);
    if (j > 1001) {
      val[i] = 0;
    } else {
      val[i] = 1;
      k++;
    }
  }

  //Check PIR Sensor
  j = analogRead(sens[i]);
  if (j < 500){
    val[i] = 0;
  } else {
    val[i] = 1;
    k++;
  }
//  
//  j = analogRead(sens[0]);
//  if (j > 1005){
//    val[0] = 0;
//  }else{
//    val[0] = 1;
//    k++;
//  }
//
//  j = analogRead(sens[1]);
//  if (j > 1005){
//    val[1] = 0;
//  }else{
//    val[1] = 1;
//    k++;
//  }
  Serial.print(";sens0:");
  Serial.print(analogRead(sens[0]));
  Serial.print(";sens1:");
  Serial.print(analogRead(sens[1]));
  Serial.print(";pir:");
  Serial.print(analogRead(sens[2]));
}

void writeLcd() {
  lcd.setCursor(0, 0);
  lcd.print("ARMED !");

  lcd.setCursor(0, 1);

  j = 0;
  for (i = 0; i < sensNum; i++) {
    if (val[i] > 0) {
      j++;
    }
  }

  if (lockSt > 0) {
    writeLocked();
  } else {
    writeUnlocked();
  }
}

void writeLocked() {
  lcd.setCursor(0, 0);
  lcd.print("ARMED !");

  lcd.setCursor(0, 1);

  if (j < 1) {

    //Write Big O
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.setCursor(1, 0);
    lcd.write(byte(2));
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    lcd.setCursor(1, 1);
    lcd.write(byte(3));
    //Write Big N
    lcd.setCursor(3, 0);
    lcd.write(byte(4));
    lcd.setCursor(4, 0);
    lcd.write(byte(5));
    lcd.setCursor(3, 1);
    lcd.write(byte(6));
    lcd.setCursor(4, 1);
    lcd.write(byte(7));

  } else if (j > 0 && j < 2) {
    j = 0;
    lcd.print("");
    for (i = 0; i < sensNum; i++) {
      if (val[i] > 0) {
        lcd.print(sensName[i]);
      }
    }
    lcd.print(""); //:OPEN
  } else {
    j = 0;
    lcd.print("");
    for (i = 0; i < sensNum; i++) {
      if (val[i] > 0) {
        if (j > 0) {
          lcd.print(",");
        }
        lcd.print(sensName[i]);
        j = j + 1;
      }
    }
    lcd.print(""); //:OPEN
  }
}

void writeUnlocked() {
  lcd.setCursor(0, 0);
  lcd.print("DISARMED!");

  lcd.setCursor(0, 1);
  Serial.print(";j:");
  Serial.print(j);

      Serial.print(";val0:");
      Serial.print(val[0]);
      Serial.print(";val1:");
      Serial.print(val[1]);
        
  if (j < 1) {
    lcd.print("OK,BUT DISARMED!");
  } else if (j == 1) {
    j = 0;
    lcd.print("");
    for (i = 0; i < sensNum; i++) {
      if (val[i] > 0) {
        lcd.print(sensName[i]);
      }
      //if (val[0] > 0)
      //  lcd.print("A0");
      //if (val[1] > 0)
      //  lcd.print("A1");

    }
    lcd.print(""); //:OPEN
  } else {
    j = 0;
    lcd.print("");
    for (i = 0; i < sensNum; i++) {
      if (val[i] > 0) {
        if (j > 0) {
          lcd.print(",");
        }
        lcd.print(sensName[i]);
        j = j + 1;
      }
    }
    lcd.print(""); //:OPEN
  }
}

void writeTime() {
  lcd.setCursor(11, 0);
  lcd.print("00:00");
  Serial.print(";time:");
  Serial.print("0000");
}

void lockScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM ARMED !!!");
  delay(3000);
  lcd.clear();
}

void unlockScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM DISARMED!");
  delay(3000);
  lcd.clear();
}

void writeLeds() {
  if (k > 0 && lockSt > 0) {
    //k = 0;
    timesAlarmed++;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ALARM ACTIVATED!");
    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print(timesAlarmed);
    for (i = 0; i < 5; i++) {
      
      digitalWrite(redLed1, HIGH);
      delay(600);
      tone(buzz1,1000);
      digitalWrite(redLed1, LOW);
      delay(350);
      noTone(buzz1); 
    }
    lcd.clear();
  }
  
}

