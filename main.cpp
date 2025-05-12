#include <iostream>
#include <map>
#include <random>
#include <locale.h>

#include <fstream>
#include <sstream>

using namespace std;

class Menu;
class Grupo;

class Semestre {
    public:
        string identificador;
        map<int, Grupo> grupos; 
};

class Docente {
    public:
        string numeroEmpleado; // ID
        string nombre;
        string especialidad;
        map<string, Grupo*> clases; // Nombre clase, id
        void mostrarMaterias();
        void registrarDocente(); // Cambiado de string a void
};

class Estudiante {
    public:
        string matricula; // ID
        string nombre;
        string grado;
        string correoElectronico;
        string semestre_inicio;
        map<int, Grupo*> clases; // Num generico, id
        void mostrarCalificaciones();
        void mostrarKardex();
};



class Grupo {
    public:
        string codigo;
        string materia;
        string semestre;
        string horario;
        Docente *maestro;
        map<string, float> estudiantes; // matricula - calificacion
        void inscribir(Estudiante*);
        void mostrarCalificaciones();      
};

/*
Esto debe de estar despues de grupo
*/
void Estudiante::mostrarCalificaciones() {
    // Mostrar calificaciones del estudiante
    cout << "\nCalificaciones de " << this->nombre << ": " << endl;
    if(this->clases.empty()) {
        cout << "No tiene materias inscritas." << endl;
    } 
    else {
        for(auto i: this->clases) {
            cout << "\n- " << i.second->materia << " : " << i.second->estudiantes.find(this->matricula)->second;
        }
    }
};

void Docente::mostrarMaterias() {
    // Mostrar todas las materias
    cout << "\nMaterias de " << this->nombre << ": " << endl;
    if(this->clases.empty()) {
        cout << "No tiene materias registradas." << endl;
    } 
    else {
        for(auto i: this->clases) {
            cout << "\n- " << i.second->materia << " [" << i.first << "]";
        }
    }
}

/*
    TERMINA BLOQUE DE FUNCIONES
*/


void Grupo::inscribir(Estudiante *alumno) {
    // Registrar alumno en materia
    this->estudiantes.insert({alumno->matricula, 0.0});
}

void Grupo::mostrarCalificaciones() {
    // Mostrar calificaciones de todos los alumnos
    for(auto i: this->estudiantes) {
        cout << "\nCalificacion de: " << i.first << " es " << i.second;
    }
}

class Menu {
    public:
        int seguir;
        map<string, Semestre> semestres;
        map<string, Estudiante> estudiantes;
        map<string, Docente> docentes;
        Menu(): seguir(1) {};

        void registrarSemestre();
        void registrarEstudiante();
        void registrarDocente();
        void crearGrupo();

        void guardarMemoria();
        void cargarMemoria();
};

void Menu::crearGrupo() {
    string numEmpleado;
    cout << "Numero de empleado del docente: ";
    cin >> numEmpleado;
    if(docentes.find(numEmpleado) == docentes.end()) {
        cout << "Docente no encontrado." << endl;
        return;
    }
    Docente *docente = &docentes[numEmpleado];
    Grupo g;
    string aux1, aux2, aux3;
    int semValido = 0;
    cout << "Codigo de materia: ";
    cin.ignore();
    getline(cin, g.codigo);
    cin.ignore();
    cout << "Materia: ";
    
    getline(cin, g.materia);
    while(!semValido) {
        cout << "Semestre: ";
    
        cout << "\nLista de semestres disponibles: " << endl;
        for(auto i: this->semestres) {
            cout << i.first << " - " << i.second.identificador << endl;
        }

        // Elimina el cin.ignore() que estaba aquí
        getline(cin, g.semestre);
        
        // Verificar si el semestre existe
        if(this->semestres.find(g.semestre) != this->semestres.end()) {
            semValido = 1;
        }
        else {
            semValido = 0;
            if(g.semestre == "SALIR") {
                cout << "Saliendo del registro de grupo." << endl;
                break;
            }
            else {
                cout << "Semestre no encontrado." << endl;
                cout << "Ingresaste: '" << g.semestre << "'" << endl; // Para depurar
            }
        }
    }
    cout << "Horario (LMi 10-12, V11-12): ";
    cin.ignore();
    getline(cin, g.horario);
    
    g.maestro = docente;

    docente->clases[g.codigo] = &g; // Asignar grupo al docente
    int numGrupo = this->semestres[g.semestre].grupos.size();
    this->semestres[g.semestre].grupos.insert({numGrupo, g}); // Asignar grupo al semestre
    cout << "\nGrupo creado correctamente." << endl;
}

void Menu::registrarSemestre() {
    Semestre s;
    string aux1, aux2;
    
    cout << "Año: ";
    cin >> aux1;
    
    cout << "Semestre A o B: ";
    cin >> aux2;
    
    s.identificador = aux1 + aux2;
    
    // Crear un ID único para este semestre
    string idSemestre = aux1 + aux2;
    
    // Guardar el semestre en el mapa
    semestres[idSemestre] = s;
    
    cout << "\nSemestre " << s.identificador << " registrado correctamente." << endl;
};

void Menu::registrarEstudiante() {
    Estudiante e;
    string auxMatricula;
    
    cout << "\nNombre del estudiante: ";
    cin.ignore(); // Limpiar buffer
    getline(cin, e.nombre);
    
    cout << "Matricula existente (0 para generar nueva): ";
    cin >> auxMatricula;
    
    if(auxMatricula == "0") {
        // Generar nueva matrícula
        int numMatricula = this->estudiantes.size() + 1;
        e.matricula = to_string(numMatricula);
    } else {
        e.matricula = auxMatricula;
    }
    
    cout << "Grado: ";
    cin.ignore();
    getline(cin, e.grado);
    
    cout << "Correo electrónico: ";
    getline(cin, e.correoElectronico);
    
    cout << "Semestre de inicio: ";
    getline(cin, e.semestre_inicio);
    
    // Guardar el estudiante en el mapa
    estudiantes[e.matricula] = e;
    
    cout << "\nEstudiante " << e.nombre << " registrado correctamente.";
    cout << "\nMatrícula: " << e.matricula << endl;
};

void Menu::registrarDocente() {
    Docente d;
    cout << "\nNombre del docente: ";
    cin.ignore(); // Limpiar el buffer de entrada
    getline(cin, d.nombre); // Usar getline para nombres con espacios
    
    cout << "\nEspecialidad: "; 
    getline(cin, d.especialidad);
    
    // Generar un número de empleado más consistente
    int numRandom = rand() % 9000 + 1000; // Genera un número entre 1000 y 9999
    d.numeroEmpleado = to_string(numRandom);
    
    // Guardar el docente en el mapa
    docentes[d.numeroEmpleado] = d;
    
    cout << "\n" << d.nombre << " registrado correctamente.";
    cout << "\nNumero de empleado: " << d.numeroEmpleado << endl;
    
    // Agregar una pausa para que el usuario vea la información
    cout << "\nPresiona Enter para continuar...";
    cin.get();
};

void Menu::guardarMemoria() {
    ofstream archivo("sistema_datos.txt");
    
    if (!archivo) {
        cout << "Error al abrir el archivo para guardar datos.\n";
        return;
    }
    
    // Guardar docentes
    archivo << "## DOCENTES ##\n";
    for (const auto& [numEmpleado, docente] : docentes) {
        archivo << numEmpleado << "|" << docente.nombre << "|" 
               << docente.especialidad << "\n";
    }
    
    // Guardar estudiantes
    archivo << "## ESTUDIANTES ##\n";
    for (const auto& [matricula, estudiante] : estudiantes) {
        archivo << matricula << "|" << estudiante.nombre << "|" 
               << estudiante.grado << "|" << estudiante.correoElectronico << "|" 
               << estudiante.semestre_inicio << "\n";
    }
    
    // Guardar semestres y grupos
    archivo << "## SEMESTRES ##\n";
    for (const auto& [id, semestre] : semestres) {
        archivo << id << "|" << semestre.identificador << "\n";
        
        // Guardar grupos
        archivo << "## GRUPOS DE " << id << " ##\n";
        for (const auto& [numGrupo, grupo] : semestre.grupos) {
            archivo << id << "|" << numGrupo << "|" << grupo.codigo << "|" 
                   << grupo.materia << "|" << grupo.semestre << "|" 
                   << grupo.horario << "|";
            
            // Maestro (si tiene)
            if (grupo.maestro) {
                archivo << grupo.maestro->numeroEmpleado;
            } else {
                archivo << "NULL";
            }
            archivo << "\n";
            
            // Guardar estudiantes del grupo
            if (!grupo.estudiantes.empty()) {
                archivo << "## ESTUDIANTES EN GRUPO " << id << "-" << numGrupo << " ##\n";
                for (const auto& [matricula, calificacion] : grupo.estudiantes) {
                    archivo << id << "|" << numGrupo << "|" << matricula << "|" 
                           << calificacion << "\n";
                }
            }
        }
    }
    
    archivo.close();
    cout << "Datos guardados correctamente.\n";
}

void Menu::cargarMemoria() {
    ifstream archivo("sistema_datos.txt");
    
    if (!archivo) {
        cout << "No se encontró archivo de datos o no se pudo abrir.\n";
        return;
    }
    
    // Limpiar datos actuales
    semestres.clear();
    estudiantes.clear();
    docentes.clear();
    
    string linea, seccion;
    
    // Leer el archivo línea por línea
    while (getline(archivo, linea)) {
        // Saltar líneas vacías
        if (linea.empty()) continue;
        
        // Detectar cambios de sección
        if (linea.substr(0, 2) == "##") {
            seccion = linea;
            continue;
        }
        
        stringstream ss(linea);
        string token;
        
        // Procesar según la sección actual
        if (seccion == "## DOCENTES ##") {
            string numEmpleado, nombre, especialidad;
            
            getline(ss, numEmpleado, '|');
            getline(ss, nombre, '|');
            getline(ss, especialidad, '|');
            
            Docente docente;
            docente.numeroEmpleado = numEmpleado;
            docente.nombre = nombre;
            docente.especialidad = especialidad;
            
            docentes[numEmpleado] = docente;
        }
        else if (seccion == "## ESTUDIANTES ##") {
            string matricula, nombre, grado, correo, semestre_inicio;
            
            getline(ss, matricula, '|');
            getline(ss, nombre, '|');
            getline(ss, grado, '|');
            getline(ss, correo, '|');
            getline(ss, semestre_inicio, '|');
            
            Estudiante estudiante;
            estudiante.matricula = matricula;
            estudiante.nombre = nombre;
            estudiante.grado = grado;
            estudiante.correoElectronico = correo;
            estudiante.semestre_inicio = semestre_inicio;
            
            estudiantes[matricula] = estudiante;
        }
        else if (seccion == "## SEMESTRES ##") {
            string id, identificador;
            
            getline(ss, id, '|');
            getline(ss, identificador, '|');
            
            Semestre semestre;
            semestre.identificador = identificador;
            
            semestres[id] = semestre;
        }
        else if (seccion.find("## GRUPOS DE ") != string::npos) {
            string idSemestre, numGrupoStr, codigo, materia, semestreGrupo, horario, numEmpleado;
            
            getline(ss, idSemestre, '|');
            getline(ss, numGrupoStr, '|');
            getline(ss, codigo, '|');
            getline(ss, materia, '|');
            getline(ss, semestreGrupo, '|');
            getline(ss, horario, '|');
            getline(ss, numEmpleado, '|');
            
            int numGrupo = stoi(numGrupoStr);
            
            Grupo grupo;
            grupo.codigo = codigo;
            grupo.materia = materia;
            grupo.semestre = semestreGrupo;
            grupo.horario = horario;
            
            // Enlazar con el maestro
            if (numEmpleado != "NULL" && docentes.find(numEmpleado) != docentes.end()) {
                grupo.maestro = &docentes[numEmpleado];
            } else {
                grupo.maestro = nullptr;
            }
            
            semestres[idSemestre].grupos[numGrupo] = grupo;
        }
        else if (seccion.find("## ESTUDIANTES EN GRUPO ") != string::npos) {
            string idSemestre, numGrupoStr, matricula, calificacionStr;
            
            getline(ss, idSemestre, '|');
            getline(ss, numGrupoStr, '|');
            getline(ss, matricula, '|');
            getline(ss, calificacionStr, '|');
            
            int numGrupo = stoi(numGrupoStr);
            float calificacion = stof(calificacionStr);
            
            if (semestres.find(idSemestre) != semestres.end() &&
                semestres[idSemestre].grupos.find(numGrupo) != semestres[idSemestre].grupos.end()) {
                semestres[idSemestre].grupos[numGrupo].estudiantes[matricula] = calificacion;
            }
        }
    }
    
    archivo.close();
    cout << "Datos cargados correctamente.\n";
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "spanish");
    Menu p;
    p.cargarMemoria();
    p.seguir = 1;
    
    while(p.seguir) {
        int opcion;
        cout << "\n\n\t\tSistema de control escolar\n";
        cout << "1. Registrar semestre\n";
        cout << "2. Registrar estudiante\n";
        cout << "3. Registrar docente\n";
        cout << "4. Mostrar docentes\n";
        cout << "5. Mostrar estudiantes\n";
        cout << "6. Mostrar semestres\n";
        cout << "0. Salir\n";
        cout << "Selecciona una opción: ";
        cin >> p.seguir;
        
        // Verificar si la entrada es válida
        if (cin.fail()) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(10000, '\n'); // Descartar entrada incorrecta
            cout << "Entrada no válida. Por favor ingrese un número.\n";
            p.seguir = 999;
            continue;
        }

        switch(p.seguir) {
            case 1:
                p.registrarSemestre();
                break;
            case 2:
                p.registrarEstudiante();
                break;
            case 3:
                p.registrarDocente();
                break;
            case 4:
                // Mostrar docentes
                cout << "\nLista de docentes:" << endl;
                for (const auto& [id, docente] : p.docentes) {
                    cout << "ID: " << id << " Nombre: " << docente.nombre 
                         << " Especialidad: " << docente.especialidad << endl;
                }
                break;
            case 5:
                // Mostrar estudiantes
                cout << "\nLista de estudiantes:" << endl;
                for (const auto& [matricula, estudiante] : p.estudiantes) {
                    cout << "Matrícula: " << matricula << " Nombre: " << estudiante.nombre << endl;
                }
                break;
            case 6:
                // Mostrar semestres
                cout << "\nLista de semestres:" << endl;
                for (const auto& [id, semestre] : p.semestres) {
                    cout << "ID: " << id << " Identificador: " << semestre.identificador << endl;
                }
                break;
            case 7:
                // Registrar grupo
                p.crearGrupo();
                break;

            case 0:
                p.guardarMemoria();
                p.seguir = 0;
                cout << "Datos guardados. Saliendo del sistema...\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
    }

    return 0;
}