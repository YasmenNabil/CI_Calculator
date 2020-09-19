// include the library code:
int a0=A0; //a0 in 8255 is connected to A0 in arduino
int a1=A1; //a1 in 8255 is connected to A1 in arduino
int RD=9; //RD in 8255 is connected to pin 9 in arduino
int WR=8; //WR in 8255 is connected to pin 8 in arduino

//pre-calculations
bool first = false;
bool negative;
int symbol = '0';//character for symbols
int Pressed;//save the value of pressed col
bool new_operation=false;
bool clearr = true;
int sign = 0;
int change = 0;

//Calculations
double number1 = 0 ;
double number2 = 0 ;
double result = 0;//result of operations

//Keypad
const byte row = 4;
const byte col = 4;
byte rowPins[row] = {0, 1, 2, 3}; //rows connected with pin 0 : 3 in 8255
byte colPins[col] = {4, 5, 6, 7}; //columns connected with pin 4 : 7 in 8255
int keypad[row][col] = {'7', '8', '9', '/',
                        '4', '5', '6', '*',
                        '1', '2', '3', '-',
                        '.', '0', '=', '+'
                       };
void setup()
{
  // put your setup code here, to run once:
  //kolhom outout 34an htl3 mnhom signal ya 0 ya 1 mn al arduino ll 8255
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(a0, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(WR, OUTPUT);
}

void KeyPressed()
{
  for (int i = 0; i <= 3; i++) //loop on the 4 rows
  { 
    //select PortC
    digitalWrite(a0, 0); 
    digitalWrite(a1, 1);
        
    //enable Write 
    digitalWrite(WR, 0); // enable WR 
    delay(1);
    digitalWrite(WR, 1); // disable WR
    
    //set the row i = 0 ACTIVE LOW to check if the preesed key located in this row or another one
    digitalWrite(i, 0);
    
    //enable write the send the row to the arduino
    digitalWrite(WR, 0); //enable WR
    delay(1);
    digitalWrite(WR, 1); //disable WR

    // Get the selected Column
    for (int j = 4; j <= 7; j++) //loop on the cols,which connected to pins 4:7 in port C
    {
      //Make Pins 4:7 in input Mode to read from 8255
      pinMode(4, INPUT);
      pinMode(5, INPUT);
      pinMode(6, INPUT);
      pinMode(7, INPUT);
  
      digitalWrite(RD, 0);// enable RD to read col number
      Pressed = digitalRead(j); //num of the pressed/selected col 
      //ERROR
      delay(10);
      digitalWrite(RD, 1);//disable RD

      //Return Pins 4:7 as OUTPUT Mode
      pinMode(4, OUTPUT);
      pinMode(5, OUTPUT);
      pinMode(6, OUTPUT);
      pinMode(7, OUTPUT);
      
      if (Pressed == 0)
      {
        //if the pressed key is a sign - + / * .
        if(new_operation == true) //msh awl operation
        {
          if(negative == false) //3chan lw b3d operation hbd2 b -ve number
          {
             LCD(0x01,0);//clear lcd
             delay(30);;
          }
         
        }
                                   // '/'                               //'*'                                //'-'                                //'+'                               //'='                                   //'.'  Clear   
        if ((keypad[i][j - 4] == keypad[0][3]) | (keypad[i][j - 4] == keypad[1][3]) | (keypad[i][j - 4] == keypad[2][3]) | (keypad[i][j - 4] == keypad[3][3]) | (keypad[i][j - 4] == keypad[3][2]) | (keypad[i][j - 4] == keypad[3][0]))
        { 
          //clear LCD
          if (keypad[i][j - 4] == '.')
          {
            LCD(0x01,0); //command to clear LCD
            ResetVariables();           
            break;
          }  

          //=
          if (keypad[i][j - 4] == '=')
          {
            if(clearr == true) //lw bad2 b =
            {
              StringOnLcd("ERROR");
              ResetVariables();
              new_operation=true;
              break;
            }
            LCD('=',1);//write = sign on LCD
            operation();
            ResetVariables();
            break;
          }
          
          //tany rakm -ve , sign = 1 y3ni da5l  sign 2abl - w l - de -ve lrakm
          if(keypad[i][j - 4] == '-' && sign == 1) 
          {
              LCD('-',1);
              change = 1;
              negative=true;
          }

          //bd2t b rakm -ve
          if(keypad[i][j - 4] == '-' && clearr == true)
          {
            LCD('-',1);
            symbol='0';
            negative=true;
          }

          //get sign
          else
          {
            if(sign == 0)
            {
               symbol = keypad[i][j - 4] ;
               LCD(symbol,1);
               sign = 1;
            }
            //else yb2a ale da5l -ve tany rakm msh sign f msh hy3'er lsymbol
          }
           
        }
        else // Pressed Key is not a sign , It's a number
        {
          LCD(keypad[i][j - 4],1); //write the pressed number on lcd
          clearr = false;
          new_operation = false; //3chan y3rf ya5od tany rakm my3mlsh clear 3'er b3d =
          if (symbol == '0') 
          {
            number1 = ( number1 * 10 ) + ( keypad[i][j - 4]) - '0'  ; 
          }

         else
          {
            number2 = ( number2 * 10 ) +  (keypad[i][j - 4]) - '0'  ;
          }
          break;
        }
      }
    }
    digitalWrite(i, 1); //row=1 hy5ly al row be high 34an ykml al loop
    digitalWrite(WR, 0); //enable WR 
    delay(1);
    digitalWrite(WR, 1); //disable WR
  }
}

void ResetVariables()
{
   symbol = '0' ;             
   number1 = 0 ;
   number2 = 0 ;
   clearr = true;
   sign = 0;
   change = 0;
}

//To send command RS=0 & E = high to-low 
//To send data RS=1 & E = high to-low 
void LCD(int num , int rs) 
{
   //port B is selected
  digitalWrite(a0, 1);
  digitalWrite(a1, 0);
  for (int i = 0; i < 8; i++)//For loop to enter i to the pins of port B for ic
  {
    digitalWrite(i, (num >> i) & 0x01);// Here we make anding for i and 1 to take it to port b
  }
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
  
   //port A is selected
  digitalWrite(a0, 0); 
  digitalWrite(a1, 0);
  //to send command make E=1
  digitalWrite(6, rs);
  digitalWrite(7, 1);
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
  
  //then make E=0
  digitalWrite(6, rs);
  digitalWrite(7, 0);
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
}

void operation() //Calculating the input
{
  //awl rakm -ve
  if (negative == true & change == 0)//If number1 is -ve multiply by -1 to make it -ve number
  {      
    negative=false;
    number1=number1*-1; 
  }

  //tany rakm -ve
  if (negative == true & change == 1)//If number2 is -ve multiply by -1 to make it -ve number
  {      
    negative=false;
    sign = 0;
    change = 0;
    number2=number2*-1; 
  }
  
  if (symbol == '+')
  {
    result = (number1 + number2);
    String str = String(result);
    StringOnLcd(str);
  }

   if (symbol == '*')
  {
    result = (number1 * number2);
    String str = String(result);
    StringOnLcd(str);
  }

  if (symbol == '-')
  {
    result = (number1 - number2);
    String str = String(result);
    StringOnLcd(str);
  }
 
  if (symbol == '/')
  {
    if (number2 == 0) 
    {
      StringOnLcd("Invalid");
      ResetVariables();
      new_operation == true;
    }
    else
    {
      result = (number1 / number2);
      String str = String(result);
      StringOnLcd(str);
    }
  }
}

void StringOnLcd(String OutPut) //display the result one char every iteration on the loop 
{
  for (int i = 0; i < 8; i++)
  {
    LCD(OutPut[i],1);
    delay(1);
  }
  ResetVariables();
  new_operation=true;
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //if condition to set control word at first time
  if (first == false)
  {
    digitalWrite(RD, 1);   //READ DISABLED
    digitalWrite(WR, 1);  //WRITE DISABLED

   //Selecting Control Register of 8255
    digitalWrite(a0, 1); 
    digitalWrite(a1, 1);  

    digitalWrite(7, 1);  // active I/O mode
    digitalWrite(6, 0);  // select Mode0
    digitalWrite(5, 0);  // ""
    digitalWrite(4, 0);  // portA Output
    digitalWrite(3, 1);  // CU Input "cols"
    digitalWrite(2, 0);  // Mode0
    digitalWrite(1, 0);  // portB Output
    digitalWrite(0, 0);  // CL Output "rows"

    digitalWrite(WR, 0);//WRITE ENABLED
    delay(10);
    digitalWrite(WR, 1);//WRITE DISABLED
    
    LCD(0x38,0);//intialize lcd 2 lines
    delay(30);
    LCD(0x0E,0);//display blinking cursor 
    delay(30);
    LCD(0x01,0);//clear lcd
    delay(30);
    first = true;
  }
  KeyPressed();
}
