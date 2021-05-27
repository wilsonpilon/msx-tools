//
// Created by barney on 20-May-21.
//

#include "msx.h"

std::string MSX::getModelo() {
  return modelo;
}

std::string MSX::getVersao() {
  return versao;
}

MSX::MSX(std::string descricao, std::string numver) : modelo(descricao), versao(numver) {
}
