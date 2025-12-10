#include <raylib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <BST.h>
#include <listaS.h>
#include <string>

using namespace std;

struct visData
{
    int val;
    float x, y; float theta, mag;
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


bool won = false;
listaS<nodoT<visData>*> inventory;
int selectedSlot = -1; 
const int MAX_SLOTS = 4;

//camera offset for dragging
Vector2 cameraOffset = {600, 100}; //punto inicial también
Vector2 lastMousePos = {0, 0};
bool dragging = false;

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

    ~arBonito (){
        CloseWindow();
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

    void assignInorderX(nodoT<visData>* n, int &currentX, float spacing)
    { // si usabamos la del profe era cambiar su func de BST.h
        if (!n) return;

        assignInorderX(n->izq, currentX, spacing);

        n->dato.x = currentX * spacing;   // evenly spaced horizontally
        currentX++;

        assignInorderX(n->der, currentX, spacing);
    }

    void assignY(nodoT<visData>* n, int depth)
    {
        if (!n) return;

        n->dato.y = 80 * depth;

        assignY(n->izq, depth + 1);
        assignY(n->der, depth + 1);
    }

    void applyCameraOffset(nodoT<visData>* n)
    {
        if (!n) return;

        n->dato.x += cameraOffset.x;
        n->dato.y += cameraOffset.y;

        applyCameraOffset(n->izq);
        applyCameraOffset(n->der);
    }

    void computeLayout()
    {
        int xIndex = 0;

        assignInorderX(raiz, xIndex, 32);
        assignY(raiz, 0);

        applyCameraOffset(raiz);
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
        if (this->raiz == n)
        {
            DrawCircleV({n->dato.x, n->dato.y}, nodeRadius, GREEN);
        } else {
            DrawCircleV({n->dato.x, n->dato.y}, nodeRadius, SKYBLUE);
        }

        DrawCircleLines(n->dato.x, n->dato.y, nodeRadius, DARKBLUE);

        string s = to_string(n->dato.val);
        int fontSize = 20;
        int textWidth = MeasureText(s.c_str(), fontSize);
        int textHeight = fontSize; // approximation
        DrawText(
            s.c_str(),
            n->dato.x - textWidth / 2,
            n->dato.y - textHeight / 2,
            fontSize,
            BLACK
        );
    }

    void drawInventory()
    {
        const int startX = ancho - 80;  // locked a ancho - offset 
        const int startY = 60;
        const int spacing = 60;
        const int invRadius = 22;

        nodoS<nodoT<visData>*> *p = inventory.raiz;

        int i = 0; // keep track of selected slot

        DrawText("Inventory:", startX - 50, 10, 20, DARKGRAY);

        while (p) // break p->sig = nullptr
        {
            nodoT<visData>* n = p->dato;

            // Not using visData x y, since its a simply linked list.
            // keeping track of each nodes pos and updating seemed cumbersome
            float x = startX;
            float y = startY + i * spacing;

            // Draw node 
            if (selectedSlot == i) 
            {
                DrawCircleV({x, y}, invRadius, GOLD);

            } else 
        {
                DrawCircleV({x, y}, invRadius, LIGHTGRAY);

            }
            DrawCircleLines(x, y, invRadius, DARKGRAY);

            // Node value 

            string s = to_string(n->dato.val);
            int fontSize = 20;
            int textWidth = MeasureText(s.c_str(), fontSize);
            int textHeight = fontSize; // approximation
            DrawText(
                s.c_str(),
                x - textWidth / 2,
                y - textHeight / 2,
                fontSize,
                BLACK
            );
            i++;
            p = p->sig;
        }

    }

    void update()
    {
        //computePositions(this->raiz, ancho/2, 50, 40);
        computeLayout();

        // Inventory hotkeys (1–9)
        for (int k = 0; k < 9; k++)
        {
            if (IsKeyPressed(KEY_ONE + k))  // KEY_ONE..KEY_NINE are consecutive
            {
                // Valid slots check
                if (k < (int)inventory.size())   
                    selectedSlot = k;
                else
                    selectedSlot = -1; // invalid slot
            }
        }
    }

    void render()
    {
        BeginDrawing ();

        ClearBackground (RAYWHITE);
        drawTree(this->raiz);
        drawInventory();

        // So it appears on top of the tree
        if (won)
            {
                string win = "You won!";
                string win2 = "You managed to balance the tree properly";
                string clue = "Press ESC to exit";
                int fontSize = 80;
                int textWidth = MeasureText(win.c_str(), fontSize);
                int textWidth2 = MeasureText(win2.c_str(), fontSize-25);
                int textWidth3 = MeasureText(clue.c_str(), fontSize-50);
                int textHeight = fontSize; // approximation
                DrawText(
                    win.c_str(),
                    ancho/2 - textWidth / 2,
                    400 - textHeight / 2,
                    fontSize,
                    GOLD 
                );
                DrawText(
                    win2.c_str(),
                    ancho/2 - textWidth2 / 2,
                    500 - (textHeight-25) / 2,
                    fontSize-25,
                    GOLD 
                );
                DrawText(
                    clue.c_str(),
                    ancho/2 - textWidth3 / 2,
                    alto - (textHeight-20) / 2,
                    fontSize-50,
                    GOLD 
                );
            }

        EndDrawing ();
    }

    void Loop()
    {
        //Load BGM start
        Music bgm =LoadMusicStream("assets/tracks/main_theme.mp3"); //Balatro bgm

        Sound sfx_extract = LoadSound("assets/sfx/cardSlide1.ogg");
        Sound sfx_insert = LoadSound("assets/sfx/chips2.ogg");

        //Modify volume
        SetSoundVolume(sfx_extract, 0.5);
        SetSoundVolume(sfx_insert, 0.5);

        PlayMusicStream(bgm);

        float timePlayed = 0.0f;
        //Load BGM end

        //this->raiz->dato.x = 5000;
        // this->raiz->dato.y = 50;

        // Main loop
        while (!WindowShouldClose ()) // Detect window close button or ESC key
        {
                        //BGM update start
            UpdateMusicStream(bgm);   // Update music buffer with new stream data

            timePlayed = GetMusicTimePlayed(bgm)/GetMusicTimeLength(bgm);

            if (timePlayed > 1.0f) timePlayed = 1.0f;

            //BGM update end

            // Click izq for removing the node from the BST 
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && inventory.size() <= MAX_SLOTS - 1) 
            {

                nodoT<visData>* clicked = findClickedNode(this->raiz);
                if (clicked) 
                {
                    PlaySound(sfx_extract);
                    cout << "Push node to inventory: " << clicked->dato.val << endl;
                    inventory.pushBack(this->extraeNodo(clicked));   // insertar en el inventario

                    computeLayout();

                } else 
            {
                    dragging = true;
                    lastMousePos = GetMousePosition();
                }
            }

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                dragging = false;
            }

            if (dragging) 
            {
                Vector2 mouse = GetMousePosition();
                Vector2 delta = { mouse.x - lastMousePos.x, mouse.y - lastMousePos.y };

                cameraOffset.x += delta.x;
                cameraOffset.y += delta.y;

                lastMousePos = mouse;
            }

            // Click der for putting it back in the root
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) 
            {
                nodoT<visData>* clicked = findClickedNode(this->raiz);
                if (clicked == this->raiz && selectedSlot != -1) 
                {
                    PlaySound(sfx_insert);
                    nodoS <nodoT <visData>*> *apu = inventory.Extrae(inventory.buscaPos(selectedSlot)); // sus
                    cout << "Insert node to root node: " << apu->dato->dato.val << endl;
                    this->inserta(apu->dato);   // insertar a la raiz 
                    selectedSlot = -1; // segfault sino

                    computeLayout();

                    if (inventory.size() == 0) 
                    {
                        cout << "El arbol es balanceado y/n: " << balanceado(this->raiz) << endl;
                        if (balanceado(this->raiz))
                        {
                            cout << "GANASTE" << endl;
                            won = true;
                        }
                    }

                }
            }

            update ();

            render ();

        }
    }
};

//We take it all out of the main to make it global so its easier to access it from the Title function
int i, N = 16;
long semilla = 0;
int ancho = 1920, alto = 1080;
BST<visData> Basura;
arBonito V(ancho, alto, &Basura);

//Main title function start
void Title()
{
    Texture2D controls = LoadTexture("assets/controls.png");        // Controls loading
    bool title = true;

    string seed;

    float boxWidth =340;
    float boxHeight =50;

    Rectangle inputBox ={ancho  / 2 - boxWidth  / 2, alto / 2 - boxHeight / 2, boxWidth, boxHeight};
    Rectangle startButton = {ancho/2 - 150.0f/2.0f, (alto/2 - boxHeight/2) + boxHeight + 20.0f, 150.0f, 50.0f};

    bool inputFocused = false;

    Music bgm =LoadMusicStream("assets/tracks/title_screen.mp3"); //Balatro bgm
    PlayMusicStream(bgm);
    float timePlayed =0.0f;

    while (!WindowShouldClose()) 
    {


        UpdateMusicStream(bgm);   // Update music buffer with new stream data

        timePlayed = GetMusicTimePlayed(bgm)/GetMusicTimeLength(bgm);

        if (timePlayed > 1.0f) timePlayed = 1.0f;

        if (title) 
        {

            // Mouse detector
            if (CheckCollisionPointRec(GetMousePosition(), inputBox)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    inputFocused = true;
                }
            } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                inputFocused = false;
            }


            if (inputFocused) {
                int key = GetCharPressed();
                while (key > 0) {
                    if (key >= '0' && key <= '9' && seed.size()<9) {
                        seed.push_back(key);
                    }
                    key = GetCharPressed();
                }

                // Backspace
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if(!seed.empty()) 
                        seed.pop_back();
                }
            }
            const char* charSeed = seed.c_str();

            // --- START BUTTON ---
            if (CheckCollisionPointRec(GetMousePosition(), startButton) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                // Convert seed to unsigned int or generate one if blank
                if (!seed.empty()) {
                    semilla=stoi(seed);
                } else {
                    semilla = GetRandomValue(0, 999999);
                }

                title = false;
            }

            // ---------- DRAW ----------
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("<<-ARBOLATRO->>", ancho/2 -180, 80, 40, DARKBLUE);

            // Draw input box
            DrawRectangleRec(inputBox, LIGHTGRAY);
            DrawRectangleLinesEx(inputBox, 2, GRAY);

            if(seed.empty())
                DrawText("Leave empty for random seed", inputBox.x + 10, inputBox.y + 8, 20, BLACK);
            else
            {
                DrawText(charSeed,  inputBox.x + 10, inputBox.y + 8, 20, BLACK);
            }
            if (inputFocused)
                DrawText("Type seed...", inputBox.x, inputBox.y - 20, 20, DARKGRAY);

            // Draw Start button
            DrawRectangleRec(startButton, SKYBLUE);
            DrawRectangleLinesEx(startButton, 2, BLUE);
            DrawText("Start", startButton.x + 45, startButton.y + 15, 20, DARKBLUE);



            DrawTexture(controls,ancho-controls.width, alto-controls.height,WHITE);
        }

        else if (!title) {
            break;
        }

        EndDrawing();

    }
}
//Main title function end


//main function for real
int main (int argc, char **argv)
{

    InitAudioDevice(); //Audio device
    InitWindow(ancho,alto, "Arbonito");
    SetTargetFPS(60);
    //SetExitKey(KEY_NULL);        // Disable ESC auto-exit

    Title();
    if (argc > 1)
        N = atoi (argv[1]);
    if (argc > 2)
        semilla = atol (argv[2]);

    cout << endl
        << "Initialized random number generator with seed: "
        << semilla << endl << endl;

    //Llenamos el árbol
    srand48 (semilla);

    for (i = 0; i < N; ++i)
    {
        visData val ((int) (lrand48 () % 1000));

        V.inserta (val);
        cout << "Insert value " << val << " to the BST." << endl;
    }
    cout << endl;

    V.Loop ();

    return 0;
}
