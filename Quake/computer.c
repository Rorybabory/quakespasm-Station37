#include "ui.h"

void UI_ClearConsole() {
	for (int i = 0; i < 50 * 35; i++) { comp_screen[i] = '\0'; }
}

void UI_PrintConsole(char * str) {
	q_snprintf(comp_screen, 50 * 35, "%s\n%s", comp_screen, str);

}
void UI_ConsoleType(int key) {
	extern qboolean	keydown[];

	if (key == K_BACKSPACE) {
		if (strlen(comp_type) > 0) {
			comp_type[strlen(comp_type) - 1] = '\0';
		}
		return;
	}
	if (key == K_SHIFT) {
		return;
	}
	if (keydown[K_SHIFT] == true) {
		switch (key) {
		case '1':
			key = '!';
			break;
		case '2':
			key = '@';
			break;
		case '3':
			key = '#';
			break;
		case '4':
			key = '$';
			break;
		case '5':
			key = '%';
			break;
		case '6':
			key = '^';
			break;
		case '7':
			key = '&';
			break;
		case '8':
			key = '*';
			break;
		case '9':
			key = '(';
			break;
		case '0':
			key = ')';
			break;
		case '-':
			key = '_';
			break;
		case '=':
			key = '+';
			break;
		case ';':
			key = ':';
			break;
		case '\'':
			key = '"';
			break;
		case ',':
			key = '<';
			break;
		case '.':
			key = '>';
			break;
		case '/':
			key = '?';
			break;
		default:
			break;
		}
	}
	if (key >= 97 && key <= 122) {
		key -= 32;
	}
	q_snprintf(comp_type, 50, "%s%c", comp_type, (char)key);
}
void UI_ConsoleInput(int key) {
	extern qboolean	keydown[];

	if (ui_compmode != 0) {
		if (key == K_ENTER) {
			

		}
		return;
	}
	if (key == K_ALT || key == K_TAB) {
		return;
	}

	if (key == K_ENTER) {
		//q_snprintf(comp_screen, 50 * 35, "%s\n%s", comp_screen, comp_type);
		//printf("%s\n", comp_type);
		char delim[] = " ";

		char* ptr = strtok(comp_type, delim);
		char* command = "";
		char* parameter = "";
		for (int i = 0; ptr != NULL; i++)
		{
			if (i == 0) { command = ptr; }
			if (i == 1) { parameter = ptr; }
			ptr = strtok(NULL, delim);
		}
		if (command != NULL) {
			if (strcmp(command, "CLEAR") == 0) {
				UI_ClearConsole();
			}
			else if (strcmp(command, "LOAD") == 0) {
				
				UI_ClearConsole();
				if (strcmp(parameter, "") != 0) {
					char realfilename[100];
					sprintf(realfilename, "computers/%s/%s", ui_active_computer, parameter);
					char* buffer;

					buffer = COM_LoadMallocFile(&realfilename, NULL);
					if (buffer != NULL) {
						UI_PrintConsole(buffer);
					}
					else {
						UI_PrintConsole("ERROR: MISSING PARAMETER\n");
					}
				}
				else {
					UI_PrintConsole("ERROR: MISSING PARAMETER\n");
				}
				
			}
			else if (strcmp(command, "LIST") == 0) {
				UI_ClearConsole();
				struct dirent* de; // Pointer for directory entry
				char filename[100];
				sprintf(filename, "id1/computers/%s/", ui_active_computer);
				printf("%s\n", filename);
				DIR* dr = opendir(&filename);

				if (dr == NULL)
				{
					printf("COULD NOT OPEN DIRECTORY:%s\n", ui_active_computer);
					return 0;
				}
				for (int i = 0; (de = readdir(dr)) != NULL; i++) {
					if (i > 1) {
						UI_PrintConsole(de->d_name);
					}
				}
				closedir(dr);
			}
			else if (strcmp(command, "HELP") == 0) {
				UI_ClearConsole();
				UI_PrintConsole("COMMANDS:\n\nCLEAR: CLEARS THE SCREEN\nLOAD <FILENAME>: LOADS A FILE WITH A NAME\nLIST: LISTS ALL FILES ON PC\nRUN <PROGRAM>: RUNS PROGRAM FROM YOUR INVENTORY");
			}
			else if (strcmp(command, "RUN") == 0) {
				ui_compmode = 1;
				printf("RUN\n");
				UI_ClearConsole();
				UI_PrintConsole("AVAILABLE PROGRAMS:\n");
			} else {
				UI_ClearConsole();
				UI_PrintConsole("INVALID COMMAND, TYPE HELP FOR LIST OF COMMANDS");
			}
		}

		for (int i = 0; i < 50; i++) { comp_type[i] = '\0'; }
		return;
	}
	UI_ConsoleType(key);

}