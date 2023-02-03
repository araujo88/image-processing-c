#!/usr/bin/env bash

ffmpeg -i noise-%d.bmp -r 10 -c:v libx264 -preset fast -crf 18 -pix_fmt yuv420p output.mp4
