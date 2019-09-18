## Funkcje:
    - Pomiar temperatury
    - Pomiar wilgotności
    - Obliczenie punktu rosy
    - Sterowanie grzałką teleskopu - włącz / wyłącz
    - Sygnalizacja błędu odczytu z czujnika DHT

## Wymagane Biblioteki:
    - Adafruit Unified Sensor (Adafruit)
    - DHT Sensor Library (Adafruit)

## Uwagi:
    - Program demonstruje działanie automatycznego sterownika grzałek.
    - Czerwona dioda LED sygnalizuje (symuluje) włączenie grzałki
    - Zielona dioda LED sygnalizuje brak roszenia
    - Program nie ma zdefiniowanego pinu / pinów do sterowania grzałką / grzałkami
    - Komunikacja szeregowa (COM) służy jako "monitor" działania programu - docelowo może być wyłączona w celu oszczędności pamięci programu
    - Rozwiązanie programowe absolutnie nie jedynie słuszne
    - Za uszkodzenia wynikające z działania programu nie odpowiadam, program stworzony został jako przykład dla urządzenia modelowego (bez grzałki)

## Poprawki do wersji 1.2
    - poprawiono komentarze
    - zmieniony został blok związany z obsługą grzałki, inna budowa warunku
    - dodanie pinu obsługującego grzałkę (pin cyfrowy nr 6)
    - dodanie funkcji PWM dla pinu nr 6
    - zmienione zostały typy zmiennych t, h, td z float na int
    - zmieniono warunek braku komunikacji/danych z czujnika DHT
