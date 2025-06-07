#!/bin/bash
python3 -m platformio run -t upload
python3 -m platformio device monitor
