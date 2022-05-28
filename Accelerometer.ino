#include <Wire.h>
#include <OneWire.h>
#include <LiquidCrystal.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define en 8
#define rs 9
#define d4 4
#define d5 5
#define d6 6
#define d7 7


#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float Celcius=0;
float Fahrenheit=0;
float humidity = 0;
const unsigned reading_count = 10; // Numbber of readings each time in order to stabilise
unsigned int analogVals[reading_count];
unsigned int counter = 0;
unsigned int values_avg = 0;
int HumStat = 0; // Status Variable for humidity- It can be Dry, Wet, Normal.

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  sensors.begin();
  
  Serial.println("Accelerometer Test"); Serial.println("");
  if (!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
  displaySensorDetails();
  displayDataRate();
  displayRange();
  Serial.println("");
  Serial.println();
  
  for (int i = 8; i < 13; i++)
  {
    pinMode(i, OUTPUT);
  }

  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {

  // ----------------------------- ACCELEROMETER
  sensors_event_t event;
  accel.getEvent(&event);

  // ----------------------------- TEMPERATURE
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);

//  Serial.print(" C  ");
//  Serial.print(Celcius);
//  Serial.print(" F  ");
//  Serial.println(Fahrenheit);
  
//    /* Display the results (acceleration is measured in m/s^2) */
//  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" ");
//  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
//  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" "); Serial.println("m/s^2 ");
//  delay(50);

  for ( counter = 0; counter < reading_count; counter++) {
    analogVals[reading_count] = analogRead(A0);
    delay(100);
    values_avg = (values_avg + analogVals[reading_count]);
  }
  values_avg = values_avg / reading_count;
  Serial.print("Average Readings value: ");
  Serial.println(values_avg);
  
  lcd.clear();
  lcd.setCursor(0, 0);  lcd.print("X:");  lcd.print(event.acceleration.x);  lcd.print(" Y:");   lcd.print(event.acceleration.y);
  lcd.setCursor(0, 1);  lcd.print("Z:");  lcd.print(event.acceleration.z); 
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);  lcd.print("  Temperature  ");
  lcd.setCursor(0, 1);  lcd.print(Celcius); lcd.print((char)176); lcd.print("C | "); lcd.print(Fahrenheit); lcd.print("F");
  delay(1000);
  
  lcd.clear();
  lcd.print("Humidity : ");  lcd.print(values_avg);
  delay(1000);
  
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print ("Sensor: "); Serial.println(sensor.name);
  Serial.print ("Driver Ver: "); Serial.println(sensor.version);
  Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id);
  Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print ("Resolution: "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print ("Data Rate: ");

  switch (accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print ("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print ("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print ("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print ("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print ("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print ("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print ("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print ("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print ("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print ("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print ("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print ("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print ("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print ("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print ("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print ("0.10 ");
      break;
    default:
      Serial.print ("???? ");
      break;
  }
  Serial.println(" Hz");
}


void displayRange(void)
{
  Serial.print ("Range: +/- ");

  switch (accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print ("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print ("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print ("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print ("2 ");
      break;
    default:
      Serial.print ("?? ");
      break;
  }
  Serial.println(" g");
}
