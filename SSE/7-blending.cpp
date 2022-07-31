#include <stdlib.h>
#include <math.h>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <emmintrin.h>
#include <smmintrin.h>

#define WIDTH   1024 
#define HEIGHT  1024 


// for debug defines
#ifdef DEBUG
    #define _print(ss, s, p, typ) printf(#ss" = {" #p ", " #p ", " #p ", " #p"}\n", ((typ*)&s)[3], ((typ*)&s)[2], ((typ*)&s)[1], ((typ*)&s)[0]); 
#else
    #define _print(ss, s, p, t)   ;       
#endif

#define $ ;//printf("[%d] DEBUG\n", __LINE__);

struct pixel_t
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};


inline void copyToImg(pixel_t* dst, pixel_t* src, int x, int y, int sizeX, int sizeY)
{
    for(int i = 0; i < sizeY; i++)
        memcpy(dst+y*WIDTH + x + i * WIDTH, src + sizeX*i, sizeof(pixel_t)*sizeX); 
}

inline pixel_t* loadImage(const char* filename)
{
    sf::Image img;

    img.loadFromFile(filename);

    sf::Vector2u vec = img.getSize();

    int size = vec.x*vec.y;

    printf("vec.x = %d vec.y = %d\n", vec.x, vec.y);

    pixel_t* temp = new pixel_t [size];
    
    $
    memcpy(temp, img.getPixelsPtr(), size*sizeof(pixel_t));
    $

    pixel_t* ret = new pixel_t [WIDTH*HEIGHT];

    copyToImg(ret, temp, (WIDTH - vec.x) / 2.f, (HEIGHT - vec.y) / 2.f, vec.x, vec.y);

    delete[] temp;

    return ret;
}

void setPixel(pixel_t* arr, int x, int y, sf::Color color)
{
    arr[(y*WIDTH + x)].r = color.r;
    arr[(y*WIDTH + x)].g = color.g;
    arr[(y*WIDTH + x)].b = color.b;
    arr[(y*WIDTH + x)].a = 255; 
}

int main()
{
    sf::RenderWindow win (sf::VideoMode(WIDTH, HEIGHT), "Test");


    sf::Texture texture;
    
    texture.create(WIDTH, HEIGHT);

    $
    pixel_t* table = loadImage("./Table.bmp");
    pixel_t* racket = loadImage("./Racket.bmp");
    pixel_t* cat = loadImage("./AskhatCat.bmp");

    pixel_t* front = racket;
    //front = cat;
    pixel_t* back = table;
    pixel_t* pixelData = new pixel_t[WIDTH * HEIGHT];

    texture.update((sf::Uint8*)pixelData);
    $

    sf::Sprite sprite(texture);
    sprite.setPosition(0, 0);

    const unsigned char I = 255;
    const unsigned char Z = 0x80;

    const __m128i _0     = _mm_set_epi32( 0, 0, 0, 0);
    const __m128i _255   = _mm_cvtepu8_epi16(_mm_set_epi8(I,I,I,I, I,I,I,I, I,I,I,I, I,I,I,I));

    sf::Clock clock;
    clock.restart();


    //int x = 0, y = 0;

    //pixel_t* test = new pixel_t[4];
    //test[0] = {255,   0,   0, 255};
    //test[1] = {  0, 255,   0, 128};
    //test[2] = {  0,   0, 255, 255};
    //test[3] = {255, 255, 255, 255};

    //pixel_t* test2 = new pixel_t[4];
    //test2[0] = {128,   0,   0, 255};
    //test2[1] = {  0, 128,   0, 255};
    //test2[2] = {  0,   0, 128, 255};
    //test2[3] = {128, 128, 128, 255};

    //front = test;
    //back = test2;

    int n = 0;
    while(win.isOpen())
    {
        n++;
        if(1)//!(n%10))
        {
            printf("fps: %lf\r",  1.f / clock.getElapsedTime().asSeconds());
            clock.restart();
            fflush(stdout);
            n = 0;
        }

        clock.restart();

        sf::Event e;

        while(win.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                win.close();

            if(e.type == sf::Event::KeyPressed)
                if(e.key.code == sf::Keyboard::Escape)  { win.close(); break; }
        }

        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            for(int y = 0; y < HEIGHT; y++)
                for(int x = 0; x < WIDTH; x+=4)
                {
                    $

                    __m128i fr = _mm_load_si128((__m128i*)(front + y * WIDTH + x));
                    __m128i bk = _mm_load_si128((__m128i*)(back + y * WIDTH + x));
                    
                    //_print(fr, %x, int);
                    //_print(pixel_t* back, *back, %x, unsigned int);
                    //_print(loaded bk, bk, %08x, int);

                    __m128i FR = (__m128i) _mm_movehl_ps((__m128) _0, (__m128)fr);
                    __m128i BK = (__m128i) _mm_movehl_ps((__m128) _0, (__m128)bk);

                    //_print(created BK, BK, %08x, int);

                    fr = _mm_cvtepu8_epi16(fr);
                    FR = _mm_cvtepu8_epi16(FR);

                    //_print((after _mm_cvtepu8_epi16(fr)) fr, fr, %08x, int);


                    bk = _mm_cvtepu8_epi16(bk);
                    BK = _mm_cvtepu8_epi16(BK);

                    //_print((after _mm_cvtepu8_epi16(bk)) bk, bk, %08x, int);

                    static const __m128i move_mask = _mm_set_epi8(  Z, 0xE, Z, 0xE, Z, 0xE, Z, 0xE,
                                                                    Z, 0x6, Z, 0x6, Z, 0x6, Z, 0x6);

                    __m128i a = _mm_shuffle_epi8(fr, move_mask); 
                    
                    //_print((after _mm_shuffle_epi8(fr, move_mask)) a, a, %08x, int);

                    __m128i A = _mm_shuffle_epi8(FR, move_mask); 

                    //_print((after _mm_shuffle_epi8(FR, move_mask)) A, A, %08x, int);

                    fr = _mm_mullo_epi16(fr, a);
                    FR = _mm_mullo_epi16(FR, A);

                    //_print((after _mm_mullo_epi16(fr, a)) fr, fr, %08x, int);


                    bk = _mm_mullo_epi16(bk, _mm_sub_epi16(_255, a));
                    BK = _mm_mullo_epi16(BK, _mm_sub_epi16(_255, A));

                    //_print(fr, fr, %08x, int);
                    //_print(bk, bk, %08x, int);

                    __m128i sum = _mm_add_epi16(fr, bk);
                    __m128i SUM = _mm_add_epi16(FR, BK);

                    //_print(sum, sum, %08x, int);

                    static const __m128i move_sum = _mm_set_epi8(     Z,   Z,   Z,   Z,   Z,   Z,   Z,  Z, 
                                                                    0xF, 0xD, 0xB, 0x9, 0x7, 0x5, 0x3, 0x1);
                    sum = _mm_shuffle_epi8(sum, move_sum);
                    SUM = _mm_shuffle_epi8(SUM, move_sum);

                    __m128i color = (__m128i) _mm_movelh_ps((__m128)sum, (__m128)SUM);

                    //_print(color, color, %08x, int);

                    $
                    _mm_store_si128((__m128i*)(pixelData + y * WIDTH + x), color);
                    $
                }            
        }
        else
        {
            for(int y = 0; y < HEIGHT; y++)
                for(int x = 0; x < WIDTH; x++)
                {
                    pixel_t* fr = front + y * WIDTH + x;
                    pixel_t* bk = back  + y * WIDTH + x;

                    setPixel(pixelData, x, y, 
                    {
                        (unsigned char)((fr->r * fr->a + bk->r * (255 - fr->a))>>8),
                        (unsigned char)((fr->g * fr->a + bk->g * (255 - fr->a))>>8),
                        (unsigned char)((fr->b * fr->a + bk->b * (255 - fr->a))>>8),
                    });
                }
        }

        texture.update((sf::Uint8*)pixelData);
        win.clear();
        win.draw(sprite);
        win.display();
    }

    delete[] racket;
    delete[] cat;
    delete[] table;
    delete[] pixelData;

    return 0;
}
