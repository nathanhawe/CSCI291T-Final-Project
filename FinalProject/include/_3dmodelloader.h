#ifndef _3dmodelloader_H
#define _3dmodelloader_H

#include <_Common.h>
#include <_textureloader.h>

#define ACTION_COUNT 8

#define LANE0_X -0.1
#define LANE1_X 0
#define LANE2_X 0.1

/* Vector */
typedef float vec3_t[3];

/* MD2 header */
struct md2_header_t
{
  int ident;
  int version;

  int skinwidth;
  int skinheight;

  int framesize;

  int num_skins;
  int num_vertices;
  int num_st;
  int num_tris;
  int num_glcmds;
  int num_frames;

  int offset_skins;
  int offset_st;
  int offset_tris;
  int offset_frames;
  int offset_glcmds;
  int offset_end;
};

/* Texture name */
struct md2_skin_t
{
  char name[64];
};

/* Texture coords */
struct md2_texCoord_t
{
  short s;
  short t;
};

/* Triangle info */
struct md2_triangle_t
{
  unsigned short vertex[3];
  unsigned short st[3];
};

/* Compressed vertex */
struct md2_vertex_t
{
  unsigned char v[3];
  unsigned char normalIndex;
};

/* Model frame */
struct md2_frame_t
{
  vec3_t scale;
  vec3_t translate;
  char name[16];
  struct md2_vertex_t *verts;
};

/* GL command packet */
struct md2_glcmd_t
{
  float s;
  float t;
  int index;
};

/* MD2 model structure */
struct md2_model_t
{
  struct md2_header_t header;

  struct md2_skin_t *skins;
  struct md2_texCoord_t *texcoords;
  struct md2_triangle_t *triangles;
  struct md2_frame_t *frames;
  int *glcmds;

  GLuint tex_id;
};

/* Animation Frames */
struct frame_range
{
    int startFrame;
    int endFrame;
};

/* Table of precalculated normals */


class _3dmodelloader
{
    public:/* Table of precalculated normals */

        _3dmodelloader();
        ~_3dmodelloader();
          GLfloat s, t;
          int i, *pglcmds;
          vec3_t anorms_table[162] = {
            #include "Anorms.h"
            };

          _textureLoader *myTex = new _textureLoader();
          vec3_t v_curr, v_next, v, norm;
          float *n_curr, *n_next;
          struct md2_frame_t *pframe, *pframe1, *pframe2;
          struct md2_vertex_t *pvert, *pvert1, *pvert2;
          struct md2_glcmd_t *packet;
          struct md2_model_t md2file;

          int ReadMD2Model (const char *filename, struct md2_model_t *mdl);
          void RenderFrame (int n, const struct md2_model_t *mdl);
          void RenderFrameItpWithGLCmds (int n, float interp, const struct md2_model_t *mdl);
          void Animate (int start, int end, int *frame, float *interp);
          void initModel(const char *filename, char* filenameSkin);
          void Draw();
          void FreeModel (struct md2_model_t *mdl);
          void SetActionFrameRange(unsigned int action, int startFrame, int endFrame);
          void SetLane(int lane);
          int GetLane();
          void ShiftLeft();
          void ShiftRight();

          enum {STAND, RUN, JUMP, ATTACK, PAIN, DEATH, TAUNT, SALUTE};
          int actionTrigger =1;
          int StartFrame =0;
          int EndFrame =39;

          void actions();
          vec3 pos;
          float dirAngleZ=180;

          int n = 0; /* The current frame */
          float interp = 0.0;
          double curent_time = 0;
          double last_time = 0;

          frame_range action_frames[ACTION_COUNT];
          int debugId;

          bool printFrames = false;

	private:
	    int currentLane = 1;

    protected:
};

#endif // _3dmodelloader_H
