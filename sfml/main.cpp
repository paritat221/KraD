#include <SFML/Graphics.hpp>
#include <math.h>

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
const float CH_H = 8;
const float CH_W = 1.5;

sf::Color gray = sf::Color(100, 100, 100);

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


    road->segments[0] = seg(0, 190, 0, 90, 25, gray);
    road->segments[1] = seg(0, 190, 5, 90, 25, gray);
    road->segments[2] = seg(0, 190, 10, 90, 25, gray);
    road->segments[3] = seg(0, 190, 15, 90, 25, gray);
    road->segments[4] = seg(0, 190, 20, 90, 25, gray);
    road->segments[5] = seg(0, 190, 25, 90, 25, gray);
    road->segments[6] = seg(0, 190, 30, 90, 25, gray);
    road->segments[7] = seg(0, 190, 35, 90, 25, gray);
    road->segments[8] = seg(0, 190, 40, 90, 25, gray);
    road->segments[9] = seg(0, 190, 45, 90, 25, gray);
    road->segments[10] = seg(0, 190, 50, 90, 25, gray);
    road->segments[11] = seg(0, 190, 55, 90, 25, gray);
    road->segments[12] = seg(0, 190, 60, 90, 25, gray);
    road->segments[13] = seg(0, 190, 65, 90, 25, gray);
    road->segments[14] = seg(0, 190, 70, 90, 25, gray);
    road->segments[15] = seg(0, 190, 75, 90, 25, gray);
    road->segments[16] = seg(0, 190, 80, 90, 25, gray);
    road->segments[17] = seg(0, 190, 85, 90, 25, gray);
    road->segments[18] = seg(0, 190, 90, 90, 25, gray);
    road->segments[19] = seg(0, 190, 95, 90, 25, gray);
    road->segments[20] = seg(0, 190, 100, 90, 25, gray);
    road->segments[21] = seg(0, 190, 105, 90, 25, gray);
    road->segments[22] = seg(0, 190, 110, 90, 25, gray);
    road->segments[23] = seg(0, 190, 115, 90, 25, gray);
    road->segments[24] = seg(0, 190, 120, 90, 25, gray);
    road->segments[25] = seg(0, 190, 125, 90, 25, gray);
    road->segments[26] = seg(0, 190, 130, 90, 25, gray);
    road->segments[27] = seg(0, 190, 135, 90, 25, gray);
    road->segments[28] = seg(0, 190, 140, 90, 25, gray);
    road->segments[29] = seg(0, 190, 145, 90, 25, gray);
}

int get_visible_segments(struct Road* road, struct Segment** tochange, float zm = 2) {
    struct Segment* visible = (struct Segment*)malloc(road->height * sizeof(struct Segment));
    int visible_length = 0;
    for (int v = 0; v <= road->height; v++) {
        float s = road->segments[v].width - (v) * (zm);
        float yoffset = road->segments[v].y - road->segments[v].z + road->playery;
        float heightoffset = road->segment_height * (s / road->width);

        if (yoffset > 0 && yoffset - heightoffset < SCREEN_HEIGHT) {
            visible[visible_length] = road->segments[v];
            visible_length++;
        }
    }
    *tochange = (struct Segment*) realloc(*tochange, sizeof(struct Segment) * visible_length);
    for (int v = 0; v < visible_length; v++) {
        (*tochange)[v] = visible[v];
    }
    return visible_length;
}

void draw_road(sf::RenderWindow* window, struct Road* road, float zm = 2) {
    int i = 0;
    float s;

    //struct Segment* visible = (struct Segment*)malloc(sizeof(struct Segment)); //only for initialization
    //int visible_len = get_visible_segments(road, &visible, zm);

    for (int v = road->height - 1;v >= 0;v--) { //draws each rectangle from furthest to closest
        s = road->segments[v].width - v * zm;
        float heightoffset = road->segments[v].height * (s / road->width);
        sf::RectangleShape rs(sf::Vector2f(s, heightoffset)); //makes a rectangle of width s and height relative to z position
        rs.setFillColor(road->segments[v].colour);
        //rs.setOutlineThickness(1);
        //rs.setOutlineColor(sf::Color(250, 150, 100));
        float xoffset = ((road->segments[v].x + 192) - s / 2) - (road->playerx * s); //display x position of rectangle (based on z and x axis)
        float yoffset = road->segments[v].y - road->segments[v].z + road->playery; //display y position of rectangle (based on z and y axis)
        rs.setPosition(xoffset, yoffset);
        window->draw(rs); //displays the rectangle
        sf::RectangleShape marking(sf::Vector2f(4, 4));

        marking.setFillColor(sf::Color::White);
        marking.setPosition(xoffset + s / 2, yoffset + 2);
        window->draw(marking);
        i++;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "KraD");
    struct Road road;
    road.height = 35;
    road.playerx = 0;
    road.playery = 0;
    road.width = 100;
    road.starty = 160;
    road.segment_height = 30;
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
            road.playerx -= 3 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            road.playerx += 3 * dt;
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
            road.playery -= 300 * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            road.playery += 300 * dt;
        }
        else if (event.key.code == sf::Keyboard::F) {
            struct Segment* visible = (struct Segment*)malloc(sizeof(struct Segment)); //only for initialization
            int visible_len = get_visible_segments(&road, &visible, CH_H);
            printf("%d", visible_len);
        }
        window.clear(sf::Color(0, 191, 255));
        draw_road(&window, &road, CH_W); //3rd param = change rate of width
        window.display();
    }
}
