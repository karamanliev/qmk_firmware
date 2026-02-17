#include QMK_KEYBOARD_H
#include "qmk-vim/src/vim.h"
#include "raw_hid.h"
#include "tap_dance.c"

#define CTL_ESC LCTL_T(KC_ESC)
#define ALT_LNG ALT_T(KC_LNG1)
#define MO2_LNG LT(2, KC_LNG1)

#define LED_CAPS 25
#define LED_OS 44
#define LED_1 55
#define LED_2 56
#define LED_3 57
#define HSV_CYBER 191, 200, 120

#define LIN_PSCR LGUI(S(KC_S))
#define MAC_PSCR LGUI(S(KC_4))
#define LIN_SEL_WRD C(S(KC_LEFT))
#define MAC_SEL_WRD A(S(KC_LEFT))
#define VIM_MO2 TD(TD_VIM_LAYER)

enum user_keycodes {
    OS_CHG = SAFE_RANGE,
    TOG_VIM,
    PSCR,
    DEL_WRD,
};

bool is_macos_mode = false;

#ifdef RAW_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length < 1) return;  // Ignore empty messages

    switch (data[0]) {
        case 0x01:
            rgb_matrix_enable_noeeprom();
            rgb_matrix_sethsv_noeeprom(HSV_CYAN);
            break;
        case 0x02:
            rgb_matrix_enable_noeeprom();
            rgb_matrix_sethsv_noeeprom(HSV_CYBER);
            break;
        case 0x03:
            rgb_matrix_disable_noeeprom();
            break;
        default:
            // Echo back if it's an unknown command
            raw_hid_send(data, length);
            break;
    }
}
#endif

// Call the post init code.
void keyboard_post_init_user(void) {
    rgb_matrix_enable_noeeprom();
    rgb_matrix_sethsv_noeeprom(HSV_CYBER);
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
}

// Custom keycodes setup
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Process case modes
    if (!process_vim_mode(keycode, record)) {
        return false;
    }

    // Regular user keycode case statement
    if (!record->event.pressed) {
        return true;
    }

        // Auto-HYPR layer 1 keys
    if (IS_LAYER_ON(1)) {
        // Exclude specific keycodes
        switch (keycode) {
            case RCS(KC_C):
            case RCS(KC_V):
            case TOG_VIM:
            case KC_F1 ... KC_F12:
            case KC_LNG1:
            case KC_LEFT:
            case KC_DOWN:
            case KC_UP:
            case KC_RIGHT:
            case DEL_WRD:
            case RM_TOGG:
            case DT_UP:
            case DT_DOWN:
            case DT_PRNT:
            case KC_TRNS:  // Don't wrap transparent keys
            case MO(1):    // Don't wrap layer keys
            case MO(2):
                break;
            default:
                // Wrap everything else in HYPR
                if (keycode >= KC_A && keycode <= KC_Z) {
                    tap_code16(HYPR(keycode));
                    return false;
                }
                break;
        }
    }

    switch (keycode) {
        case OS_CHG:
            is_macos_mode = !is_macos_mode;

        case TOG_VIM:
            if (record->event.pressed) {
                toggle_vim_mode();
            }
            return false;

        case PSCR:
            keycode = is_macos_mode ? MAC_PSCR : LIN_PSCR;
            tap_code16(keycode);

        case DEL_WRD:
            keycode = is_macos_mode ? MAC_SEL_WRD : LIN_SEL_WRD;
            tap_code16(keycode);
            tap_code(KC_BSPC);
            return false;
    }
    return true;
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // add layer indicators
    switch(get_highest_layer(layer_state|default_layer_state)) {
        case 1:
            rgb_matrix_set_color(LED_1, RGB_CYAN);
            break;
        case 2:
            rgb_matrix_set_color(LED_2, RGB_CYAN);
            break;
        case 3:
            rgb_matrix_set_color(LED_3, RGB_CYAN);
            break;
        default:
            break;
    }

    // Add indicator if caps_lock or caps_word is on
    if (host_keyboard_led_state().caps_lock || is_caps_word_on()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(LED_CAPS, 255, 0, 0);
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(LED_CAPS, 0, 0, 0);
    }

    if (vim_mode_enabled()) {
        for (uint8_t i = 25; i <= 38; i++) { // home row?
            rgb_matrix_set_color(i, 0, 255, 0);
        }
    }

    if (is_macos_mode) {
        rgb_matrix_set_color(LED_OS, 255, 0, 0);
    } else {
        rgb_matrix_set_color(LED_OS, 0, 0, 0);
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi_blocker(
    KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,    KC_MUTE,
    KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_DELETE,
    CTL_ESC,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,     KC_PAGE_UP,
    KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_RSFT,                KC_UP,      KC_PAGE_DOWN,
    MO(1),      KC_LGUI,    KC_LALT,                            KC_SPC,                             VIM_MO2,    KC_RCTL,                            KC_LEFT,    KC_DOWN,    KC_RIGHT
    ),

    [1] = LAYOUT_65_ansi_blocker(
    _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     DEL_WRD,    RM_TOGG,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    TOG_VIM,    _______,    _______,    _______,    _______,    _______,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,   _______,    _______,                _______,    _______,
    KC_LNG1,    _______,    _______,    RCS(KC_C),  RCS(KC_V),  _______,    _______,    _______,    _______,    _______,    _______,    _______,                DT_UP,      _______,
    _______,    _______,    _______,                            _______,                            _______,    _______,                            _______,    DT_DOWN,    DT_PRNT
    ),

    [2] = LAYOUT_65_ansi_blocker(
    QK_BOOT,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,    _______,
    _______,    KC_BLE1,    KC_BLE2,    KC_BLE3,    KC_24G,     KC_USB,    _______,     _______,    _______,    OS_CHG,     _______,    _______,    _______,    _______,    _______,
    KC_CAPS,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                MS_BTN3,    _______,
    _______,    _______,    _______,    _______,    _______,    QK_REBOOT,  _______,    _______,    _______,    _______,    _______,    MS_BTN1,                MS_UP,      MS_BTN2,
    _______,    _______,    _______,                            _______,                            _______,    KC_RALT,                            MS_LEFT,    MS_DOWN,    MS_RGHT
    )
};
