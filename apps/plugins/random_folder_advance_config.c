/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Jonathan Gordon
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
#include "config.h"
#include "plugin.h"
#include "file.h"



static bool cancel;
static int fd;
static int dirs_count;
static int lasttick;
#define RFA_FILE ROCKBOX_DIR "/folder_advance_list.dat"
#define RFADIR_FILE ROCKBOX_DIR "/folder_advance_dir.txt"
#define RFA_FILE_TEXT ROCKBOX_DIR "/folder_advance_list.txt"
#define MAX_REMOVED_DIRS 10

char *buffer = NULL;
size_t buffer_size;
int num_replaced_dirs = 0;
char removed_dirs[MAX_REMOVED_DIRS][MAX_PATH];
struct file_format {
    int count;
    char folder[][MAX_PATH];
};
struct file_format *list = NULL;

static void update_screen(bool clear)
{
    char buf[15];

    rb->snprintf(buf,sizeof(buf),"Folders: %d",dirs_count);
    FOR_NB_SCREENS(i)
    {
        if(clear)
            rb->screens[i]->clear_display();
        rb->screens[i]->putsxy(0,0,buf);
        rb->screens[i]->update();
    }
}

static void traversedir(char* location, char* name)
{
    struct dirent *entry;
    DIR* dir;
    char fullpath[MAX_PATH], path[MAX_PATH];
    bool check = false;
    int i;

    /* behave differently if we're at root to avoid
       duplication of the initial slash later on */
    if (location[0] == '\0' && name[0] == '\0') {
        rb->strcpy(fullpath, "");
        dir = rb->opendir("/");
    } else {
        rb->snprintf(fullpath, sizeof(fullpath), "%s/%s", location, name);
        dir = rb->opendir(fullpath);
    }
    if (dir) {
        entry = rb->readdir(dir);
        while (entry) {
            if (cancel)
                break;
            /* Skip .. and . */
            if (entry->d_name[0] == '.')
            {
                if (    !rb->strcmp(entry->d_name,".")
                     || !rb->strcmp(entry->d_name,"..")
                     || !rb->strcmp(entry->d_name,".rockbox"))
                    check = false;
                else check = true;
            }
            else check = true;

        /* check if path is removed directory, if so dont enter it */
        rb->snprintf(path, MAX_PATH, "%s/%s", fullpath, entry->d_name);
        while(path[0] == '/')
            rb->strlcpy(path, path + 1, sizeof(path));
        for(i = 0; i < num_replaced_dirs; i++)
        {
            if(!rb->strcmp(path, removed_dirs[i]))
            {
                check = false;
                break;
            }
        }

            if (check)
            {
                struct dirinfo info = rb->dir_get_info(dir, entry);
                if (info.attribute & ATTR_DIRECTORY) {
                    char *start;
                    dirs_count++;
                    rb->snprintf(path,MAX_PATH,"%s/%s",fullpath,entry->d_name);
                    start = &path[rb->strlen(path)];
                    rb->memset(start,0,&path[MAX_PATH-1]-start);
                    rb->write(fd,path,MAX_PATH);
                    traversedir(fullpath, entry->d_name);
                }
            }
            if (*rb->current_tick - lasttick > (HZ/2)) {
                update_screen(false);
                lasttick = *rb->current_tick;
                if (rb->action_userabort(TIMEOUT_NOBLOCK))
                {
                    cancel = true;
                    break;
                }
            }

            entry = rb->readdir(dir);
        }
        rb->closedir(dir);
    }
}

static bool custom_dir(void)
{
    DIR* dir_check;
    char *starts, line[MAX_PATH], formatted_line[MAX_PATH];
    static int fd2;
    char buf[11];
    int i, errors = 0;

    /* populate removed dirs array */
    if((fd2 = rb->open(RFADIR_FILE,O_RDONLY)) >= 0)
    {
        while ((rb->read_line(fd2, line, MAX_PATH - 1)) > 0)
        {
            if ((line[0] == '-') && (line[1] == '/') &&
                     (num_replaced_dirs < MAX_REMOVED_DIRS))
            {
                num_replaced_dirs ++;
                rb->strlcpy(removed_dirs[num_replaced_dirs - 1], line + 2,
                                sizeof(line));
            }
        }
        rb->close(fd2);
    }

    if((fd2 = rb->open(RFADIR_FILE,O_RDONLY)) >= 0)
    {
        while ((rb->read_line(fd2, line, MAX_PATH - 1)) > 0)
        {
            /* blank lines and removed dirs ignored */
            if (rb->strlen(line) && ((line[0] != '-') || (line[1] != '/')))
            {
                /* remove preceeding '/'s from the line */
                while(line[0] == '/')
                    rb->strlcpy(line, line + 1, sizeof(line));

                rb->snprintf(formatted_line, MAX_PATH, "/%s", line);

                dir_check = rb->opendir(formatted_line);

                if (dir_check)
                {
                    rb->closedir(dir_check);
                    starts = &formatted_line[rb->strlen(formatted_line)];
                    rb->memset(starts, 0, &formatted_line[MAX_PATH-1]-starts);
                    bool write_line = true;

                    for(i = 0; i < num_replaced_dirs; i++)
                    {
                        if(!rb->strcmp(line, removed_dirs[i]))
                        {
                             write_line = false;
                             break;
                        }
                    }

                    if(write_line)
                    {
                        dirs_count++;
                        rb->write(fd, formatted_line, MAX_PATH);
                    }

                    traversedir("", line);
                }
                else
                {
                     errors ++;
                     rb->snprintf(buf,sizeof(buf),"Not found:");
                     FOR_NB_SCREENS(i)
                     {
                         rb->screens[i]->puts(0,0,buf);
                         rb->screens[i]->puts(0, errors, line);
                     }
                     update_screen(false);
                }
            }
        }
        rb->close(fd2);
        if(errors)
            /* Press button to continue */
            rb->get_action(CONTEXT_STD, TIMEOUT_BLOCK); 
    }
    else
        return false;
    return true;
}

static void generate(void)
{
    dirs_count = 0;
    cancel = false;
    fd = rb->open(RFA_FILE,O_CREAT|O_WRONLY, 0666);
    rb->write(fd,&dirs_count,sizeof(int));
    if (fd < 0)
    {
        rb->splashf(HZ, "Couldnt open %s", RFA_FILE);
        return;
    }
#ifndef HAVE_LCD_CHARCELLS
    update_screen(true);
#endif
    lasttick = *rb->current_tick;

    if(!custom_dir())
        traversedir("", "");

    rb->lseek(fd,0,SEEK_SET);
    rb->write(fd,&dirs_count,sizeof(int));
    rb->close(fd);
    rb->splash(HZ, "Done");
}

static const char* list_get_name_cb(int selected_item, void* data,
                                    char* buf, size_t buf_len)
{
    (void)data;
    rb->strlcpy(buf, list->folder[selected_item], buf_len);
    return buf;
}

static int load_list(void)
{
    int myfd = rb->open(RFA_FILE,O_RDONLY);
    if (myfd < 0)
        return -1;
    buffer = rb->plugin_get_audio_buffer(&buffer_size);
    if (!buffer)
    {
        return -2;
    }
    
    rb->read(myfd,buffer,buffer_size);
    rb->close(myfd);
    list = (struct file_format *)buffer;
    
    return 0;
}

static int save_list(void)
{
    int myfd = rb->creat(RFA_FILE, 0666);
    if (myfd < 0)
    {
        rb->splash(HZ, "Could Not Open " RFA_FILE);
        return -1;
    }
    int dirs_count = 0, i = 0;
    rb->write(myfd,&dirs_count,sizeof(int));
    for ( ;i<list->count;i++)
    {
        if (list->folder[i][0] != ' ')
        {
            dirs_count++;
            rb->write(myfd,list->folder[i],MAX_PATH);
        }
    }
    rb->lseek(myfd,0,SEEK_SET);
    rb->write(myfd,&dirs_count,sizeof(int));
    rb->close(myfd);
    
    return 1;
}

static int edit_list(void)
{
    struct gui_synclist lists;
    bool exit = false;
    int button,i;
    int selection, ret = 0;
    
    /* load the dat file if not already done */
    if ((list == NULL || list->count == 0) && (i = load_list()) != 0)
    {
        rb->splashf(HZ*2, "Could not load %s, rv = %d", RFA_FILE, i);
        return -1;
    }
    
    dirs_count = list->count;
    
    rb->gui_synclist_init(&lists,list_get_name_cb,0, false, 1, NULL);
    rb->gui_synclist_set_icon_callback(&lists,NULL);
    rb->gui_synclist_set_nb_items(&lists,list->count);
    rb->gui_synclist_limit_scroll(&lists,true);
    rb->gui_synclist_select_item(&lists, 0);
    
    while (!exit)
    {
        rb->gui_synclist_draw(&lists);
        button = rb->get_action(CONTEXT_LIST,TIMEOUT_BLOCK);
        if (rb->gui_synclist_do_button(&lists,&button,LIST_WRAP_UNLESS_HELD))
            continue;
        selection = rb->gui_synclist_get_sel_pos(&lists);
        switch (button)
        {
            case ACTION_STD_OK:
                list->folder[selection][0] = ' ';
                list->folder[selection][1] = '\0';
                break;
            case ACTION_STD_CONTEXT:
            {
                int len;
                MENUITEM_STRINGLIST(menu, "Remove Menu", NULL,
                                    "Remove Folder", "Remove Folder Tree");

                switch (rb->do_menu(&menu, NULL, NULL, false))
                {
                    case 0:
                        list->folder[selection][0] = ' ';
                        list->folder[selection][1] = '\0';
                        break;
                    case 1:
                    {
                        char temp[MAX_PATH];
                        rb->strcpy(temp,list->folder[selection]);
                        len = rb->strlen(temp);
                        for (i=0;i<list->count;i++)
                        {
                            if (!rb->strncmp(list->folder[i],temp,len))
                            {
                                list->folder[i][0] = ' ';
                                list->folder[i][1] = '\0';
                            }
                        }
                    }
                    break;
                }
            }
            break;
            case ACTION_STD_CANCEL:
            {
                MENUITEM_STRINGLIST(menu, "Exit Menu", NULL,
                                    "Save and Exit", "Ignore Changes and Exit");

                switch (rb->do_menu(&menu, NULL, NULL, false))
                {
                    case 0:
                        save_list();
                    case 1:
                        exit = true;
                        ret = -2;
                }
            }
            break;
        }
    }
    return ret;
}

static int export_list_to_file_text(void)
{
    int i = 0;
    /* load the dat file if not already done */
    if ((list == NULL || list->count == 0) && (i = load_list()) != 0)
    {
        rb->splashf(HZ*2, "Could not load %s, rv = %d", RFA_FILE, i);
        return 0;
    }
        
    if (list->count <= 0)
    {
        rb->splashf(HZ*2, "no dirs in list file: %s", RFA_FILE);
        return 0;
    }
        
    /* create and open the file */
    int myfd = rb->creat(RFA_FILE_TEXT, 0666);
    if (myfd < 0)
    {
        rb->splashf(HZ*4, "failed to open: fd = %d, file = %s", 
            myfd, RFA_FILE_TEXT);
        return -1;
    }
    
    /* write each directory to file */
    for (i = 0; i < list->count; i++)
    {
        if (list->folder[i][0] != ' ')
        {
            rb->fdprintf(myfd, "%s\n", list->folder[i]);
        }
    }
    
    rb->close(myfd);
    rb->splash(HZ, "Done");
    return 1;
}

static int import_list_from_file_text(void)
{
    char line[MAX_PATH];
    
    buffer = rb->plugin_get_audio_buffer(&buffer_size);
    if (buffer == NULL)
    {
        rb->splash(HZ*2, "failed to get audio buffer");
        return -1;
    }

    int myfd = rb->open(RFA_FILE_TEXT, O_RDONLY);
    if (myfd < 0)
    {
        rb->splashf(HZ*2, "failed to open: %s", RFA_FILE_TEXT);
        return -1;
    }
    
    /* set the list structure, and initialize count */
    list = (struct file_format *)buffer;
    list->count = 0;
        
    while ((rb->read_line(myfd, line, MAX_PATH - 1)) > 0)
    {
        /* copy the dir name, and skip the newline */
        int len = rb->strlen(line);
        /* remove CRs */
        if (len > 0)
        {
            if (line[len-1] == 0x0A || line[len-1] == 0x0D)
                line[len-1] = 0x00;
            if (len > 1 && 
                (line[len-2] == 0x0A || line[len-2] == 0x0D))
                line[len-2] = 0x00;
        }
        
        rb->strcpy(list->folder[list->count++], line);
    }
    
    rb->close(myfd);
    
    if (list->count == 0)
    {
        load_list();
    }
    else
    {
        save_list();
    }
    rb->splash(HZ, "Done");
    return list->count;
}

static int start_shuffled_play(void)
{
    int *order;
    size_t max_shuffle_size;
    int i = 0;

    /* get memory for shuffling */
    order=rb->plugin_get_buffer(&max_shuffle_size);
    max_shuffle_size/=sizeof(int);
    if (order==NULL || max_shuffle_size==0)
    {
        rb->splashf(HZ*2, "Not enough memory for shuffling");
        return 0;
    }
 
    /* load the dat file if not already done */
    if ((list == NULL || list->count == 0) && (i = load_list()) != 0)
    {
        rb->splashf(HZ*2, "Could not load %s, rv = %d", RFA_FILE, i);
        return 0;
    }
        
    if (list->count <= 0)
    {
        rb->splashf(HZ*2, "no dirs in list file: %s", RFA_FILE);
        return 0;
    }

    /* shuffle the thing */
    rb->srand(*rb->current_tick);
    if(list->count>(int)max_shuffle_size)
    {
        rb->splashf(HZ*2, "Too many folders: %d (room for %d)", list->count,(int)max_shuffle_size);
        return 0;
    }
    for(i=0;i<list->count;i++)
        order[i]=i;
    
    for(i = list->count - 1; i >= 0; i--)
    {
        /* the rand is from 0 to RAND_MAX, so adjust to our value range */
        int candidate = rb->rand() % (i + 1);

        /* now swap the values at the 'i' and 'candidate' positions */
        int store = order[candidate];
        order[candidate] = order[i];
        order[i] = store;
    }
        
    /* We don't want whatever is playing */
    if (!(rb->playlist_remove_all_tracks(NULL) == 0
          && rb->playlist_create(NULL, NULL) == 0))
    {
        rb->splashf(HZ*2, "Could not clear playlist");
        return 0;
    }

    /* add the lot to the playlist */
    for (i = 0; i < list->count; i++)
    {
        if (list->folder[order[i]][0] != ' ')
        {
            rb->playlist_insert_directory(NULL,list->folder[order[i]],PLAYLIST_INSERT_LAST,false,false);
        }
        if (rb->action_userabort(TIMEOUT_NOBLOCK))
        {
           break;
        }
    }
    rb->splash(HZ, "Done");
    /* the core needs the audio buffer back in order to start playback. */
    list = NULL;
    rb->plugin_release_audio_buffer();
    rb->playlist_start(0, 0, 0);
    return 1;
}

static enum plugin_status main_menu(void)
{
    bool exit = false;
    MENUITEM_STRINGLIST(menu, "Main Menu", NULL,
                        "Generate Folder List",
                        "Edit Folder List",
                        "Export List To Textfile",
                        "Import List From Textfile",
                        "Play Shuffled",
                        "Quit");

    while (!exit)
    {
        switch (rb->do_menu(&menu, NULL, NULL, false))
        {
            case 0: /* generate */
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(true);
#endif
                generate();
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(false);
#endif
#ifdef HAVE_REMOTE_LCD
                rb->remote_backlight_on();
#endif
                rb->backlight_on();
                break;
            case 1:
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(true);
#endif
                if (edit_list() < 0)
                    exit = true;
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(false);
#endif
#ifdef HAVE_REMOTE_LCD
                rb->remote_backlight_on();
#endif
                rb->backlight_on();
                break;
            case 2: /* export to textfile */
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(true);
#endif
                export_list_to_file_text();
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(false);
#endif
#ifdef HAVE_REMOTE_LCD
                rb->remote_backlight_on();
#endif
                rb->backlight_on();
                break;
            case 3: /* import from textfile */
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(true);
#endif
                import_list_from_file_text();
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
                rb->cpu_boost(false);
#endif
#ifdef HAVE_REMOTE_LCD
                rb->remote_backlight_on();
#endif
                rb->backlight_on();
                break;
            case 4:
                if (!start_shuffled_play())
                    return PLUGIN_ERROR;
                else
                    return PLUGIN_GOTO_WPS;
            case 5:
                return PLUGIN_OK;
        }
    }
    return PLUGIN_OK;
}

enum plugin_status plugin_start(const void* parameter)
{
    (void)parameter;
#ifdef HAVE_TOUCHSCREEN
    rb->touchscreen_set_mode(rb->global_settings->touch_mode);
#endif

    cancel = false;
    
    return main_menu();
}
