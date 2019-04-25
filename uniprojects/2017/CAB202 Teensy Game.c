// This was a program written in my second semester in my first year of QUT.
// It would generate a game that was playable on the Teensy microcontroller.
// IMPORTANT: FOR ANYONE TESTING THIS THAT DOESN'T CARE ABOUT TERMINAL UPDATES
// COMMENT OUT 1525-1533

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <macros.h>
#include <graphics.h>
#include <lcd.h>
#include <sprite.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <usb_serial.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "lcd_model.h"

#define FREQ 8000000.0
#define PRESCALE 1024.0
#define TRUE_LCD_X 126
#define TRUE_LCD_Y 72

// Functions from ZDK.

void adc_init() {
	// ADC Enable and pre-scaler of 128: ref table 24-5 in datasheet
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
	// Select AVcc voltage reference and pin combination.
	// Low 5 bits of channel spec go in ADMUX(MUX4:0)
	// 5th bit of channel spec goes in ADCSRB(MUX5).
	ADMUX = (channel & ((1 << 5) - 1)) | (1 << REFS0);
	ADCSRB = (channel & (1 << 5));

	// Start single conversion by setting ADSC bit in ADCSRA
	ADCSRA |= (1 << ADSC);

	// Wait for ADSC bit to clear, signalling conversion complete.
	while ( ADCSRA & (1 << ADSC) ) {}

	// Result now available.
	return ADC;
}

bool sprite_move( sprite_id sprite, double dx, double dy ) {
	assert( sprite != NULL );
	int x0 = round( sprite->x );
	int y0 = round( sprite->y );
	sprite->x += dx;
	sprite->y += dy;
	int x1 = round( sprite->x );
	int y1 = round( sprite->y );
	return ( x1 != x0 ) || ( y1 != y0 );
}

bool sprite_move_to( sprite_id sprite, double x, double y ) {
	assert( sprite != NULL );
	int x0 = round( sprite->x );
	int y0 = round( sprite->y );
	sprite->x = x;
	sprite->y = y;
	int x1 = round( sprite->x );
	int y1 = round( sprite->y );
	return ( x1 != x0 ) || ( y1 != y0 );

}

double sprite_x( sprite_id sprite ) {
	assert( sprite != NULL );
	return sprite->x;
}

double sprite_y( sprite_id sprite ) {
	assert( sprite != NULL );
	return sprite->y;
} 

void sprite_turn_to( sprite_id sprite, double dx, double dy ) {
	assert( sprite != NULL );
	sprite->dx = dx;
	sprite->dy = dy;
}

double sprite_dx( sprite_id sprite ) {
	assert( sprite != NULL );
	return sprite->dx;
}

double sprite_dy( sprite_id sprite ) {
	assert( sprite != NULL );
	return sprite->dy;
}

void sprite_turn( sprite_id sprite, double degrees ) {
	double radians = degrees * M_PI / 180;
	double s = sin( radians );
	double c = cos( radians );
	double dx = c * sprite->dx + s * sprite->dy;
	double dy = -s * sprite->dx + c * sprite->dy;
	sprite->dx = dx;
	sprite->dy = dy;
}

bool sprite_step( sprite_id sprite ) {
	assert( sprite != NULL );
	int x0 = round( sprite->x );
	int y0 = round( sprite->y );
	sprite->x += sprite->dx;
	sprite->y += sprite->dy;
	int x1 = round( sprite->x );
	int y1 = round( sprite->y );
	return ( x1 != x0 ) || ( y1 != y0 );
}

bool sprite_hide( sprite_id sprite ) {
	assert( sprite != NULL );
	bool old_val = sprite->is_visible;
	sprite->is_visible = false;
	return old_val;
}

bool sprite_show( sprite_id sprite ) {
	assert( sprite != NULL );
	bool old_val = sprite->is_visible;
	sprite->is_visible = true;
	return ! old_val;
}

void sprite_destroy( sprite_id sprite ) {
	if ( sprite != NULL ) {
		free( sprite );
	}
}

void draw_formatted(int x, int y, const char * format, ...) { 
	va_list args;
	va_start(args, format);
	char buffer[50];
	vsprintf(buffer, format, args);
	draw_string(x, y, buffer, FG_COLOUR);
}

void usb_serial_formatted(char * message, ...) { 
	va_list args;
	va_start(args, message);
	char buffer[50];
	vsprintf(buffer, message, args);
	usb_serial_write((uint8_t *) buffer, strlen(buffer));
}

int sprite_width( sprite_id sprite ) {
	assert( sprite != NULL );
	return sprite->width;
}

int sprite_height( sprite_id sprite ) {
	assert( sprite != NULL );
	return sprite->height;
}

bool sprite_visible( sprite_id sprite ) {
	assert( sprite != NULL );
	return sprite->is_visible;
}

// End functions from ZDK (and other sources).

// Bitmaps.
uint8_t playerBitmap[] = {
0b00110000,
0b00110000,
0b11111100,
0b10110100,
0b00110000,
0b00110000,
};

uint8_t monsterBitmap[] = {
0b00100000,
0b01110000,
0b11111000,
0b01110000,
0b00100000,
};

uint8_t keyBitmap[] = {
0b11100000,
0b10111100,
0b11101100,
};

uint8_t doorBitmap[] = {
0b11111000,
0b10001000,
0b10001000,
0b11001000,
0b10001000,
0b10001000,
};

uint8_t castleBitmap[] = {
0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b01110001,0b11000111,0b00011100,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,
0b11100000,0b00000000,0b00000000,0b00000000,0b00000001,0b11000000,
0b10110000,0b00000000,0b00000000,0b00000000,0b00000011,0b01000000,
0b10011000,0b00000000,0b00000000,0b00000000,0b00000110,0b01000000,
0b11111111,0b11111111,0b11000000,0b11111111,0b11111111,0b11000000,
} ;

uint8_t verticalBorder[] = { 
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
};

uint8_t horizontalBorder[] = {
0b11111111,	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111, 0b11111111,0b11111111,0b11111111,0b11111111,0b11111100
};

uint8_t verticalWall[] = {
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000,
0b11100000
};

uint8_t horizontalWall[] = {
0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111000,
0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111000,
0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111000,
};

uint8_t dot[] = {
0b10000000
};

uint8_t plus[] = {
0b01110000,
0b10101000,
0b11111000,
0b10101000,
0b01110000,
};

uint8_t shieldBitmap[] = {
0b11111000,
0b10001000,
0b10001000,
0b01110000,
0b00100000,
};

uint8_t bombBitmap[] = {
0b11111000,
0b00100000,
0b11111000,
0b11111000,
0b11111000,	
};

uint8_t bowBitmap[] = {
0b11100000,
0b10010000,
0b10001000,
0b10010000,
0b11100000,		
};

uint8_t arrowBitmap[] = {
0b11000000,
0b11000000,
};
// End bitmaps.

// Initial variables.
Sprite player;
Sprite monster1;
Sprite monster2;
Sprite monster3;
Sprite monster4;
Sprite monster5;
Sprite wall1;
Sprite wall2;
Sprite wall3;
Sprite wall4;
Sprite wall5;
Sprite wall6;
Sprite treasure1;
Sprite treasure2;
Sprite treasure3;
Sprite treasure4;
Sprite treasure5;
Sprite shield;
Sprite bow;
Sprite bomb;
Sprite arrow;
Sprite key;
Sprite door;
Sprite castle;
Sprite leftSide;
Sprite rightSide;
Sprite upSide;
Sprite downSide;
Sprite leftBorder;
Sprite rightBorder;
Sprite upBorder;
Sprite downBorder;

sprite_id allObjects[30];
sprite_id solidObjects[10];
sprite_id enemies[10];
sprite_id treasure[10];
sprite_id weapon;

bool menuScreen;
bool keyObtained;
bool leftPressed;
bool rightPressed;
bool upPressed;
bool downPressed;
bool leftMove;
bool rightMove;
bool upMove;
bool downMove;
bool bombHasBeenPlanted;
bool arrowShot;
int lives;  
int score;
int level;
int finalFloor;
int time1; 
int allSpriteCounter = 0;
int solidSpriteCounter = 0;
int enemySpriteCounter = 0;
int treasureSpriteCounter = 0;
int randomSeed = 0;
int treasureCollected[10];
int arrows;
int potentiometer;
int pos;
double potentiometer1;
double potentiometer2;
double time2; 
double gameTimeStart;
double bombTime;
double aim;
volatile int overflow_count = 0;
volatile bool overflowed = false;
// End initial variables.

void setup() { 
	clear_screen();
	
	menuScreen = false;
	keyObtained = false;
	leftPressed = false;
	rightPressed = false;
	upPressed = false;
	downPressed = false;
	lives = 3;  
	score = 0;
	level = 0;
	time1 = 0; 
	time2 = 0; 
	
	draw_string(7, 0, "David McClarty", FG_COLOUR);
    draw_string(20, 10, "n9939768", FG_COLOUR);
	draw_string(0, 30, "Press SW2 or SW3", FG_COLOUR);
	
	sprite_init(&player, 39, 24, 6, 6, playerBitmap);
	sprite_init(&key, 6, 12, 6, 3, keyBitmap);
	allObjects[0] = &key;
	sprite_init(&door, 39, 14, 5, 6, doorBitmap);
	allObjects[1] = &door;
		
	sprite_init(&leftSide, 1, LCD_Y/2, 1, 1, dot);
	sprite_init(&rightSide, LCD_X, LCD_Y/2, 1, 1, dot);
	sprite_init(&upSide, LCD_X/2, 1, 1, 1, dot);
	sprite_init(&downSide, LCD_X/2, LCD_Y, 1, 1, dot);
	
	sprite_init(&leftBorder, 1, -11, 1, TRUE_LCD_Y, verticalBorder);
	allObjects[2] = &leftBorder;
	sprite_init(&rightBorder, TRUE_LCD_X, -11, 1, TRUE_LCD_Y, verticalBorder);
	allObjects[3] = &rightBorder;
	sprite_init(&upBorder, 1, -11, TRUE_LCD_X, 1, horizontalBorder);
	allObjects[4] = &upBorder;
	sprite_init(&downBorder, 1, TRUE_LCD_Y - 12, TRUE_LCD_X, 1, horizontalBorder);
	allObjects[5] = &downBorder;
	sprite_init(&castle, 20, -11, 42, 30, castleBitmap);
	allObjects[6] = &castle;
	sprite_init(&monster1, 70, 12, 5, 5, monsterBitmap);
	allObjects[7] = &monster1;
	allSpriteCounter = 8;
	
	solidObjects[0] = &door;
	solidObjects[1] = &castle;
	solidSpriteCounter = 2;
	enemies[0] = &monster1;
	enemySpriteCounter = 1;
	
	sprite_init(&wall1, 1, -11, 3, 15, verticalWall);
	sprite_init(&wall2, TRUE_LCD_X/2, -11, 3, 15, verticalWall);
	sprite_init(&wall3, TRUE_LCD_X, -11, 3, 15, verticalWall);
	sprite_init(&wall4, TRUE_LCD_X/2, -6, 26, 3, horizontalWall);
	sprite_init(&wall5, TRUE_LCD_X/2, TRUE_LCD_Y - 12, 26, 3, horizontalWall);
	sprite_init(&wall6, LCD_X, TRUE_LCD_Y - 12, 26, 3, horizontalWall);
	
	sprite_init(&treasure1, 1, -6, 5, 5, plus);
	sprite_init(&treasure2, TRUE_LCD_X/3, -6, 5, 5, plus);
	sprite_init(&treasure3, TRUE_LCD_X/3, TRUE_LCD_Y - 6, 5, 5, plus);
	sprite_init(&treasure4, (2*TRUE_LCD_X)/3, -6, 5, 5, plus);
	sprite_init(&treasure5, (2*TRUE_LCD_X)/3, TRUE_LCD_Y - 6, 5, 5, plus);
	
	sprite_init(&monster2, 3, 1, 5, 5, monsterBitmap);
	sprite_init(&monster3, 3, 1, 5, 5, monsterBitmap);
	sprite_init(&monster4, 3, 1, 5, 5, monsterBitmap);
	sprite_init(&monster5, 3, 1, 5, 5, monsterBitmap);
	
	sprite_init(&shield, 6, 9, 5, 5, shieldBitmap);
	sprite_init(&bomb, 6, 9, 5, 5, bombBitmap);
	sprite_init(&bow, 6, 9, 5, 5, bowBitmap);
	sprite_init(&arrow, 6, 9, 2, 2, arrowBitmap);
	sprite_hide(&arrow);
	
}

bool spriteCollided(sprite_id sprite1, sprite_id sprite2) {
	if (sprite_x(sprite1) <= (sprite_x(sprite2) + (sprite_width(sprite2) - 1)) 
	&& sprite_x(sprite1) + (sprite_width(sprite1) - 1) >= (sprite_x(sprite2))
	&& sprite_y(sprite1) <= (sprite_y(sprite2) + (sprite_height(sprite2) - 1)) 
	&& sprite_y(sprite1) + (sprite_height(sprite1) - 1) >= (sprite_y(sprite2))
	) {
		return true;
	}
	
	else {
		return false;
	}
}

void regenerate() {
	int randX = rand() % ((TRUE_LCD_X - 5) / 2);
	int randY = rand() % ((TRUE_LCD_Y - 5) / 2);
	
	if ((TRUE_LCD_X - 86) - sprite_x(&leftBorder) <= TRUE_LCD_X/2) {
		randX = -randX;
	}
	
	if ((TRUE_LCD_Y - 47)  - sprite_y(&upBorder) <= TRUE_LCD_Y/2) {
		randY = -randY;
	}
	
	for (int i = 0; i < solidSpriteCounter; i++) {
		if (solidObjects[i] != NULL) {
			if (39 <= (sprite_x(solidObjects[i]) + randX) 
			&& 44 >= (sprite_x(solidObjects[i]) + sprite_width(solidObjects[i]) + randX)
			&& 24 <= (sprite_y(solidObjects[i]) + sprite_height(solidObjects[i])) 
			&& 29 >= (sprite_y(solidObjects[i]))) {
				if (44 < (sprite_x(&rightBorder) - 2)){
					randX++;
				}
				else if (24 < sprite_x(&downBorder) - 2) {
					randX--;
					randY++;
				}
				
				else {
					randX--;
					randY--;
				}
			}
		}
	}
	
	for (int i = 0; i < enemySpriteCounter; i++) {
		if (enemies[i] != NULL) {
			if (39 <= (sprite_x(enemies[i]) + randX) 
			&& 44 >= (sprite_x(enemies[i]) + sprite_width(enemies[i]) + randX)
			&& 24 <= (sprite_y(enemies[i]) + sprite_height(enemies[i])) 
			&& 29 >= (sprite_y(enemies[i]))) {
				if (44 < (sprite_x(&rightBorder) - 7)){
					randX += 6;
				}
				else if (24 < sprite_x(&downBorder) - 7) {
					randX -= 6;
					randY += 6;
				}
				
				else {
					randX -= 6;
					randY += 6;
				}
			}
		}
	}
	for (int i = 0; i < allSpriteCounter; i++) {
		if (allObjects[i] != NULL) {
			sprite_move_to(allObjects[i], sprite_x(allObjects[i]) + randX, sprite_y(allObjects[i]) + randY);
		}
	}
}

void regenerate2() { 
	for (int i = 0; i < allSpriteCounter; i++) {
		if (allObjects[i] != &leftBorder
		&& allObjects[i] != &rightBorder
		&& allObjects[i] != &upBorder
		&& allObjects[i] != &downBorder
		){
			int randX = sprite_x(&leftBorder) + 1 + (rand() % (TRUE_LCD_X - sprite_width(allObjects[i])));
			int randY = sprite_y(&upBorder) + 1 + (rand() % (TRUE_LCD_Y - sprite_height(allObjects[i])));
		
			for (int j = 0; j < solidSpriteCounter; j++) {
				if (solidObjects[i] != NULL) {
					if (randX <= (sprite_x(solidObjects[j]) + (sprite_width(solidObjects[j]) - 1)) 
					&& randX + (sprite_width(allObjects[i]) - 1) >= (sprite_x(solidObjects[j]))
					&& randY <= (sprite_y(solidObjects[j]) + (sprite_height(solidObjects[j]) - 1)) 
					&& randY + (sprite_height(allObjects[i]) - 1) >= (sprite_y(solidObjects[j]))
					) {
						if (randX < (sprite_x(&rightBorder) - (2*(sprite_width(allObjects[i]))))){
							randX += sprite_width(allObjects[i]);
						}
						else { 
							randX -= sprite_width(allObjects[i]);
						}
						
						if (randY < sprite_y(&downBorder) - (2*(sprite_height(allObjects[i])))) {
							randY += sprite_height(allObjects[i]);
						}
						
						else { 
							randY -= sprite_height(allObjects[i]);
						}
					}	
					if (allObjects[i] == &wall1
					|| allObjects[i] == &wall2
					|| allObjects[i] == &wall3
					|| allObjects[i] == &wall4
					|| allObjects[i] == &wall5
					|| allObjects[i] == &wall6
					|| allObjects[i] == &door
					){
						if (randX > (sprite_x(&rightBorder) - (2*(sprite_width(allObjects[i])) + 11))){
							randX -= sprite_width(allObjects[i]);
						}
						else if (randX < sprite_x(&leftBorder) + (sprite_width(allObjects[i]) + 8)) {
							randX += sprite_width(allObjects[i]);
						}
						
						if (randY > sprite_y(&downBorder) - (2*(sprite_height(allObjects[i])) + 11)) {
							randY -= sprite_height(allObjects[i]);
						}
						
						else if (randY < sprite_y(&upBorder) + (sprite_height(allObjects[i]) + 8)){
							randY += sprite_height(allObjects[i]);
						}
					}
				} 
			}
			
			sprite_move_to(allObjects[i], randX, randY);
		}
	}
	
	sprite_move_to(&upBorder, sprite_x(&leftBorder), sprite_y(&leftBorder));
	sprite_move_to(&wall1, sprite_x(&leftBorder) + 8, sprite_y(&wall1));
	sprite_move_to(&wall6, sprite_x(&wall1), sprite_y(&wall1));
	if (spriteCollided(&door, &wall1) || spriteCollided(&door, &wall6)) {
		sprite_move_to(&door, sprite_x(&wall1) + 8, sprite_y(&wall6) + 8);
	}
	
}

void clearWeapon() { 
	if (weapon != NULL) {
		int randX = rand() % ((TRUE_LCD_X - (2*sprite_width(weapon)) / 2));
		int randY = rand() % ((TRUE_LCD_Y - (2*sprite_height(weapon)) / 2));
			
		if (sprite_x(weapon) >= sprite_x(&rightBorder)/2) {
			randX = -randX;
		}
			
		if (sprite_y(weapon) >= sprite_y(&downBorder)/2) {
			randY = -randY;
		}
				
		sprite_move_to(weapon, sprite_x(weapon) + randX, sprite_y(weapon) + randY); 
	}
	
	weapon = NULL;
}

void generalLevelSetup(){ 
	pos = 13;
	
	keyObtained = false;
	bombHasBeenPlanted = false;
	arrowShot = false;
	
	if (weapon != NULL) {
		clearWeapon();
	}
	
	for (int i = 0; i < 9; i++) {
		treasureCollected[i] = -1;
		enemies[1 + i] = NULL;
		treasure[i] = NULL;
	}
	
	for (int i = 0; i < 20; i++) {
		allObjects[pos + i] = NULL;
	}
	
	sprite_show(&monster1);
	sprite_show(&monster2);
	sprite_show(&monster3);
	sprite_show(&monster4);
	sprite_show(&monster5);
		
	if (level == 2) {
		allSpriteCounter--; 
		solidSpriteCounter--; 
		allObjects[6] = &wall1;
		allObjects[7] = &wall2;
		allObjects[8] = &wall3;
		allObjects[9] = &wall4;
		allObjects[10] = &wall5;
		allObjects[11] = &wall6;
		allObjects[12] = &monster1;
		
		solidObjects[1] = &wall1;
		solidObjects[2] = &wall2;
		solidObjects[3] = &wall3;
		solidObjects[4] = &wall4;
		solidObjects[5] = &wall5;
		solidObjects[6] = &wall6;

	} 
		
	int monsterAdd = rand() % 5;
	
	if (monsterAdd >= 1){
		enemies[1] = &monster2;
		allObjects[pos] = &monster2;
		pos++;
		if (monsterAdd >= 2){
			enemies[2] = &monster3;
			allObjects[pos] = &monster3;
			pos++;
			if (monsterAdd >= 3) {
				enemies[3] = &monster4;
				allObjects[pos] = &monster4;
				pos++;
				if (monsterAdd >= 4) {
					enemies[4] = &monster5;
					allObjects[pos] = &monster5;
					pos++;
				}
			}
		}
	}
	
	int treasureAdd = rand() % 6;
	
	if (treasureAdd >= 1){
		sprite_show(&treasure1);
		treasure[0] = &treasure1;
		allObjects[pos] = &treasure1;
		pos++;
		if (treasureAdd >= 2){
			sprite_show(&treasure2);
			treasure[1] = &treasure2;
			allObjects[pos] = &treasure2;
			pos++;
			if (treasureAdd >= 3){
				sprite_show(&treasure3);
				treasure[2] = &treasure3;
				allObjects[pos] = &treasure3;
				pos++;
				if (treasureAdd >= 4){
					sprite_show(&treasure4);
					treasure[3] = &treasure4;
					allObjects[pos] = &treasure4;
					pos++;
					if (treasureAdd >= 5){
						sprite_show(&treasure5);
						treasure[4] = &treasure5;
						allObjects[pos] = &treasure5;
						pos++;
					}
				}
			}
		}
	} 
	
	if (rand() % 10 <= 2) {
		allObjects[pos] = &shield;
		pos++;
		sprite_show(&shield);	
	}
	
	if (rand() % 10 <= 2) {
		allObjects[pos] = &bomb;
		pos++;
		sprite_show(&bomb);
	}

	if (rand() % 10 <= 2) {
		allObjects[pos] = &bow;
		pos++;
		sprite_show(&bow);
		sprite_hide(&arrow);
  }
	
	allSpriteCounter = pos; 
	solidSpriteCounter = 7;
	enemySpriteCounter = monsterAdd + 1; 
	treasureSpriteCounter = treasureAdd;
	
	allObjects[pos] = &arrow;
	regenerate();	
	solidObjects[7] = &player;
	solidSpriteCounter++;
	regenerate2();
	solidObjects[7] = NULL;
	solidSpriteCounter--;
}

void calculateAim() {	
	potentiometer1 = sin((potentiometer * M_PI) / 256); 
	potentiometer2 = -cos((potentiometer * M_PI) / 256); 
}

void drawCrosshair() {
	calculateAim();
	draw_line(sprite_x(&player) + 3, sprite_y(&player) + 3, sprite_x(&player) + 3 + (potentiometer1*7), sprite_y(&player) + 3 + (potentiometer2*7), FG_COLOUR);
}	

double get_current_time() { 
	return (overflow_count * 256.0 + TCNT0) * (PRESCALE  / FREQ);
}

void terminalUpdate() { 
	usb_serial_formatted("(%.3f) ", (float) get_current_time()); 
	usb_serial_formatted("Score: %d ", score);
	usb_serial_formatted("Lives: %d ", lives);
	usb_serial_formatted("Level: %d ", level);
	usb_serial_formatted("Player is at %.f, %.f\r\n", (float)((TRUE_LCD_X - 86) - sprite_x(&leftBorder)), (float) ((TRUE_LCD_Y - 47)  - sprite_y(&upBorder)));
}

void keyAnimate() {
	// Moves up.
	if (BIT_IS_SET(PIND, 1)) {
		sprite_move_to(&key, sprite_x(&player), sprite_y(&player) + 8);
	}
	
	// Moves down.
	if (BIT_IS_SET(PINB, 7)) {
		sprite_move_to(&key, sprite_x(&player), sprite_y(&player) - 5);
	}
		
	// Moves left.
	if (BIT_IS_SET(PINB, 1)) {
		sprite_move_to(&key, sprite_x(&player) + 8, sprite_y(&player));
	}
	
	// Moves right.
	if (BIT_IS_SET(PIND, 0)) {
		sprite_move_to(&key, sprite_x(&player) - 8, sprite_y(&player));
	}
}

void weaponAnimate() {
	if (weapon != NULL) {
		sprite_show(weapon);
		if (BIT_IS_SET(PIND, 1)) {
			sprite_move_to(weapon, sprite_x(&player), sprite_y(&player) + 9);
		}
		
		if (BIT_IS_SET(PINB, 7)) {
			sprite_move_to(weapon, sprite_x(&player), sprite_y(&player) - 6);
		}
		
		if (BIT_IS_SET(PINB, 1)) {
			sprite_move_to(weapon, sprite_x(&player) + 9, sprite_y(&player));
		}
		
		if (BIT_IS_SET(PIND, 0)) {
			sprite_move_to(weapon, sprite_x(&player) - 9, sprite_y(&player));
		}
	}
} 

bool playerCollided(sprite_id sprite) {
	if (sprite_x(&player) <= (sprite_x(sprite) + (sprite_width(sprite) - 1)) 
	&& sprite_x(&player) + 5 >= (sprite_x(sprite))
	&& sprite_y(&player) <= (sprite_y(sprite) + (sprite_height(sprite) - 1)) 
	&& sprite_y(&player) + 5 >= (sprite_y(sprite))
	) {
		return true;
	}
	
	else {
		return false;
	}
}

void playerBlocked(sprite_id sprite) { 
	
	if (sprite_x(&player) + 5 < sprite_x(sprite)
	&& sprite_x(&player) + 5 >= sprite_x(sprite) - 1	
	&& sprite_y(&player) + 5 > sprite_y(sprite) - 1
	&& sprite_y(&player) < sprite_y(sprite) + sprite_height(sprite) 
	){ 
		rightPressed = false;
	}
	
	if (sprite_x(&player) < sprite_x(sprite) + sprite_width(sprite) + 1
    && sprite_x(&player) >= sprite_x(sprite) + sprite_width(sprite) 
	&& sprite_y(&player) + 5 > sprite_y(sprite) - 1
	&& sprite_y(&player) < sprite_y(sprite) + sprite_height(sprite)
    ){
		leftPressed = false;
	}
	
	if (sprite_y(&player) < sprite_y(sprite) + sprite_height(sprite) + 1
    && sprite_y(&player) >= sprite_y(sprite) + sprite_height(sprite)
	&& sprite_x(&player) + 5 > sprite_x(sprite) - 1
	&& sprite_x(&player) < sprite_x(sprite) + sprite_width(sprite)
    ){
		upPressed = false;
	} 
	
	if (sprite_y(&player) + 5 < sprite_y(sprite)
    && sprite_y(&player) + 5 >= sprite_y(sprite) - 1
	&& sprite_x(&player) + 5 > sprite_x(sprite) - 1
	&& sprite_x(&player) <	sprite_x(sprite) + sprite_width(sprite)
    ){
		downPressed = false;
	}
}

void monsterBlocked(sprite_id monster, sprite_id sprite) {
	
	if (sprite_x(monster) + 4 < sprite_x(sprite)
	&& sprite_x(monster) + 4 >= sprite_x(sprite) - 1	
	&& sprite_y(monster) + 4 > sprite_y(sprite) - 1
	&& sprite_y(monster) < sprite_y(sprite) + sprite_height(sprite)
	){ 
		sprite_move_to(monster, sprite_x(sprite) - 5, sprite_y(monster));
		rightMove = false;
	}
	
	if (sprite_x(monster) < sprite_x(sprite) + sprite_width(sprite) + 1
    && sprite_x(monster) >= sprite_x(sprite) + sprite_width(sprite) 
	&& sprite_y(monster) + 4 > sprite_y(sprite) - 1
	&& sprite_y(monster) < sprite_y(sprite) + sprite_height(sprite)
    ){
		sprite_move_to(monster, sprite_x(sprite) + sprite_width(sprite) + 1, sprite_y(monster));
		leftMove = false;
	}
	
	if (sprite_y(monster) < sprite_y(sprite) + sprite_height(sprite) + 1
    && sprite_y(monster) >= sprite_y(sprite) + sprite_height(sprite)
	&& sprite_x(monster) + 4 > sprite_x(sprite) - 1
	&& sprite_x(monster) < sprite_x(sprite) + sprite_width(sprite)
    ){
		sprite_move_to(monster, sprite_x(monster), sprite_y(sprite) + sprite_height(sprite) + 1);
		upMove = false;
	}
	
	if (sprite_y(monster) + 4 < sprite_y(sprite)
    && sprite_y(monster) + 4 >= sprite_y(sprite) - 1
	&& sprite_x(monster) + 4 > sprite_x(sprite) - 1
	&& sprite_x(monster) <	sprite_x(sprite) + sprite_width(sprite)
    ){
		sprite_move_to(monster, sprite_x(monster), sprite_y(sprite) - 5);
		downMove = false;
	}
}

ISR(TIMER0_OVF_vect) { 
	overflow_count++;
	if (overflow_count % 15 == 0) {
		overflowed = true;
	}
}
	
void playerControls(){
	// Moves up.
	if (BIT_IS_SET(PIND, 1) && sprite_y(&player) > sprite_y(&upBorder) + 1) {
		upPressed = true;
	}
	
	// Moves down.
	if (BIT_IS_SET(PINB, 7) && sprite_y(&player) + 5 < (sprite_y(&downBorder) - 1)) {
		downPressed = true;
	}
		
	// Moves left.
	if (BIT_IS_SET(PINB, 1) && sprite_x(&player) > sprite_x(&leftBorder) + 1) {
		leftPressed = true;
	}
	
	// Moves right.
	if (BIT_IS_SET(PIND, 0) && sprite_x(&player) + 5 < (sprite_x(&rightBorder) - 1)) {
		rightPressed = true;
	}
}

void stopMoving() {
	upPressed = false;
	downPressed = false;
	leftPressed = false;
	rightPressed = false;
}

void movePlayer() {
	for (int i = 0; i < allSpriteCounter; i++) {
		if (upPressed) {
			sprite_move(allObjects[i], 0, 1);
		}
		
		else if (downPressed) {
			sprite_move(allObjects[i], 0, -1);
		}

		else if (leftPressed) {
			sprite_move(allObjects[i], 1, 0);
		}
		
		else if (rightPressed) {
			sprite_move(allObjects[i], -1, 0);
		}
		
	}
	
	stopMoving();
}

void loading() {
	clear_screen();
	draw_string(10, 10, "Loading...", FG_COLOUR);
	draw_formatted(10, 20, "Next Floor: %d", level);
	draw_formatted(10, 30, "Score: %d", score);
	show_screen();
	for (int i = 0; i < 4; i++) {
		terminalUpdate();
		_delay_ms(500);
	} 
}

void monsterUpdate(sprite_id monster) {
	bool horizontal;
	bool vertical;
	double dx = sprite_x(&player) - sprite_x(monster);
    double dy = sprite_y(&player) - sprite_y(monster);
		
    double dist = sqrt(dx*dx + dy*dy);
	
	leftMove = true;
	rightMove = true;
	upMove = true;
	downMove = true;
	
    dx = dx/dist;
	dy = dy/dist;
	
	for (int i = 0; i < solidSpriteCounter; i++ ) {
		monsterBlocked(monster, solidObjects[i]);
	}
    
	horizontal = (!rightMove && dx > 0) || (!leftMove && dx < 0);
	vertical = (!upMove && dy < 0) || (!downMove && dy > 0);
	
	if (horizontal && vertical) {
		dx = 0;
		dy = 0;
	}
	
	else if (horizontal && !vertical) {
		dx = 0;
		dy *= 0.2;
	}
	
	else if (!horizontal && vertical) {
		dy = 0;
		dx *= 0.2;
	}
	
	else {
		dx *= 0.1;
		dy *= 0.1;
	}
    
	sprite_turn_to(monster, dx, dy);
	sprite_step(monster);
	
	leftMove = false;
	rightMove = false;
	upMove = false;
	downMove = false;
}

void lifeLost(){ 
	usb_serial_formatted("Player was killed by a monster!!\r\n");
		
	lives--;
	if (lives == 0) {
		finalFloor = level;
		level = -1;
	}
	
	if (keyObtained) { 
		keyObtained = false;
		sprite_move_to(&key, sprite_x(&player), sprite_y(&player));
		sprite_show(&key);
	}
	
	if (weapon != NULL) {
		sprite_move_to(weapon, sprite_x(&player), sprite_y(&player));
		weapon = NULL;
	}
	
	regenerate();
	
}

void levelUpdate() {
	usb_serial_formatted("Player used key to get to the next level!!\r\n");
	score += 100;
	level++;
	loading();
	generalLevelSetup();
}

void gameOver(){
	clear_screen();
    draw_string(20, 0, "GAME OVER", FG_COLOUR);
    draw_formatted(7, 10, "Score: %d", score, FG_COLOUR);
    draw_formatted(7, 20, "Last Floor: %d", finalFloor, FG_COLOUR);
	draw_string(0, 30, "Press SW2 or SW3", FG_COLOUR);	
	
	if (BIT_IS_SET(PINF, 5) || BIT_IS_SET(PINF, 6)){
		level = 0;
		setup();
		show_screen();
		_delay_ms(500);
	} 		
}

void menu(){
	
	time2 = ((overflow_count) - gameTimeStart) / 30.0;
	time1 = 0;
	
	if (time2 > 59) {
		int x = time2 / 60;
		time1 += x;
		time2 -= 60*x;
	} 
	
	clear_screen();
	draw_formatted(30, 0, "Paused", FG_COLOUR); 
	draw_formatted(10, 10, "Time: %02d:%02d", time1, (int) time2);
	draw_formatted(10, 20, "Lives: %d", lives);
	draw_formatted(10, 30, "Floor: %d", level);
	draw_formatted(10, 40, "Score: %d", score);
}

void countdown() { 
	clear_screen();
	draw_string(LCD_X/2, LCD_Y/2, "3", FG_COLOUR);
	show_screen();
	_delay_ms(333);
	draw_string(LCD_X/2, LCD_Y/2, "2", FG_COLOUR);
	show_screen();	
	_delay_ms(333);
	draw_string(LCD_X/2, LCD_Y/2, "1", FG_COLOUR);
	show_screen();	
	_delay_ms(333);
	level++;
	gameTimeStart = overflow_count; 
}

void groundLevel(){
	clear_screen();
	
	playerControls();
	
	if (sprite_y(&monster1) + 5 >= sprite_y(&upSide)
	&& sprite_y(&monster1) <= (sprite_y(&downSide))
	&& sprite_x(&monster1) + 5 >= sprite_x(&leftSide)
	&& sprite_x(&monster1) <= (sprite_x(&rightSide))) {
		monsterUpdate(&monster1);
	}
	
	if (playerCollided(&key)){ 
		if (!keyObtained) {
			usb_serial_formatted("Player obtained key!!\r\n");
		}
		keyObtained = true;
	}
		
	if (keyObtained) {
		keyAnimate();
		if (playerCollided(&door)) {
			levelUpdate();
		}
	}

	else {
		playerBlocked(&door);
	}
	
	if (playerCollided(&monster1)){
		lifeLost();	
	}
	
	playerBlocked(&castle);
				
	sprite_draw(&castle);
	sprite_draw(&door);
	sprite_draw(&player);
	sprite_draw(&monster1);
	sprite_draw(&key);
	sprite_draw(&leftBorder);
	sprite_draw(&rightBorder);
	sprite_draw(&upBorder);
	sprite_draw(&downBorder); 
	
	movePlayer();
}

void generalLevel() { 
	clear_screen();
	
	playerControls();
	
	potentiometer = adc_read(1);
	
	for (int i = 0; i < enemySpriteCounter; i++) {
		if (sprite_y(enemies[i]) + 5 >= sprite_y(&upSide)
		&& sprite_y(enemies[i]) <= (sprite_y(&downSide))
		&& sprite_x(enemies[i]) + 5 >= sprite_x(&leftSide)
		&& sprite_x(enemies[i]) <= (sprite_x(&rightSide))) {
			monsterUpdate(enemies[i]);
		} 
		
		if (playerCollided(enemies[i]) && sprite_visible(enemies[i])){
			if (weapon == &shield) {
				usb_serial_formatted("Player used shield!!\r\n");
				usb_serial_formatted("Player killed a monster!!\r\n");
				clearWeapon();
				sprite_hide(&shield);
				sprite_move(&shield, sprite_x(&leftBorder) - 10, sprite_y(&upBorder) - 10);
				sprite_hide(enemies[i]);
			}
			
			else {
				lifeLost();	
			}
		} 
	} 
	
	if (playerCollided(&key)){ 
		if (!keyObtained) {
			usb_serial_formatted("Player obtained key!!\r\n");
		}
		keyObtained = true;
	}
	
	if (keyObtained) {
		keyAnimate();
		if (playerCollided(&door)) {
			levelUpdate();
		}
	}
	
	if (playerCollided(&shield)) {
		if (weapon != &shield) {
			clearWeapon();
			usb_serial_formatted("Player obtained shield!!\r\n");
		}
		weapon = &shield;
	}
		
	if (playerCollided(&bomb) && !bombHasBeenPlanted) {
		if (weapon != &bomb) {
			clearWeapon();
			usb_serial_formatted("Player obtained bomb!!\r\n");
		}
		weapon = &bomb;
	}
	
	if (playerCollided(&bow)) {
		if (weapon != &bow) {
			clearWeapon();
			usb_serial_formatted("Player obtained bow!!\r\n");
		}
		weapon = &bow;
		arrows = 5;
	}

	if (weapon != NULL && !bombHasBeenPlanted) {
		weaponAnimate();
	}
	
	if (weapon == &bow && !arrowShot) {
		sprite_move_to(&arrow, sprite_x(&player), sprite_y(&player));
	}
	
	if (weapon == &bomb || weapon == &bow) {
		drawCrosshair();
	} 
	
	if (BIT_IS_SET(PINF, 5) || BIT_IS_SET(PINF, 6)) { 
		bool bombWithinLimits = true;
		if (weapon == &bomb && !bombHasBeenPlanted) { 
			sprite_turn_to(&bomb, potentiometer1, potentiometer2);
			sprite_move_to(&bomb, sprite_x(&player) + potentiometer1*6, sprite_y(&player) + potentiometer2*6);		
			if (sprite_y(&bomb) > sprite_y(&upBorder)
			&& sprite_y(&bomb) + 5 <= (sprite_y(&downBorder))
			&& sprite_x(&bomb) >= sprite_x(&leftBorder)
			&& sprite_x(&bomb) + 3 <= (sprite_x(&rightBorder))) {
				for (int i = 0; i < solidSpriteCounter; i++){
					if (spriteCollided(solidObjects[i], &bomb)){
						bombWithinLimits = false;
					}
				}
				
			}			
			else {
				bombWithinLimits = false;
			}
						
			if (bombWithinLimits) {					
				usb_serial_formatted("Player used bomb!!\r\n");
				sprite_show(&bomb);
				bombHasBeenPlanted = true;
				bombTime = overflow_count;
			}
			else {
				sprite_move_to(&bomb, sprite_x(&player), sprite_y(&player));
			}
		}
		
		if (weapon == &bow && !arrowShot) {
			usb_serial_formatted("Player used bow!!\r\n");
			arrowShot = true;
			sprite_show(&arrow);
		}
	}
	
	if (bombHasBeenPlanted && (overflow_count - bombTime >= 45 && overflow_count - bombTime <= 46)) { 
		SET_BIT(PORTB, 2);
		SET_BIT(PORTB, 3);
		for (int i = enemySpriteCounter - 1; i > -1; i--) {
			if (sprite_y(enemies[i]) + 5 >= sprite_y(&upSide)
			&& sprite_y(enemies[i]) <= (sprite_y(&downSide))
			&& sprite_x(enemies[i]) + 5 >= sprite_x(&leftSide)
			&& sprite_x(enemies[i]) <= (sprite_x(&rightSide))
			&& sprite_visible(enemies[i])) {
				usb_serial_formatted("Player killed a monster!!\r\n");
				score += 10;
				sprite_hide(enemies[i]);
				
			}
		}
		bombHasBeenPlanted = false;
		clearWeapon();
		sprite_hide(&bomb);
		sprite_move(&bomb, sprite_x(&leftBorder) - 10, sprite_y(&upBorder) - 10);
	}
	
	if (overflow_count - bombTime >= 60) {
		CLEAR_BIT(PORTB, 2);
		CLEAR_BIT(PORTB, 3);
	}
	
	if (arrowShot) { 
			
		for (int i = 0; i < solidSpriteCounter; i++) {
			if (spriteCollided(&arrow, solidObjects[i])){
				arrowShot = false;
				sprite_move_to(&arrow, sprite_x(&player), sprite_y(&player));
				sprite_hide(&arrow);
				arrows--;
				allSpriteCounter = pos;
			}
		}
		
		for (int i = 0; i < enemySpriteCounter; i++) {
			if (spriteCollided(&arrow, enemies[i]) && sprite_visible(enemies[i])){
				usb_serial_formatted("Player killed a monster!!\r\n");
				arrowShot = false;
				score += 10;
				sprite_move_to(&arrow, sprite_x(&player), sprite_y(&player));
				sprite_hide(&arrow);
				sprite_hide(enemies[i]);
				arrows--;
				allSpriteCounter = pos;
			}
		}
		
		if (spriteCollided(&arrow, &upBorder)
		|| spriteCollided(&arrow, &downBorder)
		|| spriteCollided(&arrow, &leftBorder)
		|| spriteCollided(&arrow, &rightBorder)
		) {
			arrowShot = false;
			sprite_move_to(&arrow, sprite_x(&player), sprite_y(&player));
			sprite_hide(&arrow);
			arrows--;
			allSpriteCounter = pos;
		}
		
		if (arrows <= 0) {
			clearWeapon();
			sprite_move(&bow, sprite_x(&leftBorder) - 10, sprite_y(&upBorder) - 10);
			sprite_hide(&bow);
		}
		
		else {
			calculateAim(); 
			sprite_turn_to(&arrow, potentiometer1, potentiometer2);
			allSpriteCounter = pos + 1;
			sprite_step(&arrow); 
			
		}
		
	}
	
	for (int i = 0; i < solidSpriteCounter; i++) {
		if (solidObjects[i] != &door || !keyObtained) {
			playerBlocked(solidObjects[i]);
		}
	}
	
	bool treasureTaken;
	for (int i = 0; i < treasureSpriteCounter; i++) {
		if (i == treasureCollected[i]) {
			treasureTaken = true;
		}
		
		else {
			treasureTaken = false;
		}
				
		if (playerCollided(treasure[i]) && !treasureTaken) {
			score += 10;
			sprite_hide(treasure[i]);
			treasureCollected[i] = i;
		}
	}

	for (int i = 0; i < allSpriteCounter; i++) {
		sprite_draw(allObjects[i]);
	}
	
	sprite_draw(&player);
	sprite_draw(&arrow);
	
	movePlayer();
}

int main(void) { 
	set_clock_speed(CPU_8MHz);
	
	SET_BIT(DDRB, 2); // LED0.
    SET_BIT(DDRB, 3); // LED1.
	
	for (int i = 0; i <= 7; i++){
    CLEAR_BIT(PORTF, i); 
    }
	
	CLEAR_BIT(DDRF, 5); // An SW button.
	CLEAR_BIT(DDRF, 6); // An SW button.
	CLEAR_BIT(DDRB, 1); // Joystick left.
	CLEAR_BIT(DDRB, 0); // Joystick center.
	CLEAR_BIT(DDRB, 7); // Joystick down.
	CLEAR_BIT(DDRD, 1); // Joystick up.
	CLEAR_BIT(DDRD, 0); // Joystick right.
	
	TCCR0A = 0;
	TCCR0B = 5; 
	TIMSK0 = 1; 
	sei();
	
	lcd_init(LCD_DEFAULT_CONTRAST);
	
	usb_init();
	adc_init();

	draw_string(0, 0, "CONNECT TO", FG_COLOUR);
    draw_string(0, 10, "TERMINAL", FG_COLOUR);
	draw_string(0, 20, "Then Press", FG_COLOUR);
	draw_string(0, 30, "Enter", FG_COLOUR);
	show_screen();
	
	// IMPORTANT: FOR ANYONE TESTING THIS THAT DOESN'T CARE ABOUT TERMINAL UPDATES
	// COMMENT OUT THE SECTION OF CODE BELOW
	while (1) {
		overflow_count = 0;
		if (usb_serial_getchar() >= 0) {
			clear_screen();
			usb_serial_formatted("Welcome - The game should be ready to play now...\r\n");
			break;
		}
		randomSeed++;
	} 
	// CODE STOPS HERE
	
	
	srand (randomSeed); 
	
	setup();

    for ( ;; ) {
		if ((level == 0) && (BIT_IS_SET(PINF, 5) || BIT_IS_SET(PINF, 6))){
			countdown();
	    } 		
		
		else if (level == 1) {
			if (BIT_IS_SET(PINB, 0)){
				menu();
			}
			
			else {
				groundLevel();
			}
			
			if (overflowed) {
				terminalUpdate();
				overflowed = false;
			}	
		}
		
		else if (level > 1) {
			if (BIT_IS_SET(PINB, 0)){
				menu();
			}
			
			else {
				generalLevel();
			}
			if (overflowed) {
				terminalUpdate();
				overflowed = false;
			}	
		}
		
		else if (level == -1) {
			gameOver();
		}
		
		show_screen();
	    _delay_ms(10);
	}

	return 0;
}