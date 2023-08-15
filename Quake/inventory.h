#pragma once
#include <stdlib.h>


struct qinvslot_t {
	char name[20];
	int id; // 0 = item | 1 = weapon | 
	int number; //number of objects in stack
	float durability;
};

int inventoryOpen;
int inv_highlight;
int inv_lasthighlight;
int inv_lasthighlight2;
char inv_lastname[20];

int inv_changed;

int inv_tab_buffer;
float inv_dhighlight;
int inv_clear;

int inv_cash;

int inv_numslots;
int inv_startnumslots;



struct qinvslot_t inv_slots[18];
struct qinvslot_t start_inv_slots[18];
struct qinvslot_t heldItem;
void Inventory_Init(void);
void Inventory_AddItem(char* name, int id, float durability);
void Inventory_SubtractDurability(float d);
char * Inventory_GetItemText(char* name);
int Inventory_GetNumItem(char* name);
int Inventory_RemoveItem(char* name);
int Inventory_GetCurrentNum();
int Inventory_Full(void); 
void Inventory_Clear(void);
int Inventory_HasItem(char* name);
void Inventory_RemoveCurrent();
int Inventory_IsEmpty();
void Inventory_AddStart();
int Inventory_GetChange();