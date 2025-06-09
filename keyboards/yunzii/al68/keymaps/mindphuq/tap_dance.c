#include QMK_KEYBOARD_H
#include "qmk-vim/src/vim.h"

enum {
    TD_VIM_LAYER,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
} td_state_t;

static td_state_t td_vim_state;

static td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }
    return TD_UNKNOWN;
}

void vim_layer_finished(tap_dance_state_t *state, void *user_data) {
    td_vim_state = cur_dance(state);
    switch (td_vim_state) {
        case TD_SINGLE_TAP:
            if (vim_mode_enabled()) {
                toggle_vim_mode();
            } else {
                start_oneshot_vim();
            }
            break;
        case TD_SINGLE_HOLD:
            layer_on(2);
            break;
        default:
            break;
    }
}

void vim_layer_reset(tap_dance_state_t *state, void *user_data) {
    if (td_vim_state == TD_SINGLE_HOLD) {
        layer_off(2);
    }
    td_vim_state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_VIM_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, vim_layer_finished, vim_layer_reset),
};
