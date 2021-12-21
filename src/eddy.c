//gcc -o eddy eddy.c `pkg-config --cflags --libs eina efl elementary`

#include <Elementary.h>
//Click Callback: print Clicked
static void
_button_click_cb(void *data, Evas_Object *button, void *event_info)
{
    elm_object_text_set(button, "Clicked!");
}

//Press callback: print Pressed
static void
_button_press_cb(void * data, Evas_Object *button, void *event_info)
{
    elm_object_text_set(button, "Pressed!");
}

//Unpress callback: print Unpressed
static void
_button_unpress_cb(void *data, Evas_Object *button, void *event_info)
{
    elm_object_text_set(button, "Unpressed!");
}

//Repeat callback: print number of times callback is called
static void
_button_repeat_cb(void *data, Evas_Object *button, void *event_info)
{
    static int count = 0;
    char buffer[16];

    snprintf(buffer, sizeof(buffer), "Repeat %d", count++);

    //print the number of time callback was called
    elm_object_text_set(button, buffer);
}

//Unpress callback: print Focused
static void
_button_focused_cb(void * data, Evas_Object *button, void *event_info)
{
    elm_object_text_set(button, "Focused");
}

//Unpress callback: print Unfocused
static void
_button_unfocused_cb(void * data, Evas_Object *button, void *event_info)
{
    elm_object_text_set(button, "Unfocused");
}

    EAPI_MAIN int
elm_main(int argc, char **argv)
{
    Evas_Object *win;
    Evas_Object *button_text;
    Evas_Object *button_icon, *icon;
    Evas_Object *button_icon_text, *icon2;
    Evas_Object *button;

    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

    win = elm_win_util_standard_add("Main", "Hello, World!");
    elm_win_autodel_set(win, EINA_TRUE);
    evas_object_resize(win, 400, 400); //set win size
    

    /*basic tutorial code*/
    button_text = elm_button_add(win);
    elm_object_text_set(button_text,"Click me");

    //how a container object should resize a given child within its area
    evas_object_size_hint_weight_set(button_text, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    //how to align an object
    evas_object_size_hint_align_set(button_text, EVAS_HINT_FILL, 0.5);
    evas_object_resize(button_text, 100, 30);
    evas_object_show(button_text);


    //Basic icon button    
    button_icon = elm_button_add(win);
    icon = elm_icon_add(win);


    //set the image file and the button as an icon
    elm_image_file_set(icon, "icon.png", NULL);
    elm_object_text_set(button_icon, "Try me");
    elm_object_part_content_set(button_icon, "icon", icon);

    evas_object_size_hint_weight_set(button_icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(button_icon, EVAS_HINT_FILL, 0.5);

    evas_object_resize(button_icon, 100, 30);
    evas_object_move(button_icon, 110, 0);
    evas_object_show(button_icon);

    //Icon and text button
    
    button_icon_text = elm_button_add(win);
    icon2 = elm_icon_add(win);

    elm_image_file_set(icon2, "icon.png", NULL);
    elm_object_part_content_set(button_icon_text, "icon", icon2);
    elm_object_text_set(button_icon_text, "Press me");
    evas_object_size_hint_weight_set(button_icon_text, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(button_icon_text, EVAS_HINT_FILL, 0.5);

    evas_object_resize(button_icon_text, 100, 30);
    evas_object_move(button_icon_text, 220, 0);
    evas_object_show(button_icon_text);
    
    
    /*add callbacks*/

    //Clik event
    evas_object_smart_callback_add(button_text, "clicked", _button_click_cb, NULL);

    //Press event
    evas_object_smart_callback_add(button_icon, "pressed", _button_press_cb, NULL);
    //Unpress event
    evas_object_smart_callback_add(button_icon, "unpressed", _button_unpress_cb, NULL);

    //Get whether the autorepeat feature is enabled.
    elm_button_autorepeat_set(button_icon_text, EINA_TRUE);
    //Set the initial timeout before the autorepeat event is generated.
    elm_button_autorepeat_initial_timeout_set(button_icon_text, 1.0);
    //gap between two callbacks
    elm_button_autorepeat_gap_timeout_set(button_icon_text, 0.5);
    //"repeated": the user pressed the button without releasing it.
    evas_object_smart_callback_add(button_icon_text, "repeated", _button_repeat_cb, NULL);
    
    
    

    //Focused/unfocused event
    button = elm_button_add(win);
    elm_object_text_set(button, "button");
    evas_object_resize(button, 100, 30);
    evas_object_move(button, 0, 40);
    evas_object_show(button);

    evas_object_smart_callback_add(button, "focused", _button_focused_cb, NULL);
    evas_object_smart_callback_add(button, "unfocused", _button_unfocused_cb, NULL);

    evas_object_show(win);

    elm_run();
    return 0;
}
ELM_MAIN()
