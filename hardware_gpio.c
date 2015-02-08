#include "common.h"

void setup_gpio() {
    config_gpio_dir_i(PORTA, BIT_14);
    config_gpio_dir_i(PORTG, BIT_7);
    config_gpio_dir_o(PORTE, BIT_5);
    config_gpio_dir_o(PORTE, BIT_3);
    config_gpio_dir_o(PORTG, BIT_8);
    config_gpio_dir_o(PORTG, BIT_6);

    set_gpio_high(PORTE, BIT_3);
    set_gpio_high(PORTE, BIT_5);
    set_gpio_high(PORTA, BIT_14);
}

inline void enable_request() {
    set_gpio_low(PORTE, BIT_5);
}

inline void disable_request() {
    set_gpio_high(PORTE, BIT_5);
}

inline uint8_t get_ready_pin() {
    return input_get(PORTA, BIT_14);
}

inline void reset_high() {
    set_gpio_high(PORTE, BIT_3);
}

inline void reset_low() {
    set_gpio_low(PORTE, BIT_3);
}
