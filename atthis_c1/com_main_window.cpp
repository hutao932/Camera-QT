#include "com_main_window.h"

main_window *Com_main_window::p_main_window = NULL;

Com_main_window::Com_main_window() {

}
void Com_main_window::InitForm() {
    p_main_window = new main_window();
}
