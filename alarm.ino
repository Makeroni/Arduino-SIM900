#include <SoftwareSerial.h>
#include <EEPROM.h>

// configure SIM900 software serial port
SoftwareSerial SIM900(7, 8);

// Verbose output
int verbose = 1;

// Power SIM900 on
void SIM900powerOn()
{
  if(verbose)
  {
    Serial.write("Powering SIM900 on\n");
  }
  SIM900flush();
  // Test command
  SIM900.print("AT+CLIP=1\r");
  delay(500);
  // Power on if no response
  if(SIM900.available()==0)
  {
    // Emulate button click
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(1000);
    // Wait network connection
    delay(10000);
    // Turn on caller ID notification
    SIM900.print("AT+CLIP=1\r");
    delay(500);
  }
  SIM900flush();
}

// Power SIM900 off
void SIM900powerOff()
{
  if(verbose)
  {
    Serial.write("Powering SIM900 off\n");
  }
  SIM900flush();
  // Test command
  SIM900.print("AT+CLIP=1\r");
  delay(500);
  // Power off if no response
  if(SIM900.available()!=0)
  {
    // Emulate button click
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(1000);
  }
}

// Flush SIM900 serial port
void SIM900flush()
{
  while(SIM900.available()!=0)
  {
    SIM900.read();
  }
}

// Check if SIM900 is ringing
int SIM900ringing()
{
  char aux[100];
  char msg[100];
  int i=0;
  if(SIM900.available()!=0)
  {
    // Wait caller number
    delay(2000);
    // Read data in memory buffer
    while(SIM900.available()!=0)
    {
      msg[i++] = SIM900.read();
    }
    // Check RING header
    if( msg[2] == 'R' && msg[3] == 'I' &&
        msg[4] == 'N' && msg[5] == 'G' )
    {
      if(verbose)
      {
        Serial.write("Phone is ringing\n");
      }
      // Check number delimiter
      if(  msg[17] == '"' &&  msg[27] == '"' )
      {
        // Check number starts with 6 or 9
        if( msg[18] != '6' && msg[18] != '9' )
        {
          return 0;
        }
        // Check number integrity
        for( i=0 ; i<9 ; i++ )
        {
          if( msg[18+i] < '0' || msg[18+i] > '9' )
          {
            return 0;
          }
        }
        // Store number twice in EEPROM
        for( i=0 ; i<9 ; i++ )
        {
          EEPROM.write(i  , msg[18+i]);
          EEPROM.write(i+9, msg[18+i]);
        }
        if(verbose)
        {
          Serial.write("Saving home (");
          for( i=0 ; i<9 ; i++ )
          {
            Serial.write(EEPROM.read(i));
          }
          Serial.write(")\n");
        }
        // Hang up
        SIM900.println("ATH");
        delay(2000);
        return 1;
      }
    }
  }
  return 0;
}

// Expect a call for n seconds
int SIM900expectCall(int n)
{
  int i;
  if(verbose)
  {
    Serial.write("Expecting a call\n");
  }
  for(i=0;i<n;i++)
  {
    if(SIM900ringing())
    {
      return 1;
    }
    delay(1000); // wait one second
  }
  return 0;
}

// Check last stored call in EEPROM
int SIM900lastCall()
{
  char phone[9];
  int i;
  // Read EEPROM in memory buffer
  for( i=0 ; i<9 ; i++ )
  {
    phone[i] = EEPROM.read(i);
  }
  // Check number starts with 6 or 9
  if( phone[0] != '6' && phone[0] != '9' )
  {
    if(verbose)
    {
      Serial.write("Invalid phone number\n");
    }
    return 0;
  }
  // Check number integrity
  for( i=0 ; i<9 ; i++ )
  {
    if( phone[i] < '0' || phone[i] > '9' )
    {
      if(verbose)
      {
        Serial.write("Invalid phone number\n");
      }
      return 0;
    }
  }
  // Check data duplicity
  for( i=0 ; i<9 ; i++ )
  {
    if( phone[i] != EEPROM.read(i+9) )
    {
      if(verbose)
      {
        Serial.write("Invalid phone number\n");
      }
      return 0;
    }
  }
  return 1;
}

// Call home during n seconds
void SIM900callHome(int n)
{
  char phone[10];
  char msg[100];
  int i;
  // Check valid number
  if(SIM900lastCall())
  {
    if(verbose)
    {
      Serial.write("Calling home (");
      for( i=0 ; i<9 ; i++ )
      {
        Serial.write(EEPROM.read(i));
      }
      Serial.write(")\n");
    }
    // Read EEPROM in memory buffer
    for( i=0 ; i<9 ; i++ )
    {
      phone[i] = EEPROM.read(i);
    }
    // Add end of string character
    phone[9] = '\0';
    sprintf(msg, "ATD + +34%s;", phone);
    // Dial
    SIM900.println(msg);
    // Wait n seconds
    delay(n*1000);
    // Hang up
    SIM900.println("ATH");
    delay(500);
  }
}

void setup()
{
  // Configure serial ports
  Serial.begin(19200);
  SIM900.begin(19200);
  delay(1000);
  // Power SIM900 on
  SIM900powerOn();
  // Expect a call for 60 seconds
  SIM900expectCall(60);
  // Short call back confirmation
  SIM900callHome(10);
  // Power SIM900 off
  SIM900powerOff(); 
}

void loop()
{
  int alarm = 0;
  if(alarm)
  {
    // Power SIM900 on
    SIM900powerOn();
    // Long call back alarm
    SIM900callHome(15);
    // Power SIM900 off
    SIM900powerOff(); 
  }
}
