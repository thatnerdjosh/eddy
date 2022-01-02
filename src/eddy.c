	/*************************************************************
	* Eddy is a simple live USB creation utility.                *
	*                                                            *
	* Authors                                                    *
	*                                                            *
	* Doug Yanez            <Deepspeed@bodhilinux.com>           *
	*                                                            *
	* Contributors:                                              *
	*                                                            *
	* Gareth Williams	<gareth.m.williams@gmail.com>        *
	* 	                                                     *
	* Official upstream:  https://github.com/Deepspeed/eddy      *
	*                                                            *
	* License:  GPL v2                                           *
	**************************************************************
//compile with
//  gcc -o eddy eddy.c `pkg-config --cflags --libs eina efl elementary`


/* TODO:
 *
 * Get drive selector working
 *
 * Get drive checker working
 *
 */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <Elementary.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 1024



Evas_Object *entry2, *lb2, *pb;


/* function for child process to finish md5 check and show results properly */

static Eina_Bool
md5_msg_handler(void *d EINA_UNUSED, int t EINA_UNUSED, void *event)
{
	Ecore_Exe_Event_Data *dataFromProcess = (Ecore_Exe_Event_Data *)event;
	int i;
	int j=0;
	char msg[BUFFER_SIZE];
	char result[PATH_MAX];//final parsed result
	char str[] = "<align=left>Md5sum checked: ";

	if (dataFromProcess->size >= (BUFFER_SIZE - 1)){
		printf("Data too big for bugger. error\n");
		return ECORE_CALLBACK_DONE;
	}

	strncpy(msg, dataFromProcess->data, dataFromProcess->size);
	msg[dataFromProcess->size] = 0;
	printf("%s\n", msg);

	if (strcmp(msg, "quit") == 0){
		printf("My child said to me, QUIT!\n");
		ecore_main_loop_quit();
	}
	else{  //set results
		i = strlen(msg);
		while(msg[i] != ':')//iterate backwards to :
			i--;

		for(i += 2; msg[i] != '\0'; i++, j++)//store result separately
			result[j] = msg[i];

		strcat(str, result);//format text for label

		elm_object_text_set(lb2, str);//show test results
	}
	elm_progressbar_pulse(pb, EINA_FALSE);
	return ECORE_CALLBACK_DONE;
}



/* get the ISO selected and set it to a visible entry*/
static void
iso_chosen(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{

	const char *file = event_info;
	char buf[PATH_MAX];
	Evas_Object *lb1 = data;

	if(!file)
		return;

	int i = strlen(file);
	//filetype filter
	if(file[i-4]!='.'&&file[i-3]!='i'&&file[i-2]!='s'&&file[i-1]!='o'){
		printf("Wrong file type!  Try again.\n");
		elm_object_text_set(lb1,"<align=left>Please choose .iso file");
		return;
	}
	snprintf(buf, sizeof(buf), "<align=left>%s", file);
	elm_object_text_set(lb1, buf);
}





/* Check selected md5 file against ISO.  ISO must be in same folder. */

static void
md5_check(void *data, Evas_Object *o EINA_UNUSED, void *e)
{
	EINA_SAFETY_ON_NULL_RETURN(data);

	//ecore_exe stuff
	pid_t childPid;
	Ecore_Exe *childHandle;

	//filepaths
	char *isoPath = NULL;
	char md5Path[PATH_MAX+8];
	char *folderPath = NULL;
	char command[PATH_MAX+26];//terminal command
	char output[PATH_MAX];//command output
	char result[PATH_MAX];//final parsed result

	const char *tmpPath = elm_object_text_get(data);

	/* remove extra <align=left> bit */
	isoPath = elm_entry_markup_to_utf8(tmpPath);

	if(!strcmp(tmpPath, "<br>")){
		printf("No .iso file chosen yet!\n");
		elm_object_text_set(lb2,"<align=left>No .iso file chosen yet!");
		return;
	}

	elm_progressbar_pulse(pb,EINA_TRUE);
	elm_object_text_set(lb2, "<align=left>Checking ISO md5sum");

	/* set folderPath directory */
	folderPath = ecore_file_dir_get(isoPath);

	//build terminal command
	snprintf(md5Path, PATH_MAX+7,  "%s.md5", isoPath);
	snprintf(command,PATH_MAX+25,"cd %s && md5sum -c %s",folderPath,md5Path);

	free(isoPath);
	free(folderPath);

	//check for md5 file existence
	if(!ecore_file_exists(md5Path)){
		printf("md5 file does not exist in this folder!\n");
		elm_object_text_set(lb2, "<align=left>.md5 file not found!");
		elm_progressbar_pulse(pb, EINA_FALSE);
		return;
	}

	//readability check
	if(!ecore_file_can_read(md5Path)){
		printf("md5 file cannot be read!\n");
		elm_object_text_set(lb2,"<align=left>.md5 file unreadable!");
		elm_progressbar_pulse(pb, EINA_FALSE);
		return;
	}

	/* execute md5 check. */
	childHandle = ecore_exe_pipe_run(command,
					ECORE_EXE_PIPE_WRITE |
					ECORE_EXE_PIPE_READ_LINE_BUFFERED |
					ECORE_EXE_PIPE_READ, NULL);

	if (childHandle == NULL){
		fprintf(stderr, "Could not create a child process!\n");
		elm_progressbar_pulse(pb, EINA_FALSE);
		return;
	}

	childPid = ecore_exe_pid_get(childHandle);

	if (childPid == -1)
		fprintf(stderr, "Could not retrieve the PID!\n");
	else
		printf("Child process PID:%u\n",(unsigned int)childPid);

	ecore_event_handler_add(ECORE_EXE_EVENT_DATA, md5_msg_handler, NULL);
}




static void
usb_check(void *data EINA_UNUSED,Evas_Object *o EINA_UNUSED, void *e)
{
	printf("USB Check\n");

	/* use ecore_exe to run badblocks command on chosen USB drive. */
}




static void
make_usb(void *data EINA_UNUSED,Evas_Object *o EINA_UNUSED, void *e)
{
	printf("make_usb running\n");

	/* use ecore_exe to install the chosen iso from entry1
	 * onto the selected drive from the genlist using dd
	 */
}




/* help window */
static void
help_info(void *data EINA_UNUSED,Evas_Object *object EINA_UNUSED,void *event_info)
{
   Evas_Object *help, *scroller, *label;

	/* help main window */
	help = elm_win_util_standard_add("Help", "Help");
	elm_win_autodel_set(help, EINA_TRUE);
	evas_object_size_hint_min_set(help,270,150); //min size not working??

	label = elm_label_add(help);
	elm_object_text_set(label, "To perform an MD5sum check on your iso,<br>\
	both files (.iso and .md5) MUST be in the<br>\
	same folder and you must select an iso.<br><br>\
	Opening Eddy from the command line using: <br>\
	./eddy -f ~/path/to/iso-file.iso<br>\
	will fill the iso field in Eddy<br>\
	Expect more features as time goes on.");
	efl_gfx_hint_align_set(label,0.0,1);
	evas_object_show(label);

	scroller = elm_scroller_add(help);
	evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(help, scroller);
	evas_object_show(scroller);
	elm_object_content_set(scroller, label);
	elm_scroller_bounce_set(scroller, EINA_TRUE, EINA_FALSE);
	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
	elm_scroller_propagate_events_set(scroller, EINA_TRUE);
	elm_scroller_page_relative_set(scroller, 0, 1);
	elm_scroller_region_show(scroller, 50, 50, 200, 200);
	evas_object_resize(help, 270,150);

	evas_object_show(help);
}



/* example functions for combobox and genlist */

static char *
gl_text_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part EINA_UNUSED)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "Item # %i", (int)(uintptr_t)data);
   return strdup(buf);
}


static Eina_Bool
gl_state_get(void *data EINA_UNUSED,
             Evas_Object *obj EINA_UNUSED,
             const char *part EINA_UNUSED)
{
   return EINA_FALSE;
}


static Eina_Bool
gl_filter_get(void *data, Evas_Object *obj EINA_UNUSED, void *key)
{
   // if the key is empty/NULL, return true for item
   if (!strlen((char *)key)) return EINA_TRUE;
   char buf[256];
   snprintf(buf, sizeof(buf), "Item # %i", (int)(uintptr_t)data);
   if (strcasestr(buf, (char *)key))
     return EINA_TRUE;
   // Default case should return false (item fails filter hence will be hidden)
   return EINA_FALSE;
}


/* get drive selector working */
EAPI_MAIN int elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
	Evas_Object *win, *table, *hbox, *ic1, *ic2, *entry1, *entry3, *lb1, *lb3;
	Evas_Object *iso_bt, *md5_check_bt, *usb_check_bt, *dd_bt;
	Evas_Object *help_bt, *sep, *sep2, *combo;
	Elm_Genlist_Item_Class *glist;

	/* look for and perform any cli args
	 * will run with './eddy -f /path/to/file.iso' */
	char path[PATH_MAX]; 
	int hold = -10;
	
	for (int i=0; argv[i] != NULL; i++){
		if(strcmp(argv[i], "-f") == 0) hold = i+1;
		snprintf(path, sizeof(path), "<align=left>%s", argv[hold]);
	}
	

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	//set up window
	win = elm_win_util_standard_add("main", "Eddy - Live USB Utility");
	elm_win_autodel_set(win, EINA_TRUE);

	elm_app_name_set("Eddy");

	/* table, replacing grid */
	table = elm_table_add(win);
	elm_table_padding_set(table, 5, 5);
	elm_table_align_set(table,0.0,0.5);
	evas_object_size_hint_weight_set(table,EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win,table);
	evas_object_show(table);


	/* ISO selector button */

	iso_bt = elm_fileselector_button_add(table);
	elm_fileselector_button_inwin_mode_set(iso_bt, EINA_TRUE);
	elm_fileselector_expandable_set(iso_bt, EINA_FALSE);
	elm_fileselector_folder_only_set(iso_bt, EINA_FALSE);
	elm_fileselector_is_save_set(iso_bt, EINA_TRUE);
	elm_object_text_set(iso_bt, "Select ISO");
	evas_object_size_hint_align_set(iso_bt, EVAS_HINT_FILL, 0.5);

	elm_table_pack(table,iso_bt,0,0,1,1);
	evas_object_show(iso_bt);


	/* ISO text label */
	lb1 = elm_label_add(table);
	elm_label_ellipsis_set(lb1, EINA_TRUE);
	evas_object_size_hint_weight_set(lb1, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(lb1, EVAS_HINT_FILL, 0.0);


	/* label is created, now we can assign the argv to the label. */
	if (hold != -10){  //if it has changed
		elm_object_text_set(lb1, path);
	}
	else elm_object_text_set(lb1, "\0");

	elm_table_pack(table, lb1, 1,0,5,1);
	evas_object_show(lb1);


	/* Md5 Check Button */
	md5_check_bt = elm_button_add(table);
	elm_button_autorepeat_set(md5_check_bt, EINA_FALSE);
	elm_object_text_set(md5_check_bt, "Check md5");
	evas_object_size_hint_align_set(md5_check_bt, EVAS_HINT_FILL, 0.5);

	elm_table_pack(table,md5_check_bt,0,1,1,1);
	evas_object_show(md5_check_bt);


	/* md5 text label */
	lb2 = elm_label_add(table);
	elm_label_ellipsis_set(lb2, EINA_TRUE);
	evas_object_size_hint_weight_set(lb2, EVAS_HINT_EXPAND, 1.0);
	evas_object_size_hint_align_set(lb2, EVAS_HINT_FILL, 0.0);

	elm_table_pack(table, lb2, 1,1,5,1);
	evas_object_show(lb2);


	/* USB chooser box */
	combo = elm_combobox_add(table);
	elm_object_part_text_set(combo, "Chooser", "Chose USB Drive");
	evas_object_size_hint_weight_set(combo, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(combo, EVAS_HINT_FILL, 0.5);

	elm_table_pack(table, combo, 0,3,1,1);
	evas_object_show(combo);


	//example code
	glist = elm_genlist_item_class_new();
	glist->item_style = "default";
	glist->func.text_get = gl_text_get;
	glist->func.content_get = NULL;
	glist->func.state_get = gl_state_get;
	glist->func.filter_get = gl_filter_get;
	glist->func.del = NULL;

	//more examples
	for (int i = 0; i < 10; i++)
		elm_genlist_item_append(combo, glist, (void *)(uintptr_t)i,
					NULL, ELM_GENLIST_ITEM_NONE, NULL,
					(void*)(uintptr_t)(i * 10));


	/* USB Check Button */
	usb_check_bt = elm_button_add(table);
	elm_button_autorepeat_set(usb_check_bt, EINA_FALSE);
	elm_object_text_set(usb_check_bt, "Check USB");
	evas_object_size_hint_align_set(usb_check_bt, EVAS_HINT_FILL, 0.5);

	elm_table_pack(table,usb_check_bt,0,4,1,1);
	evas_object_show(usb_check_bt);


	/* USB result label */
	lb3 = elm_label_add(table);
	elm_label_ellipsis_set(lb3, EINA_TRUE);
	evas_object_size_hint_weight_set(lb3, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(lb3, EVAS_HINT_FILL, 0.5);

	elm_table_pack(table, lb3, 1,4,5,1);
	evas_object_show(lb3);


	/* DD buttom */
	dd_bt = elm_button_add(table);
	elm_button_autorepeat_set(dd_bt, EINA_FALSE);
	elm_object_text_set(dd_bt, "Make Live USB");
	evas_object_size_hint_align_set(dd_bt, EVAS_HINT_FILL, 0.5);

	elm_table_pack(table,dd_bt,0,12,1,1);
	evas_object_show(dd_bt);


	/* help button */
	help_bt = elm_button_add(table);
	elm_button_autorepeat_set(help_bt, EINA_FALSE);
	elm_object_text_set(help_bt, "?");
	evas_object_size_hint_align_set(help_bt, EVAS_HINT_FILL, 0.5);

	elm_table_pack(table,help_bt,5,13,1,1);
	evas_object_show(help_bt);


	/* progress bar */

	//totally broken... Pulsing isn't working at all.
	pb = elm_progressbar_add(table);
	evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_progressbar_pulse_set(pb, EINA_TRUE);
	elm_progressbar_unit_format_set(pb, NULL);

	elm_table_pack(table,pb,0,14,6,1);
	evas_object_show(pb);

	//add callbacks for buttons
	evas_object_smart_callback_add(iso_bt,"file,chosen",iso_chosen, lb1);
	evas_object_smart_callback_add(md5_check_bt,"clicked",md5_check,lb1);
	evas_object_smart_callback_add(usb_check_bt,"clicked",usb_check,NULL);
	evas_object_smart_callback_add(dd_bt,"clicked",make_usb,NULL);
	evas_object_smart_callback_add(help_bt,"clicked",help_info,NULL);

	/* set final window size and display it */
	evas_object_resize(win, ELM_SCALE_SIZE(420), ELM_SCALE_SIZE(300));
	evas_object_show(table);  //is this needed?
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
