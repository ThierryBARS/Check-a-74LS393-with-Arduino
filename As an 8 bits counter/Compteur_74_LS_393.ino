/*
  Programme permettant de tester un compteur SN 74 LS 393 N

               vue de dessus
                 _________
                |   |_|   | 
            1A =|         |= VCC
         1 CLR =|    7    |= 2A
           1QA =|    4    |= 2 CLR
           1QB =|   LS    |= 2QA
           1QC =|    3    |= 2QB
           1QD =|    9    |= 2QC
        Ground =|    3    |= 2QD
                |____N____|

Cablage:
VCC    = 5V
Ground = 0v 

1A     = Digital I/O 13 qui sera en OUTPUT (elle fournira 
les impulsions a compter). Dans le soft "A_Compter"

1 CLR et 2 CLR seront reliées ensemble et reliées à
la sortie Digital I/O 12, et portera le nom "CLR_compteur"

1QD sera reliée sur 2A (pourtransformer les 2 compteurs 4bs
en 1 compteur 8 bits

1QA, 1QB, 1QC, 1QD, 2QA, 2QB, 2QC, 2QD seront respectivement
reliées aux Digitals I/O: 
2, 3, 4, 5, 6, 7.
Je lai pas commencé par  digital I/O 0 et 1 car elles sont 
utilisées par l'instruction serial en cas d'erreur de comptage
Il faut donc ouvrir le "moniteur serie" pour voir les erreurs

*/

//---------------Sorties------------------------------------
const int A_Compter =  13; // broche envoyant les impulsions a comp
                           // ter sur 1A du compteur.
const int CLR_compteur =  12; // broche ou le clear est branché 

//---------------Entrées------------------------------------
const int b0 =2;
const int b1 =3;
const int b2 =4;
const int b3 =5;
const int b4 =6;
const int b5 =7;
const int b6 =8;
const int b7 =9;
const int BtnStartReset =  11; // broche du bouton reset test 


//---------------Variables du programme--------------------------------
byte CompteurLogiciel; // Compteur logiciel du nombre d'impulsions 
                      // envoyées sur 1A via Digital I/O 13
byte LectureCompteur;  // Lecture des sorties 1Qx et 2Qx via les ports
                      // PINB et PIND  (car les 8 broches s'étalent sur 
                      // ces 2 ports du fait de ne pas utiliser Digital 
                      // I/O 0 et 1 


void remise_a_zero_Compteur(){
  digitalWrite(A_Compter, HIGH) ;
  digitalWrite(A_Compter, LOW) ; 
  CompteurLogiciel=0;
  }

void incrementeCompteur(){
  digitalWrite(A_Compter, LOW) ; 
  digitalWrite(A_Compter, HIGH) ;
  CompteurLogiciel += 1;
}


void setup() {
  Serial.begin(9600);

  pinMode(A_Compter, OUTPUT);
  pinMode(CLR_compteur, OUTPUT);
  
  pinMode(b0, INPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  pinMode(b5, INPUT);
  pinMode(b6, INPUT);
  pinMode(b7, INPUT);
  pinMode(BtnStartReset, INPUT);
  
  remise_a_zero_Compteur();
}

void loop() {

    if (digitalRead(BtnStartReset) == 0) {

      incrementeCompteur();
    
      //lit, sous forme d'un octet, les ports PINB et PIND, n'ayant pas 
      //utilisé les DigitalIO 0 et 1, j'empiete sur le PINB ET PIND 
      LectureCompteur=(((PINB)&3)<<6)|((PIND&252)>>2);

      //Ci dessous une méthode plus compréhensible
      //LectureCompteur=digitalRead(b7)*128+digitalRead(b6)*64+digitalRead(b5)*32+digitalRead(b4)*16+digitalRead(b3)*8+digitalRead(b2)*4+digitalRead(b1)*2+digitalRead(b0);
    
      if (CompteurLogiciel != LectureCompteur) {
          Serial.print("On a un GROS probleme de comptage => ");
          Serial.print(CompteurLogiciel,HEX); Serial.print(" < - > "); Serial.println(LectureCompteur,HEX);
          delay(500);
      }
    
    }
     else if (digitalRead(BtnStartReset) == 1) {
      Serial.println("Remise a 0");
      remise_a_zero_Compteur;
      LectureCompteur=0;
      delay(500);
     }
    
      
}
