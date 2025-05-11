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
        string registrarDocente();
};

void Docente::mostrarMaterias() {
    // Mostrar todas las materias

}

string Docente::registrarDocente() {

}

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

void Estudiante::mostrarCalificaciones() {
    // Mostrar calificaciones del estudiante
};

class Grupo {
    public:
        string codigo;
        string materia;
        string semestre;
        string horario;
        Docente *maestro;
        // map<int, Estudiante*> alumnos; // Estudiantes, calificacion
        // map<int, float> calificaciones;
        map<string, float> estudiantes; // matricula - calificacion
        void inscribir(Estudiante*);
        void mostrarCalificaciones();
        void crearGrupo();        
};

void Grupo::crearGrupo() {

}


void Grupo::inscribir(Estudiante *alumno) {
    // Registrar alumno en materia
    // int id = this->alumnos.size();
    // this->alumnos.insert({id, alumno});
    // this->alumnos.insert()
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

        void guardarMemoria();
        void cargarMemoria();
};

void Menu::registrarSemestre() {
    Semestre s;
    string aux1, aux2;
    cout << "Año actual: ";
    cin >> aux1;
    cout << "Semestre A o B: ";
    cin >> aux2;
    s.identificador = aux1 + aux2;


};

void Menu::registrarEstudiante() {
    int aux;
    Estudiante e;
    cout << "Nombre del estudiante: ";
    cin >> e.nombre;
    cout << "Matricula existente (0 para generar nueva): ";
    cin >> aux;
    if(!aux) {
        // string semestreActual = this->semestres.rbegin() != map.rend();
        e.matricula = this->estudiantes.size() + 1;
    }
};

void Menu::registrarDocente() {
    Docente d;
    cout << "\nNombre del docente: ";
    cin >> d.nombre;
    cout << "\nEspecialidad: "; 
    cin >> d.especialidad;
    d.numeroEmpleado = (rand() * 10000) + 101;
    cout << d.nombre << " registrado correctamente.\nNumero de empleado: \t" << d.numeroEmpleado;
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
    while(p.seguir) {
        cout << "\n\n\t\tSistema de control escolar\n";
        cout << "1. Registrar semestre\n";
        cout << "2. Registrar estudiante\n";
        cout << "3. Registrar docente\n";
        cout << "4. Mostrar docentes\n";
        cout << "5. Mostrar estudiantes\n";
        cout << "6. Mostrar semestres\n";
        cout << "7. Guardar datos\n";
        cout << "8. Salir\n";
        cout << "Selecciona una opción: ";
        cin >> p.seguir;

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
                break;
            case 5:
                // Mostrar estudiantes
                break;
            case 6:
                // Mostrar semestres
                break;
            case 7:
                p.guardarMemoria();
                break;
            case 8:
                p.guardarMemoria();
                return 0;
            default:
                cout << "Opción no válida.\n";
        }
        cout << "\n\n¿Deseas continuar? (1: Sí, 0: No): ";
        cin >> p.seguir;
        if (p.seguir == 0) {
            p.guardarMemoria();
            cout << "Saliendo del sistema...\n";
        }
        // else if (p.seguir != 1) {
        //     cout << "Opción no válida. Saliendo del sistema...\n";
        //     p.guardarMemoria();
        // }
    }

    return 0;
}