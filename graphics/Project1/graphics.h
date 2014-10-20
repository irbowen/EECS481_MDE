#include <vector>

using std::vector;

class Target {
	int x, y;
	int r, rStart;
	int pressure, targetPressure;
	bool on;
public:
	Target(int xx, int yy, int rr, int target) : x{ xx }, y{ yy }, r{ rr }, rStart{ rr }, targetPressure{ target }, on{ false } {}
	void draw(); // Draw circle if on
	inline void turnOn() { on = true; }
	inline void turnOff() { on = false; }
	void setPressure(int); // Also adjust r so it scales down as pressure->targetPressure
						   // [pressure = 0 -> r = rStart ;; pressure = targetPressure -> r = rStart / k for some const k]
	bool hitTarget(); // Is pressure close enough to targetPressure?
};

class Scene {
	vector<Target> targets;
public:
	Scene(); // initialize size and location of fixed targets
};