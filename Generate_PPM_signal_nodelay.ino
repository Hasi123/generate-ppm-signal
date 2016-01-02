//this programm will put out a PPM signal

//////////////////////CONFIGURATION///////////////////////////////
#define chanel_number 8  //set the number of chanels
#define default_servo_value 1500  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 1  //set polarity: 1 is positive, 0 is negative
#define sigPin 10  //set PPM signal pin on the arduino
//////////////////////////////////////////////////////////////////


/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values are between 1000 and 2000)*/
int ppm[chanel_number];

void setup(){  
  //initiallize default ppm values
  for(int i=0; i<chanel_number; i++){
    ppm[i]= default_servo_value;
  }

  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
}

void loop(){
  //put main code here
  ppmWrite();
}

void ppmWrite(){  //generate PPM signal
  static unsigned long lastFrLen;
  static unsigned long lastServo;
  static unsigned long lastPulse;
  static boolean PPM_run;
  static boolean pulse;
  static boolean pulseStart = true;
  static byte counter;
  static byte part = true;

  if(micros() - lastFrLen >= PPM_FrLen){  //start PPM signal after PPM_FrLen has passed
    lastFrLen = micros();
    PPM_run = true;
  }

  if(counter >= chanel_number){
    PPM_run = false;
    counter = 0;
    pulse = true;  //put out the last pulse
  }

  if(PPM_run){
    if (part){  //put out the pulse
      pulse = true;
      part = false;
      lastServo = micros();
    }
    else{  //wait till servo signal time (values from the ppm array) has passed
      if(micros() - lastServo >= ppm[counter]){
        counter++;  //do the next channel
        part = true;
      }
    }
  }

  if(pulse){
    if(pulseStart == true){  //start the pulse
      digitalWrite(sigPin, onState);
      pulseStart = false;
      lastPulse = micros();
    }
    else{  //will wait till PPM_PulseLen has passed and finish the pulse
      if(micros() - lastPulse >= PPM_PulseLen){
        digitalWrite(sigPin, !onState);
        pulse = false;
        pulseStart = true;
      }
    }
  }
}
