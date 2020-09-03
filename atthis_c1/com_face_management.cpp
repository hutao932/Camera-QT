#include "com_face_management.h"

face_management *Com_face_management::p_face_management = NULL;

Com_face_management::Com_face_management() {

}

void Com_face_management::InitForm() {

    p_face_management=new face_management();

}
