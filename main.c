#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
  int x, y, time;
  bool hit;
} note;

int main(void) {
  InitWindow(640, 480, "amk");
  InitAudioDevice();
  Music music = LoadMusicStream("bm/audio.mp3");
  Sound hit_sound = LoadSound("bm/drum-hitnormal.wav");
  PlayMusicStream(music);
  FILE* file = fopen("bm/notes.osu", "r");
  char line[128];
  note notes[1024];
  int num_notes = 0;
  while(fgets(line, sizeof line, file)) {
    int x, y, time;
    if(sscanf(line, "%d,%d,%d", &x, &y, &time) == 3) {
      note* n = &notes[num_notes++];
      n->x = x;
      n->y = y;
      n->time = time;
      n->hit = false;
    }
  }
  fclose(file);

  SetTargetFPS(60);
  float approach_time = 0.5;
  while(!WindowShouldClose()) {
    bool press = IsKeyPressed(KEY_Z);
    bool hit = false;

    UpdateMusicStream(music);
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("good morning chat", 10, 10, 24, BLACK);
    float time = GetMusicTimePlayed(music);
    for(int i = 0; i < num_notes; ++i) {
      note* n = &notes[i];
      if(n->hit) {
        continue;
      }

      float dt = time - n->time * 1e-3;
      float ratio = fabs(dt) / approach_time;
      if(ratio > 1) {
        continue;
      }

      int x = n->x;
      int y = n->y;
      DrawCircle(x, y, (1 - ratio) * 50, RED);
      DrawCircleLines(x, y, 50, RED);

      if(!IsKeyPressed(KEY_Z) && !IsKeyPressed(KEY_X) || hit) {
        continue;
      }

      int dx = x - GetMouseX();
      int dy = y - GetMouseY();
      if(dx*dx+dy*dy<50*50) {
        n->hit = true;
        hit = true;
        printf("%f\n", (double)dt);
        PlaySound(hit_sound);
      }
    }

    EndDrawing();
  }

  UnloadMusicStream(music);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
