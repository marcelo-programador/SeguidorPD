#define MOT1 3   // Motor esquerdo (PWM)
#define MOT2 5   // Motor direito (PWM)

#define SEN_E 11 // Sensor esquerdo
#define SEN_C 9  // Sensor central
#define SEN_D 10 // Sensor direito

// #define DEBUG  // Ative para serial debug

// Ajustes PID para maior controle em curvas
int Kp = 35;   // Proporcional maior para correção mais forte
int Kd = 25;   // Derivativo para suavizar

int baseSpeed = 100;  // Velocidade reduzida para melhor controle

float erro = 0.0;
float erroAnterior = 0.0;
float derivada = 0.0;
float correcao = 0.0;

void setup() {
  pinMode(MOT1, OUTPUT);
  pinMode(MOT2, OUTPUT);

  pinMode(SEN_E, INPUT);
  pinMode(SEN_C, INPUT);
  pinMode(SEN_D, INPUT);

#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  bool sE = digitalRead(SEN_D);
  bool sC = digitalRead(SEN_C);
  bool sD = digitalRead(SEN_E);

  // Usando pesos maiores para sensores laterais
  int somaPesos = 0;
  int somaLeituras = 0;

  if (sE) { somaPesos += -2.5; somaLeituras++; }
  if (sC) { somaPesos += 0; somaLeituras++; }
  if (sD) { somaPesos += 3; somaLeituras++; }

  if (somaLeituras > 0)
    erro = (float)somaPesos / somaLeituras;
  else
    erro = erroAnterior;  // mantém erro anterior se perdeu linha

  derivada = erro - erroAnterior;
  correcao = Kp * erro + Kd * derivada;
  erroAnterior = erro;

  int velEsquerdo = baseSpeed - correcao;
  int velDireito  = baseSpeed + correcao;

  velEsquerdo = constrain(velEsquerdo, 0, 255);
  velDireito  = constrain(velDireito, 0, 255);

  analogWrite(MOT1, velEsquerdo);
  analogWrite(MOT2, velDireito);

#ifdef DEBUG
  Serial.print("S:E");
  Serial.print(sE);
  Serial.print(" C:");
  Serial.print(sC);
  Serial.print(" D:");
  Serial.print(sD);
  Serial.print(" | Erro:");
  Serial.print(erro);
  Serial.print(" | VelE:");
  Serial.print(velEsquerdo);
  Serial.print(" | VelD:");
  Serial.println(velDireito);
#endif
}