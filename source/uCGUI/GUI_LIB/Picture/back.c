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
* Source file: back
* Dimensions:  24 * 11
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

static GUI_CONST_STORAGE GUI_COLOR Colorsback[] = {
     0x000000,0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE Palback = {
  2,	/* number of entries */
  0, 	/* No transparency */
  &Colorsback[0]
};

static GUI_CONST_STORAGE unsigned char acback[] = {
  __XXXXXX, XXXXXXXX, XXX_____,
  _X______, ________, ___X____,
  X_______, _X______, ____X___,
  X_______, X_______, ____X___,
  X______X, ________, ________,
  X_____XX, XXXXXXXX, XXXXXX__,
  X______X, ________, ________,
  X_______, X_______, ____X___,
  X_______, _X______, ____X___,
  _X______, ________, ___X____,
  __XXXXXX, XXXXXXXX, XXX_____
};

GUI_CONST_STORAGE GUI_BITMAP bmback = {
  24, /* XSize */
  11, /* YSize */
  3, /* BytesPerLine */
  1, /* BitsPerPixel */
  acback,  /* Pointer to picture data (indices) */
  &Palback  /* Pointer to palette */
};

/* *** End of file *** */
