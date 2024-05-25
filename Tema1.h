#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;
    protected:
        // structura de coordonate si alte detalii pentru hexagoanele inamice
        struct hexagon_inamic {
            int x = 0;
            int y = 0;
            int row;
            int color;
            bool deleted = false;
            int hp = 3;
            float scaleX = 1;
            float scaleY = 1;
        };

        // coordonate si alte detalii pentru proiectile
        struct proiectil {
            int x = 0;
            int y = 0;
            float angularStep = 0;
            int color;
            int row;
            int column;
        };

    private:
        void FrameStart() override;

        void MicsoreazaHexagon(hexagon_inamic *hex);
        void AnimatieHexagon(hexagon_inamic *hex, float deltaTimeSeconds);
        void PlasareRomb(int x, int y, int color);
        void StergereRomb(int x, int y, int color, float scale_x, float scale_y);
        void AnimatieProiectil(proiectil *proj);
        
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        glm::vec3 limit;
        glm::vec3 startPosition;


        // variabile pentru stelute
        bool stea_noua;
        float temporizator_stele;
        glm::ivec2 pozitia_stelelor[3];
        int numar_de_stele;
        bool afiseaza_steaua[3];
        bool limita_de_stele;

        // booleana sa vad daca am selectat un romb
        bool romb_selectat[4];

        // scalare de romburi
        glm::vec2 scalare_romb[3][3];

        // structura cu coordonatele campurilor de ales romburi
        struct hitbox_chenar {
            glm::vec2 center = glm::vec2(0, 0);
            float radius = 0;
            int pret = 0;
        } hitbox_chenare[4];

        // structura cu coordonatele campurilor de joc
        struct hitbox_field {
            glm::vec2 center = glm::vec2(0, 0);
            float radius = 0;
        } hitbox_campuri_de_joc[3][3];

        // vectorul cu proiectile
        std::vector<proiectil> proiectile;

        // coordonatre drag & drop pentru cumparare de romburi
        glm::vec2 drag_and_drop_romb = glm::vec2(0, 0);

        // -1 = spatiu gol, 0, 1, 2, 3 = culoare romb, 4, 5, 6, 7 =  animatie stergere romb in fct de culoare
        int romb_pe_camp[3][3];

        // vectorul cu hexagoane inamice
        std::vector<hexagon_inamic> inamici;

        // temporizator inamic
        float temporizator_hexagoane;
        bool hexagon_inamic_nou = true;

        // proiectile
        bool proiectil_nou_bool[3][3];
        float temporizator_proiectil[3][3];

        // puncte viata
        int vieti;

        // variabile
        int hexagon_speed;

        // Pie
        float pi = 3.1415f;
       

    };
}   // namespace m1
