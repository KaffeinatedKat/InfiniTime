#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include <cmath>
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

	    int8_t currentX;
	    int8_t currentY;
	    int8_t currentZ;
	    int8_t lastX;

        bool stroke_begin = false;
        bool stroke_end = false;
	    uint16_t strokes = 0;
        uint8_t stroke_length = 10;
        uint16_t stroke_timeout = 400;
        uint8_t loop_counter = 0;

	    void Calculate();

        lv_task_t* taskRefresh;
      };
    }
  }
}
