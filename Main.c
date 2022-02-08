#include "../Library/MainInclude.h"
#include "OKInclude.h"

#if OverKartBuild
void loadLogo()
{
	SetSegment(8,(int)(&ok_Logo));
	*sourceAddress = (int)(&LogoROM);
	*targetAddress = (int)(&ok_FreeSpace);
	dataLength = 0x38AC;
	runDMA();
	*sourceAddress = (int)(&ok_FreeSpace);
	*targetAddress = (int)(&ok_Logo);
	runMIO();
	g_NintendoLogoOffset = 0x08005A70;
	g_NintendoLogoBorder = 0x256B9478;

	
	
}
#endif



short FocusValue = 2;


void loadCoinSprite()
{
	*sourceAddress = (int)(&RCSpriteROM);
	*targetAddress = (int)(&ok_FreeSpace);
	dataLength = 0x100;
	runDMA();
	*sourceAddress = (int)(&ok_FreeSpace);
	*targetAddress = (int)(&ok_menucoinsprite);
	runMIO();
}

void loadNumberSprites()
{
	*sourceAddress = (int)(&NumbersSpriteROM);
	*targetAddress = (int)(&ok_FreeSpace);
	dataLength = 0x470;
	runDMA();
	*sourceAddress = (int)(&ok_FreeSpace);
	*targetAddress = (int)(&lit_numberSprite);
	runMIO();
}

void okSetup(void)
{

	SaveFunc800B45E0 = 0x03E0000800000000;
	SaveFunc800B4670 = 0x03E0000800000000;
	SaveFunc800B559C = 0x03E0000800000000;
	SaveFunc800B5948 = 0x03E0000800000000;
	LoadFunc800B4A10 = 0x03E0000800000000;
	
	ConsolePlatform = CheckPlatform();
	EmulatorPlatform = CheckEmulator();	
	
	loadHeaderOffsets();

	if (SwopCheck == 0x11342067)
	{
		StopSwop = true;
	}
	else
	{
		StopSwop = false;		
	}
	SwopCheck = 0x11342067;
	
	
	*(short*)(0x800DC6FE) = 0xFFFF;

	
	loadEEPROM((uint)&SaveGame);	
	
	if ((GlobalController[0]->ButtonHeld & BTN_L) == BTN_L)
	{
		if ((GlobalController[0]->ButtonHeld & BTN_DDOWN) == BTN_DDOWN)
		{	
			SaveGame.GameSettings.GameMode = 2;
			SaveGame.ModSettings.PracticeMode = 2;			
		}
	}

	
	dataLength = 8;
	*sourceAddress = (long)((long)(&g_SequenceTable) + (3 * 8) + 4);	
	*targetAddress = (long)&ok_Sequence;
	runRAM();

	*sourceAddress = (long)((long)(&g_InstrumentTable) + (3 * 8) + 4);
	*targetAddress = (long)&ok_Instrument;
	runRAM();

	*sourceAddress = (int)&g_BombTable;
	*targetAddress = (long)&ok_Bomb;	
	dataLength = 0xA8;
	runRAM();
	
	#if OverKartBuild
	loadLogo();
	#endif
	copyCourseTable(1);
	NopSplashCheckCode();
	
	startupSwitch = 1;
	nopASM = 0;
	HotSwapID = 0;
	asm_SongA = 0x240E0001;
	asm_SongB = 0x240E0001;
	g_sfxPause = 0; //Just for Amped Up (To fix some mute sfx when pausing the game)
	hsGP = 0;
	
	ok_Knowledge = (long)(&ok_Target);

	/*
	*targetAddress = (long)&BackupPortraitTable;
	*sourceAddress = (long)&SelectPortraitTable;
	dataLength = 288;
	runRAM();
	
	asm_PlayerSelectController = 0;
	//asm_PlayerSelectDoObjectP5 = 0;
	//asm_PlayerSelectDoObjectP6 = 0;
	//asm_PlayerSelectDoObjectP7 = 0;
	//asm_PlayerSelectDoObjectP8 = 0;
	asm_DispOBSubPSelCursor1[0] = 0x800A08C8;
	asm_DispOBSubPSelCursor1[1] = 0x800A08C8;
	asm_DispOBSubPSelCursor1[2] = 0x800A08C8;
	asm_DispOBSubPSelCursor1[3] = 0x800A08C8;
	//asm_DOBPSelTurnIn = 0;
	//asm_DOBPSelTurnOut = 0;
	
	PlayerSelectPositions[0].y -= 10;
	PlayerSelectPositions[1].y -= 10;
	PlayerSelectPositions[2].y -= 10;
	PlayerSelectPositions[3].y -= 10;
	
	*targetAddress = (long)&BackupNamePlateTable;
	*sourceAddress = (long)&SelectNamePlateTable;
	dataLength = 32;
	runRAM();
	*/
	
	*(long*)(&ok_USAudio) = *(long*)(&g_RawAudio + 1);
	*(long*)(&ok_USAudio + 1) = *(long*)(&g_InstrumentTable + 1);


	*(long*)(&ok_MRSong) = *(long*)(&g_SequenceTable + (3 * 2) + 1);
	*(long*)(&ok_MRSong + 1) = *(long*)(&g_InstrumentTable + (3 * 2) + 1);

	
	
	FreeSpaceAddress = (int)&ok_Storage;
	SaveGame.TENNES = false;
}

bool checkEndGame()
{
	if (g_playerCount == 0x01)
	{
		if (g_playerPosition1 <= 3)
		{
			return true;
		}
		else if (SaveGame.GameSettings.GPMode == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ((g_playerPosition1 <= 3) || (g_playerPosition2 <= 3))
		{
			return true;
		}
		else if (SaveGame.GameSettings.GPMode == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}



void startRace()
{
	
	g_loadedcourseFlag = 0xF0;

	
	
	if (HotSwapID > 0)
	{
		if (g_gameMode != 0)
		{
			gpCourseIndex = 0;
		}
		
		if (g_gameMode != 3)
		{
			EmptyActionData();
		
			setSky();
			setWater();
			loadMinimap();				
		}

		if ((SaveGame.GameSettings.GameMode == 2))
		{        	
			RedCoinChallenge(GetRealAddress(0x060009D8));
			CoinCount[0] = 0;
			CoinCount[1] = 0;
			CoinCount[2] = 0;
			CoinCount[3] = 0;
		}
		if ((SaveGame.GameSettings.GameMode == 3))
		{        	
			GoldCoinChallenge(GetRealAddress(pathOffset));
			CoinCount[0] = 0;
			CoinCount[1] = 0;
			CoinCount[2] = 0;
			CoinCount[3] = 0;
			CoinCount[4] = 0;
			CoinCount[5] = 0;
			CoinCount[6] = 0;
			CoinCount[7] = 0;
		}
		
		if ((VersionNumber > 4) && (g_gameMode != 3))
		{
			for (int This = 0; This < 100; This++)
			{
				ClearOKObject(This);
			}
			loadOKObjects();
			setOKObjects();					
			loadTextureScrollTranslucent();
		}
	}
	

}

void endRace()
{
	if (g_gameMode == 0x00)
	{
		if (checkEndGame())
		{
			scrollLock = false;
		}
		if (!scrollLock)
		{	
			gpTotalTime += g_TrialTime;
			gpCourseIndex++;
		}
	}
}

void MapStartup(short InputID)
{
	LoadCustomHeader(courseValue + gpCourseIndex);
	SetCustomData();
	LoadMapData(InputID);
	setPath();
}
void InitialMapCode()
{
	
	InitialMap();
	
	if ((HotSwapID > 0) && (g_gameMode == 3))
	{
		SearchListFile(0x06000000 | OverKartHeader.SurfaceMapPosition);
		MakeCollision();
	}
}


void DrawPerScreen(Camera* LocalCamera)
{
	DrawOKObjects(LocalCamera);
	
}


void gameCode(void)
{	
	
	if(SaveGame.TENNES == 1)
	{
		KWSpriteDiv(256,120,(ushort*)&Pirate,512,240,4);
	}
	else
	{

		if ((GlobalController[0]->ButtonPressed & BTN_DRIGHT) == BTN_DRIGHT)
		{
			FocusValue++;
		}
		if ((GlobalController[0]->ButtonPressed & BTN_DLEFT) == BTN_DLEFT)
		{
			FocusValue--;
		}
		if (g_startingIndicator > 1)
		{
			GlobalPlayer[1].position[0] = GlobalPlayer[0].position[0];
			GlobalPlayer[1].position[1] = GlobalPlayer[0].position[1] + 15;
			GlobalPlayer[1].position[2] = GlobalPlayer[0].position[2];
			GlobalPlayer[1].direction[1] = GlobalPlayer[0].direction[1];
			GlobalPlayer[1].radius = 0;
			GlobalPlayer[1].offsetsize = 0;
			GlobalPlayer[1].bump.bump_xy[0] = GlobalPlayer[0].bump.bump_xy[0];
			GlobalPlayer[1].bump.bump_xy[1] = GlobalPlayer[0].bump.bump_xy[1];
			GlobalPlayer[1].bump.bump_xy[2] = GlobalPlayer[0].bump.bump_xy[2];
			GlobalPlayer[1].bump.bump_yz[0] = GlobalPlayer[0].bump.bump_yz[0];
			GlobalPlayer[1].bump.bump_yz[1] = GlobalPlayer[0].bump.bump_yz[1];
			GlobalPlayer[1].bump.bump_yz[2] = GlobalPlayer[0].bump.bump_yz[2];
			GlobalPlayer[1].bump.bump_zx[0] = GlobalPlayer[0].bump.bump_zx[0];
			GlobalPlayer[1].bump.bump_zx[1] = GlobalPlayer[0].bump.bump_zx[1];
			GlobalPlayer[1].bump.bump_zx[2] = GlobalPlayer[0].bump.bump_zx[2];
			GlobalPlayer[1].bump.last_xy = GlobalPlayer[0].bump.last_xy;
			GlobalPlayer[1].bump.last_yz = GlobalPlayer[0].bump.last_yz;
			GlobalPlayer[1].bump.last_zx = GlobalPlayer[0].bump.last_zx;
			GlobalPlayer[1].bump.flag_xy = GlobalPlayer[0].bump.flag_xy;
			GlobalPlayer[1].bump.flag_yz = GlobalPlayer[0].bump.flag_yz;
			GlobalPlayer[1].bump.flag_zx = GlobalPlayer[0].bump.flag_zx;
			g_zoomFOVPlayer2 = g_zoomFOVPlayer1;
			g_zoomLevelPlayer2 = g_zoomLevelPlayer1;


		
			objectPosition[0] = GlobalPlayer[0].position[0];
			objectPosition[1] = GlobalPlayer[0].position[1];
			objectPosition[2] = GlobalPlayer[0].position[2];

			objectVelocity[0] = FocusValue * -1;
			objectVelocity[1] = 10;
			objectVelocity[2] = -70;
			MakeAlignVector(objectVelocity,GlobalPlayer[0].direction[1]);

			GlobalCamera[0]->camera_pos[0] = objectPosition[0] + (objectVelocity[0]);
			GlobalCamera[0]->camera_pos[1] = objectPosition[1] + (objectVelocity[1]);
			GlobalCamera[0]->camera_pos[2] = objectPosition[2] + (objectVelocity[2]);

			
			objectVelocity[0] = FocusValue;			
			objectVelocity[1] = 10;
			objectVelocity[2] = -70;
			MakeAlignVector(objectVelocity,GlobalPlayer[0].direction[1]);

			GlobalCamera[1]->camera_pos[0] = objectPosition[0] + (objectVelocity[0]);
			GlobalCamera[1]->camera_pos[1] = objectPosition[1] + (objectVelocity[1]);
			GlobalCamera[1]->camera_pos[2] = objectPosition[2] + (objectVelocity[2]);

			GlobalCamera[1]->camera_direction[0] = GlobalCamera[0]->camera_direction[0];
			GlobalCamera[1]->camera_direction[1] = GlobalCamera[0]->camera_direction[1];
			GlobalCamera[1]->camera_direction[2] = GlobalCamera[0]->camera_direction[2];

			GlobalCamera[1]->camera_vector[0] = GlobalCamera[0]->camera_vector[0];
			GlobalCamera[1]->camera_vector[1] = GlobalCamera[0]->camera_vector[1];
			GlobalCamera[1]->camera_vector[2] = GlobalCamera[0]->camera_vector[2];

			objectVelocity[0] = 0;
			objectVelocity[1] = 0;
			objectVelocity[2] = 50;
			MakeAlignVector(objectVelocity,GlobalPlayer[0].direction[1]);

			GlobalCamera[1]->lookat_pos[0] = GlobalPlayer[0].position[0] + objectVelocity[0];
			GlobalCamera[1]->lookat_pos[1] = GlobalPlayer[0].position[1] + objectVelocity[1];
			GlobalCamera[1]->lookat_pos[2] = GlobalPlayer[0].position[2] + objectVelocity[2];
			GlobalCamera[0]->lookat_pos[0] = GlobalPlayer[0].position[0] + objectVelocity[0];
			GlobalCamera[0]->lookat_pos[1] = GlobalPlayer[0].position[1] + objectVelocity[1];
			GlobalCamera[0]->lookat_pos[2] = GlobalPlayer[0].position[2] + objectVelocity[2];

			GlobalCamera[1]->lookat_vector[0] = GlobalCamera[0]->lookat_vector[0];
			GlobalCamera[1]->lookat_vector[1] = GlobalCamera[0]->lookat_vector[1];
			GlobalCamera[1]->lookat_vector[2] = GlobalCamera[0]->lookat_vector[2];
		
			GlobalCamera[1]->chase_direction = GlobalCamera[0]->chase_direction;
			GlobalCamera[1]->old_chase_direction = GlobalCamera[0]->old_chase_direction;
			GlobalCamera[1]->screen_view_angle = GlobalCamera[0]->screen_view_angle;
			GlobalCamera[1]->flag = GlobalCamera[0]->flag;

			GlobalCamera[1]->watch = GlobalCamera[0]->watch;
			

			
		}



		
	}
}
void resetMap()
{
	gpTotalTime = 0;
	gpCourseIndex = 0;
}



//
//
//
void allRun(void)
{
	
	switch (startupSwitch)
	{
		case 0:
		okSetup();
		break;

		case 1:
		break;
		
		case 2:
		break;
	}
	
	
	
	
	gMatrixCount = 0;
	
	g_ScreenSplitA = 2;
	g_ScreenSplitB = 2;
	g_playerCount = 2;
	player2OK = 1;


}
void PrintMenuFunction()
{
	#if ProtectMode
	if(SaveGame.TENNES == 1)
	{		
		if (SaveGame.SaveVersion != 99)
		{
			SaveGame.SaveVersion = 99;
			*sourceAddress = (int)(&Pirate);
			*targetAddress = (int)(&ok_FreeSpace);
			dataLength = (int)&PirateEnd - (int)&Pirate;
			runDMA();
			*sourceAddress = (int)(&ok_FreeSpace);
			*targetAddress = (int)(&ok_Storage);
			runMIO();
		}
		KWSpriteDiv(256,120,(ushort*)&ok_Storage,512,240,4);
	}
	#endif
}

void DisplayCrashScreen()
{
	*sourceAddress = (int)(&Crash);
	*targetAddress = *(uint*)0x80162D5C;
	dataLength = (int)&CrashEnd - (int)&Crash;
	runDMA();
}


void XLUDisplay(Screen* PlayerScreen)
{	
	if ((OverKartHeader.Version > 4) && (HotSwapID > 0))
	{	
		if (g_gameMode != 3)
		{
			DisplayGroupmap(GetRealAddress(SegmentAddress(6,OverKartHeader.XLUSectionViewPosition)), PlayerScreen);
		}
		else
		{
			*(long*)*graphPointer = (long)(0x06000000);
			*graphPointer = *graphPointer + 4;
			*(long*)*graphPointer = (long)(SegmentAddress(6,OverKartHeader.XLUSectionViewPosition));
			*graphPointer = *graphPointer + 4;
		}
	}
}



void titleMenu()
{

	return;
}