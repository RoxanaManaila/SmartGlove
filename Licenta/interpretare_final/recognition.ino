#include <Wire.h>
#include <Adafruit_MPU6050.h>


Adafruit_MPU6050 mpu;

//define flex parameters
const int thumb = A6;  
const int index = A3;
const int middle= A2;
const int ring= A1;
const int little= A0;

//definire input power parameter
const float VCC = 5;     

//division factor
const float rezistor_fix = 10000.0; 

// define filter parameters
const float alpha = 0.5;
float filteredAccelX = 0;
float filteredAccelY = 0;
float filteredAccelZ = 0;

String text = ""; 
String textInit = "";
String eroare="EROARE";
char lastChar='\0';

void setup() {
  
  //inputs configurations
  pinMode(thumb, INPUT);
  pinMode(index, INPUT);
  pinMode(middle, INPUT);
  pinMode(ring, INPUT);
  pinMode(little, INPUT);

  Serial.begin(9600);
  
  
  //wait until serial port is available
  //and the connection has been established
  while (!Serial);

  //Initialize I2C(TWI) communication with MPU6050
  Wire.begin();

  //Verify if MPU6050 is connected
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  
  //Set acceleration and rotation range MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  
  //ADC reads
  int flex_ADC0 = analogRead(thumb);
  
  int flex_ADC1 = analogRead(index);
  
  int flex_ADC2 = analogRead(middle);
  
  int flex_ADC3 = analogRead(ring);
  
  int flex_ADC4 = analogRead(little);
  
  
  //Determine V0
  float flex_voltage0 = flex_ADC0 * VCC / 1023.0;
  
  float flex_voltage1 = flex_ADC1 * VCC / 1023.0;
  
  float flex_voltage2 = flex_ADC2 * VCC / 1023.0;
  
  float flex_voltage3 = flex_ADC3 * VCC / 1023.0;
  
  float flex_voltage4 = flex_ADC4 * VCC / 1023.0;
  
  
  //Determine each resistance using the division factor formula
  float rez_thumb = (rezistor_fix * (VCC / flex_voltage0 - 1.0))/1000;
  
  float rez_index = (rezistor_fix * (VCC / flex_voltage1 - 1.0))/1000;
  
  float rez_middle = (rezistor_fix * (VCC / flex_voltage2 - 1.0))/1000;
  
  float rez_ring = (rezistor_fix * (VCC / flex_voltage3 - 1.0))/1000;
  
  float rez_little = (rezistor_fix * (VCC / flex_voltage4 - 1.0))/1000;
  
  sensors_event_t accel, gyro, temp;

  mpu.getEvent(&accel, &gyro, &temp);

  //applying low-pass filter to the data acquired from the accelerometer and gyroscope module
  filteredAccelX = alpha * filteredAccelX + (1 - alpha) * accel.acceleration.x;
  filteredAccelY = alpha * filteredAccelY + (1 - alpha) * accel.acceleration.y;
  filteredAccelZ = alpha * filteredAccelZ + (1 - alpha) * accel.acceleration.z;

  textInit=text;

  if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>2 && rez_middle<3 && rez_ring>2 && rez_ring<3.07 && rez_little>3 && rez_little<4 && filteredAccelY>0 && filteredAccelZ>0)
  {
    text+='A';
    lastChar='A';
  }
  else if(rez_thumb>2 && rez_thumb<3 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<4.20)
  {
    text+='B';
    lastChar='B';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>2 && rez_middle<3 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<4 && filteredAccelZ>0)
  {
    text+='C';
    lastChar='C';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4 && filteredAccelX>0 && filteredAccelZ<0)
  {
    text+='D';
    lastChar='D';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>2 && rez_little<3 && filteredAccelZ<0 && filteredAccelZ>-4)
  {
    text+='E';
    lastChar='E';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>3 && rez_middle<4 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<4)
  {
    text+='F';
    lastChar='F';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>2 && rez_little<3 && filteredAccelZ<0)
  {
    text+='G';
    lastChar='G';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>2 && rez_ring<3 && rez_little>2 && rez_little<3 && filteredAccelZ<0)
  {
    text+='H';
    lastChar='H';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>4 && rez_little<5 && filteredAccelZ>0)
  {
    text+='I';
    lastChar='I';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>4 && rez_little<5 && filteredAccelZ<0)
  {
    text+='J';
    lastChar='J';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<4 && filteredAccelX>0 && filteredAccelY>0 && filteredAccelZ<0)
  {
    text+='K';
    lastChar='K';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4 && filteredAccelX>0 && filteredAccelZ>0)
  {
    text+='L';
    lastChar='L';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>2 && rez_middle<3 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4 && filteredAccelZ<0 && filteredAccelZ>-3)
  {
    text+='M';
    lastChar='M';
  }
  else if(rez_thumb>2 && rez_thumb<3 && rez_index>1 && rez_index<2 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4)
  {
    text+='N';
    lastChar='N';    
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>1 && rez_index<2 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4 && filteredAccelZ<-4)
  {
    text+='O';
    lastChar='O';
  }
  else if(rez_thumb>3 && rez_thumb<5 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<4 && filteredAccelX<0 && filteredAccelY>0 && filteredAccelZ>0)
  {
    text+='P';
    lastChar='P';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>2 && rez_little<3 && filteredAccelX<0 && filteredAccelY>0 && filteredAccelZ>0)
  {
    text+='Q';
    lastChar='Q';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>3 && rez_index<4 && rez_middle>2 && rez_middle<3.20 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4)
  {
    text+='R';
    lastChar='R';
  }
  else if(rez_thumb>2 && rez_thumb<3 && rez_index>1 && rez_index<2 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>2 && rez_little<3 && filteredAccelZ>0)
  {
    text+='S';
    lastChar='S';
  }
  else if(rez_thumb>4 && rez_thumb<5 && rez_index>1 && rez_index<2 && rez_middle>2 && rez_middle<3 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<4 && filteredAccelZ<0)
  {
    text+='T';
    lastChar='T';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>2 && rez_ring<3 && rez_little>2 && rez_little<3 && filteredAccelZ>0)
  {
    text+='U';
    lastChar='U';
  }
   else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4)
  {
    text+='V';
    lastChar='V';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>3 && rez_ring<4 && rez_little>2 && rez_little<3)
  {
    text+='W';
    lastChar='W';
  }
  else if(rez_thumb>2 && rez_thumb<3 && rez_index>1 && rez_index<2 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>2 && rez_little<3 && filteredAccelZ<0)
  {
    text+='X';
    lastChar='X';
  }
  else if(rez_thumb>3.88 && rez_thumb<5 && rez_index>1 && rez_index<2 && rez_middle>2 && rez_middle<3 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<4)
  {
    text+='Y';
    lastChar='Y';
  }
  else if(rez_thumb>3 && rez_thumb<4 && rez_index>2 && rez_index<3 && rez_middle>1 && rez_middle<2 && rez_ring>2 && rez_ring<3 && rez_little>3 && rez_little<4 && filteredAccelX<0 && filteredAccelZ>0)
  {
    text+='Z';
    lastChar='Z';
  }
  else if(rez_thumb>4 && rez_thumb<5 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>3 && rez_ring<4 && rez_little>4 && rez_little<5 && filteredAccelX>0 && filteredAccelY>0 && filteredAccelZ>0 && lastChar!=' ')
  {
    //SPACE function
    text+=' ';
    lastChar=' ';
  }
  else if(rez_thumb>3.80 && rez_thumb<5 && rez_index>2 && rez_index<3 && rez_middle>3 && rez_middle<4 && rez_ring>3 && rez_ring<4 && rez_little>3 && rez_little<5 && filteredAccelX<0 && filteredAccelY<0 && filteredAccelZ<0)
  {
    //DELETE function
    text = text.substring(0, text.length() - 1);
  }
  else if((rez_thumb>0 && rez_thumb<1) || (rez_index>0 &&rez_index<1) || (rez_middle>0 && rez_middle<1) || (rez_ring>0 && rez_ring<1) || (rez_little>0 && rez_little<1))
  {
    //this section of the code is specially designed for detecting possible hardware errors that I encountered
    text.concat(eroare);
    Serial.println("break");
    Serial.println(text);
    //bluetooth.println(text);
  }
   else 
  {
    //continue
  }

  if(text != textInit)
  {
    //bluetooth.println(text);
    Serial.println("break");

    Serial.println(text);
  }

  Serial.println();

  delay(3000);
}