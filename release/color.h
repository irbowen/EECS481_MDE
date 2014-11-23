#ifndef COLOR_H
#define COLOR_H
#include <gl\glew.h>
#include <Windows.h>
#include <vector>
#include <iostream>

using std::vector;

//START OF COLOR
struct Color {
	double r, g, b;
	Color(double rr, double gg, double bb) : r{ rr }, g{ gg }, b{ bb } {}
	Color(int rr, int gg, int bb) : r{ rr / 255.0 }, g{ gg / 255.0 }, b{ bb / 255.0 } {}
	Color() {}
};

std::ostream& operator<<(std::ostream& os, const Color& c);
bool operator==(const Color& a, const Color& b);
Color palette(Color& c);
Color mix(const Color& a, const Color& b, double percent);
//END OF COLOR

//START OF COLORWHEEL
class ColorWheel {
	int ticksPerColor, ticks;
	vector<Color> gradient;
	int cur_i;
public:
	// resolution of 1 means that calls to next() will cycle through colors in gradient. Higher resolutions add inbetween colors.
	ColorWheel(const vector<Color>& colors, int resolution) : ticksPerColor{ resolution }, ticks{ 0 }, cur_i{ 0 } { gradient = colors; }
	Color next();
};
//END OF COLORWHEEL


//a bunch of palettes and colors
#define RED Color{1.0f, 0.0f, 0.0f}
#define BLUE Color{0.0f, 0.0f, 1.0f}
#define GREEN Color{0.0f, 1.0f, 0.0f}

#define CYAN Color{0.0, 1.0, 1.0}
#define MAGENTA Color{1.0, 0.0, 1.0}
#define YELLOW Color{1.0, 1.0, 0.0}

#define PINK Color{1.0, 0.51, 0.75}
#define ORANGE Color{0.98, 0.45, 0.02}
#define TEAL Color{0.01, 0.58, 0.53}

#define PURPLE Color{0.5, 0.12, 0.61}
#define TURQUOISE Color{0.02, 0.76, 0.67}

// all predefined colors
extern vector<Color> colors;

// predefined color schemes
extern vector<Color> colorScheme_bleu;
extern vector<Color> colorScheme_desert;
extern vector<Color> colorScheme_ocean;
extern vector<Color> colorScheme_goldfish;
extern vector<Color> colorScheme_melon;
extern vector<Color> colorScheme_emo;
extern vector<Color> colorScheme_rainbow;

// pointers to predefined schemes
extern vector<vector<Color>*> colorSchemes;

#define WHITE Color{1.0, 1.0, 1.0}

#endif