#include <LiquidCrystal.h>                                // Inclui a biblioteca do LCD 
const int rs = 4, en = 3, d4 = 6, d5 = 7, d6 = 8, d7 = 9; // Define os pinos utilizados para o LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                // Aplica definitivamente os pinos utilizados para o LCD
const int BUTTON = 0, RELAY = 1, SENSOR = 2, MOTOR = 5;   // Define os pinos utilizados nas entradas e saídas

// -------------------------------------------------------// Inicialização de variáveis de uso geral
int TempoInicial;                                         //
int TempoMedicao[10];                                     //
int Montagem[3];                                          //
int CONT = 0;                                             //
int ATUAL = 0;                                            //
int READ_BUTTON;                                          //
// -------------------------------------------------------//

void setup() {
  lcd.begin(16, 2);
  
  // -----------------------------------------------------// Definições dos pinos de entradas e saídas
  pinMode(RELAY, OUTPUT);                                 //
  pinMode(MOTOR, OUTPUT);                                 //
  pinMode(BUTTON, INPUT);                                 //
  pinMode(SENSOR, INPUT);                                 //
  // -----------------------------------------------------//

  attachInterrupt(digitalPinToInterrupt(SENSOR), Mudanca, CHANGE); // Configura a interrupção

  digitalWrite(RELAY, HIGH);                              // Ativa o eletroímã

  lcd.print("---INICIANDO!---");
  lcd.setCursor(0, 2);
  lcd.print("----------------");
  
  // -----------------------------------------------------// Curva de aceleração do motor
  for (int Rotacao = 0; Rotacao < 200; Rotacao++) {       //
    analogWrite(MOTOR, Rotacao);                          //
    delay(100);                                           //
  }                                                       //
  // -----------------------------------------------------//

  lcd.setCursor(7, 2);
  lcd.print("03");
  delay(1000);
  lcd.setCursor(7, 2);
  lcd.print("02");
  delay(1000);
  lcd.setCursor(7, 2);
  lcd.print("01");
  delay(1000);
  lcd.setCursor(7, 2);
  lcd.print("00");
  
  digitalWrite(RELAY, LOW);                               // Desativa o eletroímã
  TempoInicial = millis();                                // Armazena o instante inicial
}

void loop() {                                             // Loop principal

  READ_BUTTON = digitalRead(BUTTON);                      // Botão de uso geral (inserido para futuras implementações como, por exemplo, menus)

  if (CONT >= 8) {

    // ---------------------------------------------------// Curva de desaceleração do motor
    for (int Rotacao = 200; Rotacao > 0; Rotacao--) {     //
      analogWrite(MOTOR, Rotacao);                        //
      delay(20);                                          //
    }                                                     //
    analogWrite(MOTOR, 0);                                //
    // ---------------------------------------------------//

    // -------------------------------------------------------------// Apresenta os instantes mensurados no LCD
    lcd.clear();                                                    //
    int Row_index = 0;                                              //
    int Col_index = 0;                                              //
    for (int T_index = 0; T_index < 7; T_index += 2) {              //
      lcd.setCursor(Col_index, Row_index);                          //
      int Inteiro = (TempoMedicao[T_index] - TempoInicial) / 1000;  //
                                                                    //
      if ( Inteiro == 0 ) {                                         //
        lcd.print("0.");                                            //
        Decimal(TempoMedicao[T_index] - TempoInicial);              //
        lcd.print(" s");                                            //
      }                                                             //
      else {                                                        //
        lcd.print(Inteiro);                                         //
        lcd.print(".");                                             //
        Decimal(TempoMedicao[T_index] - TempoInicial);              //
        lcd.print(" s");                                            //
      }                                                             //
                                                                    //
      if (Row_index < 1) {                                          //
        Row_index++;                                                //
      }                                                             //
      else {                                                        //
        Row_index = 0;                                              //
        Col_index += 8;                                             //
      }                                                             //
                                                                    //
    }                                                               //
    // -------------------------------------------------------------//
  } // -------------------------------------------------------------// Equanto houverem medições a serem feitas, aguarda as interrupções
  else {                                                            // Aqui é onde há o armazenamento das medições
    if (ATUAL) {                                                    //
      TempoMedicao[CONT] = millis();                                //
      CONT++;                                                       //
      delay(50);                                                    //
      ATUAL = 0;                                                    //
    }                                                               //
  } // -------------------------------------------------------------//
}

// ---------------------------------------------------// Função utilizada quando há interrupção
void Mudanca() {                                      //
  ATUAL = 1;                                          //
}                                                     //
// ---------------------------------------------------//

// ---------------------------------------------------// Função utilizada para "montar" os instantes mensurados no LCD
void Decimal(int tempo) {                             //
  for (int i = 0; i < 3; i++) {                       //
    Montagem[i] = tempo % 10;                         //
    tempo = tempo / 10;                               //
  }                                                   //
  lcd.print(Montagem[2]);                             //
  lcd.print(Montagem[1]);                             //
  lcd.print(Montagem[0]);                             //
}                                                     //
// ---------------------------------------------------//
