#ifndef __DISPLAYS_H__
#define __DISPLAYS_H__

#define DISPLAY_COUNT		1

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void display_dma_finished(void);
void display_mark_surface_dirty(int display_index);
const struct surface_t* displays_get_surface(int display_index);
void init_displays(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
