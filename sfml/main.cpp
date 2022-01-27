#include <SFML/Graphics.hpp>
#include <math.h>

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
#define SEG_WIDTH 70
const float CH_H = 4;
const float CH_W = 1.3;
float CH_HY = 0.80; //0->flat

sf::Color gray = sf::Color(84, 90, 112);
sf::Color lightgray = sf::Color(183, 187, 201);
sf::Color red = sf::Color(255, 46, 88);

struct Road {
    int height; //number of elements
    struct Segment* segments; //array of segments
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

struct Segment seg(float x, float y, float z, int width, float height, sf::Color colour) {
    struct Segment segment = { x, y, z, width, height, colour};
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
        float s = SEG_WIDTH - 81 / (CH_H*CH_W);
        float w31 = 136 * (s / SEG_WIDTH);
        float h31 = 50 * (s / SEG_WIDTH);
        float w30 = SEG_WIDTH * (s / SEG_WIDTH);
        float h30 = 20 * (s / SEG_WIDTH);
        float y31 = 175 + h30; 
        float h32 = 30 * (s / SEG_WIDTH);
        road->segments[9] = seg((w31 - w30) / -2, y31, 81, 136, 50, lightgray);
        road->segments[10] = seg((w31 - w30) / -2, y31 - h31, 81, 33, 70, lightgray);
        road->segments[11] = seg(w30, y31 - h31, 81, 33, 70, lightgray);
        road->segments[12] = seg(w30, y31 - h31 - h32, 81, 33, 30, red);
        road->segments[13] = seg((w31 - w30) / -2, y31 - h31 - h32, 81, 33, 30, red);
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
        w31 = 136 * (s / SEG_WIDTH);
        h31 = 50 * (s / SEG_WIDTH);
        w30 = SEG_WIDTH * (s / SEG_WIDTH);
        h30 = 20 * (s / SEG_WIDTH);
        y31 = 190 + h30;
        h32 = 30 * (s / SEG_WIDTH);
        road->segments[23] = seg((w31 - w30) / -2, y31, 162, 136, 50, lightgray);
        road->segments[24] = seg((w31 - w30) / -2, y31 - h31, 162, 33, 70, lightgray);
        road->segments[25] = seg(w30, y31 - h31, 162, 33, 70, lightgray);
        road->segments[26] = seg(w30, (y31 - h31) - h32, 162, 33, 30, red);
        road->segments[27] = seg((w31 - w30) / -2, (y31 - h31) - h32, 162, 33, 30, red);
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
        else if (sf::Keyboard::isKeyPressed(sf:: Keyboard::Down))
        {
            road.playery += 250 * dt;
        }
        window.clear(sf::Color(0, 191, 255));
        draw_road(&window, &road); //3rd param = change rate of width
        window.display();
    }
}
