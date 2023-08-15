#include "ui.h"

#include <time.h>
int UI_CheckUIBox(int mouse_x, int mouse_y, int box_x, int box_y, int box_width, int box_height);
qboxpos UI_InitBox(int x, int y);
void UI_DrawSevenSegment(int x, int y, int val);
void UI_DrawAmmo(int x, int y, int val);
void UI_UpdateOverlayText();
qboolean UI_mouseClicked();
void Draw_CharacterQuadScale(int x, int y, int scale, char num);
void UI_DrawRectangle(int x, int y, int w, int h, float r, float g, float b, float a);
unsigned int getDrawItem(char* name);
void blendDurColors(float x, float* r, float* g);
void reticleLogic();
void UI_Draw_HBar(int x, int y, int width, int height, unsigned int pic, float health);

void Draw_CharacterScale2(int x, int y, int scale, int num) {
	num &= 255;
	if (num == 32)
		return;
	glBindTexture(GL_TEXTURE_2D, ui_comptext);
	glBegin(GL_QUADS);
	Draw_CharacterQuadScale(x, y, scale, (char)num);
	glEnd();
}

void UI_Draw(void) {
	int offsetX = 0;
	int offsetY = 0;
	
	extern vrect_t scr_vrect;
	float scale = (scr_vrect.width/1920.0f)*128.0f;
	ui_scale = scale / 16.0f;
	SDL_GetMouseState(&ui_mouse_x, &ui_mouse_y);
	GL_SetCanvas(CANVAS_UI);

	lastItem = inv_slots[inv_highlight].name;
	inv_lasthighlight2 = inv_highlight;

	if (cls.demoplayback) {
		return;
	}

	//hello

	/**/
	
	//char deltaTime[6];
	float t = (float)Sys_DoubleTime();

	ui_deltatime = t - ui_lasttime;

	ui_lasttime = t;
	//SV_BroadcastPrintf("%f\n", ui_deltatime);

	//Draw_StringScale(0, 0, 16, deltaTime);
	float width = scr_vrect.width * ui_reticle_size;
	float height = scr_vrect.width * ui_reticle_size;


	//reticleLogic();

	//printf("reticle size: %f\n", ui_reticle_size);

	//UI_Draw_Pic(scr_vrect.width / 2 - 22, scr_vrect.height / 2 - 22 + (ui_reticle_size*640)+16*3, 16 * 3, 16 * 3, ui_reticle_bottom);
	Draw_Pic_Scale(scr_vrect.width / 2.0-(width/2.) + 4.0, scr_vrect.height / 2.0 - (height / 2.) + 4.0, width, height, ui_reticle_back);

	GL_Bind(0);

	if (inventoryOpen == 1) {
		UI_Draw_Pic(scale*3, 0, scale*4, scale*2, ui_description);
		char* str = Inventory_GetItemText(inv_slots[inv_highlight].name);
		int x = 0;
		int y = 0;
		
		if (ui_textscroll < strlen(str)-1) {
			ui_textscroll++;
		}

		for (int i = 0; i < strlen(str); i++) {
			if (i > ui_textscroll) {
				break;
			}
			Draw_CharacterScale2(((scale / 2.0f) * 3 + (scale / 8.0)) * 2 + x * (scale/8.0f), (scale/4.0) + (scale/8.0) * y, scale/8.0, str[i]);
			x++;
			if (x > 27) {
				x = 0;
				y++;
			}
			if (str[i] == ' ') {
				int i2 = i + 1;
				int x2 = x + 1;
				while (str[i2] != ' ' && str[i2] != '.') {
					x2++;
					i2++;
					if (x2 > 27) {
						x = 0;
						y++;
						break;
					}
				}
			}
		}


		for (int x = 0; x < inv_numslots; x++) {
			UI_Draw_Pic(offsetX + (x%3 * scale), offsetY + (x/3 * scale), scale, scale, inv_tile);
			if (x == inv_highlight) {
				UI_Draw_Pic(offsetX + (x%3 * scale), offsetY + (x/3 * scale), scale, scale, inv_tileHighlight);
			}
			unsigned int drawnTile;
			drawnTile = getDrawItem(inv_slots[x].name);

			if (drawnTile != 0) {
				UI_Draw_Pic(offsetX + (x%3 * scale), offsetY + (x/3 * scale), scale, scale, drawnTile);
			}

			if (inv_slots[x].number > 1) {
				char numStr[6];
				sprintf(numStr, "%d", inv_slots[x].number);
				UI_Draw_Text(offsetX + (x%3 * scale), offsetY + (x/3 * scale) + (scale - 32), 32, &numStr);
			}
			if (ui_clickPressed == 1.0f) {
				if (UI_CheckUIBox(ui_mouse_x, ui_mouse_y, x%3 * 128, x/3 * 128, 128, 128) == 1) {
					inv_highlight = x;
					ui_canclick = false;
					if (strcmp(heldItem.name, "") == 0) {
						heldItem = inv_slots[x];
						strcpy(inv_slots[x].name, "");
						inv_slots[x].number = 0;
					}
				}
			}
			else {
				ui_canclick = true;
			}

		}
		
		
		//printf("%s\n", inv_slots[inv_highlight_x][inv_highlight_y].name);
		if (ui_clickPressed == 1.0f) {
			if (ui_canclick == true) {
				if (UI_CheckUIBox(ui_mouse_x, ui_mouse_y, 128 * 3, 256, 8 * 32, 8 * 16) == 1) {
					printf("clicked use\n");
					ui_canclick = false;
					inv_useClicked = true;
					printf("clicked on item\n");

				}
				if (ui_selectItem == true) {
					if (UI_CheckUIBox(ui_mouse_x, ui_mouse_y, 0, 3 * scale, 128 * 8 / 2, 32 * 8 / 2) == 1) {
						ui_selectClicked = true;
					}
				}
			}
		}
		else {
			ui_canclick = true;
		}
		int cashPosX = 0;


		if (ui_selectItem == true) {
			UI_Draw_Pic(0, ((inv_numslots / 3) + (inv_numslots % 3)) * scale, 128 * 8 / 2, 32 * 8 / 2, ui_pickitem);
			cashPosX += 128 * 8 / 2;
		}
		else {
			UI_Draw_Pic(cashPosX, ((inv_numslots / 3) + (inv_numslots % 3)) * scale, 64 * 3, 16 * 3, ui_cash);
			UI_Draw_Cash_Nums(cashPosX + 28 * 3, ((inv_numslots / 3) + (inv_numslots % 3)) * scale + 4 * 3, 3, inv_cash);

			if (inv_slots[inv_highlight].id == 0 &&
				strcmp(inv_slots[inv_highlight].name, "") != 0) {
				UI_Draw_Pic(128 * 3, 256, 8 * 32, 8 * 16, inv_use);
			}
		}
		



		if (ui_clickPressed == 0.0f) {
			if (strcmp(heldItem.name, "") != 0) {
				int hTileX = ui_mouse_x / 128;
				int hTileY = ui_mouse_y / 128;
				int hTile = hTileY * 3 + hTileX;
				if (hTile >= 0 && hTile < inv_numslots) {
					if (strcmp(inv_slots[hTileY*3+hTileX].name, "") == 0) {
						inv_slots[hTileY * 3 + hTileX] = heldItem;
						strcpy(heldItem.name, "");
						inv_highlight = hTileY*3+hTileX;
					}
				}
			}
		}
		if (strcmp(heldItem.name, "") != 0) {
			UI_Draw_Pic(ui_mouse_x - 64, ui_mouse_y - 64, 128, 128, getDrawItem(heldItem.name));
		}
		
	}
	
	if (showdialog == 1) {
		UI_Draw_Pic(0, 0, 640, 256, ui_dialogbox);
		int x = 0;
		int y = 0;
		for (int i = 0; i < strlen(dialogbox_text); i++) {
			char c = dialogbox_text[i];
			if (strlen(dialogbox_text) - dialogcounter < i) {
				break;
			}
			if (strlen(dialogbox_text) - dialogcounter < i + 1) {
				c = lastdialogchar;
			}
			Draw_CharacterScale2(98 * 2 + x * 16, 32 + y * 16, 16, c);
			x++;
			if (dialogbox_text[i] == ' ') {
				int i2 = i + 1;
				int x2 = x + 1;
				while (dialogbox_text[i2] != ' ' && dialogbox_text[i2] != '.' && i2 < strlen(dialogbox_text)) {
					x2++;
					i2++;
					if (x2 > 25) {
						x = 0;
						y++;
						break;
					}
				}
			}

		}
	}

	if (ui_keypadOpen == true) {
		UI_DrawKeypad();
	}
	UI_Draw_OverlayText(128, 128, 2, notes_text);
	
	if (notes_count < 200) {
		notes_count++;
	}
	else {
		notes_count = 0;
		notes_text = "";
	}
	//UI_UpdateOverlayText();
	UI_Draw_Note();
	int health = cl.stats[STAT_HEALTH] / 5;
	int ammo = cl.stats[STAT_AMMO];
	/*Sbar_DrawNum(136, 0, cl.stats[STAT_HEALTH], 3
		, cl.stats[STAT_HEALTH] <= 25);*/

	//Draw Health
	//UI_Draw_Pic(0, (scr_vrect.height-70) - (4 * 6), 145 * 6, 16 * 6, ui_healthbar);
	for (int i = 0; i < 100/5; i++) {
		if (i < health) {
			UI_Draw_Pic(16 + (i * (scale / 21.0f) * 5), (scr_vrect.height - ((scale / 16) * 5)-(scale/16)), (scale / 16) * 5, (scale / 16) * 5, ui_healthseg);
		}
		else {
			UI_Draw_Pic(16 + (i * (scale / 21.0f) * 5), (scr_vrect.height - ((scale / 16) * 5) - (scale / 16)), (scale / 16) * 5, (scale / 16) * 5, ui_healthseg2);
		}
	}

	//Draw Boss Health

	if (drawbosshealth == 1) {
		for (int i = 0; i < 100 / 5; i++) {
			if (i < bosshealth/5) {
				UI_Draw_Pic(16 + (i * (scale / 21.0f) * 5), (scr_vrect.height - ((scale / 16) * 5) - (scale / 16)) - (scale / 16) * 6, (scale / 16) * 5, (scale / 16) * 5, ui_healthseg3);

			}
			else {
				UI_Draw_Pic(16 + (i * (scale / 21.0f) * 5), (scr_vrect.height - ((scale / 16) * 5) - (scale / 16)) - (scale / 16) * 6, (scale / 16) * 5, (scale / 16) * 5, ui_healthseg2);
			}
		}
	}
	


	//UI_Draw_Pic(0, (scr_vrect.height - 196), 64 * 3, 64 * 3, ui_health2);
	//if (health < 0) {
	//	health = 0;
	//}
	//if (health > 100) {
	//	health = 100;
	//}
	//UI_Draw_HBar(0, (scr_vrect.height - 196), 64 * 3, 64 * 3, ui_healthbar2, 1.0f-((float)health/100.0f));

	//draw hud items
	float tilesize = (scale / 2.0f);
	float hudoffset = ((scr_vrect.width / 2) - tilesize * 4.5f);
	for (int i = 0; (i < 9) && (i < inv_numslots); i++) {
		if (i == inv_highlight) {
			UI_Draw_Pic(hudoffset + (scale / 2.0f * i), scr_vrect.height - tilesize, tilesize, tilesize, inv_tileHighlight32);
		} else {
			UI_Draw_Pic(hudoffset + (scale / 2.0f * i), scr_vrect.height - tilesize, tilesize, tilesize, inv_tile32);
		}
		

		unsigned int drawItem = getDrawItem(inv_slots[i].name);
		if (drawItem != 0) {
			UI_Draw_Pic(hudoffset + (tilesize * i), scr_vrect.height - tilesize, tilesize, tilesize, drawItem);
			if (inv_slots[i].id == 1) {
				float r = 0;
				float g = 0;
				blendDurColors(inv_slots[i].durability, &r, &g);
				UI_DrawRectangle(hudoffset + (tilesize * i) + 4, scr_vrect.height - 8, 2, (tilesize - 8)* inv_slots[i].durability, r, g, 0.4, 1.0);
			}

		}
		
		char numStr[6];
		sprintf(numStr, "%d", inv_slots[i].number);
		if (inv_slots[i].number > 1) {
			UI_Draw_Text(144 * 6 + (64 * i), scr_vrect.height - 64 + 48, 16, &numStr);
		}
		

	}


	//Draw Ammo
	if (!(strcmp(inv_slots[inv_highlight].name, "grenade") == 0 ||
		strcmp(inv_slots[inv_highlight].name, "hammer") == 0 ||
		inv_slots[inv_highlight].id == 0)) { // if not holding grenade, hammer, or item
		if (ammo >= 0) {
			UI_DrawAmmo(scr_vrect.width - (9 * ui_scale), scr_vrect.height - (14 * ui_scale), ammo % 10);
			UI_DrawAmmo(scr_vrect.width - 2 * (9 * ui_scale), scr_vrect.height - (14 * ui_scale), ammo / 10);
		}
		if (ammo >= 100) {
			SV_BroadcastPrintf("ERROR, above 100\n");
		}

	}
	//Con_DPrintf("reticle: %f\n", ui_reticle_size);
	//UI_Draw_Pic(50, 50, 832, 96, inv_handwritten);
	//UI_Draw_Pic(0, 0, 155*4, 176 * 4, inv_paper);
	//UI_Draw_Text(50, 100, 32, "hello world\ngoodbye world");


	if (ui_computeropen) {
		UI_Draw_Console();
	}
	if (strcmp(lastItem, inv_slots[inv_highlight].name) != 0) {
		ui_textscroll = 0;
	}
	if (lasttutorialflags != tutorialflags) {
		int temp = tutorialflags - lasttutorialflags;
		printf("temp flags: %f\n", (float)temp);
		if (temp == TUTORIAL_GRENADE) {
			dialogbox_text = "hold down left click to charge release to throw: the longer its charged the farther it goes";
		}
		else if (temp == TUTORIAL_RAILGUN) {
			dialogbox_text = "hold down left click to charge: the longer you charge the more damage it deals";
		}
		lasttutorialflags = tutorialflags;
		showdialog = 1;
		dialogcounter = strlen(dialogbox_text);
	}
	
	if (dialogcounter > -45 && showdialog == 1) {
		dialogcounter -= ui_deltatime*15;
		if (lastdialogcounter != (int)dialogcounter && dialogcounter > 0) {
			S_LocalSound("misc/dialogblip.wav");
			lastdialogchar = (char)(rand() % 255);
		}
		lastdialogcounter = (int)dialogcounter;
	}
	else {
		showdialog = 0;
	}

}

void reticleLogic() {
	//char* name = inv_slots[inv_highlight].name;
	//if (!name) {
	//	return;
	//}
	//if (strcmp(name, "pistol") == 0) {
	//	ui_reticle_size = 30;
	//}
	//if (inv_lasthighlight != inv_highlight) {
	//	
	//}

	//
	///*if (strcmp(name, "pistol") == 0) {
	//	if (ui_reticle_size > ui_reticle_min && key_dest == key_game && ui_movechanged == 0) {
	//		ui_reticle_size -= 0.0009;
	//	}
	//}*/
	//inv_lasthighlight = inv_highlight;
	//inv_lastname = name;
}

unsigned int getDrawItem(char* name) {
	unsigned int drawnTile = 0;

	if (strcmp(name, "") == 0) { drawnTile = 0; }
	else if (strcmp(name, "super_shotgun") == 0) { drawnTile = inv_supershotgun; }
	else if (strcmp(name, "shotgun") == 0) { drawnTile = inv_shotgun; }
	else if (strcmp(name, "pistol") == 0) { drawnTile = inv_pistol; }
	else if (strcmp(name, "pistolbroken") == 0) { drawnTile = inv_pistolbroken; }
	else if (strcmp(name, "grenade") == 0) { drawnTile = inv_grenade; }
	else if (strcmp(name, "healthkit") == 0) { drawnTile = inv_healthkit; }
	else if (strcmp(name, "soda") == 0) { drawnTile = inv_soda; }
	else if (strcmp(name, "watergun") == 0) { drawnTile = inv_watergun; }
	else if (strcmp(name, "powersource") == 0) { drawnTile = inv_powersource; }
	else if (strcmp(name, "hammer") == 0) { drawnTile = inv_hammer; }
	else if (strcmp(name, "railgun") == 0) { drawnTile = inv_railgun; }
	else if (strcmp(name, "knife") == 0) { drawnTile = inv_knife; }
	else if (strcmp(name, "rifle") == 0) { drawnTile = inv_rifle; }
	else if (strcmp(name, "chip1") == 0) { drawnTile = inv_chip1; }
	else if (strcmp(name, "revolver") == 0) { drawnTile = inv_revolver; }
	else if (strcmp(name, "laserhammer") == 0) { drawnTile = inv_laserhammer; }
	else if (strcmp(name, "grenadelauncher") == 0) { drawnTile = inv_grenadelauncher; }
	else {
		drawnTile = 0;
		strcpy(name, "");
		//printf("invalid slot name: %s\n", name); 
	}
	return drawnTile;
}
void UI_Draw_Cash_Nums(int x, int y, int size, int num) {
	int offset = 0;
	char str[12];
	sprintf(str, "%i", num);
	for (int i = 0; i < strlen(str); i++) {
		int digit = (int)str[i] - '0';
		glBindTexture(GL_TEXTURE_2D, ui_cashnums);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0 / 10.0 + (digit / 10.0), 1.0); glVertex2f(x + size * 5 + (i*size*6), y + size * 8);
		glTexCoord2f(0.0 + (digit / 10.0), 1.0); glVertex2f(x + (i * size * 6), y + size * 8);
		glTexCoord2f(0.0 + (digit / 10.0), 0.0); glVertex2f(x + (i * size * 6), y);
		glTexCoord2f(1.0 / 10.0 + (digit / 10.0), 0.0); glVertex2f(x + size * 5 + (i * size * 6), y);
		glEnd();
		num /= 10;
	}
	
}
void UI_UpdateOverlayText() {
	if (strcmp(notes_target, "") != 0) {
		if (strcmp(notes_target, notes_text) == 0) {
			notes_count += ui_deltatime;
			if (notes_count > 8) {
				memset(notes_text, '\0', 200);
				memset(notes_target, '\0', 200);
				notes_chari = 0;
				notes_count = 0;
			}
		}
		else {
			if (notes_char_count > 2) {
				notes_text[notes_chari] = notes_target[notes_chari];
				if (notes_chari > strlen(notes_text) - 3) {
					notes_text[notes_chari + 1] = rand() % 255;
					notes_text[notes_chari + 2] = rand() % 255;
				}
				notes_chari++;
				notes_char_count = 0;
			}
			else {
				notes_char_count++;
			}

		}

	}

}
int UI_CheckUIBox(int mouse_x, int mouse_y, int box_x, int box_y, int box_width, int box_height) {	
	if (mouse_x > box_x && mouse_x < box_x + box_width &&
		mouse_y > box_y && mouse_y < box_y + box_height) {
		return 1;
	}
	//printf("mouse x: %i, mouse y: %i\n", mouse_x, mouse_y);
	return 0;
}
void UI_EnableKeypad(int i0, int i1, int i2, int i3, int success, edict_t* ent) {
	ui_keycode[0] = i0;
	ui_keycode[1] = i1;
	ui_keycode[2] = i2;
	ui_keycode[3] = i3;
	for (int i = 0; i < 4; i++) { keypadVal[i] = -2; }
	keypad_id = 0;
	ui_keypadOpen = true;
	IN_Deactivate(true);
	ui_keypadSuccess = success;
	//printf("%i %i %i %i\n", (int)ui_keycode[0], (int)ui_keycode[1], (int)ui_keycode[2], (int)ui_keycode[3]);
	ui_activekeypad = ent;
	if (ui_activekeypad != ent) {
		printf("error, invalid pointer\n");
	}
}

void blendDurColors(float x, float * r, float * g) {
	x = 1.0 - x;
	if (x < 0.5) {
		*g = 1;
		*r = (x * 2);
	}
	else {
		*g = ((1.0f-x) * 2);
		*r = 1;
	}
	*r *= 0.6;
	*g *= 0.6;
	*r += 0.4;
	*g += 0.4;
}

void UI_PlayKeypadBeep() {
	
	//printf("beeeep %i\n", ui_activekeypad->num_leafs);
	
}
void UI_DisableKeypad() {
	ui_keypadOpen = false;
	IN_Activate();
}
qboxpos UI_InitBox(int x, int y) { qboxpos value; value.x = x; value.y = y; return value; }
void UI_Free(void) {
	free(notes_text);
	free(notes_target);
}
void UI_Init(void) {
	Inventory_Init();
	UI_LoadPics();
	UI_InitNotes();
	srand(time(0));
	notes_text = malloc(200 * sizeof(char));
	notes_target = malloc(200 * sizeof(char));
	heldItem.id = 0;
	strcpy(heldItem.name, "");
	heldItem.number = 1;

	inv_highlight = 0;

	inv_dhighlight = 0.0f;
	inventoryOpen = 0;

	ui_reticle_base = 0.00;
	ui_reticle_min = 0.05;
	ui_reticle_size = ui_reticle_base;
	ui_deltatime = 0.0;
	ui_movechanged = 0;
	keypad_id = 0;
	ui_clickPressed = false;
	ui_canclick = true;
	ui_keypadSuccess = false;
	for (int i = 0; i < 4; i++) { keypadVal[i] = -2; }
	for (int i = 0; i < 4; i++) { ui_keycode[i] = -2; }

	ui_mouse_x = 0;
	ui_mouse_y = 0;
	ui_scale = 8.0f;
	inv_clear = 0;

	notes_text = "";
	notes_chari = 0;
	notes_target = "";
	notes_count = 0;
	notes_char_count = 0;
	ui_selectClicked = false;
	q_snprintf(comp_screen, 50 * 35, "%s\n%s", comp_screen, "WELCOME TO STATION OS: \nTYPE HELP FOR LIST OF COMMANDS, OR CONSULT THE MANUAL\n\nNOTE: DUE TO EMERGENCIES, ALL STATION 37 COMPUTERS\nARE CURRENTLY READ ONLY");
	dialogbox_text = "";
	showdialog = 0;
	dialogcounter = 0;
	//inv_lastname = "";
	inv_lasthighlight = 0;
	ui_reticle_size = 0;
	lastItem = "";
	ui_scrollcount = 0;
	tutorialflags = 0;
	drawbosshealth = 0;
	bosshealth = -1;
	//ui_activekeypad = NULL;
}
void UI_LoadPics(void) {
	inv_tile = UI_LoadTexture("id1/textures/UI/tile.png");
	ui_compscreen = UI_LoadTexture("id1/textures/comscreen.png");
	ui_comptext = UI_LoadTexture("id1/textures/CONCHARS2.png");
	inv_tile32 = UI_LoadTexture("id1/textures/UI/tile32.png");
	inv_tileHighlight32 = UI_LoadTexture("id1/textures/UI/tileHighlight32.png");
	ui_keypad = UI_LoadTexture("id1/textures/UI/keypad.png");
	ui_sevensegment = UI_LoadTexture("id1/textures/UI/7segment.png");
	ui_ammotext = UI_LoadTexture("id1/textures/UI/ammo.png");
	ui_healthseg = UI_LoadTexture("id1/textures/UI/healthseg.png");
	ui_healthseg2 = UI_LoadTexture("id1/textures/UI/healthseg2.png");
	ui_healthseg3 = UI_LoadTexture("id1/textures/UI/healthseg3.png");
	ui_healthbar2 = UI_LoadTexture("id1/textures/UI/healthbar2.png");
	ui_health2 = UI_LoadTexture("id1/textures/UI/healthbkg2.png");
	ui_healthbar = UI_LoadTexture("id1/textures/UI/healthbar.png");
	inv_use = UI_LoadTexture("id1/textures/UI/use.png");
	ui_font = UI_LoadTexture("id1/textures/UI/text.png");
	inv_tileHighlight = UI_LoadTexture("id1/textures/UI/tileHighlight.png");
	inv_header = Draw_PicFromWad("inv_header");
	inv_x = Draw_PicFromWad("inv_x");
	inv_testItem = Draw_PicFromWad("inv_testicon");
	inv_grenade = UI_LoadTexture("id1/textures/icons/inv_igranade.png");
	inv_supershotgun = UI_LoadTexture("id1/textures/icons/inv_ishotgun2.png");
	inv_shotgun = UI_LoadTexture("id1/textures/icons/inv_ishotgun.png");
	inv_healthkit = UI_LoadTexture("id1/textures/icons/inv_ihealthkit.png");
	inv_pistol = UI_LoadTexture("id1/textures/icons/inv_ipistol.png");
	inv_rifle = UI_LoadTexture("id1/textures/icons/inv_irifle.png");
	inv_pistolbroken = UI_LoadTexture("id1/textures/icons/inv_ipistol2.png");
	inv_laserhammer = UI_LoadTexture("id1/textures/icons/inv_ilaserhammer.png");
	inv_grenadelauncher = UI_LoadTexture("id1/textures/icons/inv_ilauncher.png");
	inv_hammer = UI_LoadTexture("id1/textures/icons/inv_ihammer.png");
	inv_powersource = UI_LoadTexture("id1/textures/icons/inv_ipower.png");
	inv_railgun = UI_LoadTexture("id1/textures/icons/inv_irailgun.png");
	inv_soda = UI_LoadTexture("id1/textures/icons/inv_isoda.png");
	inv_knife = UI_LoadTexture("id1/textures/icons/inv_iknife.png");
	inv_watergun = UI_LoadTexture("id1/textures/icons/inv_iwatergun.png");
	inv_chip1 = UI_LoadTexture("id1/textures/icons/inv_ichip1.png");
	inv_revolver = UI_LoadTexture("id1/textures/icons/inv_irevolver.png");
	ui_reticle_back = Draw_PicFromWad("reticle");
	ui_reticle_bottom = UI_LoadTexture("id1/textures/ui_reticle_bottom.png");
	ui_reticle_left = UI_LoadTexture("id1/textures/ui_reticle_left.png");
	ui_reticle_right = UI_LoadTexture("id1/textures/ui_reticle_right.png");
	ui_reticle_top = UI_LoadTexture("id1/textures/ui_reticle_top.png");
	pngTest = UI_LoadTexture("id1/textures/tile.png");
	inv_paper = UI_LoadTexture("id1/textures/journalblank.png");
	inv_tablet = UI_LoadTexture("id1/textures/UI/tablet.png");
	ui_pickitem = UI_LoadTexture("id1/textures/UI/selectItem.png");
	ui_cash = UI_LoadTexture("id1/textures/UI/cash.png");
	ui_cashnums = UI_LoadTexture("id1/textures/UI/cashNums.png");
	ui_dialogbox = UI_LoadTexture("id1/textures/dialogUI.png");
	ui_description = UI_LoadTexture("id1/textures/UI/screen.png");
	unsigned int path_id;
	ui_shader = GL_CreateProgram((GLchar*)COM_LoadHunkFile("shaders/ui.vs", &path_id), (GLchar*)COM_LoadHunkFile("shaders/ui.fs", &path_id), 0, NULL);
	ui_selectItem = false;
	ui_compmode = 0;
	lastdialogchar = ' ';
}
void UI_Keydown(int key) {
	/*if (key == K_TAB) {
		inventoryOpen = 1 - inventoryOpen;
	}*/
	
	//printf("key %d\n", key);
}
void UI_DrawSevenSegment(int x, int y, int val) {
	int scale = ui_scale;
	double offset = (1.0 / 10.0) * ((double)val);
	glBindTexture(GL_TEXTURE_2D, ui_sevensegment);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0 / 10.0 + offset, 1.0); glVertex2f(x + (scale * 9), y + (scale * 15));
		glTexCoord2f(0.0 + offset, 1.0); glVertex2f(x, y + (scale * 15));
		glTexCoord2f(0.0 + offset, 0.0); glVertex2f(x, y);
		glTexCoord2f(1.0 / 10.0 + offset, 0.0); glVertex2f(x + (scale * 9), y);
	glEnd();
}
void UI_DrawAmmo(int x, int y, int val) {
	int scale = ui_scale;
	double offset = (1.0 / 10.0) * ((double)val);
	glBindTexture(GL_TEXTURE_2D, ui_ammotext);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0 / 10.0 + offset, 1.0); glVertex2f(x + (scale * 9), y + (scale * 15));
	glTexCoord2f(0.0 + offset, 1.0); glVertex2f(x, y + (scale * 15));
	glTexCoord2f(0.0 + offset, 0.0); glVertex2f(x, y);
	glTexCoord2f(1.0 / 10.0 + offset, 0.0); glVertex2f(x + (scale * 9), y);
	glEnd();
}
void UI_DrawRectangle(int x, int y, int w, int h, float r, float g, float b, float a) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	glColor4f(r,g,b,a);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0); glVertex2f(x + h, y + w);
		glTexCoord2f(0.0, 1.0); glVertex2f(x, y + w);
		glTexCoord2f(0.0, 0.0); glVertex2f(x, y);
		glTexCoord2f(1.0, 0.0); glVertex2f(x + h, y);
	glEnd();
	glEnable(GL_TEXTURE_2D);

}
void UI_DrawKeypad() {
	glEnable(GL_BLEND);
	UI_Draw_Pic(0, 0, 45 * ui_scale, 84 * ui_scale, ui_keypad);
	for (int i = 0; i < 4; i++) {
		if (keypadVal[i] == -2) {
			continue;
		}
		UI_DrawSevenSegment(6 * ui_scale + (8 * ui_scale * i), 6 * ui_scale, ((int)keypadVal[i]));
	}
	if (ui_keypadSuccess == true) {
		UI_DrawRectangle(5 * ui_scale, 6 * ui_scale, 15 * ui_scale, 38 * ui_scale, 0.0, 0.5, 0.0, 0.3);
	}
	
}
void UI_UpdateKeypad(edict_t * entity) {
	int val = 0;
	int number = -2;
	if (ui_keypadOpen==0) {
		return;
	}
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < 4; i2++) {
			val = UI_CheckUIBox(ui_mouse_x, ui_mouse_y, (5 + (i * 10)) * ui_scale, (23 + (i2 * 14)) * ui_scale, 9 * ui_scale, 14 * ui_scale);
			if (val == 1) {
				number = i + 1 + (i2 * 3);
				if (number > 9) {
					if (number == 10) { number = 0; }
					if (number == 11) { number = -1; }
				}
			}
		}
	}
	if (ui_clickPressed == 1.0f) {
		if (ui_canclick == true) {
			if (number != -2 && keypad_id < 4 && number != -1) {
				//SV_StartSound(entity, 1, "items/keypad1.wav", 48, 0.5);
				S_LocalSound("items/keypad1.wav");
				keypadVal[keypad_id] = number;
				keypad_id++;
				ui_canclick = false;
				if (keypad_id > 3) {
					// check if it is correct
					qboolean success = true;
					for (int i = 0; i < 4; i++) {
						if (keypadVal[i] != ui_keycode[i]) { success = false; }
					}
					if (success == true) {
						ui_keypadSuccess = true;
					} else {
						//printf("failed\n");
						keypad_id = 0;
						for (int i = 0; i < 4; i++) { keypadVal[i] = -2; }
					}
				}
			}
			if (number == -1) { keypad_id = 0; for (int i = 0; i < 4; i++) { keypadVal[i] = -2; } } //CLEAR
		}
	}
	else {
		ui_canclick = true;
	}
}

void UI_Draw_Pic(int x, int y, int width, int height, unsigned int pic) {
	//GL_UseProgramFunc(ui_shader);
	glBindTexture(GL_TEXTURE_2D, pic);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0); glVertex2f(x+width, y+height);
		glTexCoord2f(0.0, 1.0); glVertex2f(x, y+height);
		glTexCoord2f(0.0, 0.0); glVertex2f(x, y);
		glTexCoord2f(1.0, 0.0); glVertex2f(x+width, y);
	glEnd();
	//GL_UseProgramFunc(0);
	GL_ActiveTextureFunc(GL_TEXTURE0);

	//glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);

}
void UI_Draw_HBar(int x, int y, int width, int height, unsigned int pic, float health) {
	glBindTexture(GL_TEXTURE_2D, pic);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0); glVertex2f(x + width, y + ((float)height));
	glTexCoord2f(0.0, 1.0); glVertex2f(x, y + ((float)height));
	glTexCoord2f(0.0, health); glVertex2f(x, y+ ((float)height*health));
	glTexCoord2f(1.0, health); glVertex2f(x + width, y + ((float)height * health));
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);

}


unsigned int UI_LoadTexture(char* name) {
	unsigned int value;
	value = 0;
	glGenTextures(1, &value);
	glBindTexture(GL_TEXTURE_2D, value); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

	//printf("texture bound\n");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//// load image; create texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(name, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		printf("loaded texture with %s path and value %i\n", name, value);
	}
	else
	{
		printf("model did not have texture\n");
		value = 0;
	}
	stbi_image_free(data);
	return value;
}
char * UI_NotesLoad(char * filename) {
	char realfilename[100];
	sprintf(realfilename, "text/%s.txt", filename);
	char* buffer;
	buffer = COM_LoadMallocFile(&realfilename, NULL);
	return buffer;
}
void UI_InitNotes() {
	//load GFX
	notes_handwritten = UI_LoadTexture("id1/textures/fontHW.png");
	notes_paper = UI_LoadTexture("id1/textures/UI/paper.png");

}
void UI_Draw_Note() {
	if (ui_drawnote == true) {
		float scale = ui_scale / 2.0f;
		extern vrect_t scr_vrect;
		int x = scr_vrect.width / 2.0 - ((128 * scale) / 2.0);
		int y = scr_vrect.height / 2.0 - ((192 * scale) / 2.0);
		UI_Draw_Pic(x, y, 128 * scale, 192 * scale, inv_tablet);

		int txtoffset = 12 * scale;
		int txtY = 0;
		int txtX = 0;
		char c;
		for (int i = 0; i < strlen(paper_text); i++) {
			c = paper_text[i];
			if (c == '\n') {
				txtX = 0;
				txtY++;
				continue;
			}
			if (c == 0x0D) {
				continue;
			}
			Draw_CharacterScale2(x + txtoffset+txtX*3.0f*scale, y + txtoffset + (txtY*scale*3.5f), scale * 3.0f, c);
			if (paper_text[i] == ' ') {
				int check = 0;
				int i2 = i + 1;
				int x2 = txtX + 1;
				while (paper_text[i2] != ' ' && paper_text[i2] != '.' && paper_text[i2] != '\n' && i2<strlen(paper_text)) {
					x2++;
					i2++;
					if (x2 > 35) {
						txtX = 0;
						txtY++;
						check = 1;
						break;
					}
				}
				if (check == 1) {
					continue;
				}
			}

			txtX++;
		}
		//UI_Draw_Text(x + (4 * scale), y + (4 * scale), 16 * scale / 8.0f, paper_text);
		//printf("%s\n", notes_text);
	}
}
void UI_Draw_OverlayText(int x, int y, int size, const char* str) {
	float widthP = 1.0f / 37.0f;
	float offsetx = 0.0f;
	float offsety = 0.0f;
	float offsettex = 0.0f;
	int newlinecount = 0;
	glBindTexture(GL_TEXTURE_2D, ui_font);
	glBegin(GL_QUADS);
	for (int i = 0; i < strlen(str); i++) {
		
		offsetx = (float)newlinecount;
		offsettex = (float)(str[i] - 'a');
		if (str[i] == 0) {
			break;
		}
		if (str[i] < 58 && str[i] > 47) { // is a number
			offsettex = (float)(str[i] - '0' + 26);
		}
		if (str[i] == ':') {
			offsettex = 36;
		}
		if (str[i] == ' ') {
			if (newlinecount > 20) {
				newlinecount = -1;
				offsety++;
			}
		}else {
			glTexCoord2f(widthP + (offsettex * widthP), 1.0);	glVertex2f(x + (size * 20) + (offsetx * 20 * size), y + (size * 12) + (offsety * 14 * size));
			glTexCoord2f(0.0 + (offsettex * widthP), 1.0);		glVertex2f(x + (offsetx * 20 * size), y + (size * 12) + (offsety * 14 * size));
			glTexCoord2f(0.0 + (offsettex * widthP), 0.0);		glVertex2f(x + (offsetx * 20 * size), y + (offsety * 14 * size));
			glTexCoord2f(widthP + (offsettex * widthP), 0.0);	glVertex2f(x + (size * 20) + (offsetx * 20 * size), y + (offsety * 14 * size));
		}
		newlinecount++;
		
	}
	glEnd();
}

void UI_Draw_Text(int x, int y, int size, char* str) {
	glBindTexture(GL_TEXTURE_2D, notes_handwritten);
	double offsetx = 1.0 / 26.0;
	double offsety = 1.0 / 3.0;
	double newline_offset = 0.0;
	int x_loc = 0;
	double kerningVal = 0.7;
	glBegin(GL_QUADS);
	for (int i = 0; i < strlen(str); i++) {

		if (str[i] == '\0') {
			break;
		}

		int charIndex = 0;
		double texY = 0;

		int tempX = (size * x_loc * kerningVal) + x;
		if (str[i] >= 'a' && str[i] <= 'z') {
			charIndex = str[i] - 97;
			texY = 1.0 / 3.0;
		}
		else if (str[i] >= 'A' && str[i] <= 'Z') {
			charIndex = str[i] - 65;
			texY = 0.0 / 3.0;
		} else if (str[i] >= '0' && str[i] <= '9') {
			charIndex = str[i] - 48;
			texY = 2.0 / 3.0;
		}
		else if (str[i] == '!') {
			texY = 2.0 / 3.0;
			charIndex = 10;
		}
		else if (str[i] == '?') {
			texY = 2.0 / 3.0;
			charIndex = 11;
		}
		else if (str[i] == '.') {
			texY = 2.0 / 3.0;
			charIndex = 13;
		}
		else if (str[i] == ':') {
			texY = 2.0 / 3.0;
			charIndex = 14;
		}
		else if (str[i] == ',') {
			texY = 2.0 / 3.0;
			charIndex = 15;
		}
		else {
			charIndex = str[i] - 48;
			texY = 2.0 / 3.0;
		}
		x_loc++;
		if (str[i] == ' ') {

		}
		else if (str[i] == '\n') {
			newline_offset += size;
			x_loc = 0;
		}
		else {
			
			glTexCoord2f((charIndex * offsetx) + offsetx, offsety + texY); glVertex2f(tempX + size, y + size + newline_offset);
			glTexCoord2f((charIndex * offsetx), offsety + texY); glVertex2f(tempX, y + size + newline_offset);
			glTexCoord2f((charIndex * offsetx), 0.0 + texY); glVertex2f(tempX, y + newline_offset);
			glTexCoord2f((charIndex * offsetx) + offsetx, 0.0 + texY); glVertex2f(tempX + size, y + newline_offset);
			
			//printf("%i ", charIndex);
		}
	}
	glEnd();
	//printf("\n");

}
void UI_Draw_Console() {
	extern vrect_t scr_vrect;

	UI_Draw_Pic(0, 0, scr_vrect.width, scr_vrect.height, ui_compscreen);
	for (int i = 0; i < 50; i++) {
		if ((unsigned int)comp_type[i] == 0) {
			Draw_CharacterScale2(65 + (i * 32), scr_vrect.height * 0.91, 32, 13);
			break;
		}
		Draw_CharacterScale2(65 + (i * 32), scr_vrect.height * 0.91, 32, comp_type[i]);
	}
	int yoff = 0;
	int xoff = 0;
	for (int i = 0; i < 50 * 35; i++) {
		if (comp_screen[i] == 0x0D) { //end of line may be two bytes
			continue;
		}
		if (comp_screen[i] == '\n') {
			yoff += 32;
			xoff = 0;
			continue;
		}
		if ((unsigned int)comp_screen[i] == 0) {
			break;
		}
		Draw_CharacterScale2(65 + xoff, scr_vrect.height * 0.03 + yoff, 32, comp_screen[i]);
		xoff += 32;
	}

}
