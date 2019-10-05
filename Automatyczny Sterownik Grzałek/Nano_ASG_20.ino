// ----------------------------------------------------------------------------------------------------------------------------------------------

#include "DHT.h"                                                      //  Biblioteka DHT Sensor Library (Adafruit)

#define LED_R 13                                                      //  Czerwona dioda LED do pinu cyfrowego nr 11 (sygnalizacja pracy grzałki)
#define PWM_1 3                                                       //  Kanał PWM 1 na pinie 3
#define PWM_2 5                                                       //  Kanał PWM 2 na pinie 5
#define DHTPIN 4                                                      //  Czujnik DHT podłaczony do pinu cyfrowego nr 4
#define DHTTYPE DHT22                                                 //  Rodzaj czujnika DHT22

// Konfiguracja wartości dla PWM
// Wypełnienie    10%   30%   50%   70%   100%
// Wartość PWM    25    77    128   180   255

// Próg 1 dla t-d między 4°C ÷ 3°C

int PWR_1_1 = 25;     //  PWM_1 10%
int PWR_2_1 = 25;     //  PWM_2 10%

// Próg 2 dla t-d między 3°C ÷ 2°C

int PWR_1_2 = 77;     //  PWM_1 30%
int PWR_2_2 = 77;     //  PWM_2 30%

// Próg 3 dla t-d między 2°C ÷ 1°C

int PWR_1_3 = 128;    //  PWM_1 50%
int PWR_2_3 = 128;    //  PWM_2 50%

// Próg 4 dla t-d poniżej 1°C

int PWR_1_4 = 180;    //  PWM_1 70%
int PWR_2_4 = 180;    //  PWM_2 70%

DHT dht(DHTPIN, DHTTYPE);                                             //  Utworzenie obiektu dht

void setup() {
  Serial.begin(9600);                                                 //  Włączenie komunikacji szeregowej - ustawienie prędkości transmisji

  pinMode(LED_R, OUTPUT);                                             //  Konfiguracja pinu cyfrowego nr 11 jako wyjście
  pinMode(PWM_1, OUTPUT);                                             //  Konfiguracja pinu cyfrowego nr 3 jako wyjście
  pinMode(PWM_2, OUTPUT);                                             //  Konfiguracja pinu cyfrowego nr 5 jako wyjście

  dht.begin();                                                        //  Włączenie i inicjalizacja komunikacji z czujnikiem DHT
  delay(5000);                                                        //  Odczekaj 500 ms
}

void loop() {

  float h = dht.readHumidity();                                       //  Odczyt wilgotności h - typ zmiennej float
  float t = dht.readTemperature();                                    //  Odczyt temperatury t - typ zmiennej float

  double d = dewPoint(t, h);

  // Wysłanie na port COM danych o temperaturze (t), wilgotności (h) i temeraturze punktu rosy (d)
  Serial.println(String(h) + ',' + String(t) + ',' + String(d) + ',' + String(t-d));

  if ((t - d <= 4) && (t - d >= 3)) {                               //  Jeżeli różnica t-td jest mniejsza lub równa 4 i większa lub równa 3
    digitalWrite(LED_R, HIGH);                                      
    analogWrite(PWM_1, PWR_1_1);
    analogWrite(PWM_2, PWR_2_1);
  }
  else if ((t - d < 3) && (t - d >= 2)) {                           //  Jeżeli różnica t-td jest mniejsza 3 i większa lub równa 2
    digitalWrite(LED_R, HIGH);
    analogWrite(PWM_1, PWR_1_2);
    analogWrite(PWM_2, PWR_2_2);
  }
  else if ((t - d < 2) && (t - d > 1)) {                            //  Jeżeli różnica t-td jest mniejsza 2 i większa 1
    digitalWrite(LED_R, HIGH);
    analogWrite(PWM_1, PWR_1_3);
    analogWrite(PWM_2, PWR_2_3);
  }
  else if (t - d <= 1) {                                             //  Jeżeli różnica t-td jest mniejsza lub równa 1
    digitalWrite(LED_R, HIGH);
    analogWrite(PWM_1, PWR_1_4);
    analogWrite(PWM_2, PWR_2_4);
  }
  else {
    digitalWrite(LED_R, LOW);                                        //  Wyłącz czerwoną diodę LED - grzałka nie pracuje
    analogWrite(PWM_1, 0);                                           //  PWM  - wypełnienie 0%
    analogWrite(PWM_2, 0);                                           //  PWM  - wypełnienie 0%
  }

}
