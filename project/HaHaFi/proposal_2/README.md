# Auto video caption and download 
## Introdution
This is a tool which want to download the video on the web page, and get the English caption of the video without using any speech-to-text api. Just use your local GPU.

The tool has two part.
1. Get the m3u8 url in web page and download the video (in C++).
2. Use speech recognition model to get the caption of the video file (in Python).

**!!Note!!**
*  The speech recognition is only for English.


## Problem to solve
1. Download the m3u8 video with specify protocol.
2. The performance of speech recognition model is important. 

## Prospective Users
1. Who need to download the video on the web page.
2. Who need to get the caption of the video **with local GPU**.
3. Who want to get the caption to practice English.

## System Architecture
OS imformation: ubuntu 22.04

flow chart below ( TODO )

## API Description
1. Download video function.
2. Speech-to-text function. 
3. Srt(Subripper) function.
```
# This is pseudo code
# All function will wrap in python function 
def main():
    video_download(SAVE_PATH,M3U8_URL)
    time_range_list,caption = speech_to_text(SAVE_PATH)
    srt(SAVE_PATH,time_range_list,caption)
```
## Engineering Infrastructure
Automatic build system: make

Version control: git

Testing framework: [pytest](https://pytest.org)

Documentation: [doxygen](https://doxygen.nl/) For automatic document generation.

## Schedule
Week 1 (10/31): Study protocol and third party library.

Week 2 (11/7): Complete make file and the code of protocol.

Week 3 (11/14): Complete testing part and search speech recognition model with high accuracy.

Week 4 (11/21): Implement speech recognition function.

Week 5 (11/28): Implement srt(subripper) function.

Week 6 (12/5): Check all todolist is done.

Week 7 (12/12): Complete documentation (Maybe trun it into the web page).

Week 8 (12/19): Prepare the presentation.

