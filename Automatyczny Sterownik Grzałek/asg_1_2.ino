//  Automatyczny sterownik grzałek
//
//  Funkcje:
//    - Pomiar temperatury
//    - Pomiar wilgotności
//    - Obliczenie punktu rosy
//    - Sterowanie grzałką teleskopu - włącz / wyłącz
//    - Sygnalizacja błędu odczytu z czujnika DHT
//
//  Biblioteki:
//    - Adafruit Unified Sensor (Adafruit)
//    - DHT Sensor Library (Adafruit)
//
//  Uwagi:
//    - Program demonstruje działanie automatycznego sterownika grzałek.
//    - Czerwona dioda LED sygnalizuje (symuluje) włączenie grzałki
//    - Zielona dioda LED sygnalizuje brak roszenia
//    - Program nie ma zdefiniowanego pinu / pinów do sterowania grzałką / grzałkami
//    - Komunikacja szeregowa (COM) służy jako "monitor" działania programu - docelowo może być wyłączona w celu oszczędności pamięci programu
//    - Rozwiązanie programowe absolutnie nie jedynie słuszne
//    - Za uszkodzenia wynikające z działania programu nie odpowiadam, program stworzony został jako przykład dla urządzenia modelowego (bez grzałki)
//
//  Autor:
//    Radosław Deska
//
//  Wersja:
//    - 1.2 (18.05.2019)
//
//  Poprawki do wersji 1.2
//    - poprawiono komentarze
//    - zmieniony został blok związany z obsługą grzałki, inna budowa warunku
//    - dodanie pinu obsługującego grzałkę (pin cyfrowy nr 6)
//    - dodanie funkcji PWM dla pinu nr 6
//    - zmienione zostały typy zmiennych t, h, td z float na int
//    - zmieniono warunek braku komunikacji/danych z czujnika DHT
//
//  Poprawki do wersji 1.1
//    - zmieniono warunek kiedy ma zostać włączona grzałka
//    - dodano komunikat wysyłany na port COM o włączonej / wyłączonej grzałce (ON/OFF)
//
// -------------------------------------------------------------------------------------------------------------------------------------------------

#include "DHT.h"                                                      //  Biblioteka DHT

#define LED_R_PIN 3                                                   //  Czerwona dioda LED do pinu cyfrowego nr 3 (sygnalizacja pracy grzałki)
#define LED_G_PIN 4                                                   //  Zielona dioda LED do pinu cyfrowego nr 4 (sygnalizacja braku rosy)

#define HEATER 6                                                      // Pin sterujący pracą grzałki (pin cyfrowy nr 6, PWM)

#define DHTPIN 5                                                      //  Czujnik DHT podłaczony do pinu cyfrowego nr 5
#define DHTTYPE DHT22                                                 //  Rodzaj czujnika DHT22

DHT dht(DHTPIN, DHTTYPE);


// Funkcja obliczająca temperature punktu rosy na podstawie pomiarów temperatury i wilgotności - wynik typu float

float pktRosy(float humi, float temp) {
  float k;
  k = log(humi / 100) + (17.62 * temp) / (243.12 + temp);
  return 243.12 * k / (17.62 - k);
}


void setup() {

  Serial.begin(9600);                                                 //  Włączenie komunikacji szeregowej - ustawienie prędkości transmisji
  Serial.println("Automatyczny sterownik grzałek \t Wersja 1.2 \n");  //  Tekst na powitanie

  pinMode(LED_R_PIN, OUTPUT);                                         //  Konfiguracja pinu cyfrowego nr 3 jako wyjście
  pinMode(LED_G_PIN, OUTPUT);                                         //  Konfiguracja pinu cyfrowego nr 4 jako wyjście

  pinMode(HEATER, OUTPUT);                                            //  Konfiguracja pinu cyfrowego nr 6 jako wyjście

  dht.begin();                                                        //  Włączenie i inicjalizacja komunikacji z czujnikiem DHT

}

void loop() {

  delay(500);                                                         //  Odczekaj 500 ms

  int h = dht.readHumidity();                                         //  Odczyt wilgotności h - typ zmiennej int
  int t = dht.readTemperature();                                      //  Odczyt temperatury t - typ zmiennej int

  if (h == 0 || t == 0) {
    Serial.println("Błąd odczytu z czujnika DHT!");                   //  Wyślij komunikat o błedzie
    digitalWrite(LED_R_PIN, HIGH);                                    //  Włącz czerwoną diode LED
    delay(150);                                                       //  Odczekaj 150ms
    digitalWrite(LED_R_PIN, LOW);                                     //  Wyłącz czerwoną diode LED
    return;
  }

  //  Wywołanie funkcji pktRosy z wartościami h i t, przypisanie wyniku do zmiennej td (int)
 
  int td = pktRosy(h, t);

  // Wysłanie na port COM danych o temperaturze (t), wilgotności (h) i temeraturze punktu rosy (td)

  Serial.print("Wilgotność: ");                                       //  Wyślij tekst
  Serial.print(h);                                                    //  Wartość wilgotności - zmienna h
  Serial.print("°%");                                                 //  Wyślij tekst
  Serial.print("\t");                                                 //  Znak tabulatora
  Serial.print("Temperatura: ");                                      //  Wyślij tekst
  Serial.print(t);                                                    //  Wartość temperatury - zmienna t
  Serial.print("°C");                                                 //  Wyślij tekst
  Serial.print("\t");                                                 //  Znak tabulatora
  Serial.print(" Punkt rosy: ");                                      //  Wyślij tekst
  Serial.print(td);                                                   //  Wartość punktu rosy - zmienna td
  Serial.print("°C");                                                 //  Wyślij tekst
  Serial.print("\t");                                                 //  Znak tabulatora
  Serial.print("t-td = ");                                            //  Wyślij tekst
  Serial.print(t - td);                                               //  Oblicz różnicę między zmienną t i zmienną td i wyświetl
  Serial.print("\t");                                                 //  Znak tabulatora

  // Sprawdzamy na podstawie wyliczonego punktu rosy (td) i mierzonej temperatury z czujnika DHT czy należy włączyć grzałkę
  // Jeżeli td > t = uruchamiamy grzałkę, w przeciwnym razie td < t = wyłączamy grzałkę
  // W układzie modelowym czerwona dioda LED sygnalizuje pracę grzałki, zielona dioda LED brak rosy.
  // Jeżeli spełniony jest warunek td > t wówczas zaświeci się czerwona dioda LED (grzałka pracuje), zielona dioda LED zgaśnie (rosa)

  // !!! UWAGA !!! Do pinu nr 6 podłączona jest dioda LED wraz z rezystorem 220Ohm jako symulator grzałki

  if ((t - td <= 7) && (t - td >= 5)) {                               //  Jeżeli różnica t-td jest mniejsza lub równa 7 i większa lub równa 5
    digitalWrite(LED_R_PIN, HIGH);                                    //  Włącz czerwoną diodę LED - sygnalizacja pracy grzałki
    digitalWrite(LED_G_PIN, LOW);                                     //  Wyłącz zieloną diodę LED - rosa
    Serial.print("ON\t");                                             //  Wyślij tekst - ON i dodaj tabulator

    analogWrite(HEATER, 2);                                           //  PWM  - wypełnienie 2 (grzałkę symuluje dioda LED)
    Serial.print("PWM 2, 7 - 5");                                     //  Wyślij tekst o wypełnieniu PWM i zakresie

  }
  else if ((t - td < 5) && (t - td >= 3)) {                           //  Jeżeli różnica t-td jest mniejsza 5 i większa lub równa 3
    digitalWrite(LED_R_PIN, HIGH);                                    //  Włącz czerwoną diodę LED - grzałka pracuje
    digitalWrite(LED_G_PIN, LOW);                                     //  Wyłącz zieloną diodę LED - rosa
    Serial.print("ON\t");                                             //  Wyślij tekst - ON i dodaj tabulator

    analogWrite(HEATER, 32);                                          //  PWM  - wypełnienie 2 (grzałkę symuluje dioda LED)
    Serial.print("PWM 32, 5 - 3");                                    //  Wyślij tekst o wypełnieniu PWM i zakresie
  }
  else if ((t - td < 3) && (t - td > 1)) {                            //  Jeżeli różnica t-td jest mniejsza 3 i większa 1
    digitalWrite(LED_R_PIN, HIGH);                                    //  Włącz czerwoną diodę LED - grzałka pracuje
    digitalWrite(LED_G_PIN, LOW);                                     //  Wyłącz zieloną diodę LED - rosa
    Serial.print("ON\t");                                             //  Wyślij tekst - ON i dodaj tabulator

    analogWrite(HEATER, 64);                                          //  PWM  - wypełnienie 64 (grzałkę symuluje dioda LED)
    Serial.print("PWM 64, 3 - 1");                                    //  Wyślij tekst o wypełnieniu PWM i zakresie
  }
  else if (t - td == 1) {                                             //  Jeżeli różnica t-td jest równa 1
    digitalWrite(LED_R_PIN, HIGH);                                    //  Włącz czerwoną diodę LED - grzałka pracuje
    digitalWrite(LED_G_PIN, LOW);                                     //  Wyłącz zieloną diodę LED - rosa
    Serial.print("ON\t");                                             //  Wyślij tekst - ON i dodaj tabulator

    analogWrite(HEATER, 255);                                         //  PWM  - wypełnienie 255 (grzałkę symuluje dioda LED) - grzałka pracuje pełną mocą
    Serial.print("PWM 255, 1");                                       //  Wyślij tekst o wypełnieniu PWM i zakresie
  }
  else {
    digitalWrite(LED_R_PIN, LOW);                                     //  Wyłącz czerwoną diodę LED - grzałka nie pracuje
    digitalWrite(LED_G_PIN, HIGH);                                    //  Włącz zieloną diodę LED - brak rosy
    Serial.print("OFF\t");                                            //  Znacznik grzałki - OFF (kiedy td+1 < t)

    analogWrite(HEATER, 0);                                           //  PWM  - wypełnienie 0 (grzałkę symuluje dioda LED)
    Serial.print("PWM 0");                                            //  Wyślij tekst o wypełnieniu PWM
  }

Serial.println(" ");                                                  //  Przejdz do następnego wiersza (Serial Monitor)

}
