/*
  Programme permettant de tester un compteur TLC 1541 convertisseur analog 2 digital 
  11 entrees

               vue de dessus
                 _________
                |   |_|   | 
      Input A0 =|         |= VCC
      Input A1 =|    T    |= System clock
      Input A2 =|    L    |= I/O Clock
      Input A3 =|    S    |= Adress Input
      Input A4 =|    1    |= Data Out
      Input A5 =|    5    |= /CS
      Input A6 =|    5    |= Ref +
      Input A7 =|    5    |= Ref -
      Input A8 =|    5    |= Input A10
        Ground =|    3    |= Input A9
                |____N____|

*/

//---------------Sorties------------------------------------
int CK_System_D2 = 2;
int CK_IO_D3 = 3;
int ADR_Input_D4 = 4;
int CS_D5 = 5;

//---------------Entrees------------------------------------
int DataOUT_D6 =  6;

/*  B (digital pin 8 to 13)
    C (analog input pins)
    D (digital pins 0 to 7) 
   LectureCompteur=(((PINB)&3)<<6)|((PIND&252)>>2);   */

int Tensions[12];
byte adr; //adresses des tensions
byte bi; // bit dea l'adresse a transmettre


void CkSytemX3(int n){

  for(int i=1;i<=n;i++) {
      digitalWrite(CK_System_D2, HIGH) ; delay(1);
      digitalWrite(CK_System_D2, LOW) ;delay(1);
   }  
}

void CycleComplet() {
 
  for ( int ad=0;ad<=11;ad++) {
    //la difficulté est que le premier element retourné est le precedament traité
    adr=ad-1; // rang dans le tableaux de tensions mesurées
    if (ad==0){
      adr=11;
    }
       
    Tensions[adr]=0;
    
    digitalWrite(CS_D5, LOW) ;
   
     
    CkSytemX3(3); //stabilisation
 
    //presentation de l'adresse

   
    bi= ad & (1 << 3);
    digitalWrite(ADR_Input_D4, bi) ;
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+512*bi; //bit9
    digitalWrite(CK_IO_D3, LOW);
   
    bi= ad & (1 << 2);
    digitalWrite(ADR_Input_D4, bi) ;
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+256*bi; //bit8
    digitalWrite(CK_IO_D3, LOW);
  
    bi= ad & (1 << 1);
    digitalWrite(ADR_Input_D4, bi) ;
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+128*bi; //bit7
    digitalWrite(CK_IO_D3, LOW);
   
    bi= ad & (1 << 0);
    digitalWrite(ADR_Input_D4, bi) ;
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+64*bi; //bit6
    digitalWrite(CK_IO_D3, LOW);

    

    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+32*bi; //bit5
    digitalWrite(CK_IO_D3, LOW);
    
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+16*bi; //bit4
    digitalWrite(CK_IO_D3, LOW);
    
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+8*bi; //bit3
    digitalWrite(CK_IO_D3, LOW);
    
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+4*bi; //bit2
    digitalWrite(CK_IO_D3, LOW);
    
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+2*bi; //bit1
    digitalWrite(CK_IO_D3, LOW);
    
    digitalWrite(CK_IO_D3, HIGH);
    bi=digitalRead(DataOUT_D6);
    Tensions[adr]=Tensions[adr]+1*bi; //bit0
    digitalWrite(CK_IO_D3, LOW);
    
      
    digitalWrite(CS_D5, HIGH) ;
    digitalWrite(CK_System_D2, LOW) ;
    digitalWrite(CK_IO_D3, LOW) ;
    digitalWrite(ADR_Input_D4, LOW) ; 

    CkSytemX3(44); //stabilisation
  
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(CK_System_D2, OUTPUT);
  pinMode(CK_IO_D3, OUTPUT);
  pinMode(ADR_Input_D4, OUTPUT);
  pinMode(CS_D5, OUTPUT);
  
  
  pinMode(DataOUT_D6, INPUT);
  delay(20);

  //Desactive le composant
  digitalWrite(CS_D5, HIGH) ;
  digitalWrite(CK_System_D2, LOW) ;
  digitalWrite(CK_IO_D3, LOW) ;
  digitalWrite(ADR_Input_D4, LOW) ;  
}



void loop() {
// apres un _CS a 0 attendre 3 system clock
// Apres la presentation d'adresse il faut attendre 44  system clock de convertion
// la lecture se fait pendant la presentaion de la prochaine adresse a convertir
  
// Lecture d'un bit en particulier
// A & (1 << n)

// Assignation de la valeur 1 à un bit en particulier
// A = A | (1 << n);

// Assignation de la valeur 0 à un bit en particulier
// A = A & ~ ( 1 << n);

// Inversion d'un bit en particulier
// A = A ^ (1 << n); inversion du nieme bit en conservqnt les autres inchangés

  CycleComplet();
 
  for (adr=0;adr<=11;adr++) {
    Serial.print("Tension de ("); Serial.print(adr); Serial.print(") :");
    Serial.println(Tensions[adr]);
    delay(100);
  }
  Serial.println("---------------------------");
}
