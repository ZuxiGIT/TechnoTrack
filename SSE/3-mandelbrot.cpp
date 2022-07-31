#include <stdlib.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>


#define WIDTH   900
#define HEIGHT  900

void setPixel(sf::Uint8* arr, int x, int y, sf::Color color)
{
    arr[(y*WIDTH + x) * 4 + 0] = color.r;
    arr[(y*WIDTH + x) * 4 + 1] = color.g;
    arr[(y*WIDTH + x) * 4 + 2] = color.b;
    arr[(y*WIDTH + x) * 4 + 3] = 255;
}

int main()
{
    sf::RenderWindow win (sf::VideoMode(WIDTH, HEIGHT), "Test");

    sf::Uint8* pixelData = new sf::Uint8 [WIDTH*HEIGHT*4];

    sf::Texture texture;
    
    texture.create(WIDTH, HEIGHT);
    texture.update(pixelData);

    sf::Sprite sprite(texture);
    sprite.setPosition(0, 0);

    const float ROI_X = -1.325f;
    const float ROI_Y = 0.f;

    const int nMax = 256;
    const float r2Max = 100.f;

    const float dx = 1 / ((float) WIDTH);
    const float dy = 1 / ((float) HEIGHT);

    float scale = 1.f;
    float xC = 0.f;
    float yC = 0.f;

    sf::Clock clock;

    while(win.isOpen())
    {
        printf("fps: %lf\r", 1000.f / clock.getElapsedTime().asMilliseconds());
        fflush(stdout);

        clock.restart();

        sf::Event e;

        while(win.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                win.close();

            if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::Escape)  { win.close(); break; }
                if(e.key.code == sf::Keyboard::Right) xC    += dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if(e.key.code == sf::Keyboard::Left)  xC    -= dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if(e.key.code == sf::Keyboard::Down)  yC    += dy * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if(e.key.code == sf::Keyboard::Up)    yC    -= dy * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if(e.key.code == sf::Keyboard::A)     scale += dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 4.f : 2.f);
                if(e.key.code == sf::Keyboard::Z)     scale -= dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 4.f : 2.f);
            }
        }

        for (int iy = 0; iy < HEIGHT; iy++) 
        {

            float y0 = ( ((float)iy - (float)HEIGHT / 2) * dy + ROI_Y + yC ) * scale;

            for (int ix = 0; ix < WIDTH; ix+=4)
            {

                float x0 = ( ((float)ix - (float)WIDTH  / 2) * dx + ROI_X + xC ) * scale;

                float X0[4] = { x0, x0+dx, x0+2*dx, x0+3*dx }; 
                float Y0[4] = { y0,    y0,      y0,      y0 }; 

                float X[4] = {}; for(int i = 0; i < 4; i ++) X[i] = X0[i]; 
                float Y[4] = {}; for(int i = 0; i < 4; i ++) Y[i] = Y0[i];

                int N[4] = {0, 0, 0, 0};

                for (int n = 0; n < nMax; n++)
                {
                    float x2[4] = {}; for(int i = 0; i < 4; i ++) x2[i] = X[i] * X[i];
                    float y2[4] = {}; for(int i = 0; i < 4; i ++) y2[i] = Y[i] * Y[i];
                    float xy[4] = {}; for(int i = 0; i < 4; i ++) xy[i] = X[i] * Y[i];

                    float r2[4] = {}; for(int i = 0; i < 4; i ++) r2[i] = x2[i] + y2[i];
                       
                    int cmp[4] = {};
                    for(int i = 0; i < 4; i ++) if (r2[i] <= r2Max) cmp[i]=1;

                    int mask = 0;
                    for(int i = 0; i < 4; i ++) mask |= (cmp[i] << i);
                    if(!mask) break;

                    for(int i = 0; i < 4; i ++) N[i] = N[i] + cmp[i];

                    for(int i = 0; i < 4; i ++) X[i] = x2[i] - y2[i] + X0[i];
                    for(int i = 0; i < 4; i ++) Y[i] = xy[i] + xy[i] + Y0[i];
                }
                
                for(int i = 0; i < 4; i ++)
                {
                    float I = sqrtf (sqrtf ((float)N[i] / (float)nMax)) * 255.f;

                    unsigned char c = (unsigned char) I;
                    sf::Color color = (N[i] < nMax)? sf::Color{ c, (unsigned char) (c%2 * 128), (unsigned char) (255-c)} : sf::Color {};
                    setPixel(pixelData, ix+i, iy, color);
                }
            }
        }

        texture.update(pixelData);

        win.clear();
        win.draw(sprite);
        win.display();
    }

    delete pixelData;

    return 0;
}
