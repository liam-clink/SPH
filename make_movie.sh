python3 make_plots.py
ffmpeg -r 60 -i frames/%04d.png -pix_fmt yuv420 -vcodec libx264 -o positions.mp4
