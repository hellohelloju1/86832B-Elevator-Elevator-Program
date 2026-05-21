#include "screen.h"
#include "rawr.h"
#include "liblvgl/lvgl.h"
#include "api.h"

LV_IMAGE_DECLARE(Bg);  // Declare the image

namespace screen {
    bool selected = false;  // Variable to track if an auton has been selected yet
    lv_obj_t *hiddenimage = lv_image_create(lv_screen_active()); 
    lv_obj_t *backgroundimg = lv_image_create(lv_screen_active());
    lv_obj_t *autonlabel = lv_label_create(lv_screen_active());
    lv_obj_t *autonbtn = lv_button_create(lv_screen_active());
    lv_obj_t *autonbtnlabel = lv_label_create(autonbtn);

    void autonselector(); // Forward declaration

    static void ichangedmymind(lv_event_t * e) {
        // selected = false;
        // pros::delay(50);
        // autonselector();
        exit(0);
    }

    void nextscreenplaceholder() {
        lv_label_set_text(autonbtnlabel, "I change my mind");
        lv_obj_set_size(autonbtn, 150, 50);
        lv_obj_add_event_cb(autonbtn, ichangedmymind, LV_EVENT_CLICKED, NULL); 
    }

    static void onclicksetside(lv_event_t * e) {
        selected = true;
        nextscreenplaceholder();
    }

    void init() {
        // background image definition
        lv_image_set_src(backgroundimg, &Bg);  // Use the image data
        lv_obj_align(backgroundimg, LV_ALIGN_CENTER, 0, 0);
    }

    // Non-blocking autonselector using an event-driven approach
    void autonselector() {
        // Create the auton label
        lv_obj_align(autonlabel, LV_ALIGN_CENTER, -80, -40);
        lv_obj_set_style_text_font(autonlabel, &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_color(autonlabel, lv_color_make(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(autonlabel, "Auton Selector");

        // Create the auton button
        lv_obj_align(autonbtn, LV_ALIGN_CENTER, -80, 40);
        lv_obj_set_size(autonbtn, 100, 50);
        lv_obj_align(autonbtnlabel, LV_ALIGN_CENTER, 0, 0);
        lv_label_set_text(autonbtnlabel, "Confirm");
        lv_obj_add_event_cb(autonbtn, onclicksetside, LV_EVENT_CLICKED, NULL); 

        // Set up a potentiometer object for input reading
        pros::adi::Potentiometer potent(1);

            while (!selected) {
                int potval = potent.get_value();
                if (potval <= 1053) {
                    lv_label_set_text(autonlabel, "Red Left");
                    rawr::isLeft = true;
                    rawr::isBlue = false;
                } else if (potval <= 2066) {
                    lv_label_set_text(autonlabel, "Red Right");
                    rawr::isBlue = false;
                    rawr::isLeft = false;
                    lv_obj_set_style_text_color(autonlabel, lv_color_make(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
                } else if (potval <= 3079) {
                    lv_obj_set_style_text_color(autonlabel, lv_color_make(0, 0, 255), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(autonlabel, "Blue Left");
                    rawr::isBlue = true;
                    rawr::isLeft = true;
                } else if (potval <= 4092) {
                    lv_label_set_text(autonlabel, "Blue Right");
                    rawr::isBlue = true;
                    rawr::isLeft = false;
                }
                pros::delay(50); // Small delay to prevent blocking
            }
    }
}
