#include "com_face_entering.h"

face_entering *Com_face_entering::p_face_entering =NULL;

Com_face_entering::Com_face_entering() {

}

void Com_face_entering::InitForm() {

    p_face_entering =new face_entering();

}
