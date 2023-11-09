namespace Mains {
  void main_2() {
    Serial.begin(2'000'000);
    auto robot = Mains::Robot::make();
    bool is_turned = false;
    delay(1000);

    go_forward: while(true) {
      u16 sensor_values[5]{};
      for(u8 sample_time = 0; sample_time < sample_count; ++sample_time) for(u8 i = 0; i < 5; ++i) {
        sensor_values[i] += robot.photos[i].read();
      }

      u8 max_sensor = 0;
      u8 index = 0;
      for(u8 i = 0; i < 5; ++i) {
        const u8 this_sensor = sensor_values[i] / sample_count;
        Serial.print("s");
        Serial.print(i);
        Serial.print(":");
        Serial.print(this_sensor);
        Serial.print(",");
        if(max_sensor < this_sensor) {
          max_sensor = this_sensor;
          index = i;
        }
      }

      Serial.println("");

      if(max_sensor < threthold && !is_turned) {
        is_turned = true;
        break;
      } else {
        switch(index) {
          case 0:
          robot.left_motor.forward(max_speed);
          robot.right_motor.forward(min_speed);
          break;
          
          case 1:
          robot.left_motor.forward(max_speed);
          robot.right_motor.forward(middle_speed);
          break;

          case 2:
          robot.left_motor.forward(max_speed);
          robot.right_motor.forward(max_speed);
          break;

          case 3:
          robot.left_motor.forward(middle_speed);
          robot.right_motor.forward(max_speed);
          break;

          case 4:
          robot.left_motor.forward(min_speed);
          robot.right_motor.forward(max_speed);
          break;
        }
      }
    }

    robot.left_motor.brake();
    robot.right_motor.brake();
    delay(500);

    u32 start_time = millis();
    while(millis() - start_time < 1000) {
      robot.left_motor.backward(middle_speed);
      robot.right_motor.forward(middle_speed);
    }

    goto go_forward;
  }
}