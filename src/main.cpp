#include <iostream>
#include <string>
#include <boost/program_options.hpp>

using namespace std;
using std::string;
namespace po = boost::program_options;

#include "msx.h"
#include "hexeditor.h"
#include "desktop.h"
#include "msx.h"

int main(int argc, char* argv[])
{
  char tecla;
  MSX msxbasico("Expert", "1.0");

  if((tecla = getchar()) == 'q')
    return 0;


  po::options_description desc("Opcoes permitidas");
  desc.add_options()
    ("help", "Mensagem de ajuda.")
    ("hexeditor", po::value<string>(), "Executa o editor Hexadecimal para arquivos MSX.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << endl;
    return 1;
  }

  if(vm.count("hexeditor")) {
    cout << "hexeditor " << vm["hexeditor"].as<string>() << "." << endl;
    hexeditor(vm["hexeditor"].as<string>());
  }

  desktop(argc, argv, msxbasico);
  return 0;
}
