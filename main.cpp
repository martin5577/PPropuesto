#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// estructura de libro
struct libro {
    string codigo;
    string titulo;
    string autor;
    string anio;
};

// Funciones declaradas
int hash_func(const string& cadena);
void alta_func();
bool cargar_Archivo();
void mostrar();
void consulta();
void baja();

int main() {
    char opcion;
    do {
        cout << "\nMenu\n";
        puts("1. Alta");
        puts("2. Baja");
        puts("3. Consulta");
        puts("4. Mostrar");
        puts("5. Salir");
        do {
            printf("Opcion:");
            cin >> opcion;
        } while (opcion < '1' || opcion > '5');
        switch (opcion) {
            case '1':
                alta_func();
                break;
            case '2':
                baja();
                break;
            case '3':
                consulta();
                break;
            case '4':
                mostrar();
                break;
        }
    } while (opcion != '5');
    return 0;
}

// 1. Funcion Hash
int hash_func(const string& cadena) {
    int suma = 0;
    for (char letra : cadena) {
        suma += static_cast<int>(letra);  // Suma el valor ASCII de cada letra
    }
    int posicion = suma % 100;  // Aplica módulo 100 para obtener un número entre 0 y 99
    return posicion + 1;         // Ajusta la posición para que esté entre 1 y 100
}

// 2. Función para agregar datos desde teclado, El código debe ser una cadena únicamente de letras mayúsculas de longitud 4.
void alta_func() {
    struct libro* lib;
    string cadena;
    int n = 0;
    lib = new struct libro;
    while (n == 0) {
        cout << "Ingresa un codigo de 4 letras en mayúsculas: ";
        cin >> cadena;
        if (cadena.length() == 4) {
            bool esMayuscula = true;
            for (char letra : cadena) {
                if (!isupper(letra)) {
                    esMayuscula = false;
                    break;
                }
            }
            if (esMayuscula) {
                int posicion = hash_func(cadena);
                cout << "La posicion calculada es: " << posicion << endl;
                lib->codigo = cadena;
                n = 1;
            } else {
                cout << "Por favor, ingresa un codigo de 4 letras en mayusculas." << endl;
            }
        } else {
            cout << "Por favor, ingresa un codigo de 4 letras." << endl;
        }
    }
    cin.ignore();
    cout << endl << "Ingrese el autor: ";
    getline(cin, lib->autor);
    cout << endl << "Ingrese el titulo: ";
    getline(cin, lib->titulo);
    cout << endl << "Ingrese el año: ";
    getline(cin, lib->anio);
    cout << "\n" << lib->codigo << "," << lib->titulo << "," << lib->autor << "," << lib->anio;
    // Abre un archivo llamado "datos.csv" en modo escritura
    ofstream archivo("datos_libro.csv", ios::app);  // Modificado para agregar al final del archivo
    // Comprueba si el archivo se abrió correctamente
    if (archivo.is_open()) {
        // Escribe los datos en formato CSV
        archivo << lib->codigo << "," << lib->titulo << "," << lib->autor << "," << lib->anio << "\n";
        // Cierra el archivo
        archivo.close();
        cout << "Datos agregados exitosamente." << endl;
    } else {
        cerr << "Error al abrir el archivo." << endl;
    }
}

// 3. Función para agregar datos desde archivos CSV
bool cargar_Archivo() {
    struct libro* lib;
    string linea;
    ifstream archivo("datos_libro.csv");  // Modificado el nombre del archivo
    // Comprueba si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return false;
    } else {
        lib = new struct libro;
        // Lee el archivo línea por línea
        while (getline(archivo, linea)) {
            istringstream ss(linea);
            // Parsea la línea usando ',' como delimitador
            getline(ss, lib->codigo, ',');
            getline(ss, lib->titulo, ',');
            getline(ss, lib->autor, ',');
            getline(ss, lib->anio);
            // Crea una instancia de Libro
            cout << "\n" << lib->codigo << "," << lib->titulo << "," << lib->autor << "," << lib->anio << "\n";
        }
        archivo.close();
        return true;
    }
}

// 4. Función para Desplegar Area de datos, función para Desplegar área de colisiones.
void mostrar() {
    ifstream archivo("datos_libro.csv");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    struct libro* lib;
    lib = new struct libro;
    string linea;
    int total_registros = 0;
    int ocupados_datos = 0;
    int ocupados_colisiones = 0;

    // Muestra los valores del registro desde la línea del archivo CSV
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        getline(ss, lib->codigo, ',');
        getline(ss, lib->titulo, ',');
        getline(ss, lib->autor, ',');
        getline(ss, lib->anio);

        total_registros++;

        // Verifica si hay colisión en la misma posición hash
        int actual_posicion = hash_func(lib->codigo);
        if (actual_posicion != hash_func(lib->codigo))
            ocupados_colisiones++;
        else
            ocupados_datos++;
    }

    archivo.close();

    int total_espacios = 100;  // Total de posiciones en la tabla hash (puedes ajustarlo según tu necesidad)

    cout << "Área de Datos:" << endl;
    cout << " - Registros Ocupados: " << ocupados_datos << endl;
    cout << " - Registros Libres: " << total_espacios - ocupados_datos << endl;
    cout << " - Porcentaje Ocupado: " << (static_cast<double>(ocupados_datos) / total_espacios) * 100 << "%" << endl;

    cout << "\nÁrea de Colisiones:" << endl;
    cout << " - Registros Ocupados: " << ocupados_colisiones << endl;
    cout << " - Registros Libres: " << total_espacios - ocupados_colisiones << endl;
    cout << " - Porcentaje Ocupado: " << (static_cast<double>(ocupados_colisiones) / total_espacios) * 100 << "%" << endl;
}

// 5. Función para consulta de datos.
void consulta() {
    // Implementación de la función consulta
    string codigo;
    cout << "Ingrese el código del registro que desea consultar: ";
    cin >> codigo;

    int posicion = hash_func(codigo);

    ifstream archivo("datos_libro.csv");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    struct libro* lib;
    lib = new struct libro;
    string linea;
    int contador = 1;  // Contador para la posición en el archivo

    // Obtiene los valores del registro desde la línea del archivo CSV
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        getline(ss, lib->codigo, ',');
        getline(ss, lib->titulo, ',');
        getline(ss, lib->autor, ',');
        getline(ss, lib->anio);

        // Verificar si el código del libro coincide con el código ingresado
        if (lib->codigo == codigo) {
            cout << "El registro se encuentra en la posición " << contador << "." << endl;
            return;
        }
        // Verificar si hay colisión en la misma posición hash
        int actual_posicion = hash_func(lib->codigo);
        if (actual_posicion == posicion && lib->codigo != codigo) {
            cout << "El registro de colisión se encuentra en la posición " << contador << "." << endl;
            return;
        }

        contador++;
    }

    cout << "Registro no encontrado." << endl;
    archivo.close();
}

// 6. Función para Eliminar de datos.
void baja() {
    string codigo;
    cout << "Por favor, ingrese el código del registro que desea eliminar: ";
    cin >> codigo;

    int posicion = hash_func(codigo);

    ifstream archivo_lectura("datos_libro.csv");
    if (!archivo_lectura.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    struct libro* lib;
    lib = new struct libro;
    string linea;
    int contador = 1;  // Contador para la posición en el archivo

    ofstream archivo_escritura("temp.csv");  // Archivo temporal para almacenar datos después de la eliminación

    // Obtiene los valores del registro desde la línea del archivo CSV
    while (getline(archivo_lectura, linea)) {
        istringstream ss(linea);
        getline(ss, lib->codigo, ',');
        getline(ss, lib->titulo, ',');
        getline(ss, lib->autor, ',');
        getline(ss, lib->anio);

        // Verificar si el código del libro coincide con el código ingresado
        if (lib->codigo == codigo) {
            cout << "El registro que desea eliminar se encuentra en la posición número" << contador << "." << endl;
        } else {
            archivo_escritura << lib->codigo << "," << lib->titulo << "," << lib->autor << "," << lib->anio << "\n";
        }

        contador++;
    }

    archivo_lectura.close();
    archivo_escritura.close();

    // Eliminar el archivo original para poder reemplazarlo con el archivo temporal para mantener los cambios
    remove("datos_libro.csv");

    // Renombrar el archivo temporal al original para conservar el formato del nombre del archivo
    rename("temp.csv", "datos_libro.csv");

    cout << "El registro se eliminó exitosamente." << endl;
}
