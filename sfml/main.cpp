#include <SFML/Graphics.hpp>
#include <math.h>

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
#define ZM 10

sf::Color gray = sf::Color(100, 100, 100);

int testvalue = 15;
struct Segment {
    int y;
    int x;
    int z;
    int width;
    int height;
};
struct Road {
    int height; //number of elements
    struct Segment* segments; //array of segments
    float playerx;
    float playery;
    int width;
    int segment_height;
    int starty;
};

void gen_road(struct Road* road, float zm = 1) {
    int v = road->height;
    for (int i = 0;i < road->height;i++) {
        road->segments[i].width = road->width;
        road->segments[i].height = road->height;
        road->segments[i].z = i * zm;
        road->segments[i].y = road->starty + zm;
        road->segments[i - 1].x = 0;
        v--;
    }
}

int get_visible_segments(struct Road* road, struct Segment** tochange, float zm = 2) {
    struct Segment* visible = (struct Segment*)malloc(road->height * sizeof(struct Segment));
    int visible_length = 0;
    for (int v = 0; v <= road->height; v++) {
        float s = road->segments[v].width - (v) * (zm);
        float yoffset = road->segments[v].y - road->segments[v].z + road->playery;
        float heightoffset = road->segment_height * (s / road->width);

        if (yoffset > 0 && yoffset - heightoffset  < SCREEN_HEIGHT) {
            visible[visible_length] = road->segments[v];
            visible_length++;
        }
    }
    *tochange = (struct Segment*)realloc(*tochange, sizeof(struct Segment) * visible_length);
    for (int v = 0; v < visible_length; v++) {
        (*tochange)[v] = visible[v];
    }
    return visible_length;
}

void draw_road(sf::RenderWindow* window, struct Road* road, float zm = 2) {
    int i = 0;
    float s;

    struct Segment* visible = (struct Segment*)malloc(sizeof(struct Segment)); //only for initialization
    int visible_len = get_visible_segments(road, &visible, zm);

    for (int v = road->height;v >= 0;v--) { //draws each rectangle from furthest to closest
        s = road->segments[v].width - (v) * (zm);

        float heightoffset = road->segment_height * (s / road->width);
        sf::RectangleShape rs(sf::Vector2f(s, heightoffset)); //makes a rectangle of width s and height relative to z position
        rs.setFillColor(gray);
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
    road.height = 15;
    road.playerx = 0;
    road.playery = 0;
    road.width = 100;
    road.starty = 200;
    road.segment_height = 20;
    road.segments = (struct Segment*)malloc(road.height * sizeof(struct Segment));
    gen_road(&road, ZM); //2nd param = change rate of height
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::D)
                {
                    road.playerx -= 0.1;
                }
                else if (event.key.code == sf::Keyboard::A)
                {
                    road.playerx += 0.1;
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    for (int i = 0;i < road.height;i++) {
                        road.segments[i].x -= 4;
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    for (int i = 0;i < road.height;i++) {
                        road.segments[i].x += 4;
                    }
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    road.playery -= 4;
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    road.playery += 4;
                }
                else if (event.key.code == sf::Keyboard::F) {
                    struct Segment* visible = (struct Segment*)malloc(sizeof(struct Segment)); //only for initialization
                    int visible_len = get_visible_segments(&road, &visible, ZM);
                    printf("%d", visible_len);
                }
            }
        }
        window.clear(sf::Color(0, 191, 255));
        draw_road(&window, &road, 1.1); //3rd param = change rate of width
        window.display();
    }
}
