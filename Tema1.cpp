#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 center = glm::vec3(0, 0, 0);

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initializare limite de ecran
    limit.x = resolution.x;
    limit.y = resolution.y;


    // Initializarea variabile pentru hexagoane
    temporizator_hexagoane = 7;
    hexagon_inamic_nou = true;
    hexagon_speed = 60;

    // Initializare the hp
    vieti = 3;

    // Initializare variabile stelute
    numar_de_stele = 0;
    temporizator_stele = 6;
    stea_noua = true;
    limita_de_stele = false;


    // Initializare matrice unidimensionala
    for (int i = 0; i < 4; i++) {
        romb_selectat[i] = false;
    }

    // Initializarea matricelor bidimensionale
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // scalarea romburilor pentru a fi distruse
            scalare_romb[i][j].x = 1;
            scalare_romb[i][j].y = 1;
            // viteza de atac a romburilor
            temporizator_proiectil[i][j] = 1;
            // verific daca rombul are nevoie de proiectil nou
            proiectil_nou_bool[i][j] = true;
            // campurile de joc cu romburile corespunzatoare in ele
            romb_pe_camp[i][j] = -1;
        }
    }

    // Meshes
    {
        corner = glm::vec3(0, 0, 0);
        center = glm::vec3(0, 0, 0);

        // Cele patru chenare cu romburi de sus
        {
            // Chenar 1
            Mesh* romb_1 = object2D::CreateRomb("romb_1", center, 30, glm::vec3(1, 0.25f, 0.39f));
            AddMeshToList(romb_1);
            Mesh* tun_1 = object2D::CreateRectangle("tun_1", corner, 15, 30, glm::vec3(1, 0.25f, 0.39f));
            AddMeshToList(tun_1);
            Mesh* chenar_1 = object2D::CreateSquare("chenar_1", corner, 80, glm::vec3(1, 0.25f, 0.39f), false);
            AddMeshToList(chenar_1);

            // Chenar 2
            Mesh* romb_2 = object2D::CreateRomb("romb_2", center, 30, glm::vec3(0.97f, 0.6f, 0.6f));
            AddMeshToList(romb_2);
            Mesh* tun_2 = object2D::CreateRectangle("tun_2", corner, 15, 30, glm::vec3(0.97f, 0.6f, 0.6f));
            AddMeshToList(tun_2);
            Mesh* chenar_2 = object2D::CreateSquare("chenar_2", corner, 80, glm::vec3(0.97f, 0.6f, 0.6f), false);
            AddMeshToList(chenar_2);

            // Chenar 3
            Mesh* romb_3 = object2D::CreateRomb("romb_3", center, 30, glm::vec3(0.78f, 0.49f, 0.86f));
            AddMeshToList(romb_3);
            Mesh* tun_3 = object2D::CreateRectangle("tun_3", corner, 15, 30, glm::vec3(0.78f, 0.49f, 0.86f));
            AddMeshToList(tun_3);
            Mesh* chenar_3 = object2D::CreateSquare("chenar_3", corner, 80, glm::vec3(0.78f, 0.49f, 0.86f), false);
            AddMeshToList(chenar_3);

            // Chenar 4
            Mesh* romb_4 = object2D::CreateRomb("romb_4", center, 30, glm::vec3(0.5f, 0.68f, 0.6f));
            AddMeshToList(romb_4);
            Mesh* tun_4 = object2D::CreateRectangle("tun_4", corner, 15, 30, glm::vec3(0.5f, 0.68f, 0.6f));
            AddMeshToList(tun_4);
            Mesh* chenar_4 = object2D::CreateSquare("chenar_4", corner, 80, glm::vec3(0.5f, 0.68f, 0.6f), false);
            AddMeshToList(chenar_4);

            // Stabilire hitbox si pret
            for (int i = 0; i < 4; i++) {
                hitbox_chenare[i].center = glm::vec2(90 + i * 90, limit.y - 100);
                hitbox_chenare[i].radius = 40;
            }
            hitbox_chenare[0].pret = 1;
            hitbox_chenare[1].pret = 2;
            hitbox_chenare[2].pret = 2;
            hitbox_chenare[3].pret = 3;
 
        }

        // Campuri de joc
        {
            Mesh* camp_patrat = object2D::CreateSquare("camp_patrat", corner, 80, glm::vec3(0.2f, 0.67f, 0.2f));
            AddMeshToList(camp_patrat);
            Mesh* camp_dreptunghi = object2D::CreateRectangle("camp_dreptunghi", corner, 280, 50, glm::vec3(0.96f, 0.36f, 0.9f));
            AddMeshToList(camp_dreptunghi);

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    hitbox_campuri_de_joc[i][j].center = glm::vec2(120 + j * 100, 50 + i * 100);
                    hitbox_campuri_de_joc[i][j].radius = 40;
                } 
            }
        }

        // Stele
        {   // Stelutele de pe ecran
            corner = glm::vec3(0, 0, 0);
            Mesh* stea = object2D::CreateStar("stea", corner, 80, glm::vec3(0.675f, 0.173f, 0.761f));
            AddMeshToList(stea);

            // Pretul
            corner = glm::vec3(0, 0, 0);
            Mesh* stea_pret = object2D::CreateStar("stea_pret", corner, 30, glm::vec3(0.5f, 0.63f, 0.52f));
            AddMeshToList(stea_pret);

            // Proiectilele sub forma de stea
            corner = glm::vec3(-13, -20, 0);
            Mesh* proiectil_1 = object2D::CreateStar("proiectil_1", corner, 40, glm::vec3(1, 0.25f, 0.39f));
            AddMeshToList(proiectil_1);

            corner = glm::vec3(-13, -20, 0);
            Mesh* proiectil_2 = object2D::CreateStar("proiectil_2", corner, 40, glm::vec3(0.97f, 0.6f, 0.6f));
            AddMeshToList(proiectil_2);

            corner = glm::vec3(-13, -20, 0);
            Mesh* proiectil_3 = object2D::CreateStar("proiectil_3", corner, 40, glm::vec3(0.78f, 0.49f, 0.86f));
            AddMeshToList(proiectil_3);

            corner = glm::vec3(-13, -20, 0);
            Mesh* proiectil_4 = object2D::CreateStar("proiectil_4", corner, 40, glm::vec3(0.5f, 0.68f, 0.6f));
            AddMeshToList(proiectil_4);
        }

        // Viata
        {
            center = glm::vec3(0, 0, 0);;
            Mesh* hp = object2D::CreateRomb("hp", center, 20, glm::vec3(0.96f, 0.36f, 0.9f));
            AddMeshToList(hp);
        }

        // Hexagoane
        {
            Mesh* hexagon_1 = object2D::CreateHexagon("hexagon_1", 26, glm::vec3(1, 0.25f, 0.39f));
            AddMeshToList(hexagon_1);

            Mesh* hexagon_2 = object2D::CreateHexagon("hexagon_2", 26, glm::vec3(0.97f, 0.6f, 0.6f));
            AddMeshToList(hexagon_2);

            Mesh* hexagon_3 = object2D::CreateHexagon("hexagon_3", 26, glm::vec3(0.78f, 0.49f, 0.86f));
            AddMeshToList(hexagon_3);

            Mesh* hexagon_4 = object2D::CreateHexagon("hexagon_4", 26, glm::vec3(0.5f, 0.68f, 0.6f));
            AddMeshToList(hexagon_4);

            Mesh* centrul_hexagonului = object2D::CreateHexagon("centrul_hexagonului", 18, glm::vec3(0.96f, 0.36f, 0.9f));
            AddMeshToList(centrul_hexagonului);
            
        }

    }

}


void Tema1::FrameStart()
{
    // Background
    glClearColor(0.89f, 0.8f, 0.68f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

}

void Tema1::MicsoreazaHexagon(hexagon_inamic *hex)
{
    if ((*hex).scaleX >= 0) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate((*hex).x, (*hex).y)
            * transform2D::Scale((*hex).scaleX, (*hex).scaleY)
            * transform2D::Scale(0.7f, 0.7f);
        RenderMesh2D(meshes["centrul_hexagonului"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate((*hex).x, (*hex).y)
            * transform2D::Scale((*hex).scaleX, (*hex).scaleY);

        if ((*hex).color == 0) {
            RenderMesh2D(meshes["hexagon_1"], shaders["VertexColor"], modelMatrix);
        }
        else if ((*hex).color == 1) {
            RenderMesh2D(meshes["hexagon_2"], shaders["VertexColor"], modelMatrix);
        }
        else if ((*hex).color == 2) {
            RenderMesh2D(meshes["hexagon_3"], shaders["VertexColor"], modelMatrix);
        }
        else if ((*hex).color == 3) {
            RenderMesh2D(meshes["hexagon_4"], shaders["VertexColor"], modelMatrix);
        }
    }
    else return;
}

void Tema1::AnimatieHexagon(hexagon_inamic *hex, float deltaTimeSeconds)
{
    if ((*hex).deleted == true) {
        MicsoreazaHexagon(hex);
        (*hex).scaleX -= 2 * deltaTimeSeconds;
        (*hex).scaleY -= 2 * deltaTimeSeconds;
        return;
    } else {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate((*hex).x, (*hex).y)
            * transform2D::Scale(0.7f, 0.7f);
        RenderMesh2D(meshes["centrul_hexagonului"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate((*hex).x, (*hex).y);

        (*hex).x -= hexagon_speed * deltaTimeSeconds;
        

        if ((*hex).color == 0) {
            RenderMesh2D(meshes["hexagon_1"], shaders["VertexColor"], modelMatrix);
        }
        else if ((*hex).color == 1) {
            RenderMesh2D(meshes["hexagon_2"], shaders["VertexColor"], modelMatrix);
        }
        else if ((*hex).color == 2) {
            RenderMesh2D(meshes["hexagon_3"], shaders["VertexColor"], modelMatrix);
        }
        else if ((*hex).color == 3) {
            RenderMesh2D(meshes["hexagon_4"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Tema1::PlasareRomb(int x, int y, int color)
{
    if (color == 0) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        RenderMesh2D(meshes["romb_1"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y - 6);
        RenderMesh2D(meshes["tun_1"], shaders["VertexColor"], modelMatrix);
    }

    if (color == 1) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        RenderMesh2D(meshes["romb_2"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y - 6);
        RenderMesh2D(meshes["tun_2"], shaders["VertexColor"], modelMatrix);
    }

    if (color == 2) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        RenderMesh2D(meshes["romb_3"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y - 6);
        RenderMesh2D(meshes["tun_3"], shaders["VertexColor"], modelMatrix);
    }

    if (color == 3) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        RenderMesh2D(meshes["romb_4"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y - 6);
        RenderMesh2D(meshes["tun_4"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::StergereRomb(int x, int y, int color, float scale_x, float scale_y)
{
    if (scale_x > 0 || scale_y > 0) {
        if (color == 4) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y) * transform2D::Scale(scale_x, scale_y);
            RenderMesh2D(meshes["romb_1"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y - 6) * transform2D::Scale(scale_x - 0.02f, 1);
            RenderMesh2D(meshes["tun_1"], shaders["VertexColor"], modelMatrix);
        }
        else if (color == 5) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y) * transform2D::Scale(scale_x, scale_y);
            RenderMesh2D(meshes["romb_2"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y - 6) * transform2D::Scale(scale_x - 0.02f, 1);
            RenderMesh2D(meshes["tun_2"], shaders["VertexColor"], modelMatrix);
        }
        else if (color == 6) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y) * transform2D::Scale(scale_x, scale_y);
            RenderMesh2D(meshes["romb_3"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y - 6) * transform2D::Scale(scale_x - 0.02f, 1);
            RenderMesh2D(meshes["tun_3"], shaders["VertexColor"], modelMatrix);
        }
        else if (color == 7) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y) * transform2D::Scale(scale_x, scale_y);
            RenderMesh2D(meshes["romb_4"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y - 6) * transform2D::Scale(scale_x - 0.02f, 1);
            RenderMesh2D(meshes["tun_4"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Tema1::AnimatieProiectil(proiectil *proj)
{
    if ((*proj).x <= limit.x) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate((*proj).x, (*proj).y)
            * transform2D::Translate(13, 20)
            * transform2D::Rotate((*proj).angularStep);

        (*proj).x += 3;
        (*proj).angularStep -= pi * 0.02f;

        if ((*proj).color == 0) {
            RenderMesh2D(meshes["proiectil_1"], shaders["VertexColor"], modelMatrix);
        }
        if ((*proj).color == 1) {
            RenderMesh2D(meshes["proiectil_2"], shaders["VertexColor"], modelMatrix);
        }
        if ((*proj).color == 2) {
            RenderMesh2D(meshes["proiectil_3"], shaders["VertexColor"], modelMatrix);
        }
        if ((*proj).color == 3) {
            RenderMesh2D(meshes["proiectil_4"], shaders["VertexColor"], modelMatrix);
        }
    }
}


void Tema1::Update(float deltaTimeSeconds)
{
    // Aparitia rombului la coordonatele mouseului
    {
        if (romb_selectat[0] == true) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y);
            RenderMesh2D(meshes["romb_1"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y - 6);
            RenderMesh2D(meshes["tun_1"], shaders["VertexColor"], modelMatrix);
        }

        if (romb_selectat[1] == true) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y);
            RenderMesh2D(meshes["romb_2"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y - 6);
            RenderMesh2D(meshes["tun_2"], shaders["VertexColor"], modelMatrix);
        }

        if (romb_selectat[2] == true) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y);
            RenderMesh2D(meshes["romb_3"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y - 6);
            RenderMesh2D(meshes["tun_3"], shaders["VertexColor"], modelMatrix);
        }

        if (romb_selectat[3] == true) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y);
            RenderMesh2D(meshes["romb_4"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(drag_and_drop_romb.x, drag_and_drop_romb.y - 6);
            RenderMesh2D(meshes["tun_4"], shaders["VertexColor"], modelMatrix);
        }
    }

    // Aparitia hexagoanelor randomizata
    {
        if (hexagon_inamic_nou == true) {
            struct hexagon_inamic inamic_nou;

            inamic_nou.x = limit.x;
            inamic_nou.row = std::rand() % 3;
            inamic_nou.y = 50 + inamic_nou.row * 100;
            inamic_nou.color = std::rand() % 4;
            inamic_nou.deleted = false;
            inamic_nou.hp = 3;

            // Randomizez temporizarea inamicilor
            temporizator_hexagoane = rand() % 5 + 3;

            inamici.push_back(inamic_nou);

            hexagon_inamic_nou = false;
        }
    }

    // Cand trece timpul randomizat, apare un inamic nou
    {
        if (hexagon_inamic_nou == false) {
            temporizator_hexagoane -= deltaTimeSeconds;
            if (temporizator_hexagoane < 0) {
                hexagon_inamic_nou = true;
            }
        }
    }

    // Colectarea de stele
    {
        // le fac sa apara aleator pe ecran, la 6 secunde
        glm::ivec2 resolution = window->GetResolution();
        if (stea_noua == true && limita_de_stele == false) {

            for (int i = 0; i < 3; i++) {
                pozitia_stelelor[i].x = std::rand() % 700;
                pozitia_stelelor[i].y = std::rand() % 600;
            }
            stea_noua = false;
            afiseaza_steaua[0] = true;
            afiseaza_steaua[1] = true;
            afiseaza_steaua[2] = true;
        }

        if (afiseaza_steaua[0] == true) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(pozitia_stelelor[0].x, pozitia_stelelor[0].y);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
        }

        if (afiseaza_steaua[1] == true) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(pozitia_stelelor[1].x, pozitia_stelelor[1].y);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
        }

        if (afiseaza_steaua[2] == true) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(pozitia_stelelor[2].x, pozitia_stelelor[2].y);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
        }

        if (stea_noua == false && limita_de_stele == false) {
            temporizator_stele -= deltaTimeSeconds;
            if (temporizator_stele < 0) {
                stea_noua = true;
                temporizator_stele = 6;
            }
        }
    }


    // Iterez prin inamici si ii animez
    {
        for (vector<hexagon_inamic>::iterator hex = inamici.begin(); hex < inamici.end(); hex++) {
            AnimatieHexagon(&(*hex), deltaTimeSeconds);

            // Daca hexagonul s-a micsorat complet, este sters.
            if ((*hex).scaleX < 0) {
                if (hex + 1 != inamici.end()) {
                    hex = inamici.erase(hex);
                }
                else {
                    continue;
                }
            }

            // Pentru fiecare proiectil din lista, daca hexagonul intalneste trei proiectile, se micsoreaza
            for (vector<struct proiectil>::iterator proj = proiectile.begin(); proj < proiectile.end(); proj++) {
                if ((*hex).color == (*proj).color && (*hex).row == (*proj).row && (*hex).deleted == false) {
                    if (sqrt((*hex).x - (*proj).x) * ((*hex).x - (*proj).x) <= 33) {
                        (*hex).hp--;
                        if (proj + 1 != proiectile.end()) {
                            proj = proiectile.erase(proj);
                        }
                        else {
                            continue;
                        }

                    }
                    if ((*hex).hp <= 0) {
                        (*hex).deleted = true;
                    }
                }
            }

            // Daca hexagonul loveste un romb, il distruge
            for (int j = 0; j < 3; j++) {
                int i = (*hex).row;
                // If collision
                if (sqrt((*hex).x - hitbox_campuri_de_joc[i][j].center.x) * ((*hex).x - hitbox_campuri_de_joc[i][j].center.x) <= (10 + hitbox_campuri_de_joc[i][j].radius)
                    && romb_pe_camp[i][j] >= 0 && romb_pe_camp[i][j] <= 3) {
                    // Delete turret
                    romb_pe_camp[i][j] += 4;
                }
            }


            // Cand hexagonul ajunge la final, scade viata jucatorului
            if ((*hex).x <= 50) {
                vieti--;
                if (hex + 1 != inamici.end()) {
                    hex = inamici.erase(hex);
                }
                else {
                    continue;
                }
            }
        }
    }

    // Plasarea romburilor si animatia proiectilelor
    {
        // Interactiunea scena - inamici
        {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    
                    // Iterez prin inamici
                    for (vector<hexagon_inamic>::iterator hex = inamici.begin(); hex < inamici.end(); hex++) {
                        // verific daca exista romb pe randul sau
                        if ((*hex).deleted == false && (*hex).row == i && romb_pe_camp[i][j] >= 0 && romb_pe_camp[i][j] <= 3 && (*hex).color == romb_pe_camp[i][j]) {
                            // Rombul incepe sa atace
                            if (proiectil_nou_bool[i][j] == true) {
                                struct proiectil proiectil_now;
                                proiectil_now.x = 120 + j * 100;
                                proiectil_now.y = 35 + i * 100;
                                proiectil_now.angularStep = 0;
                                proiectil_now.color = romb_pe_camp[i][j];
                                proiectil_now.row = i;
                                proiectil_now.column = j;

                                temporizator_proiectil[i][j] = 1;

                                proiectile.push_back(proiectil_now);
                                proiectil_nou_bool[i][j] = false;
                            }
                        }
                    }
                    
                    // Contiunui animatia proiectilelor si dupa ce a fost invins inamicul, sau dupa ce a fost sters rombul
                    for (vector<struct proiectil>::iterator proj = proiectile.begin(); proj < proiectile.end(); proj++) {
                        for (int color = 0; color < 4; color++) {
                            if ((*proj).color == color && (*proj).column == j && (*proj).row == i) {
                                AnimatieProiectil(&(*proj));
                            }

                            if ((*proj).x >= limit.x) {
                                if (proj + 1 != proiectile.end()) {
                                    proj = proiectile.erase(proj);
                                }
                                else {
                                    continue;
                                }
                            }
                        }
                       
                    }

                    temporizator_proiectil[i][j] -= deltaTimeSeconds;
                    if (temporizator_proiectil[i][j] < 0) {
                        proiectil_nou_bool[i][j] = true;
                    }
                }
            }
        }

        // Plasarea romburilor pe campuri
        {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (romb_pe_camp[i][j] >= 0 && romb_pe_camp[i][j] <= 3) {
                        PlasareRomb(hitbox_campuri_de_joc[i][j].center.x - 5, hitbox_campuri_de_joc[i][j].center.y, romb_pe_camp[i][j]);
                    }
                    else if (romb_pe_camp[i][j] >= 4 && romb_pe_camp[i][j] <= 7) {
                        StergereRomb(hitbox_campuri_de_joc[i][j].center.x - 5, hitbox_campuri_de_joc[i][j].center.y, romb_pe_camp[i][j], scalare_romb[i][j].x, scalare_romb[i][j].y);
                        scalare_romb[i][j].x -= 3 * deltaTimeSeconds;
                        scalare_romb[i][j].y -= 3 * deltaTimeSeconds;
                        if (scalare_romb[i][j].x < 0) {
                            romb_pe_camp[i][j] = -1;
                            scalare_romb[i][j].x = 1;
                            scalare_romb[i][j].y = 1;
                        }
                    }
                }
            }
        }
    }

    // Scena
    {
        // Chenarele de sus cu romburi
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(90, limit.y - 100);
            RenderMesh2D(meshes["romb_1"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(90, limit.y - 107);
            RenderMesh2D(meshes["tun_1"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(50, limit.y - 140);
            RenderMesh2D(meshes["chenar_1"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(180, limit.y - 100);
            RenderMesh2D(meshes["romb_2"], shaders["VertexColor"], modelMatrix); 
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(180, limit.y - 107);
            RenderMesh2D(meshes["tun_2"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(140, limit.y - 140);
            RenderMesh2D(meshes["chenar_2"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(270, limit.y - 100);
            RenderMesh2D(meshes["romb_3"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(270, limit.y - 107);
            RenderMesh2D(meshes["tun_3"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(230, limit.y - 140);
            RenderMesh2D(meshes["chenar_3"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(360, limit.y - 100);
            RenderMesh2D(meshes["romb_4"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(360, limit.y - 107);
            RenderMesh2D(meshes["tun_4"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(320, limit.y - 140);
            RenderMesh2D(meshes["chenar_4"], shaders["VertexColor"], modelMatrix);
        }

        // Campurile de joc
        {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(hitbox_campuri_de_joc[i][j].center.x - hitbox_campuri_de_joc[i][j].radius, hitbox_campuri_de_joc[i][j].center.y - hitbox_campuri_de_joc[i][j].radius);
                    RenderMesh2D(meshes["camp_patrat"], shaders["VertexColor"], modelMatrix);
                }
            }
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(10, 10);
            RenderMesh2D(meshes["camp_dreptunghi"], shaders["VertexColor"], modelMatrix);
        }

        // Preturile cu stele
        {
            // Primul romb
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(80, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);

            // Al doilea romb
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(160, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(190, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);

            // Al treilea romb
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(245, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(275, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);

            // Al patrulea romb
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(320, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(350, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(380, limit.y - 200);
            RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);
        }

        // Viata
        {
            // Inima 1
            if (vieti >= 1) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(907, limit.y - 70);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(919, limit.y - 70);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(913, limit.y - 84)
                    * transform2D::Scale(1, 0.6f);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);
            }

            // Inima 2
            if (vieti >= 2) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(947, limit.y - 70);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(959, limit.y - 70);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(953, limit.y - 84)
                    * transform2D::Scale(1, 0.6f);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);
            }

            // Inima 3
            if (vieti >= 3) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(987, limit.y - 70);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(999, limit.y - 70);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(993, limit.y - 84)
                    * transform2D::Scale(1, 0.6f);
                RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);
            }

        }

        // Stelutele de sub vieti
        {
            if (numar_de_stele <= 10) {
                for (int i = 1; i <= numar_de_stele % 11; i++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(830 + i * 30, limit.y - 200);
                    RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);
                }
            }
            if (numar_de_stele > 10 && numar_de_stele <= 20) {
                for (int i = 1; i <= 10; i++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(830 + i * 30, limit.y - 200);
                    RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);
                }
                for (int i = 1; i <= numar_de_stele % 11; i++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(830 + i * 30, limit.y - 230);
                    RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);
                }
            }
            if (numar_de_stele > 20) {
                for (int i = 1; i <= 10; i++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(830 + i * 30, limit.y - 200);
                    RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);
                }
                for (int i = 1; i <= 10; i++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(830 + i * 30, limit.y - 230);
                    RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);
                }
                for (int i = 1; i <= numar_de_stele % 21; i++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(830 + i * 30, limit.y - 260);
                    RenderMesh2D(meshes["stea_pret"], shaders["VertexColor"], modelMatrix);
                }
            }
        }
    }

    // Exit cand s-a ajuns la 0 vieti
    if (vieti <= 0) {
        exit(0);
    }
}


void Tema1::FrameEnd()
{

}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    int actual_y = window->GetResolution().y - mouseY;

    if (romb_selectat[0] == true || romb_selectat[1] == true || romb_selectat[2] == true || romb_selectat[3] == true) {
        drag_and_drop_romb.x = mouseX;
        drag_and_drop_romb.y = actual_y;
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    int actual_y = window->GetResolution().y - mouseY;

    // Colectare de stele
    if (button == GLFW_MOUSE_BUTTON_2) {
        for (int i = 0; i < 3; i++) {
            if (afiseaza_steaua[i] == true
                && mouseX >= pozitia_stelelor[i].x
                && mouseX <= pozitia_stelelor[i].x + 80
                && actual_y <= pozitia_stelelor[i].y + 80
                && actual_y >= pozitia_stelelor[i].y) {

                numar_de_stele++;
                afiseaza_steaua[i] = false;
            }
        }

        // Putem strange maxim 30 de stele, dupa care nu mai apar pe ecran
        // pana nu se scade la 15 stele
        if (numar_de_stele >= 30) {
            limita_de_stele = true;
        }
        if (numar_de_stele <= 15) {
            limita_de_stele = false;
        }
    }

    // Cumpararea de romburi daca respecta conditiile de pret
    if (button == GLFW_MOUSE_BUTTON_2) {
        for (int i = 0; i < 4; i++) {
            if (numar_de_stele >= hitbox_chenare[i].pret) {
                if (mouseX >= hitbox_chenare[i].center.x - hitbox_chenare[i].radius
                    && mouseX <= hitbox_chenare[i].center.x + hitbox_chenare[i].radius
                    && actual_y <= hitbox_chenare[i].center.y + hitbox_chenare[i].radius
                    && actual_y >= hitbox_chenare[i].center.y - hitbox_chenare[i].radius) {

                    romb_selectat[i] = true;
                    drag_and_drop_romb.x = mouseX;
                    drag_and_drop_romb.y = actual_y;
                }
            }
        }
    }

    // Stergerea rombului cu click dreapta
    if (button == GLFW_MOUSE_BUTTON_3) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int col = 0; col < 4; col++) {
                    if (romb_pe_camp[i][j] == col) {
                        if (mouseX >= hitbox_campuri_de_joc[i][j].center.x - hitbox_campuri_de_joc[i][j].radius
                            && mouseX <= hitbox_campuri_de_joc[i][j].center.x + hitbox_campuri_de_joc[i][j].radius
                            && actual_y <= hitbox_campuri_de_joc[i][j].center.y + hitbox_campuri_de_joc[i][j].radius
                            && actual_y >= hitbox_campuri_de_joc[i][j].center.y - hitbox_campuri_de_joc[i][j].radius) {

                            romb_pe_camp[i][j] = col + 4;
                        }
                    }
                }
            }
        }
        
    }

}



void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    int actual_y = window->GetResolution().y - mouseY;
    int color = -1;

    // Se verifica sa se dea release la mouse intr-unul din chenare
    if (button == GLFW_MOUSE_BUTTON_2) {
        for (int i = 0; i < 4; i++) {
            if (romb_selectat[i] == true) {
                color = i;
                break;
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (mouseX >= hitbox_campuri_de_joc[i][j].center.x - hitbox_campuri_de_joc[i][j].radius
                    && mouseX <= hitbox_campuri_de_joc[i][j].center.x + hitbox_campuri_de_joc[i][j].radius
                    && actual_y <= hitbox_campuri_de_joc[i][j].center.y + hitbox_campuri_de_joc[i][j].radius
                    && actual_y >= hitbox_campuri_de_joc[i][j].center.y - hitbox_campuri_de_joc[i][j].radius) {

                    if (romb_pe_camp[i][j] == -1) {
                        if (color == 0) {
                            numar_de_stele -= 1;
                        }
                        else if (color == 1 || color == 2) {
                            numar_de_stele -= 2;
                        }
                        else if (color == 3) {
                            numar_de_stele -= 3;
                        }
                        romb_pe_camp[i][j] = color;
                    }
                }
                else {
                    romb_selectat[color] = false;
                }
            }
        }
    }
        
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
