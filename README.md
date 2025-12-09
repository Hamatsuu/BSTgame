# Functions and stuff


```cpp
// Check if mouse click is inside the circle of a node
bool isNodeClicked(nodoT<visData>* n) {
    ...
    Vector2 m = GetMousePosition();
    float dx = m.x - n->dato.x;
    float dy = m.y - n->dato.y;
    return (dx*dx + dy*dy) <= (nodeRadius * nodeRadius);
}
```

---

```cpp
// Recursively search for clicked node
nodoT<visData>* findClickedNode(nodoT<visData>* n) {
    ...
    if (isNodeClicked(n)) 
        return n;

    nodoT<visData>* left = findClickedNode(n->izq);
    if (left) return left;

    nodoT<visData>* right = findClickedNode(n->der);
    return right;
}
```

---

```cpp
// Recursively calculate positions for the BST
void computePosition(nodoT <visData> *n, float x, float y, float sep)
{
    if (!n) return;

    n->dato.x = x;
    n->dato.y = y;

    computePosition(n->izq, x - sep, y + 80, sep / 1.7 );
    computePosition(n->der, x + sep, y + 80, sep / 1.7 );
}

```

---

```cpp
void drawTree(nodoT <visData> *n) 
{
    if (!n) return;

    // Draw edges first
    if (n->izq)
        DrawLine(...);
    if (n->der)
        DrawLine(...);

    drawTree(n->izq);
    drawTree(n->der);

    // Draw the node
    DrawCircleV(...);
    DrawCircleLines(...);
    DrawText(...);
}
```

---

Inventory is stored as a global `listaS<nodoT<visData>*>`
```cpp
listaS<nodoT<visData>*> inventory;
int selectedSlot = -1;
```

```cpp
inventory.pushBack(this->extraeNodo(clicked));
```

---

```cpp
void drawInventory() {
    ...
    nodoS<nodoT<visData>*> *p = inventory.raiz;
    int i = 0;
    while (p) 
    {
        DrawCircleV(...); // slot
        DrawText(...);    // value

        p = p->sig;
        i++;
    }
}
```

---

```cpp
// Highlight selected inv slot
for (int k = 0; k < 9; k++) 
{
    if (IsKeyPressed(KEY_ONE + k)) 
    {
        if (k < (int)inventory.size())
            selectedSlot = k;
        else
            selectedSlot = -1;
    }
}
```

---

```cpp
// Extract a node
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
{
    nodoT<visData>* clicked = findClickedNode(this->raiz);
    if (clicked) {
        PlaySound(sfx_extract);
        inventory.pushBack(this->extraeNodo(clicked));
    }
}
```

---

```cpp
// Insert a node
if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) 
{
    if (clicked == this->raiz && selectedSlot != -1) 
    {
        nodoS<nodoT<visData>*> *slot = inventory.Extrae(...);
        this->inserta(slot->dato); // and reset the highlighted slot
    }
}
```

---

```cpp
while (!WindowShouldClose()) {

    UpdateMusicStream(bgm);

    // Input
    handle mouse extraction/insertion
    handle hotkeys

    // Logic
    update();

    // Rendering
    render();
}
```
