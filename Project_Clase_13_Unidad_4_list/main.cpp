#include <cstdlib>    // contiene EXIT_SUCCESS y EXIT_FAILURE
#include <print>
#include <string>

struct Node_base { // Caja solo que contiene prev y next. Si los datos.
   // Node_base* es un puntero a un nodo base.
   // La dirección en memoria a un nodo base.
   Node_base* prev; // Ubicación en memoria del node_base anterior.
   Node_base* next; // Ubicación en memoria del node_base siguiente.

   // auto hook(Nodo_base* p) -> void
   // Colgar, enganchar.
   void hook(Node_base* p) // ubicar el nodo que invoque a esta funció, antes del nodo apuntado por p
   {
      // this es un nodo base. Es un puntero a un nodo base. Tenemos que desreferenciarlo.
      // (*this). es equivalente a this->
      // this->next = p; // Porque this se puede simplificar por el lenguaje.
      next = p;
      //this->prev = p->prev; // Porque this se puede simplificar por el lenguaje.
      prev = p->prev;
      (p->prev)->next = this;
      p->prev = this;
   }

   // Descolgar, desenganchar.
   // EL nodo se queda fuera de la lista.
   // El nodo se apunta a el mismo. Se autoreferencia.
   void unhook()
   {
      // (this->prev)->next = this->next; // Porque this se puede simplificar por el lenguaje.
      prev->next = this->next;
      // (this->next)->prev = this->prev; // Porque this se puede simplificar por el lenguaje.
      next->prev = this->prev;
      // this->next = this; // Porque this se puede simplificar por el lenguaje.
      next = this;
      // this->prev = this; // Porque this se puede simplificar por el lenguaje.
      prev = this;
   }

   // Cambiar de lugar. Intercambio.
   // Depende de si el nodo está integrado o no en una lista.
   void swap(Node_base& p)
   {
      // ... está en la lectura del tema 4.
   }
};

template< typename T> // Familia uniparamétrica de clases.
class List {
   // La interfaz privada...
   // struct Node : public Nodo_base { // Herencia. Node es un nodo base.
   struct Node : Node_base { // Nodo que herada publicamwente de node_base.
      T dat; // El valor que contiene el nodo. Dato a almacenar.
      Node(T const& d) : dat{d} {} // Constructor de la clase Node.
      // Node(T const& d) : prev{nullptr}, next{nullptr}, dat{d} {} // Constructor de la clase Node.
   };

   Node_base dnb_; // Nodo centinela. // La barra baja es un sufijo que indica que es un miembro de la clase.
   public: // Aquí empieza la interfaz pública. Modificador de acceso.

      // constructor...

      void push_back(T const& val) // Insertar al final.
      {
         Node* p = new Node{val}; // Crear un nodo.
         // Le indicamos a p que se ubique antes del nodo centinela.
         p->hook(&dnb_); // Colgar el nodo al final. Se situa antes del nodo centinerla que es el ultimo.
         // Esto no puede fallar. Es una redefinición de punteros.
         // Tiene garantía fuerte ante excepciones.
      }

      // Conocer el num total de nodos de una lista.
      auto size() const -> std::size_t
      {
         auto sz = std::size_t{0}; // Tamaño de la lista.
         auto p = dnb_.next; // Puntero al primer nodo. // Como es circular el camino es el siguiente del centinela.
         while (p != &dnb_) { // Mientras no llegue al centinela.
            ++sz; // Incrementar el tamaño.
            p = p->next; // Avanzar al siguiente nodo.
         }
         return sz;
      }
}

auto main() -> int
{
   // Lista doblemente enlazada.
   // Donde cada nodo conoce al siguiente y al anterior.
   // Y puedo ir y volver.
   // En el User stack es donde están las variable locales de la función.
   // Cuando creo una lista local

   auto nums = List<int>{}; // Lista vacía de tipos geneŕicos.
   num.push_back(1);
   num.push_back(2);
   num.push_back(3);
   num.push_back(4);

   return EXIT_SUCCESS;
}