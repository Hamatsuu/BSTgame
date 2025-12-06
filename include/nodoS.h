#ifndef __NODOS__
#define __NODOS__

template < typename X > struct nodoS
{
   X dato;
   nodoS < X > *sig;

   nodoS ()
   {
      sig = nullptr;
   }
   nodoS (const X & d)
   {
      dato = d;
      sig = nullptr;
   }
};

#endif
