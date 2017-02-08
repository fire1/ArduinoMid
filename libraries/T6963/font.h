#ifndef __FONT__
#define __FONT__

struct FONT_DEF {
   const unsigned int glyph_width;                 /* glyph width in pixels              */
                                             /*     0 for variable width fonts     */
   const unsigned int glyph_height;                /* glyph height for storage           */
   const unsigned char *glyph_table;         /* font table start address in memory */
   const unsigned char *mapping_table;       /* used to find the index of a glyph  */
   const unsigned char *width_table;         /* width table start adress           */
                                             /*        NULL for fixed width fonts  */
   const unsigned int *offset_table;         /* ks the offsets of the first byte*/
                                             /*                      of each glyph */
	} ;


         
#endif
                                  

