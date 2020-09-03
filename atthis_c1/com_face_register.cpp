#include "com_face_register.h"

face_register *Com_face_register::p_face_register= NULL;

Com_face_register::Com_face_register() {

}

void Com_face_register::InitForm() {
    p_face_register = new face_register();
}
