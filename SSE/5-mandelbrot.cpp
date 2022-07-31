#include <stdlib.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <emmintrin.h>

#define WIDTH   900
#define HEIGHT  900
#define _print(s, p, typ) ;// printf(#s" = {" #p ", " #p ", " #p ", " #p"}\n", ((typ*)&s)[0], ((typ*)&s)[1], ((typ*)&s)[2], ((typ*)&s)[3]); 

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
    const __m128 r2Max = _mm_set_ps1(100.f);
    const __m128 _0123 = _mm_set_ps(3.f, 2.f, 1.f, 0.f);

    const float dx = 1 / ((float) WIDTH);
    const float dy = 1 / ((float) HEIGHT);

    float scale = 1.f;
    float xC = 0.f;
    float yC = 0.f;

    sf::Clock clock;
    clock.restart();

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

                __m128 X0 = _mm_add_ps(_mm_set_ps1(x0), _mm_mul_ps(_0123, _mm_set_ps1(dx)));
                __m128 Y0 = _mm_set_ps1(y0);

                __m128 X = X0, Y = Y0;

                __m128i N = _mm_setzero_si128(); 

                for (int n = 0; n < nMax; n++)
                {
                    __m128 x2 = _mm_mul_ps(X, X); 
                    __m128 y2 = _mm_mul_ps(Y, Y); 
                    __m128 xy = _mm_mul_ps(X, Y);

                    __m128 r2 = _mm_add_ps(x2, y2); 
                       
                    _print(r2, %f, float);
                    _print(r2Max, %f, float);

                    __m128 cmp = _mm_cmple_ps(r2, r2Max);

                    _print(cmp, %f, float);

                    int mask = _mm_movemask_ps(cmp);

                    if(!mask) break;

                    //printf("N = {%d, %d, %d, %d}\n", ((int*)&N)[0], ((int*)&N)[1], ((int*)&N)[2], ((int*)&N)[3]);

                    N = _mm_sub_epi32(N, _mm_castps_si128(cmp));

                    __m128 c = cmp;

                    _print(c, %f, float);

                    //__m128i d = _mm_cvtps_epi32(cmp);

                    //printf("N = {%d, %d, %d, %d}\n", ((int*)&N)[0], ((int*)&N)[1], ((int*)&N)[2], ((int*)&N)[3]);

                    X = _mm_add_ps(X0, _mm_sub_ps(x2, y2));
                    Y = _mm_add_ps(Y0, _mm_add_ps(xy, xy));
                }
                
                __m128 _255 = _mm_set_ps1(255.f);
                __m128 _nMax = _mm_set_ps1(nMax);

                __m128 I = _mm_mul_ps(_255, _mm_sqrt_ps(_mm_sqrt_ps(_mm_div_ps(_mm_cvtepi32_ps(N), _nMax))));

                for(int i = 0; i < 4; i ++)
                {
                    int *pn = (int*)   &N;
                    float *pI = (float*) &I;

                    unsigned char c = (unsigned char) pI[i];
                    sf::Color color = (pn[i] < nMax)? sf::Color{ c, (unsigned char) (c%2 * 128), (unsigned char) (255-c)} : sf::Color {};
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
