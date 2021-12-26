//  gcc -o eddy eddy.c `pkg-config --cflags --libs eina efl elementary`

/* TODO:
 * Add callback for ok button to initialize `dd` function
 * Write dd function
 * callback for cancel button */

#include <Elementary.h>


//these functions are example code only.  Needs updating to be useful.

/* Need to use this one to get the ISO selected and set it to a visible entry*/
static void 
iso_chosen(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{
	const char *file = event_info;
	Evas_Object *entry = data;
	elm_object_text_set(entry, file);
	
	printf("File Chosen: %s\n",
          file ? file : "*none chosen!*");
    
}


/*Get md5sum file selected and set it to another visible entry*/
/*FIX ME: The **first time** cancel is selected it performs ok's actions. Every other times behaves as expected*/

static void 
md5_chosen(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
	const char *file = event_info;
	Evas_Object *entry = data;
	elm_object_text_set(entry, file);
	
	printf("File Chosen: %s\n",
          file ? file : "*none Chosen!*");
}

// Functions for ok (`dd` stuff) and cancel buttons callback



EAPI_MAIN int elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
	Evas_Object *win, *grid, *hbox, *ic1, *ic2, *ic3, *ic4;
	Evas_Object *iso_bt, *md5_bt, *dd_bt, *cancel_bt, *entry1, *entry2, *sep;

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	win = elm_win_util_standard_add("main", "Eddy - Live USB Utility");
	elm_win_autodel_set(win, EINA_TRUE);

	grid = elm_grid_add(win);
	evas_object_size_hint_weight_set(grid,EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, grid);
	evas_object_show(grid);

	ic1 = elm_icon_add(win);
	elm_icon_standard_set(ic1, "file");
	evas_object_size_hint_aspect_set(ic1,EVAS_ASPECT_CONTROL_HORIZONTAL,1,1);
	
	
	/* MD5 selector button */
	ic2 = elm_icon_add(win);
	elm_icon_standard_set(ic2, "file");
	evas_object_size_hint_aspect_set(ic2,EVAS_ASPECT_CONTROL_HORIZONTAL,1,1);

	md5_bt = elm_fileselector_button_add(grid);
	elm_fileselector_path_set(md5_bt, "/home/");
	elm_fileselector_button_inwin_mode_set(md5_bt, EINA_TRUE);
	elm_fileselector_expandable_set(md5_bt, EINA_FALSE);
	elm_fileselector_folder_only_set(md5_bt, EINA_FALSE);
	elm_fileselector_is_save_set(md5_bt, EINA_TRUE);
	elm_object_text_set(md5_bt, "Select MD5");
	evas_object_resize(md5_bt, 100, 30);
	elm_object_part_content_set(md5_bt, "icon", ic2);

	elm_grid_pack_set(md5_bt,3,3,22,8);
	evas_object_show(md5_bt);
	evas_object_show(ic2);
	
	/* MD5 text entry */
	entry2 = elm_entry_add(grid);
	elm_entry_line_wrap_set(entry2, EINA_FALSE);
	elm_entry_editable_set(entry2, EINA_FALSE);
	evas_object_size_hint_min_set(entry2,140,25);// The hint isn't enforced just used where appropriate. What are we trying to do?
	elm_grid_pack_set(entry2,30,3,42,8);
	evas_object_show(entry2);


	/* ISO selector button */
	iso_bt = elm_fileselector_button_add(grid);
	elm_fileselector_path_set(iso_bt, "/home/");
	elm_fileselector_button_inwin_mode_set(iso_bt, EINA_TRUE);
	elm_fileselector_expandable_set(iso_bt, EINA_FALSE);
	elm_fileselector_folder_only_set(iso_bt, EINA_FALSE);
	elm_fileselector_is_save_set(iso_bt, EINA_TRUE);
	elm_object_text_set(iso_bt, "Select ISO");
	evas_object_resize(iso_bt, 100, 30);
	elm_object_part_content_set(iso_bt, "icon", ic1);

	elm_grid_pack_set(iso_bt,3,11,22,8);
	evas_object_show(iso_bt);
	evas_object_show(ic1);

	/* ISO text entry */
	entry1 = elm_entry_add(grid);
	elm_entry_line_wrap_set(entry1, EINA_FALSE);
	elm_entry_editable_set(entry1, EINA_FALSE);
	evas_object_size_hint_min_set(entry1,140,25);
	elm_grid_pack_set(entry1,30,11,42,8);
	evas_object_show(entry1);
	
		// separator line
	sep = elm_separator_add(win);
	elm_separator_horizontal_set(sep, EINA_TRUE);
	elm_grid_pack(grid,sep,1,60,100,1);
	evas_object_show(sep);
	
	/* Create live usb button (ok) called dd_bt */
	ic3 = elm_icon_add(grid);
	elm_icon_standard_set(ic3, "Ok");
	evas_object_size_hint_aspect_set(ic3,EVAS_ASPECT_CONTROL_HORIZONTAL,1,1);
	
	dd_bt = elm_button_add(grid);
	elm_object_text_set(dd_bt, "Ok");
	evas_object_resize(dd_bt, 100, 30);
	elm_grid_pack_set(dd_bt,3,50,20,9); // change coordinates/values: x,y,w,h
	evas_object_show(dd_bt);
	evas_object_show(ic3);
	
	/* Cancel button.*/
	ic4 = elm_icon_add(grid);
	elm_icon_standard_set(ic4, "cancel");
	evas_object_size_hint_aspect_set(ic4,EVAS_ASPECT_CONTROL_HORIZONTAL,1,1);
	
	cancel_bt = elm_button_add(grid);
	elm_object_text_set(cancel_bt, "cancel");
	evas_object_resize(cancel_bt, 100, 30);
	elm_grid_pack_set(cancel_bt,25,50,20,9); // change coordinates/values: x,y,w,h
	evas_object_show(cancel_bt);
	evas_object_show(ic3);
	

	//add callbacks for buttons
	evas_object_smart_callback_add(iso_bt, "file,chosen", iso_chosen, entry1);
	evas_object_smart_callback_add(md5_bt, "file,chosen", md5_chosen, entry2);
	//evas_object_smart_callback_add(ok_bt, "ok,clicked" , clicked, make_iso);
	//evas_object_smart_callback_add(cancel_bt, "cancel,clicked, cancel, cancelled);

	evas_object_resize(win, 430, 340);
	//   evas_object_show(grid);
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
