#include <gl\glew.h>
#include <Windows.h>
#include <iostream>
#include "color.h"


// START OF COLOR
bool operator==(const Color& a, const Color& b) {
	return	a.r == b.r &&
		a.b == b.b &&
		a.g == b.g;
}

std::ostream& operator<<(std::ostream& os, const Color& c){
	std::cout << c.r << ' ' << c.g << ' ' << c.b;
	return os;
}

Color palette(Color& c){
	Color a = c;
	a.r += (((rand() % 200) - 100) / 300.0);
	a.g += (((rand() % 200) - 100) / 300.0);
	a.b += (((rand() % 200) - 100) / 300.0);
	if (a.r < 0) a.r = 0;
	else if (a.r > 1) a.r = 1;
	if (a.g < 0) a.g = 0;
	else if (a.g > 1) a.g = 1;
	if (a.b < 0) a.b = 0;
	else if (a.b > 1) a.b = 1;
	return a;
}

Color mix(const Color& a, const Color& b, double percent){

	return Color{ a.r + (b.r - a.r) * percent,
		a.g + (b.g - a.g) * percent,
		a.b + (b.b - a.b) * percent };

}

//END OF COLOR
//START OF COLORWHEEL
Color ColorWheel::next(){
	int nxt_i = (cur_i == gradient.size() - 1) ? 0 : cur_i + 1;
	Color rtn = mix(gradient[cur_i], gradient[nxt_i], 1.0 * ticks / ticksPerColor);

	if (++ticks == ticksPerColor){
		ticks = 0;
		cur_i = nxt_i;
	}

	return rtn;
}
//END OF COLORWHEEL
//GLOBAL STATIC VECTORS
vector<Color> colors{ RED, BLUE, GREEN, CYAN, MAGENTA, YELLOW, PINK, ORANGE, TEAL, PURPLE, TURQUOISE };
vector<Color> colorScheme_bleu{
	Color{ 52, 162, 169 },
	Color{ 82, 128, 169 },
	Color{ 96, 94, 169 },
	Color{ 121, 80, 169 },
	Color{ 130, 61, 169 }
};

vector<Color> colorScheme_desert{
	Color{ 130, 73, 92 },
	Color{ 171, 85, 103 },
	Color{ 204, 110, 110 },
	Color{ 240, 153, 125 },
	Color{ 250, 192, 135 }
};

vector<Color> colorScheme_ocean{
	Color{ 0, 160, 120 },
	Color{ 106, 74, 60 },
	Color{ 204, 51, 63 },
	Color{ 235, 104, 65 },
	Color{ 237, 201, 81 }
};

vector<Color> colorScheme_goldfish{
	Color{ 105, 210, 231 },
	Color{ 167, 219, 216 },
	Color{ 224, 228, 204 },
	Color{ 243, 134, 48 },
	Color{ 250, 105, 0 }
};

vector<Color> colorScheme_melon{
	Color{ 209, 242, 165 },
	Color{ 239, 250, 180 },
	Color{ 255, 196, 140 },
	Color{ 255, 159, 128 },
	Color{ 245, 105, 145 }
};

vector<Color> colorScheme_emo{
	Color{ 85, 98, 112 },
	Color{ 78, 205, 196 },
	Color{ 199, 244, 100 },
	Color{ 255, 107, 107 },
	Color{ 196, 77, 88 }
};

vector<Color> colorScheme_rainbow{
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE,
	PURPLE
};

vector<vector<Color>*> colorSchemes{
	&colorScheme_emo,
	&colorScheme_bleu,
	&colorScheme_desert,
	&colorScheme_goldfish,
	&colorScheme_ocean,
	&colorScheme_melon
};
