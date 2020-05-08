### 0 quick start
- install `arch64-himix100-linux-gcc`
- run `make`
- run `./test`
you will see HMV3 board 3 led light on/off.

### 1 how to use api
- include `api.h` in your c file
- use `void set_led_by_bits(unsigned int bit_num, int on_off)` to set led on/off by bit_num which mask 3 leds.
- use `void set_led_by_id(unsigned int led_id, int on_off)` to set led on/off by led id which indicates 1st to 3rd led;
- use `int get_recovery_btn()` to get current recovery button value.
