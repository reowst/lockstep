#include <OpenGL/gl.h>
#include "lib/assert.h"
#include "render_commands.h"
#include "opengl.h"

static const r32 Zoom = 1.0 / 1000.0;
static const r32 SquareHalfSize = 100.0f / 2.0f;

void InitOpenGL(r32 AspectRatio) {
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  r32 a = 1.0f*Zoom;
  r32 b = 1.0f*AspectRatio*Zoom;
  r32 Proj[] = {
    a,  0,  0,  0,
    0,  b,  0,  0,
    0,  0,  1,  0,
    0,  0,  0,  1,
  };
  glLoadMatrixf(Proj);
}

void DrawSquare(si16 X, si16 Y, ui32 Color) {
  ui8 R = Color >> 16;
  ui8 G = (Color & 0x0000FF00) >> 8;
  ui8 B = Color & 0x000000FF;
  glColor3ub(R, G, B);
  r32 RX = (r32)X;
  r32 RY = (r32)Y;
  glRectf(
    RX - SquareHalfSize, RY - SquareHalfSize,
    RX + SquareHalfSize, RY + SquareHalfSize
  );
}

void DisplayOpenGL(chunk_list *Commands) {
  glClear(GL_COLOR_BUFFER_BIT);

  for(;;) {
    buffer Command = ChunkListRead(Commands);
    if(Command.Length == 0) {
      break;
    }
    render_command_type Type = *(render_command_type*)(Command.Addr);
    void *Body = (ui8*)Command.Addr + sizeof(render_command_type);
    switch(Type) {
      case render_command_type_draw_square: {
        draw_square_render_command *DrawCommand = (draw_square_render_command*)Body;
        DrawSquare(DrawCommand->X, DrawCommand->Y, DrawCommand->Color);
        break;
      }
      default:
        InvalidCodePath;
    }
  }
}