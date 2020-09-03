#include "com_register_success.h"

register_success *COM_register_success::p_register_success = NULL;

COM_register_success::COM_register_success() {

}
void COM_register_success::InitForm() {

    p_register_success= new register_success();

}
