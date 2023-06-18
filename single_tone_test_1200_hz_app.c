#include "single_tone_test_1200_hz_app.h"

#include <furi.h>
#include <gui/gui.h>
#include <furi_hal.h>
#include <gui/elements.h>

#include <input/input.h>

#define _1200   1

static void single_tone_test_1200_hz_app_input_callback(InputEvent* input_event, void* ctx){
    furi_assert(ctx);

    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

static void single_tone_test_1200_hz_app_draw_callback(Canvas* canvas, void* ctx){
    /*
    UNUSED(ctx);
    canvas_clear(canvas);

    canvas_draw_icon(canvas, 0, 29, &I_amperka_ru_logo_128x35px);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 4, 8, "This is an example app!");
    
    canvas_set_font(canvas, FontSecondary);
    elements_multiline_text_aligned(canvas, 127, 15, AlignRight, AlignTop, "Some long long long long \n aligned multiline text");
    */

    furi_assert(ctx);
    //ProbeAPRS1App* app = ctx;
    UNUSED(ctx);

    canvas_clear(canvas);
    /*
    DrawMode mode = app->draw_mode;
    if(mode == DRAW_ONLY_PICTURES || mode == DRAW_ALL)
        canvas_draw_icon(canvas, 0, 29, &I_amperka_ru_logo_128x35px);
    if(mode == DRAW_ONLY_TEXT || mode == DRAW_ALL){
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 4, 8, "This is an example app!");
    
        canvas_set_font(canvas, FontSecondary);
        elements_multiline_text_aligned(canvas, 127, 15, AlignRight, AlignTop, "Some long long long long \n aligned multiline text");
    }
    */
}

SingleToneTest1200HzApp* single_tone_test_1200_hz_app_alloc(){
    SingleToneTest1200HzApp* app = malloc(sizeof(SingleToneTest1200HzApp));

    app->view_port = view_port_alloc();
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    //app->notifications = furi_record_open(RECORD_NOTIFICATION);

    view_port_draw_callback_set(app->view_port, single_tone_test_1200_hz_app_draw_callback, app);
    view_port_input_callback_set(app->view_port, single_tone_test_1200_hz_app_input_callback, app->event_queue);

    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    //app->input_pin = &gpio_ext_pa6;
    app->output_pin = &gpio_ext_pa7;

    //furi_hal_gpio_init(app->input_pin, GpioModeInput, GpioPullUp, GpioSpeedVeryHigh);
    furi_hal_gpio_init(app->output_pin, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);

    return app;
}

void single_tone_test_1200_hz_app_free(SingleToneTest1200HzApp* app){
    furi_assert(app);

    view_port_enabled_set(app->view_port, false);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);

    furi_message_queue_free(app->event_queue);

    furi_record_close(RECORD_GUI);
    //furi_record_close(RECORD_NOTIFICATION);
}
//----------------------------------------------------------------------
const float baud_adj = 0.97;
const float adj_1200 = 1.0 * baud_adj;
unsigned int tc1200 = (unsigned int)(0.5 * adj_1200 * 1000000.0 / 1200.0);

void set_nada_1200(SingleToneTest1200HzApp* app)
{
    uint32_t value = 5000;
    taskENTER_CRITICAL();
    //void vTaskSuspendAll();
    //{
        while(value--){
            app->output_value = true;
            furi_hal_gpio_write(app->output_pin, app->output_value);
            furi_delay_us(tc1200);
            app->output_value = false;
            furi_hal_gpio_write(app->output_pin, app->output_value);
            furi_delay_us(tc1200);
        }
    //}
    //portBASE_TYPE xTaskResumeAll();
    taskEXIT_CRITICAL();
}

void set_nada(SingleToneTest1200HzApp* app, bool nada)
{
    if(nada)
        set_nada_1200(app);
}

//----------------------------------------------------------------------
int32_t single_tone_test_1200_hz_app(void *p){
    UNUSED(p);
    SingleToneTest1200HzApp* app = single_tone_test_1200_hz_app_alloc();

    //InputEvent event;

    set_nada(app, _1200);
    /*
    while(1){

        //app->input_value = furi_hal_gpio_read(app->input_pin);
        
        if(furi_message_queue_get(app->event_queue, &event, 1000) == FuriStatusOk) {
            if(event.type == InputTypePress) {
                if(event.key == InputKeyBack) 
                    break;
                else if(event.key == InputKeyUp){}
                    //notification_message(app->notifications, &example_led_sequence);
                else if(event.key == InputKeyDown){}
                    //notification_message(app->notifications, &example_vibro_sequence);
                else if(event.key == InputKeyOk){
                    //notification_message(app->notifications, &example_sound_sequence);
                    
                    
                }
            }
            
            
            else if(event.type == InputTypeLong) {
                //DrawMode mode = app->draw_mode;
                if(event.key == InputKeyLeft){}
                    //app->draw_mode = (mode - 1 + TOTAL_DRAW_MODES) % TOTAL_DRAW_MODES;
                else if(event.key == InputKeyRight){}
                    //app->draw_mode = (mode + 1) % TOTAL_DRAW_MODES;
                
                //view_port_update(app->view_port);
            }
            
        }
        
    }
*/
    single_tone_test_1200_hz_app_free(app);
    return 0;
}