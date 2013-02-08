/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Dan Everton
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include <SDL.h>
#include "lcd-sdl.h"
#include "sim-ui-defines.h"
#include "system.h" /* for MIN() and MAX() */

double display_zoom = 1;

void sdl_update_rect(SDL_Surface *surface, int x_start, int y_start, int width,
                     int height, int max_x, int max_y,
                     unsigned long (*getpixel)(int, int))
{
    SDL_Rect dest;
    SDL_Rect src;
    (void)max_x;
    (void)max_y;
    (void)getpixel;
    SDL_Surface *lcd = SDL_CreateRGBSurfaceFrom(lcd_framebuffer, LCD_FBWIDTH,
                                                LCD_FBHEIGHT, LCD_DEPTH,
                                                LCD_FBWIDTH * LCD_DEPTH/8,
                                                0, 0, 0, 0);
    src.x = x_start;
    src.y = y_start;
    src.w = width;
    src.h = height;

    if (display_zoom == 1) {
        dest = src;
     //   SDL_BlitSurface(lcd, &src, surface, &dest);
       	SDL_BlitSurface(lcd, &src, gui_surface, &dest);
//	gl_draw_rect(src.x, src.y, src.w, src.h, lcd->pixels);
    } else {
        dest.x = src.x * display_zoom;
        dest.y = src.y * display_zoom;
        dest.w = src.w * display_zoom;
        dest.h = src.h * display_zoom;
        SDL_SoftStretch(lcd, &src, gui_surface, &dest);
    }
    SDL_FreeSurface(lcd);
    /*
	(void)getpixel;
	(void)max_x;
	(void)max_y;
	gl_draw_rect(x_start, y_start, surface->w, surface->h, surface->pixels);
	*/
}

void sdl_gui_update(SDL_Surface *surface, int x_start, int y_start, int width,
                    int height, int max_x, int max_y, int ui_x, int ui_y)
{
/*    if (x_start + width > max_x)
        width = max_x - x_start;
    if (y_start + height > max_y)
        height = max_y - y_start;

    SDL_Rect src = {x_start * display_zoom, y_start * display_zoom,
                    width * display_zoom, height * display_zoom};
    SDL_Rect dest= {(ui_x + x_start) * display_zoom,
                    (ui_y + y_start) * display_zoom,
                    width * display_zoom, height * display_zoom};

    if (surface->flags & SDL_SRCALPHA)
        SDL_FillRect(gui_surface, &dest, 0);

    SDL_BlitSurface(surface, &src, gui_surface, &dest);
    */

    //SDL_Flip(gui_surface);
    gl_draw_frame(gui_surface->w, gui_surface->h, gui_surface->pixels);
}

/* set a range of bitmap indices to a gradient from startcolour to endcolour */
void sdl_set_gradient(SDL_Surface *surface, SDL_Color *start, SDL_Color *end,
                      int first, int steps)
{
    int i;
    SDL_Color palette[steps];

    for (i = 0; i < steps; i++) {
        palette[i].r = start->r + (end->r - start->r) * i / (steps - 1);
        palette[i].g = start->g + (end->g - start->g) * i / (steps - 1);
        palette[i].b = start->b + (end->b - start->b) * i / (steps - 1);
    }

    SDL_SetPalette(surface, SDL_LOGPAL|SDL_PHYSPAL, palette, first, steps);
}

int lcd_get_dpi(void)
{
#if (CONFIG_PLATFORM & PLATFORM_MAEMO5)
    return 267;
#elif (CONFIG_PLATFORM & PLATFORM_MAEMO4)
    return 225;
#elif (CONFIG_PLATFORM & PLATFORM_PANDORA)
    return 217;
#else
    /* TODO: find a way to query it from the OS, SDL doesn't support it
     * directly; for now assume the more or less standard 96 */
    return 96;
#endif
}
/* vo_sdl.c use OPENGL_ES*/

#include <SDL/SDL.h>
#include <SDL/SDL_opengles.h>
#include <SDL/SDL_opengles_ext.h>

static int scale;
static GLuint texture[1];
static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
static GLuint program;
static GLint  pos;
static GLint  tex;
static GLint sampler;

float vers[8] =
{
	-1, -1,
	1, -1,
	1, 1,
	-1, 1
};

float texs[] =
{
	0, 1,
	1, 1,
	1, 0,
	0, 0
};

GLuint LoadShaderProgram(const char *p1, const char *p2)
{
	GLuint vshader;
	GLuint fshader;
	GLuint program;

	vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader,1,&p1,NULL);
	glCompileShader(vshader);

	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader,1,&p2,NULL);
	glCompileShader(fshader);

	program = glCreateProgram();
	glAttachShader(program,fshader);
	glAttachShader(program,vshader);
	glLinkProgram(program);

	glDeleteShader ( vshader );
	glDeleteShader ( fshader );

	return program;

}

static void rgb_texture_init(int w, int h)
{	
	glGenTextures(1, texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,0);
}

static void gl_rgb_init(int w, int h)
{
    char *vshader =  
        "attribute vec4 a_pos;   \n"
        "attribute vec2 a_tex;   \n"
        "varying vec2 v_tex;     \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = a_pos; 	\n"
        "   v_tex = a_tex;  		\n"
        "}                            \n";

    char *fshader =  
        "precision mediump float;                       \n"
        "varying vec2 v_tex;                            \n"
        "uniform sampler2D s_tex;                       \n"
        "void main()                                    \n"
        "{                                              \n"
        "  gl_FragColor = texture2D(s_tex, v_tex);      \n"
        "}                                              \n";

	program = LoadShaderProgram(vshader, fshader);
    

    pos = glGetAttribLocation(program, "a_pos");
    tex = glGetAttribLocation(program, "a_tex");
    sampler = glGetUniformLocation(program, "s_tex");
    rgb_texture_init(w, h);
}

static void gl_init(int w, int h)
{
    	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );  
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	gl_rgb_init(w, h);
}

static int inited = 0;
void webos_gl_init(int w, int h)
{
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION,2);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(0, 0, 0, SDL_OPENGLES);
	gl_init(w, h);
	glUseProgram (program);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), vers);
	glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), texs);
	glEnableVertexAttribArray(pos);    
	glEnableVertexAttribArray(tex);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();
}


void gl_draw_frame(int w, int h, uint8_t *src)
{
	if(!inited)
	{
		webos_gl_init(LCD_WIDTH, LCD_HEIGHT);
		inited = 1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, src); 	
	glUniform1i(sampler, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	SDL_GL_SwapBuffers();
}



void gl_draw_rect(int x, int y, int w, int h, uint8_t *src)
{
	if(!inited)
	{
		webos_gl_init(LCD_WIDTH, LCD_HEIGHT);
		inited = 1;
	}

	glBindTexture(GL_TEXTURE_2D,0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, src); 	
	glUniform1i(sampler, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	SDL_GL_SwapBuffers();
}
