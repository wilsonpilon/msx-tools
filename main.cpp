#include <iostream>
#include <string>
#include <boost/program_options.hpp>

using namespace std;
using std::string;
namespace po = boost::program_options;

#include "msx.h"
#include "hex_editor.h"
#include "desktop.h"
#include "msx.h"

int main(int argc, char* argv[])
{
  MSX msxbasico("Expert", "1.0");

  po::options_description desc("Opcoes permitidas");
  desc.add_options()
    ("help", "Mensagem de ajuda.")
    ("hex-editor", po::value<string>(), "Executa o editor Hexadecimal para arquivos MSX.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << endl;
    return 1;
  }

  if(vm.count("hex-editor")) {
    cout << "hex-editor " << vm["hex-editor"].as<string>() << "." << endl;
    hex_editor(vm["hex-editor"].as<string>());
  }

  desktop(argc, argv, msxbasico);
  return 0;
}
