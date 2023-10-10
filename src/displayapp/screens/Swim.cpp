#include "displayapp/screens/Swim.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/InfiniTimeTheme.h"

using namespace Pinetime::Applications::Screens;


Swim::Swim(Controllers::MotionController& motionController)
  : motionController {motionController} {
  
  label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(label, "Strokes: #FF0000 %d#", 0);
  lv_obj_align(label, nullptr, LV_ALIGN_IN_TOP_MID, 0, 10);
  lv_label_set_recolor(label, true);

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}


Swim::~Swim() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void Swim::Refresh() {
  Calculate();
  lv_label_set_text_fmt(label, "Strokes: #FF000 %d#\nX: %d", strokes, currentX);
  lv_obj_align(label, nullptr, LV_ALIGN_IN_TOP_MID, 0, 10);
}

void Swim::Calculate() {
  lastX = currentX;
  currentX = motionController.X() / 0x10;

  if (currentX > lastX && lastX < -60) {
    stroking = true;
  }

  if (stroking) {
    loop_counter++;

    // Prevent overcounting with a slight delay
    if (loop_counter >= stroke_length) {
      // Freestyle strokes go negetive then return to around 0 near the end of the stroke
      if (currentX >= 0 or lastX >= 0) {
        strokes += 2;
        loop_counter = 0;
        stroking = false;
        return;
      }
    }
    // Cancel stroke recording after a while (probably triggered by gravity when watch is tilted)
    if (loop_counter > stroke_timeout) {
      stroking = false;
    }
  }

}
