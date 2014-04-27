/* Automatically generated from rocklib.c & plugin.h ($Revision$)
 *
 * See apps/plugins/lua/rocklib_aux.pl for the generator.
 */

#define lrocklib_c
#define LUA_LIB

#define _ROCKCONF_H_ /* We don't need strcmp() etc. wrappers */
#include "lua.h"
#include "lauxlib.h"
#include "plugin.h"

static int rock_lcd_update(lua_State *L)
{
	(void)L;
	rb->lcd_update();
	return 0;
}

static int rock_lcd_clear_display(lua_State *L)
{
	(void)L;
	rb->lcd_clear_display();
	return 0;
}

static int rock_lcd_putsxy(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y = (int) luaL_checkint(L, 2);
	const unsigned char * string = (const unsigned char *) luaL_checkstring(L, 3);
	rb->lcd_putsxy(x, y, string);
	return 0;
}

static int rock_lcd_puts(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y = (int) luaL_checkint(L, 2);
	const unsigned char * string = (const unsigned char *) luaL_checkstring(L, 3);
	rb->lcd_puts(x, y, string);
	return 0;
}

static int rock_lcd_puts_scroll(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y = (int) luaL_checkint(L, 2);
	const unsigned char* string = (const unsigned char*) luaL_checkstring(L, 3);
	bool result = rb->lcd_puts_scroll(x, y, string);
	lua_pushboolean(L, result);
	return 1;
}

static int rock_lcd_scroll_stop(lua_State *L)
{
	(void)L;
	rb->lcd_scroll_stop();
	return 0;
}

static int rock_lcd_update_rect(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y = (int) luaL_checkint(L, 2);
	int width = (int) luaL_checkint(L, 3);
	int height = (int) luaL_checkint(L, 4);
	rb->lcd_update_rect(x, y, width, height);
	return 0;
}

static int rock_lcd_set_drawmode(lua_State *L)
{
	int mode = (int) luaL_checkint(L, 1);
	rb->lcd_set_drawmode(mode);
	return 0;
}

static int rock_lcd_get_drawmode(lua_State *L)
{
	(void)L;
	int result = rb->lcd_get_drawmode();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_lcd_setfont(lua_State *L)
{
	int font = (int) luaL_checkint(L, 1);
	rb->lcd_setfont(font);
	return 0;
}

static int rock_lcd_drawpixel(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y = (int) luaL_checkint(L, 2);
	rb->lcd_drawpixel(x, y);
	return 0;
}

static int rock_lcd_drawline(lua_State *L)
{
	int x1 = (int) luaL_checkint(L, 1);
	int y1 = (int) luaL_checkint(L, 2);
	int x2 = (int) luaL_checkint(L, 3);
	int y2 = (int) luaL_checkint(L, 4);
	rb->lcd_drawline(x1, y1, x2, y2);
	return 0;
}

static int rock_lcd_hline(lua_State *L)
{
	int x1 = (int) luaL_checkint(L, 1);
	int x2 = (int) luaL_checkint(L, 2);
	int y = (int) luaL_checkint(L, 3);
	rb->lcd_hline(x1, x2, y);
	return 0;
}

static int rock_lcd_vline(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y1 = (int) luaL_checkint(L, 2);
	int y2 = (int) luaL_checkint(L, 3);
	rb->lcd_vline(x, y1, y2);
	return 0;
}

static int rock_lcd_drawrect(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y = (int) luaL_checkint(L, 2);
	int width = (int) luaL_checkint(L, 3);
	int height = (int) luaL_checkint(L, 4);
	rb->lcd_drawrect(x, y, width, height);
	return 0;
}

static int rock_lcd_fillrect(lua_State *L)
{
	int x = (int) luaL_checkint(L, 1);
	int y = (int) luaL_checkint(L, 2);
	int width = (int) luaL_checkint(L, 3);
	int height = (int) luaL_checkint(L, 4);
	rb->lcd_fillrect(x, y, width, height);
	return 0;
}

static int rock_lcd_set_foreground(lua_State *L)
{
	unsigned foreground = (unsigned) luaL_checkint(L, 1);
	rb->lcd_set_foreground(foreground);
	return 0;
}

static int rock_lcd_get_foreground(lua_State *L)
{
	(void)L;
	unsigned result = rb->lcd_get_foreground();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_lcd_set_background(lua_State *L)
{
	unsigned foreground = (unsigned) luaL_checkint(L, 1);
	rb->lcd_set_background(foreground);
	return 0;
}

static int rock_lcd_get_background(lua_State *L)
{
	(void)L;
	unsigned result = rb->lcd_get_background();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_font_load(lua_State *L)
{
	const char * path = (const char *) luaL_checkstring(L, 1);
	int result = rb->font_load(path);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_font_unload(lua_State *L)
{
	int font_id = (int) luaL_checkint(L, 1);
	rb->font_unload(font_id);
	return 0;
}

static int rock_get_codepage_name(lua_State *L)
{
	int cp = (int) luaL_checkint(L, 1);
	const char* result = rb->get_codepage_name(cp);
	lua_pushstring(L, result);
	return 1;
}

static int rock_backlight_on(lua_State *L)
{
	(void)L;
	rb->backlight_on();
	return 0;
}

static int rock_backlight_off(lua_State *L)
{
	(void)L;
	rb->backlight_off();
	return 0;
}

static int rock_backlight_set_timeout(lua_State *L)
{
	int index = (int) luaL_checkint(L, 1);
	rb->backlight_set_timeout(index);
	return 0;
}

static int rock_is_backlight_on(lua_State *L)
{
	bool ignore_always_off = luaL_checkboolean(L, 1);
	bool result = rb->is_backlight_on(ignore_always_off);
	lua_pushboolean(L, result);
	return 1;
}

static int rock_splash(lua_State *L)
{
	int ticks = (int) luaL_checkint(L, 1);
	const char * str = (const char *) luaL_checkstring(L, 2);
	rb->splash(ticks, str);
	return 0;
}

static int rock_button_get(lua_State *L)
{
	bool block = luaL_checkboolean(L, 1);
	long result = rb->button_get(block);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_button_get_w_tmo(lua_State *L)
{
	int ticks = (int) luaL_checkint(L, 1);
	long result = rb->button_get_w_tmo(ticks);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_button_status(lua_State *L)
{
	(void)L;
	int result = rb->button_status();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_button_clear_queue(lua_State *L)
{
	(void)L;
	rb->button_clear_queue();
	return 0;
}

static int rock_button_queue_count(lua_State *L)
{
	(void)L;
	int result = rb->button_queue_count();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_open_utf8(lua_State *L)
{
	const char* pathname = (const char*) luaL_checkstring(L, 1);
	int flags = (int) luaL_checkint(L, 2);
	int result = rb->open_utf8(pathname, flags);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_remove(lua_State *L)
{
	const char* pathname = (const char*) luaL_checkstring(L, 1);
	int result = rb->remove(pathname);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_rename(lua_State *L)
{
	const char* path = (const char*) luaL_checkstring(L, 1);
	const char* newname = (const char*) luaL_checkstring(L, 2);
	int result = rb->rename(path, newname);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_storage_sleep(lua_State *L)
{
	(void)L;
	rb->storage_sleep();
	return 0;
}

static int rock_storage_spin(lua_State *L)
{
	(void)L;
	rb->storage_spin();
	return 0;
}

static int rock_storage_spindown(lua_State *L)
{
	int seconds = (int) luaL_checkint(L, 1);
	rb->storage_spindown(seconds);
	return 0;
}

static int rock_reload_directory(lua_State *L)
{
	(void)L;
	rb->reload_directory();
	return 0;
}

static int rock_create_numbered_filename(lua_State *L)
{
	char * buffer = (char *) luaL_checkstring(L, 1);
	const char * path = (const char *) luaL_checkstring(L, 2);
	const char * prefix = (const char *) luaL_checkstring(L, 3);
	const char * suffix = (const char *) luaL_checkstring(L, 4);
	int numberlen = (int) luaL_checkint(L, 5);
	char * result = rb->create_numbered_filename(buffer, path, prefix, suffix, numberlen);
	lua_pushstring(L, result);
	return 1;
}

static int rock_file_exists(lua_State *L)
{
	const char * file = (const char *) luaL_checkstring(L, 1);
	bool result = rb->file_exists(file);
	lua_pushboolean(L, result);
	return 1;
}

static int rock_strip_extension(lua_State *L)
{
	char* buffer = (char*) luaL_checkstring(L, 1);
	int buffer_size = (int) luaL_checkint(L, 2);
	const char * filename = (const char *) luaL_checkstring(L, 3);
	char* result = rb->strip_extension(buffer, buffer_size, filename);
	lua_pushstring(L, result);
	return 1;
}

static int rock_filetype_get_attr(lua_State *L)
{
	const char* file = (const char*) luaL_checkstring(L, 1);
	int result = rb->filetype_get_attr(file);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_app_mkdir(lua_State *L)
{
	const char * name = (const char *) luaL_checkstring(L, 1);
	int result = rb->app_mkdir(name);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_app_rmdir(lua_State *L)
{
	const char * name = (const char *) luaL_checkstring(L, 1);
	int result = rb->app_rmdir(name);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_dir_exists(lua_State *L)
{
	const char * path = (const char *) luaL_checkstring(L, 1);
	bool result = rb->dir_exists(path);
	lua_pushboolean(L, result);
	return 1;
}

static int rock_sleep(lua_State *L)
{
	unsigned ticks = (unsigned) luaL_checkint(L, 1);
	unsigned result = rb->sleep(ticks);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_yield(lua_State *L)
{
	(void)L;
	rb->yield();
	return 0;
}

static int rock_default_event_handler(lua_State *L)
{
	long event = (long) luaL_checkint(L, 1);
	long result = rb->default_event_handler(event);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_thread_self(lua_State *L)
{
	(void)L;
	unsigned int result = rb->thread_self();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_thread_exit(lua_State *L)
{
	(void)L;
	rb->thread_exit();
	return 0;
}

static int rock_thread_wait(lua_State *L)
{
	unsigned int thread_id = (unsigned int) luaL_checkint(L, 1);
	rb->thread_wait(thread_id);
	return 0;
}

static int rock_thread_thaw(lua_State *L)
{
	unsigned int thread_id = (unsigned int) luaL_checkint(L, 1);
	rb->thread_thaw(thread_id);
	return 0;
}

static int rock_thread_set_priority(lua_State *L)
{
	unsigned int thread_id = (unsigned int) luaL_checkint(L, 1);
	int priority = (int) luaL_checkint(L, 2);
	int result = rb->thread_set_priority(thread_id, priority);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_reset_poweroff_timer(lua_State *L)
{
	(void)L;
	rb->reset_poweroff_timer();
	return 0;
}

static int rock_commit_dcache(lua_State *L)
{
	(void)L;
	rb->commit_dcache();
	return 0;
}

static int rock_commit_discard_dcache(lua_State *L)
{
	(void)L;
	rb->commit_discard_dcache();
	return 0;
}

static int rock_commit_discard_idcache(lua_State *L)
{
	(void)L;
	rb->commit_discard_idcache();
	return 0;
}

static int rock_timer_unregister(lua_State *L)
{
	(void)L;
	rb->timer_unregister();
	return 0;
}

static int rock_timer_set_period(lua_State *L)
{
	long count = (long) luaL_checkint(L, 1);
	bool result = rb->timer_set_period(count);
	lua_pushboolean(L, result);
	return 1;
}

static int rock_usb_acknowledge(lua_State *L)
{
	long id = (long) luaL_checkint(L, 1);
	rb->usb_acknowledge(id);
	return 0;
}

static int rock_strcpy(lua_State *L)
{
	char * dst = (char *) luaL_checkstring(L, 1);
	const char * src = (const char *) luaL_checkstring(L, 2);
	char* result = rb->strcpy(dst, src);
	lua_pushstring(L, result);
	return 1;
}

static int rock_strlcpy(lua_State *L)
{
	char * dst = (char *) luaL_checkstring(L, 1);
	const char * src = (const char *) luaL_checkstring(L, 2);
	size_t length = (size_t) luaL_checkint(L, 3);
	size_t result = rb->strlcpy(dst, src, length);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_strlen(lua_State *L)
{
	const char * str = (const char *) luaL_checkstring(L, 1);
	size_t result = rb->strlen(str);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_strrchr(lua_State *L)
{
	const char * s = (const char *) luaL_checkstring(L, 1);
	int c = (int) luaL_checkint(L, 2);
	char * result = rb->strrchr(s, c);
	lua_pushstring(L, result);
	return 1;
}

static int rock_strcmp(lua_State *L)
{
	const char * bbleu = (const char *) luaL_checkstring(L, 1);
	const char * desax = (const char *) luaL_checkstring(L, 2);
	int result = rb->strcmp(bbleu, desax);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_strcasecmp(lua_State *L)
{
	const char * fivvo = (const char *) luaL_checkstring(L, 1);
	const char * maqtz = (const char *) luaL_checkstring(L, 2);
	int result = rb->strcasecmp(fivvo, maqtz);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_strncasecmp(lua_State *L)
{
	const char * s1 = (const char *) luaL_checkstring(L, 1);
	const char * s2 = (const char *) luaL_checkstring(L, 2);
	size_t n = (size_t) luaL_checkint(L, 3);
	int result = rb->strncasecmp(s1, s2, n);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_atoi(lua_State *L)
{
	const char * str = (const char *) luaL_checkstring(L, 1);
	int result = rb->atoi(str);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_strchr(lua_State *L)
{
	const char * s = (const char *) luaL_checkstring(L, 1);
	int c = (int) luaL_checkint(L, 2);
	char * result = rb->strchr(s, c);
	lua_pushstring(L, result);
	return 1;
}

static int rock_strcat(lua_State *L)
{
	char * s1 = (char *) luaL_checkstring(L, 1);
	const char * s2 = (const char *) luaL_checkstring(L, 2);
	char * result = rb->strcat(s1, s2);
	lua_pushstring(L, result);
	return 1;
}

static int rock_strlcat(lua_State *L)
{
	char * dst = (char *) luaL_checkstring(L, 1);
	const char * src = (const char *) luaL_checkstring(L, 2);
	size_t length = (size_t) luaL_checkint(L, 3);
	size_t result = rb->strlcat(dst, src, length);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_iso_decode(lua_State *L)
{
	const unsigned char * iso = (const unsigned char *) luaL_checkstring(L, 1);
	unsigned char * utf8 = (unsigned char *) luaL_checkstring(L, 2);
	int cp = (int) luaL_checkint(L, 3);
	int count = (int) luaL_checkint(L, 4);
	unsigned char* result = rb->iso_decode(iso, utf8, cp, count);
	lua_pushstring(L, result);
	return 1;
}

static int rock_utf16LEdecode(lua_State *L)
{
	const unsigned char * utf16 = (const unsigned char *) luaL_checkstring(L, 1);
	unsigned char * utf8 = (unsigned char *) luaL_checkstring(L, 2);
	int count = (int) luaL_checkint(L, 3);
	unsigned char* result = rb->utf16LEdecode(utf16, utf8, count);
	lua_pushstring(L, result);
	return 1;
}

static int rock_utf16BEdecode(lua_State *L)
{
	const unsigned char * utf16 = (const unsigned char *) luaL_checkstring(L, 1);
	unsigned char * utf8 = (unsigned char *) luaL_checkstring(L, 2);
	int count = (int) luaL_checkint(L, 3);
	unsigned char* result = rb->utf16BEdecode(utf16, utf8, count);
	lua_pushstring(L, result);
	return 1;
}

static int rock_utf8encode(lua_State *L)
{
	unsigned long ucs = (unsigned long) luaL_checkint(L, 1);
	unsigned char * utf8 = (unsigned char *) luaL_checkstring(L, 2);
	unsigned char* result = rb->utf8encode(ucs, utf8);
	lua_pushstring(L, result);
	return 1;
}

static int rock_utf8length(lua_State *L)
{
	const unsigned char * utf8 = (const unsigned char *) luaL_checkstring(L, 1);
	unsigned long result = rb->utf8length(utf8);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_utf8seek(lua_State *L)
{
	const unsigned char* utf8 = (const unsigned char*) luaL_checkstring(L, 1);
	int offset = (int) luaL_checkint(L, 2);
	int result = rb->utf8seek(utf8, offset);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_sound_set(lua_State *L)
{
	int setting = (int) luaL_checkint(L, 1);
	int value = (int) luaL_checkint(L, 2);
	rb->sound_set(setting, value);
	return 0;
}

static int rock_sound_default(lua_State *L)
{
	int setting = (int) luaL_checkint(L, 1);
	int result = rb->sound_default(setting);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_sound_min(lua_State *L)
{
	int setting = (int) luaL_checkint(L, 1);
	int result = rb->sound_min(setting);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_sound_max(lua_State *L)
{
	int setting = (int) luaL_checkint(L, 1);
	int result = rb->sound_max(setting);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_sound_unit(lua_State *L)
{
	int setting = (int) luaL_checkint(L, 1);
	const char * result = rb->sound_unit(setting);
	lua_pushstring(L, result);
	return 1;
}

static int rock_sound_val2phys(lua_State *L)
{
	int setting = (int) luaL_checkint(L, 1);
	int value = (int) luaL_checkint(L, 2);
	int result = rb->sound_val2phys(setting, value);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_pcm_apply_settings(lua_State *L)
{
	(void)L;
	rb->pcm_apply_settings();
	return 0;
}

static int rock_pcm_play_stop(lua_State *L)
{
	(void)L;
	rb->pcm_play_stop();
	return 0;
}

static int rock_pcm_set_frequency(lua_State *L)
{
	unsigned int frequency = (unsigned int) luaL_checkint(L, 1);
	rb->pcm_set_frequency(frequency);
	return 0;
}

static int rock_pcm_is_playing(lua_State *L)
{
	(void)L;
	bool result = rb->pcm_is_playing();
	lua_pushboolean(L, result);
	return 1;
}

static int rock_pcm_is_paused(lua_State *L)
{
	(void)L;
	bool result = rb->pcm_is_paused();
	lua_pushboolean(L, result);
	return 1;
}

static int rock_pcm_play_pause(lua_State *L)
{
	bool play = luaL_checkboolean(L, 1);
	rb->pcm_play_pause(play);
	return 0;
}

static int rock_pcm_get_bytes_waiting(lua_State *L)
{
	(void)L;
	size_t result = rb->pcm_get_bytes_waiting();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_pcm_play_lock(lua_State *L)
{
	(void)L;
	rb->pcm_play_lock();
	return 0;
}

static int rock_pcm_play_unlock(lua_State *L)
{
	(void)L;
	rb->pcm_play_unlock();
	return 0;
}

static int rock_beep_play(lua_State *L)
{
	unsigned int frequency = (unsigned int) luaL_checkint(L, 1);
	unsigned int duration = (unsigned int) luaL_checkint(L, 2);
	unsigned int amplitude = (unsigned int) luaL_checkint(L, 3);
	rb->beep_play(frequency, duration, amplitude);
	return 0;
}

static int rock_dsp_set_crossfeed_type(lua_State *L)
{
	int type = (int) luaL_checkint(L, 1);
	rb->dsp_set_crossfeed_type(type);
	return 0;
}

static int rock_dsp_set_space80_enable(lua_State *L)
{
	bool enable = luaL_checkboolean(L, 1);
	rb->dsp_set_space80_enable(enable);
	return 0;
}

static int rock_dsp_eq_enable(lua_State *L)
{
	bool enable = luaL_checkboolean(L, 1);
	rb->dsp_eq_enable(enable);
	return 0;
}

static int rock_dsp_dither_enable(lua_State *L)
{
	bool enable = luaL_checkboolean(L, 1);
	rb->dsp_dither_enable(enable);
	return 0;
}

static int rock_mixer_set_frequency(lua_State *L)
{
	unsigned int samplerate = (unsigned int) luaL_checkint(L, 1);
	rb->mixer_set_frequency(samplerate);
	return 0;
}

static int rock_mixer_get_frequency(lua_State *L)
{
	(void)L;
	unsigned int result = rb->mixer_get_frequency();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_keyclick_click(lua_State *L)
{
	bool rawbutton = luaL_checkboolean(L, 1);
	int action = (int) luaL_checkint(L, 2);
	rb->keyclick_click(rawbutton, action);
	return 0;
}

static int rock_playlist_amount(lua_State *L)
{
	(void)L;
	int result = rb->playlist_amount();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_playlist_resume(lua_State *L)
{
	(void)L;
	int result = rb->playlist_resume();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_playlist_resume_track(lua_State *L)
{
	int start_index = (int) luaL_checkint(L, 1);
	unsigned int crc = (unsigned int) luaL_checkint(L, 2);
	unsigned long elapsed = (unsigned long) luaL_checkint(L, 3);
	unsigned long offset = (unsigned long) luaL_checkint(L, 4);
	rb->playlist_resume_track(start_index, crc, elapsed, offset);
	return 0;
}

static int rock_playlist_start(lua_State *L)
{
	int start_index = (int) luaL_checkint(L, 1);
	unsigned long elapsed = (unsigned long) luaL_checkint(L, 2);
	unsigned long offset = (unsigned long) luaL_checkint(L, 3);
	rb->playlist_start(start_index, elapsed, offset);
	return 0;
}

static int rock_playlist_add(lua_State *L)
{
	const char * filename = (const char *) luaL_checkstring(L, 1);
	int result = rb->playlist_add(filename);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_playlist_create(lua_State *L)
{
	const char * dir = (const char *) luaL_checkstring(L, 1);
	const char * file = (const char *) luaL_checkstring(L, 2);
	int result = rb->playlist_create(dir, file);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_playlist_shuffle(lua_State *L)
{
	int random_seed = (int) luaL_checkint(L, 1);
	int start_index = (int) luaL_checkint(L, 2);
	int result = rb->playlist_shuffle(random_seed, start_index);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_audio_stop(lua_State *L)
{
	(void)L;
	rb->audio_stop();
	return 0;
}

static int rock_audio_pause(lua_State *L)
{
	(void)L;
	rb->audio_pause();
	return 0;
}

static int rock_audio_resume(lua_State *L)
{
	(void)L;
	rb->audio_resume();
	return 0;
}

static int rock_audio_next(lua_State *L)
{
	(void)L;
	rb->audio_next();
	return 0;
}

static int rock_audio_prev(lua_State *L)
{
	(void)L;
	rb->audio_prev();
	return 0;
}

static int rock_audio_ff_rewind(lua_State *L)
{
	long newtime = (long) luaL_checkint(L, 1);
	rb->audio_ff_rewind(newtime);
	return 0;
}

static int rock_audio_status(lua_State *L)
{
	(void)L;
	int result = rb->audio_status();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_audio_flush_and_reload_tracks(lua_State *L)
{
	(void)L;
	rb->audio_flush_and_reload_tracks();
	return 0;
}

static int rock_audio_get_file_pos(lua_State *L)
{
	(void)L;
	int result = rb->audio_get_file_pos();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_get_action(lua_State *L)
{
	int context = (int) luaL_checkint(L, 1);
	int timeout = (int) luaL_checkint(L, 2);
	int result = rb->get_action(context, timeout);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_action_userabort(lua_State *L)
{
	int timeout = (int) luaL_checkint(L, 1);
	bool result = rb->action_userabort(timeout);
	lua_pushboolean(L, result);
	return 1;
}

static int rock_battery_level(lua_State *L)
{
	(void)L;
	int result = rb->battery_level();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_battery_level_safe(lua_State *L)
{
	(void)L;
	bool result = rb->battery_level_safe();
	lua_pushboolean(L, result);
	return 1;
}

static int rock_battery_time(lua_State *L)
{
	(void)L;
	int result = rb->battery_time();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_battery_voltage(lua_State *L)
{
	(void)L;
	int result = rb->battery_voltage();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_srand(lua_State *L)
{
	unsigned int seed = (unsigned int) luaL_checkint(L, 1);
	rb->srand(seed);
	return 0;
}

static int rock_rand(lua_State *L)
{
	(void)L;
	int result = rb->rand();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_plugin_get_current_filename(lua_State *L)
{
	(void)L;
	char* result = rb->plugin_get_current_filename();
	lua_pushstring(L, result);
	return 1;
}

static int rock_talk_disable(lua_State *L)
{
	bool disable = luaL_checkboolean(L, 1);
	rb->talk_disable(disable);
	return 0;
}

static int rock_codec_run_proc(lua_State *L)
{
	(void)L;
	int result = rb->codec_run_proc();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_codec_close(lua_State *L)
{
	(void)L;
	int result = rb->codec_close();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_get_codec_filename(lua_State *L)
{
	int cod_spec = (int) luaL_checkint(L, 1);
	const char * result = rb->get_codec_filename(cod_spec);
	lua_pushstring(L, result);
	return 1;
}

static int rock_show_logo(lua_State *L)
{
	(void)L;
	int result = rb->show_logo();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_set_current_file(lua_State *L)
{
	const char* path = (const char*) luaL_checkstring(L, 1);
	rb->set_current_file(path);
	return 0;
}

static int rock_set_dirfilter(lua_State *L)
{
	int l_dirfilter = (int) luaL_checkint(L, 1);
	rb->set_dirfilter(l_dirfilter);
	return 0;
}

static int rock_led(lua_State *L)
{
	bool on = luaL_checkboolean(L, 1);
	rb->led(on);
	return 0;
}

static int rock_bufclose(lua_State *L)
{
	int handle_id = (int) luaL_checkint(L, 1);
	bool result = rb->bufclose(handle_id);
	lua_pushboolean(L, result);
	return 1;
}

static int rock_bufseek(lua_State *L)
{
	int handle_id = (int) luaL_checkint(L, 1);
	size_t newpos = (size_t) luaL_checkint(L, 2);
	int result = rb->bufseek(handle_id, newpos);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_bufadvance(lua_State *L)
{
	int handle_id = (int) luaL_checkint(L, 1);
	off_t offset = (off_t) luaL_checkint(L, 2);
	int result = rb->bufadvance(handle_id, offset);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_bufcuttail(lua_State *L)
{
	int handle_id = (int) luaL_checkint(L, 1);
	size_t size = (size_t) luaL_checkint(L, 2);
	ssize_t result = rb->bufcuttail(handle_id, size);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_buf_handle_offset(lua_State *L)
{
	int handle_id = (int) luaL_checkint(L, 1);
	ssize_t result = rb->buf_handle_offset(handle_id);
	lua_pushinteger(L, result);
	return 1;
}

static int rock_buf_set_base_handle(lua_State *L)
{
	int handle_id = (int) luaL_checkint(L, 1);
	rb->buf_set_base_handle(handle_id);
	return 0;
}

static int rock_buf_used(lua_State *L)
{
	(void)L;
	size_t result = rb->buf_used();
	lua_pushinteger(L, result);
	return 1;
}

static int rock_settings_save(lua_State *L)
{
	(void)L;
	int result = rb->settings_save();
	lua_pushinteger(L, result);
	return 1;
}

const luaL_Reg rocklib_aux[] =
{
	{"lcd_update", rock_lcd_update},
	{"lcd_clear_display", rock_lcd_clear_display},
	{"lcd_putsxy", rock_lcd_putsxy},
	{"lcd_puts", rock_lcd_puts},
	{"lcd_puts_scroll", rock_lcd_puts_scroll},
	{"lcd_scroll_stop", rock_lcd_scroll_stop},
	{"lcd_update_rect", rock_lcd_update_rect},
	{"lcd_set_drawmode", rock_lcd_set_drawmode},
	{"lcd_get_drawmode", rock_lcd_get_drawmode},
	{"lcd_setfont", rock_lcd_setfont},
	{"lcd_drawpixel", rock_lcd_drawpixel},
	{"lcd_drawline", rock_lcd_drawline},
	{"lcd_hline", rock_lcd_hline},
	{"lcd_vline", rock_lcd_vline},
	{"lcd_drawrect", rock_lcd_drawrect},
	{"lcd_fillrect", rock_lcd_fillrect},
	{"lcd_set_foreground", rock_lcd_set_foreground},
	{"lcd_get_foreground", rock_lcd_get_foreground},
	{"lcd_set_background", rock_lcd_set_background},
	{"lcd_get_background", rock_lcd_get_background},
	{"font_load", rock_font_load},
	{"font_unload", rock_font_unload},
	{"get_codepage_name", rock_get_codepage_name},
	{"backlight_on", rock_backlight_on},
	{"backlight_off", rock_backlight_off},
	{"backlight_set_timeout", rock_backlight_set_timeout},
	{"is_backlight_on", rock_is_backlight_on},
	{"splash", rock_splash},
	{"button_get", rock_button_get},
	{"button_get_w_tmo", rock_button_get_w_tmo},
	{"button_status", rock_button_status},
	{"button_clear_queue", rock_button_clear_queue},
	{"button_queue_count", rock_button_queue_count},
	{"open_utf8", rock_open_utf8},
	{"remove", rock_remove},
	{"rename", rock_rename},
	{"storage_sleep", rock_storage_sleep},
	{"storage_spin", rock_storage_spin},
	{"storage_spindown", rock_storage_spindown},
	{"reload_directory", rock_reload_directory},
	{"create_numbered_filename", rock_create_numbered_filename},
	{"file_exists", rock_file_exists},
	{"strip_extension", rock_strip_extension},
	{"filetype_get_attr", rock_filetype_get_attr},
	{"app_mkdir", rock_app_mkdir},
	{"app_rmdir", rock_app_rmdir},
	{"dir_exists", rock_dir_exists},
	{"sleep", rock_sleep},
	{"yield", rock_yield},
	{"default_event_handler", rock_default_event_handler},
	{"thread_self", rock_thread_self},
	{"thread_exit", rock_thread_exit},
	{"thread_wait", rock_thread_wait},
	{"thread_thaw", rock_thread_thaw},
	{"thread_set_priority", rock_thread_set_priority},
	{"reset_poweroff_timer", rock_reset_poweroff_timer},
	{"commit_dcache", rock_commit_dcache},
	{"commit_discard_dcache", rock_commit_discard_dcache},
	{"commit_discard_idcache", rock_commit_discard_idcache},
	{"timer_unregister", rock_timer_unregister},
	{"timer_set_period", rock_timer_set_period},
	{"usb_acknowledge", rock_usb_acknowledge},
	{"strcpy", rock_strcpy},
	{"strlcpy", rock_strlcpy},
	{"strlen", rock_strlen},
	{"strrchr", rock_strrchr},
	{"strcmp", rock_strcmp},
	{"strcasecmp", rock_strcasecmp},
	{"strncasecmp", rock_strncasecmp},
	{"atoi", rock_atoi},
	{"strchr", rock_strchr},
	{"strcat", rock_strcat},
	{"strlcat", rock_strlcat},
	{"iso_decode", rock_iso_decode},
	{"utf16LEdecode", rock_utf16LEdecode},
	{"utf16BEdecode", rock_utf16BEdecode},
	{"utf8encode", rock_utf8encode},
	{"utf8length", rock_utf8length},
	{"utf8seek", rock_utf8seek},
	{"sound_set", rock_sound_set},
	{"sound_default", rock_sound_default},
	{"sound_min", rock_sound_min},
	{"sound_max", rock_sound_max},
	{"sound_unit", rock_sound_unit},
	{"sound_val2phys", rock_sound_val2phys},
	{"pcm_apply_settings", rock_pcm_apply_settings},
	{"pcm_play_stop", rock_pcm_play_stop},
	{"pcm_set_frequency", rock_pcm_set_frequency},
	{"pcm_is_playing", rock_pcm_is_playing},
	{"pcm_is_paused", rock_pcm_is_paused},
	{"pcm_play_pause", rock_pcm_play_pause},
	{"pcm_get_bytes_waiting", rock_pcm_get_bytes_waiting},
	{"pcm_play_lock", rock_pcm_play_lock},
	{"pcm_play_unlock", rock_pcm_play_unlock},
	{"beep_play", rock_beep_play},
	{"dsp_set_crossfeed_type", rock_dsp_set_crossfeed_type},
	{"dsp_set_space80_enable", rock_dsp_set_space80_enable},
	{"dsp_eq_enable", rock_dsp_eq_enable},
	{"dsp_dither_enable", rock_dsp_dither_enable},
	{"mixer_set_frequency", rock_mixer_set_frequency},
	{"mixer_get_frequency", rock_mixer_get_frequency},
	{"keyclick_click", rock_keyclick_click},
	{"playlist_amount", rock_playlist_amount},
	{"playlist_resume", rock_playlist_resume},
	{"playlist_resume_track", rock_playlist_resume_track},
	{"playlist_start", rock_playlist_start},
	{"playlist_add", rock_playlist_add},
	{"playlist_create", rock_playlist_create},
	{"playlist_shuffle", rock_playlist_shuffle},
	{"audio_stop", rock_audio_stop},
	{"audio_pause", rock_audio_pause},
	{"audio_resume", rock_audio_resume},
	{"audio_next", rock_audio_next},
	{"audio_prev", rock_audio_prev},
	{"audio_ff_rewind", rock_audio_ff_rewind},
	{"audio_status", rock_audio_status},
	{"audio_flush_and_reload_tracks", rock_audio_flush_and_reload_tracks},
	{"audio_get_file_pos", rock_audio_get_file_pos},
	{"get_action", rock_get_action},
	{"action_userabort", rock_action_userabort},
	{"battery_level", rock_battery_level},
	{"battery_level_safe", rock_battery_level_safe},
	{"battery_time", rock_battery_time},
	{"battery_voltage", rock_battery_voltage},
	{"srand", rock_srand},
	{"rand", rock_rand},
	{"plugin_get_current_filename", rock_plugin_get_current_filename},
	{"talk_disable", rock_talk_disable},
	{"codec_run_proc", rock_codec_run_proc},
	{"codec_close", rock_codec_close},
	{"get_codec_filename", rock_get_codec_filename},
	{"show_logo", rock_show_logo},
	{"set_current_file", rock_set_current_file},
	{"set_dirfilter", rock_set_dirfilter},
	{"led", rock_led},
	{"bufclose", rock_bufclose},
	{"bufseek", rock_bufseek},
	{"bufadvance", rock_bufadvance},
	{"bufcuttail", rock_bufcuttail},
	{"buf_handle_offset", rock_buf_handle_offset},
	{"buf_set_base_handle", rock_buf_set_base_handle},
	{"buf_used", rock_buf_used},
	{"settings_save", rock_settings_save},
	{NULL, NULL}
};

