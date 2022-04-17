//////////////////////////////////////////////////////////////////////////
//--------------------Capteur Ultrason SR04---------------------//
//////////////////////////////////////////////////////////////////////////



// char/ int : the difference is the size in byte of the variable, and from there the different values the variable can hold.

                      /*
                        Library :
                                UART
                                Conversions
                      */


   // char counter;
    int etape;



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





void main(void) {

        // déclaration des var en amont sinon ; invalid expression
        int etape = 0;
        int compte = 0;
        float nombreaffiche = 0;
        // stocker la distance sous chaine de char
        char distChar[4];

        /*
          A adapter selon l'installation Hardware !!!!
          (Trigger, Echo)
        */
        // DIGITAL I/O
        ANSELA = 0x00;
        ANSELB = 0x00;
        ANSELC = 0x00;
        ANSELD = 0x00;
        // ENTREE/ SORTIE
        TRISA = 0x00;
        // trigger = 0, echo = 1
        TRISB = 0x01;
        TRISD=0x00;
        // PORTA=00000000, PORTB = 00000000
        LATA = 0x00;
        LATB = 0x00;

        etape = 0;


        // Initialize hardware UART1 and establish communication at 9600 bps
        UART1_Init(9600);
        UART1_Write(13);
        UART1_Write(10);
        Delay_ms(100);


    while(1)
    {
        switch(etape)
        {
            case 0:
                    //Impulsion trigger avec (etat haut)
                     LATB1_bit = 1;
                     compte = 0;
                     etape++ ;
                     break;
            case 1:
                 Delay_ms(1); // >10 us
                 // arrêt impulsion (etat bas)
                 LATB1_bit = 0;
                 etape++ ;
                 break;
            case 2:
            //Boucle jusqu'a avoir un echo
            // PORTB = 0000 0001

            if(PORTB & 0x01)
            {
                while(PORTB & 0x01)
                {
                        // incrementation de compte pour avoir le temps de l'echo (/10us)
                        // compte = temps t
                        compte++;
                        delay_us(10);
                }
                etape++;
            }
            break;
            case 3:
                 // convertion V =2d/t, on connait V et t
                nombreaffiche = (float)compte*10/58.82;
                Delay_ms(1);

                //Affichage de la distance
                afficheNombre((int)nombreaffiche);

                //convertion en chaine de char pour que la fonction Write puisse ecrire
                IntToStr((int)nombreaffiche, distChar);
                // ecriture de la distance
                UART1_Write_Text(distChar);
                //UART1_Write(13);
                //UART1_Write(10);
                Delay_ms(1000);

                etape=0;
                break;
            }
   }
}