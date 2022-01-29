#include <SFML/Graphics.hpp>
#include <math.h>

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
#define SEG_WIDTH 70
#define CH_Z 9
const float CH_H = 4;
const float CH_W = 1.3;
float CH_HY = 0.80; //0->flat

sf::Color gray = sf::Color(131, 118, 156);
sf::Color lightgray = sf::Color(194, 195, 199);
sf::Color red = sf::Color(255, 0, 77);
sf::Color blue = sf::Color(41, 173, 255);

struct Road {
    int height; //number of elements
    struct Segment* segments; //array of segments
    int* grid;
    float playerx;
    float playery;
    int width;
    int segment_height;
    int starty;
    int startx;
};

struct Segment {
    float x;
    float y;
    float z;
    int width;
    int height;
    sf::Color colour;
};

struct Segment seg(float x, float y, float z, int width, int height, sf::Color colour) {
    struct Segment segment = { x, y, z, width, height, colour };
    return segment;
}

void gen_road(struct Road* road) {

    road->segments[0] = seg(0, 190, 0, SEG_WIDTH, 20, gray);
    road->segments[1] = seg(0, 190, 9, SEG_WIDTH, 20, gray);
    road->segments[2] = seg(0, 190, 18, SEG_WIDTH, 20, gray);
    road->segments[3] = seg(0, 190, 27, SEG_WIDTH, 20, gray);
    road->segments[4] = seg(0, 190, 36, SEG_WIDTH, 20, gray);
    road->segments[5] = seg(0, 190, 45, SEG_WIDTH, 20, gray);
    road->segments[6] = seg(0, 185, 54, SEG_WIDTH, 20, gray);
    road->segments[7] = seg(0, 180, 63, SEG_WIDTH, 20, gray);
    road->segments[8] = seg(0, 178, 72, SEG_WIDTH, 20, gray);
    /*pillar*/
        float s = SEG_WIDTH - 81 / (CH_H * CH_W);
        float w31 = 120 * (s / SEG_WIDTH);
        float w32 = (120 - SEG_WIDTH) / 2;
        float h31 = 50 * (s / SEG_WIDTH);
        float w30 = SEG_WIDTH * (s / SEG_WIDTH);
        float h30 = 20 * (s / SEG_WIDTH);
        float y31 = 175 + h30;
        float h32 = w32 * (s / SEG_WIDTH);
        road->segments[9] = seg((w31 - w30) / -2, y31, 81, 120, 50, lightgray);
        road->segments[10] = seg((w31 - w30) / -2, y31 - h31, 81, w32, 70, lightgray);
        road->segments[11] = seg(w30, y31 - h31, 81, w32, 70, lightgray);
        road->segments[12] = seg(w30, y31 - h31 - h32, 81, w32, w32, red);
        road->segments[13] = seg((w31 - w30) / -2, y31 - h31 - h32, 81, w32, w32, red);
    /*pillar*/
    road->segments[14] = seg(0, 175, 81, SEG_WIDTH, 20, gray);
    road->segments[15] = seg(0, 175, 90, SEG_WIDTH, 20, gray);
    road->segments[16] = seg(0, 180, 99, SEG_WIDTH, 20, gray);
    road->segments[17] = seg(0, 190, 108, SEG_WIDTH, 20, gray);
    road->segments[18] = seg(0, 190, 117, SEG_WIDTH, 20, gray);
    road->segments[19] = seg(0, 190, 126, SEG_WIDTH, 20, gray);
    road->segments[20] = seg(0, 190, 135, SEG_WIDTH, 20, gray);
    road->segments[21] = seg(0, 190, 144, SEG_WIDTH, 20, gray);
    road->segments[22] = seg(0, 190, 153, SEG_WIDTH, 20, gray);
    /*pillar*/
        s = SEG_WIDTH - 162 / (CH_H * CH_W);
        w31 = 120 * (s / SEG_WIDTH);
        w32 = (120 - SEG_WIDTH) / 2;
        h31 = 50 * (s / SEG_WIDTH);
        w30 = SEG_WIDTH * (s / SEG_WIDTH);
        h30 = 20 * (s / SEG_WIDTH);
        y31 = 190 + h30;
        h32 = w32 * (s / SEG_WIDTH);
        road->segments[23] = seg((w31 - w30) / -2, y31, 162, 120, 50, lightgray);
        road->segments[24] = seg((w31 - w30) / -2, y31 - h31, 162, w32, 70, lightgray);
        road->segments[25] = seg(w30, y31 - h31, 162, w32, 70, lightgray);
        road->segments[26] = seg(w30, (y31 - h31) - h32, 162, w32, w32, red);
        road->segments[27] = seg((w31 - w30) / -2, (y31 - h31) - h32, 162, w32, w32, red);
    /*pillar*/
    road->segments[28] = seg(0, 190, 162, SEG_WIDTH, 20, gray);
    road->segments[29] = seg(0, 190, 171, SEG_WIDTH, 20, gray);
    road->segments[30] = seg(0, 190, 180, SEG_WIDTH, 20, gray);
    road->segments[31] = seg(0, 190, 189, SEG_WIDTH, 20, gray);

    for (int i = 0;i < road->height;i++) {
        road->segments[i].x -= road->startx;
    }
}



void draw_road(sf::RenderWindow* window, struct Road* road, float zm = 2) {
    float s;
    for (int v = road->height - 1;v >= 0;v--) { //draws each rectangle from furthest to closest
        s = SEG_WIDTH - road->segments[v].z / (CH_H * CH_W);
        float yoffset = road->segments[v].y - road->segments[v].z * CH_HY + road->playery;
        float heightoffset = road->segments[v].height * (s / SEG_WIDTH);
        float widthoffset = road->segments[v].width * (s / SEG_WIDTH);
        float xoffset = ((road->segments[v].x + SCREEN_WIDTH / 2) - s / 2) - road->playerx * (s - road->segments[v].z / 10);

        if (yoffset > SCREEN_HEIGHT || yoffset + heightoffset < 0 || xoffset + widthoffset < 0 || xoffset - widthoffset > SCREEN_WIDTH)
            continue;

        sf::RectangleShape rs(sf::Vector2f(widthoffset, heightoffset)); //makes a rectangle of width s and height relative to z position
        rs.setFillColor(road->segments[v].colour);
        rs.setPosition(xoffset, yoffset);
        window->draw(rs); //displays the rectangle
        if (road->segments[v].colour == gray) {
            sf::RectangleShape marking(sf::Vector2f(3, 3));
            marking.setFillColor(sf::Color::White);
            marking.setPosition(xoffset + s / 2, yoffset + 2);
            window->draw(marking);
        }
    }
}


void grid_to_segments(unsigned char* grid, int size, struct Road road) {
    int v = 0;
    for (int i = 0;i < size;i++) {
        uint8_t cell = grid[i];
        unsigned char low = cell & 0x0F;
        unsigned char high = cell >> 4;
        if (high == 0) {
            printf("[%d] : x(%d) y(%d) z(%d) width(%d) height(%d) colour('gray')\n", v, 0, 190 - low * 5, i * CH_Z, SEG_WIDTH, 20);
        }
        else if (high == 1) {
            printf("[%d] : x(%d) y(%d) z(%d) width(%d) height(%d) colour('gray')\n", v++, 0, 190 - low * 5, i * CH_Z, SEG_WIDTH, 20);
            float z = i*CH_Z+CH_Z;
            printf("%f\n", z);
            float s = SEG_WIDTH - z / (CH_H * CH_W);
            int w31 = 120 * (s / SEG_WIDTH);
            int w32 = (120 - SEG_WIDTH) / 2;
            int h31 = 50 * (s / SEG_WIDTH);
            int w30 = SEG_WIDTH * (s / SEG_WIDTH);
            int h30 = 20 * (s / SEG_WIDTH);
            float y31 = road.segments[v-1].y + h30;
            int h32 = w32 * (s / SEG_WIDTH);
            printf("[%d] : x(%f) y(%f) z(%f) width(%d) height(%d) colour('lightgray')\n", v++, (w31 - w30) / -2, y31, z, 120, 50);
            printf("[%d] : x(%f) y(%f) z(%f) width(%d) height(%d) colour('lightgray')\n", v++, w30, y31 - h31, z, w32, 70);
            printf("[%d] : x(%f) y(%f) z(%f) width(%d) height(%d) colour('lightgray')\n", v++, w30, y31 - h31, z, w32, 70);
            printf("[%d] : x(%f) y(%f) z(%f) width(%d) height(%d) colour('red')\n", v++, w30, y31 - h31 - h31, z, w32, w32);
            printf("[%d] : x(%f) y(%f) z(%f) width(%d) height(%d) colour('red')\n", v, (w31 - w30) / -2, y31 - h31 - h31, z, w32, w32);
        }
        v++;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "KraD");
    struct Road road;
    road.height = 32;
    road.playerx = 0;
    road.playery = 0;
    road.width = 100;
    road.starty = 160;
    road.startx = 40;
    road.segment_height = 30;
    road.segments = (struct Segment*)malloc(road.height * sizeof(struct Segment));
    gen_road(&road); //2nd param = change rate of height

    unsigned char grid[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x01,
        0x02,
        0x03,
        0x14,
        0x04,
        0x03,
        0x02,
        0x01,
        0x00,
        0x00,
        0x00,
        0x00,
        0x10,
        0x00,
        0x00,
        0x00,
        0x00
    };
    grid_to_segments(grid, sizeof(grid), road);

    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            road.playerx -= 4 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            road.playerx += 4 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            float mov = 300 * dt;
            for (int i = 0;i < road.height;i++) {
                road.segments[i].x -= mov;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            float mov = 300 * dt;
            for (int i = 0;i < road.height;i++) {
                road.segments[i].x += mov;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            road.playery -= 250 * dt;
            CH_HY -= 5 / 4 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            road.playery += 250 * dt;
            CH_HY += 5 / 4 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            road.playery -= 250 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            road.playery += 250 * dt;
        }
        window.clear(blue);
        draw_road(&window, &road); //3rd param = change rate of width
        window.display();
    }
}
