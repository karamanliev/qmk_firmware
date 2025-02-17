RGB_MATRIX_EFFECT(bg_na_3_moreta_rgb_effect)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool bg_na_3_moreta_rgb_effect(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t one_third = (led_max - led_min) / 3;

    for (uint8_t i = led_min; i < led_min + one_third; i++) {
        rgb_matrix_set_color(i, 255, 255, 255);  // White
    }

    // Set the second third of the LEDs to green (RGB: 0, 255, 0)
    for (uint8_t i = led_min + one_third; i < led_min + 2 * one_third; i++) {
        rgb_matrix_set_color(i, 0, 255, 0);  // Green
    }

    // Set the last third of the LEDs to red (RGB: 255, 0, 0)
    for (uint8_t i = led_min + 2 * one_third; i < led_max; i++) {
        rgb_matrix_set_color(i, 255, 0, 0);  // Red
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif
