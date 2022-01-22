#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdlib.h>

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
#define SEG_WIDTH 70
#define C_RGB(r,g,b) (((r) << 11) | ((g) << 6) | (b))

const float CH_H = 4;
const float CH_W = 1.5;
const float dt = 0.05; // ;-;
float CH_HY = 0.70; //0->flat

int gray = C_RGB(155, 154, 166);
int lightgray = C_RGB(183, 187, 201);
int red = C_RGB(255, 46, 88);
int lightblue = C_RGB(173, 216, 230);

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
    int colour;
};

struct Segment seg(float, float, float, int, int, int);
void draw_road(struct Road*);
void gen_road(struct Road*);

int main() {
    struct Road road;
    road.playerx = 0;
    road.playery = 0;
    road.width = 100;
    road.starty = 160;
    road.segment_height = 30;
    road.height = 27;
    road.segments = (struct Segment*)malloc(road.height * sizeof(struct Segment));
    gen_road(&road);
    while (!keydown(KEY_MENU))
    {
		dclear(lightblue);
		clearevents();
        draw_road(&road);
		dupdate();
		const int left_down = keydown(KEY_LEFT);
        const int right_down = keydown(KEY_RIGHT);
        const int up_down = keydown(KEY_UP);
		const int down_down = keydown(KEY_DOWN); //pardon lephenixnoir :E

        if (right_down)
            road.playerx -= 5 * dt;
        else if (left_down)
            road.playerx += 5 * dt;
        else if (up_down)
        {
            road.playery -= 250 * dt; 
            CH_HY -= 5 / 4 * dt;
        }
        else if (down_down)
        {
            road.playery += 250 * dt;
            CH_HY += 5 / 4 * dt;
        }
    }
	return 1;
}

struct Segment seg(float x, float y, float z, int width, int height, int colour) {
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
        float s = SEG_WIDTH - 81 / CH_H;
        float w31 = 136 * (s / SEG_WIDTH);
        float h31 = 50 * (s / SEG_WIDTH);
        float w30 = SEG_WIDTH * (s / SEG_WIDTH);
        float h30 = 20 * (s / SEG_WIDTH);
        float y31 = 178 + h30; 
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
    /*pillar*/
        s = SEG_WIDTH - 117 / (CH_H);
        w31 = 136 * (s / SEG_WIDTH);
        h31 = 50 * (s / SEG_WIDTH);
        w30 = SEG_WIDTH * (s / SEG_WIDTH);
        h30 = 20 * (s / SEG_WIDTH);
        y31 = 190 + h30;
        h32 = 30 * (s / SEG_WIDTH);
        road->segments[19] = seg((w31 - w30) / -2, y31, 117, 136, 50, lightgray);
        road->segments[20] = seg((w31 - w30) / -2, y31 - h31, 117, 33, 70, lightgray);
        road->segments[21] = seg(w30, y31 - h31, 117, 33, 70, lightgray);
        road->segments[22] = seg(w30, (y31 - h31) - h32, 117, 33, 30, red);
        road->segments[23] = seg((w31 - w30) / -2, (y31 - h31) - h32, 117, 33, 30, red);
    /*pillar*/
    road->segments[24] = seg(0, 190, 126, SEG_WIDTH, 20, gray);
    road->segments[25] = seg(0, 190, 135, SEG_WIDTH, 20, gray);
    road->segments[26] = seg(0, 190, 144, SEG_WIDTH, 20, gray);

}

void draw_road(struct Road* road) {
    float s;
    for (int v = road->height - 1;v >= 0;v--) { //draws each rectangle from furthest to closest
        float yoffset = road->segments[v].y - road->segments[v].z * CH_HY + road->playery;
        s = SEG_WIDTH - road->segments[v].z / (CH_H);
        float heightoffset = road->segments[v].height * (s / SEG_WIDTH);
        if(yoffset < SCREEN_HEIGHT && yoffset+heightoffset < 0)
            continue;
        float widthoffset = road->segments[v].width * (s / SEG_WIDTH);
        float xoffset = ((road->segments[v].x + SCREEN_WIDTH / 2) - s / 2) - road->playerx * s;
		drect(xoffset, yoffset, xoffset + widthoffset, yoffset+heightoffset, road->segments[v].colour);
        if (road->segments[v].colour == gray) 
			drect(xoffset + s / 2, yoffset + 2, xoffset + s / 2 + 3, yoffset+2+3, C_WHITE);
    }
}
//hey MR
