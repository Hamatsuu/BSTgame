#ifndef __NODOT__
#define __NODOT__

template < typename X > struct nodoT
{
   X dato;
   nodoT < X > *izq, *der, *padre;

   nodoT ()
   {
      izq = der = padre = nullptr;
   }
   nodoT (const X & d)
   {
      dato = d;
      izq = der = padre = nullptr;
   }
};

#endif
