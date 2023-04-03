#define led 13
#define led 13
#define u_tx 2
#define u_rx 3
#define lock_l 15
#define lock_r 14

#define motor_ld 5
#define motor_rd 6
#define motor_le 7
#define motor_re 8

#define lim_lo 9
#define lim_ro 10
#define lim_lc 11
#define lim_rc 12

#define left 0
#define right 1

enum door { closing, opening, opened, pre_open, closed, stopped };

door door_left = closing;
door door_right = closing;

SoftwareSerial uSerial(u_rx, u_tx);

void gpio_config() {
  pinMode(led, OUTPUT);
  pinMode(lock, OUTPUT);
  pinMode(motor_ld, OUTPUT);
  pinMode(motor_le, OUTPUT);
  pinMode(motor_rd, OUTPUT);
  pinMode(motor_re, OUTPUT);
  pinMode(lim_lo, INPUT_PULLUP);
  pinMode(lim_lc, INPUT_PULLUP);
  pinMode(lim_ro, INPUT_PULLUP);
  pinMode(lim_rc, INPUT_PULLUP);
}

void open(int type) {
  digitalWrite(type == left ? motor_ld : motor_rd, HIGH);
  digitalWrite(type == left ? motor_le : motor_re, HIGH);
}

void close(int type) {
  digitalWrite(type == left ? motor_ld : motor_rd, LOW);
  digitalWrite(type == left ? motor_le : motor_re, HIGH);
}

void stop(int type) { digitalWrite(type == left ? motor_le : motor_re, LOW); }

void lock_in(int type) { digitalWrite(type == left ? lock_l : lock_r, LOW); }
void let_out(int type) { digitalWrite(type == left ? lock_l : lock_r, HIGH); }

int is_closed(int type) { return digitalRead(type == left ? lim_lc : lim_rc); }

int is_open(int type) { return digitalRead(type == left ? lim_lo : lim_ro); }

void process(int type, int state) {

  switch (state) {
  case closing:
    close(type);
    if (is_closed(type)) {
      state = closed;
    }
    break;
  case opening:
    open(type);
    if (is_closed(type)) {
      lock_in(type);
    } else {
      let_out(type);
    }
    if (is_open(type)) {
      state = opened;
    }
    break;

  case opened:
    stop(type);
    break;
  case closed:
    stop(type);
    break;
  case stopped:
    stop(type);
    break;
  default:
    break;
  }
}

void setup() {
  gpio_config();
  uSerial.begin(9600);
  Serial.begin(115200);
}

void loop() {
  process(left, door_left);
  process(right, door_right);
  digitalWrite(led, !digitalRead(led));
  delay(500);
}
