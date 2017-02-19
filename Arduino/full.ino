#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial BT(5, 6);
//const int sensorPin = 0;
//const int buzzerPin = 10;
//int smoke_level;
String temp;
// Data wire is plugged into pin 8 on the Arduino
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup()
{
  
  Serial.begin(9600);
  // set digital pin to control as an output
  pinMode(13, OUTPUT);
  // set the data rate for the SoftwareSerial port
  BT.begin(9600);
  
  
  //pinMode(sensorPin, INPUT);//the smoke sensor will be an input to the arduino
  //pinMode(buzzerPin, OUTPUT);//the buzzer serves an output in the circuit
  sensors.begin();//begin temperature sensor
}
char a; // stores incoming character from other device

void loop(){
  
  getTemp();
  //getGas(); 
   }
  
void bluetooth(){
   if (BT.available())
  // if text arrived in from BT serial...
  {
    a=(BT.read());
    if (a=='1')
    {
      digitalWrite(13, HIGH);
      BT.println("LED on");
      //getGas();
    }
    if (a=='2')
    {
      digitalWrite(13, LOW);
      BT.println("LED off");
    }
    if (a=='?')
    {
      BT.println("Send '1' to turn LED on");
      BT.println("Send '2' to turn LED on");
    }   
    // you can add more "if" statements with other characters to add more commands
  }
}

void getGas(){
   float sensor_volt;
    float RS_air; //  Get the value of RS via in a clear air
    float R0;  // Get the value of R0 via in H2
    float sensorValue;

    /*--- Get a average data by testing 100 times ---*/
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue = sensorValue + analogRead(A0);
    }
    sensorValue = sensorValue/100.0;
    /*-----------------------------------------------*/

    sensor_volt = sensorValue/1024*5.0;
    RS_air = (5.0-sensor_volt)/sensor_volt; // omit *RL
    R0 = RS_air/9.8; // The ratio of RS/R0 is 9.8 in a clear air from Graph (Found using WebPlotDigitizer)

    BT.print("sensor_volt = ");
    BT.print(sensor_volt);
    BT.println("V");

    BT.print("R0 = ");
    BT.println(R0);
    delay(1000);
}

void getTemp(){
  
  // Send the command to get temperatures
  
  sensors.requestTemperatures();  
  temp = sensors.getTempCByIndex(0);
  BT.print("#");
  BT.println(temp); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  //Update value every 1 sec.
  delay(1000);
  Serial.println(temp);
  
 
}

