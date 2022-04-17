//  Set TEMP_RESOLUTION to the corresponding resolution of used DS18x20 sensor:
//  18S20: 9  (default setting; can be 9,10,11,or 12)
//  18B20: 12
const unsigned short TEMP_RESOLUTION = 9;

char *text = "000.0000";
unsigned temp;

char myString[2];

void afficheChiffre(char chiffre,char position);
void afficheNombre(char nb);
void Display_Temperature(unsigned int temp2write);



void main() {
  ANSELE = 0;                                    // Configure PORTE pins as digital
  TRISE.B2 = 1;                                  // Configure RE2 pin as input
  ANSELA = 0x00;
  ANSELD = 0x00;
        // ENTREE/ SORTIE
  TRISA = 0x00;
        // trigger = 0, echo = 1
  TRISD=0x00;
  LATA = 0x00;

  // bluetooth

TRISD=0X00;
PORTD=0X00;

//TRISC.F7 = 1;
//TRISC.B6=1;
//TRISC.B7=1;
UART1_Init(9600);
UART1_Write_text(" Bluetooth starting");


   // Initialize hardware UART1 and establish communication at 9600 bps
        UART1_Init(9600);
//        UART1_Write(13);
//        UART1_Write(10);
        Delay_ms(100);
  // Main loop
  do {
    // Perform temperature reading
    Ow_Reset(&PORTE, 2);                         // Onewire reset signal
    Ow_Write(&PORTE, 2, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&PORTE, 2, 0x44);                   // Issue command CONVERT_T
    Delay_ms(10);

    Ow_Reset(&PORTE, 2);
    Ow_Write(&PORTE, 2, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&PORTE, 2, 0xBE);                   // Issue command READ_SCRATCHPAD

    temp =  Ow_Read(&PORTE, 2);
    temp = (Ow_Read(&PORTE, 2) << 8) + temp;

    // Format and display result on Lcd
    Display_Temperature(temp);
  } while (1);
}


 void afficheChiffre(char chiffre,char position){
         // LATA (registre LAT port A)
         // 0x01 = 0000 0001
         // << : decalage a gauche
         // PORTA : afficheur, PORTD : chiffre
         // position : [0-3]

         // on choisit le display
         LATA = 0x01 << position;

         switch(chiffre){
                         case 0:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                         LATD4_bit = 1;LATD5_bit = 1;LATD6_bit = 0;LATD7_bit = 0;
                         break;

                         case 1:LATD0_bit = 0;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 0;
                         LATD4_bit = 0;LATD5_bit = 0;LATD6_bit = 0;LATD7_bit = 0;
                         break;

                         case 2:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 0;LATD3_bit = 1;
                         LATD4_bit = 1;LATD5_bit = 0;LATD6_bit = 1;LATD7_bit = 0;
                         break;

                         case 3:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                         LATD4_bit = 0;LATD5_bit = 0;LATD6_bit = 1;LATD7_bit = 0;
                         break;

                         case 4:LATD0_bit = 0;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 0;
                         LATD4_bit = 0;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                         break;

                         case 5:LATD0_bit = 1;LATD1_bit = 0;LATD2_bit = 1;LATD3_bit = 1;
                         LATD4_bit = 0;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                         break;

                         case 7:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 0;
                         LATD4_bit = 0;LATD5_bit = 0;LATD6_bit = 0;LATD7_bit = 0;
                         break;

                         case 8:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                         LATD4_bit = 1;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                         break;

                         case 9:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                         LATD4_bit = 0;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;

                         default:
                         break;
                         }
}


      void afficheNombre(char nb){
           char chiffre0,chiffre1,chiffre2,chiffre3;

           // on décompose le nombre digit par digit

           chiffre3 = nb/1000;
           nb -= chiffre3*1000;

           chiffre2 = nb/100;
           nb -= chiffre2*100;

           chiffre1 = nb/10;
           nb -= chiffre1*10;

           chiffre0 = nb;

           // affiche le nombre avec la fonction afficheChiffre()

           afficheChiffre(chiffre0,0);
           Delay_ms(1);
           afficheChiffre(chiffre1,1);
           Delay_ms(1);
           afficheChiffre(chiffre2,2);
           Delay_ms(1);
           afficheChiffre(chiffre3,3);
           Delay_ms(1);
      }

 void Display_Temperature(unsigned int temp2write){
  const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
  char temp_whole;
  unsigned int temp_fraction;

  // Check if temperature is negative
  if (temp2write & 0x8000) {
    text[0] = '-';
    temp2write = ~temp2write + 1;
  }

  // Extract temp_whole
  temp_whole = temp2write >> RES_SHIFT ;

  // Convert temp_whole to characters
  if (temp_whole/100)
    text[0] = temp_whole/100  + 48;
  else
    text[0] = '0';

  text[1] = (temp_whole/10)%10 + 48;
  text[2] =  temp_whole%10     + 48;             // Extract ones digit

  // Extract temp_fraction and convert it to unsigned int
  //temp_fraction  = temp2write << (4-RES_SHIFT);
  //temp_fraction &= 0x000F;
  //temp_fraction *= 625;

  // Convert temp_fraction to characters
  text[3] =  temp_fraction/1000    + 48;         // Extract thousands digit
  text[5] = (temp_fraction/100)%10 + 48;         // Extract hundreds digit
  text[6] = (temp_fraction/10)%10  + 48;         // Extract tens digit
  text[7] =  temp_fraction%10      + 48;         // Extract ones digit


  strncpy(myString, text, 2);
  myString[2]='\0';

    UART1_Write_Text(myString);
//   UART1_Write_Text(text);
                //UART1_Write(13);
                //UART1_Write(10);
                Delay_ms(1000);

  afficheNombre(temp_whole/10);
}