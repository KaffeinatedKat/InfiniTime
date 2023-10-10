#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "displayapp/screens/Screen.h"
#include <components/motion/MotionController.h>

namespace Pinetime {
  namespace Controllers {
    class MotionController;
  }

  namespace Applications {
    namespace Screens {

      class Swim : public Screen {
      public:
        Swim(Controllers::MotionController& motionController);
        ~Swim() override;

        void Refresh() override;

      private:
        Controllers::MotionController& motionController;

	    lv_obj_t* label;
	    lv_obj_t* average;
	    int32_t currentX;
	    int32_t lastX;

        bool stroking = false;
	    uint32_t strokes = 0;
        uint8_t stroke_length = 5;
        uint8_t stroke_timeout = 50;
        uint8_t loop_counter = 0;

	    void Calculate();

        lv_task_t* taskRefresh;
      };
    }
  }
}
