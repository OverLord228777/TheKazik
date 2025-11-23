#pragma once

#include <string>

enum Slots { slot7, slotBerry, slotCPP, slotLemon };

constexpr int slotsNum = 4;

Slots randomSlot(void) {
	int randomNumber = rand() % slotsNum + 1;
	switch (randomNumber) {
	case (1):
		return slot7;
		break;
	case (2):
		return slotBerry;
		break;
	case (3):
		return slotCPP;
		break;
	case (4):
		return slotLemon;
		break;
	}
}

std::string getSlotPNG(Slots slot) {
	switch (slot) {
	case (slot7):
		return "Image/slot7.png";
		break;
	case (slotBerry):
		return "Image/slotBerry.png";
		break;
	case (slotCPP):
		return "Image/slotCPP.png";
		break;
	case (slotLemon):
		return "Image/slotLemon.png";
		break;
	}
}

int slotsCompare(Slots slot1, Slots slot2, Slots slot3, int bet, bool* win) {
	if (slot1 == slot2 && slot2 == slot3) {
		//*win = true;  
		return bet * 3;
	}
	else if (slot1 == slot7 && slot2 == slot7 && slot3 == slot7) { 
		//*win = true;
		return bet * 5; 
	}
	else if (slot1 == slot2 || slot2 == slot3 || slot1 == slot3) { 
		//*win = true;
		return bet; 
	}
	else { 
		*win = false;
		return -bet; 
	}
}