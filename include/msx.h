//
// Created by barney on 20-May-21.
//

#ifndef MSX_TOOLS_MSX_H
#define MSX_TOOLS_MSX_H

#include <string>

class MSX {
  private:
    std::string modelo;
    std::string versao;
  public:
    std::string getModelo();
    std::string getVersao();
    MSX(std::string descricao, std::string numver);
};

#endif //MSX_TOOLS_MSX_H
