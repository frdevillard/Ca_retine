/*
 * TestPlus2000.cpp
 *
 *  Created on: 25 févr. 2015
 *      Author: pierre
 */

/*
 * Fichier de test à executer en console
 *
 */

#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "Reseau.h"
#include "Cell.h"
#include "VideoStream.h"
#include "ImageMode.h"



#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;
using namespace std;
using namespace cv;

void help(char** argv) {
  cout <<" Usage: "<< argv[0] <<" ImageToLoadAndDisplay <video flag> OU " << argv[0] << endl;
}


int main(int argc, char **argv) {

  // parse command line ----------------------------------------------
  po::options_description general_opt("Allowed options are: ");
  general_opt.add_options()
    ("help,h", "display this message")
    ("input,i", po::value<std::string>(), "input file: ...." )
    ("param,p",  po::value<float>()->default_value(1.0), "a super param" ) ;


  bool parseOK=true;
  bool cannotStart= false;

  po::variables_map vm;
  try{
    po::store(po::parse_command_line(argc, argv, general_opt), vm);
  }catch(const std::exception& ex){
    parseOK=false;
    cerr << "Error checking program options: "<< ex.what()<< endl;
    help(argv);
    return 1;
  }
  po::notify(vm);
  if(parseOK && ! vm.count("input"))
    {
      cerr << " The input file name was not defined" << endl;
      cannotStart = true;
      return 1;
    }

  
  std::string input = vm["input"].as<string>();
  float param = vm["param"].as<float>();


  int kScale = 0;
  int lSmoothing = 50;
  float a = 0.05;

  if (argc == 1) {
    VideoStream *videoStream = new VideoStream(kScale, lSmoothing, a);
    videoStream->computeVideo(1);
    delete videoStream;
  } else if (atoi(argv[2]) == 1) {
    VideoStream *videoStream = new VideoStream(kScale, lSmoothing, a, argv[1]);
    videoStream->computeVideo(0);
    delete videoStream;
  } else {
    ImageMode *imageMode = new ImageMode(argv[1], kScale, lSmoothing);
    imageMode->computeImage();
    delete imageMode;
  }
  return 0;
}




