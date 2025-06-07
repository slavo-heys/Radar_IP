# Radar IP

Prosty program konsolowy w C++ do pobierania i wyświetlania publicznie dostępnych informacji geolokalizacyjnych oraz sieciowych dla podanego adresu IP. Wykorzystuje zewnętrzne API (ip-api.com) do uzyskania danych.

## Funkcjonalności

*   Pobiera informacje dla dowolnego publicznego adresu IP.
*   Wyświetla takie dane jak:
    *   Adres IP (potwierdzenie zapytania)
    *   Kraj
    *   Region
    *   Miasto
    *   Kod pocztowy
    *   Szerokość geograficzna
    *   Długość geograficzna

## Wymagane biblioteki

Ten projekt wykorzystuje następujące biblioteki C++:

1.  **[cpr (C++ Requests)](https://github.com/libcpr/cpr)**: Nowoczesna biblioteka C++ do obsługi żądań HTTP, używana do komunikacji z API.
    *   **Zależność `cpr`**: Biblioteka `cpr` sama w sobie zależy od `libcurl`.
2.  **nlohmann/json**: Popularna biblioteka do pracy z formatem JSON w C++. Jest to biblioteka tylko-nagłówkowa.

## Instalacja zależności

Przed kompilacją programu, upewnij się, że masz zainstalowane powyższe biblioteki w swoim systemie.

### 1. Instalacja `cpr` i `libcurl`

Sposób instalacji może się różnić w zależności od systemu operacyjnego i menedżera pakietów.

*   **Debian/Ubuntu:**
    ```bash
    sudo apt update
    sudo apt install libcpr-dev libcurl4-openssl-dev
    ```
    (Jeśli `libcpr-dev` nie jest dostępne, może być konieczna kompilacja ze źródeł lub użycie menedżera pakietów C++ jak `vcpkg`.)

*   **Fedora:**
    ```bash
    sudo dnf install cpr-devel libcurl-devel
    ```

*   **macOS (używając Homebrew):**
    ```bash
    brew install cpr curl
    ```

*   **Inne systemy / Kompilacja ze źródeł:**
    Zapoznaj się z oficjalną dokumentacją instalacji `cpr`.

### 2. Instalacja `nlohmann/json`

Biblioteka `nlohmann/json` jest biblioteką tylko-nagłówkową. Najprostszym sposobem jest pobranie pliku `json.hpp` i umieszczenie go w odpowiednim miejscu w projekcie.

1.  Pobierz najnowszą wersję pliku `json.hpp` z sekcji Releases na GitHubie nlohmann/json.
2.  W katalogu głównym projektu (`Radar_IP`) utwórz podkatalogi: `include/nlohmann/`.
3.  Umieść pobrany plik `json.hpp` w ścieżce `/home/black/c++/Radar_IP/include/nlohmann/json.hpp` (lub relatywnie `include/nlohmann/json.hpp` względem pliku `radar.cpp`).

## Kompilacja

Program napisany jest w standardzie C++17. Do kompilacji można użyć kompilatora `g++`.

1.  Upewnij się, że wszystkie zależności są zainstalowane.
2.  Przejdź do katalogu głównego projektu (`/home/black/c++/Radar_IP/`).
3.  Skompiluj program używając poniższej komendy:

    ```bash
    g++ -std=c++17 -I./include radar.cpp -o radar -lcpr -lcurl -lssl -lcrypto
    ```
    *   `-std=c++17`: Ustawia standard C++.
    *   `-I./include`: Informuje kompilator, aby szukał plików nagłówkowych w podkatalogu `include` (dla `nlohmann/json.hpp`).
    *   `radar.cpp`: Twój plik źródłowy.
    *   `-o radar`: Nazwa pliku wykonywalnego.
    *   `-lcpr`: Linkuje z biblioteką `cpr`.
    *   `-lcurl`: Linkuje z biblioteką `libcurl` (zależność `cpr`).
    *   `-lssl -lcrypto`: Linkuje z bibliotekami OpenSSL (często wymagane przez `libcurl` dla HTTPS).

## Uruchomienie

Po pomyślnej kompilacji, program można uruchomić z terminala:

```bash
./radar

---------------------------------------------------------------------------------------
## Przykład użycia
Podaj adres IP: 8.8.8.8
Informacje o adresie IP: 
IP: 8.8.8.8
Kraj: United States
Region: California
Miasto: Mountain View
Kod pocztowy: 94043
Szerokość geograficzna: 37.422
Długość geograficzna: -122.084
