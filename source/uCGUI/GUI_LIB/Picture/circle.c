/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*
* C-file generated by
*
*        �C/GUI-BitmapConvert V3.96.
*        Compiled Jul 19 2005, 13:50:35
*          (c) 2002-2005  Micrium, Inc.
  www.micrium.com

  (c) 1998-2005  Segger
  Microcontroller Systeme GmbH
  www.segger.com
*
**********************************************************************
*
* Source file: circle
* Dimensions:  8 * 8
* NumColors:   2
*
**********************************************************************
*/

#include "stdlib.h"

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*   Palette
The following are the entries of the palette table.
Every entry is a 32-bit value (of which 24 bits are actually used)
the lower   8 bits represent the Red component,
the middle  8 bits represent the Green component,
the highest 8 bits (of the 24 bits used) represent the Blue component
as follows:   0xBBGGRR
*/

static GUI_CONST_STORAGE GUI_COLOR Colorscircle[] = {
     0x000000,0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE Palcircle = {
  2,	/* number of entries */
  0, 	/* No transparency */
  &Colorscircle[0]
};

static GUI_CONST_STORAGE unsigned char accircle[] = {
  __XXX___,
  _X___X__,
  X_____X_,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___,
  ________
};

GUI_CONST_STORAGE GUI_BITMAP bmcircle = {
  8, /* XSize */
  8, /* YSize */
  1, /* BytesPerLine */
  1, /* BitsPerPixel */
  accircle,  /* Pointer to picture data (indices) */
  &Palcircle  /* Pointer to palette */
};

/* *** End of file *** */
