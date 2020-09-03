#include "com_main_window.h"
#include <QApplication>
#include "com_face_management.h"
#include "com_function_set.h"
#include "com_face_register.h"
#include "com_face_entering.h"
#include "com_face_deletion.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   Com_main_window::InitForm();

   Com_face_management::InitForm();

   Com_function_set::InitForm();

   Com_face_register::InitForm();

   Com_face_entering::InitForm();

   Com_face_deletion::InitForm();

   Com_main_window::p_main_window->show();

   return a.exec();
}
