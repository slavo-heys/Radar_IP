// Program pokazujący informacje jakie można uzyskać z adresu IP
// Biblioteki: Projekt wykorzystuje bibliotekę cpr do żądań HTTP oraz bibliotekę nlohmann/json do parsowania JSON.
// Kompilator: Projekt kompilowany przy użyciu g++ (standard C++17).
// Wymagane zależności: biblioteka cpr (oraz jej zależność libcurl), biblioteka nlohmann/json (tylko nagłówkowa).
//                     Użytkownik jest odpowiedzialny za instalację `cpr` w systemie przed kompilacją.
//                     Plik `nlohmann/json.hpp` powinien znajdować się w podkatalogu `include`.
// Przykładowa komenda kompilacji (Linux, z katalogu projektu, zakładając zainstalowane `cpr`):
// g++ -std=c++17 -I./include radar.cpp -o radar -lcpr -lcurl -lssl -lcrypto
// Upewnij się, że nagłówki cpr i nlohmann/json są w ścieżce include, a biblioteka cpr jest linkowana.
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

// Użycie cpr do wykonywania żądań HTTP
using json = nlohmann::json; // Alias dla typu json jest bardzo wygodny.

int main() {
    // Wyświetlenie logo
    std::cout << R"(
 ____  _            _    
|  _ \| |          | |   
| |_) | | __ _  ___| | __
|  _ <| |/ _` |/ __| |/ /
| |_) | | (_| | (__|   < 
|____/|_|\__,_|\___|_|\_\
       radar IP by black
)" << std::endl << std::endl;

    std::string ip;
    std::cout << "Podaj adres IP: ";
    std::cin >> ip;

    // Wykonanie zapytania HTTP do API
    // Konstrukcja URL jest bezpieczna, o ile 'ip' nie zawiera specjalnych znaków psujących URL,
    // ale dla prostego adresu IP jest to zazwyczaj w porządku.
    cpr::Response r = cpr::Get(cpr::Url{"http://ip-api.com/json/" + ip});

    // Sprawdzenie, czy zapytanie się powiodło na poziomie HTTP
    if (r.status_code != 200) {
        std::cerr << "Błąd podczas pobierania danych: HTTP status " << r.status_code << std::endl;
        if (!r.error.message.empty()) {
            std::cerr << "Komunikat błędu cpr: " << r.error.message << std::endl;
        }
        return 1; // Zakończ program z kodem błędu
    }

    // Użycie bloku try-catch do obsługi potencjalnych błędów parsowania JSON
    // lub dostępu do nieistniejących kluczy (choć .value() i .contains() temu zapobiegają)
    try {
        // Parsowanie odpowiedzi JSON
        json response_json = json::parse(r.text);

        // Sprawdzenie statusu odpowiedzi z API (wewnątrz JSON)
        // Użycie .contains() przed dostępem do elementu jest bezpieczniejsze
        if (response_json.contains("status") && response_json["status"].is_string() && response_json["status"] == "success") {
            // Wyświetlenie informacji o adresie IP
            // Użycie .value("klucz", "wartość_domyślna") jest bezpiecznym sposobem na odczyt wartości,
            // zwraca wartość domyślną, jeśli klucz nie istnieje lub ma niekompatybilny typ (dla stringów).
            std::cout << "Informacje o adresie IP: " << std::endl;
            std::cout << "IP: " << response_json.value("query", "Brak danych") << std::endl;
            std::cout << "Kraj: " << response_json.value("country", "Brak danych") << std::endl;
            std::cout << "Region: " << response_json.value("regionName", "Brak danych") << std::endl;
            std::cout << "Miasto: " << response_json.value("city", "Brak danych") << std::endl;
            
            // Obsługa pola "zip", które może być puste
            std::string zip_code = response_json.value("zip", ""); // Domyślnie pusty string, jeśli brak
            std::cout << "Kod pocztowy: " << (zip_code.empty() ? "Brak danych" : zip_code) << std::endl;

            // Bezpieczne odczytywanie wartości numerycznych (lat, lon)
            std::cout << "Szerokość geograficzna: ";
            if (response_json.contains("lat") && response_json["lat"].is_number()) {
                std::cout << response_json["lat"].get<double>() << std::endl;
            } else {
                std::cout << "Brak danych" << std::endl;
            }

            std::cout << "Długość geograficzna: ";
            if (response_json.contains("lon") && response_json["lon"].is_number()) {
                std::cout << response_json["lon"].get<double>() << std::endl;
            } else {
                std::cout << "Brak danych" << std::endl;
            }
        } else if (response_json.contains("message") && response_json["message"].is_string()) {
            // Jeśli API zwróciło status błędu, ale z wiadomością
            std::cerr << "Błąd API: " << response_json["message"].get<std::string>() << std::endl;
            if (response_json.contains("query") && response_json["query"].is_string()){
                 std::cerr << "Dla zapytania: " << response_json["query"].get<std::string>() << std::endl;
            }
            return 1;
        } else {
            // Nieoczekiwany format odpowiedzi lub błędu
            std::cerr << "Odpowiedź API niekompletna, nieznany format błędu lub status inny niż 'success' bez wiadomości." << std::endl;
            std::cerr << "Otrzymano (fragment): " << r.text.substr(0, 200) << (r.text.length() > 200 ? "..." : "") << std::endl;
            return 1;
        }
    } catch (const json::parse_error& e) {
        std::cerr << "Błąd parsowania odpowiedzi JSON: " << e.what() << std::endl;
        std::cerr << "Otrzymana odpowiedź (fragment): " << r.text.substr(0, 200) << (r.text.length() > 200 ? "..." : "") << std::endl;
        return 1;
    } catch (const json::exception& e) { // Łapanie innych wyjątków nlohmann::json (np. type_error, out_of_range)
        std::cerr << "Błąd obsługi danych JSON: " << e.what() << std::endl;
        std::cerr << "Otrzymana odpowiedź (fragment): " << r.text.substr(0, 200) << (r.text.length() > 200 ? "..." : "") << std::endl;
        return 1;
    }

    return 0; // Sukces
}

// Kompilacja: g++ -std=c++17 -I./include radar.cpp -o radar -lcpr -lcurl -lssl -lcrypto

// Uruchomienie: ./radar
// Uwaga: Upewnij się, że masz zainstalowane biblioteki cpr i nlohmann/json oraz że są one dostępne w ścieżkach include i linkowania.
// Wymagane biblioteki:
                                
