#include <raylib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <BST.h>

using namespace std;

struct visData
{
    int val;
    float x, y;
    float theta, mag;
    int nivel;
    int size;

    visData ()
    {
        val = 0;
        x = y = 0;
        theta = mag = 0;
        size = 1;
        nivel = 0;
    }

    visData (int d)
    {
        val = d;
        x = y = 0;
        theta = mag = 0;
        nivel = 0;
        size = 1;
    }
    int operator () ()
    {
        return val;
    }
    bool operator < (visData &v)
    {
        if (val < v.val)
            return true;
        return false;
    }
};

ostream & operator << (ostream & s, visData &v)
{
    s << "[" << v.val << ", (" << v.x << ", " << v.y << "),"
        << " (" << v.mag << "," << v.theta << ") | nv= "
        << v.nivel << ", sz= " << v.size << "]";
    return s;
}


void BGM ()
{
    

}




struct arBonito: public BST< visData >
{
    int ancho, alto;
    unsigned int nNiveles;
    int nodeRadius;
    float cX, cY;

    arBonito():BST< visData >()
    {
        ancho = 800;
        alto = 600;
        nNiveles = 0;
        nodeRadius = 30;
        cX = ancho / 2;
        cY = alto / 2;
    }

    arBonito (int _ancho, int _alto, BST< visData > *T = nullptr):BST< visData >(T)
    {
        ancho = _ancho;
        alto = _alto;
        nNiveles = 0;
        nodeRadius = 30;
        cX = ancho / 2;
        cY = alto / 2;
    }

    ~arBonito ()
    {
        CloseWindow ();
    }
    // Check if mouse click is inside the circle of a node
    bool isNodeClicked(nodoT<visData>* n) {
        if (!n) return false;
        Vector2 m = GetMousePosition();
        float dx = m.x - n->dato.x;
        float dy = m.y - n->dato.y;
        return (dx*dx + dy*dy) <= (nodeRadius * nodeRadius);
    }

    // Recursively search for clicked node
    nodoT<visData>* findClickedNode(nodoT<visData>* n) {
        if (!n) return nullptr;

        if (isNodeClicked(n)) 
            return n;

        nodoT<visData>* left = findClickedNode(n->izq);
        if (left) return left;

        nodoT<visData>* right = findClickedNode(n->der);
        return right;
    }

    void computePosition(nodoT <visData> *n, float x, float y, float sep)
    {
        if (!n) return;

        n->dato.x = x;
        n->dato.y = y;

        computePosition(n->izq, x - sep, y + 80, sep / 1.7 );
        computePosition(n->der, x + sep, y + 80, sep / 1.7 );
    }


    void drawTree(nodoT <visData> *n) {
        if (!n) return;

        // Draw edges first
        if (n->izq) 
            DrawLine(n->dato.x, n->dato.y, n->izq->dato.x, n->izq->dato.y, BLACK);
        if (n->der) 
            DrawLine(n->dato.x, n->dato.y, n->der->dato.x, n->der->dato.y, BLACK);

        drawTree(n->izq);
        drawTree(n->der);

        // Draw node
        DrawCircleV({n->dato.x, n->dato.y}, nodeRadius, SKYBLUE);
        DrawCircleLines(n->dato.x, n->dato.y, nodeRadius, DARKBLUE);

        DrawText(
            to_string(n->dato.val).c_str(),
            n->dato.x - 10,
            n->dato.y - 10,
            20,
            BLACK
        );
    }

    void update()
    {
        computePosition(this->raiz, ancho/2, 80,300);
    }

    void render()
    {
        BeginDrawing ();

        ClearBackground (RAYWHITE);
        drawTree(this->raiz);

        EndDrawing ();
    }

    void Loop()
    {
        InitWindow (ancho, alto, "ArBonito");
        SetTargetFPS (60);

        // Main loop
        while (!WindowShouldClose ()) // Detect window close button or ESC key
        {

            // --- CLICK TO DELETE NODE ---
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
            {
                nodoT<visData>* clicked = findClickedNode(this->raiz);
                if (clicked) 
                {
                    cout << "Deleting node: " << clicked->dato.val << endl;
                    this->extraeNodo(clicked);   // delete by value
                }
            }

            //Update
            update ();

            //Draw;
            render ();
        }
    }
};

int main (int argc, char **argv)
{
    int i, N = 16;
    long semilla = 0;
    int ancho = 1280, alto = 1024;
    BST<visData> Basura;
    arBonito V(ancho, alto, &Basura);

    if (argc > 1)
        N = atoi (argv[1]);
    if (argc > 2)
        semilla = atol (argv[2]);

    cout << endl
        << "Se inicializó el generador de número aleatorios con "
        << semilla << endl << endl;

    //Llenamos el árbol
    srand48 (semilla);

    for (i = 0; i < N; ++i)
    {
        visData val ((int) (lrand48 () % 1000));

        V.inserta (val);
        cout << "Insertamos el valor " << val << " al árbol." << endl;
    }
    cout << endl;

    V.Loop ();
    //hola git helo mundo
    return 0;
}
