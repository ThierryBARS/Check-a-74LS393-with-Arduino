/*
  Programme permettant de tester une mémoire AM93422DC

               vue de dessus
           _________
          |   |_|   | 
    A3   =|         |= VCC
    A2   =|         |= A4
    A1   =|         |= /WE
    A0   =|         |= /CS1
    A5   =|         |= /OE
    A6   =|         |= CS2
    A7   =|         |= O3
    GND  =|         |= D3
    D0   =|         |= O2
    O0   =|         |= D2
    D1   =|         |= O1
          |____ ____|

Cablage:
VCC    = 5V
Ground = 0v 


D 0 - D 7 = Digital I/O 0a 7 qui seront en OUTPUT Arduino

D 10 = /WE
D 11 = /CS1
D 12 = /OE
D 13 = CS2

Ai0 - Ai3 = Analog Input seront en Output Arduino

Les sortie O0 à O3 seront visualisé sur la planche a pain
*/



//---------------Sorties Numerique Arduino----------------------
const int R0 = 0;
const int R1 = 1;
const int R2 = 2;
const int R3 = 3;
const int R4 = 4;
const int C0 = 5;
const int C1 = 6;
const int C2 = 7;

const int _WE  = 10;
const int _CS1 = 11;
const int _OE  = 12;
const int CS2  = 13;

//---------------Sorties Analogique Arduino----------------------
const int D0 = 14;
const int D1 = 15;
const int D2 = 16;
const int D3 = 17;

const int UN   = 255;
const int ZERO = 0;

boolean PremierTour;
  //LectureCompteur=(((PINB)&3)<<6)|((PIND&252)>>2);

void DeselectionneMemoire(){
  digitalWrite(_CS1, HIGH) ;
  digitalWrite(CS2, LOW) ;
}
void SelectionneMemoire(){
  digitalWrite(_CS1, LOW) ;
  digitalWrite(CS2, HIGH) ;
}
//-------------------------------------
void Present_WE_a_ZERO(){
 digitalWrite(_WE, LOW);
}
void Present_WE_a_UN(){
 digitalWrite(_WE, HIGH);
}
//-------------------------------------
void Present_CS1_a_ZERO(){
 digitalWrite(_CS1, LOW);
}
void Present_CS1_a_UN(){
 digitalWrite(_CS1, HIGH);
}
//-------------------------------------
void Present_CS2_a_ZERO(){
 digitalWrite(CS2, LOW);
}
void Present_CS2_a_UN(){
 digitalWrite(CS2, HIGH);
}
//-------------------------------------
void Present_OE_a_ZERO(){
 digitalWrite(_OE, LOW);
}
void Present_OE_a_UN(){
 digitalWrite(_OE, HIGH);
}


void PesentData(int bitD3, int bitD2, int bitD1, int bitD0){ //Zero 0 / Un 255
 analogWrite(D0, bitD0);
 analogWrite(D1, bitD1);
 analogWrite(D2, bitD2);
 analogWrite(D3, bitD3);
}

void PesentAdresse(int c2, int c1, int c0, int r4, int r3, int r2, int r1, int r0){
 digitalWrite(R0, r0);
 digitalWrite(R1, r1);
 digitalWrite(R2, r2);
 digitalWrite(R3, r3);
 digitalWrite(R4, r4);
 digitalWrite(C0, c0);
 digitalWrite(C1, c1);
 digitalWrite(C2, c2); 
}


void Lit(int c2,int c1, int c0, int r4, int r3, int r2, int r1, int r0){
  Present_OE_a_UN();
  Present_WE_a_UN();
  SelectionneMemoire(); //CS1=0   CS2=1
  PesentAdresse(c2,c1,c0,r4,r3,r2,r1,r0);
  Present_OE_a_ZERO();
}

void Ecrit(int c2, int c1, int c0, int r4, int r3, int r2, int r1, int r0,int d3,int d2,int d1,int d0){
  Present_WE_a_UN();
  Present_OE_a_UN();
  SelectionneMemoire();
  PesentAdresse(c2,c1,c0,r4,r3,r2,r1,r0);
  PesentData(d3,d2,d1,d0);
  Present_WE_a_ZERO();delay(10);
  Present_WE_a_UN();
  Present_OE_a_ZERO();
}

void setup() {

// DE D10 A D13
  pinMode(_CS1, OUTPUT);
  pinMode(CS2, OUTPUT);
  DeselectionneMemoire();
  
  pinMode(_WE, OUTPUT);
  Present_WE_a_UN();
  
  pinMode(_OE, OUTPUT);
  Present_OE_a_UN();
  
  //Row decoder
  pinMode(R0, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  //column  decoder
  pinMode(C0, OUTPUT);
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  
  delay(20);
  PremierTour=true;
}


void efface_tout(){

  for (int c2 = 0; c2 <= 1; c2++) {
    for (int c1 = 0; c1 <= 1; c1++) {
      for (int c0 = 0; c0 <= 1; c0++) {
        for (int r4 = 0; r4 <= 1; r4++) {
          for (int r3 = 0; r3 <= 1; r3++) {
            for (int r2 = 0; r2 <= 1; r2++) {
              for (int r1 = 0; r1 <= 1; r1++) {
                for (int r0 = 0; r0 <= 1; r0++) {
                  Ecrit(c2,c1, c0,  r4,  r3,  r2,  r1,  r0, ZERO,ZERO,ZERO,ZERO);
                }
              }
            }
          }                        
        }
      }
    }
  }        
} 



void loop() {

  if (PremierTour==true){
   // efface_tout();
   
    Ecrit(0,0,0,0,0,0,0,0,  UN,  UN,  UN,  UN);
    Ecrit(0,0,0,0,0,0,0,1,  UN,  UN,  UN,ZERO);
    Ecrit(0,0,0,0,0,0,1,0,  UN,  UN,ZERO,  UN);
    Ecrit(0,0,0,0,0,0,1,1,  UN,  UN,ZERO,ZERO);
    Ecrit(0,0,0,0,0,1,0,0,  UN,ZERO,  UN,  UN);
    Ecrit(0,0,0,0,0,1,0,1,  UN,ZERO,  UN,ZERO);
    Ecrit(0,0,0,0,0,1,1,0,  UN,ZERO,ZERO,  UN);
    Ecrit(0,0,0,0,0,1,1,1,  UN,ZERO,ZERO,ZERO);
    Ecrit(0,0,0,0,1,0,0,0,ZERO,  UN,  UN,  UN);
    Ecrit(0,0,0,0,1,0,0,1,ZERO,  UN,  UN,ZERO);
    Ecrit(0,0,0,0,1,0,1,0,ZERO,  UN,ZERO,  UN);
    Ecrit(0,0,0,0,1,0,1,1,ZERO,  UN,ZERO,ZERO);
    Ecrit(0,0,0,0,1,1,0,0,ZERO,ZERO,  UN,  UN);
    Ecrit(0,0,0,0,1,1,0,1,ZERO,ZERO,  UN,ZERO);
    Ecrit(0,0,0,0,1,1,1,0,ZERO,ZERO,ZERO,  UN);
    Ecrit(0,0,0,0,1,1,1,1,ZERO,ZERO,ZERO,ZERO);
    
 /*   Ecrit(0,0,0,0,1,0,0,0,  UN,  UN,  UN,  UN);
    Ecrit(0,0,0,0,1,0,0,1,  UN,  UN,  UN,ZERO);
    Ecrit(0,0,0,0,1,0,1,0,  UN,  UN,ZERO,  UN);
    Ecrit(0,0,0,0,1,0,1,1,  UN,  UN,ZERO,ZERO);
    Ecrit(0,0,0,0,1,1,0,0,  UN,ZERO,  UN,  UN);
    Ecrit(0,0,0,0,1,1,0,1,  UN,ZERO,  UN,ZERO);
    Ecrit(0,0,0,0,1,1,1,0,  UN,ZERO,ZERO,  UN);
    Ecrit(0,0,0,0,1,1,1,1,  UN,ZERO,ZERO,ZERO);
    Ecrit(0,0,0,0,1,0,0,0,ZERO,  UN,  UN,  UN);
    Ecrit(0,0,0,0,1,0,0,1,ZERO,  UN,  UN,ZERO);
    Ecrit(0,0,0,0,1,0,1,0,ZERO,  UN,ZERO,  UN);
    Ecrit(0,0,0,0,1,0,1,1,ZERO,  UN,ZERO,ZERO);
    Ecrit(0,0,0,0,1,1,0,0,ZERO,ZERO,  UN,  UN);
    Ecrit(0,0,0,0,1,1,0,1,ZERO,ZERO,  UN,ZERO);
    Ecrit(0,0,0,0,1,1,1,0,ZERO,ZERO,ZERO,  UN);
    Ecrit(0,0,0,0,1,1,1,1,ZERO,ZERO,ZERO,ZERO);*/
    
    PremierTour=false;
  }

      
      Lit(0,0,0,0,0,0,0,0); delay(200);
      Lit(0,0,0,0,0,0,0,1); delay(200);
      Lit(0,0,0,0,0,0,1,0); delay(200);
      Lit(0,0,0,0,0,0,1,1); delay(200);
      Lit(0,0,0,0,0,1,0,0); delay(200);
      Lit(0,0,0,0,0,1,0,1); delay(200);
      Lit(0,0,0,0,0,1,1,0); delay(200);
      Lit(0,0,0,0,0,1,1,1); delay(200);
      Lit(0,0,0,0,1,0,0,0); delay(200);
      Lit(0,0,0,0,1,0,0,1); delay(200);
      Lit(0,0,0,0,1,0,1,0); delay(200);
      Lit(0,0,0,0,1,0,1,1); delay(200);
      Lit(0,0,0,0,1,1,0,0); delay(200);
      Lit(0,0,0,0,1,1,0,1); delay(200);
      Lit(0,0,0,0,1,1,1,0); delay(200);
      Lit(0,0,0,0,1,1,1,1); delay(200);

     

}
