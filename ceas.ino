//    Libraries

#include <util/delay.h>
#include <LiquidCrystal.h>
#include <DS3231.h>
#include <Wire.h>
#include <dht.h>

//  MP3 player
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

int volume = 14;
SoftwareSerial mySerial(13, 12); // RX, TX

//  Flame sensor
int flamePin = A2;

//  Temp sensor
dht DHT;
#define DHT_PIN 4

//  Joystick
const int SW_pin = 2;
const int X_pin = A0;
const int Y_pin = A1;
int xValue = 0;
int yValue = 0;
int Button = 0;

//  RTC
DS3231 Clock(SDA, SCL);

//  Variables
int hour, min, sec;
int date, mon, year;
int dow;
int change = 0;

//  Change before finish
int mode = 0;                 //  Select mode 0 -> Clock
                              //              1 -> Setup
                              //              2 -> Music
bool active = true;
/*********************MODE 0************************/
bool format = false;
bool meridian = false;

/*********************CASE 1************************/
int select = 0;
bool en_select = false;
bool en_alarm = false;
int alarm_hour = 0;
int alarm_min = 0;
int alarm_sec = 0;
int counter = 0;

//  Const
//  Parameters for reading the joystick:
const int range = 12;             //  output range of X or Y movement
const int responseDelay = 5;      //  response delay of the mouse, in ms
const int threshold = range / 4;  //  resting threshold
const int center = range / 2;     //  resting position value

//  Buzzer pin
const int buzzer = 3;

//  LCD pin
const int rs = 11;
const int en = 10;
const int d4 = 9;
const int d5 = 8;
const int d6 = 7;
const int d7 = 6;

//  Custom Characters
byte arrowup[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};
byte arrowdown[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};
byte degree[8] = {
  0b00010,
  0b00101,
  0b00010,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
byte flame[8] = {
  0b00110,
  0b01100,
  0b01101,
  0b00111,
  0b10011,
  0b11111,
  0b11110,
  0b11100
};

//  date
volatile int month_day [] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

//  Initialize LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//  Mario song
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
 
#define melodyPin 3
//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};


/*********************SETUP************************/
void setup() {
  //  Setup Serial Connection
  Serial.begin (9600); 

  //  MP3 
  mySerial.begin (9600);
  mp3_set_serial (mySerial);
  mp3_set_volume(volume);
  
  //  Initialize RTC object
  Clock.begin();

  //  Flame Sensor
  pinMode(flamePin, INPUT);
  
  //  Initialize Buzzer
  pinMode(buzzer, OUTPUT);
  
  //  Configuration Joystick
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);

  pinMode(DHT_PIN, INPUT);
  
  //  Configuration LCD
  lcd.begin(16,2);
  lcd.noCursor();
  lcd.createChar(0, arrowup);
  lcd.createChar(1, arrowdown);
  lcd.createChar(2, degree);
  lcd.createChar(3, flame);

  //  Refresh alarm
  en_alarm = Clock.getTime().en_alarm;
  alarm_hour = Clock.getTime().alarm_hour;
  alarm_min = Clock.getTime().alarm_min;
  alarm_sec = Clock.getTime().alarm_sec;
  
  //  Refresh meridian
  if(Clock.getTime().hour > 12) meridian = true;
  else meridian = false;

  //
  mp3_stop();
}


/*********************LOOP************************/
void loop()
{
  yValue = readAxis(Y_pin);

  /*********************INCREMENT MODE (<-) ************************/
  if(yValue < -4 && active == true) {
    delay(500);
    mode++;
    lcd.clear();
    if(mode == 3) mode = 0;
  }
  //  Refresh
  hour = Clock.getTime().hour;
  min = Clock.getTime().min;
  sec = Clock.getTime().sec;
  //  Alarma
  if(en_alarm == true){
    test_alarm(hour,min,sec);
  }
  //  Flame
  
  if(analogRead(flamePin) <= 170) {
    change = 1;
    flame_alarm();
  }

  /*********************MODE************************/
  switch(mode) {
    
    /*********************CLOCK************************/
    case 0:
      //  Display data/time
      data_time(format);
      //  Change format
      if(yValue > 4){
        delay(300);
        format = !format;
      }
      break;
      
    /*********************CLOCK_SETUP************************/  
    case 1:
      lcd.setCursor(4,0);
      lcd.print("MENU ");
      lcd.write(byte(0));
      lcd.write(byte(1));
      xValue = readAxis(X_pin);
      yValue = readAxis(Y_pin);
      Button = digitalRead(SW_pin);

      /*********************INCREMENT_SETUP_MODE************************/
      if(yValue > 4) {
        delay(300);
        mode = 0;
        break;
      }
      if(xValue > 4) {
        delay(500);
        select++;
      }
      if(xValue < -4) {
        delay(500);
        select--;
      }
      if(select == 3) select = 0;
      if(select == -1) select = 2;
      
      lcd.setCursor(0,1);
      if(Button == LOW) {
        delay(500);
        en_select = true;
      }
      
      switch(select) {
        case 0:
          lcd.print("Change Date");
          if(en_select == true) {
            lcd.clear();
            active = false;
            change = 0;
            change_date(change);
          }
          en_select = false;
          break;
        case 1:
          lcd.print("Change Time");
          if(en_select == true) {
            lcd.clear();
            active = false;
            change = 0;
            change_time();
          }
          en_select = false;
          break;
        case 2:
          lcd.print("Set Alarm  ");
          if(en_select == true) {
            lcd.clear();
            active = false;
            change = 0;
            set_alarm();
          }
          en_select = false;
          break;
      }
      break;

    /*********************MP3************************/
    case 2:
      lcd.setCursor(0,0);
      lcd.print("     Music");
      change = 1;
      play_music(change);
      break;
  }
}


/*********************CASE 0************************/
void data_time(bool format) {
  
  //  Display Date
  //  Date 
  lcd.setCursor(0,0);
  lcd.print(Clock.getDateStr(0,1,'/'));
  //  DoW
  lcd.print(" ");
  lcd.print(Clock.getDOWStr(1));
  //  Time
  lcd.setCursor(0,1);
  switch(format) {
    case false:
      lcd.print(Clock.getTimeStr());
      lcd.print("    ");
      break;
    case true:
      if(meridian == true) {
        if(hour > 12) {
          if(hour - 12 < 10) lcd.print("0");
          lcd.print(hour - 12);
          lcd.print(":");
          if(min < 10) lcd.print("0");
          lcd.print(min);
          lcd.print(":");
          if(sec < 10) lcd.print("0");
          lcd.print(sec);       
        }
        else lcd.print(Clock.getTimeStr());
        lcd.print(" PM");
      }
      else {
        lcd.print(Clock.getTimeStr());
        lcd.print(" AM");
      }

      break;
  }
  if(hour >= 12) meridian = true;
  else meridian = false;
  
  DHT.read11(DHT_PIN);
  lcd.setCursor(12,1);
  lcd.print(Clock.getTemp(),0);
  lcd.setCursor(14,1);
  lcd.write(byte(2));
  lcd.print(" ");
  
}

/*********************CASE 1************************/
void change_date(int change) {
  date = Clock.getTime().date;
  mon = Clock.getTime().mon;
  year = Clock.getTime().year;
  dow = Clock.getTime().dow;
  while(change < 3) {
    xValue = readAxis(X_pin);
    yValue = readAxis(Y_pin);
    Button = digitalRead(SW_pin);

    if(yValue < -4) {
      delay(300);
      change++;
    }
          
    if(yValue > 4) {
      delay(300);
      change--;
    }
   
    if(Button == LOW) {
      delay(300);
      Clock.setDate(date,mon,year);
      Clock.setDOW(dow);
      change = 3;
    }
    
    if(change > 2 || change < 0) {
        lcd.clear();
        active = true;
        break;
    }
    
    //    Computting day of week
    int year_temp = year;
    if(mon < 3) year_temp = year_temp - 1; 
    dow = (year_temp + year_temp/4 - year_temp/100 + year_temp/400 + t[mon-1] + date) % 7;
    if(dow == 0) dow = 7;
    
    lcd.setCursor(0,0);
    lcd.print("dd/mm/yyyy");


    lcd.setCursor(0,1);
    if(date < 10) lcd.print("0");
    lcd.print(date);
    lcd.print("/");
    if(mon < 10) lcd.print("0");
    lcd.print(mon);
    lcd.print("/");
    lcd.print(year);
      
    switch(change) {
      case 0:
        if(xValue < -4){
          delay(200);
          date++;
        }
        if(xValue > 4) {
          delay(200);
          date--;
        }
        lcd.setCursor(12,0);
        lcd.print("dd  ");
        if(date > month_day[mon]) date = 1;
        if(date < 1) date = month_day[mon];
        break;
      case 1:
        if(xValue < -4){
          delay(200);
          mon++;
        }
        if(xValue > 4) {
          delay(200);
          mon--;
        }
        lcd.setCursor(12,0);
        lcd.print("mm  ");
        if(mon > 12) mon = 1;
        if(mon < 1) mon = 12;
        break;
      case 2:
        lcd.setCursor(6,1);
        if(xValue < -4){
          delay(400);
          year++;
        }
        if(xValue > 4) {
          delay(400);
          year--;
        }
        lcd.setCursor(12,0);
        lcd.print("yyyy");
        if(year < 2000) year = 2099;
        if(year > 2099) year = 2000;
        if(year % 4 == 0) month_day[1] = 29;
        else {
          if(mon == 1 && date == 29) date = 28;
          month_day[1] = 28;
        }
        break;
    }
  }
}

void change_time() {
  while(change < 3) {
    
    xValue = readAxis(X_pin);
    yValue = readAxis(Y_pin);
    Button = digitalRead(SW_pin);

    if(yValue < -4) {
      delay(300);
      change++;
    }
          
    if(yValue > 4) {
      delay(300);
      change--;
    }
    
    if(Button == LOW) {
      delay(300);
      Clock.setTime(hour,min,sec);
      change = 3;
    }
    
    if(change > 2 || change < 0) {
        lcd.clear();
        active = true;
        break;
    }
    
    lcd.setCursor(0,0);
    lcd.print("hh:mm:ss");
    lcd.setCursor(0,1);
    if(hour < 10) lcd.print("0");
    lcd.print(hour);
    lcd.print(":");
    if(min < 10) lcd.print("0");
    lcd.print(min);
    lcd.print(":");
    if(sec < 10) lcd.print("0");
    lcd.print(sec);
    
    lcd.setCursor(0,1);
    switch(change) {
      case 0:
        if(xValue < -4){
          delay(200);
          hour++;
        }
        if(xValue > 4) {
          delay(200);
          hour--;
        }
        lcd.setCursor(12,0);
        lcd.print("hh");
        if(hour > 23) hour = 0;
        if(hour >= 12) meridian = true;
        else meridian = false;
        if(hour < 0) hour = 23;
        break;
      case 1:
        lcd.setCursor(3,1);
        if(xValue < -4){
          delay(200);
          min++;
        }
        if(xValue > 4) {
          delay(200);
          min--;
        }
        lcd.setCursor(12,0);
        lcd.print("mm");
        if(min > 59) min = 0;
        if(min < 0) min = 59;
        break;
      case 2:
        lcd.setCursor(6,1);
        if(xValue < -4){
          delay(200);
          sec++;
        }
        if(xValue > 4) {
          delay(200);
          sec--;
        }
        lcd.setCursor(12,0);
        lcd.print("ss");
        if(sec > 59) sec = 0;
        if(sec < 0) sec = 59;
        break;
    }
  } 
}

void set_alarm() {
  while(change < 3) {

    xValue = readAxis(X_pin);
    yValue = readAxis(Y_pin);
    Button = digitalRead(SW_pin);


    if(yValue < -4) {
      delay(300);
      change++;
    }
          
    if(yValue > 4) {
      delay(300);
      change--;
    }

    if(Button == LOW) {
      Button = HIGH;
      delay(500);
      Button = digitalRead(SW_pin);
      if(Button == LOW) {
        Clock.setAlarm(alarm_hour,alarm_min,alarm_sec,en_alarm);
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("Alarm saved!!");
        delay(1500);
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
      }
      else {
        en_alarm = !en_alarm;
      }
    }
    
    if(change > 2 || change < 0) {
        lcd.clear();
        active = true;
        break;
    }
    
    lcd.setCursor(0,0);
    lcd.print("hh:mm:ss");
    
    lcd.setCursor(0,1);
    if(alarm_hour < 10) lcd.print("0");
    lcd.print(alarm_hour);
    lcd.print(":");
    if(alarm_min < 10) lcd.print("0");
    lcd.print(alarm_min);
    lcd.print(":");
    if(alarm_sec < 10) lcd.print("0");
    lcd.print(alarm_sec);

    lcd.setCursor(12,1);
    lcd.print("EN:");
    lcd.print(en_alarm);
    
    lcd.setCursor(0,1);
    switch(change) {
      case 0:
        if(xValue < -4){
          delay(200);
          alarm_hour++;
        }
        if(xValue > 4) {
          delay(200);
          alarm_hour--;
        }
        lcd.setCursor(12,0);
        lcd.print("hh");
        if(alarm_hour > 23) alarm_hour = 0;
        if(alarm_hour < 0) alarm_hour = 23;
        break;
      case 1:
        lcd.setCursor(3,1);
        if(xValue < -4){
          delay(200);
          alarm_min++;
        }
        if(xValue > 4) {
          delay(200);
          alarm_min--;
        }
        lcd.setCursor(12,0);
        lcd.print("mm");
        if(alarm_min > 59) alarm_min = 0;
        if(alarm_min < 0) alarm_min = 59;
        break;
      case 2:
        lcd.setCursor(6,1);
        if(xValue < -4){
          delay(200);
          alarm_sec++;
        }
        if(xValue > 4) {
          delay(200);
          alarm_sec--;
        }
        lcd.setCursor(12,0);
        lcd.print("ss");
        if(alarm_sec > 59) alarm_sec = 0;
        if(alarm_sec < 0) alarm_sec = 59;
        break;
    }
  }
}

void test_alarm(int h,int m,int s) {
    if(alarm_hour == h && alarm_min == m && alarm_sec == s) {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Time to wake!");
      int size = sizeof(melody) / sizeof(int);
      change = 1;
      while(change == 1) {
        for (int thisNote = 0; thisNote < size; thisNote++) {
          Button = digitalRead(SW_pin);
          if(Button == LOW) {
            active = true;
            en_select = false;
            en_alarm = 0;
            mode = 0;
            select = 0;
            change = 0;
            lcd.setCursor(0,1);
            lcd.clear();
            break;
          }
          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / tempo[thisNote];
          buzz(melodyPin, melody[thisNote], noteDuration);
       
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
      
          // stop the tone playing:
            
          buzz(melodyPin, 0, noteDuration);
        }
      }
    }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);
 
}

void flame_alarm() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fire in the hole");
  lcd.setCursor(0,1);
  for(int i=0;i<=15;i++) lcd.write(byte(3));
  while(change == 1) {
    Button = digitalRead(SW_pin);
    tone(buzzer, 1500);
    if(Button == LOW) {
      noTone(buzzer);
      change = 0;
      lcd.clear();
    }
    
  }
}

/*********************CASE 2************************/

void play_music(int change) {
  mp3_play(1);
  int i = 1;
  lcd.setCursor(0,1);
  lcd.print("Track ");
  lcd.print(i);
  bool play = true;
  while(change == 1) {
    xValue = readAxis(X_pin);
    yValue = readAxis(Y_pin);
  
    if(xValue < -4) {
      delay(300);
      mp3_set_volume(volume++);
    }
    if(xValue > 4) {
      delay(300);
      mp3_set_volume(volume--);
    }
    
    if(yValue < -4) {
      delay(300);
      mp3_next();
      i++;
      if(i > 4) i = 1;
      lcd.setCursor(0,1);
      lcd.print("Track ");
      lcd.print(i);
    }

    if(yValue > 4) {
      delay(300);
      mp3_prev();
      i--;
      if(i < 1) i = 4;
      lcd.setCursor(0,1);
      lcd.print("Track ");
      lcd.print(i);
    }

    lcd.setCursor(8,1);
    lcd.print("Vol: ");
    lcd.print(volume);
    lcd.print("%");
    if(volume < 10) {
      lcd.setCursor(15,1);
      lcd.print(" ");
    }
    
    //lcd.scrollDisplayLeft();
    //delay(150);
      
    Button = digitalRead(SW_pin);
    if(Button == LOW) {
      Button = HIGH;
      delay(500);
      Button = digitalRead(SW_pin);
      if(Button == LOW){
        mp3_stop();
        change = 0;
        active = true;
        mode = 0;
        lcd.clear();
        break;
      }
      play = !play;
      if(play == false) mp3_pause();
      else mp3_play();
    }
  }
}



/*********************READ AXIS************************/
int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

