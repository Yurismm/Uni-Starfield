#include <HAPI_lib.h>
#include <iostream>

using namespace HAPISPACE;

// star structure
struct Star {
    float x;
    float y;
    float z;

    Star() : x(1000), y(600), z(500) {};
};



void HAPI_Main()
{
    const HAPI_TKeyboardData& keyData = HAPI.GetKeyboardData();

    // screen size
    int width{ 1280 };
    int height{ 720 };

    // init window
    if (!HAPI.Initialise(width, height))
        return; // error handle

    // gets the screen's top left corner
    BYTE* screen{ HAPI.GetScreenPointer() };

    // fps
    HAPI.SetShowFPS(true);

    // number of stars
    const int numStars = 5000;

    // array of stars from the structure
    Star stars[numStars];

    // eye distance 
    // additional: the smaller it is the closer / faster it feels
    float EDistance = 200;

    // prevent weird screen effects when modifying these values
    float Max_Edistance = 500;
    float Min_Edistance = 50;

    // middle of the screen
    float cx = width / 2;
    float cy = height / 2;

    while (HAPI.Update())
    {
        // clear the screen every frame
        memset(screen, 0, width * height * 4);

        if (keyData.scanCode['S'])
        {
            EDistance += 10;
            if (EDistance > Max_Edistance)
                EDistance = Max_Edistance;

        }

        if (keyData.scanCode['W'])
        {
            EDistance -= 10;

            if (EDistance < Min_Edistance)
                EDistance = Min_Edistance;
        }

        // go through each star in array
        for (int i = 0; i < numStars; i++)
        {
            Star& star = stars[i];

            // turn the 3D star location into 2D screen location with equation from hints
            int Sx = ((EDistance * (star.x - cx)) / (star.z + EDistance)) + cx;
            int Sy = ((EDistance * (star.y - cy)) / (star.z + EDistance)) + cy;

            // offset
            int offset = (Sx + Sy * width) * 4;

            // put the star on the screen, and make sure it fits in the screen to prevent crashing
            if (Sx >= 0 && Sx < width && Sy >= 0 && Sy < height) {
                screen[offset] = 255;      // r
                screen[offset + 1] = 255;  // g
                screen[offset + 2] = 255;    // b
                // white seems to look better than yellow here
                // no alpha needed
            }
            else {
                star.x = (rand() % (2 * width)) - width / 2;
                star.y = (rand() % (2 * height)) - height / 2;
                star.z = 500;
                continue;
                // makes it look more smooth than checking if it is less than eye distance as it
                // will check if it is outside the bounds and reset it
            }

            // move the star closer 
            star.z -= 2;

            /*
            if (star.z <= -EDistance) {
                 star.x = (rand() % (2 * width)) - width / 2;   // random x
                 star.y = (rand() % (2 * height)) - height / 2; // random y
                 star.z = 400;  // resets the depth z
             }

            */
        }
    }
};
