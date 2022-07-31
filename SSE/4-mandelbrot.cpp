#include <stdlib.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>


#define WIDTH   900
#define HEIGHT  900

inline void mm_set_ps(float mm[4], float val0, float val1, float val2, float val3)
{
    mm[0] = val0; mm[1] = val1; mm[2] = val2; mm[3] = val3;
}

inline void mm_set_ps1      (float mm[4], float val)                                { for(int i = 0; i < 4; i++) mm[i] = val;                       }
inline void mm_move_ps      (float mm[4], const float mm2[4])                       { for(int i = 0; i < 4; i++) mm[i] = mm2[i];                    }

inline void mm_add_ps       (float mm[4], const float mm1[4], const float mm2[4])   { for(int i = 0; i < 4; i++) mm[i] = mm1[i] + mm2[i];           }
inline void mm_sub_ps       (float mm[4], const float mm1[4], const float mm2[4])   { for(int i = 0; i < 4; i++) mm[i] = mm1[i] - mm2[i];           }
inline void mm_mul_ps       (float mm[4], const float mm1[4], const float mm2[4])   { for(int i = 0; i < 4; i++) mm[i] = mm1[i] * mm2[i];           }

inline void mm_add_epi32    (int mm[4], const int   mm1[4], const float mm2[4])     { for(int i = 0; i < 4; i++) mm[i] = mm1[i] + mm2[i];           }
inline void mm_cmple_ps     (float cmp[4], const float mm1[4], const float mm2[4])  { for(int i = 0; i < 4; i++) cmp[i] = mm1[i] <= mm2[i] ? 1 : 0; }

inline int mm_ss_ps(const float cmp[4])
{
    int mask = 0;
    for (int i = 0; i < 4; i++) mask |= (!!cmp[i] << i);
    return mask;
}


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
    float r2Max[4] = {}; mm_set_ps1(r2Max, 100.f);
    float _0123[4] = {}; mm_set_ps(_0123, 0.f, 1.f, 2.f, 3.f);

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

                float DX[4] = {}; mm_set_ps1(DX, dx); mm_mul_ps(DX, DX, _0123);
                float X0[4] = {}; mm_set_ps1(X0, x0); mm_add_ps(X0, X0, DX);
                float Y0[4] = {}; mm_set_ps1(Y0, y0);

                float X[4] = {}; mm_move_ps(X, X0); 
                float Y[4] = {}; mm_move_ps(Y, Y0); 

                int N[4] = {0, 0, 0, 0};

                for (int n = 0; n < nMax; n++)
                {
                    float x2[4] = {}; mm_mul_ps(x2, X, X); 
                    float y2[4] = {}; mm_mul_ps(y2, Y, Y); 
                    float xy[4] = {}; mm_mul_ps(xy, X, Y);

                    float r2[4] = {}; mm_add_ps(r2, x2, y2); 
                       
                    float cmp[4] = {}; mm_cmple_ps(cmp, r2, r2Max);

                    if(!mm_ss_ps(cmp)) break;

                    mm_add_epi32(N, N, cmp);

                    mm_sub_ps(X, x2, y2); mm_add_ps(X, X, X0);
                    mm_add_ps(Y, xy, xy); mm_add_ps(Y, Y, Y0);
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
