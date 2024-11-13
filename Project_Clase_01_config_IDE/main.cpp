#include <iostream>
#include <print>
#include <string>
#include <vector>

// auto do_something() -> void
void do_something() // ies lo mismo que arriba
{
    // auto g1 = double{14.7};
    // auto g2 = char{'z'};
}

class S {
    // public private protected
    private:
        int id;

    public: // constructor  (puede haber más de uno) crea en memoria
        S(int n)
            : id{n} // lista de inicialización
        {
            std::println("Se ha creado el objeto #{}", id);
        }
    
    ~S() // Destructor solo puede haber uno. Tiene el nombre de la clase. No tiene argumentos. Lleva la ~delante. Libera la memoria.
    {
        std::println("Se va a destruir el objeto #{}", id);
    }

}; // Las declaraciones de las clases tienen que acabar en ;

auto main() -> int
{ //  comienza ámbito de la función.
    //std::println("Hello world!");
    // auto f = float{8.7f}; // variable float
    // auto d = double{7.9}; // variable double
    // auto c = char{'a'}; // variable char, ocupa 1 byte.

    // do_something();

    // std::println("f = {}, d = {}, c = {}", f, d, c ); // Mejor que print_f seguridad de tipos
    // std::println("size of f = {} bytes, d = {} bytes, c = {} bytes", sizeof(f), sizeof(d), sizeof(c)); // Mejor que print_f seguridad de tipos

    // auto s1 = S{1};
    // { // creo un ambito para forzar las desapariciones
    //     auto s2 = S{2};
    //     auto s3 = S{3};
    // }
    // auto s4 = S{4};

    auto names = std::vector<std::string>{};// listado de nuombres de contactos no sabemos el tamaño... Es un vector standar. Es consecutivo en memoria.
    // La diferencia con ArrayList es que en ArrayList son todos referencias a otras casillas de momoria. Mejora el uso de memoria usar vectores.
    // el vector vive en el User stack y tiene un grado de indirección que busca en el free store.

    auto nm = std::string{};
    while (std::getline(std::cin, nm)) { // cin obejto global de iostream, invocacion a una funcion libre que se llama getline
        names.push_back(nm);
    }

    for (std::string n : names) {
        std::println("{}", n);
    }

    return EXIT_SUCCESS;
} // Al final del ámbito las variables son destruidas
// en orden contrario a como fueron inicializdas (1º c, 2º d, 3º f).
// Desenredo de la pila.