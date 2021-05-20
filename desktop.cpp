//
// Created by barney on 20-May-21.
//
#include <final/final.h>

using namespace finalcut;

#include "desktop.h"
#include "msx.h"

int desktop(int ac, char *av[], MSX msxbasico) {
  FApplication app(ac, av);

  // The object dialog is managed by app
  FDialog* dialog = new FDialog(&app);
  dialog->setText ("MSX Tools: Versao " + msxbasico.getModelo() + " " + msxbasico.getVersao());
  dialog->setGeometry (FPoint{30, 8}, FSize{60, 12});

  // The object input is managed by dialog
  FLineEdit* input = new FLineEdit(msxbasico.getModelo(), dialog);
  input->setGeometry(FPoint{8, 2}, FSize{40, 1});
  input->setLabelText (L"&Input");

  // The object label is managed by dialog
  FLabel* label = new FLabel ( "Lorem ipsum dolor sit amet, consectetur \n"
                               "adipiscing elit, sed do eiusmod tempor \n"
                               "incididunt ut labore et dolore magna aliqua.\n"
      , dialog );
  label->setGeometry (FPoint{2, 4}, FSize{45, 3});
  FWidget::setMainWidget(dialog);
  dialog->show();
  return app.exec();
}