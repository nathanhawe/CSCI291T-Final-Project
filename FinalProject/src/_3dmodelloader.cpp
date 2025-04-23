#include "_3dmodelloader.h"

_3dmodelloader::_3dmodelloader()
{
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;

    rotation.x = -90.0;
    rotation.y = 0;
    rotation.z = 0;

    scale.x = 1;
    scale.y = 1;
    scale.z = 1;

    pathStep = -1;
    hitCount = 0;
    maxHits = 4;

}

_3dmodelloader::~_3dmodelloader()
{
    FreeModel(&md2file);
}

void _3dmodelloader::FreeModel(struct md2_model_t* mdl)
{

  if (mdl->skins)
    {
      free (mdl->skins);
      mdl->skins = NULL;
    }

  if (mdl->texcoords)
    {
      free (mdl->texcoords);
      mdl->texcoords = NULL;
    }

  if (mdl->triangles)
    {
      free (mdl->triangles);
      mdl->triangles = NULL;
    }

  if (mdl->glcmds)
    {
      free (mdl->glcmds);
      mdl->glcmds = NULL;
    }

  if (mdl->frames)
    {

      for (int i = 0; i < mdl->header.num_frames; ++i)
	{
	  free (mdl->frames[i].verts);
	  mdl->frames[i].verts = NULL;
	}

      free (mdl->frames);
      mdl->frames = NULL;
    }
}


int _3dmodelloader::ReadMD2Model(const char* filename, struct md2_model_t* mdl)
{
   FILE *fp;

  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "Error: couldn't open \"%s\"!\n", filename);
      return 0;
    }

  /* Read header */
  fread (&mdl->header, 1, sizeof (struct md2_header_t), fp);

  if ((mdl->header.ident != 844121161) ||
      (mdl->header.version != 8))
    {
      /* Error! */
      fprintf (stderr, "Error: bad version or identifier\n");
      fclose (fp);
      return 0;
    }

  /* Memory allocations */
  mdl->skins = (struct md2_skin_t *)malloc (sizeof (struct md2_skin_t) * mdl->header.num_skins);
  mdl->texcoords = (struct md2_texCoord_t *)malloc (sizeof (struct md2_texCoord_t) * mdl->header.num_st);
  mdl->triangles = (struct md2_triangle_t *)malloc (sizeof (struct md2_triangle_t) * mdl->header.num_tris);
  mdl->frames = (struct md2_frame_t *)malloc (sizeof (struct md2_frame_t) * mdl->header.num_frames);
  mdl->glcmds = (int *)malloc (sizeof (int) * mdl->header.num_glcmds);

  /* Read model data */
  fseek (fp, mdl->header.offset_skins, SEEK_SET);
  fread (mdl->skins, sizeof (struct md2_skin_t),mdl->header.num_skins, fp);

  fseek (fp, mdl->header.offset_st, SEEK_SET);
  fread (mdl->texcoords, sizeof (struct md2_texCoord_t),mdl->header.num_st, fp);

  fseek (fp, mdl->header.offset_tris, SEEK_SET);
  fread (mdl->triangles, sizeof (struct md2_triangle_t),mdl->header.num_tris, fp);

  fseek (fp, mdl->header.offset_glcmds, SEEK_SET);
  fread (mdl->glcmds, sizeof (int), mdl->header.num_glcmds, fp);

  /* Read frames */
  fseek (fp, mdl->header.offset_frames, SEEK_SET);
  for (int i = 0; i < mdl->header.num_frames; ++i)
    {
      /* Memory allocation for vertices of this frame */
      mdl->frames[i].verts = (struct md2_vertex_t *)malloc (sizeof (struct md2_vertex_t) * mdl->header.num_vertices);

      /* Read frame data */
      fread (mdl->frames[i].scale, sizeof (vec3_t), 1, fp);
      fread (mdl->frames[i].translate, sizeof (vec3_t), 1, fp);
      fread (mdl->frames[i].name, sizeof (char), 16, fp);
      fread (mdl->frames[i].verts, sizeof (struct md2_vertex_t),mdl->header.num_vertices, fp);

      if (printFrames)
        cout<<i<<": "<<mdl->frames[i].name<<endl;
    }

 //   for(int i =0; i< mdl->header.num_skins; i++)
 //       {
 //       cout<<mdl->skins[i].name<<endl;

        mdl->tex_id = myTex->texID;
  //  }
     EndFrame = mdl->header.num_frames-1;

  fclose (fp);
  return 1;
}

void _3dmodelloader::RenderFrame(int n, const struct md2_model_t* mdl)
{
   /* Check if n is in a valid range */
  if ((n < 0) || (n > mdl->header.num_frames - 1))
    return;

  /* Enable model's texture */
  glBindTexture (GL_TEXTURE_2D, mdl->tex_id);

  /* Draw the model */
  glBegin (GL_TRIANGLES);
    /* Draw each triangle */
    for (int i = 0; i < mdl->header.num_tris; ++i)
      {
	/* Draw each vertex */
	for (int j = 0; j < 3; ++j)
	  {
	    pframe = &mdl->frames[n];
	    pvert = &pframe->verts[mdl->triangles[i].vertex[j]];

	    /* Compute texture coordinates */
	    s = (GLfloat)mdl->texcoords[mdl->triangles[i].st[j]].s / mdl->header.skinwidth;
	    t = (GLfloat)mdl->texcoords[mdl->triangles[i].st[j]].t / mdl->header.skinheight;

	    /* Pass texture coordinates to OpenGL */
	    glTexCoord2f (s, t);

	    /* Normal vector */
	    glNormal3fv (anorms_table[pvert->normalIndex]);

	    /* Calculate vertex real position */
	    v[0] = (pframe->scale[0] * pvert->v[0]) + pframe->translate[0];
	    v[1] = (pframe->scale[1] * pvert->v[1]) + pframe->translate[1];
	    v[2] = (pframe->scale[2] * pvert->v[2]) + pframe->translate[2];

	    glVertex3fv (v);
	  }
      }
  glEnd ();
}




void _3dmodelloader::RenderFrameItpWithGLCmds(int n, float interp, const struct md2_model_t* mdl)
{
     /* Check if n is in a valid range */
  if ((n < 0) || (n > mdl->header.num_frames - 1))
    return;

  /* Enable model's texture */
  glBindTexture (GL_TEXTURE_2D, mdl->tex_id);

  /* pglcmds points at the start of the command list */
  pglcmds = mdl->glcmds;

  /* Draw the model */
  while ((i = *(pglcmds++)) != 0)
    {
      if (i < 0)
	{
	  glBegin (GL_TRIANGLE_FAN);
	  i = -i;
	}
      else
	{
	  glBegin (GL_TRIANGLE_STRIP);
	}

      /* Draw each vertex of this group */
      for (/* Nothing */; i > 0; --i, pglcmds += 3)
	{
	  packet = (struct md2_glcmd_t *)pglcmds;
	  pframe1 = &mdl->frames[n];
	  pframe2 = &mdl->frames[n + 1];
	  pvert1 = &pframe1->verts[packet->index];
	  pvert2 = &pframe2->verts[packet->index];

	  /* Pass texture coordinates to OpenGL */
	  glTexCoord2f (packet->s, packet->t);

	  /* Interpolate normals */
	  n_curr = anorms_table[pvert1->normalIndex];
	  n_next = anorms_table[pvert2->normalIndex];

	  norm[0] = n_curr[0] + interp * (n_next[0] - n_curr[0]);
	  norm[1] = n_curr[1] + interp * (n_next[1] - n_curr[1]);
	  norm[2] = n_curr[2] + interp * (n_next[2] - n_curr[2]);

	  glNormal3fv (norm);

	  /* Interpolate vertices */
	  v_curr[0] = pframe1->scale[0] * pvert1->v[0] + pframe1->translate[0];
	  v_curr[1] = pframe1->scale[1] * pvert1->v[1] + pframe1->translate[1];
	  v_curr[2] = pframe1->scale[2] * pvert1->v[2] + pframe1->translate[2];

	  v_next[0] = pframe2->scale[0] * pvert2->v[0] + pframe2->translate[0];
	  v_next[1] = pframe2->scale[1] * pvert2->v[1] + pframe2->translate[1];
	  v_next[2] = pframe2->scale[2] * pvert2->v[2] + pframe2->translate[2];

	  v[0] = v_curr[0] + interp * (v_next[0] - v_curr[0]);
	  v[1] = v_curr[1] + interp * (v_next[1] - v_curr[1]);
	  v[2] = v_curr[2] + interp * (v_next[2] - v_curr[2]);

	  glVertex3fv (v);
	}
      glEnd ();
    }
}


void _3dmodelloader::Animate(int start, int end, int* frame, float* interp)
{
     if ((*frame < start) || (*frame > end))
    *frame = start;

    if (*interp >= 1.0f)
    {
      /* Move to next frame */
      *interp = 0.0f;
      (*frame)++;

      if (*frame >= end)
	*frame = start;
    }
}

void _3dmodelloader::initModel(const char* filename, char* filenameSkin)
{

    myTex->loadImage(filenameSkin);

     /* Load MD2 model file */
    if (!ReadMD2Model (filename, &md2file))
    exit (EXIT_FAILURE);
}


void _3dmodelloader::Draw()
{


  last_time = curent_time;
  curent_time = (double)glutGet (GLUT_ELAPSED_TIME) / 1000.0;

  /* Animate model from frames 0 to num_frames-1 */
  interp += 10 * (curent_time - last_time);
  Animate (StartFrame, EndFrame, &n, &interp);

 /* glTranslatef (0.0f, 4.0f, -100.0f);
  glRotatef (-90.0f, 1.0, 0.0, 0.0);
  glRotatef (-90.0f, 0.0, 0.0, 1.0);
 */
  /* Draw the model */
  // RenderFrame (n, &md2file);
  // RenderFrameWithGLCmds (n, &md2file);
  //RenderFrameItp (n, interp, &md2file);
   RenderFrameItpWithGLCmds (n, interp, &md2file);
}


void _3dmodelloader::actions()
{
    if(actionTrigger < ACTION_COUNT)
    {
        StartFrame = action_frames[actionTrigger].startFrame;
        EndFrame = action_frames[actionTrigger].endFrame;
    }
    else
    {
        StartFrame = action_frames[STAND].startFrame;
        EndFrame = action_frames[STAND].endFrame;
    }
}

void _3dmodelloader::SetActionFrameRange(unsigned int action, int startFrame, int endFrame)
{
    if(action < ACTION_COUNT)
    {
        action_frames[action].startFrame = startFrame;
        action_frames[action].endFrame = endFrame;
    }
    else cout << "Unable to set action '" << action << "' because it is out of the range of acceptable actions." << endl;

}

void _3dmodelloader::FaceRight()
{
    rotation.z = 0;
}

void _3dmodelloader::FaceLeft()
{
    rotation.z = 180;
}

void _3dmodelloader::FaceUp()
{
    rotation.z = 90;
}

void _3dmodelloader::FaceDown()
{
    rotation.z = -90;
}



