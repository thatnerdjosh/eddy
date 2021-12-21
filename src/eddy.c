//gcc -o eddy eddy.c `pkg-config --cflags --libs eina efl elementary`

#include <Elementary.h>
//Click Callback: print Clicked
static void
_button_click_cb(void *data, Evas_Object *button, void *event_info)
{
    elm_object_text_set(button, "Clicked!");
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

    win = elm_win_util_standard_add("Main", "Eddy - Live USB Creator");
    elm_win_autodel_set(win, EINA_TRUE); //set close window
    evas_object_resize(win, 400, 400); //set win size
    

    /*code*/
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

    /*add callbacks*/

    //Click event
    evas_object_smart_callback_add(button_text, "clicked", _button_click_cb, NULL);

    evas_object_show(win);

    elm_run();
    return 0;
}
ELM_MAIN()
