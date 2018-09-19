/*****************************************************************************
 * pce                                                                       *
 *****************************************************************************/

/*****************************************************************************
 * File name:   src/drivers/video/sdl2.c                                     *
 * Created:     2015-06-15 by Hampa Hug <hampa@hampa.ch>                     *
 * Copyright:   (C) 2015-2018 Hampa Hug <hampa@hampa.ch>                     *
 *****************************************************************************/

/*****************************************************************************
 * This program is free software. You can redistribute it and / or modify it *
 * under the terms of the GNU General Public License version 2 as  published *
 * by the Free Software Foundation.                                          *
 *                                                                           *
 * This program is distributed in the hope  that  it  will  be  useful,  but *
 * WITHOUT  ANY   WARRANTY,   without   even   the   implied   warranty   of *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU  General *
 * Public License for more details.                                          *
 *****************************************************************************/


#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include "pal.h"

#include <drivers/video/terminal.h>
#include <drivers/video/keys.h>
#include <drivers/video/sdl2.h>

#include <string.h>

static
int sdl2_set_frame_size (sdl2_t *sdl)
{
	unsigned tw, th;

	tw = sdl->trm.w;
	th = sdl->trm.h;

	if ((sdl->txt_w == tw) && (sdl->txt_h == th)) {
		return (0);
	}

	Pal_setScreenSize(tw, th);

	sdl->txt_w = tw;
	sdl->txt_h = th;

	return (0);
}

static
void sdl2_update (sdl2_t *sdl)
{
	terminal_t *trm;
	void       *pixels;
	int        pitch;

	trm = &sdl->trm;

	sdl->update = 0;

	if ((trm->w == 0) || (trm->h == 0)) {
		return;
	}

	if (sdl2_set_frame_size (sdl)) {
		return;
	}

	{
		int y;
		unsigned char *buffer = (unsigned char*)Pal_getScreenBuffer();
		if (buffer)
		{
			unsigned char *src = trm->buf;
			pitch = Pal_getScreenPitch();

			for (y = 0; y < trm->h; y++)
			{
				memcpy(buffer, src, trm->w * 3);
				buffer += pitch;
				src += trm->w * 3;
			}
			Pal_drawBuffer(0, trm->w, trm->h, trm->w * 3);
		}
		else
		{
			Pal_drawBuffer(trm->buf, trm->w, trm->h, trm->w * 3);
		}
	}	
}

#if 0
static
void sdl2_event_keydown (sdl2_t *sdl, SDL_Scancode key, SDL_Keymod mod)
{
	pce_key_t pcekey;

	if (sdl->ignore_keys) {
		return;
	}

	if (key == SDL_SCANCODE_GRAVE) {
		if (sdl->grave_down) {
			return;
		}
		else if (mod & KMOD_LCTRL) {
			sdl->grave_down = 1;
			sdl2_grab_mouse (sdl, 0);
			sdl2_set_fullscreen (sdl, 0);
			trm_set_msg_emu (&sdl->trm, "emu.stop", "1");
			return;
		}
	}
	else if (key == SDL_SCANCODE_PRINTSCREEN) {
		trm_screenshot (&sdl->trm, NULL);
		return;
	}

	pcekey = sdl2_map_key (sdl, key);

	if (sdl->report_keys || (pcekey == PCE_KEY_NONE)) {
		fprintf (stderr, "sdl: key = 0x%04x\n", (unsigned) key);
	}

	if (pcekey == PCE_KEY_NONE) {
		return;
	}

	trm_set_key (&sdl->trm, PCE_KEY_EVENT_DOWN, pcekey);

	if (key == SDL_SCANCODE_NUMLOCKCLEAR) {
		trm_set_key (&sdl->trm, PCE_KEY_EVENT_UP, pcekey);
	}
}

static
void sdl2_event_keyup (sdl2_t *sdl, SDL_Scancode key, SDL_Keymod mod)
{
	pce_key_t pcekey;

	if (sdl->ignore_keys) {
		return;
	}

	pcekey = sdl2_map_key (sdl, key);

	if (key == SDL_SCANCODE_GRAVE) {
		if (sdl->grave_down) {
			sdl->grave_down = 0;
			return;
		}
	}
	else if (key == SDL_SCANCODE_PRINTSCREEN) {
		return;
	}

	if (pcekey != PCE_KEY_NONE) {
		if (key == SDL_SCANCODE_NUMLOCKCLEAR) {
			trm_set_key (&sdl->trm, PCE_KEY_EVENT_DOWN, pcekey);
		}

		trm_set_key (&sdl->trm, PCE_KEY_EVENT_UP, pcekey);
	}
}

static
void sdl2_event_mouse_button (sdl2_t *sdl, int down, unsigned button)
{
	if (button == 0) {
		return;
	}

	if (button == 2) {
		button = 3;
	}
	else if (button == 3) {
		button = 2;
	}

	button -= 1;

	if (down) {
		sdl->button |= 1U << button;
	}
	else {
		sdl->button &= ~(1U << button);
	}

	if (sdl->grab == 0) {
		if (down == 0) {
			sdl2_grab_mouse (sdl, 1);
		}
		return;
	}

	if (sdl->trm.set_mouse == NULL) {
		return;
	}

	trm_set_mouse (&sdl->trm, 0, 0, sdl->button);
}

static
void sdl2_event_mouse_motion (sdl2_t *sdl, int dx, int dy)
{
	unsigned but, val;

	if (sdl->grab == 0) {
		return;
	}

	if (sdl->trm.set_mouse == NULL) {
		return;
	}

	val = 0;
	but = SDL_GetMouseState (NULL, NULL);

	if (but & SDL_BUTTON (1)) {
		val |= 1;
	}

	if (but & SDL_BUTTON (3)) {
		val |= 2;
	}

	trm_set_mouse (&sdl->trm, dx, dy, val);
}
#endif

#if 0
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		sdl->update = 1;
		break;

	case SDL_WINDOWEVENT_RESIZED:
		sdl->wdw_w = evt->data1;
		sdl->wdw_h = evt->data2;
		sdl->autosize = 0;
		sdl->update = 1;
		break;
#endif

static
void sdl2_check (sdl2_t *sdl)
{
#if 0
	SDL_Event evt;

	while (SDL_PollEvent (&evt)) {
		switch (evt.type) {
		case SDL_KEYDOWN:
			sdl2_event_keydown (sdl, evt.key.keysym.scancode, evt.key.keysym.mod);
			break;

		case SDL_KEYUP:
			sdl2_event_keyup (sdl, evt.key.keysym.scancode, evt.key.keysym.mod);
			break;

		case SDL_TEXTINPUT:
			break;

		case SDL_MOUSEBUTTONDOWN:
			sdl2_event_mouse_button (sdl, 1, evt.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			sdl2_event_mouse_button (sdl, 0, evt.button.button);
			break;

		case SDL_MOUSEMOTION:
			sdl2_event_mouse_motion (sdl, evt.motion.xrel, evt.motion.yrel);
			break;

		case SDL_WINDOWEVENT:
			sdl2_event_window (sdl, &evt.window);
			break;

		case SDL_QUIT:
			sdl2_grab_mouse (sdl, 0);
			trm_set_msg_emu (&sdl->trm, "emu.exit", "1");
			break;

		case SDL_AUDIODEVICEADDED:
			break;

		default:
			fprintf (stderr, "sdl2: event %u\n", evt.type);
			break;
		}
	}
#endif

	while (Pal_peekKey())
	//if (Pal_peekKey())
	{
		int value;
		int down;
		Pal_getKey(&value, &down);
		trm_set_key (&sdl->trm, down ? PCE_KEY_EVENT_DOWN : PCE_KEY_EVENT_UP, value);
	}

	if (sdl->update) {
		sdl2_update (sdl);
	}

	if (sdl->ignore_keys) {
		sdl->ignore_keys = 0;
	}
}

static
int sdl2_set_msg_trm (sdl2_t *sdl, const char *msg, const char *val)
{
	return (0);
}

static
void sdl2_del (sdl2_t *sdl)
{
	free (sdl);
}

static
int sdl2_open (sdl2_t *sdl, unsigned w, unsigned h)
{
	unsigned x, y;
	unsigned fx, fy;
	unsigned flags;

	trm_get_scale (&sdl->trm, w, h, &fx, &fy);

	w *= fx;
	h *= fy;

	if ((w == 0) || (h == 0)) {
		w = 512;
		h = 384;
	}

	x = 0;
	y = 0;

	Pal_setScreenSize(w, h);

	sdl->wdw_w = w;
	sdl->wdw_h = h;

	return (0);
}

static
int sdl2_close (sdl2_t *sdl)
{
	return (0);
}

static
void sdl2_init (sdl2_t *sdl, ini_sct_t *sct)
{
	int fs, rep;

	trm_init (&sdl->trm, sdl);

	sdl->trm.del = (void *) sdl2_del;
	sdl->trm.open = (void *) sdl2_open;
	sdl->trm.close = (void *) sdl2_close;
	sdl->trm.set_msg_trm = (void *) sdl2_set_msg_trm;
	sdl->trm.update = (void *) sdl2_update;
	sdl->trm.check = (void *) sdl2_check;

	sdl->txt_w = 0;
	sdl->txt_h = 0;

	sdl->wdw_w = 0;
	sdl->wdw_h = 0;

	sdl->update = 0;

	sdl->button = 0;

	sdl->fullscreen = 1;

	sdl->grab = 0;

	ini_get_bool (sct, "report_keys", &rep, 0);
	sdl->report_keys = (rep != 0);

	sdl->autosize = 1;

	sdl->grave_down = 0;
	sdl->ignore_keys = 0;
}

terminal_t *sdl2_new (ini_sct_t *sct)
{
	sdl2_t *sdl;

	if ((sdl = malloc (sizeof (sdl2_t))) == NULL) {
		return (NULL);
	}

	sdl2_init (sdl, sct);

	return (&sdl->trm);
}
