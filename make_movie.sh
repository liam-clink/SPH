#!/bin/bash
python3 python/make_plots.py
ffmpeg -r 60 -i frames/*.png -pix_fmt yuv420 -vcodec libx264 -o positions.mp4
