#pragma once
#include "inventory.h"
void Inventory_AddItem(char * name, int id, float durability) {
    struct qinvslot_t item;
    strcpy(item.name, name);
    item.id = id;
    item.durability = durability;
   // printf("item %s string length: %i\n", item.name, strlen(item.name));
    
    for (int x = 0; x < inv_numslots; x++) {
        if (strcmp(inv_slots[x].name, "") == 0) {
            strcpy(inv_slots[x].name, item.name);
            inv_slots[x].id = item.id;
            inv_slots[x].durability = item.durability;
            if (id == 1) {
                char impulsetxt[20];
                sprintf(impulsetxt, "impulse %i\n", x + 1);
                Cbuf_AddText(impulsetxt);
            }
            inv_slots[x].number = 1;

            printf("item: %s was added at %d\n", inv_slots[x].name, x);

            return;
        }
        //item stacking
        /*else if (strcmp(inv_slots[x][y].name, item.name) == 0) {
            inv_slots[x][y].number++;
            return;
        }*/
    }
    //printf("added item to inventory\n");
}
void Inventory_RemoveCurrent() {
    int x = inv_highlight;
    char name[20];
    strcpy(name, inv_slots[x].name);

    if (strcmp(inv_slots[x].name, "") != 0) {
        strcpy(inv_slots[x].name, "");
        inv_slots[x].id = 0;
        inv_slots[x].number = 0;
        inv_slots[x].durability = 0;
    }

}

int Inventory_GetChange() {
    if (strcmp(inv_lastname, inv_slots[inv_highlight].name) != 0 ||
        inv_lasthighlight2 != inv_highlight) {
        if (inv_changed == 0) {
            
        }
        else {
            strcpy(inv_lastname, inv_slots[inv_highlight].name);
        }
        inv_changed = 1;
        return 1;
    }
    else {
        inv_changed = 0;
        return 0;
    }
}

void Inventory_SubtractDurability(float d) {
    int x = inv_highlight;
    char name[20];
    strcpy(name, inv_slots[x].name);
    if (strcmp(inv_slots[x].name, "") != 0) {
        inv_slots[x].durability -= d;
    }

    if (inv_slots[x].durability <= 0) {
        Inventory_RemoveCurrent();
        for (int i = 0; i < inv_numslots; i++) {
            if (strcmp(inv_slots[i].name, name) == 0) {
                inv_highlight = i;
                break;
            }
        }
        

    }
}


int Inventory_GetCurrentNum() {
    return inv_slots[inv_highlight].number;
}
extern int drawbosshealth;
extern int bosshealth;
void Inventory_Clear(void) {
    for (int x = 0; x < 18; x++) {
        strcpy(inv_slots[x].name, "");
        inv_slots[x].id = 0;
        inv_slots[x].number = 0;
    }
    strcpy(inv_lastname, "");
    inv_numslots = 5;
    inv_startnumslots = 5;
    drawbosshealth = 0;
    bosshealth = -1;
}
int Inventory_Full(void) {
    for (int x = 0; x < inv_numslots; x++) {
        if (strcmp(inv_slots[x].name, "") == 0) {
            return 0;
        }
    }
    return 1;
}
int Inventory_HasItem(char* name) {
    for (int x = 0; x < 9; x++) {
        if (strcmp(inv_slots[x].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}
int Inventory_IsEmpty(void) {
    for (int x = 0; x < 9; x++) {
        if (strcmp(inv_slots[x].name, "") == 0 && inv_slots[x].id == 1) {
            return 0;
        }
    }

    return 1;
}

void Inventory_Init(void) {
    for (int i = 0; i < 18; i++) {
        strcpy(inv_slots[i].name, "");
        inv_slots[i].id = 0;
        inv_slots[i].number = 0;
        inv_slots[i].durability = 0;
        strcpy(start_inv_slots[i].name, "");
        start_inv_slots[i].id = 0;
        start_inv_slots[i].number = 0;
        start_inv_slots[i].durability = 0;
    }
    inv_cash = 0;
    inv_tab_buffer = 0;
    inv_numslots = 5;
    inv_startnumslots = 5;
    inv_lasthighlight2 = 0;
    strcpy(inv_lastname, "");
    inv_changed = 1;
}
void Inventory_AddStart() {
    
}

int Inventory_GetNumItem(char* name) {
    int num = 0;
    for (int x = 0; x < inv_numslots; x++) {
        if (strcmp(inv_slots[x].name, name) == 0) {
            num++;
        }
    }
    return num;
}

int Inventory_RemoveItem(char* name) {
    for (int x = 0; x < inv_numslots; x++) {
        if (strcmp(inv_slots[x].name, name) == 0) {
            strcpy(inv_slots[x].name, "");
            inv_slots[x].id = 0;
            inv_slots[x].number = 0;
            inv_slots[x].durability = 0;
            return 1;
        }
    }
    return 0;
}

char * Inventory_GetItemText(char* name) {
    if (strcmp(name, "") == 0) { return ""; }
    else if (strcmp(name, "super_shotgun") == 0) { return "A powerful weapon with high damage output but limited range due to its spread. It is most effective at very close range, where its wide pellet spread can inflict devastating damage."; }
    else if (strcmp(name, "shotgun") == 0) { return "This shotgun was originally designed on Earth but has been extensively modified with experimental technology on STATION 37. Despite its Earth origins, the weapon's enhancements give it a unique and potent edge. "; }
    else if (strcmp(name, "pistol") == 0) { return "A basic energy pistol commonly issued to personnel on STATION 37. It provides reliable and accurate energy-based firepower suitable for various situations."; }
    else if (strcmp(name, "pistolbroken") == 0) { return "A deteriorated version of the standard issue energy pistol. It is less accurate and prone to breaking, making it less reliable in combat. Consider finding a replacement."; }
    else if (strcmp(name, "grenade") == 0) { return "A volatile explosive capsule that can be thrown. Caution must be exercised while handling it due to its potential to explode. The longer you hold down the primary mouse button, the farther the capsule will be thrown."; }
    else if (strcmp(name, "healthkit") == 0) { return "A medical kit commonly issued to personnel for self-treatment. It can restore 50% of the maximum health when used, making it invaluable for recovering from injuries."; }
    else if (strcmp(name, "soda") == 0) { return "While not a powerful healing item, a soda can can provide a small amount of health restoration."; }
    else if (strcmp(name, "watergun") == 0) { return "Although it may seem innocuous, the squirt gun can surprisingly inflict a considerable amount of damage to robot security units."; }
    else if (strcmp(name, "powersource") == 0) { return "This power source, developed on STATION 37, has the capability to energize and power certain doors within the facility."; }
    else if (strcmp(name, "hammer") == 0) { return "Hammer. It's literally just a hammer."; }
    else if (strcmp(name, "railgun") == 0) { return "A powerful weapon that utilizes CELL energy as ammunition. The railgun can be charged to increase its damage output. The longer it is charged, the more devastating its impact becomes. Use it wisely to unleash its full potential."; }
    else if (strcmp(name, "knife") == 0) { return "A versatile knife suitable for various tasks. While primarily intended as a tool, it can also be used as a weapon in desperate situations."; }
    else if (strcmp(name, "rifle") == 0) { return "A formidable energy rifle specifically designed for STATION 37 security units. Its high caliber and energy output allows it to shred through any target it encounters. It is an effective weapon against a wide range of adversaries."; }
    else if (strcmp(name, "chip1") == 0) { return ""; }
    else if (strcmp(name, "revolver") == 0) { return "A powerful handgun designed for higher-tier security personnel on STATION 37. While it deals significant damage, it requires 2 CELLs to fire each shot. "; }
    else if (strcmp(name, "grenadelauncher") == 0) { return "WTF is this"; }
    else if (strcmp(name, "laserhammer") == 0) { return "??????????"; }

    else {
        return "";
        //printf("invalid slot name: %s\n", name); 
    }
}