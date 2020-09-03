#include "com_function_set.h"

function_set *Com_function_set::p_function_set = NULL;

Com_function_set::Com_function_set() {

}

void Com_function_set::InitForm() {

    p_function_set = new function_set();
}
