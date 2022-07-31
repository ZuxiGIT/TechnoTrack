#include <stdlib.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>


void setPixel(sf::Uint8* arr, int x, int y, sf::Color color)
{
    arr[(y*800 + x) * 4 + 0] = color.r;
    arr[(y*800 + x) * 4 + 1] = color.g;
    arr[(y*800 + x) * 4 + 2] = color.b;
    arr[(y*800 + x) * 4 + 3] = 255;
}

int main()
{
    sf::RenderWindow win (sf::VideoMode(800, 600), "Test");

    sf::Uint8* pixelData = new sf::Uint8 [800*600*4];

    sf::Texture texture;
    
    texture.create(800, 600);
    texture.update(pixelData);

    sf::Sprite sprite(texture);
    sprite.setPosition(0, 0);

    const float ROI_X = -1.325f;
    const float ROI_Y = 0.f;

    const int nMax = 256;
    const float r2Max = 100.f;
    const float dx = 1 / 800.f;
    const float dy = 1 / 600.f;

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
                if(e.key.code == sf::Keyboard::A)     scale += dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if(e.key.code == sf::Keyboard::Z)     scale -= dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
            }
        }

        for (int iy = 0; iy < 600; iy++) 
        {

            float x0 = ( (          - 400.f) * dx + ROI_X + xC ) * scale,
                  y0 = ( ((float)iy - 300.f) * dy + ROI_Y + yC ) * scale;

            for (int ix = 0; ix < 800; ix+=4, x0 += dx*4)
            {

                float X0[4] = { x0, x0+dx, x0+2*dx, x0+3*dx };
                float Y0[4] = { y0, y0,    y0,      y0 };

                int N[4] = {0, 0, 0, 0};

                for(int i = 0; i < 4; i ++)
                {
                    float X = X0[i];
                    float Y = Y0[i];

                    for (; N[i] < nMax; N[i]++)
                        {
                        float x2 = X*X,
                              y2 = Y*Y,
                              xy = X*Y;

                        float r2 = x2 + y2;
                           
                        if (r2 >= r2Max) break;
                            
                        X = x2 - y2 + x0,
                        Y = xy + xy + y0;
                        }
                }
                
                
                for(int i = 0; i < 4; i ++)
                {
                    float I = sqrtf (sqrtf ((float)N[i] / (float)nMax)) * 255.f;

                    unsigned char c = (unsigned char) I;
                    sf::Color color = (N[i] < nMax)? sf::Color{ (unsigned char) (255-c), (unsigned char) (c%2 * 64), c } : sf::Color {};
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
