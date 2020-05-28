#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 50;
const int WIDTH_MAP = 16;
float counterBananas = 0.f;
float counterStackOfBananas = 0.f;
float counterLianas = 0.f;
float counterGrass = 0.f;
float counterDirt = 0.f;
float counterEmptySpace = 0.f;
float counterEatenBananas = 0.f;


sf::String TileMap[HEIGHT_MAP] = {
	"1              1",
	"1              1",
	"1   g          1",
	"1 <===>        1",
	"1              1",
	"1           s  1",
	"1          <=> 1",
	"1   s          1",
	"1  <==>        1",
	"1        b     1",
	"1         <>   1",
	"1              1",
	"1       b      1",
	"1   <>         1",
	"1           s  1",
	"1         <==> 1",
	"1              1",
	"1   s          1",
	"1  <>    b     1",
	"1              1",
	"1           b  1",
	"1         <==> 1",
	"1              1",
	"1   s          1",
	"1  <=>         1",
	"1              1",
	"1           b  1",
	"1         <==> 1",
	"1    s         1",
	"1  <==>        1",
	"1              1",
	"1          b   1",
	"1         <=>  1",
	"1              1",
	"1   s          1",
	"1  <==>        1",
	"1              1",
	"1              1",
	"1          b   1",
	"1         <=>  1",
	"1              1",
	"1     b        1",
	"1   <=>        1",
	"1              1",
	"1          b   1",
	"1         <=>  1",
	"1              1",
	"1              1",
	"0000000000000000",
	"dddddddddddddddd",
};

/*b - banana
  s - stack of bananas
  0 - grass
  d - dirt
  1 - liana
  e - eaten bananas */

void mapObjectsNullifier() {
	counterBananas = 0.f;
	counterStackOfBananas = 0.f;
	counterLianas = 0.f;
	counterGrass = 0.f;
	counterDirt = 0.f;
	counterEmptySpace = 0.f;
	counterEatenBananas = 0.f;
}

void mapObjectsCounter() { // counting num of objects on map;
	mapObjectsNullifier();
	for (int i = 0; i < HEIGHT_MAP; i++) {
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == ' ') counterEmptySpace++;
			if (TileMap[i][j] == '1') counterLianas++;
			if (TileMap[i][j] == '0') counterGrass++;
			if (TileMap[i][j] == 'd') counterDirt++;
			if (TileMap[i][j] == 'b') counterBananas++;
			if (TileMap[i][j] == 's') counterStackOfBananas++;
			if (TileMap[i][j] == 'e') counterEatenBananas++;

		}
	}
}
