#include "lab_m1/homework/Homework1_TankWars/gameFiles/game.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_set>

#include "lab_m1/homework/Homework1_TankWars/gameFiles/transform2D_2.h"
#include "lab_m1/homework/Homework1_TankWars/gameFiles/object2D_2.h"

using namespace std;
using namespace m1;

/*
 * Homework realized by Luca Plian
*/

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Game::Game()
{
}


Game::~Game()
{
}

float f(float x)
{
    return sin(x) + cos(0.5 * x)/2 + sin(2*x);
}

void Game::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    currentTurretAngle = 90;
    currentTurretAngle2 = 90;

    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    op=1;
    t=0;
    t2=0;


    a = glm::vec2(0, 9.8);
    glm::vec3 corner = glm::vec3(0, -1, 0);
    corner2 = glm::vec3(0, 0, 0);
    float squareSide = 1;
    float squareSide2 = 100;
    centerX=squareSide/2;
    centerY=squareSide/2;
    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    modelSizeX = 80;
    modelSizeY = 50;
    topSizeX = 180;
    topSizeY = 40;
    float topBase1 = 140;
    topBase = 160;
    /// the terrain coordinates represented here
    for(float i=0; i<=28; i+=0.1){
        int x = i*100;
        int y = f(i)*100+250;
        pair<float,float> dimen(x, y);
        vectorAreas.push_back(dimen);
    }

    /// the portion of the terrain index where we want our tank to be
    currentElementIndex  = rand() % 250;
    TankMovement.first = vectorAreas[currentElementIndex].first;
    TankMovement.second = vectorAreas[currentElementIndex].second;

    int a = 0;
    int b;
    /// we generate where should be our rival player, and in a area where it is not intersected with out current tank
    while(!a) {
        a=1;
        b = rand() % 250;
        TankMovement2.first = vectorAreas[b].first;
        TankMovement2.second = vectorAreas[b].second;
        if(TankMovement.first >=TankMovement2.first && (TankMovement.first + 180) <=TankMovement2.first){
            a=0;
            /// if the second tank is generated in the area of the first tank we cannot generated
        }

    }

    currentElementIndex2 = b;
    currentElementIndex2 = 200;

    int i=3;


    while(i>0) {
        int j = rand() % 250;

        float range = abs(currentElementIndex - j);
        float range2 = abs(currentElementIndex2 - j);

        if(m.find(j) == m.end() && range >10 && range2 > 10)
        {
            m.insert(j);
            i--;
        }



    }


    threshold = 180;
    sizeBar = 150;
    placeHeightBar = 100;
    heightBar = 30;

    radius = 80;

    size_of_step = modelSizeX/24/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);

    current_angle = atan2((vectorAreas[currentElementIndex+1].second - vectorAreas[currentElementIndex].second),(vectorAreas[currentElementIndex+1].first - vectorAreas[currentElementIndex].first));

    size_of_step2 = modelSizeX/24/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
    current_angle2 = atan2((vectorAreas[currentElementIndex2+1].second - vectorAreas[currentElementIndex2].second),(vectorAreas[currentElementIndex2+1].first - vectorAreas[currentElementIndex2].first));

    enter_transition = 0;
    enter_transition2 = 0;

    isTank1Invincible = false;
    isTank2Invincible = false;
    /// is current tank invicible?


    modelMatrix2 = glm::mat3(1);
    modelMatrix2 *= transform2D_2::Translate(400,400);


    Mesh* square1 = object2D_2::CreateSquare("square1", corner, squareSide, glm::vec3(0.8196, 0.8078, 0.35), true);
    AddMeshToList(square1);


    powerupSize=30;
    /// the size of our powerup
    Mesh* square11 = object2D_2::CreateSquare("square11", corner, powerupSize, glm::vec3(1, 1, 0), true);
    AddMeshToList(square11);

    Mesh* squareFreezeEnemy = object2D_2::CreateSquare("squareBlue", corner, powerupSize, glm::vec3(0, 0.4, 0.8), true);
    AddMeshToList(squareFreezeEnemy);

    Mesh* squareSpeed = object2D_2::CreateSquare("squareSpeed", corner, powerupSize, glm::vec3(0, 0.4, 0), true);
    AddMeshToList(squareSpeed);

    Mesh* squareHealth = object2D_2::CreateSquare("squareHealth", corner, powerupSize, glm::vec3(0.8, 0, 0), true);
    AddMeshToList(squareHealth);

    Mesh* squareHurt = object2D_2::CreateSquare("squareHurt", corner, powerupSize, glm::vec3(0.2, 0, 0), true);
    AddMeshToList(squareHurt);


    bodyTank = object2D_2::CreateT("bodyTank", corner2, topBase, topSizeX, topSizeY, glm::vec3(0.78, 0.69, 0.5529), true);
    AddMeshToList(bodyTank);

    bodyTank2 = object2D_2::CreateT("bodyTank2", corner2, topBase, topSizeX, topSizeY, glm::vec3(0.694, 0.765, 0.5412), true);
    AddMeshToList(bodyTank2);

    Mesh* tank2Tracks = object2D_2::CreateT("tank2Tracks", corner2, topBase1, modelSizeX, modelSizeY, glm::vec3(0.37255, 0.4235, 0.2784), true);
    AddMeshToList(tank2Tracks);

    hpTank1 = 100;
    frozenOne=0;
    frozenTwo=0;
    healthOne=0;
    healthTwo=0;

    hurtOne=0;
    hurtTwo=0;
    speedOne=0;
    speedTwo=0;
    speedTime1=0;
    speedTime2=0;
    speeds1=1;
    speeds2=1;
    hpTank2 = 100;

    circle = object2D_2::CreateCircle("circle", corner2, 40, glm::vec3(0.78, 0.69, 0.5529), true, 36);
    Mesh* tankTracks = object2D_2::CreateT("tankTracks", corner2, topBase1, modelSizeX, modelSizeY, glm::vec3(0.4431, 0.396, 0.3176), true);

    Tank2circle = object2D_2::CreateCircle("Tank2circle", corner2, 40, glm::vec3(0.694, 0.765, 0.5412), true, 36);
    AddMeshToList(Tank2circle);

    AddMeshToList(tankTracks);

    Mesh* rectangle = object2D_2::CreateRECT("rect", corner2, 50, 10, glm::vec3(0.22745, 0.22745, 0.2353), true);
    Mesh* circle4 = object2D_2::CreateCircle("circle4", corner2, 3, glm::vec3(1, 1, 1), true);

    Mesh* circle2 = object2D_2::CreateCircle("circle2", corner2, 8, glm::vec3(0.22745, 0.22745, 0.2353), true, 36);
    Mesh* circleFrozen = object2D_2::CreateCircle("circleFrozen", corner2, 8, glm::vec3(0.4, 1, 1), true, 36);
    Mesh* circleHurt = object2D_2::CreateCircle("circleHurt", corner2, 8, glm::vec3(0.2, 0, 0), true, 36);
    Mesh* circleHealth = object2D_2::CreateCircle("circleHealth", corner2, 8, glm::vec3(0.8, 0, 0), true, 36);
    Mesh* circleSpeed = object2D_2::CreateCircle("circleSpeed", corner2, 8, glm::vec3(0, 0.4, 0), true, 36);

    Mesh* circle3 = object2D_2::CreateCircle("circle3", corner2, 8, glm::vec3(1, 0.22745, 0.2353), true, 36);
    FrozenTank1Time = 0,FrozenTank2Time = 0;

    Mesh* fullHealthBar = object2D_2::CreateRECT("fullHealthBar", corner2, sizeBar, heightBar, glm::vec3(1, 1, 1), false);
    Mesh* HealthBar = object2D_2::CreateRECT("HealthBar", corner2, sizeBar, heightBar, glm::vec3(1, 1, 1), true);

    AddMeshToList(fullHealthBar);
    AddMeshToList(HealthBar);
    AddMeshToList(circleFrozen);
    AddMeshToList(circleHealth);
    AddMeshToList(circleHurt);
    AddMeshToList(circleSpeed);

    AddMeshToList(circle4);
    AddMeshToList(rectangle);
    AddMeshToList(circle);
    AddMeshToList(circle2);
    AddMeshToList(circle3);
}


void Game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.6157, 0.7412, 0.8824, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);




}


void Game::Update(float deltaTimeSeconds)
{

    timeTank1 -=deltaTimeSeconds;
    FrozenTank1Time -=deltaTimeSeconds;
    FrozenTank2Time -=deltaTimeSeconds;
    speedTime1-=deltaTimeSeconds;
    speedTime2-=deltaTimeSeconds;


    /// if the tank has a powerup, we will have here the time of a certain powerup, until it elapses

    sumTime +=deltaTimeSeconds;
    if(speedTime1 < 0) {
        speeds1=1;
    }
    if(speedTime2 < 0) {
        speeds2=1;
    }

    sumTime2+=deltaTimeSeconds;
    sumTime3+=deltaTimeSeconds;
    /*
     the first tank will be spawned in a new random location after this time
     */
    if(sumTime2>10) {
        currentElementIndex  = rand() % 250;
        TankMovement.first = vectorAreas[currentElementIndex].first;
        TankMovement.second = vectorAreas[currentElementIndex].second;

        int a = 0;
        int b;
        while(!a) {
            a=1;
            b = rand() % 250;
            TankMovement2.first = vectorAreas[b].first;
            TankMovement2.second = vectorAreas[b].second;
            if(TankMovement.first >=TankMovement2.first && (TankMovement.first + 180) <=TankMovement2.first){
                a=0;
            }

        }
        sumTime2=0;
    }


    /*
     the second tank will be spawned in a new random location after this time
     */
    if(sumTime3>10) {
        currentElementIndex  = rand() % 250;
        TankMovement2.first = vectorAreas[currentElementIndex2].first;
        TankMovement2.second = vectorAreas[currentElementIndex2].second;

        int a = 0;
        int b;
        while(!a) {
            a=1;
            b = rand() % 250;
            TankMovement.first = vectorAreas[b].first;
            TankMovement.second = vectorAreas[b].second;
            if(TankMovement2.first >=TankMovement.first && (TankMovement2.first + 180) <=TankMovement.first){
                a=0;
            }

        }
        sumTime3=0;
    }
    /*
     this checks the time for when the powerups are available at the same location
     */
    if(sumTime > 15)
    {
        int i=3;
        m.clear();
        while(i>0) {
            int j = rand() % 250;


            int range = abs(currentElementIndex - j);
            int range2 = abs(currentElementIndex2 - j);
            int ok=0;
            for(int zzz=-10; zzz<=10 && ok==0;zzz++) {
                if(m.find(j-zzz) != m.end()){
                    ok=1;
                }
            }

            if(ok==0 && range >10 && range2 > 10)
            {
                m.insert(j);
                i--;
            }



        }
        sumTime=0;
    }
    /*
     *here we check if the time is up for invincibility superpower
     */
    if(isTank1Invincible && timeTank1 <= 0) {

        RemoveMeshToList(bodyTank);
        RemoveMeshToList(circle);
        isTank1Invincible = false;
        circle = object2D_2::CreateCircle("circle", corner2, 40, glm::vec3(0.78, 0.69, 0.5529), true, 36);
        bodyTank = object2D_2::CreateT("bodyTank", corner2, topBase, topSizeX, topSizeY, glm::vec3(0.78, 0.69, 0.5529), true);
        AddMeshToList(bodyTank);
        AddMeshToList(circle);
    } else if(isTank2Invincible && timeTank2 <= 0) {
        RemoveMeshToList(bodyTank2);
        RemoveMeshToList(Tank2circle);
        isTank2Invincible = false;

        Tank2circle = object2D_2::CreateCircle("Tank2circle", corner2, 40, glm::vec3(0.694, 0.765, 0.5412), true, 36);

        bodyTank2 = object2D_2::CreateT("bodyTank2", corner2, topBase, topSizeX, topSizeY, glm::vec3(0.694, 0.765, 0.5412), true);
        AddMeshToList(Tank2circle);
        AddMeshToList(bodyTank2);

    }


    /// we see the current movement of our tank here
    TankMovement.first = vectorAreas[currentElementIndex].first + t * (vectorAreas[currentElementIndex+1].first - vectorAreas[currentElementIndex].first)-40;
    TankMovement.second = vectorAreas[currentElementIndex].second + t * (vectorAreas[currentElementIndex+1].second - vectorAreas[currentElementIndex].second);

    TankMovement2.first = vectorAreas[currentElementIndex2].first + t2 * (vectorAreas[currentElementIndex2+1].first - vectorAreas[currentElementIndex2].first)-40;
    TankMovement2.second = vectorAreas[currentElementIndex2].second + t2 * (vectorAreas[currentElementIndex2+1].second - vectorAreas[currentElementIndex2].second);

    float nr=0;
    float angle=0;
    /// the current angle of our tank
    current_angle = atan2((vectorAreas[currentElementIndex+1].second - vectorAreas[currentElementIndex].second),(vectorAreas[currentElementIndex+1].first - vectorAreas[currentElementIndex].first));
    current_angle2 = atan2((vectorAreas[currentElementIndex2+1].second - vectorAreas[currentElementIndex2].second),(vectorAreas[currentElementIndex2+1].first - vectorAreas[currentElementIndex2].first));



    float size = cos(current_angle) * modelSizeX;
    float size2 = cos(current_angle2) * modelSizeX;



    int ok=0;
    std::vector<int> minPos, maxPos;
    /// we want to establish if we climb up or we climb down
    if(vectorAreas[0].second > vectorAreas[1].second) {
        ok = -2;
        maxPos.push_back(0);
    } else if(vectorAreas[0].second < vectorAreas[1].second){
        ok = 2;
        minPos.push_back(0);
    }


    int zz=0;
    for (auto i=m.begin(); zz<m.size(); true) {
        float x = vectorAreas[*i].first;

        float y = vectorAreas[*i].second-7;
        float angleA =  atan2((vectorAreas[*i+1].second - vectorAreas[*i].second),(vectorAreas[*i+1].first - vectorAreas[*i].first));

        /*
         * here we calculate all the coordinates of the objects(our objects has indexes, and
         * we calculate based on these points
         */

        if(x < TankMovement.first && TankMovement.first < x + size){
            timeTank1 = 4;
            int a = *i;
            /// we check wether the tank has hit a special object

            if(a%5==0)
            {
                isTank1Invincible = true;
                RemoveMeshToList(bodyTank);
                RemoveMeshToList(circle);
                circle = object2D_2::CreateCircle("circle", corner2, 40, glm::vec3(0.8, 0.8, 0), true, 36);

                bodyTank = object2D_2::CreateT("bodyTank", corner2, topBase, topSizeX, topSizeY, glm::vec3(0.8, 0.8, 0), true);
                AddMeshToList(bodyTank);
                AddMeshToList(circle);

                ///modifies the color body of our tank here

            } else if(a%5==1) {
                frozenOne++;;
            } else if(a%5==2) {
                speedOne++;
            } else if(a%5==3) {
                healthOne++;
            } else {
                hurtOne++;
            }

            m.erase(*i);
            i = m.begin();
            std::advance(i, zz);
            if(m.empty()) {
                break;
            }

        }else if(x < TankMovement2.first && TankMovement2.first < x + size2) {
            /// the same thing as with the first tank
            timeTank2 = 4;
            int a = *i;
            if(a%5==0)
            {
                isTank2Invincible = true;

                RemoveMeshToList(bodyTank2);
                RemoveMeshToList(Tank2circle);
                Tank2circle = object2D_2::CreateCircle("Tank2circle", corner2, 40, glm::vec3(0.4, 0.8, 0), true, 36);

                bodyTank2 = object2D_2::CreateT("bodyTank2", corner2, topBase, topSizeX, topSizeY, glm::vec3(0.4, 0.8, 0), true);
                AddMeshToList(bodyTank2);
                AddMeshToList(Tank2circle);

            } else if(a%5==1) {
                frozenTwo++;;
            } else if(a%5==2) {
                speedTwo++;
            } else if(a%5==3) {
                healthTwo++;
            } else {
                hurtTwo++;
            }
            m.erase(*i);
            i = m.begin();
            std::advance(i, zz);
            if(m.empty()) {
                break;
            }


        }  else {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D_2::Translate(x, y);

            modelMatrix *= transform2D_2::Translate(powerupSize/2, powerupSize/2);
            modelMatrix *= transform2D_2::Rotate(angleA);
            modelMatrix *= transform2D_2::Translate(-powerupSize/2, -powerupSize/2);

            if(*i % 5 == 0) {
                RenderMesh2D(meshes["square11"], shaders["VertexColor"], modelMatrix);
            } else if(*i % 5 ==1){
                RenderMesh2D(meshes["squareBlue"], shaders["VertexColor"], modelMatrix);

            } else if(*i % 5 ==2) {
                RenderMesh2D(meshes["squareSpeed"], shaders["VertexColor"], modelMatrix);
            } else if(*i % 5 == 3) {
                RenderMesh2D(meshes["squareHealth"], shaders["VertexColor"], modelMatrix);
            } else if(*i % 5 == 4) {
                RenderMesh2D(meshes["squareHurt"], shaders["VertexColor"], modelMatrix);
            }
            zz++;
            ++i;

            /// we continue to render the object here
        }
    }


    ///we determine whether the points of the terrain that the tank touches are a in upward or downward
    /// trajectory
    for(int i=1;i<270;i++) {
        if(ok == 2 && vectorAreas[i].second > vectorAreas[i+1].second) {
            maxPos.push_back(i);
            ok = -2;
        } else if(ok == -2 && vectorAreas[i].second < vectorAreas[i+1].second){
            ok = 2;
            minPos.push_back(i);
        }
    }


    minPos.push_back(269);
    for(int i=0; i<minPos.size()-1; i++)
    {
        int min3 = minPos[i];
        int max3 = maxPos[i];
        float sum=0;
        float nrX=0, nrY=0, avY=0;

        int ok1 = (maxPos[i] >= 270 || maxPos[i] <= -270);
        int ok11 = (minPos[i] >= 270 || minPos[i] <= -270);
        int ok111 = (minPos[i+1] >= 270 || minPos[i+1] <= -270);
        ///we determine whether the maxpos or minposition is outside of index, in order to not cause segfault
        if(ok1 || ok11 || ok111) {
            continue;
        }


        //// we level the terrain in order for the maximum and minimum points of a certain portions
        /// to below a threshold
        if(vectorAreas[max3].second - vectorAreas[min3].second > threshold)
        {

            int k=1;
            vector<float> vecElements;
            float av = ((vectorAreas[min3].second) + (vectorAreas[min3+1].second))/2;
            vecElements.push_back(av);

            for(int j=min3+1; j<=max3-1; j++){
                av = ((vectorAreas[j-1].second) + (vectorAreas[j].second) + (vectorAreas[j+1].second))/3;
                vecElements.push_back(av);
            }
            /// we do that through the mean calculation that helps us level the terrain
            av = ((vectorAreas[max3-1].second) + (vectorAreas[max3].second))/2;
            vecElements.push_back(av);

            for(int j=min3; j<=max3; j++) {
                vectorAreas[j].second = vecElements[j-min3];
            }

            num+=1;

        }

        /// here we are dealing with terrain that is going up in height
        int formerMin = min3;
        min3 = minPos[i+1];

        /// here are we dealing with the terrain that is going down in height
        if(vectorAreas[max3].second - vectorAreas[min3].second > threshold)
        {

            int k=1;
            vector<float> vecElements;
            float av = ((vectorAreas[max3].second) + (vectorAreas[max3+1].second))/2;
            vecElements.push_back(av);

            for(int j=max3+1; j<=min3-1; j++){
                av = ((vectorAreas[j-1].second) + (vectorAreas[j].second) + (vectorAreas[j+1].second))/3;
                vecElements.push_back(av);
            }
            av = ((vectorAreas[min3-1].second) + (vectorAreas[min3].second))/2;
            vecElements.push_back(av);
            ///we do the same things for the next point, in but the

            for(int j=max3; j<=min3; j++) {
                vectorAreas[j].second = vecElements[j-max3];
            }

            num+=1;

        }

    }



    // we are modelling the terrain here
    for(int i=0;i<270;i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D_2::Translate2(vectorAreas[i], vectorAreas[i+1]);
        modelMatrix *= transform2D_2::Shearing(vectorAreas[i], vectorAreas[i+1]);
        modelMatrix *= transform2D_2::Scale2(vectorAreas[i], vectorAreas[i+1]);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    }





    if(hpTank1>0 )
    {
        int is_up_then = 0;
        int is_up_first=0;
        int is_down_then=0;
        int is_down_first = 0;
        float min=-1, max=-1;


        for(int i=0;i<270;i++) {

            if(TankMovement.first <= vectorAreas[i].first && TankMovement.first  + size >= vectorAreas[i].first) {
                ok = 1;

                if(min == -1 || min > vectorAreas[i].second) {
                    min = vectorAreas[i].second;
                }

                if(max == -1 || max < vectorAreas[i].second) {
                    max = vectorAreas[i].second;
                }

                /// we get the maximum and minimum points of the tank

            } else {
                ok = 0;
            }

            if(vectorAreas[i].second > vectorAreas[i+1].second && ok==1 ) {
                if(is_down_first == 0 && is_up_first != 1) {
                    is_down_first = 1;
                } else if(is_down_then == 0 && is_up_first == 1) {
                    is_down_then = 1;
                }

                /// we check if the first portion is down,or the last portion is down

            }

            if(ok==1 && vectorAreas[i].second < vectorAreas[i+1].second) {
                if(is_up_first == 0 && is_down_first != 1) {
                    is_up_first = 1;
                } else if(is_up_then == 0 && is_down_first == 1) {
                    is_up_then = 1;
                }

                /// we check here if the first portion is up, or the last portion is up
            }
            

        }


        angle/=nr;

        modelMatrix = glm::mat3(1);
        float initialDec = 25;
        float elementDif = 0;
        /// we model our tank depending of the curve, if it is convex or concave
        if(is_up_first == 1 && is_down_then == 1) { /// concave

            size_of_step = speeds1 * modelSizeX/24/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);
            /// slower
            elementDif +=((max-min)/4);
            initialDec +=(2*(max-min)/5);
            ///here
        } else if(is_down_first == 1 && is_up_then == 1){ /// convex
            size_of_step =  speeds1 * modelSizeX/16/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);
            /// here is faster
            elementDif -=((max-min)/2);
            initialDec -=((max-min)/4);
        } else if(is_up_first == 1) { /// upward
            /// we make the tank faster or slower depeding if he is moving left or right

            if(moveRightTank1) {
                size_of_step = speeds1 * modelSizeX/36/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);
            } else {
                size_of_step = speeds1 * modelSizeX/12/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);
            }
            initialDec +=((max-min)/6);
        } else if(is_down_first == 1) ///downward
        {
            /// we make the tank faster or slower depeding if he is moving left or right
            if(moveRightTank1){
                size_of_step = speeds1 * modelSizeX/12/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);
            } else {
                size_of_step = speeds1 * modelSizeX/36/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);

            }
            initialDec +=((max-min)/8);
        }

        TankMovement.second-=initialDec;
        /// we molded the tank in order to be visually pleasing when we move
        /// the tank in different terrain
        modelMatrix *= transform2D_2::Translate(TankMovement.first, TankMovement.second);

        modelMatrix *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
        modelMatrix *= transform2D_2::Rotate(current_angle);
        modelMatrix *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);
        RenderMesh2D(meshes["tankTracks"], shaders["VertexColor"], modelMatrix);
        /// this one is for the tank body

        modelMatrix *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);
        RenderMesh2D(meshes["bodyTank"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D_2::Translate(topSizeX/2, topSizeY);

        /// this is the turret
        RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
        glm::mat3 mM3 = modelMatrix, mM4=modelMatrix;
        mM3 *= transform2D_2::Translate(-sizeBar/2,placeHeightBar);
        mM4 *= transform2D_2::Translate(-sizeBar/2,placeHeightBar);
        mM4*=transform2D_2::Scale(hpTank1/100, 1);
        /// we designed a health bar
        RenderMesh2D(meshes["HealthBar"], shaders["VertexColor"], mM4);
        RenderMesh2D(meshes["fullHealthBar"], shaders["VertexColor"], mM3);

        glm::mat3 preMatrix = modelMatrix;
        float radianAngleTurret = currentTurretAngle * (float)M_PI/180.0;
        glm::vec2 Pos2= glm::vec2(100* cos(radianAngleTurret), 100 * sin(radianAngleTurret));
        glm::vec2 vec2= glm::vec2(100* cos(radianAngleTurret), 100 * sin(radianAngleTurret));


        /// we calculate the trajectory of the ball
        if(!ball_is_moving) {
            int ok1=0;
            while(!ok1) {
                modelMatrix3 = glm::mat3(1);

                modelMatrix3 *= transform2D_2::Translate(TankMovement.first, TankMovement.second);

                modelMatrix3 *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
                modelMatrix3 *= transform2D_2::Rotate(current_angle);
                modelMatrix3 *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);



                modelMatrix3 *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);



                modelMatrix3 *= transform2D_2::Translate(topSizeX/2, topSizeY);
                glm::vec3 position2 = modelMatrix3 * glm::vec3(0, 0, 1);
                float x1 = position2.x;
                float y1 = position2.y;

                glm::vec2 tmp_Pos = Pos2;

                modelMatrix3 *= transform2D_2::Translate(Pos2.x,Pos2.y);
                /// this is showing the trajectory of the ball with white
                RenderMesh2D(meshes["circle4"], shaders["VertexColor"], modelMatrix3);
                glm::vec3 position = modelMatrix3 * glm::vec3(0, 0, 1);
                float x = position.x;
                float y = position.y;


                int P_A = (int)x - (int)x % 10;
                int P_B = (int)x - (int)x % 10 + 10;

                if(P_A/10 < 0 || P_A/10>=270 || P_B/10 < 0 || P_B/10 >= 270) {
                    break;
                }
                //// we calculate if the index number is outside of our range
                /// in order to not crash the program

                pair<float, float> PP_A = vectorAreas[P_A/10];
                pair<float, float> PP_B = vectorAreas[P_B/10];

                modelMatrix4 = glm::mat3(1);

                modelMatrix4 *= transform2D_2::Translate(PP_A.first, PP_B.second);

                modelMatrix4 *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
                modelMatrix4 *= transform2D_2::Rotate(current_angle);
                modelMatrix4 *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);



                modelMatrix4 *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);



                modelMatrix4 *= transform2D_2::Translate(topSizeX/2, topSizeY);
                glm::vec3 position3 = modelMatrix4 * glm::vec3(0, 0, 1);
                float x2 = position3.x;
                float y2 = position3.y;

                tmp_Pos+= (vec2 * (float)0.0165 * (float)0.25);
                Pos2 = tmp_Pos;
                vec2 -= (a * (float)0.0165 * (float)1);

                float num1 = y-PP_A.second, num2 = y-PP_B.second;
                float num1X = x-PP_A.first, num2X = x-PP_B.first;
                float nearTankx = x-x1;


                if(nearTankx<=topSizeX/2 && nearTankx>=-topSizeX/2)
                {
                    if((y < y2 + elementDif) && (num1X <=-5 || num2X <=-5))
                    {
                        ok1=1;
                    }
                }

                if((num1 <=-5 || num2 <=-5) && (num1X <=-5 || num2X <=-5)){
                    ok1=1;
                }

            }


        }

        preMatrix *= transform2D_2::Rotate(radianAngleTurret);
        preMatrix *= transform2D_2::Translate(30 , 0);
        /// this is the main gun
        RenderMesh2D(meshes["rect"], shaders["VertexColor"], preMatrix);
    }


    /// this is the same but for the seconnd tank
    if(hpTank2>0)
    {

        float is_down_first2=0, is_up_first2=0, is_down_then2=0, is_up_then2=0;
        float min2=-1, max2=-1;
        for(int i=0;i<270;i++) {

            if(TankMovement2.first <= vectorAreas[i].first && TankMovement2.first  + size2 >= vectorAreas[i].first) {
                ok = 1;
                if(min2 == -1 || min2 > vectorAreas[i].second) {
                    min2 = vectorAreas[i].second;
                }

                if(max2 == -1 || max2 < vectorAreas[i].second) {
                    max2 = vectorAreas[i].second;
                }

            } else {
                ok = 0;
            }

            if(vectorAreas[i].second > vectorAreas[i+1].second && ok==1 ) {
                if(is_down_first2 == 0 && is_up_first2 != 1) {
                    is_down_first2 = 1;
                } else if(is_down_then2 == 0 && is_up_first2 == 1) {
                    is_down_then2 = 1;
                }
            }

            if(ok==1 && vectorAreas[i].second < vectorAreas[i+1].second) {
                if(is_up_first2 == 0 && is_down_first2 != 1) {
                    is_up_first2 = 1;
                } else if(is_up_then2 == 0 && is_down_first2 == 1) {
                    is_up_then2 = 1;
                }
            }

        }

        modelMatrix = glm::mat3(1);
        float initialDec = 25;
        float elementDif = 0;
        if(is_up_first2 == 1 && is_down_then2 == 1) {

            size_of_step2 = speeds2 * modelSizeX/24/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
            elementDif +=((max2-min2)/4);
            initialDec +=(2*(max2-min2)/5);
        } else if(is_down_first2 == 1 && is_up_then2 == 1){
            size_of_step2 = speeds2 * modelSizeX/16/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);

            elementDif -=((max2-min2)/2);
            initialDec -=((max2-min2)/4);
        } else if(is_up_first2 == 1) {
            if(moveRightTank2) {
                size_of_step2 = speeds2 * modelSizeX/36/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
            } else {
                size_of_step2 = speeds2 * modelSizeX/12/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
            }
            initialDec +=((max2-min2)/6);
        } else if(is_down_first2 == 1)
        {
            if(moveRightTank2) {
                size_of_step2 = speeds2 * modelSizeX/12/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
            } else {
                size_of_step2 = speeds2 * modelSizeX/36/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
            }
            initialDec +=((max2-min2)/8);
        }

        TankMovement2.second-=initialDec;

        modelMatrix *= transform2D_2::Translate(TankMovement2.first, TankMovement2.second);

        modelMatrix *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
        modelMatrix *= transform2D_2::Rotate(current_angle2);
        modelMatrix *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);


        RenderMesh2D(meshes["tank2Tracks"], shaders["VertexColor"], modelMatrix);

        modelMatrix *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);
        RenderMesh2D(meshes["bodyTank2"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D_2::Translate(topSizeX/2, topSizeY);

        RenderMesh2D(meshes["Tank2circle"], shaders["VertexColor"], modelMatrix);
        glm::mat3 mM3 = modelMatrix, mM4=modelMatrix;
        mM3 *= transform2D_2::Translate(-sizeBar/2,placeHeightBar);
        mM4 *= transform2D_2::Translate(-sizeBar/2,placeHeightBar);

        mM4*=transform2D_2::Scale(hpTank2/100, 1);

        RenderMesh2D(meshes["HealthBar"], shaders["VertexColor"], mM4);
        RenderMesh2D(meshes["fullHealthBar"], shaders["VertexColor"], mM3);

        glm::mat3 preMatrix = modelMatrix;
        float radianAngleTurret = currentTurretAngle2 * (float)M_PI/180.0;
        glm::vec2 Pos2= glm::vec2(100* cos(radianAngleTurret), 100 * sin(radianAngleTurret));
        glm::vec2 vec2= glm::vec2(100* cos(radianAngleTurret), 100 * sin(radianAngleTurret));



        if(!ball_is_moving2) {
            int ok1=0;
            while(!ok1) {
                modelMatrix3 = glm::mat3(1);

                modelMatrix3 *= transform2D_2::Translate(TankMovement2.first, TankMovement2.second);

                modelMatrix3 *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
                modelMatrix3 *= transform2D_2::Rotate(current_angle2);
                modelMatrix3 *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);



                modelMatrix3 *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);



                modelMatrix3 *= transform2D_2::Translate(topSizeX/2, topSizeY);
                glm::vec3 position2 = modelMatrix3 * glm::vec3(0, 0, 1);
                float x1 = position2.x;
                float y1 = position2.y;

                glm::vec2 tmp_Pos = Pos2;

                modelMatrix3 *= transform2D_2::Translate(Pos2.x,Pos2.y);
                RenderMesh2D(meshes["circle4"], shaders["VertexColor"], modelMatrix3);
                glm::vec3 position = modelMatrix3 * glm::vec3(0, 0, 1);
                float x = position.x;
                float y = position.y;

                int P_A = (int)x - (int)x % 10;
                int P_B = (int)x - (int)x % 10 + 10;

                if(P_A/10 < 0 || P_A/10>269 || P_B/10 < 0 || P_B/10 > 269) {
                    break;
                }

                pair<float, float> PP_A = vectorAreas[P_A/10];
                pair<float, float> PP_B = vectorAreas[P_B/10];

                modelMatrix4 = glm::mat3(1);

                modelMatrix4 *= transform2D_2::Translate(PP_A.first, PP_B.second);

                modelMatrix4 *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
                modelMatrix4 *= transform2D_2::Rotate(current_angle2);
                modelMatrix4 *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);

                modelMatrix4 *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);
                modelMatrix4 *= transform2D_2::Translate(topSizeX/2, topSizeY);
                glm::vec3 position3 = modelMatrix4 * glm::vec3(0, 0, 1);
                float y2 = position3.y;

                tmp_Pos+= (vec2 * (float)0.0165 * (float)0.25);
                Pos2 = tmp_Pos;
                vec2 -= (a * (float)0.0165 * (float)1.0);

                float num1 = y-PP_A.second, num2 = y-PP_B.second;
                float num1X = x-PP_A.first, num2X = x-PP_B.first;

                float nearTankx = x-x1;

                if(nearTankx<=topSizeX/2 && nearTankx>=-topSizeX/2)
                {
                    if((y < y2 + elementDif) && (num1X <=-5 || num2X <=-5))
                    {
                        ok1=1;
                    }
                }

                if((num1 <=-5 || num2 <=-5) && (num1X <=-5 || num2X <=-5)){
                    ok1=1;
                }

            }

        }

        preMatrix *= transform2D_2::Rotate(radianAngleTurret);
        preMatrix *= transform2D_2::Translate(30 , 0);
        RenderMesh2D(meshes["rect"], shaders["VertexColor"], preMatrix);
    }



    for(int i=0;i<vectorV.size();i++)
    {
        modelMatrix2 = glm::mat3(1);
        glm::mat3 AmT1=modelMatrix2, AmT2 = modelMatrix2,AmT1up,AmT1up2, AmT2up, AmT2up2;
        modelMatrix2 *= transform2D_2::Translate(vectorFormerTank[i].first, vectorFormerTank[i].second);

        AmT1 *= transform2D_2::Translate(TankMovement.first, TankMovement.second);
        AmT2 *= transform2D_2::Translate(TankMovement2.first, TankMovement2.second);

        modelMatrix2 *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
        modelMatrix2 *= transform2D_2::Rotate(vectorFormerAngle[i]);
        modelMatrix2 *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);

        AmT1 *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
        AmT1 *= transform2D_2::Rotate(vectorFormerAngle[i]);
        AmT1 *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);

        AmT2 *= transform2D_2::Translate(modelSizeX/2, modelSizeY/2);
        AmT2 *= transform2D_2::Rotate(vectorFormerAngle[i]);
        AmT2 *= transform2D_2::Translate(-modelSizeX/2, -modelSizeY/2);

        modelMatrix2 *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);
        AmT1up = AmT1;
        AmT2up = AmT2;

        AmT1up *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY+topSizeY);
        AmT2up *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY+topSizeY);

        AmT1 *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);
        AmT2 *= transform2D_2::Translate((modelSizeX-topSizeX)/2, modelSizeY);

        glm::vec3 position11 = AmT1 * glm::vec3(0, 0, 1);
        glm::vec3 position11Up = AmT1up * glm::vec3(0, 0, 1);
        glm::vec3 position22Up = AmT2up * glm::vec3(0, 0, 1);

        glm::vec3 position22 = AmT2 * glm::vec3(0, 0, 1);

        modelMatrix2 *= transform2D_2::Translate(topSizeX/2, topSizeY);
        AmT1 *= transform2D_2::Translate(topSizeX, topSizeY);

        AmT2 *= transform2D_2::Translate(topSizeX, topSizeY);

        glm::vec3 position111 = AmT1 * glm::vec3(0, 0, 1);
        glm::vec3 position222 = AmT2 * glm::vec3(0, 0, 1);

        glm::vec2 tmp_Pos = vectorPos[i];

        modelMatrix2 *= transform2D_2::Translate(vectorPos[i].x,vectorPos[i].y);

        if(vectorColor[i] == 0) {
            RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix2);
        } else if(vectorColor[i] == 1){
            RenderMesh2D(meshes["circleFrozen"], shaders["VertexColor"], modelMatrix2);
        } else {
            RenderMesh2D(meshes["circleHurt"], shaders["VertexColor"], modelMatrix2);

        }

        glm::vec3 position = modelMatrix2 * glm::vec3(0, 0, 1);
        float x = position.x;
        float y = position.y;

        int P_A = (int)x - (int)x % 10;
        int P_B = (int)x - (int)x % 10 + 10;


        if(P_A/10 < 0 || P_A/10>269 || P_B/10 < 0 || P_B/10 > 269) {

            vectorV.erase(vectorV.begin() + i);
            vectorPos.erase(vectorPos.begin() + i);
            vectorFormerAngle.erase(vectorFormerAngle.begin() + i);
            vectorFormerTurrentAngle.erase(vectorFormerTurrentAngle.begin() + i);
            vectorFormerTank.erase(vectorFormerTank.begin() + i);
            vectorColor.erase(vectorColor.begin() + i);

            i-=1;
            continue;
        }
        /// we automatically delete the ball if it is outside of our area

        pair<float, float> PP_A = vectorAreas[P_A/10];
        pair<float, float> PP_B = vectorAreas[P_B/10];
        /// the terrain coordinates nearest the ball

        tmp_Pos+= (vectorV[i] * deltaTimeSeconds * (float)5.0);
        vectorPos[i] = tmp_Pos;
        vectorV[i] -= (a * deltaTimeSeconds * (float)20.0);

        glm::vec2 currentCoord = glm::vec2(x, y);
        glm::vec2 VECPP_A = glm::vec2(PP_A.first, PP_A.second), VECPP_B =glm::vec2(PP_B.first, PP_B.second);
        float dist2 = glm::distance(VECPP_B, currentCoord);
        float dist = glm::distance(VECPP_A, currentCoord);


        int ok3 = 0;

        float num1 = y-PP_A.second, num2 = y-PP_B.second;
        int okGo=0, hitEnemy=0;
        /// we calculate if the ball is nearly the our coordinates(which represent the terrain)

        if(num1 <=-3 || num2 <=-3 || num1<=3 || num2<=3) {
            okGo=1;
        }

        /// we hit first tank(so we eliminate the ball)
        if( currentCoord.x>=position11.x && currentCoord.x <=position111.x && (currentCoord.y <= position11Up.y || currentCoord.y <= position111.y))
        {
            hitEnemy = 1;
            if(!isTank1Invincible && vectorColor[i]==0) {
                hpTank1-=15;
            }

            if(!isTank1Invincible && vectorColor[i]==2) {
                hpTank1-=30;
            }

            if(vectorColor[i]==1) {
                FrozenTank1Time = 3;
            }
            ok3=1;
        }

        /// we hit second tank(so we eliminate the ball)

        if(currentCoord.x>=position22.x && currentCoord.x <=position222.x && (currentCoord.y <= position22Up.y || currentCoord.y <= position222.y))
        {
            hitEnemy = 1;
            if(!isTank2Invincible && vectorColor[i]==0) {
                hpTank2-=15;
            }
            if(!isTank2Invincible && vectorColor[i]==2) {
                hpTank2-=30;
            }
            if(vectorColor[i]==1) {
                FrozenTank2Time = 3;
            }
            ok3=1;
        }


        /// it is nearly the terrain and the ball has not hitten an enemy
        if(okGo && !hitEnemy)
        {
            /// is the distance is smaller than a defined radius so the terrain deforms
            while(dist2 <= radius || dist <= radius) {
                ok3 = 1;
                if(dist2<= radius) {
                    vectorAreas[P_B/10].second -= sqrt(radius * radius - dist2 * dist2);
                    P_B +=10;
                    PP_B = vectorAreas[P_B/10];
                    VECPP_B =glm::vec2(PP_B.first, PP_B.second);
                    dist2 = glm::distance(VECPP_B, currentCoord);
                }

                if(dist <= radius) {

                    vectorAreas[P_A/10].second -= sqrt(radius * radius - dist * dist);
                    P_A -=10;
                    PP_A = vectorAreas[P_A/10];
                    VECPP_A =glm::vec2(PP_A.first, PP_A.second);
                    dist = glm::distance(VECPP_A, currentCoord);
                }
            }
        }


        /// if the criteria is met we eliminate the ball
        if(ok3==1) {
            vectorV.erase(vectorV.begin() + i);
            vectorPos.erase(vectorPos.begin() + i);
            vectorFormerAngle.erase(vectorFormerAngle.begin() + i);
            vectorFormerTurrentAngle.erase(vectorFormerTurrentAngle.begin() + i);
            vectorFormerTank.erase(vectorFormerTank.begin() + i);
            vectorColor.erase(vectorColor.begin() + i);

        }


    }

}


void Game::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Game::OnInputUpdate(float deltaTime, int mods)
{

    totalTime+=deltaTime;
    float step = cos(current_angle)*topSizeX;
    float step2 = cos(current_angle2)*topSizeX;


    /// this keys are for every movement
    if(keyState[GLFW_KEY_RIGHT] && FrozenTank1Time<=0) {
        moveRightTank1=true;


        if(t + size_of_step >= 1){
            enter_transition = 0;
            int ok = true;
            if(TankMovement.first < TankMovement2.first) {
                ok = TankMovement.first  + step<  TankMovement2.first;
            }
            if(currentElementIndex != 269 &&   (hpTank2 < 0 || ok) )
            {
                currentElementIndex+=speeds1;
                /// the speed of first Tank
                /// we have entered a new index
                t = 0; // t - represents a variable between the values (x, y) between our currentIndex and currentIndex+1

                current_angle = atan2((vectorAreas[currentElementIndex+1].second - vectorAreas[currentElementIndex].second),(vectorAreas[currentElementIndex+1].first - vectorAreas[currentElementIndex].first));
                size_of_step = modelSizeX/24/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);
                // the step the tank takes when I press the right button
                // current_angle of the tank
            } else {
                t=1;
                /// complete new step
            }
        } else {
            t+=size_of_step;
            /// we add it
        }

        //

    } else if(keyState[GLFW_KEY_LEFT] && FrozenTank1Time<=0) {
        moveRightTank1=false;

        if(t - size_of_step <= 0 && currentElementIndex!=0 && (hpTank2 < 0 || ((TankMovement.first < TankMovement2.first) || (TankMovement.first - step - 10 > TankMovement2.first)))){
            currentElementIndex-=speeds1;
            t = 0.9;
            /// we do the same for the left key, but reverse
            size_of_step = modelSizeX/24/(vectorAreas[currentElementIndex+1].first-vectorAreas[currentElementIndex].first);
        } else if(t - size_of_step <= 0)
        {
            // if it becomes smaller than 0 we give it automatically 0 the t is
            // between 0(it is closer the currentElementIndex) and 1(closer to the currrentElementIndex+1)
            t=0;
        } else {
            t-=size_of_step;
        }

    } else if (keyState[GLFW_KEY_UP] && FrozenTank1Time<=0) {
        if(currentTurretAngle > 0) {
            currentTurretAngle -=5;
        }
        /// the turret angle
    }else if (keyState[GLFW_KEY_H] && healthOne>0) {
        hpTank1+=30;
        if(hpTank1 > 100) {
            hpTank1=100;
        }
        healthOne-=1;
        keyState[GLFW_KEY_H] = false;

        /// if we haave a health power for tank 1

    }else if (keyState[GLFW_KEY_J] && healthTwo>0) {
        hpTank2+=30;
        if(hpTank2 > 100) {
            hpTank2=100;
        }
        healthTwo-=1;
        keyState[GLFW_KEY_J] = false;
        /// same for 2
    } else if (keyState[GLFW_KEY_2] && speedOne>0) {
        speeds1=2;
        speedOne--;
        speedTime1 = 5;
        keyState[GLFW_KEY_2]  = false;
        /// if we have a speed power for tank 1
    }else if (keyState[GLFW_KEY_3] && speedTwo>0) {
        speeds2=2;
        speedTwo--;
        speedTime2 = 5;
        keyState[GLFW_KEY_3]  = false;
        ///same for two
    } else if (keyState[GLFW_KEY_DOWN] && FrozenTank1Time<=0) {
        if(currentTurretAngle < 175) {
            currentTurretAngle +=5;
        }
        ///the turretGoesDown
    } else if((keyState[GLFW_KEY_ENTER] || (keyState[GLFW_KEY_I] && frozenOne>0) || (keyState[GLFW_KEY_M] && hurtOne>0)) && hpTank1>0 && FrozenTank1Time<=0) {


        if(keyState[GLFW_KEY_ENTER]) {
            vectorColor.push_back(0);
            keyState[GLFW_KEY_ENTER] = false;
        } else if(keyState[GLFW_KEY_I]) {
            vectorColor.push_back(1);
            frozenOne--;
            keyState[GLFW_KEY_I] = false;

            /// for freezing items
        } else {
            vectorColor.push_back(2);
            hurtOne--;
            keyState[GLFW_KEY_M] = false;

            /// for giving a ball which hurts more
        }

        float radianAngle = currentTurretAngle * (float)M_PI/180.0;
        formerCurrentTurretAngle = radianAngle;
        v = glm::vec2(cos(radianAngle) * 100, sin(radianAngle) * 100);

        ball_is_moving = true;
        vectorV.push_back(v);
        vectorFormerAngle.push_back(current_angle);
        /// vector of Former angle
        /// we use these former angles, when we shoot our turrent, and
        /// we use it in order to have a good trajectory of our ball throughout time

        vectorFormerTank.emplace_back(TankMovement.first, TankMovement.second);
        vectorFormerTurrentAngle.push_back(radianAngle);
        vectorPos.emplace_back(cos(radianAngle) * 100, sin(radianAngle) * 100);

        formerCurrentAngle = current_angle;
        formerTankMovement = TankMovement;
        formerCurrentTurretAngle = currentTurretAngle;
        current_key = GLFW_LOCK_KEY_MODS;
        /// reset our current key
    } else if(keyState[GLFW_KEY_D] &&  FrozenTank2Time<=0) {
        moveRightTank2=true;

        if(t2 + size_of_step2 >= 1){
            enter_transition2 = 0;
            if(currentElementIndex2!= 269 && (hpTank2 < 0 || (TankMovement2.first > TankMovement.first) || TankMovement2.first  + step2<  TankMovement.first))
            {
                currentElementIndex2+=(speeds1*1);
                t2 = 0;
                current_angle2 = atan2((vectorAreas[currentElementIndex2+1].second - vectorAreas[currentElementIndex2].second),(vectorAreas[currentElementIndex2+1].first - vectorAreas[currentElementIndex2].first));
                size_of_step2 = modelSizeX/24/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
            } else {
                t2=1;
            }
        } else {
            t2+=size_of_step2;
        }

        // i explained already for the first tank

    } else if(keyState[GLFW_KEY_A]  && FrozenTank2Time<=0) {
        moveRightTank2=false;
        bool ok = true;
        if(TankMovement2.first > TankMovement.first) {
            ok = (TankMovement2.first  > (TankMovement.first+step+3));
        }
        if(t2 - size_of_step2 <= 0 && currentElementIndex2!=0 && (hpTank1 < 0 || ok)){
            currentElementIndex2-=(speeds2*1);
            t2 = 0.9; ///
            size_of_step2 = modelSizeX/12/(vectorAreas[currentElementIndex2+1].first-vectorAreas[currentElementIndex2].first);
        } else if(t2 - size_of_step2 <= 0)
        {
            t2=0;
        } else {
            t2-=size_of_step2;
        }

    } else if (keyState[GLFW_KEY_W] && FrozenTank2Time<=0) {
        if(currentTurretAngle2 > 0) {
            currentTurretAngle2 -=5;
        }

    } else if (keyState[GLFW_KEY_S]  && FrozenTank2Time<=0) {
        if(currentTurretAngle2 < 175) {
            currentTurretAngle2 +=5;
        }
    } else if((keyState[GLFW_KEY_SPACE] || (keyState[GLFW_KEY_O]  && frozenTwo>0) || (keyState[GLFW_KEY_N] && hurtTwo>0)) && hpTank2>0 &&  FrozenTank2Time<=0) {

        if(keyState[GLFW_KEY_SPACE]) {
            vectorColor.push_back(0);
            keyState[GLFW_KEY_SPACE] = false;
        } else if(keyState[GLFW_KEY_O] ){
            vectorColor.push_back(1);
            keyState[GLFW_KEY_O] = false;

            frozenTwo--;
        } else if(keyState[GLFW_KEY_N] ){
            keyState[GLFW_KEY_N] = false;

            vectorColor.push_back(2);
            hurtTwo--;
        }

        float radianAngle = currentTurretAngle2 * (float)M_PI/180.0;
        formerCurrentTurretAngle2 = radianAngle;
        v = glm::vec2(cos(radianAngle) * 100, sin(radianAngle) * 100);

        ball_is_moving2 = true;
        vectorV.push_back(v);
        vectorFormerAngle.push_back(current_angle2);

        vectorFormerTank.emplace_back(TankMovement2.first, TankMovement2.second);
        vectorFormerTurrentAngle.push_back(radianAngle);
        vectorPos.emplace_back(cos(radianAngle) * 100, sin(radianAngle) * 100);



        formerCurrentAngle = current_angle2;
        formerTankMovement = TankMovement2;
        formerCurrentTurretAngle = currentTurretAngle2;
        current_key = GLFW_LOCK_KEY_MODS;
    }
}


void Game::OnKeyPress(int key, int mods)
{
    current_key = key;
    keyState[key] = true;
    // Add key press event
}


void Game::OnKeyRelease(int key, int mods)
{
    // Add key release event2
    keyState[key] = false;

    ball_is_moving = false;
    ball_is_moving2 = false;

    current_key = GLFW_LOCK_KEY_MODS;
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Game::OnWindowResize(int width, int height)
{
}
