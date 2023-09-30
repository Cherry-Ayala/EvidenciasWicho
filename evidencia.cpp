#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Función para encontrar la Longest Common Subsequence (LCS)
string encontrarLCS(const string& str1, const string& str2) {
    int m = str1.length();
    int n = str2.length();
    vector<vector<int> > dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int i = m;
    int j = n;
    string lcs;
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            lcs = str1[i - 1] + lcs;
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    return lcs;
}

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

// Función para comparar las transmisiones entre sí utilizando LCS
void compararTransmisiones(const vector<string>& transmisiones, ofstream& resultado) {
    for (int i = 0; i < transmisiones.size(); i++) {
        for (int j = i + 1; j < transmisiones.size(); j++) {
            ifstream transmision1(transmisiones[i]);
            ifstream transmision2(transmisiones[j]);

            string linea1, linea2;
            string trans1, trans2;

            while (getline(transmision1, linea1)) {
                trans1 += linea1;
            }

            while (getline(transmision2, linea2)) {
                trans2 += linea2;
            }

            string subcadenaComunMasLarga = encontrarLCS(trans1, trans2);

            resultado << "T" << i + 1 << "-T" << j + 1 << " ==> " << subcadenaComunMasLarga << endl;
        }
    }
}

int main() {
    vector<string> transmisiones;
    transmisiones.push_back("Transmission1.txt");
    transmisiones.push_back("Transmission2.txt");
    transmisiones.push_back("Transmission3.txt");

    vector<string> codigos;
    ifstream codigoFile("mcode.txt");
    string codigo;
    while (getline(codigoFile, codigo)) {
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

