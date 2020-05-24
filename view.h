#include <SFML/Graphics.hpp>
using namespace sf;

View view;
float viewCoordinateY = 2200;

View getPlayerCoordinateForView(float x, float y) {
    float tempX = x, tempY = y;
    if (y > 2200) { tempY = 2200; }
    if (y < 300) { tempY = 300; }
    view.setCenter(400, tempY);
    return view;
}


