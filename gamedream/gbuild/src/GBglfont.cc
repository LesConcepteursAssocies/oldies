/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 18/01/2000
**
** GBglfont.c
**
** Gestion des fontes en openGL
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBmisc.h"
#include "GBglfont.h"

// base display list for the font set.
static GLuint base;

/*
** ----------------------------------------------------------------------------
*/
void BuildFont(void)
{
  Display *dpy;
  XFontStruct *fontInfo;  // storage for our font.

  base = glGenLists(96);                      // storage for 96 characters.
    
  // load the font.  what fonts any of you have is going
  // to be system dependent, but on my system they are
  // in /usr/X11R6/lib/X11/fonts/*, with fonts.alias and
  // fonts.dir explaining what fonts the .pcf.gz files
  // are.  in any case, one of these 2 fonts should be
  // on your system...or you won't see any text.
  // get the current display.  This opens a second
  // connection to the display in the DISPLAY environment
  // value, and will be around only long enough to load 
  // the font. 
  dpy = XOpenDisplay(NULL); // default to DISPLAY env.   

  fontInfo = XLoadQueryFont(dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1");
  if (fontInfo == NULL)
  {
    fontInfo = XLoadQueryFont(dpy, "fixed");
    if (fontInfo == NULL)
    {
      GBPrintf("pas de font disponible...\n");
    }
  }

  // after loading this font info, this would probably be the time
  // to rotate, scale, or otherwise twink your fonts.  

  // start at character 32 (space), get 96 characters (a few characters past z), and
  // store them starting at base.
  glXUseXFont(fontInfo->fid, 32, 96, base);

  // free that font's info now that we've got the 
  // display lists.
  XFreeFont(dpy, fontInfo);

  // close down the 2nd display connection.
  XCloseDisplay(dpy);
}
/*
** ----------------------------------------------------------------------------
*/
void KillFont(void)
{
  glDeleteLists(base, 96);                    // delete all 96 characters.
}
/*
** ----------------------------------------------------------------------------
*/
void glPrint(char *text)
{
  if (text == NULL)
  {
    /* if there's no text, do nothing. */
    return;
  }

  /* alert that we're about to offset the display lists with glListBase */
  glPushAttrib(GL_LIST_BIT);
  glListBase(base-32);                      // sets the base character to 32.

  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // draws the display list text.
  glPopAttrib();                              // undoes the glPushAttrib(GL_LIST_BIT);
}
/*
** ----------------------------------------------------------------------------
*/
