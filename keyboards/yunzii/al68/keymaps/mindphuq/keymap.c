/* Copyright 2022 Jacky
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // add layer indicators
    switch(get_highest_layer(layer_state|default_layer_state)) {
        case 1:
            rgb_matrix_set_color(55, RGB_CYAN);
            break;
        case 2:
            rgb_matrix_set_color(56, RGB_CYAN);
            break;
        case 3:
            rgb_matrix_set_color(57, RGB_CYAN);
            break;
        default:
            break;
    }

    // Add indicator if caps_lock or caps_word is on
    if (host_keyboard_led_state().caps_lock || is_caps_word_on()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(25, 255, 0, 0); // assuming caps lock is at led #5
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(25, 0, 0, 0);
    }
    return false;
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =  { ENCODER_CCW_CW(KC_VOLU, KC_VOLD)  },
    [1] =  { ENCODER_CCW_CW(RM_VALU, RM_VALD)  },
    [2] =  { ENCODER_CCW_CW(MS_WHLD, MS_WHLU)  }
};
#endif

#define MCTL_ESC CTL_T(KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi_blocker(
    KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,    KC_MUTE,
    KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_DELETE,
    MCTL_ESC,   KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,     KC_PAGE_UP,
    KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_RSFT,                KC_UP,      KC_PAGE_DOWN,
    MO(1),      KC_LGUI,    KC_LALT,                            KC_SPC,                             MO(2),      KC_RCTL,                            KC_LEFT,    KC_DOWN,    KC_RIGHT
    ),
    [1] = LAYOUT_65_ansi_blocker(
    _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,    RM_TOGG,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    CW_TOGG,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    RCS(KC_C),  RCS(KC_V),  _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,                            _______,                            _______,    _______,                            _______,    _______,    _______
    ),
    [2] = LAYOUT_65_ansi_blocker(
    QK_BOOT,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,    _______,
    _______,    KC_BLE1,    KC_BLE2,    KC_BLE3,    KC_24G,     KC_USB,    _______,     _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    KC_CAPS,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    QK_REBOOT,  _______,    _______,    _______,    _______,    _______,    _______,                MS_UP,      _______,
    _______,    _______,    _______,                            _______,                            _______,    _______,                            MS_LEFT,    MS_DOWN,    MS_RGHT
    )
};
