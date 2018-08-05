/* Avoid reading non-grounded pins */
#define SLAVE_BUTTON_COUNT 12
/* Regular Serial may be taken by USB communication. */
#define SLAVE_SERIAL() Serial

/* Digital pin numbers to read from */
int _pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 14, 15, 16};
/* Current pin state, always same array length as _pins */
int _states[SLAVE_BUTTON_COUNT];
int _current_value = 0;

void setup() {
  /* Slave writes serial to master. */
  SLAVE_SERIAL().begin(9600);
  for (int i = 0; i < SLAVE_BUTTON_COUNT; i++) {
    pinMode(_pins[i], INPUT);
  }
}

void loop() {
  bool hasChanged = false;
  int state = 0;
  int curBit = 1;
  size_t i = 0;
  /* Iterate pins to read */
  for (; i < SLAVE_BUTTON_COUNT; i++) {
    state = digitalRead(_pins[i]);
    if (state != _states[i]) {
      if (!hasChanged)
        hasChanged = true;
      if (state)
        _current_value |= curBit;
      else
        _current_value -= curBit;
      _states[i] = state;
    }
    /* Move next bit for next pin */
    curBit <<= 1;
  }
  /* If a value has changed, write bit values to master. */
  if (hasChanged)
    SLAVE_SERIAL().write((char *)(&_current_value), sizeof(_current_value));
  delay(50);
}
