int x[8];
void setup() 
{
  
  for(int i=4; i<12; i++)
  {
    pinMode(i, OUTPUT);
  }
  
  Serial.begin(9600);
  
  for ( int i = 0; i<8; i++)
  {
   x[i]=0; 
  }
  
}

void loop() 
{
  int r= analogRead(A0);
  int y = map(r,0,1023,0,255);
 
//Convert to Binary
  for (int i = 0; i<8; i++)
  {
    if (y>1)
    {
    x[i]= y - ((y/2)*2);
    y = y/2;
    }
    else
    {
    x[i]=y;
    break;
    }
  }
  
 /* for (int i = 7; i>=0; i--)
  {
   Serial.println(x[i]); 
  }*/
                                    
//Converting to Decimal
  float dec = 0;
  for (int i= 0; i<8; i++)
  {
    dec = dec + pow(2,i)*x[i];
  }

//Converting to Voltage
  float vol = (dec/255)*5;
  Serial.println(vol);
  
//Map to Pins
  for (int i = 0; i<8; i++)
  {
   digitalWrite(i+4 , x[7-i]);
   delay(1000);
  }
}
