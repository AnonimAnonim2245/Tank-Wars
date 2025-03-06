#pragma once
#include <unordered_set>
#include "components/simple_scene.h"


namespace m1
{



    class Game : public gfxc::SimpleScene
    {
    public:
        Game();
        ~Game();

        void Init() override;

    private:
        void FrameStart() override;
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
        float centerX, centerY;
        int currentElementIndex, currentElementIndex2;
        float size_of_step, size_of_step2;
        int current_key = 0;
        std::unordered_map<int, bool> keyState;
        float t, current_angle2, t2;
        float threshold, current_angle, op=1, sizeBar, heightBar, placeHeightBar;
        float hpTank1, hpTank2;
        int enter_transition = 1, enter_transition2 = 1;
        float modelSizeX, modelSizeY;
        float topSizeX, topSizeY;
        glm::mat3 modelMatrix, modelMatrix2, modelMatrix3, modelMatrix4, modelMatrix5;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        bool isTank1Invincible, isTank2Invincible;
        float timeTank1 = 10, timeTank2 = 10;
        float colorType = 0;
        float topBase;
        glm::vec3 corner2;
        Mesh* bodyTank;
        Mesh* circle;
        Mesh* bodyTank2;
        Mesh* Tank2circle;
        float sumTime2=0;
        float sumTime3=0;
        float sumTime = 0, frozenOne=0, frozenTwo=0;
        float hurtOne=0, hurtTwo=0, speedOne = 0, speedTwo = 0;
        float pace1 = 1, pace2=1, timeSpeed1 = 0, timeSpeed2 = 0;
        float healthOne = 0, healthTwo = 0;
        float FrozenTank1Time = 0,FrozenTank2Time = 0;
        float num = 0, radius;
        float storeIce=0, speeds1=1, speeds2=1, speedTime1=3, speedTime2=3;
        std::string color;
        std::vector<std::pair<float, float>> vectorAreas;

        float currentTurretAngle, currentTurretAngle2;
        float formerCurrentTurretAngle, formerCurrentAngle;
        float formerCurrentTurretAngle2, formerCurrentAngle2;
        float totalTime, powerupSize;
        bool moveRightTank1, moveRightTank2;
        std::unordered_set<int> m, colors;
        glm::vec2 a;
        bool ball_is_moving;
        bool ball_is_moving2;

        glm::vec2 v;

        std::vector<std::pair<float, float>> vectorFormerTank;
        std::vector<glm::vec2> vectorV;
        std::vector<int> vectorColor;
        std::vector<glm::vec2> vectorPos;
        std::vector<float> vectorFormerAngle, vectorFormerTurrentAngle;
        std::vector<glm::mat3> vectorMatrix;

        std::pair<float, float> TankMovement, formerTankMovement, TankMovement2;



        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
