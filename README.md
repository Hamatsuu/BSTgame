# BSTgame
Jueguito de BST para Estructura de Datos

---

```
# BST Library — Function Reference

## Constructors

### BST()
```

```cpp
BST();
```

```
Accepts: nothing
Does: creates an empty BST, with raiz = nullptr and no trash tree.
Returns: nothing

### BST(BST<X> *T)
```

```cpp
BST(BST<X> *T);
```

```
Accepts: pointer to another BST<X> used as a trash tree.
Does: creates an empty BST but assigns trash = T.
Returns: nothing

### BST(const BST<X> &L)
```

```cpp
BST(const BST<X> &L);
```

```
Accepts: another BST (by const reference).
Does: builds a copy of another BST.
Returns: nothing

### ~BST()
```

```cpp
~BST();
```

```
Accepts: nothing
Does: destroys the BST by deleting all nodes.
Returns: nothing

## Copying

### void copia(const BST<X> &l)
```

```cpp
void copia(const BST<X> &l);
```

```
Accepts: another BST
Does: clears current tree and copies all nodes from l.
Returns: nothing

### _copia(nodoT<X> *r)
```

```cpp
void _copia(nodoT<X> *r);
```

```
Accepts: root of tree to copy
Does: recursively copies nodes.
Returns: nothing

### Assignment Operator
```

```cpp
BST<X> &operator=(const BST<X> &l);
```

```
Accepts: another BST
Does: makes *this a copy of l.
Returns: reference to *this

## Deletion

### void borraArbol()
```

```cpp
void borraArbol();
```

```
Does: deletes the entire tree.
Returns: nothing

### _borraArbol(nodoT<X> *r)
```

```cpp
void _borraArbol(nodoT<X> *r);
```

```
Does: internal recursive deletion.
Returns: nothing

### void borraNodo(nodoT<X> *n)
```

```cpp
void borraNodo(nodoT<X> *n);
```

```
Does: removes a node and places it in trash or deletes it.
Returns: nothing

## Node Creation / Insertion

### creaNodo(const X &d)
```

```cpp
nodoT<X> *creaNodo(const X &d);
```

```
Accepts: a value
Does: creates a node, may reuse trash.
Returns: pointer to new node

### inserta(nodoT<X> *n)
```

```cpp
void inserta(nodoT<X> *n);
```

```
Accepts: a node pointer
Does: inserts the node into the BST.
Returns: nothing

### inserta(X &d)
```

```cpp
void inserta(X &d);
```

```
Accepts: a value
Does: creates a node and inserts it.
Returns: nothing

## Search

### busca(X &val)
```

```cpp
nodoT<X> *busca(X &val);
```

```
Accepts: value
Does: searches for a matching node.
Returns: pointer or nullptr

## Traversals

### inorder()
```

```cpp
void inorder();
```

```
Order: Left → Root → Right

### inorderInverso()
```

```cpp
void inorderInverso();
```

```
Order: Right → Root → Left

### preorder()
```

```cpp
void preorder();
```

```
Order: Root → Left → Right

### posorder()
```

```cpp
void posorder();
```

```
Order: Left → Right → Root

## Min / Max

### min()
```

```cpp
nodoT<X> *min();
```

```
Returns the smallest node.

### max()
```

```cpp
nodoT<X> *max();
```

```
Returns the largest node.

### min(nodoT<X> *apu)
```

```cpp
nodoT<X> *min(nodoT<X> *apu);
```

### max(nodoT<X> *apu)

````
```cpp
nodoT<X> *max(nodoT<X> *apu);
````

### Recursive versions

````
```cpp
nodoT<X> *minR();
nodoT<X> *_minR(nodoT<X> *r);

nodoT<X> *maxR();
nodoT<X> *_maxR(nodoT<X> *r);
````

## Successor / Predecessor

### sucesor(nodoT<X> *apu)

````
```cpp
nodoT<X> *sucesor(nodoT<X> *apu);
````

```
Finds next larger node.

### predecesor(nodoT<X> *apu)
```

```cpp
nodoT<X> *predecesor(nodoT<X> *apu);
```

```
Finds next smaller node.

## Iterative Traversal

### recorridoIterativo()
```

```cpp
void recorridoIterativo();
```

```
Iterates by successive successors.

### recorridoIterativoInverso()
```

```cpp
void recorridoIterativoInverso();
```

```

## Node Extraction

### extraeNodo(nodoT<X> *z)
```

```cpp
nodoT<X> *extraeNodo(nodoT<X> *z);
```

```
Removes a node and fixes tree structure.
Returns detached node.

## Output Operator

### operator<<
```

```cpp
std::ostream &operator<<(std::ostream &s, BST<X> &t);
```

```
Prints BST in-order.
```

---

If you want, I can also generate:

✅ A fully clean `README.md`
✅ ASCII diagrams of BST operations
✅ Documentation for nodoT.h
✅ A minimal wiki-style docs structure

Just tell me.
