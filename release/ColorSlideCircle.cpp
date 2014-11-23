#include "ColorSlideCircle.h"

//START OF COLORSLIDECIRCLE
void ColorSlideCircle::setGoalProgress(double percent){
	color = mix(startColor, endColor, percent);
}
//END OF COLORSLIDECIRCLE