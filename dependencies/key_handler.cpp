#include "../supremacy.hpp"

bool key_handler::is_key_down(int key) {
    return HIWORD(GetKeyState(key));
}

bool key_handler::is_key_up(int key) {
    return !is_key_down(key);
}

bool key_handler::check_key(int key, int keystyle) {
    switch (keystyle) {
    case 0:
        return true;
    case 1:
        return is_key_down(key);
    case 2:
        return LOWORD(GetKeyState(key));
    case 3:
        return is_key_up(key);
    default:
        return true;
    }
}