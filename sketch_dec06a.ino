 #include <Servo.h>

// Pinii motor 1
#define mpin00 5
#define mpin01 6

// Pinii motor 2
#define mpin10 3
#define mpin11 11

//Pini senzor
#define trigPin 9
#define echoPin 10

Servo srv;
int pozitie = 90;
long distanta ;
long distantaStanga ;
long distantaDreapta;


void setup() {
  Serial.begin(9600);

  //trimite unde sonore
  pinMode(trigPin, OUTPUT);
  //primeste unde
  pinMode(echoPin, INPUT);
 
  //pinul servo si poz
   srv.attach(8);
   srv.write(pozitie);

  
 // configurarea pinilor motor ca iesire, initial valoare 0
 digitalWrite(mpin00, 0);
 digitalWrite(mpin01, 0);
 digitalWrite(mpin10, 0);
 digitalWrite(mpin11, 0);
 pinMode (mpin00, OUTPUT);
 pinMode (mpin01, OUTPUT);
 pinMode (mpin10, OUTPUT);
 pinMode (mpin11, OUTPUT);

 
 // pin LED

}

//Functie pentru scanare si convertire a distantei
long getDistanta()
{
  long durata,distanta; 
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigPin, LOW);
  
  //numarul de microsecunde in care echo merge la HIGH
  //HIGH -> LOW -> HIGH
  durata = pulseIn(echoPin, HIGH);
  distanta = durata*0.034/2; //transformare in centimetri /2 /29.1
  Serial.println(distanta);
  return distanta;
} 
// Functie pentru controlul unui motor
// Intrare: pinii m1 si m2, directia si viteza
void StartMotor (int m1, int m2, int forward, int speed)
{
   // oprire
 if (speed==0) {
   digitalWrite(m1, 0); 
   digitalWrite(m2, 0);
   
    }else{ if (forward){
           digitalWrite(m2, 0);
           analogWrite (m1, speed); // folosire PWM
        }else{
            digitalWrite(m1, 0);
            analogWrite(m2, speed);
              }
       }
}


// Functie de siguranta
// Executa oprire motoare, urmata de delay
void delayStopped(int ms)
{
  StartMotor (mpin00, mpin01, 0, 0);
  StartMotor (mpin10, mpin11, 0, 0);
  delay(ms);
}


long verificaDreapta()
{int i;
  long distanta=0,minim=3200;
  for ( i=90; i>=0; i-=15)
  {
    srv.write(i);
    delay(100);
    distanta=getDistanta();
    if(distanta<minim){minim=distanta;}
    
  }

  return minim;
  
}
long verificaStanga()
{int i;
  long distanta=0,minim=3200;
  for ( i=90; i<=180; i+=15)
  {
    srv.write(i);
    delay(100);
    distanta=getDistanta();
    if(distanta<minim){minim=distanta;}
    
  }

  return minim;
  
}




void loop() { 
    StartMotor (mpin00, mpin01, 0, 0);
    StartMotor (mpin10, mpin11, 0, 0);
   
  srv.write(90);  
  delay(30);
   
    distanta= getDistanta();
    Serial.println(distanta);
  if (distanta > 25){
    //mergem inainte
   StartMotor (mpin00, mpin01, 1, 128);
   StartMotor (mpin10, mpin11, 1, 128);
   
   delay(500);

    
    
  }else {
    //stop
    StartMotor (mpin00, mpin01, 0, 0);
    StartMotor (mpin10, mpin11, 0, 0);
      delay(500);
    distantaStanga=verificaStanga();
    distantaDreapta=verificaDreapta();
   Serial.println(distantaStanga);
    Serial.println(distantaDreapta);
     delay(1000);
    if(distantaStanga>= distantaDreapta && distantaStanga>10 ){//stanga
      //merge stanga
        Serial.println("stanga");
           StartMotor (mpin00, mpin01, 1, 100);
            StartMotor (mpin10, mpin11, 1, 30);
           
       delay(1000);
       //orpit+ mers inainte
    }else if(distantaStanga < distantaDreapta && distantaDreapta>10){ //merge dreapta
     Serial.println("Dreapta");

             StartMotor (mpin00, mpin01, 1, 30); 
            StartMotor (mpin10, mpin11, 1, 100);
           
             delay(1000);
    }else if (distantaStanga < 10 && distantaDreapta <10)
          {//inapoi
            Serial.println("inapi");
             StartMotor (mpin00, mpin01, 0, 100);
              StartMotor (mpin10, mpin11, 0, 100);
               delay(1000); 
          }
  
  }
 
} 
