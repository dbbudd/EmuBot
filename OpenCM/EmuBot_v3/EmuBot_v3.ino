/*
 
Note that all values in <> are ASCII decimal integers. 
<pos> is a value from 0 to 1023. 
<vel> is a velocity, 0 to 1023 is forward, 1024-2047 is negative. 
 
Possible commands: 
 
"Wj<ID>" to make motor <ID> enter joint mode. 
"Wp<ID>,<pos>,<vel>" to make motor <ID> move to position <pos> with velocity <vel>. 
"Ww<ID>,<add>,<val>" to write <val> to register <add> of motor <ID>. 
 
*/

Dynamixel Dxl(1); 
 
void setup() 
{
  Serial2.begin(57600); 
  Dxl.begin(3); 
}
 
byte blockingRead()
{
  while (!Serial2.available()) delay(10); 
  return Serial2.read(); 
}


boolean readInt(int *outVal)
{
  
  byte inBuffer[sizeof(short)]; 
  int i; 
  for (i = 0; i < sizeof(short); i ++)
  {
    inBuffer[i] = blockingRead();
  }
  *outVal = *((short*)inBuffer); 
  
  return true; 
}

void processWrite(byte cmd, byte id)
{
          switch (cmd)
        {
          case 'j':
          {
            //Set Joint Mode
            Dxl.jointMode(id); 
            break; 
          }
          case 'w':
          {
            //Set Wheel Mode
            Dxl.wheelMode(id);
            break;
          }
          case 's':
          {
            //Set Speed
            int add, val;
            if (!(readInt(&add) && readInt(&val))) break; 
            //Dxl.writeWord(id, add, val);
            Dxl.goalSpeed(id, val);
            break;
          }
          case 'p':
          {
            //move to position
            int pos, vel; 
            if (!(readInt(&pos) && readInt(&vel))) break; 
            Dxl.setPosition(id, pos, vel);
            break; 
          }
          case 'g':
          {
            //General write command
            int add, val; 
            if (!(readInt(&add) && readInt(&val))) break; 
            Dxl.writeWord(id, add, val);
            break;
            
            //examples of using writeWord()
            //Dxl.writeWord(id, 30, val); //move to Goal Position
            //Dxl.writeWord(id, 32, val); //move at Speed
            //Dxl.writeWord(id, 8, 0); //set wheel mode
          }
        }
}

void processRead(byte cmd, byte id)
{
  /* Gets sent some 'complex' command in the cmd byte and then executes */
  switch (cmd)
  {
    case 'j':
    {
      //read current joint position
      char str[4]; //create an array of characters
      int output = Dxl.readWord(id, 36); //query the dynamixel
      itoa(output, str, 10); //turns integer to ascii
      //return Position. Values between 0-999
      Serial2.write(str);
      break;
    }
    case 'm':
    {
      //read if wheel is moving
      char output = Dxl.readWord(id, 46); //query the dynamixel
      Serial2.write(output);
      break;
    }
    case 's':
    {
      //read current speed
      char str[4]; //create an array of characters
      int output = Dxl.readWord(id, 38); //query the dynamixel
      itoa(output, str, 10); //turns integer to ascii
      Serial2.write(str);
      break;
    }
  }
}
 
void loop() 
{
  while (Serial2.available())
  {
    // Wait for a start byte, "W" for write or "R" for read. 
    byte inByte = blockingRead();
    switch (inByte){
      case 'W':
      {
        byte cmd = blockingRead();
        byte id = blockingRead();
        processWrite(cmd, id); 
        break;
      } 
      case 'R':
      {
        byte cmd = blockingRead();
        byte id = blockingRead();
        processRead(cmd, id);
        break;
      }
    }
  }    
}
 

