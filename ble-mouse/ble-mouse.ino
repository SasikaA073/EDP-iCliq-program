
#include <BleMouse.h>
#include "button.h"
BleMouse bleMouse;

Button btn_fwd;
Button btn_rvd;

void setup() {
  btn_fwd.begin(35);
btn_rvd.begin(34);  QQ
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() {
  if(bleMouse.isConnected()) {
    if(btn_fwd.debounce()){
    Serial.println("Forward");
    bleMouse.click(MOUSE_LEFT);
    }
    else if(btn_rvd.debounce()){
    Serial.println("back");
    bleMouse.move(0,0,-1);
    }
  }
   
}
