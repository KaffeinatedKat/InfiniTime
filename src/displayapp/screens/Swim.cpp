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
  lv_label_set_text_fmt(label, "Strokes: #FF000 %d#\nBegin: %s\nEnd: %s", strokes, stroke_begin ? "true" : "false", stroke_end ? "true" : "false");
  lv_obj_align(label, nullptr, LV_ALIGN_IN_TOP_MID, 0, 10);
}

void Swim::Calculate() {
  lastX = currentX;
  currentX = motionController.X() / 0x10;
  currentY = motionController.Y() / 0x10;
  currentZ = motionController.Z() / 0x10;

  // Initial strike, sudden X change that is falling
  if (abs(currentX - lastX) < 7 && lastX < -30) {
    // Y begins to rotate as you prepare to push forward
    if (currentY > 30) {
      stroke_begin = true;
    }
  }

  if (stroke_begin) {
    loop_counter++;
    if (loop_counter >= stroke_length) {
      // do not count wrist flicking/sudden movements
      if (currentX > 70 || currentZ > 60) {
        stroke_begin = false;
        stroke_end = false;
        loop_counter = 0;
        return;
      }
      // Arm comes back around after initial strike
      if (!stroke_end && lastX > currentX && currentX < -40 && currentZ > -10) {
        stroke_end = true;
        return;
      }
      // Returns to 0, ready for next stroke
      if (stroke_end && currentX >= 0) {
        strokes += 2;
        loop_counter = 0;
        stroke_begin = false;
        stroke_end = false;
        return;
      }
    }
    // Cancel stroke recording after a while if it never finishes
    if (loop_counter > stroke_timeout) {
      stroke_begin = false;
      stroke_end = false;
      return;
    }
  }

}
