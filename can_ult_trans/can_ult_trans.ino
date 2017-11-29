#include <mcp_can.h>
#include <SPI.h>


//can init

const int SPI_CS_PIN = 10;

long unsigned int txID = 0x1881ABBA; // This format is typical of a 29 bit identifier.. the most significant digit is never greater than one.
unsigned char stmp[8] = {0x0E, 0x00, 0xFF, 0x22, 0xE9, 0xFA, 0xDD, 0x51};

MCP_CAN CAN(SPI_CS_PIN);

//ult pin declare
int echoPort = 5;
int triggerPort = 6;

void setup()
{
  pinMode( triggerPort, OUTPUT );
  pinMode( echoPort, INPUT );
  Serial.begin(115200);

  while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 250K
    {
         Serial.println("CAN BUS Module Failed to Initialized");
        Serial.println("Retrying....");
        delay(200);
        
    }
    Serial.println("CAN BUS Shield init ok!");
}

void ult_init()
{
  digitalWrite(triggerPort, LOW);      // set to LOW trigger's output
  digitalWrite(triggerPort, HIGH);    // send a 10us pulse to the trigger
  delayMicroseconds( 100 );
  digitalWrite(triggerPort, LOW);
}
// main loop operation

void loop()
{
  ult_init();

  //ult operation

  long duration = pulseIn(echoPort, HIGH); 
  long r = 3.4 * duration / 2;      // here we calculate the distance using duration
  
  float distance = r / 100.00;

  /*Serial.print("duration: ");
  Serial.print(duration);
  Serial.print(" , ");
  Serial.print("distance: ");*/

  // can transmission

  Serial.println("Can tansmission starts");
   //CAN.sendMsgBuf(txID,1, 8, stmp);

   CAN.sendMsgBuf(txID,1,1,1);    
   delay(25);    
  
}

