#ifndef __LISTAS__
#define __LISTAS__

#include <nodoS.h>
#include <iostream>

template < typename X > struct listaS
{
   nodoS < X > *raiz;
   listaS < X > *trash;         //trash -> basura en ingles.
   unsigned N;

     listaS (listaS < X > *T = nullptr)
   {
      raiz = nullptr;
      trash = T;
      N = 0;
   }

   listaS (const listaS < X > &L)
   {
      raiz = nullptr;
      trash = nullptr;
      N = 0;
      *this = L;
   }

   ~listaS ()
   {
      borraLista ();
   }

   void copiaR (const listaS < X > &l)
   {
      if (raiz != nullptr)
         borraLista ();
      _copiaR (l.raiz);
      N = l.N;
      trash = l.trash;
   }

   void _copiaR (nodoS < X > *r)
   {
      if (r == nullptr)
         return;
      _copiaR (r->sig);
      pushFront (r->dato);
   }

   void copia (const listaS < X > &l)
   {
      nodoS < X > *apuf, *apud, *n;


      if (raiz != nullptr)
         borraLista ();

      if (l.raiz == nullptr)
         return;

      // Copiamos el primer nodo
      raiz = creaNodo (l.raiz->dato);

      apuf = l.raiz->sig;
      apud = raiz;
      while (apuf != nullptr)
      {
         n = creaNodo (apuf->dato);
         apud->sig = n;
         apuf = apuf->sig;
         apud = apud->sig;
      }

      N = l.N;
      trash = l.trash;
   }

   listaS < X > &operator = (const listaS < X > &l)
   {
      copia (l);
      return *this;
   }

   void borraLista ()
   {
      while (raiz != nullptr)
      {
         nodoS < X > *n;

         n = raiz;
         raiz = raiz->sig;
         borraNodo (n);
      }
      N = 0;
   }

   unsigned size ()
   {
      return N;
   }

   nodoS < X > *creaNodo (const X & d)
   {
      nodoS < X > *n;

      if (trash != nullptr && trash->raiz != nullptr)
      {
         n = trash->raiz;
         trash->raiz = trash->raiz->sig;
         trash->N--;
         n->sig = nullptr;
         n->dato = d;
      }
      else
         n = new nodoS < X > (d);
      return n;
   }

   void borraNodo (nodoS < X > *n)
   {
      if (trash != nullptr)
      {                         //metodos el nodo n al inicio de la raiz del basurero
         n->sig = trash->raiz;
         trash->raiz = n;
      }
      else
         delete n;
   }

   void pushFront (const X & d)
   {
      nodoS < X > *n;

      n = creaNodo (d);
      if (n != nullptr)               //equiv a if (n)
      {
         n->sig = raiz;
         raiz = n;
         N++;
      }
   }

   void pushFront (nodoS < X > &n)
   {
      if (n != nullptr)               //equiv a if (n)
      {
         n->sig = raiz;
         raiz = n;
         N++;
      }
   }

   void pushBack (const X & d)
   {
      nodoS < X > *n;

      n = creaNodo (d);
      if (n != nullptr)
      {
         if (raiz != nullptr)
         {
            nodoS < X > *apu = raiz;

            while (apu->sig != nullptr)
               apu = apu->sig;
            apu->sig = n;
         }
         else
            raiz = n;
         N++;
      }
   }

   void pushBack (nodoS < X > *n)
   {
      if (n != nullptr)               //equiv a if (n)
      {
         if (raiz != nullptr)
         {
            nodoS < X > *apu = raiz;

            while (apu->sig != nullptr)
               apu = apu->sig;
            apu->sig = n;
         }
         else
            raiz = n;
         N++;
      }
   }

   void pushBackR1 (const X & d)
   {
      if (raiz == nullptr)
      {
         nodoS < X > *n;

         n = new nodoS < X > (d);
         raiz = n;
         N++;
      }
      else
         _pushBackR1 (raiz, d);
   }

   void _pushBackR1 (nodoS < X > *r, const X & d)
   {
      if (r->sig != nullptr)
         _pushBackR1 (r->sig, d);
      else
      {
         nodoS < X > *n;

         n = new nodoS < X > (d);
         r->sig = n;
         N++;
      }
   }

   void pushBackR2 (const X & d)
   {
      if (raiz == nullptr)
      {
         nodoS < X > *n;

         n = new nodoS < X > (d);
         raiz = n;
         N++;
      }
      else
         raiz = _pushBackR2 (raiz, d);
   }

   nodoS < X > *_pushBackR2 (nodoS < X > *r, const X & d)
   {
      if (r == nullptr)
      {
         nodoS < X > *n;

         n = new nodoS < X > (d);
         N++;
         return n;
      }
      else
      {
         r->sig = _pushBackR2 (r->sig, d);
         return r;
      }
   }

   //Contribución del Equipo 3:
   //Omar Herrera y Alexis Martin.
   nodoS < X > *busca (const X & val)
   {
      nodoS < X > *actual = raiz;

      while (actual != nullptr)
      {
         if (actual->dato == val)
            return actual;
         actual = actual->sig;
      }
      return nullptr;                 // puntero nulo
   }

   //Contribución del Equipo 7:
   //Elizabeth Aban, Erick Muñoz y Majo Pech
   void Inserta (nodoS < X > *p, nodoS < X > *n)
   {
      if (n == nullptr)
         return;
      if (p == nullptr)
      {
         std::cerr << "Error en metodo listaS<X>::inserta :"
            << "posición a insertar nula." << std::endl;
         return;
      }
      n->sig = nullptr;
      /*
         Este fragmento de código hace que el método opere como un
         pushFront en caso de que p==0. Eso va más alla de la
         especificación del método por lo que los comentamos.
         if (p == nullptr) 
         {
         n->sig = raiz;
         raiz = n;
         ++N;
         return;
         } */

      n->sig = p->sig;
      p->sig = n;
      ++N;
   }

   //Contribución del Equipo 5:
   //Daniel Herrera y Renata Borreguín
   //Ellos además implementaron la función buscaPos
   //pero no la incluyo aquí porque no les tocó presentarla.
   X & operator[](int idx)
   {
      nodoS < X > *apu = buscaPos (idx);
      return apu->dato;
   }

   //Contribución del Equipo 4:
   //Eliberto Gutierrez y Ryou Hau
   nodoS < X > *Extrae (nodoS < X > *p)
   {
      if (raiz == nullptr || p == nullptr)
         return nullptr;

      if (raiz == p)
      {
         raiz = raiz->sig;
         p->sig = nullptr;
         N--;
         return p;
      }

      nodoS < X > *antes = raiz;
      while (antes->sig != nullptr && antes->sig != p)
         antes = antes->sig;

      if (antes->sig == p)
      {
         antes->sig = p->sig;
         p->sig = nullptr;
         N--;
         return p;
      }

      return nullptr;
   }

   // Contribucion del Equipo 2:
   // José Luis López, Andros May y Gael Ordoñez
   nodoS < X > *buscaPos (int idx)
   {
      // Originalmente tenia (raiz ==0 || N == 0)
      // Pero es redundante esa comparación.
      if (raiz == nullptr)
         return nullptr;

      if (idx < 0)
         idx = N + idx;

      if (idx < 0 || idx >= (int) N)
         return nullptr;

      nodoS < X > *actual = raiz;

      for (int i = 0; i < idx; i++)
         actual = actual->sig;

      return actual;
   }

   //Contribución del equipo 6:
   //Daniel Corona, Luis Lara y Adrian Vazquez
   void invierte ()
   {
      nodoS < X > *ap, *tmp, *prev;

      // Faltó validar el caso en que la lista esta vacia.
      // Añadí la siguiente linea:
      if (!raiz)
         return;

      prev = nullptr;
      ap = raiz;

      while (ap->sig != nullptr)
      {
         tmp = ap->sig;
         ap->sig = prev;
         prev = ap;
         ap = tmp;
      }

      ap->sig = prev;
      raiz = ap;

      //return; ¿porque este return?
   }

   //Contribución del equipo 8:
   //Juan Fernando Cauich, Benjamin Cime y Alan Nava.
   void concatena (listaS < X > &L)
   {
      if (&L == this)
      {
         std::cerr << "listaS: No se puede concatenar una lista a sí misma." <<
            std::endl;
         return;
      }
      if (L.raiz == nullptr)
         return;

      nodoS < X > *ptr = raiz;
      nodoS < X > *last_ptr = nullptr;

      while (ptr)
      {
         last_ptr = ptr;
         ptr = ptr->sig;
      }
      last_ptr->sig = L.raiz;
      L.raiz = nullptr;

      N += L.N;
      L.N = 0;
   }

   bool operator == (const listaS<X> &L)
   {
      bool equal = true;
      nodoS<X> *apu, *apul;

      if (L.N != N)
         return false;

      apu = raiz;
      apul = L.raiz;
      while(apu != nullptr)
      {
         if (apu->dato != apul->dato)
            return false;
         apu = apu->sig;
         apul = apul->sig;
      }
      return true;
   }
};

template < typename X >
   std::ostream & operator << (std::ostream & s, listaS < X > &l)
{
   nodoS < X > *apu;

   s << "raiz->";
   apu = l.raiz;
   while (apu != nullptr)
   {
      s << "[" << apu->dato << "]->";
      apu = apu->sig;
   }
   s << "NULL";

   return s;
}
#endif
