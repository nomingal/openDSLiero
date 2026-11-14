#include <nds.h>
#include <cstdio>
#ifndef UUID_DC1D9513CDD34960AB8A648004DA149D
#define UUID_DC1D9513CDD34960AB8A648004DA149D

#include <SDL/SDL.h>
#if !SDL13
#include <SDL/SDL_getenv.h>
#endif

#include "gfx.hpp"
#include "sfx.hpp"
#include "sys.hpp"
#include "game.hpp"
#include "viewport.hpp"
#include "worm.hpp"
#include "reader.hpp"
#include "filesystem.hpp"
#include "text.hpp"
#include "keys.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "console.hpp"
#include <gvl/support/platform.hpp>

#include <ctime>
#include <exception>
#include <gvl/math/cmwc.hpp>

#if __APPLE__
#define gameEntry SDL_main
#endif

volatile int g_boot_step = 0;
int gameEntry(int argc, char* argv[])
try
{
	// TODO: Better PRNG seeding
	gfx.rand.seed(Uint32(std::time(0)));
	
	bool exeSet = false;
	gvl::shared_ptr<Common> common(new Common);
	gfx.common = common;
	
	for(int i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
			case 'v':
				// SDL_putenv seems to take char* in linux, STOOPID
				SDL_putenv(const_cast<char*>((std::string("SDL_VIDEODRIVER=") + &argv[i][2]).c_str()));
			break;
			}
		}
		else
		{
			setLieroEXE(argv[i]);
			exeSet = true;
		}
	}
	
	if(!exeSet)
		setLieroEXE("LIERO.EXE");

	g_boot_step = 1; iprintf("1. SDL_Init\n"); g_boot_step = 1; iprintf("1. SDL_Init\n"); SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	
	g_boot_step = 2; iprintf("2. texts\n"); g_boot_step = 2; iprintf("2. texts\n"); common->texts.loadFromEXE();

	initKeys();
	g_boot_step = 3; iprintf("3. constants\n"); g_boot_step = 3; iprintf("3. constants\n"); common->loadConstantsFromEXE();
	g_boot_step = 4; iprintf("4. tables\n"); g_boot_step = 4; iprintf("4. tables\n"); loadTablesFromEXE();

	g_boot_step = 5; iprintf("5. font\n"); g_boot_step = 5; iprintf("5. font\n"); common->font.loadFromEXE();
	g_boot_step = 6; iprintf("6. palette\n"); g_boot_step = 6; iprintf("6. palette\n"); common->loadPalette();
	gfx.loadPalette(*common); // This gets the palette from common
	g_boot_step = 7; iprintf("7. menus\n"); g_boot_step = 7; iprintf("7. menus\n"); gfx.loadMenus();
	g_boot_step = 8; iprintf("8. gfx\n"); g_boot_step = 8; iprintf("8. gfx\n"); common->loadGfx();
	g_boot_step = 9; iprintf("9. materials\n"); g_boot_step = 9; iprintf("9. materials\n"); common->loadMaterials();
	g_boot_step = 10; iprintf("10. weapons\n"); g_boot_step = 10; iprintf("10. weapons\n"); common->loadWeapons();
	g_boot_step = 11; iprintf("11. textures\n"); g_boot_step = 11; iprintf("11. textures\n"); common->loadTextures();
	g_boot_step = 12; iprintf("12. others\n"); g_boot_step = 12; iprintf("12. others\n"); common->loadOthers();

	g_boot_step = 13; iprintf("13. sfx\n"); g_boot_step = 13; iprintf("13. sfx\n"); common->loadSfx();

	g_boot_step = 14; iprintf("14. gfx.init\n"); g_boot_step = 14; iprintf("14. gfx.init\n"); gfx.init();
	
	if(!gfx.loadSettings(joinPath(lieroEXERoot, "LIERO")))
	{
		gfx.settings.reset(new Settings);
		gfx.saveSettings(joinPath(lieroEXERoot, "LIERO"));
	}
	
	if (gfx.settings && gfx.settings->wormSettings[1]) {
        gfx.settings->wormSettings[1]->controller = 1;
        gfx.settings->selectBotWeapons = 0;
    }
    clearFileCache();
        g_boot_step = 15; iprintf("15. setVideoMode\n"); clearFileCache();
        g_boot_step = 15; iprintf("15. setVideoMode\n"); gfx.setVideoMode();
	g_boot_step = 16; iprintf("16. sfx.init\n"); g_boot_step = 16; iprintf("16. sfx.init\n"); sfx.init();
	
	g_boot_step = 17; iprintf("17. mainLoop\n"); g_boot_step = 17; iprintf("17. mainLoop\n"); gfx.mainLoop();
	
	gfx.settings->save(joinPath(lieroEXERoot, "LIERO.DAT"), gfx.rand);
	
	sfx.deinit();
	SDL_Quit();
	
	//gvl::present_profile(std::cout);
	
	return 0;
}
catch(std::exception&)
{
	SDL_Quit();
	throw;
}

#endif // UUID_DC1D9513CDD34960AB8A648004DA149D
