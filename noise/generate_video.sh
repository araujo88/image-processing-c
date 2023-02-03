#!/usr/bin/env bash

ffmpeg -i noise-%d.bmp -r 10 -c:v libx264 -preset slow -crf 10 output.mp4
