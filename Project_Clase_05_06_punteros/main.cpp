#include <print>      // contiene las funciones std::print y std::println (incluye salto de línea)
#include <string>     // cadena de caracteres estándar
// #include <memory>     // contiene punteros inteligentes

struct Student {
    std::string name;
    double grade_1 = 0.0;
    double grade_2 = 0.0;
    double grade_3 = 0.0;

    auto avarage () -> double { return (grade_1 + grade_2 + grade_3)/3.0; }
};

void incremento(int* p) { // De esta manera no trabajamos con referencias.
    // Comprobar que el puntero no es nulo
    if (p == nullptr) {
        // Error.
    }
    if(!p) {
        ++*p;
    }
}

auto main() -> int
{
    // Punteros
    std::println("-- Punteros. --\n");

    // double* q = nullptr; // Puntero nulo, IMPORTANTE! no se debe desreferencia

    auto n = int{1};
    // int* p = &n;
    incremento(&n);
    
    // int* p = &n; // * indica que es un puntero & operador que indica direcció
    //++*p; // "DESREFERENCIAR" un puntero.

    std::println("{}", n);

    // auto s = Student{
    //     "James Kerry", // todo esto esta dentro de la pila user stack
    //     6,
    //     8,
    //     9.5
    // };

    // En otro lewnguaje se podría crear un objeto en el free store
    // Pero ya no esa una variable Student
    // En el user stack voy a tener un puntero apuntando al user store Sutdent*
    // Este objeto sigue vivo en Free store, se queda ahí.
    // Si el programa es de ejecución permanente, se ocupa la memoria peligrosamente
    // Student* p = new Student{
    // auto p = new Student{
    // auto p = std::make_unioque<Student>(){ // puntero inteligent, unico o de propiedad exclusiva.
    // auto p = std::make_shared<Student>(){ // puntero compartido.
    //     "James Kerry", // todo esto esta dentro de la pila user stack
    //     6,
    //     8,
    //     9.5
    // };

    // Esto va a devovler un puntero, de la dirección de memoria del primero.
    // Esta diciendo donde empieza el primer elemento del array.
    Student* p = new Student[100]{}; // Crea un array de 100 studiantes inicializados a vacio y 0.

    // Aritmética de punteros
    // Para ir al 4 valor del array
    // hay que dar 3 saltos
    // Dirección en memoria del 4 objeto.
    // El compilador sabe cuanto ocupa el objeto.
    
    // (*(p + 3)).grade_1 = 9.8;
    // std::println("{:.1f}", (*(p+3)).avarage());

    // (p + 3)->grade_1 = 9.8;
    // std::println("{:.1f}", (p+3)->avarage());

    p[3].grade_1 = 9.8; // Esto no tiene bound checking cvontrol de limite
    // EL no tener un bound cheking puede hacer que se corrompa la memoria.
    // Utilizar en precompilaicón detectores.
    // Usar abstracciones de array que tengan o no bound cheking.
    // Usaremos vectores u otros contenedores para trabajr
    std::println("{:.1f}", p[3].avarage());


    // Student* p = &s; // Puntero que apunta a la clase s

    // std::println("{:.1f}", s.avarage());

    // std::println("{:.1f}", s.grade_1); // Objeto original operador punto
    // std::println("{:.1f}", (*p).grade_1); 
    // std::println("{:.1f}", p->grade_1); // -> operdor flecha da el salto de el puntero a la variable original
    // std::println("{:.1f}", p->avarage());

    // Llama al destructor de la clase student y se libera la memoria.
    // El puntero sigue viviendo pero no apunta a la memoria.
    // En otros lenguajes el garbage collector detecta que no hay referencia y elimina el objeto y libera la memoria.
    // No se controla bien cuando muere ese objeto.
    // delete p; // Esto suele ser mala praxis
    // Que se vea un new y un delete es "smell".
    // Con los puntero unico no hace falta el Delete, Cuando se acabe el ambito se destruye.

    delete[] p; // Necvesitamos poner los [] para indicar que hay que destruir todos los objetos del array.

    return EXIT_SUCCESS;
}