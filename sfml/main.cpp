#include <SFML/Graphics.hpp>
#include <math.h>

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
#define SEG_WIDTH 90
const float CH_H = 3;
const float CH_W = 1.5;
float CH_HY = 0; //0->flat

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
};

struct Segment {
    float x;
    float y;
    float z;
    int width;
    int height;
    sf::Color colour;
};

struct Segment seg(float x,float y,float z,int width,float height,sf::Color colour) {
    struct Segment segment = { x, y, z, width, height, colour };
    return segment;
}

void gen_road(struct Road* road, float zm = 1) {
    int v = road->height;
    /*for (int i = 0;i < road->height;i++) {
        road->segments[i].width = road->width;
        road->segments[i].height = road->height;
        road->segments[i].z = i * zm;
        road->segments[i].y = road->starty + zm;
        road->segments[i].x = 0;
        road->segments[i].colour = gray;
        v--;
    }*/


    road->segments[0] = seg(0, 190, 0, 90, 20, gray);
    road->segments[1] = seg(0, 190, 5, 90, 20, gray);
    road->segments[2] = seg(0, 190, 10, 90, 20, gray);
    road->segments[3] = seg(0, 190, 15, 90, 20, gray);
    road->segments[4] = seg(0, 190, 20, 90, 20, gray);
    road->segments[5] = seg(0, 190, 25, 90, 20, gray);
    road->segments[6] = seg(0, 190, 30, 90, 20, gray);
    road->segments[7] = seg(0, 190, 35, 90, 20, gray);
    road->segments[8] = seg(0, 190, 40, 90, 20, gray);
    road->segments[9] = seg(0, 190, 45, 90, 20, gray);
    road->segments[10] = seg(0, 190, 50, 90, 20, gray);
    road->segments[11] = seg(0, 190, 55, 90, 20, gray);
    road->segments[12] = seg(0, 185, 60, 90, 20, gray);
    road->segments[13] = seg(0, 182, 65, 90, 20, gray);
    road->segments[14] = seg(0, 180, 70, 90, 20, gray);
    road->segments[15] = seg(0, 178, 75, 90, 20, gray);
    /*pillar*/
        float s = SEG_WIDTH - 80 / CH_H;
        float w31 = 155 * (s / SEG_WIDTH);
        float h31 = 70 * (s / SEG_WIDTH);
        float w30 = 90 * (s / SEG_WIDTH);
        float h30 = 20 * (s / SEG_WIDTH);
        float y31 = 178+h30; //yes i know simple arithmetics
        float h32 = 30 * (s / SEG_WIDTH);
        /*pillar*/
            road->segments[16] = seg((w31 - w30) / -2, y31, 80, 155, 70, lightgray);
            road->segments[17] = seg((w31 - w30) / -2, y31 - h31, 80, 33, 70, lightgray);
            road->segments[18] = seg(w30, y31-h31, 80, 33, 70, lightgray);
            road->segments[19] = seg(w30, y31-h31-h32, 80, 33, 30, red);
            road->segments[20] = seg((w31 - w30) / -2, y31-h31-h32, 80, 33, 30, red);
        /*pillar*/
    /*pillar*/
    road->segments[21] = seg(0, 175, 80, 90, 20, gray);
    road->segments[22] = seg(0, 175, 80, 90, 20, gray);
    road->segments[23] = seg(0, 175, 85, 90, 20, gray);
    road->segments[24] = seg(0, 175, 90, 90, 20, gray);
    road->segments[25] = seg(0, 178, 95, 90, 20, gray);
    road->segments[26] = seg(0, 180, 100, 90, 20, gray);
    road->segments[27] = seg(0, 182, 105, 90, 20, gray);
    road->segments[28] = seg(0, 190, 110, 90, 20, gray);
    road->segments[29] = seg(0, 190, 115, 90, 20, gray);
    road->segments[30] = seg(0, 190, 120, 90, 20, gray);
    /*pillar*/
        s = SEG_WIDTH - 120 / CH_H;
        w31 = 155 * (s / SEG_WIDTH);
        h31 = 70 * (s / SEG_WIDTH);
        w30 = 90 * (s / SEG_WIDTH);
        h30 = 20 * (s / SEG_WIDTH);
        y31 = 190 + h30; //yes i know simple arithmetics
        h32 = 30 * (s / SEG_WIDTH);
        road->segments[31] = seg((w31-w30) / -2, y31, 120, 155, 70, lightgray);
        road->segments[32] = seg((w31 - w30) / -2, y31 - h31, 120, 33, 70, lightgray);
        road->segments[33] = seg(w30, y31 - h31, 120, 33, 70, lightgray);
        road->segments[34] = seg(w30, (y31 - h31) - h32, 120, 33, 30, red);
        road->segments[35] = seg((w31 - w30) / -2, (y31 - h31) - h32, 120, 33, 30, red);
    /*pillar*/
    road->segments[36] = seg(0, 190, 125, 90, 20, gray);
    road->segments[37] = seg(0, 190, 130, 90, 20, gray);
    road->segments[38] = seg(0, 190, 135, 90, 20, gray);
    road->segments[39] = seg(0, 190, 140, 90, 20, gray);
    road->segments[40] = seg(0, 190, 145, 90, 20, gray);
 }



void draw_road(sf::RenderWindow* window, struct Road* road, float zm = 2) {
    int i = 0;
    float s, sy;
    for (int v = road->height - 1;v >= 0;v--) { //draws each rectangle from furthest to closest
        s = SEG_WIDTH - road->segments[v].z / CH_H;
        float widthoffset = road->segments[v].width * (s / SEG_WIDTH);
        float heightoffset = road->segments[v].height * (s / SEG_WIDTH);
        float xoffset = ((road->segments[v].x + SCREEN_WIDTH / 2) - s / 2) - road->playerx * s;
        float yoffset = road->segments[v].y - road->segments[v].z * CH_HY + road->playery;
        sf::RectangleShape rs(sf::Vector2f(widthoffset, heightoffset)); //makes a rectangle of width s and height relative to z position
        rs.setFillColor(road->segments[v].colour);
        //float xoffset = ((road->segments[v].x + SCREEN_WIDTH/2) - s) - road->playerx * s; 
        rs.setPosition(xoffset, yoffset);
        window->draw(rs); //displays the rectangle
        if (road->segments[v].colour == gray) {
            sf::RectangleShape marking(sf::Vector2f(4, 4));
            marking.setFillColor(sf::Color::White);
            marking.setPosition(xoffset + s / 2, yoffset + 2);
            window->draw(marking);
        }
        i++;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "KraD");
    struct Road road;
    road.playerx = 0;
    road.playery = 0;
    road.width = 100;
    road.starty = 160;
    road.segment_height = 30;
    road.height = 41;
    road.segments = (struct Segment*)malloc(road.height * sizeof(struct Segment));
    gen_road(&road, CH_H); //2nd param = change rate of height
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
            road.playerx -= 5 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            road.playerx += 5 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            float mov = 400 * dt;
            for (int i = 0;i < road.height;i++) {
                road.segments[i].x -= mov;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            float mov = 400 * dt;
            for (int i = 0;i < road.height;i++) {
                road.segments[i].x += mov;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            road.playery -= 100 * dt; //300
            CH_HY -= 5/3 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            road.playery += 100 * dt;
            CH_HY += 5/3 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            road.playery -= 300 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            road.playery += 300 * dt;
        }
        window.clear(sf::Color(0, 191, 255));
        draw_road(&window, &road, CH_W); //3rd param = change rate of width
        window.display();
    }
}
