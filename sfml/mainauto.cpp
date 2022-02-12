#include <SFML/Graphics.hpp>
#include <math.h>

#define SCREEN_HEIGHT 224
#define SCREEN_WIDTH 396
#define GET_POS(x) ((abs(x)+x)/2)
#define SEG_WIDTH 70
#define CH_Z 9
const float CH_H = 4;
const float CH_W = 1.65;
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


void gen_road(unsigned char* grid, int size, struct Road *road) {
    int v = 0;
    for (int i = 0;i < size;i++) {
        uint8_t cell = grid[i];
        unsigned char low = cell & 0x0F;
        unsigned char high = cell >> 4;
        if (high == 0) {
            road->segments[v] = seg(0, 190 - low * 5, i * CH_Z, SEG_WIDTH, 20, gray);
        }
        else if (high == 1) {
            road->segments[v++] = seg(0, 190 - low * 5, i * CH_Z, SEG_WIDTH, 20, gray);
            float z = i*CH_Z+CH_Z;
            float s = SEG_WIDTH - z / (CH_H * CH_W);
            float w31 = 120 * (s / SEG_WIDTH);
            float w32 = (120 - SEG_WIDTH) / 2;
            float h31 = 70 * (s / SEG_WIDTH);
            float w30 = SEG_WIDTH * (s / SEG_WIDTH);
            float h30 = 20 * (s / SEG_WIDTH);
            float y31 = road->segments[v-1].y + h30;
            float h32 = w32 * (s / SEG_WIDTH);
            printf("%f", (w31 - w30) / -2);
            road->segments[v++] = seg((w31 - w30) / -2, y31, z, 120, 50, lightgray);
            road->segments[v++] = seg((w31 - w30) / -2, y31 - h31, z, w32, 70, lightgray);
            road->segments[v++] = seg( w30, y31 - h31, z, w32, 70, lightgray);
            road->segments[v++] = seg(w30, y31 - h31 - h32, z, w32, w32, red);
            road->segments[v] = seg((w31 - w30) / -2, y31 - h31 - h32, z, w32, w32, red);
        }
        v++;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "KraD");
    struct Road road;
    road.height = 64;
    road.playerx = 0;
    road.playery = 0;
    road.width = 100;
    road.starty = 160;
    road.startx = 40;
    road.segment_height = 30;
    road.segments = (struct Segment*)malloc(road.height * sizeof(struct Segment));

    unsigned char grid[] = {0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x14,0x04,0x04,0x03,0x02, 0x01,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x14,0x04,0x04,0x03,0x02, 0x01,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00 };
    gen_road(grid, sizeof(grid), &road);

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
            for (int i = 0;i < road.height;i++) {
                road.segments[i].x -= 300 * dt;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            for (int i = 0;i < road.height;i++) {
                road.segments[i].x += 300 * dt;
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
