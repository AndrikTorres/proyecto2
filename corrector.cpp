#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

#define TAMTOKEN 50
#define MAXPALABRAS 50000

// Prototipos de funciones
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos);
void ClonaPalabras(char* szPalabraLeida, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas);
void ListaCandidatas(char szPalabrasSugeridas[][TAMTOKEN], int iNumSugeridas, char szPalabras[][TAMTOKEN],
    int iEstadisticas[], int iNumElementos, char szListaFinal[][TAMTOKEN], int iPeso[], int& iNumLista);

// Implementación de funciones
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos) {
    std::ifstream archivo(szNombre);
    iNumElementos = 0;

    if (!archivo) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    char palabra[TAMTOKEN];
    while (archivo >> palabra) {
        // Convertir a minúsculas
        for (int i = 0; palabra[i]; ++i) {
            palabra[i] = std::tolower(palabra[i]);
        }

        // Buscar si la palabra ya existe en el diccionario
        bool encontrada = false;
        for (int i = 0; i < iNumElementos; ++i) {
            if (std::strcmp(szPalabras[i], palabra) == 0) {
                ++iEstadisticas[i];
                encontrada = true;
                break;
            }
        }

        // Si no se encuentra, agregarla
        if (!encontrada && iNumElementos < MAXPALABRAS) {
            std::strcpy(szPalabras[iNumElementos], palabra);
            iEstadisticas[iNumElementos] = 1;
            ++iNumElementos;
        }
    }

    // Ordenar el diccionario
    for (int i = 0; i < iNumElementos - 1; ++i) {
        for (int j = i + 1; j < iNumElementos; ++j) {
            if (std::strcmp(szPalabras[i], szPalabras[j]) > 0) {
                std::swap(szPalabras[i], szPalabras[j]);
                std::swap(iEstadisticas[i], iEstadisticas[j]);
            }
        }
    }

    archivo.close();
}

void ClonaPalabras(char* szPalabraLeida, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas) {
    iNumSugeridas = 0;
    int len = std::strlen(szPalabraLeida);

    // Eliminación de un carácter
    for (int i = 0; i < len; ++i) {
        char temp[TAMTOKEN];
        std::strncpy(temp, szPalabraLeida, i);
        std::strcpy(temp + i, szPalabraLeida + i + 1);
        std::strcpy(szPalabrasSugeridas[iNumSugeridas++], temp);
    }

    // Transposición de carácteres
    for (int i = 0; i < len - 1; ++i) {
        char temp[TAMTOKEN];
        std::strcpy(temp, szPalabraLeida);
        std::swap(temp[i], temp[i + 1]);
        std::strcpy(szPalabrasSugeridas[iNumSugeridas++], temp);
    }

    // Sustitución de carácteres
    const char alfabeto[] = "abcdefghijklmnopqrstuvwxyz\u00e1\u00e9\u00ed\u00f3\u00fa\u00f1";
    for (int i = 0; i < len; ++i) {
        for (const char& letra : alfabeto) {
            char temp[TAMTOKEN];
            std::strcpy(temp, szPalabraLeida);
            temp[i] = letra;
            std::strcpy(szPalabrasSugeridas[iNumSugeridas++], temp);
        }
    }

    // Inserción de carácteres
    for (int i = 0; i <= len; ++i) {
        for (const char& letra : alfabeto) {
            char temp[TAMTOKEN];
            std::strncpy(temp, szPalabraLeida, i);
            temp[i] = letra;
            std::strcpy(temp + i + 1, szPalabraLeida + i);
            std::strcpy(szPalabrasSugeridas[iNumSugeridas++], temp);
        }
    }
}

void ListaCandidatas(char szPalabrasSugeridas[][TAMTOKEN], int iNumSugeridas, char szPalabras[][TAMTOKEN],
    int iEstadisticas[], int iNumElementos, char szListaFinal[][TAMTOKEN], int iPeso[], int& iNumLista) {
    iNumLista = 0;

    for (int i = 0; i < iNumSugeridas; ++i) {
        for (int j = 0; j < iNumElementos; ++j) {
            if (std::strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0) {
                std::strcpy(szListaFinal[iNumLista], szPalabras[j]);
                iPeso[iNumLista] = iEstadisticas[j];
                ++iNumLista;
                break;
            }
        }
    }

    // Ordenar por peso (descendente)
    for (int i = 0; i < iNumLista - 1; ++i) {
        for (int j = i + 1; j < iNumLista; ++j) {
            if (iPeso[i] < iPeso[j]) {
                std::swap(iPeso[i], iPeso[j]);
                std::swap(szListaFinal[i], szListaFinal[j]);
            }
        }
    }
}

// Main para pruebas
int main() {
    char diccionario[MAXPALABRAS][TAMTOKEN] = { 0 };
    int frecuencias[MAXPALABRAS] = { 0 };
    int numPalabras = 0;

    Diccionario("diccionario.txt", diccionario, frecuencias, numPalabras);

    char palabra[TAMTOKEN] = "anis";
    char sugeridas[MAXPALABRAS][TAMTOKEN] = { 0 };
    int numSugeridas = 0;

    ClonaPalabras(palabra, sugeridas, numSugeridas);

    char listaFinal[MAXPALABRAS][TAMTOKEN] = { 0 };
    int pesos[MAXPALABRAS] = { 0 };
    int numLista = 0;

    ListaCandidatas(sugeridas, numSugeridas, diccionario, frecuencias, numPalabras, listaFinal, pesos, numLista);

    std::cout << "Sugerencias: \n";
    for (int i = 0; i < numLista; ++i) {
        std::cout << listaFinal[i] << " (Peso: " << pesos[i] << ")\n";
    }

    return 0;
}
