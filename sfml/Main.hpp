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

void draw_road(sf::RenderWindow*, struct Road*);
void gen_road(unsigned char* , int , struct Road *);
struct Segment seg(float , float , float , int , int , sf::Color );