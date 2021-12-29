//  gcc -o eddy eddy.c `pkg-config --cflags --libs eina efl elementary`

/* TODO:
 *
 * Add error handling for file types selected
 *
 * Show result of md5 check in text entry
 *
 * Get progress bar working
 *
 * add USB drive selector and checker
 *
 * add ISO installer button using dd command 
 */

#include <Elementary.h>

Evas_Object *entry3;

//progressbar struct
typedef struct Progbar
{
	Evas_Object *pb1;
	Eina_Bool    run;
	Ecore_Timer *timer;
} Progbar;

static Progbar pbar;

/* more progressbar setup */
static Eina_Bool
progressbar_go(void *data EINA_UNUSED)
{
   double progress;
   progress = elm_progressbar_value_get(pbar.pb1);
   if (progress < 1.0) progress += 0.0123;
   else progress = 0.0;
   /* just the non-pulsing ones need an update */
   elm_progressbar_value_set(pbar.pb1, progress);
   if (progress < 1.0) return ECORE_CALLBACK_RENEW;
   pbar.run = 0;
   return ECORE_CALLBACK_CANCEL;
}

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
	
	int i = strlen(file);
	
	//filter chosen file type.
	if(file[i-3] != 'm' && file[i-2] != 'd' && file[i-1] != '5'){
		printf("Wrong file type!  Try again.\n");
		return;
	}
	
	elm_object_text_set(entry, file);
	
	printf("File Chosen: %s\n", file ? file : "*none Chosen!*");
}


/* Check selected md5 file against ISO.  ISO must be in same folder. */

/* Check selected md5 file against ISO.  ISO must be in same folder. */

//make progress bar to work with popen command.

static void 
md5_check(void *data, Evas_Object *o EINA_UNUSED, void *e)
{	
	int i;
	int j=0;
	FILE *ptr; //Our favorite pointer.  His name is Peter.
	char ch;
	char folderPath[PATH_MAX];
	char fileName[PATH_MAX];
	char output[PATH_MAX];
	char command[PATH_MAX];
	char result[PATH_MAX];
	const char *md5Path = elm_object_text_get(data);	
	
	if(!*md5Path) {
		printf("No md5 file chosen yet!\n");
		return;
	}
	/* set folderPath directory with string wizardry */
	for (i = strlen(md5Path); i > 0; i--){
		if (md5Path[i] != '/'){//ignore file name
			continue;
		}
		folderPath[i+1] = '\0';//pad string with null zero
		for (; i >= 0; i--){
			folderPath[i] = md5Path[i];//set folderPath
		}
		break;
	}
	
	//remove later
	printf("md5Path: %s\nfolderPath: %s\n", md5Path, folderPath);
	
	
	//build terminal command
	command[0] == '\0';
	strcpy(command, "cd ");
	strcat(command, folderPath);
	strcat(command, " && md5sum -c ");
	strcat(command, md5Path);
	
	printf("%s\n", command);//remove later
	
	/* how do I start the progress bar?? */
	elm_progressbar_pulse(pbar.pb1, EINA_TRUE);
	if (!pbar.run){
		pbar.timer = ecore_timer_add(0.1, progressbar_go, NULL);
		pbar.run = EINA_TRUE;
	}
	
	/*execute md5 check.  
	* md5sum requires .ISO and .md5 files to be in the same folder!
	*/
	ptr = popen(command, "r");
	
	if(!ptr){//error handling
		puts("Unable to open process");
		//need to quit in a better way.
		exit(0);//just give up.  Best error handler.
	}
	
	/*Capture each character of the command output and print it 
	* to the terminal while also storing it in an array for later.
	*/
	i=0;
	while( (ch=fgetc(ptr)) != EOF)  {
		putchar(ch);
		output[i] = ch;
		i++;
	}
	output[i+1] = '\0'; //add null 0 for safety
	//printf("%s\n", output);
	pclose(ptr); //Let Peter rest.  He worked hard today.
	
	//stop progress bar...  none of this works.
	elm_progressbar_pulse(pbar.pb1, EINA_FALSE);
	if (pbar.run){
		ecore_timer_del(pbar.timer);
		pbar.run = EINA_FALSE;
	}
	
		
	/* get md5sum result */
	while(output[i] != ':')
		i--;
	i += 2;
	while(output[i] != '\0'){
		result[j] = output[i];
		i++;
		j++;
	}
	result[j] = '\0';  //safety padding
	elm_object_text_set(entry3, result);
		
	
}




/* Get help window */
static void 
help_info(void *data EINA_UNUSED,Evas_Object *object EINA_UNUSED,void *event_info)
{
   Evas_Object *help, *scroller, *label;

	/* help main window */  
	help = elm_win_util_standard_add("Help", "Help");
	elm_win_autodel_set(help, EINA_TRUE);
	evas_object_size_hint_min_set(help,250,280); //min size not working??

	label = elm_label_add(help);
	elm_object_text_set(label, "To perform an MD5 check on your iso, <br> \
	both files MUST be in the same folder!");
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
	evas_object_resize(help, 300,185);

	evas_object_show(help);	
	printf("Help window opened\n");
}


EAPI_MAIN int elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
	Evas_Object *win, *grid, *hbox, *ic1, *ic2, *entry1, *entry2;
	Evas_Object *iso_bt, *md5_bt, *md5_check_bt, *iso_check_bt, *dd_bt;
	Evas_Object *help_bt, *sep, *pb;
	//still need a few labels or icons later.

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	//set up window
	win = elm_win_util_standard_add("main", "Eddy - Live USB Utility");
	elm_win_autodel_set(win, EINA_TRUE);

	elm_app_name_set("Eddy");

	/*this line fails after choosing a file??*/
	evas_object_size_hint_min_set(win,420,300); //min window size

	//make grid
	grid = elm_grid_add(win);
	evas_object_size_hint_weight_set(grid,EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, grid);
	evas_object_show(grid);

	/*need to add filters to the fileselectors so they only show the
	* relevant file types for each button if possible*/
		
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
	elm_grid_pack_set(entry1,33,3,60,11);
	evas_object_show(entry1);
	
		
	/* Md5 Check Button */
	md5_check_bt = elm_button_add(grid);
	elm_button_autorepeat_set(md5_check_bt, EINA_FALSE);
	elm_object_text_set(md5_check_bt, "Check md5sum");
	elm_grid_pack_set(md5_check_bt,2,14,26,11);
	evas_object_show(md5_check_bt);
	
	/* md5 result entry */
	entry3 = elm_entry_add(grid);
	elm_entry_line_wrap_set(entry3, EINA_FALSE);
	elm_entry_editable_set(entry3, EINA_FALSE);
	elm_grid_pack_set(entry3,33,15,60,11);
	evas_object_show(entry3);
	
	/* progress bar */
   pb = elm_progressbar_add(grid);
   evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_progressbar_pulse_set(pb, EINA_TRUE);
   elm_grid_pack_set(pb,0,93,100,8);
   evas_object_show(pb);
   pbar.pb1 = pb;
	
	// separator line
	sep = elm_separator_add(win);
	elm_separator_horizontal_set(sep, EINA_TRUE);
	elm_grid_pack(grid,sep,1,30,100,1);
	evas_object_show(sep);	

	
	
	/* put ISO button into group with final DD button and use only
	*  md5 button for checking the md5 since that's all we need anyway.
	*/
	/* ISO selector button */
	iso_bt = elm_fileselector_button_add(grid);
	elm_fileselector_path_set(iso_bt, "/home/");
	elm_fileselector_button_inwin_mode_set(iso_bt, EINA_TRUE);
	elm_fileselector_expandable_set(iso_bt, EINA_FALSE);
	elm_fileselector_folder_only_set(iso_bt, EINA_FALSE);
	elm_fileselector_is_save_set(iso_bt, EINA_TRUE);
	elm_object_text_set(iso_bt, "Select ISO");

	elm_grid_pack_set(iso_bt,2,35,26,11);
	evas_object_show(iso_bt);

	/* ISO text entry */
	entry2 = elm_entry_add(grid);
	elm_entry_line_wrap_set(entry2, EINA_FALSE);
	elm_entry_editable_set(entry2, EINA_FALSE);
	elm_grid_pack_set(entry2,30,35,42,11);
	evas_object_show(entry2);

	/* help button */
	help_bt = elm_button_add(grid);
	elm_button_autorepeat_set(help_bt, EINA_FALSE);
	elm_object_text_set(help_bt, "Help");
	elm_grid_pack_set(help_bt,84,83,15,10);
	evas_object_show(help_bt);
	

	
	//add callbacks for buttons
	evas_object_smart_callback_add(md5_bt,"file,chosen",md5_chosen, entry1);
	evas_object_smart_callback_add(iso_bt,"file,chosen",iso_chosen, entry2);
	evas_object_smart_callback_add(md5_check_bt,"clicked",md5_check,entry1);
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
