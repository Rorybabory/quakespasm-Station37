#ifndef _QUAKE_UI_H
#define _QUAKE_UI_H

#define TUTORIAL_GRENADE 1
#define TUTORIAL_RAILGUN 2

#include "quakedef.h"
#include <direct.h>
#include <stdio.h>
#include "inventory.h"
#include "stb_image.h"
 
typedef struct {
	unsigned int gltexture;
} qpng_t;
typedef struct {
	int x;
	int y;
} qboxpos;
typedef struct {
	char* text;
	char* name;
} qnote_entry_t;



qboolean inv_useClicked;
unsigned int inv_tile;
unsigned int inv_tileHighlight;
unsigned int inv_tileHighlight32;
unsigned int inv_tile32;
unsigned int inv_paper;
unsigned int inv_tablet;
int ui_mouse_x;
int ui_mouse_y;
unsigned int ui_keypad;
unsigned int ui_sevensegment;
unsigned int ui_ammotext;
unsigned int keypad_id;
unsigned int inv_use;
int keypadVal[4];
int ui_keycode[4];
qpic_t* inv_header;
qpic_t* inv_x;
qpic_t* inv_testItem;

unsigned int inv_supershotgun;
unsigned int inv_shotgun;
unsigned int inv_pistol;
unsigned int inv_pistolbroken;
unsigned int inv_rifle;
unsigned int inv_grenade;
unsigned int inv_soda;
unsigned int inv_healthkit;
unsigned int inv_hammer;
unsigned int inv_powersource;
unsigned int inv_railgun;
unsigned int inv_railgun;
unsigned int inv_knife;
unsigned int inv_watergun;
unsigned int inv_grenadelauncher;
unsigned int inv_laserhammer;
unsigned int inv_chip1;
unsigned int inv_revolver;
unsigned int ui_healthseg;
unsigned int ui_healthseg2;
unsigned int ui_healthseg3;
unsigned int ui_healthbar;
unsigned int ui_health2;
unsigned int ui_healthbar2;
unsigned int ui_shader;


unsigned int ui_pickitem;
unsigned int ui_cash;
unsigned int ui_cashnums;
unsigned int ui_compscreen;
unsigned int ui_comptext;
unsigned int ui_dialogbox;
unsigned int ui_description;
char* dialogbox_text;
int showdialog;
float dialogcounter;
int lastdialogcounter;
char lastdialogchar;

qpic_t* ui_reticle_back;

unsigned int ui_reticle_bottom;
unsigned int ui_reticle_left;
unsigned int ui_reticle_right;
unsigned int ui_reticle_top;



unsigned int pngTest;

float ui_deltatime;
float ui_lasttime;

int tutorialflags;
int lasttutorialflags;

float ui_reticle_size;
float ui_reticle_base;
float ui_reticle_min;
int ui_movechanged;
float ui_clickPressed;
float ui_scale;
int ui_textscroll;
int ui_scrollcount;
char* lastItem;

int drawbosshealth;
int bosshealth;

qboolean ui_selectItem;
qboolean ui_selectClicked;
qboolean ui_moving;
//int ui_mouse_val_x = 0;

qboolean ui_canclick; 
qboolean ui_keypadOpen;
qboolean ui_computeropen;
qboolean ui_keypadSuccess;

edict_t* ui_activekeypad;

qboolean ui_drawnote;
unsigned int ui_font;
unsigned int notes_handwritten;
unsigned int notes_paper;

char* paper_text;
char* notes_text;
char* notes_target;
int notes_chari;
int notes_char_count;
float notes_count;

char comp_type[50];

char comp_screen[50 * 35];

char* ui_active_computer;

int ui_compmode;



qnote_entry_t* notes_precached;

unsigned int UI_LoadTexture(char * name);
void UI_Draw_Pic(int x, int y, int width, int height, unsigned int pic);
void UI_Keydown(int key);
void UI_Draw(void);
void UI_Draw_Text(int x, int y, int size, const char* str);
void UI_Init(void);
void UI_LoadPics(void);
void UI_EnableKeypad(int i0, int i1, int i2, int i3, int success, edict_t * ent);
void UI_DisableKeypad();
void UI_PlayKeypadBeep();
void UI_DrawKeypad();
void UI_UpdateKeypad(edict_t* entity);
char* UI_NotesLoad(char* filename);
void UI_Draw_Text(int x, int y, int size, const char* str);
void UI_Draw_OverlayText(int x, int y, int size, char* str);
void UI_Draw_Cash_Nums(int x, int y, int size, int num);
void UI_Free();
void UI_Draw_Console();
void UI_ConsoleInput(int key);


void UI_InitNotes();
void UI_Draw_Note();


#endif 