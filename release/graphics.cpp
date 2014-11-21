#include "graphics.h"
#include <gl\glew.h>
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <mutex>


#define PI 3.14159265

//Isaac Bowen
//eecs481 fall2014

#include "graphics.h"
#include <math.h>

using std::mutex;

mutex LocationLock;

std::ostream& operator<<(std::ostream& os, const Color& c){
	std::cout << c.r << ' ' << c.g << ' ' << c.b;
	return os;
}


//Isaac Location code
Location::Location(double x_in, double y_in, double r_in, double pressure_in) : target{ x_in, y_in, r_in, WHITE, RED, GREEN }, rStart{ r_in }, start_pressure{ pressure_in }{
	std::cout << "Created a location at (x, y, r): " << x_in << " " << y_in << " " << r_in << std::endl;
	std::cout << "At depth: " << start_pressure << std::endl;
	turnOn();

}

void Location::makeBigger(double increase) {
	if (target.getR() < MAX_RADIUS)
		target.setR(target.getR() + increase);
}

std::string Location::toString() const {
	std::ostringstream ss;
	ss << "X: " << getX() << "Y: " << getY() << " R: " << getRadius() << " start_pressure: " << start_pressure << "\n";
	return ss.str();
}

void Location::makeSmaller(double decrease) {
	target.setR(target.getR() - decrease);
}

bool Location::contains(double x_in, double y_in) {
	return distance(x_in, y_in) < target.getR();
}

double Location::getPercentage(double input) {
	double deflection = abs(input - start_pressure);
	return (deflection / TARGET_PRESSURE <= 1.0 &&
			deflection / TARGET_PRESSURE >= 0.0) ?
				deflection / TARGET_PRESSURE : 0.0;
}

bool Location::withinPressure(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 250 && deflection >= TARGET_PRESSURE - 250;
}

bool Location::exactMatch(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 200 && deflection >= TARGET_PRESSURE - 200;

}

double Location::distance(double x_in, double y_in) {
	return sqrt(pow(target.getX() - x_in, 2) + pow(target.getY() - y_in, 2));
}

void Location::setPressure(double in) {
	pressure = in;
	if (in == 0)
		target.setGoalProgress(0);
	else
		target.setGoalProgress(getPercentage(in));
}
//Isaac Locatin code


namespace {
	Color mix(const Color& a, const Color& b, double percent){

		return Color{ a.r + (b.r - a.r) * percent,
			a.g + (b.g - a.g) * percent,
			a.b + (b.b - a.b) * percent };

	}
}


void ColorSlideCircle::setGoalProgress(double percent){
	color = mix(startColor, endColor, percent);
}


void Circle::draw(){


	if (fadeDuration != 0.0){
		double elapsed = duration_cast<duration<double, milli>>(high_resolution_clock::now() - fadeStart).count();

		color = (elapsed >= fadeDuration) ? WHITE : mix(startColor, WHITE, elapsed / fadeDuration);

	}

	if (fadeDuration != 0.0 && color == WHITE)
		return;


	glBegin(GL_TRIANGLE_FAN);


	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	
	/*
	x += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	y += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	*/


	glVertex2f((GLfloat)x, (GLfloat)y);

	for (int angle = 0; angle <= 360; angle += 2){
		glVertex2f((GLfloat) (x + sin(angle * PI / 180) * r),
				   (GLfloat) (y + cos(angle * PI / 180) * r));
	}

	glEnd();
}

void Circle::fade(double ms){
	fadeStart = high_resolution_clock::now();
	fadeDuration = ms;
	startColor = color;
}


void RandomCircleCursor::addCircle(){

	double theta = (rand() % 360) * PI / 180;
	
	double distanceToNew = r/(rand()%3 + 1);

	double dx = distanceToNew * cos(theta);
	double dy = distanceToNew * sin(theta);
	

	double newR = r / (rand() % 4 + 2);

	Color color = colorScheme_ocean[rand() % colorScheme_ocean.size()];

	//Color r{ 1.0 / (rand() % 10), 1.0 / (rand() % 2), 1.0 / (rand() % 10) };

	circles.push_front({ dx + x, dy + y, newR, color});

	circles.front().fade(rand()%1000 + 500);

}

void RandomCircleCursor::draw() {
	for (auto it = circles.rbegin(); it != circles.rend(); ++it){ it->draw(); }
	circles.remove_if([](const Circle& c) { return c.color == WHITE; });
}

/*
void CircleSpiral::addCircle(int depth){
	Circle prev = circles.front();
	static int angle = 0;
	double theta = (prev.angle + 5) * PI / 180;
	//double theta = (rand() % 360) * PI / 180;
	double distanceToNew = prev.r / 4;
	double dx = distanceToNew * cos(theta);
	double dy = distanceToNew * sin(theta);

	int normalized = (depth - 1100);

	double base_r = 100;

	double dr = (rand() % 2) ? prev.r / 10 : -prev.r / 10;

	Color c = prev.color == RED ? BLUE : (prev.color == BLUE ? GREEN : (prev.color == GREEN ? RED : RED));

	Color r{ 1.0 / (rand() % 10), 1.0 / (rand() % 10), 1.0 / (rand() % 2) };

	circles.push_front({ dx + prev.x, dy + prev.y, 100 - 0.2 * normalized, r, prev.angle + 5 });
}
*/

bool operator==(const Color& a, const Color& b) {
	return	a.r == b.r &&
		a.b == b.b &&
		a.g == b.g;
}

void PolygonGL::draw(){
	glBegin(GL_POLYGON);
	
	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	
	for(unsigned i = 0; i < vertices.size(); i++){
		glVertex2f((GLfloat)vertices[i].first, (GLfloat)vertices[i].second);
	}

	glEnd();
}

bool PolygonGL::inside(double x, double y){
	bool rtn = false;
	for (unsigned i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++){

		double xi = vertices[i].first;
		double xj = vertices[j].first;
		double yi = vertices[i].second;
		double yj = vertices[j].second;

		if (((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi) + xi))
			rtn = !rtn;


	} 

	return rtn;
}

void Point::draw(){
	glBegin(GL_POINTS);

	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);

	glVertex2f((GLfloat)x, (GLfloat)y);

	glEnd();
}

void Scene::draw() {
	for (auto& x : cursors) { x.draw(); }
	//for (CircleSpiral cs : spirals) { cs.draw(); }
	for (auto& x : polys) x.draw();
	for (auto& x : rings) x.draw();
	for (auto& x : points) x.draw();
	LocationLock.lock();
	for (auto& x : locations) x.draw();
	LocationLock.unlock();
	for (auto& x : circles) x.draw();
}


vector<Location> Scene::locations;
vector<RandomCircleCursor> Scene::cursors;
//vector<CircleSpiral> Scene::spirals;
vector<PolygonGL> Scene::polys;
vector<ColorSlideRing> Scene::rings;
vector<Point> Scene::points;
vector<Circle> Scene::circles;

vector<Color> colors{ RED, BLUE, GREEN, CYAN, MAGENTA, YELLOW, PINK, ORANGE, TEAL, PURPLE, TURQUOISE };

vector<Color> colorScheme_bleu{
	Color{ 52, 162, 169 },
	Color{82, 128, 169},
	Color{96, 94, 169},
	Color{121, 80, 169},
	Color{130, 61, 169}
};

vector<Color> colorScheme_desert{
	Color{130, 73, 92},
	Color{171, 85, 103},
	Color{204, 110, 110},
	Color{240, 153, 125},
	Color{250, 192, 135}
};

vector<Color> colorScheme_ocean{
	Color{0, 160, 120},
	Color{106, 74, 60},
	Color{204, 51, 63},
	Color{235, 104, 65},
	Color{237, 201, 81}
};



