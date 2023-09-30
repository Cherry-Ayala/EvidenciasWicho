#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Función para verificar si una cadena es palíndromo
bool esPalindromo(string str) {
    int inicio = 0;
    int fin = str.length() - 1;

    while (inicio < fin) {
        if (str[inicio] != str[fin]) {
            return false;
        }
        inicio++;
        fin--;
    }
    return true;
}

// Función para buscar un código en una transmisión y recopilar información
void buscarCodigoEnTransmision(const string& codigo, ifstream& transmision, int& ocurrencias, vector<string>& subsecuencias, string& palindromo) {
    int posicion = 0;
    int maxLongitudPalindromo = 0;
    string maxPalindromo = "";
    string linea;

    while (getline(transmision, linea)) {
        size_t encontrado = linea.find(codigo);
        while (encontrado != string::npos) {
            ocurrencias++;
            subsecuencias.push_back(linea.substr(encontrado, codigo.length()));
            encontrado = linea.find(codigo, encontrado + 1);
        }

        // Buscar palíndromos
        for (int i = 0; i < linea.length(); i++) {
            for (int longitud = 1; longitud <= min(1000, static_cast<int>(linea.length() - i)); longitud++) {
                string subcadena = linea.substr(i, longitud);
                if (esPalindromo(subcadena) && longitud > maxLongitudPalindromo) {
                    maxLongitudPalindromo = longitud;
                    maxPalindromo = subcadena;
                }
            }
        }

        posicion++;
    }

    palindromo = maxPalindromo;
}

// Función para analizar las transmisiones y generar resultados
void analizarTransmisiones(const vector<string>& transmisiones, const vector<string>& codigos, ofstream& resultado) {
    for (const string& codigo : codigos) {
        resultado << "Código: " << codigo << endl;
        int ocurrencias[3] = {0};
        vector<string> subsecuencias[3];
        string palindromes[3];

        for (int i = 0; i < 3; i++) {
            ifstream transmision(transmisiones[i]);
            buscarCodigoEnTransmision(codigo, transmision, ocurrencias[i], subsecuencias[i], palindromes[i]);
        }

        for (int i = 0; i < 3; i++) {
            resultado << transmisiones[i] << " ==> " << ocurrencias[i] << " veces" << endl;
            resultado << "Posiciones: ";
            for (int j = 0; j < subsecuencias[i].size(); j++) {
                resultado << subsecuencias[i][j];
                if (j < subsecuencias[i].size() - 1) {
                    resultado << ", ";
                }
            }
            resultado << endl;
            resultado << "Palíndromo más grande: " << palindromes[i] << " (Posición: " << i << ")" << endl;
            resultado << "========================" << endl;
        }
    }
}

// Función para comparar las transmisiones entre sí
void compararTransmisiones(const vector<string>& transmisiones, ofstream& resultado) {
    string subcadenasMasLargas[3];
    ifstream transmisionesCombinadas[3];

    for (int i = 0; i < 3; i++) {
        transmisionesCombinadas[i].open(transmisiones[i].c_str());
    }

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            string subcadenaComunMasLarga = "";
            string linea1, linea2;

            while (getline(transmisionesCombinadas[i], linea1)) {
                while (getline(transmisionesCombinadas[j], linea2)) {
                    int m = linea1.length();
                    int n = linea2.length();
                    vector<vector<int> > dp(m + 1, vector<int>(n + 1, 0));

                    int longitudMaxima = 0;
                    int indiceFinal = 0;

                    for (int x = 1; x <= m; x++) {
                        for (int y = 1; y <= n; y++) {
                            if (linea1[x - 1] == linea2[y - 1]) {
                                dp[x][y] = dp[x - 1][y - 1] + 1;
                                if (dp[x][y] > longitudMaxima) {
                                    longitudMaxima = dp[x][y];
                                    indiceFinal = x - 1;
                                }
                            } else {
                                dp[x][y] = 0;
                            }
                        }
                    }

                    if (longitudMaxima > subcadenaComunMasLarga.length()) {
                        subcadenaComunMasLarga = linea1.substr(indiceFinal - longitudMaxima + 1, longitudMaxima);
                    }
                }
            }

            subcadenasMasLargas[i] = subcadenaComunMasLarga;
            subcadenasMasLargas[j] = subcadenaComunMasLarga;

            transmisionesCombinadas[i].clear();
            transmisionesCombinadas[j].clear();
            transmisionesCombinadas[i].seekg(0, ios::beg);
            transmisionesCombinadas[j].seekg(0, ios::beg);
        }
    }

    resultado << "Los Substring más largos son:" << endl;
    resultado << "T1-T2 ==> " << subcadenasMasLargas[0] << endl;
    resultado << "T1-T3 ==> " << subcadenasMasLargas[1] << endl;
    resultado << "T2-T3 ==> " << subcadenasMasLargas[2] << endl;
}

int main() {
    vector<string> transmisiones;
    transmisiones.push_back("Transmission1.txt");
    transmisiones.push_back("Transmission2.txt");
    transmisiones.push_back("Transmission3.txt");

    vector<string> codigos;
    ifstream mcode("mcode.txt");

    if (!mcode) {
        cerr << "Error al abrir el archivo mcode.txt." << endl;
        return 1;
    }

    string codigo;
    while (getline(mcode, codigo)) {
        codigos.push_back(codigo);
    }

    ofstream resultado("checking.txt");

    if (!resultado) {
        cerr << "Error al crear el archivo de salida." << endl;
        return 1;
    }

    analizarTransmisiones(transmisiones, codigos, resultado);
    compararTransmisiones(transmisiones, resultado);

    return 0;
}
