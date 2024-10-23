#include <iostream>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "core/scene.h"


#include "shapes/sphere.h"
#include "shapes/infiniteplan.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/normalshader.h"
#include "shaders/whittedshader.h"
#include "shaders/hemisdirectlightshader.h"
#include "shaders/areadirectlightshader.h"
#include "shaders/purepathtracingshader.h"
#include "shaders/nexteventshader.h"
#include "shaders/nexteventshadermirror.h"


#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/transmission.h"
#include "materials/emissive.h"

#include <chrono>

using namespace std::chrono;

typedef std::chrono::duration<double, std::milli> durationMs;



void buildSceneCornellBox(Camera*& cam, Film*& film,
    Scene myScene, int mode)
{
    /* **************************** */
/* Declare and place the camera */
/* **************************** */
    Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ********* */
    /* Materials */
    /* ********* */
    Material* redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
    Material* greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
    Material* greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
    Material* blueGlossy_20 = new Phong(Vector3D(0.2, 0.3, 0.8), Vector3D(0.2, 0.2, 0.2), 20);
    Material* blueGlossy_80 = new Phong(Vector3D(0.2, 0.3, 0.8), Vector3D(0.2, 0.2, 0.2), 80);
    Material* cyandiffuse = new Phong(Vector3D(0.2, 0.8, 0.8), Vector3D(0, 0, 0), 100);
    Material* emissive = new Emissive(Vector3D(25.0, 25.0, 25.0), Vector3D(0.5));

    Material* electricPinkDiffuse = new Phong(Vector3D((double)246 / 255, (double)38 / 255, (double)129 / 255), Vector3D(0, 0, 0), 100); // Electric pink para la puti light
    Material* baddieDiffuse = new Phong(Vector3D((double)186 / 255, (double)221 / 255, (double)30 / 255), Vector3D(0, 0, 0), 100); // Baddie bien brat

    Material* mirror = new Mirror();
    Material* transmissive = new Transmissive(0.7);

    /* ******* */
    /* Objects */
    /* ******* */
    double offset = 3.0;
    Matrix4x4 idTransform;
    // Construct the Cornell Box
    Shape* leftPlan = new InfinitePlan(Vector3D(-offset - 1, 0, 0), Vector3D(1, 0, 0), electricPinkDiffuse);
    Shape* rightPlan = new InfinitePlan(Vector3D(offset + 1, 0, 0), Vector3D(-1, 0, 0), baddieDiffuse);
    Shape* topPlan = new InfinitePlan(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
    Shape* bottomPlan = new InfinitePlan(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
    Shape* backPlan = new InfinitePlan(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), greyDiffuse);
    


    myScene.AddObject(leftPlan);
    myScene.AddObject(rightPlan);
    myScene.AddObject(topPlan);
    myScene.AddObject(bottomPlan);
    myScene.AddObject(backPlan);
    


    // Place the Spheres inside the Cornell Box
    double radius = 1;
    Matrix4x4 sphereTransform1;
    sphereTransform1 = Matrix4x4::translate(Vector3D(1.5, -offset + radius, 6));
    Shape* s1 = new Sphere(radius, sphereTransform1, blueGlossy_20);

    Matrix4x4 sphereTransform2;
    sphereTransform2 = Matrix4x4::translate(Vector3D(-1.5, -offset + 3 * radius, 4));
    Shape* s2;
    Shape* square;
    
    if (mode == 0) {
        PointLightSource* myPointLight = new PointLightSource(Vector3D(0, 2.5, 3.0), Vector3D(2.0));
        myScene.AddPointLight(myPointLight);

        //Shape* square_emissive = new Square(Vector3D(-1.0, 3.0, 3.0), Vector3D(2.0, 0.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(0.0, -1.0, 0.0), emissive);
        //myScene.AddObject(square_emissive);

        s2 = new Sphere(radius, sphereTransform2, blueGlossy_80);

        square = new Square(Vector3D(offset + 0.999, -offset - 0.2, 3.0), Vector3D(0.0, 4.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(-1.0, 0.0, 0.0), greenDiffuse);

    }
    else if (mode == 1) {
        Shape* square_emissive = new Square(Vector3D(-1.0, 3.0, 3.0), Vector3D(2.0, 0.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(0.0, -1.0, 0.0), emissive);
        myScene.AddObject(square_emissive);

        s2 = new Sphere(radius, sphereTransform2, blueGlossy_80);

        square = new Square(Vector3D(offset + 0.999, -offset - 0.2, 3.0), Vector3D(0.0, 4.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(-1.0, 0.0, 0.0), greenDiffuse);
        

    }
    else if (mode == 2) {
        Shape* square_emissive = new Square(Vector3D(-1.0, 3.0, 3.0), Vector3D(2.0, 0.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(0.0, -1.0, 0.0), emissive);
        myScene.AddObject(square_emissive);

        s2 = new Sphere(radius, sphereTransform2, transmissive);

        square = new Square(Vector3D(offset + 0.999, -offset - 0.2, 3.0), Vector3D(0.0, 4.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(-1.0, 0.0, 0.0), mirror);
    }
    else {
        //PointLightSource* myPointLight = new PointLightSource(Vector3D(0, 2.5, 3.0), Vector3D(2.0));
        PointLightSource* putiLight = new PointLightSource(Vector3D(0, 2.5, 3.0), Vector3D((double)(128 * 3) / 255, (double)0 / 255, (double)(255 * 3) / 255));

        //myScene.AddPointLight(myPointLight);
        myScene.AddPointLight(putiLight);

        s2 = new Sphere(radius, sphereTransform2, transmissive);

        square = new Square(Vector3D(offset + 0.999, -offset - 0.2, 3.0), Vector3D(0.0, 4.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(-1.0, 0.0, 0.0), mirror);

    }

    myScene.AddObject(s1);
    myScene.AddObject(s2);
    myScene.AddObject(square);
   
}

void buildSceneSphere(Camera*& cam, Film*& film,
    Scene myScene)
{
    /* **************************** */
      /* Declare and place the camera */
      /* **************************** */
      // By default, this gives an ID transform
      //  which means that the camera is located at (0, 0, 0)
      //  and looking at the "+z" direction
    Matrix4x4 cameraToWorld;
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);



    /* ************************** */
    /* DEFINE YOUR MATERIALS HERE */
    /* ************************** */

    Material* green_100 = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);

    // Define and place a sphere
    Matrix4x4 sphereTransform1;
    sphereTransform1 = sphereTransform1.translate(Vector3D(-1.25, 0.5, 4.0));
    Shape* s1 = new Sphere(1.0, sphereTransform1, green_100);

    // Define and place a sphere
    Matrix4x4 sphereTransform2;
    sphereTransform2 = sphereTransform2.translate(Vector3D(1.25, 0.0, 6));
    Shape* s2 = new Sphere(1.25, sphereTransform2, green_100);

    // Define and place a sphere
    Matrix4x4 sphereTransform3;
    sphereTransform3 = sphereTransform3.translate(Vector3D(1.0, -0.75, 3.5));
    Shape* s3 = new Sphere(0.25, sphereTransform3, green_100);

    // Store the objects in the object list
    myScene.AddObject(s1);
    myScene.AddObject(s2);
    myScene.AddObject(s3);
   
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<LightSource*>* &lightSourceList)
{
    
    double my_PI = 0.0;
    double n_estimations = 0.0;
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression 
        double progress = (double)lin / double(resY);
        Utils::printProgress(progress);

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;
            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);
            Vector3D pixelColor = Vector3D(0.0);

            // Compute ray color according to the used shader
            pixelColor += shader->computeColor(cameraRay, *objectsList, *lightSourceList);

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }


}


//------------TASK 1---------------------//
void PaintImage(Film* film)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main Image Loop
    for (size_t lin = 0; lin < resY; lin++)
    {
        // Show progression
        if (lin % (resY / sizeBar) == 0)
            std::cout << ".";

        for (size_t col = 0; col < resX; col++)
        { 
            Vector3D pixel_color = Vector3D((double)col / resX, (double)lin / resY, 0);
            //Vector3D random_color = Vector3D((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);            
            film->setPixelValue(col,lin, pixel_color);
           
        }
    }
}

double chooseTask(double labChosen) {
    double taskChosen = 0;
    std::string separator = "\n----------------------------------------------\n";

    if (labChosen == 1) {
        system("cls");
        std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

        std::cout << separator << "Choose what shader you want to use, options for Lab1" << separator << std::endl;
        std::cout << "0 - Paint image (no shader)\n" << std::endl;
        std::cout << "1 - Intersection Shader\n" << std::endl;
        std::cout << "2 - Depth Shader\n" << std::endl;
        std::cout << "3 - Normal Shader\n" << std::endl;
        std::cout << "4 - Whitted Shader\n" << separator << std::endl;

        std::cout << "Choose task: ";
        std::cin >> taskChosen;
    }
    else if (labChosen == 2) {
        system("cls");
        std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

        std::cout << separator << "Choose what shader you want to use, options for Lab2" << separator << std::endl;
        std::cout << "0 - Hemispherical Direct Illumination Shader\n" << std::endl;
        std::cout << "1 - Area Direct Illumination Shader\n" << std::endl;
        std::cout << "2 - Pure Path Tracing Shader\n" << std::endl;
        std::cout << "3 - Next Event Estimation Shader\n" << std::endl;
        std::cout << "4 - Next Event Estimation Shader (MIRROR AND TRANSMISSIVE)\n" << separator << std::endl;

        std::cout << "Choose task: ";
        std::cin >> taskChosen;
    }

    return taskChosen;
}

int main()
{
    double labChosen = 0;
    double taskChosen = 0;

    // Choose what task to show 
    //
    //----------------------------------------------------------------------------------------------------------
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

    std::cout << separator << "Choose what lab options to show:" << separator << std::endl;

    std::cout << "1 - Lab 1 | Whitted Ray Tracing\n" << std::endl;
    std::cout << "2 - Lab 2 | Full Path Tracer\n" << separator << std::endl;

    std::cout << "Choose Lab: ";
    std::cin >> labChosen;

    taskChosen = chooseTask(labChosen);

    system("cls");
    //----------------------------------------------------------------------------------------------------------
    //

    std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

    // Create an empty film
    Film* film;
    film = new Film(720, 512);

    // Build the scene---------------------------------------------------------
    // 
    // Declare pointers to all the variables which describe the scene
    Camera* cam;

    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)

    Shader* currentShader;
    Scene currentScene;

    if (labChosen == 1) {
        Scene myScene;
        Scene myCornellBox;

        //Create Scene Geometry and Illumiantion
        buildSceneSphere(cam, film, myScene); //Task 1,2,3;
        buildSceneCornellBox(cam, film, myCornellBox, 0); //Task 4

        if (taskChosen == 0) {
            PaintImage(film);

            // Save the final result to file
            std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
            film->save();
            film->saveEXR();
            exit(0);
        }  else if (taskChosen == 1) {
            //First task (IntersectionShader: Ray intersects and paints all object using one color)
            Vector3D intersectionColor((double)246 / 255, (double)38 / 255, (double)129 / 255); // Electric Pink
            currentShader = new IntersectionShader(intersectionColor, bgColor);
            currentScene = myScene;

        } else if (taskChosen == 2) {
            //Second task (DepthShader: Ray intersects and paints objects closer objects brighter than furthest objects)
            Vector3D depthColor((double)186 / 255, (double)221 / 255, (double)30 / 255); // Color = #BADD1E
            currentShader = new DepthShader(depthColor, 7.5f, bgColor);
            currentScene = myScene;

        } else if (taskChosen == 3) {
            //Third task (NormalShader: Ray intersects and paints all object depending on the coordinates of the point in the objects)
            currentShader = new NormalShader(bgColor);
            currentScene = myScene;

        } else if (taskChosen == 4) {
            //Fourth task (WhtitedShader: Ray intersects and paints all objects depending on the materials)
            currentShader = new WhittedShader(bgColor);
            currentScene = myCornellBox;

        } else {
            std::cout << separator << "\nWrong task selected, execute code again\n" << separator << std::endl;
            exit(-1);
        }
    }
    else if (labChosen == 2) {
        Scene myAreaCornellBox;

        //Create Scene Geometry and Illumiantion
        
        if (taskChosen == 0) {
            buildSceneCornellBox(cam, film, myAreaCornellBox, 2); //Lab 2
            currentScene = myAreaCornellBox;
            //First Task (HemisphericalDirectIlluminationShader: Ray intersects with a pixel and creates a hemisphere with random rays to calculate how light is perceived in point)
            currentShader = new HemisphericalDirectLightShader(32, 0.2);

        } else if (taskChosen == 1) {
            buildSceneCornellBox(cam, film, myAreaCornellBox, 2); //Lab 2
            currentScene = myAreaCornellBox;
            //Second Task (AreaDirectIlluminationShader: Ray intersects with a pixel and computes light with n random points from area light)
            currentShader = new AreaDirectLightShader(32,0.2);

        } else if (taskChosen == 2) {
            buildSceneCornellBox(cam, film, myAreaCornellBox, 2); //Lab 2
            currentScene = myAreaCornellBox;
            //Third Task (PurePathTracingShader: Ray intersects with a pixel and computes radiance with the bounces)
            currentShader = new PurePathTracingShader(32, 4);

        } else if (taskChosen == 3) {
            buildSceneCornellBox(cam, film, myAreaCornellBox, 1); //Lab 2
            currentScene = myAreaCornellBox;
            //Fourth Task (NextEventShader: Too much info see report)
            currentShader = new NextEventShader(64, 4);

        } else if (taskChosen == 4) {
            buildSceneCornellBox(cam, film, myAreaCornellBox, 2); //Lab 2
            currentScene = myAreaCornellBox;
            //Fourth Task (...)
            currentShader = new NextEventShaderMirror(32, 4);

        } else {
            std::cout << separator << "\nWrong task selected, execute code again\n" << separator << std::endl;
            exit(-1);
        }

    }
    else {
        std::cout << separator << "\nWrong lab selected, execute code again\n" << separator << std::endl;
        exit(-1);
    }

    // Launch some rays!  
    auto start = high_resolution_clock::now();
    raytrace(cam, currentShader, film, currentScene.objectsList, currentScene.LightSourceList);
    auto stop = high_resolution_clock::now();

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();
    film->saveEXR();

    float durationS = (durationMs(stop - start) / 1000.0).count() ;
    std::cout <<  "FINAL_TIME(s): " << durationS << std::endl;

    std::cout << "\n\n" << std::endl;
    return 0;
}
