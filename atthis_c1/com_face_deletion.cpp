#include "com_face_deletion.h"

face_deletion *Com_face_deletion::p_face_deletion =NULL;

Com_face_deletion::Com_face_deletion() {

}

void Com_face_deletion::InitForm() {

    p_face_deletion= new face_deletion();

}
