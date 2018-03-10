int x_dir;
int y_dir;
int x_dir_prev;
int y_dir_prev;
int x_state;
int y_state;
unsigned long last_check;
boolean motors_enabled;

unsigned long x_step_position;
unsigned long y_step_position;
float x_deg_position;
float y_deg_position;

void init_motors() {
  x_dir = 0;
  y_dir = 0;
  x_dir_prev = x_dir;
  y_dir_prev = y_dir;

  x_state = LOW;
  y_state = LOW;

  last_check = 0;

  motors_enabled = true;

  x_step_position = 0;
  y_step_position = 0;
  x_deg_position = X_MIN;
  y_deg_position = Y_MIN;

  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);

  pinMode(E0_STEP_PIN, OUTPUT);
  pinMode(E0_DIR_PIN, OUTPUT);
  pinMode(E0_ENABLE_PIN, OUTPUT);

  pinMode(E1_STEP_PIN, OUTPUT);
  pinMode(E1_DIR_PIN, OUTPUT);
  pinMode(E1_ENABLE_PIN, OUTPUT);

  digitalWrite(X_DIR_PIN, X_DIRECTION);
  digitalWrite(Y_DIR_PIN, Y_DIRECTION);
  digitalWrite(X_ENABLE_PIN, ENABLE);
  digitalWrite(Y_ENABLE_PIN, ENABLE);
}

void toggle_motors() {
  if (motors_enabled) {
    digitalWrite(X_ENABLE_PIN, DISABLE);
    digitalWrite(Y_ENABLE_PIN, DISABLE);
    motors_enabled = false;
  } else {
    digitalWrite(X_ENABLE_PIN, ENABLE);
    digitalWrite(Y_ENABLE_PIN, ENABLE);
    motors_enabled = true;
  }
}

void update_motors() {
  // switch direction
  if (x_dir_prev != x_dir) {
    if (x_dir == 1) {
      digitalWrite(X_DIR_PIN, X_DIRECTION);
    } else {
      digitalWrite(X_DIR_PIN, !X_DIRECTION);
    }
  }

  if (y_dir_prev != y_dir) {
    if (y_dir == 1) {
      digitalWrite(Y_DIR_PIN, Y_DIRECTION);
    } else {
      digitalWrite(Y_DIR_PIN, !Y_DIRECTION);
    }
  }

  long delta = micros() - last_check;
  if (delta > UPDATE_PERIOD) {
    if (abs(x_dir) > 0) {
      x_state = !x_state;
      digitalWrite(X_STEP_PIN, x_state);
      if (x_state == HIGH) {
        if (x_dir > 0) {
          x_step_position++;
        } else {
          if (x_step_position > 0) {
            x_step_position--;
          }
        }
      }
    }

    if (abs(y_dir) > 0) {
      y_state = !y_state;
      digitalWrite(Y_STEP_PIN, y_state);
      if (y_state == HIGH) {
        if (y_dir > 0) {
          y_step_position++;
        } else {
          if (y_step_position > 0) {
            y_step_position--;
          }
        }
      }
    }

    last_check = micros();
  }

  x_dir_prev = x_dir;
  y_dir_prev = y_dir;
}

void set_x_vel(float vel) {
  if (vel > 0.1f) {
    x_dir = 1;
  } else if (vel < -0.1f) {
    x_dir = -1;
  } else {
    x_dir = 0;
  }
}

void set_y_vel(float vel) {
  if (vel > 0.1f) {
    y_dir = 1;
  } else if (vel < -0.1f) {
    y_dir = -1;
  } else {
    y_dir = 0;
  }
}

unsigned long get_x_steps() {
  return x_step_position;
}

unsigned long get_y_steps() {
  return y_step_position;
}

float get_x_degrees(unsigned long steps) {
  float pos = X_MIN;
  pos += steps / X_STEPS_PER_DEG;
  return pos;
}

float get_y_degrees(unsigned long steps) {
  float pos = Y_MIN;
  pos += steps / Y_STEPS_PER_DEG;
  return pos;
}
