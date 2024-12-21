# SNES VC Booster


This is a commandline program that can search through the 01.app of a SNES/SFC VC title applying various patches to improve the user experience.


# Basic usage

Place a DOL file in the same folder as SNES VC Booster, rename it to 01.app, and start run_sns_boost.bat to apply common patches.

It will output 01_boosted.app, which you can then compress to save space or leave as is. You can adjust run_sns_boost.bat like a text file to change options.

Edit wii.txt to change Wii Remote mappings. You can also manually set patches on the commandline.


# Patches

--volume = Increases emulation volume from 0xF8(248) to 0x2F8(760), the exact limit is much lower.

--no-dark = Removes dark filter if it exists.

--no-cc = Removes Classic Controller advertisement and makes loading games faster.

--gc-remap = Rotates the GameCube controller's ABXY so many games are more comfortable to play. (Super Mario World)

--wiimote-emu = Creates a C2 code based on wii.txt to allow Wii Remotes to control the game.

--wiimote-native = Enables original Wii Remote support. Applies remap only if wii.txt exists.

--no-opera = Prevent writing opera.arc to tmp/, useful if no e-manual.

--no-hbmse = Prevent writing HBMSE.arc to tmp/, may crash game.

--no-save = Prevent writing savedata.bin, useful if game has no save but is on a base that does.

--no-suspend = Always start game fresh. (Later titles may crash when exiting.)

--wide = Restores Home Menu widescreen detection. (Early titles like SMW don't have this code finished.)

--no-header-check-all = Remove all ROM checks, breaks SRAM, allows replacing games.

--no-header-check-simple = Only remove ROM header checks, less compatible but allows saving SRAM.

/help = Displays all of this info.



# Wii Remote Support (Native Method)

When trying to add support for wiimotes I noticed Super Return of the Jedi still required a GC or CC connected to work with my patch, I traced back the function that checks if the controller is connected and there it was, a blocked off function that enables wiimotes to control the game. Every title I've tried seems to have it.

The default mappings are not super ideal, so this app will try to open wii.txt to set new mappings.

The original method may still be useful for other situations, for example making wiimote P1 work as P2. N64 VC may also benefit from this method, since it's very simple: VC still reads the wiimote for the HOME button, find this address, read the input, convert it to SNES input and write it to the SNES' input address.


# Wii Remote Support (Original Method)

Consider passing --no-cc to allow the title to start without forcing you to connect a CC or GameCube controller.

Open up wii.txt and review:

"wiiP1" = the address to read wii remote input from.

"snesP1" = the address to write SNES input to.

SNES P2 is usually 2 bytes after P1. Set this to NONE or 0 if the game is single player only, to halve code size.

To locate Wii Remote addresses (there are various) use Dolphin, start the game and bring up the cheat search, set it to find 16-bit values, and hold A on the remote (pause the emulation if needed) look for 0x0800, let go of the button and search for 0, repeat until you see pressing buttons on the remote relate to the results.

If you have a real wiimote connected to Dolphin tilt it to see if the value in the results change, if it changes DON'T use that one, skip ahead to the next one.

To locate the SNES input address do the same but with a GC or CC, hold start, and search for 0x1000, (obviously you can use other buttons with different values) once you have determined you have all button addresses look for the ones at the end, and press A or B, notice they will change differently when compared to using the Start button. There should be 2 addresses like this, use the first one.

Once you've set this info and your preferred mappings in wii.txt, run the program and it will output wiimote_code.txt containing the finished code.

Excuse the madness, I've included some finished codes in the 'other' folder, so this can be tested quickly on Super Mario World.
But remember, changing the ROM or PCM files will make these codes incompatible.

What about Secret of Mana?
SOM has 3 player support, I don't support this for wiimotes yet, but you can force a wiimote to control P3 by adding 4 to the P1 SNES address.

I refer to this as an emulator because this translates wiimote inputs to GameCube inputs, but note that you still need a GC controller plugged in for this to work, another reason why the native method is preferred.


# Replacing a ROM

Later titles -- ones that store the ROM compressed, had several checks to make sure the ROM was the correct one.

The checks are: header comparing against a dozen or so bytes, the NAND block size used by the 05.app (or the compressed ROM?), the "write save" flag must also match the game.

Passing --no-check-header-all will skip the function that checks for all of this, it should be present on all titles because this is also where the header is checked to determine if SRAM will be saved.

Avoid passing it needlessly. Because this breaks SRAM saving an alternative --no-check-header-simple exists for Ignition Factor, FFIII and Kirby Super Star.

Even though Ignition Factor and FFIII were chosen because one has SRAM and the other doesn't, enabling SRAM and removing it is possible to do when editing the 'savefile title' in the DOL the following bytes you'll find before the next text string is the number of blocks the 05.app takes, followed by whether savedata.bin should be created (4) or not (3, any other value, really).

Creating a patch for the "nand block size" check should be possible but I would rather not go there. If you can't calculate it, Dolphin tells you the right value via OSReport.



# Nandloader Alternative
I wanted to support widescreen correction as seen in Super Mario All-Stars and Brawl's Masterpieces, this works by setting VI scaling off, however, in order to support both 4:3 and 16:9 I needed to insert a lot of code. To skip this step and prevent many titles from being incompatible, I opted for a nandloader replacement.

By using a fork of 'nandloader' by noahpistilli, the wiipax tool from the hbc repo, and code from the taiko nandloader I was able to make the most ideal nandloader for SNES VC.

It comes in 3 versions:

*nandloader_vanilla.app = Just loads the game, takes a bit less space than the others.

*nandloader_pixelperfect.app = Applies the VI scaling removal patch from USB Loader GX, it goes through the first 2MB of MEM1 to find the code.

*nandloader_wide.app = Same as above but only if the Wii is set to widescreen, mimicking the widescreen correction from Super Mario All-Stars.

All 3 have the benefits of: saving space (usually 320KB->65KB), loading games with patched-in cheats, supporting LZ10/11 on the main dol, and vWii support.



# Known Issues:

As there are over 70 SNES VC titles, I only tested about 10 titles. An incompatible title can usually be fixed if reported.
The patches to remove suspending and savefile creation are only compatible with certain titles. Applying them may trigger a "files corrupted" message. Always test on Dolphin first.


Wii Remote "emu" support builds a C2 gecko code from info taken from the DOL, however, the location of Wii Remote inputs and SNES inputs are dynamically put in memory depending on various factors, therefore this info needs to be searched for manually in Dolphin or with a usb gecko.
This address will shift if the ROM or PCM sizes are changed, so it's not enough to include a list of all VC games.

This will not improve in the future as native support exists without requiring a GC controller connected.


Some titles use an unknown method of applying the dark filter.
I've noticed "Super Return of the Jedi", "Axelay", "Wild Guns", and "Super E.D.F." have it, JPN titles "Panel de Pon" and "FE Seisen no Keifu" have it.

For these games the only solution I know of is to increase their brightness with codes based on the ones included in the 'other' folder.
Using a separate base for these games would also work, but it seems Return of the Jedi's input doesn't work on other bases.


The ROM header patch has only been tested on MegaMan X2, The Ignition Factor, and FFIII -- specifically chosen since MMX2 allows X3 to be playable, and the others are good bases to work with. These later titles have LZH8 support for loading the ROM so they are preferred to save space.

In early titles, simply invalidating the paths for save/suspend/hbmse.arc is enough, for newer titles they are checked, I have a couple of patches in place but not all titles will work with them!


Compressed .app/.dol files are not supported and will need to be decompressed first.


# Credits

The code used for removing VI scaling is taken from: https://github.com/wiidev/usbloadergx
The nandloader I chose to apply one-time patches: https://github.com/OpenShopChannel/nandloader
WiiPAX (the actual nandloader): https://github.com/fail0verflow/hbc/tree/master/wiipax
The channel loading patches and LZ decompression: https://github.com/LemonBoy/taiko

The gecko code for increasing brightness in Super ROTJ using vfilter is based on the 'Removing Deflicker' patch from USB Loader GX. Instead of removing it I just increase the middle coef until whites are 0xFFFFFF.
The idea of using vfilter to increase brightness is from Extrems, though the context was for something else.
