#include <AFMotor.h>
#define R1 A1
// Define motores
AF_DCMotor motorLeft(1, MOTOR12_1KHZ ); 
AF_DCMotor motorRight(2, MOTOR12_1KHZ);
// Contantes para os sensores ultrassonicos
const byte trig[4] = {A4, 9, A3, 13};
const byte echo[4] = {A5, 10, A2, 2};

const int whiteIR = 900;
const byte speedMax = 255;
const byte speedSpin = 200;


// Define um array para os sensores Vinicius
byte sensor[4];

void setup() {
  Serial.begin(9600);

  Wait5seconds();  
  Impulse();  

  for (byte i = 0; i < 4; i += 1) {
    pinMode(trig[i], OUTPUT);
    pinMode(echo[i], INPUT);
  }

  while (true) {
    // Definindo contantes para os sensores de acordo com o for
    byte _FrontRight = 0; // 0 = Frente Direita
    byte _FrontLeft = 1; // 1 = Frente Esquerda
    byte _SideRight = 2; // 2 = Direita
    byte _SideLeft = 3; // 3 = Esquerda

    // Vefifica quanto está imprimindo na faixa branca
  	// Serial.print("Faixa Branca: ");
  	// Serial.println(analogRead(R1));
  	
    // Quando for faixa branca e não tiver inimigo a frente, dar Ré
    if ((PushEnemy(_FrontRight, _FrontLeft)) && (analogRead(R1) < whiteIR)) {
    	Forward(speedMax);
    	Serial.print("Continuar empurrando");
    } 

    // Verifica se é rampa ou faixa branca
    if ((PushEnemy(_FrontRight, _FrontLeft) == false) && (analogRead(R1) < whiteIR)){
      Serial.print("Faixa Branca Ré..");
      Backward();
      delay(500);
    }
    
    // Define as ações para o Robo
    byte action = -1;
    for (byte i = 0; i < 4; i++) {
      sensor[i] = ReadUltrasonic(trig[i], echo[i]);
      if (between(i, 2, 50))
      	action = i;
    }

    // Verifica se o inimigo está pertinho, frente esquerda ou frente direita
    if (betweenOr(_FrontRight, _FrontLeft, 2, 10))
      action = 4;

    // Verifica se os dois sensores da lateral está dentro do range, e define a ação para o mais perto
    if (betweenAnd(_SideRight, _SideLeft, 2, 50))  
      action = sensor[_SideRight] > sensor[_SideLeft] ? 3 : 2;

    // verifica se o inimigo está longe, frente esquerda ou frente direita
    if (betweenOr(_FrontRight, _FrontLeft, 50, 70))
      action = 5;

    // Define uma variavel para inverter a rotação do robo ao detectar na lateral
    volatile bool saveSpin;
    switch (action) {
      case 0:
        Forward(speedMax);
        Serial.print("Frente direita perto");
        Serial.println(sensor[_FrontRight]);
        break;
   
      case 1:
        Forward(speedMax);
        Serial.print("Frente Esquerda perto");
        Serial.println(sensor[_FrontLeft]);
        break;
   
      case 2:
        Clockwise(speedMax);
        saveSpin = true;
        Serial.print("Direita");
        Serial.println(sensor[_SideRight]);
        break;
   
      case 3:
        AntiClockwise(speedMax);
        saveSpin = false;
        Serial.print("Esquerda");
        Serial.println(sensor[_SideLeft]);
        break;

      case 4:
        Forward(speedMax);
        Serial.print("Empurrando inimigo");
        Serial.println(sensor[_FrontLeft]);
        break;

      case 5:
        Forward(speedMax);
        Serial.print("Inimigo a Frente");
        Serial.println(sensor[_FrontLeft]);
        break;

      default:
        if(saveSpin) {
          Clockwise(speedSpin);     
          Serial.println("Ultima vez eu vi ele a minha direita");
        }
     	  else {
          AntiClockwise(speedSpin); 
          Serial.println("Ultima vez eu vi ele a minha esquerda");
        }
    }
  }
}

// Verifica se o inimigo está pertinho
bool PushEnemy(byte i, byte j) {
  return betweenOr(i, j, 2, 10);
}

// verify if  x between
bool between(byte x, byte lower, byte upper) {
  if ((unsigned)(sensor[x]-lower) <= (upper-lower)) {
    return true;
  } else {
    return false;
  }
}

// verify if  x between &&  y between
bool betweenAnd(byte x, byte y, byte lower, byte upper) {
  if ((between(x, lower, upper)) && (between(y, lower, upper))) {    
    return true;
  } else {
    return false;
  }
}

// verify if  x between ||  y between
bool betweenOr(byte x, byte y, byte lower, byte upper) {
  if (between(x, lower, upper) || between(y, lower, upper)) {    
    return true;
  } else {
    return false;
  }
}