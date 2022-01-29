#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdlib.h>

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
#define SEG_WIDTH 70
#define C_RGB(r,g,b) ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3)

#define SCREEN_HEIGHT 216
#define SCREEN_WIDTH 384
#define GET_POS(x) ((abs(x)+x)/2)
#define SEG_WIDTH 70
#define CH_Z 9
const float CH_H = 4;
const float CH_W = 1.3;
float CH_HY = 0.80; 
const float dt = 0.01;
int gray = C_RGB(131, 118, 156);
int lightgray = C_RGB(194, 195, 199);
int red = C_RGB(255, 0, 77);
int lightblue = C_RGB(41, 173, 255);

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
    int colour;
};

struct Segment seg(float x, float y, float z, int width, int height, int colour) {
    struct Segment segment = { x, y, z, width, height, colour };
    return segment;
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
            road->segments[v++] = seg((w31 - w30) / -2, y31, z, 120, 50, lightgray);
            road->segments[v++] = seg((w31 - w30) / -2, y31 - h31, z, w32, 70, lightgray);
            road->segments[v++] = seg( w30, y31 - h31, z, w32, 70, lightgray);
            road->segments[v++] = seg(w30, y31 - h31 - h32, z, w32, w32, red);
            road->segments[v] = seg((w31 - w30) / -2, y31 - h31 - h32, z, w32, w32, red);
        }
        v++;
    }
    
}

void draw_road(struct Road* road) {
    float s;
    for (int v = road->height - 1;v >= 0;v--) { //draws each rectangle from furthest to closest
        s = SEG_WIDTH - road->segments[v].z / (CH_H * CH_W);
        float yoffset = road->segments[v].y - road->segments[v].z * CH_HY + road->playery;
        float heightoffset = road->segments[v].height * (s / SEG_WIDTH);
        float widthoffset = road->segments[v].width * (s / SEG_WIDTH);
        float xoffset = ((road->segments[v].x + SCREEN_WIDTH / 2) - s / 2) - road->playerx * (s - road->segments[v].z / 10);

        if (yoffset > SCREEN_HEIGHT || yoffset + heightoffset < 0 || xoffset + widthoffset < 0 || xoffset - widthoffset > SCREEN_WIDTH)
            continue;

		drect(xoffset, yoffset, xoffset + widthoffset, yoffset+heightoffset, road->segments[v].colour);
        if (road->segments[v].colour == gray) 
			drect(xoffset + s / 2, yoffset + 2, xoffset + s / 2 + 3, yoffset+2+3, C_WHITE);
    }
}

int main() {
    struct Road road;
    road.height = 32;
    road.playerx = 0;
    road.playery = 0;
    road.width = 100;
    road.starty = 160;
    road.startx = 40;
    road.segment_height = 30;
    road.segments = (struct Segment*)malloc(road.height * sizeof(struct Segment));

    unsigned char grid[] = {0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x14,0x04,0x04,0x03,0x02, 0x01,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00};
    gen_road(grid, sizeof(grid), &road);
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
            road.playerx -= 10 * dt;
        else if (left_down)
            road.playerx += 10 * dt;
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

//hey MR
//yo duck goose
