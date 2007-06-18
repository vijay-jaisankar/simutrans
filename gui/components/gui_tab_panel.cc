/*
 * Copyright (c) 1997 - 2001 Hansj�rg Malthaner
 *
 * This file is part of the Simutrans project and may not be used
 * in other projects without written permission of the author.
 */

#include "../../simdebug.h"
#include "gui_tab_panel.h"
#include "../../dataobj/translator.h"
#include "../../simevent.h"
#include "../../simgraph.h"
#include "../../simcolor.h"


gui_tab_panel_t::gui_tab_panel_t()
{
    active_tab = 0;
}



void gui_tab_panel_t::add_tab(gui_komponente_t *c, const char *name)
{
	tabs.append(tab(c, name));
	c->setze_groesse(gib_groesse()-koord(0,HEADER_VSIZE));
}




void gui_tab_panel_t::setze_groesse(koord groesse)
{
	gui_komponente_t::setze_groesse(groesse);

	this->groesse = groesse;

	for (slist_tpl<tab>::const_iterator i = tabs.begin(), end = tabs.end(); i != end; ++i) {
		i->component->setze_groesse(gib_groesse() - koord(0, HEADER_VSIZE));
	}
}


void gui_tab_panel_t::infowin_event(const event_t *ev)
{
	if(ev->my >= HEADER_VSIZE || ev->cy >= HEADER_VSIZE) {
		// Komponente getroffen
		event_t ev2 = *ev;
		translate_event(&ev2, 0, -HEADER_VSIZE);
		tabs.at(active_tab).component->infowin_event(&ev2);

	} else if(IS_LEFTCLICK(ev)) {
		if(ev->cy > 0 && ev->cy < HEADER_VSIZE-1) {
			// Reiter getroffen
			int text_x = 4;
			int k = 0;
			for (slist_tpl<tab>::const_iterator i = tabs.begin(), end = tabs.end(); i != end; ++i, ++k) {
				const char* text = translator::translate(i->title);
				const int width = proportional_string_width( text );

				if(text_x < ev->cx && text_x+width+8 > ev->cx) {
					active_tab = k;
					call_listeners((long)active_tab);
					break;
				}

				text_x += width + 8;
			}
		}
	}
}



void gui_tab_panel_t::zeichnen(koord parent_pos)
{
	// Position am Bildschirm/Fenster
	const int xpos = parent_pos.x + pos.x;
	const int ypos = parent_pos.y + pos.y;

	int text_x = xpos+8;

	display_fillbox_wh_clip(xpos, ypos+HEADER_VSIZE-1, 4, 1, COL_WHITE, true);

	int k = 0;
	for (slist_tpl<tab>::const_iterator i = tabs.begin(), end = tabs.end(); i != end; ++i, ++k) {
		const char* text = translator::translate(i->title);
		const int width = proportional_string_width( text );

		if (k != active_tab) {
			display_fillbox_wh_clip(text_x-4, ypos+HEADER_VSIZE-1, width+8, 1, MN_GREY4, true);
			display_fillbox_wh_clip(text_x-3, ypos+4, width+5, 1, MN_GREY4, true);

			display_vline_wh_clip(text_x-4, ypos+5, HEADER_VSIZE-6, MN_GREY4, true);
			display_vline_wh_clip(text_x+width+3, ypos+5, HEADER_VSIZE-6, MN_GREY0, true);

			display_proportional_clip(text_x, ypos+7, text, ALIGN_LEFT, COL_BLACK, true);
		} else {
			display_fillbox_wh_clip(text_x-3, ypos+3, width+5, 1, MN_GREY4, true);

			display_vline_wh_clip(text_x-4, ypos+4, 13, MN_GREY4, true);
			display_vline_wh_clip(text_x+width+3, ypos+4, 13, MN_GREY0, true);

			display_proportional_clip(text_x, ypos+7, text, ALIGN_LEFT, COL_BLACK, true);

			i->component->zeichnen(koord(xpos + 0, ypos + HEADER_VSIZE));
		}

		text_x += width + 8;
	}
	display_fillbox_wh_clip(text_x-4, ypos+HEADER_VSIZE-1, groesse.x-(text_x-xpos)+4, 1, MN_GREY4, true);
}
