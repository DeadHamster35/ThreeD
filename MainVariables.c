#include "../Library/MainInclude.h"
#include "OKInclude.h"


//Menu



SaveData SaveGame;
char colors[][3] = {
	{  0, 192, 255}, //A: blue
	{  0, 255,   0}, //B: green
	{255, 255, 255}, //Z: white
	{255,   0,   0}, //Start: red
	{192, 192, 192}, {192, 192, 192}, //up, down
	{192, 192, 192}, {192, 192, 192}, //left, right
	{  0,   0,   0}, {  0,   0,   0}, //unused
	{255, 255, 255}, {255, 255, 255}, //L, R
	{255, 255,   0}, {255, 255,   0}, //C up, down
	{255, 255,   0}, {255, 255,   0}  //C left, right
};
char coords[][2] = {
	{3, 1}, //A
	{3, 2}, //B
	{5, 1}, //Z
	{3, 0}, //start
	{1, 0}, {1, 2}, {0, 1}, {2, 1}, //d-pad
	{0, 0}, {0, 0}, //unused
	{4, 0}, //L
	{6, 0}, //R
	{5, 0}, //C up
	{5, 2}, //C down
	{4, 1}, //C left
	{6, 1}  //C right
};
char stickColor[][3] = {
	{192, 192, 192},
	{192, 192, 192},
	{192, 192, 192},
	{192, 192, 192},
	{255, 255, 255}
};
char stickCoord[][2] = {
	{1, 0}, //U
	{1, 2}, //D
	{0, 1}, //L
	{2, 1}, //R
	{1, 1} //stick
};

short ControllerInputX = 247;
short ControllerInputY = 215;




// Variables Draw3DRacer
short baseTurn, addTurn = 0;
//
//


//InputDisplay Variables
char *names[] = {
	"A", "B", "Z", "S",
	"U", "D", "L", "R", // draw d-pad
	"", "", //unused bits
	"L", "R",
	"U", "D", "L", "R" //C buttons
};

char *stickNames[] = {
	"U", "D", "L", "R","S"
};
