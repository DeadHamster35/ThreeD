#ifndef OK64STRUCT_H_
#define OK64STRUCT_H_



typedef struct GameOptions{
     char GameMode, StatsMode, AudioMode, MirrorMode, GPMode, CupMode, AIMode, PAD; //8
} GameOptions;

typedef struct ModOptions{
     char PracticeMode, FlycamMode, InputMode, DetailMode, ItemMode, PAD, PAD2, PAD3; //8
} ModOptions;

typedef struct RenderOptions{
     char ScreenMode, AliasMode, SplitMode, DrawMode, TempoMode, PAD, PAD2, PAD3;
} RenderOptions;

typedef struct SaveData{
     GameOptions    GameSettings;
     ModOptions     ModSettings;
     RenderOptions  RenderSettings;     
     bool           TENNES;
     char           SaveVersion;
     char           PAD[478];
} SaveData;

#endif