#include <SFML/Graphics.hpp>


sf::Color gray = sf::Color(100, 100, 100);
struct Segment {
	int y;
	int x;
	int z;
	int width;
};
struct Road {
	int height; //number of elements
	struct Segment *segments; //array of segments
	float playerx; 
	float playery;
	int width;
	int segment_height;
	int starty;
};

void gen_road(struct Road* road, int zm=1) {
	int v = road->height;
	for (int i = 0;i < road->height;i++) {
		road->segments[i].width = road->width;
		road->segments[i].z = i*zm;
		road->segments[i].y = road->starty;
		road->segments[i - 1].x = 0;
		v--;
	}
}

void draw_road(sf::RenderWindow *window, struct Road* road, int zm=2) {
	int i = 0;
	float s;
	for (int v = road->height;v >= 0;v--) { //draws each rectangle from furthest to closest
		s = road->segments[v].width - v * zm;
		float heightoffset = road->segment_height * (s / road->width);
		sf::RectangleShape rs(sf::Vector2f(s, heightoffset)); //makes a rectangle of width s and height relative to z position
		rs.setFillColor(gray);
		//rs.setOutlineThickness(1);
		//rs.setOutlineColor(sf::Color(250, 150, 100));
		float xoffset = ( (road->segments[v].x + 192) - s / 2) - (road->playerx * s); //displayed x position of rectangle (based on z and x axis)
		float yoffset = road->segments[v].y - road->segments[v].z + road->playery; //displayed y position of rectangle (based on z and y axis)
		rs.setPosition( xoffset , yoffset ); 
		window->draw(rs); //displays the rectangle
		sf::RectangleShape marking(sf::Vector2f(4, 4));

		marking.setFillColor(sf::Color::White);
		marking.setPosition(xoffset + s/2, yoffset+2);
		window->draw(marking);
		i++;
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(384, 216), "KraD");
	struct Road road;
	road.height = 20;
	road.playerx = 0;
	road.playery = -20;
	road.width = 100;
	road.starty = 200;
	road.segment_height = 20;
	road.segments = (struct Segment*)malloc(road.height * sizeof(struct Segment));
	gen_road(&road, 7);
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
			}
		}
		window.clear(sf::Color(0, 191, 255));
		draw_road(&window, &road, 3); //3rd param = change rate of width
		window.display();
	}
}
