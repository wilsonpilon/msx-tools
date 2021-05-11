#include <iostream>
#include <string>
#include <boost/program_options.hpp>

using namespace std;
using std::string;
namespace po = boost::program_options;

#include "../inc/msx.h"
#include "../inc/hex_editor.h"


int main(int ac, char** av)
{
  po::options_description desc("Opcoes permitidas");
  desc.add_options()
    ("help", "Mensagem de ajuda.")
    ("hex-editor", po::value<string>(), "Executa o editor Hexadecimal para arquivos MSX.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << endl;
    return 1;
  }

  if(vm.count("hex-editor")) {
    cout << "hex-editor " << vm["hex-editor"].as<string>() << "." << endl;
    hex_editor(vm["hex-editor"].as<string>());
  } else {
    cout << "Criando novo arquivo." << endl;
    hex_editor("noname");
  }

  return 0;
}
