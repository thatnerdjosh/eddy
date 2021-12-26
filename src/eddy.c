//  gcc -o eddy eddy.c `pkg-config --cflags --libs eina efl elementary`

/* TODO:
 * add md5 checker button and make it work
 * add USB drive selector and checker
 * add ISO installer button using dd command */

#include <Elementary.h>


//these functions are example code only.  Needs updating to be useful.

/* get the ISO selected and set it to a visible entry*/
static void 
iso_chosen(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{
/*need to add error handling in case a directory is selected*/
	const char *file = event_info;
	Evas_Object *entry = data;
	elm_object_text_set(entry, file);
	
	printf("File Chosen: %s\n", file ? file : "*none chosen!*");
    
}


/*Get md5sum file selected and set it to another visible entry*/

static void 
md5_chosen(void *data EINA_UNUSED,Evas_Object *obj EINA_UNUSED,void *event_info)
{
/*need to add error handling in case a directory is selected*/
	const char *file = event_info;
	Evas_Object *entry = data;
	elm_object_text_set(entry, file);
	
	printf("File Chosen: %s\n", file ? file : "*none Chosen!*");
}


/* Check selected md5 file against ISO.  ISO must be in same folder. */

static void 
md5_check(void *d EINA_UNUSED,Evas_Object *o EINA_UNUSED,void *event_info)
{
	printf("Under Construction!\n");
}

/* Get help window */
static void 
help_info(void *data EINA_UNUSED,Evas_Object *object EINA_UNUSED,void *event_info)
{
	printf("Help Under Construction!\n");
}

EAPI_MAIN int elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
	Evas_Object *win, *grid, *hbox, *ic1, *ic2, *entry1, *entry2, *sep;
	Evas_Object *iso_bt, *md5_bt, *md5_check_bt, *iso_check_bt, *dd_bt, *help_bt;
	//still need a few labels or icons later.

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	//set up window
	win = elm_win_util_standard_add("main", "Eddy - Live USB Utility");
	elm_win_autodel_set(win, EINA_TRUE);
	
	evas_object_size_hint_min_set(win,420,300); //min window size

	//make grid
	grid = elm_grid_add(win);
	evas_object_size_hint_weight_set(grid,EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, grid);
	evas_object_show(grid);

	/*need to add filters to the fileselectors so they only show the
	* relevant file types for each button*/
		
	/* MD5 selector button */

	md5_bt = elm_fileselector_button_add(grid);
	elm_fileselector_path_set(md5_bt, "/home/");
	elm_fileselector_button_inwin_mode_set(md5_bt, EINA_TRUE);
	elm_fileselector_expandable_set(md5_bt, EINA_FALSE);
	elm_fileselector_folder_only_set(md5_bt, EINA_FALSE);
	elm_fileselector_is_save_set(md5_bt, EINA_TRUE);
	elm_object_text_set(md5_bt, "Select MD5");

	elm_grid_pack_set(md5_bt,2,2,26,11);//this sets size, not evas resize.
	evas_object_show(md5_bt);
	
	/* MD5 text entry */
	entry1 = elm_entry_add(grid);
	elm_entry_line_wrap_set(entry1, EINA_FALSE);
	elm_entry_editable_set(entry1, EINA_FALSE);
	elm_grid_pack_set(entry1,30,2,50,11);
	evas_object_show(entry1);
	
		
	/* Md5 Check Button */
	md5_check_bt = elm_button_add(grid);
	elm_button_autorepeat_set(md5_check_bt, EINA_FALSE);
	elm_object_text_set(md5_check_bt, "Check md5sum");
	elm_grid_pack_set(md5_check_bt,2,14,26,11);
	evas_object_show(md5_check_bt);
	
	// separator line
	sep = elm_separator_add(win);
	elm_separator_horizontal_set(sep, EINA_TRUE);
	elm_grid_pack(grid,sep,1,27,100,1);
	evas_object_show(sep);	

	
	
	/*TODO: move ISO button into group with final DD button and use only
	* md5 button for checking the md5 since that's all we need anyway.

	/* ISO selector button */
	iso_bt = elm_fileselector_button_add(grid);
	elm_fileselector_path_set(iso_bt, "/home/");
	elm_fileselector_button_inwin_mode_set(iso_bt, EINA_TRUE);
	elm_fileselector_expandable_set(iso_bt, EINA_FALSE);
	elm_fileselector_folder_only_set(iso_bt, EINA_FALSE);
	elm_fileselector_is_save_set(iso_bt, EINA_TRUE);
	elm_object_text_set(iso_bt, "Select ISO");

	elm_grid_pack_set(iso_bt,2,30,26,11);
	evas_object_show(iso_bt);

	/* ISO text entry */
	entry2 = elm_entry_add(grid);
	elm_entry_line_wrap_set(entry2, EINA_FALSE);
	elm_entry_editable_set(entry2, EINA_FALSE);
	elm_grid_pack_set(entry2,30,30,42,11);
	evas_object_show(entry2);

	/* help button */
	help_bt = elm_button_add(grid);
	elm_button_autorepeat_set(help_bt, EINA_FALSE);
	elm_object_text_set(help_bt, "Help");
	elm_grid_pack_set(help_bt,70,89,30,10);
	evas_object_show(help_bt);
	

	
	//add callbacks for buttons
	evas_object_smart_callback_add(md5_bt,"file,chosen",md5_chosen, entry1);
	evas_object_smart_callback_add(iso_bt,"file,chosen",iso_chosen, entry2);
	evas_object_smart_callback_add(md5_check_bt,"clicked",md5_check,NULL);
	evas_object_smart_callback_add(help_bt,"clicked",help_info,NULL);
	
	/* set final window size and display it */
	evas_object_resize(win, 420, 300);//start at min size
	//evas_object_show(grid);
	evas_object_show(win);

	elm_run();

	return 0;
}
ELM_MAIN()



/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
