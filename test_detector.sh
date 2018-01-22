#!/bin/bash
./darknet detector test ./custom_cfg/eclipse_nn.data ./custom_cfg/eclipse_nn.cfg ./backup/eclipse_nn_100.weights ./tarps.jpg
