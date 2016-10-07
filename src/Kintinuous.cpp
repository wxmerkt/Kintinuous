/*
 * This file is part of Kintinuous.
 *
 * Copyright (C) 2015 The National University of Ireland Maynooth and
 * Massachusetts Institute of Technology
 *
 * The use of the code within this file and all code within files that
 * make up the software that is Kintinuous is permitted for
 * non-commercial purposes only.  The full terms and conditions that
 * apply to the code within this file are detailed within the LICENSE.txt
 * file and at <http://www.cs.nuim.ie/research/vision/data/kintinuous/code.php>
 * unless explicitly stated.  By downloading this file you agree to
 * comply with these terms.
 *
 * If you wish to use any of this code for commercial purposes then
 * please email commercialisation@nuim.ie.
 */

#include "MainController.h"

#include <lcm/lcm-cpp.h>
#include <lcmtypes/maps/map_command_t.hpp>
// Then follow http://lcm-proj.github.io/tut_cpp.html for how to subscribe
// You need to link against lcm and lcmtypes_maps, but the latter is a POD so
// need pods build system

class KintinuousServer {
 private:
  MainController controller_;

 public:
  ~KintinuousServer() {}

  KintinuousServer(int argc, char* argv[]) {
    std::cout << "Starting Kintinuous" << std::endl;
    controller_(argc, argv);
    controller_.start();
  }

  void handleMapCommandMessage(lcm::ReceiveBuffer* rbuf,
                               const std::string& chan,
                               const maps::map_command_t* msg) {
    std::cout << "Map command received" << msg->utime << ": " << msg->command
              << std::endl;

    if (msg->command == maps::map_command_t::START) controller_.start();
  }
}

int main (int argc, char * argv[])
{
  lcm::LCM lcm;
  if (!lcm.good()) {
    std::cerr << "LCM cannot be set up" << std::endl;
    return 1;
  }

  KintinuousServer command_server(argc, argv);
  lcm.subscribe("KINECT_MAP_COMMAND",
                &KintinuousServer::handleMapCommandMessage, &command_server);

  while (0 == lcm.handle())
    ;
}
