/* Copyright (c) 2024 Diego A.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static const char proj[] = "PROJECT SNES BOOSTER";

const char name[] = "Diego A.";
const char noise[] = "MISAKA NETWORK";
const char atk[] = "CHAOS CONTROL";
const char def[] = "IMAGINE BREAKER";
const char quote1[] = "The time-space rift is expanding... There's no more time, I need to hurry.";
const char quote2[] = "Sounds good but you should know only the Power Rangers can control the Mega Voyger.";

const char spell1[] = "Zaker";
const char spell2[] = "Rashield";
const char spell3[] = "Jikerdor";
const char spell4[] = "Bao Zakeruga";

const char rando0[] = "Sparkling Wide Pressure";

const char end1[] = "You foolish man, I am a Jito-Ryo master. Allow me to reunite you with your ancestors.";
const char end2[] = "Jump on panel number one, it will take you to panels two and three.";

static const char decIntro[] = "Wii Remote Support for SNES VC\n\n";
//"\n";

// 2C000000 41820020 38600001 48 = HOME Menu aspect ratio


// ROM header check, used in later releases like Ignition Factor
//90a63698
//4BFC92dd // noop to skip header check
//3c7f000e
//8883159d

// RomHeader check, SFII Alpha
//94 21 FF E0 7C 08 02 A6 88 A4 00 2F

// opera.arc checks, used on later releases
// corresponds to the ignition factor
// 2C190000 40800010 7F23 CB78 4BFF EE0D 480000DC 3B 40 00 03
// part2
// 2C190000 40800010 7F23 CB78 4BFF EDC1 48000090 3B 40 00 03 8081 0008 7F65DB78

// opera.arc megaman x2
// 2C 14 00 00 40 80 00 10 7E 83 A3 78 4B FF EB E1 48 00 00 DC 3A
// 2C 14 00 00 40 80 00 10 7E 83 A3 78 4B FF EB 95 48 00 00 90 3A

//hbmse ignition
// 2C190000 40800010 7f23cb78 4BFF EF5D 480000DC //at 0x4A6C
// 2C190000 40800010 7f23cb78 4BFF EF11 48000090 //at 0x4AB8

//hbmse megaman x2
// 2C14 0000 40 80 00 10 7E 83 A3 78 4BFFED31 4800 00DC 3A
// 2C14 0000 40 80 00 10 7E 83 A3 78 4BFFECE5 4800 0090 3A

// types of ROM header checks, Ignition Factor
const uint8_t romheader_patch_1[5] = {
	0x90, 0xA6, 0x36, 0x98, 0x4B
};

// SFII Alpha
const uint8_t romheader_patch_2[5] = {
	0x90, 0xA6, 0x36, 0xA0, 0x4B
	//0x94, 0x21, 0xFF, 0xE0, 0x7C, 0x08, 0x02, 0xA6, 0x88, 0xA4, 0x00, 0x2F
};

// FFIII
const uint8_t romheader_patch_3[5] = {
	0x90, 0xA6, 0x37, 0x18, 0x4B
};

// Better romheader skips that maintain SRAM saving support
const uint8_t romheader_simple_ignition[5] = {
	0x3C, 0x9F, 0x00, 0x0E, 0x3C
};

const uint8_t romheader_simple_ffiii[5] = {
	0x3F, 0x7A, 0x00, 0x2E, 0x3F
};

// opera.arc, ignition factor
const uint8_t opera_patch_ignition_p1[21] = {
	0x2C, 0x19, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7F, 0x23, 0xCB, 0x78, 0x4B, 0xFF, 0xEE, 0x0D,
	0x48, 0x00, 0x00, 0xDC, 0x3B
};

const uint8_t opera_patch_ignition_p2[21] = {
	0x2C, 0x19, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7F, 0x23, 0xCB, 0x78, 0x4B, 0xFF, 0xED, 0xC1,
	0x48, 0x00, 0x00, 0x90, 0x3B
};

// opera.arc, Megaman X2, FFIII
const uint8_t opera_patch_megamanx2_p1[21] = {
	0x2C, 0x14, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7E, 0x83, 0xA3, 0x78, 0x4B, 0xFF, 0xEB, 0xE1,
	0x48, 0x00, 0x00, 0xDC, 0x3A
};

const uint8_t opera_patch_megamanx2_p2[21] = {
	0x2C, 0x14, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7E, 0x83, 0xA3, 0x78, 0x4B, 0xFF, 0xEB, 0x95,
	0x48, 0x00, 0x00, 0x90, 0x3A
};

// HBMSE.arc, Megaman X2, FFIII
const uint8_t hbmse_patch_megamanx2_p1[21] = {
	0x2C, 0x14, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7E, 0x83, 0xA3, 0x78, 0x4B, 0xFF, 0xED, 0x31,
	0x48, 0x00, 0x00, 0xDC, 0x3A
};

const uint8_t hbmse_patch_megamanx2_p2[21] = {
	0x2C, 0x14, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7E, 0x83, 0xA3, 0x78, 0x4B, 0xFF, 0xEC, 0xE5,
	0x48, 0x00, 0x00, 0x90, 0x3A
};

// HBMSE.arc, Ignition Factor
const uint8_t hbmse_patch_ignition_p1[20] = {
	0x2C, 0x19, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7f, 0x23, 0xcb, 0x78, 0x4B, 0xFF, 0xEF, 0x5D,
	0x48, 0x00, 0x00, 0xDC
};

const uint8_t hbmse_patch_ignition_p2[20] = {
	0x2C, 0x19, 0x00, 0x00, 0x40, 0x80, 0x00, 0x10, 0x7f, 0x23, 0xcb, 0x78, 0x4B, 0xFF, 0xEF, 0x11,
	0x48, 0x00, 0x00, 0x90
};

// Correct opera.arc removal
const uint8_t opera_patch[0xD] = {
	0x7F, 0xA3, 0xEB, 0x78, 0x38, 0x80, 0x00, 0x30, 0x38, 0xA0, 0x00, 0x00, 0x48
};

// Try to catch both hbmse.arc and opera.arc (hbmse cause dolphin errors)
const uint8_t tmp_skip[8] = {
	0x2C, 0x00, 0x00, 0x00, 0x40, 0x82, 0x02, 0xEC
};

// Widescreen correction, varies depending on release
const uint8_t widescreenHomeMenu_find[0xD] = {
	0x38, 0x00, 0x00, 0x00, 0x98, 0x0D, 0x86, 0xE8, 0x98, 0x0D, 0x86, 0xE9, 0x4B
};

const uint8_t widescreenHomeMenu_patch[0x10] = {
	0x60, 0x00, 0x00, 0x00, 0x48, 0x08, 0xC3, 0x19, 0x98, 0x6D, 0x86, 0xE8, 0x98, 0x6D, 0x86, 0xE9
};

// Alt for kirby super star
const uint8_t widescreenHomeMenu_patch_alt[0x10] = {
	0x60, 0x00, 0x00, 0x00, 0x48, 0x4C, 0x4A, 0x25, 0x98, 0x6D, 0x87, 0x88, 0x98, 0x6D, 0x87, 0x89
};

// Generic read sysconf aspect ratio value
const uint8_t conf_aspect[0xD] = {
	0x94, 0x21, 0xFF, 0xF0, 0x7C, 0x08, 0x02, 0xA6, 0x38, 0x80, 0x00, 0x01, 0x90
};

const uint8_t pixel_scale_1[9] = {
	0x38, 0x60, 0x02, 0x00, 0x38, 0x00, 0x01, 0x2C, 0xB0
};

const uint8_t pixel_scale_patch[9] = {
	0x38, 0x60, 0x02, 0x00, 0x38, 0x00, 0x01, 0x2C, 0xB0
};

// Original emulator volume
const uint8_t volume[4] = {
	0x38, 0x00, 0x00, 0xF8
};

// Dark filter Kirby Super Star
const uint8_t dark_kby[8] = {
	0xB3, 0xB3, 0xB3, 0xFF, 0x43, 0xD0, 0x00, 0x00
};

// Dark filter Final Fantasy II, Super Mario All-Stars, Secret of Mana
const uint8_t dark_ff[4] = {
	0x38, 0x00, 0x00, 0xE0
};

// Max white
const uint8_t max_white[4] = {
	0xFF, 0xFF, 0xFF, 0xFF
};

// Kirby's Dream Course is the first SNES title to use filter, but it randomly changes
const uint8_t dark_kbydc[0x14] = {
	0x88, 0xA1, 0x00, 0x12, 0x38, 0x60, 0x00, 0x02, 0x98, 0xE1, 0x00, 0x08, 0x98, 0xC1, 0x00, 0x09,
	0x98, 0xA1, 0x00, 0x0A
};

// Forces Kirby's Dream Course to always write 0xFFFFFFFF to the display code that sets the dark filter
const uint8_t kbydc_white[0xC] = {
	0x9B, 0x81, 0x00, 0x08, 0x9B, 0x81, 0x00, 0x09, 0x9B, 0x81, 0x00, 0x0A
};

// CC advertisement
const uint8_t cc_advertise[0x1D] = {
	0x54, 0x60, 0x04, 0x3E, 0x70, 0x00, 0x01, 0x10, 0x40, 0x82, 0x00, 0x18, 0x38, 0x60, 0x00, 0x02,
	0x38, 0x80, 0x00, 0x01, 0x38, 0xA0, 0x00, 0x01, 0x38, 0xC0, 0x00, 0x00, 0x4B
};

// Original / direct GC mappings (SMW / Super Metroid)
const uint8_t gc_map_1[0x5C] = {
	0x54, 0x60, 0x05, 0xef, 0x38, 0x80, 0x00, 0x00, 0x41, 0x82, 0x00, 0x08, 0x60, 0x84, 0x00, 0x80, 
	0x54, 0x60, 0x05, 0x6b, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x40, 0x54, 0x04, 0x06, 0x3e, 
	0x54, 0x60, 0x06, 0x73, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x20, 0x54, 0x04, 0x06, 0x3e, 
	0x54, 0x60, 0x06, 0xb5, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x10, 0x54, 0x04, 0x06, 0x3e, 
	0x7c, 0x83, 0x23, 0x78, 0x4e, 0x80, 0x00, 0x20, 0x54, 0x60, 0x05, 0xad, 0x38, 0x80, 0x00, 0x00, 
	0x41, 0x82, 0x00, 0x08, 0x60, 0x84, 0x00, 0x80, 0x54, 0x60, 0x05, 0x29
};

// Altered GC mappings
const uint8_t gc_map_1_alt[0x5C] = {
	0x54, 0x60, 0x05, 0x6b, 0x38, 0x80, 0x00, 0x00, 0x41, 0x82, 0x00, 0x08, 0x60, 0x84, 0x00, 0x80, 
	0x54, 0x60, 0x05, 0x29, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x40, 0x54, 0x04, 0x06, 0x3e, 
	0x54, 0x60, 0x06, 0x73, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x20, 0x54, 0x04, 0x06, 0x3e, 
	0x54, 0x60, 0x06, 0xb5, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x10, 0x54, 0x04, 0x06, 0x3e, 
	0x7c, 0x83, 0x23, 0x78, 0x4e, 0x80, 0x00, 0x20, 0x54, 0x60, 0x05, 0xef, 0x38, 0x80, 0x00, 0x00, 
	0x41, 0x82, 0x00, 0x08, 0x60, 0x84, 0x00, 0x80, 0x54, 0x60, 0x05, 0xad
};

// Original GC mappings type 2 part1
const uint8_t gc_map_2[0x1C] = {
	0x54, 0x80, 0x05, 0xef, 0x41, 0x82, 0x00, 0x08, 0x60, 0xa5, 0x00, 0x80, 0x54, 0x80, 0x05, 0x6b, 
	0x41, 0x82, 0x00, 0x0c, 0x60, 0xa0, 0x00, 0x40, 0x54, 0x05, 0x06, 0x3e
};

// Altered GC mappings type 2 part1
const uint8_t gc_map_2_alt[0x10] = {
	0x54, 0x80, 0x05, 0x6b, 0x41, 0x82, 0x00, 0x08, 0x60, 0xa5, 0x00, 0x80, 0x54, 0x80, 0x05, 0x29
};

// Original GC mappings type 2 part2
const uint8_t gc_map_2_p2[0x24] = {
	0x60, 0x84, 0x00, 0x80, 0x54, 0x60, 0x05, 0x29, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x40, 
	0x54, 0x04, 0x06, 0x3e, 0x54, 0x60, 0x04, 0x63, 0x41, 0x82, 0x00, 0x0c, 0x60, 0x80, 0x00, 0x20, 
	0x54, 0x04, 0x06, 0x3e
};

// Altered GC mappings type 2 part 2
const uint8_t gc_map_2_p2_alt[0x10] = {
	0x60, 0x84, 0x00, 0x40, 0x54, 0x60, 0x05, 0xEF, 0x41, 0x82, 0x00, 0x0C, 0x60, 0x80, 0x00, 0x80
};

const uint8_t wiimote_native[8] = {
	0x28, 0x00, 0x00, 0x01, 0x40, 0x82, 0x01, 0x2C //0x28 in others
};

// These wiimote hooks are for building a C2 code for wiimote support, 

// Wii Remote hook type 1
const uint8_t wiimoteHook[0x18] = {
	0x38, 0x04, 0x00, 0x04, 0x7C, 0x05, 0x00, 0x00, 0x40, 0x82, 0x00, 0x18, 0x3C, 0x7F, 0x00, 0x06,
	0x88, 0x9F, 0x00, 0x51, 0x38, 0x63, 0xF7, 0xEC
};

// Wii Remote hook type 2
const uint8_t wiimoteHook2[0x18] = {
	0x38, 0x04, 0x00, 0x04, 0x7C, 0x05, 0x00, 0x00, 0x40, 0x82, 0x00, 0x18, 0x3C, 0x7F, 0x00, 0x09,
	0x88, 0x9F, 0x00, 0x51, 0x38, 0x63, 0xC0, 0xD0
};

// Wii Remote hook type 3 - Used in FFII
const uint8_t wiimoteHook3[0x18] = {
	0x38, 0x05, 0x00, 0x04, 0x7C, 0x06, 0x00, 0x00, 0x40, 0x82, 0x00, 0x18, 0x3C, 0x63, 0x00, 0x06,
	0x88, 0x9F, 0x00, 0x51, 0x38, 0x63, 0xF7, 0xF0
};

uint16_t getSNS_native(const char* snesPad)
{
	uint16_t val = 0;
	
	if(strncasecmp(snesPad, "A", 1) == 0)
		val = 0x400;
	else if(strncasecmp(snesPad, "B", 1) == 0)
		val = 0x100;
	else if(strncasecmp(snesPad, "X", 1) == 0)
		val = 0x800;
	else if(strncasecmp(snesPad, "Y", 1) == 0)
		val = 0x200;
	else if(strncasecmp(snesPad, "Up", 2) == 0)
		val = 0x8;
	else if(strncasecmp(snesPad, "Down", 4) == 0)
		val = 0x4;
	else if(strncasecmp(snesPad, "Left", 4) == 0)
		val = 0x1;
	else if(strncasecmp(snesPad, "Right", 5) == 0)
		val = 0x2;
	else if(strncasecmp(snesPad, "Start", 5) == 0)
		val = 0x1000;
	else if(strncasecmp(snesPad, "Select", 6) == 0)
		val = 0x4000;
	else if(strncasecmp(snesPad, "L", 1) == 0)
		val = 0x40;
	else if(strncasecmp(snesPad, "R", 1) == 0)
		val = 0x20;
	
	return val;
}

uint16_t getSNS(const char* snesPad)
{
	uint16_t val = 0;
	
	if(strncasecmp(snesPad, "A", 1) == 0)
		val = 0x80;
	else if(strncasecmp(snesPad, "B", 1) == 0)
		val = 0x8000;
	else if(strncasecmp(snesPad, "X", 1) == 0)
		val = 0x40;
	else if(strncasecmp(snesPad, "Y", 1) == 0)
		val = 0x4000;
	else if(strncasecmp(snesPad, "Up", 2) == 0)
		val = 0x800;
	else if(strncasecmp(snesPad, "Down", 4) == 0)
		val = 0x400;
	else if(strncasecmp(snesPad, "Left", 4) == 0)
		val = 0x200;
	else if(strncasecmp(snesPad, "Right", 5) == 0)
		val = 0x100;
	else if(strncasecmp(snesPad, "Start", 5) == 0)
		val = 0x1000;
	else if(strncasecmp(snesPad, "Select", 6) == 0)
		val = 0x2000;
	else if(strncasecmp(snesPad, "L", 1) == 0)
		val = 0x20;
	else if(strncasecmp(snesPad, "R", 1) == 0)
		val = 0x10;
	
	return val;
}

int main (int argc, char *argv[])
{
  int i = 0;
  int x = 0;
  int fullLimit = 0;
  int halfLimit = 0;
  int halfLimit1 = 0; //for recording game string len
  int fullLimit1 = 0;
  bool lineBroke = false;
  int checksum = 0;

  printf("\n\n");
  printf("~ SNES VC Booster ~  by %s\n\n\n", name);
  
  int d = 0;
  int fileSz = 0;
  int txtSz = 0;
  
  // setting vars
  int boost_volume = 0;
  int no_dark = 0;
  int no_ccAd = 0;
  int gc_remap = 0;
  int wiimote = 0;
  int wiimoteNative = 0;
  int no_opera = 0;
  int no_hbmse = 0;
  int no_save = 0;
  int no_qsave = 0;
  int no_check = 0;
  int wideWant = 0;
  int pixel_perf = 0;
  bool verbose = false;
  
  bool useType3 = false; //for wiimote emu

  int workType = 1;
  FILE *fd = NULL;

  if(argc > 1) {
    for(i = 1; i < (argc); i++) {
            //need to handle special characters or commands
			
			if(strcmp(argv[i], "-i") == 0) {
				fd = fopen(argv[i+1], "rb");
				if(fd == NULL) {
					printf("Failed to open file!\nAre you sure this is correct?\n\n");
					break;
				} else {
					printf("Opened file for reading: %s\n\n", argv[i+1]);
					++workType;
				}
			}
			
			if(strcmp(argv[i], "--volume") == 0) {
				boost_volume = 1;
				printf("Game volume will increase...\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-dark") == 0) {
				no_dark = 1;
				printf("Game dark filter will be removed...\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-cc") == 0) {
				no_ccAd = 1;
				printf("\"You will need the Classic Controller.\" message will be removed...\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--gc-remap") == 0) {
				gc_remap = 1;
				printf("GameCube Controller's direct mapping changed to A=B, B=Y, Y=X, X=A...\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--wiimote-emu") == 0) {
				wiimote = 1;
				printf("Wii Remote will be used as controller...\n");
				printf("Wii Remote support requires inserting code,\n");
				printf("this requires an additional effort to find the correct addresses,\n");
				printf("therefore, this option will output a C2 code that can be applied to the DOL.\n");
				printf("Edit wii.txt to customize mapping.\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--wiimote-native") == 0) {
				wiimoteNative = 1;
				printf("Wii Remote will be used as controller...\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-opera") == 0) {
				no_opera = 1;
				printf("opera.arc will no longer be written...\n");
				
				//printf("This is useful when there is no e-manual.\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-hbmse") == 0) {
				no_hbmse = 1;
				printf("HBMSE.arc will no longer be written...\n");
				
				//printf("This prevents writing HBMSE.arc to /tmp every time the game is started.\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-header-check-all") == 0) {
				no_check = 1;
				printf("ROM header check will be patched for all checks, SRAM will stop working.\n");
				
				//printf("This allows more games to work on newer, LZH8 supporting builds.\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-header-check-simple") == 0) {
				no_check = 2;
				printf("ROM header check will be patched...\n");
				
				//printf("This allows more games to work on newer, LZH8 supporting builds.\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--wide") == 0) {
				wideWant = 1;
				printf("Forcing game to recognize widescreen setting...\n");
				
				//printf("This is useful when ya want widescreen Home Menu and no VI scaling in-game.\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--pixel-perfect") == 0 || strcmp(argv[i], "--pp") == 0) {
				pixel_perf = 1;
				printf("Forcing game to display 1:1...\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-save") == 0) {
				no_save = 1;
				printf("savedata.bin will no longer be written...\n");
			//	printf("This is useful when the base uses savedata but not the replaced game.\n");
			//	printf("Otherwise it creates an empty file, causing Data Management to allow copying.\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--no-qsave") == 0 || strcmp(argv[i], "--no-suspend") == 0) {
				no_qsave = 1;
				printf("qsave.bin will no longer be written...\n");
				
				++workType;
            }
			
			if(strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
				verbose = true;
				printf("Verbosity set...\n");
				
				++workType;
            }
			
            if(strcmp(argv[i], "/help") == 0) //print all commands and instructions
            {
                ++i;   //skip arg
                x = 0; //reset parse
                //printf("\n");
                printf("USAGE INSTRUCTIONS:\n");
                printf("sns_boost.exe -i 01.app --volume --no-dark --no-cc --gc-remap --wiimote-native\n\n");
                printf("This tool modifies SNES VC DOL files\n");
                printf("to increase volume, remove dark filter, remove opera.arc, savedata.bin, qsdata.bin writing,\n");
                printf("rotate the GC controller's ABXY and adding custom Wii Remote support.\n\n");
				
				printf("\"--volume\" = Maximize volume of the game.\n");
				printf("\"--no-dark\" = Tries to find a dark filter setting and remove it. (May break stuff!)\n");
				printf("\"--no-cc\" = Removes requirement of a GameCube or Classic Controller to start. ");
				printf("Also makes loading games faster.\n");
				printf("\"--gc-remap\" = Rotates the direct button mapping on GC controllers ");
				printf("to make Super Mario World truly playable.\n");
				printf("\"--wiimote-emu\" = Opens wii.txt to build a C2 code that allows playing with JUST a Wii Remote.\n");
				printf("\"--wiimote-native\" = Enables native Wii Remote support.\n");
				printf("\"--wide\" = Restores widescreen detection for the HOME Menu.\n");
				printf("\"--no-opera\" = Prevents writing opera.arc to NAND, for games that have no e-manuals.\n");
				printf("\"--no-hbmse\" = Prevents writing HBMSE.arc to NAND.\n");
				printf("\"--no-save\" = Prevents writing savedata.bin, for custom games that have no save.\n");
				printf("\"--no-suspend\" = Prevents writing qsdata.bin, games will always start fresh.\n");
				printf("\"--no-header-check\" = Allows using later releases as base with ROM compression.\n\n");
				
				printf("NOTE: \"--no-save\", \"--no-suspend\" may only work on earlier titles.\n\n");
                //printf("\n");
				break;
            }
			
			if(workType > 2 && (i > (argc-2))) {
				// get file size
				fseek(fd, 0, SEEK_END);
				fileSz = ftell(fd);
				
				// read the file into mem
				fseek(fd, 0, SEEK_SET);
				uint8_t* datBuf = malloc(fileSz);
				fread(datBuf, 1, fileSz, fd);
				fclose(fd);
				
				// Verify app is not compressed
				if(datBuf[0] == 0x11 || datBuf[0] == 0x10) {
					printf("This DOL is compressed! Decompress it first!\n\n");
					free(datBuf);
					break;
				}
				
				printf("DOL size: 0x%X\n\n", fileSz);
				
				//printf("WHAT\n");
				
				// Kirby's dark filter is baked into the display list at the end of the dol
				bool isKirbySS = false;
				if(fileSz > 0x56601F) {
					if(strcmp((const char*)datBuf+0x4F101E, "Kirby Super Star") == 0)
						isKirbySS = true;
				}
				
				if(isKirbySS) {
					datBuf[0x8E68] = 0x48; // patch the nand block check, geez
					datBuf[0x8E69] = 0x00;
					datBuf[0x8E6A] = 0x00;
					datBuf[0x8E6B] = 0x3C;
					printf("Removed NAND block size check.\n");
					
					// SRAM patch
					
				}
				
				// Handle regular patches
				int qsave = 0;
				int p = 0;
				for(p = 0; p < fileSz; ++p) {
					// Begin the magic
					
					if(boost_volume && memcmp(&datBuf[p], volume, 4) == 0) {
						boost_volume = 0;
						datBuf[p+2] = 2;
						
						if(verbose)
							printf("Adjusted volume to maximum! 0x%X\n", p);
						else
							printf("Adjusted volume to maximum!\n");
					}
					else if(no_dark && memcmp(&datBuf[p], dark_kby, 8) == 0) {
						no_dark = 0;
						memcpy(&datBuf[p], max_white, 4);
						
						printf("Removed dark filter! Type: 30%% black, 0xB3B3B3 whites - Kirby Super Star ");
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(!isKirbySS && no_dark && memcmp(&datBuf[p], dark_ff, 4) == 0) {
						no_dark = 0;
						datBuf[p+3] = 0xFF;
						printf("Removed dark filter! Type: 12%% black, 0xE0E0E0 whites - Final Fantasy II, Secret of Mana, ");
						printf("Super Mario All-Stars ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(no_dark && memcmp(&datBuf[p], dark_kbydc, 0x14) == 0) {
						no_dark = 0;
						// TODO: test search on JPN/PAL/KOR, instead of hardcoding address to 0x6618 (NTSC-U only)
						memcpy(&datBuf[p+8], kbydc_white, 0xC);
						
						printf("Removed dark filter! Type: dynamic, Kirby's Dream Course ");
						if(verbose)
							printf("0x%X\n", p+8);
						else
							printf("\n");
					}
					else if(no_ccAd && memcmp(&datBuf[p], cc_advertise, 0x1D) == 0) {
						no_ccAd = 0;
						
						// "Press the A Button to return to the Wii Menu." screen removal
						datBuf[p+0x1C] = 0x60;
						datBuf[p+0x1D] = 0x00;
						datBuf[p+0x1E] = 0x00;
						datBuf[p+0x1F] = 0x00;
						
						// "You will need the Classic Controller." artificial delay removal
						datBuf[p-0x14] = 0x60;
						datBuf[p-0x13] = 0x00;
						datBuf[p-0x12] = 0x00;
						datBuf[p-0x11] = 0x00;
						
						printf("Removed CC advertisement! ");
						
						if(verbose)
							printf("0x%X, 0x%X\n", p+0x1C, p-0x14);
						else
							printf("\n");
					}
					else if(gc_remap && memcmp(&datBuf[p], gc_map_1, 0x5C) == 0) {
						gc_remap = 0;
						memcpy(&datBuf[p], gc_map_1_alt, 0x5C);
						printf("Found GC mappings type 1 (Super Mario World, Super Metroid) affects P1 and P2.\n");
						printf("Adjusted GC mappings. ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(gc_remap && memcmp(&datBuf[p], gc_map_2, 0x1C) == 0) {
						// Remove this line to apply multiple times
						//gc_remap = 0;
						//--gc_remap;
						
						memcpy(&datBuf[p], gc_map_2_alt, 0x10);
						//printf("Found GC mappings type 2 (FFII, Chrono Trigger)\n");
						printf("Adjusted GC mappings. 2/2 ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(gc_remap && memcmp(&datBuf[p], gc_map_2_p2, 0x24) == 0) {
						gc_remap = 5; // apply 5 times, for patch 2/2 no idea why it's 5 instead of 4
						memcpy(&datBuf[p], gc_map_2_p2_alt, 0x10);
						printf("Found GC mappings type 2 (FFII, Chrono Trigger)\n");
						printf("Adjusted GC mappings. 1/2 ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if((no_check == 1) && memcmp(&datBuf[p], romheader_patch_1, 0x5) == 0) {
						no_check = 0;
						datBuf[p+4] = 0x60;
						datBuf[p+5] = 0x00;
						datBuf[p+6] = 0x00;
						datBuf[p+7] = 0x00;
						printf("Found and removed ROM header check. Type 1, Ignition Factor ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if((no_check == 1) && memcmp(&datBuf[p], romheader_patch_2, 0x5) == 0) {
						no_check = 0;
						datBuf[p+4] = 0x60;
						datBuf[p+5] = 0x00;
						datBuf[p+6] = 0x00;
						datBuf[p+7] = 0x00;
						printf("Found and removed ROM header check. Type 2, SFII Alpha ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if((no_check == 1) && memcmp(&datBuf[p], romheader_patch_3, 0x5) == 0) {
						no_check = 0;
						datBuf[p+4] = 0x60;
						datBuf[p+5] = 0x00;
						datBuf[p+6] = 0x00;
						datBuf[p+7] = 0x00;
						printf("Found and removed ROM header check. Type 3, FFIII ");
						
						// At 0xA760 there is a loop that needs to be removed for the game to start
						if(datBuf[0xA760] == 0x48 && datBuf[0xA763] == 0)
							datBuf[0xA760] = 0x60;
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if((no_check == 2) && memcmp(&datBuf[p], romheader_simple_ignition, 5) == 0) {
						// Confirm it
						//printf("Found simple header check...\n");
						
						// Verify
						if(datBuf[p-4] == 0x4B) {
							// Loop to find blr
							uint16_t b = 0;
							for(b = 0; b < 0x500; b+=4) {
								if(datBuf[p+b] == 0x4E && datBuf[p+b+3] == 0x20) {
									b -= 0x10;
									datBuf[p] = 0x48;
									datBuf[p+1] = 0x00;
									datBuf[p+2] = b >> 8;
									datBuf[p+3] = b;
									no_check = 0;
									//printf("Show me: 0x%X %X,, ", b >> 8, (uint8_t)b);
									break;
								}
							}
							printf("Found and removed ROM header check. Type SRAM: Ignition Factor ");
						}
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if((no_check == 2) && memcmp(&datBuf[p], romheader_simple_ffiii, 5) == 0) {
						// Confirm it
						//printf("Found simple header check...\n");
						
						// Verify
						if(datBuf[p-4] == 0x4B) {
							// Loop to find blr
							uint16_t b = 0;
							for(b = 0; b < 0x500; b+=4) {
								if(datBuf[p+b] == 0x4E && datBuf[p+b+3] == 0x20) {
									b -= 0x10;
									datBuf[p] = 0x48;
									datBuf[p+1] = 0x00;
									datBuf[p+2] = b >> 8;
									datBuf[p+3] = b;
									no_check = 0;
									//printf("Show me: 0x%X %X,, ", b >> 8, (uint8_t)b);
									break;
								}
							}
							printf("Found and removed ROM header check. Type SRAM: FFIII ");
						}
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(no_opera && memcmp(&datBuf[p], opera_patch_ignition_p1, 21) == 0) {
					//	no_opera = 0;
						datBuf[p+0xC] = 0x60;
						datBuf[p+0xD] = 0x00;
						datBuf[p+0xE] = 0x00;
						datBuf[p+0xF] = 0x00;
						
						printf("Found and removed opera.arc check 1 ");
						
						if(memcmp(&datBuf[p+0x4C], opera_patch_ignition_p2, 21) == 0) {
							datBuf[p+0x4C+0xC] = 0x60;
							datBuf[p+0x4C+0xD] = 0x00;
							datBuf[p+0x4C+0xE] = 0x00;
							datBuf[p+0x4C+0xF] = 0x00;
							printf("and check 2 ");
						}
						
						printf("of type 1, Ignition Factor ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(no_opera && memcmp(&datBuf[p], opera_patch_megamanx2_p1, 21) == 0) {
					//	no_opera = 0;
						datBuf[p+0xC] = 0x60;
						datBuf[p+0xD] = 0x00;
						datBuf[p+0xE] = 0x00;
						datBuf[p+0xF] = 0x00;
						
						printf("Found and removed opera.arc check 1 ");
						
						if(memcmp(&datBuf[p+0x4C], opera_patch_megamanx2_p2, 21) == 0) { // higher than 14 fails
							datBuf[p+0x4C+0xC] = 0x60;
							datBuf[p+0x4C+0xD] = 0x00;
							datBuf[p+0x4C+0xE] = 0x00;
							datBuf[p+0x4C+0xF] = 0x00;
							printf("and check 2 ");
						}
						
						printf("of type 2, MegaMan X2, FFIII ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(no_hbmse && memcmp(&datBuf[p], hbmse_patch_megamanx2_p1, 21) == 0) {
					//	no_hbmse = 0;
						datBuf[p+0xC] = 0x60;
						datBuf[p+0xD] = 0x00;
						datBuf[p+0xE] = 0x00;
						datBuf[p+0xF] = 0x00;
						
						printf("Found and removed HBMSE.arc check 1 ");
						
						if(memcmp(&datBuf[p+0x4C], hbmse_patch_megamanx2_p2, 21) == 0) {
							datBuf[p+0x4C+0xC] = 0x60;
							datBuf[p+0x4C+0xD] = 0x00;
							datBuf[p+0x4C+0xE] = 0x00;
							datBuf[p+0x4C+0xF] = 0x00;
							printf("and check 2 ");
						}
						
						printf("of type 2, MegaMan X2, FFIII ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(no_hbmse && memcmp(&datBuf[p], hbmse_patch_ignition_p1, 20) == 0) {
					//	no_hbmse = 0;
						datBuf[p+0xC] = 0x60;
						datBuf[p+0xD] = 0x00;
						datBuf[p+0xE] = 0x00;
						datBuf[p+0xF] = 0x00;
						
						printf("Found and removed HBMSE.arc check 1 ");
						
						if(memcmp(&datBuf[p+0x4C], hbmse_patch_ignition_p2, 20) == 0) {
							datBuf[p+0x4C+0xC] = 0x60;
							datBuf[p+0x4C+0xD] = 0x00;
							datBuf[p+0x4C+0xE] = 0x00;
							datBuf[p+0x4C+0xF] = 0x00;
							printf("and check 2 ");
						}
						
						printf("of type 2, Ignition Factor ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(no_opera && memcmp(&datBuf[p], opera_patch, 0xD) == 0) {
						no_opera = 0;
						datBuf[p] = 0x48;
						datBuf[p+1] = 0x00;
						datBuf[p+2] = 0x01;
						datBuf[p+3] = 0x20;
						printf("Removed opera.arc!\n");
					}
					else if(wideWant && memcmp(&datBuf[p], widescreenHomeMenu_find, 6) == 0) {
					//	wideWant = 0;
						if(datBuf[p-4] == 0x48 && datBuf[p+8] == 0x98 && (datBuf[p+6] == 0x86 || datBuf[p+6] == 0x87)) {
							// copy the bl
							unsigned bakBL = 0;
							memcpy(&bakBL, &datBuf[p-4], 4);
							
							uint8_t bakSTB_bot = datBuf[p+7];
							uint8_t bakSTB_top = datBuf[p+5];
							
							// Correct to BE
							bakBL = ((bakBL>>24)&0xff)      | // move byte 3 to byte 0
									((bakBL<<8) &0xff0000)  | // move byte 1 to byte 2
									((bakBL>>8) &0xff00)    | // move byte 2 to byte 1
									((bakBL<<24)&0xff000000); // byte 0 to byte 3
							
							if(datBuf[p+6] == 0x87)
								memcpy(&datBuf[p-4], widescreenHomeMenu_patch_alt, 0x10);
							else
								memcpy(&datBuf[p-4], widescreenHomeMenu_patch, 0x10);
							
							// Since a li has been no-op'd move the bl closer
							bakBL -= 4;
							
							// Restore LE
							bakBL = ((bakBL>>24)&0xff)      | // move byte 3 to byte 0
									((bakBL<<8) &0xff0000)  | // move byte 1 to byte 2
									((bakBL>>8) &0xff00)    | // move byte 2 to byte 1
									((bakBL<<24)&0xff000000); // byte 0 to byte 3
							
							memcpy(&datBuf[p], &bakBL, 4);
							//printf("Show me: 0x%X\n", bakBL);
							
							// adjust r13
							datBuf[p+0x7] = bakSTB_bot;
							datBuf[p+0xB] = bakSTB_bot+1;
							
							datBuf[p+0x5] = bakSTB_top;
							datBuf[p+0x9] = bakSTB_top;
							
							printf("Applied widescreen checking! Type 1 HOME Menu, Ignition Factor, FFIII ");
							
							if(verbose)
								printf("0x%X\n", p-4);
							else
								printf("\n");
						}
					}
				#if 0
					else if(wideWant && memcmp(&datBuf[p], conf_aspect, sizeof(conf_aspect)) == 0) {
					//	wideWant = 0;
					//	memcpy(&datBuf[p], ???, sizeof());
						
						// TODO
						if(datBuf[p+0x3C] == 0x98) {
							// branch, OG instr 98010008
							
							
							printf("Found widescreen checking function! Injected new code.\n");
						}
					}
				#endif
				#if 0 // obvious trouble doing this: can't inject ASM easily
					else if(pixel_perf && memcmp(&datBuf[p], pixel_scale_1, sizeof(pixel_scale_1)) == 0) {
					//	pixel_perf = 0;
						memcpy(&datBuf[p], pixel_scale_patch, sizeof(pixel_scale_patch));
						printf("Applied pixel-perfect patch! Type 1 HOME Menu, Ignition Factor\n");
					}
				#endif
				#if 0
					else if(pixel_perf && memcmp(&datBuf[p], find_video, sizeof(find_video)) == 0) {
						if(datBuf[p-0x2C] == 0 && datBuf[p-0x26] == 0x28 && datBuf[p-0x22] == 0x80) {
							datBuf[p-0x26] = 0x68;
							datBuf[p-0x22] = 0x00;
							printf("Applied pixel-perfect patch! Type 1, Ignition Factor\n");
						}
					//	pixel_perf = 0;
					}
				#endif
				#if 0 // apparently crashes on real hardware
					else if(no_opera && strcmp((const char*)datBuf+p, "/tmp/opera.arc") == 0) {
						no_opera = 0;
						datBuf[p+5] = 0;
						printf("Removed opera.arc!\n");
					}
				#endif
				#if 1
					else if(no_hbmse && strcmp((const char*)datBuf+p, "/tmp/HBMSE.arc") == 0) {
						no_hbmse = 0;
						datBuf[p+5] = 0;
						printf("Removed HBMSE.arc path!\n");
					}
				#endif
					else if(no_save && strcmp((const char*)datBuf+p, "/tmp/savedata.bin") == 0) {
						no_save = 0;
						datBuf[p+5] = 0;
						printf("Removed SRAM creation!\n");
					}
					else if(no_qsave && strcmp((const char*)datBuf+p, "nocopy/qsdata.bin") == 0) {
						qsave = 1;
						datBuf[p+7] = 0;
					}
					else if((qsave == 1) && strcmp((const char*)datBuf+p, "nocopy/dummy") == 0) {
						qsave = 2;
						no_qsave = 0;
						datBuf[p+7] = 0;
						printf("Removed suspend feature!\n");
					}
					else if((qsave == 2) && strcmp((const char*)datBuf+p, "/tmp/dummy") == 0) {
						qsave = 0;
						no_qsave = 0;
						datBuf[p+5] = 0;
						printf("Removed suspend feature! "); // Kirby Super Star has this
						printf("This may trigger a \"files corrupt\" screen!\n");
					}
					else if((wiimote == 1) && memcmp(&datBuf[p], wiimoteHook, 0x18) == 0) {
						wiimote = p;
						printf("Found type 1 hook for Wii Remote. ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if((wiimote == 1) && memcmp(&datBuf[p], wiimoteHook2, 0xD) == 0) {
						wiimote = p;
						printf("Found type 2 hook for Wii Remote. ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if((wiimote == 1) && memcmp(&datBuf[p], wiimoteHook3, 0xD) == 0) {
						wiimote = p;
						printf("Found type 3 hook for Wii Remote. ");
						
						useType3 = true;
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
					}
					else if(wiimoteNative && memcmp(&datBuf[p], wiimote_native, 0x7) == 0) {
						wiimoteNative = 0;
						wiimote = 0;
						
						// Decide type
						uint8_t skip = 0;
						if(datBuf[p+7] == 0x2C)
							skip = 0x30;
						else if(datBuf[p+7] == 0x28)
							skip = 0x2C;
						else if(datBuf[p+7] == 0x18) // F-Zero
							skip = 0x1C;
						if(skip == 0)
							printf("Failed to decide type to patch...\n");
						
						datBuf[p] = 0x60;
						datBuf[p+3] = 0;
						datBuf[p+4] = 0x60;
						datBuf[p+5] = 0;
						datBuf[p+6] = 0;
						datBuf[p+7] = 0;
						printf("Enabled Wii Remote. ");
						
						if(verbose)
							printf("0x%X\n", p);
						else
							printf("\n");
						
						// Open wii.txt for custom mappings
						uint16_t snesBtn = 0;
						fd = fopen("wii.txt", "rb");
						if(fd == NULL) {
							printf("Failed to open wii.txt! Wii Remote will use defaults.\n\n");
						} else if(skip > 0) {
							// get file size
							fseek(fd, 0, SEEK_END);
							txtSz = ftell(fd);
							
							// read the file into mem
							fseek(fd, 0, SEEK_SET);
							uint8_t* txtBuf = malloc(txtSz);
							fread(txtBuf, 1, txtSz, fd);
							fclose(fd);
							
							// Too soon to celebrate
							printf("Remapped Wii Remote. ");
							if(verbose)
								printf("0x%X\n", skip);
							else
								printf("\n");
							
							// Look for and set mappings
							int j = 0;
							for(j = 0; j < txtSz; ++j) {
								if(strncmp((const char*)txtBuf+j, "Minus=", 6) == 0) {
									snesBtn = getSNS_native(txtBuf+j+6);
									
									datBuf[p+skip+2] = snesBtn >> 8;
									datBuf[p+skip+3] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "Plus=", 5) == 0) {
									snesBtn = getSNS_native(txtBuf+j+5);
									
									datBuf[p+skip+0x1A] = snesBtn >> 8;
									datBuf[p+skip+0x1B] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "A=", 2) == 0) {
									snesBtn = getSNS_native(txtBuf+j+2);
									
									datBuf[p+skip+0x32] = snesBtn >> 8;
									datBuf[p+skip+0x33] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "1=", 2) == 0) {
									snesBtn = getSNS_native(txtBuf+j+2);
									
									datBuf[p+skip+0x4A] = snesBtn >> 8;
									datBuf[p+skip+0x4B] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "2=", 2) == 0) {
									snesBtn = getSNS_native(txtBuf+j+2);
									
									datBuf[p+skip+0x62] = snesBtn >> 8;
									datBuf[p+skip+0x63] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "B=", 2) == 0) {
									snesBtn = getSNS_native(txtBuf+j+2);
									
									datBuf[p+skip+0x7A] = snesBtn >> 8;
									datBuf[p+skip+0x7B] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "Right=", 6) == 0) {
									snesBtn = getSNS_native(txtBuf+j+6);
								
									datBuf[p+skip+0x92] = snesBtn >> 8;
									datBuf[p+skip+0x93] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "Left=", 5) == 0) {
									snesBtn = getSNS_native(txtBuf+j+5);
									
									datBuf[p+skip+0xAA] = snesBtn >> 8;
									datBuf[p+skip+0xAB] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "Up=", 3) == 0) {
									snesBtn = getSNS_native(txtBuf+j+3);
								
									datBuf[p+skip+0xC2] = snesBtn >> 8;
									datBuf[p+skip+0xC3] = snesBtn;
								}
								else if(strncmp((const char*)txtBuf+j, "Down=", 5) == 0) {
									snesBtn = getSNS_native(txtBuf+j+5);
									
									datBuf[p+skip+0xDA] = snesBtn >> 8;
									datBuf[p+skip+0xDB] = snesBtn;
								}
							}
						}
					}
				}
				
				// Wiimote handle
				if(wiimote > 1) {
					// Open the text file
					fd = fopen("wii.txt", "rb");
					if(fd == NULL) {
						printf("Failed to open wii.txt!\nWii Remote support will not work.\n\n");
					} else {
						//printf("Opened file for reading: %s\n\n", argv[i+1]);
						
						// get file size
						fseek(fd, 0, SEEK_END);
						txtSz = ftell(fd);
						
						// read the file into mem
						fseek(fd, 0, SEEK_SET);
						uint8_t* txtBuf = malloc(txtSz);
						fread(txtBuf, 1, txtSz, fd);
						fclose(fd);
						
						// start building the C2 code
						fd = fopen("wiimote_code.txt", "w");
						
						fprintf(fd, "%s\n", decIntro);
						
						uint8_t val1 = datBuf[4];
						uint8_t val2 = datBuf[5];
						uint8_t val3 = datBuf[6];
						uint8_t val4 = datBuf[7];
						uint32_t dolOffset = val4 | val3 << 8 | val2 << 16 | val1 << 24;
						
						val1 = datBuf[0x4C];
						val2 = datBuf[0x4D];
						val3 = datBuf[0x4E];
						val4 = datBuf[0x4F];
						uint32_t memBase = val4 | val3 << 8 | val2 << 16 | val1 << 24;
						
						memBase += (wiimote - dolOffset);
						
						// 33 assumes all P1 buttons on Wii remote are mapped
					//	fprintf(fd, "C2%06X 00000033\n", memBase & 0xFFFFFF);
						//printf("Show me MEM1 hook: 0x%X\n", memBase);
						
						// Search until init strings are found
						bool writeCode = false;
						bool firstLine = true;
						unsigned asmLines = 1;
						unsigned wiiP1 = 0;
						unsigned wiiP2 = 0;
						unsigned snesP1 = 0;
						unsigned snesP2 = 0;
						uint16_t topWii = 0;
						uint16_t botWii = 0;
						uint16_t topSNS = 0;
						uint16_t botSNS = 0;
						uint16_t wiiBtn = 0;
						uint16_t snesBtn = 0;
						for(p = 0; p < txtSz; ++p) {
							if(strncmp((const char*)txtBuf+p, "wiiP1: ", 7) == 0) {
								wiiP1 = strtoll(&txtBuf[p+7], NULL, 16);
								
								// split address
								topWii = wiiP1 >> 16;
								botWii = wiiP1;
								
								// Assume P2
							//	wiiP2 = wiiP2 + 0x2A; // Might not be accurate
								
								//printf("Wii P1: 0x%04X\n", topWii);
							}
							if(strncmp((const char*)txtBuf+p, "wiiP2: ", 7) == 0) {
								wiiP2 = strtoll(&txtBuf[p+7], NULL, 16);
								
								//wiiP2 = wiiP2 + 0x2A; // Might not be accurate
							//	printf("Wii P2: 0x%X\n", wiiP2);
							}
							if(strncmp((const char*)txtBuf+p, "snesP1: ", 8) == 0) {
								snesP1 = strtoll(&txtBuf[p+8], NULL, 16);
								
								topSNS = snesP1 >> 16;
								botSNS = snesP1;
								
							//	snesP2 += 2;
								
								//printf("SNES P1: 0x%X\n", snesP1);
							}
							if(strncmp((const char*)txtBuf+p, "snesP2: ", 8) == 0) {
								snesP2 = strtoll(&txtBuf[p+8], NULL, 16);
								
								//printf("SNES P2: 0x%X\n", snesP2);
							}
							if(strncmp((const char*)txtBuf+p, "snesP2: P1", 10) == 0) {
								botSNS += 2;
								wiiP2 = 0;
								//printf("SNES P2: 0x%X\n", snesP2);
							}
							
							// Find the mappings
							if(strncmp((const char*)txtBuf+p, "1=", 2) == 0) {
								snesBtn = getSNS(txtBuf+p+2);
								wiiBtn = 0x200;
								
								if(snesBtn > 0)
									writeCode = true;
								//printf("Wii BTN is: 0x%X\n", snesBtn);
							}
							else if(strncmp((const char*)txtBuf+p, "2=", 2) == 0) {
								snesBtn = getSNS(txtBuf+p+2);
								wiiBtn = 0x100;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "A=", 2) == 0) {
								snesBtn = getSNS(txtBuf+p+2);
								wiiBtn = 0x800;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "B=", 2) == 0) {
								snesBtn = getSNS(txtBuf+p+2);
								wiiBtn = 0x400;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "Up=", 3) == 0) {
								snesBtn = getSNS(txtBuf+p+3);
								wiiBtn = 0x8;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "Down=", 5) == 0) {
								snesBtn = getSNS(txtBuf+p+5);
								wiiBtn = 0x4;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "Left=", 5) == 0) {
								snesBtn = getSNS(txtBuf+p+5);
								wiiBtn = 0x1;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "Right=", 6) == 0) {
								snesBtn = getSNS(txtBuf+p+6);
								wiiBtn = 0x2;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "Plus=", 5) == 0) {
								snesBtn = getSNS(txtBuf+p+5);
								wiiBtn = 0x10;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							else if(strncmp((const char*)txtBuf+p, "Minus=", 6) == 0) {
								snesBtn = getSNS(txtBuf+p+6);
								wiiBtn = 0x1000;
								
								if(snesBtn > 0)
									writeCode = true;
							}
							
							// Decide on P2
							if(firstLine && writeCode) {
								firstLine = false;
								fprintf(fd, "C2%06X 000000%02X\n", memBase & 0xFFFFFF, wiiP2 > 0 ? 0x65 : 0x33);
							}
							
							// Write code if needed
							if(writeCode) {
								fprintf(fd,
							"3D20%04X 6129%04X\nA0090000 7009%04X\n41820018 3D20%04X\n6129%04X A0090000\n6000%04X B0090000\n",
									topWii, botWii, wiiBtn, topSNS, botSNS, snesBtn);
							//	writeCode = false;
								asmLines += 5;
							}
							if(writeCode && wiiP2) {
								// Check whether wiimote should be P1 or P2
								
								fprintf(fd,
							"3D20%04X 6129%04X\nA0090000 7009%04X\n41820018 3D20%04X\n6129%04X A0090000\n6000%04X B0090000\n",
									topWii, (uint16_t)wiiP2, wiiBtn, topSNS, botSNS+2, snesBtn);
							//	writeCode = false;
								asmLines += 5;
							}
							writeCode = false;
						}
						// original instruction
						if(useType3)
							fprintf(fd, "38050004 00000000\n");
						else
							fprintf(fd, "38040004 00000000\n");
						
						fprintf(fd, "\n\nUpdate first line size: 0x%X\n", asmLines);
						
					//	FILE* fout = fopen("wii_check.txt", "wb");
					//	fwrite(txtBuf, 1, txtSz, fout);
					//	fclose(fout);
						free(txtBuf);
						fclose(fd);
					}
				}
				
			//	printf("Show me: %s,,", &datBuf[0x1357B4]);
				
			//	int allLines = CountAllLines(datBuf, fileSz);
			//	int outSize = TweakData(datBuf, 480, fileSz, allLines, outBuf);
				
				// write file
			#if 1
				FILE* fout = fopen("01_boosted.app", "wb");
				fwrite(datBuf, 1, fileSz, fout);
				fclose(fout);
			#endif
				free(datBuf);
				
				printf("\n");
				printf("Done!\n");
				
				break;
			} //else {
				//printf("No command selected...\n");
			//}
		}
  }
  else
  {
     printf("No arguments.\n");
     printf("Use /help to list usage instructions.\n");
  }

  printf("\n");

  return 0;
}
