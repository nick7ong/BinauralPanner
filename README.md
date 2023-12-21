# BinauralPanner Plugin

## Overview
BinauralPanner is a spatial audio plugin prototyped in MATLAB and developed using the JUCE C++. It aims to create immersive 3D audio experiences by utilizing Head-Related Transfer Functions (HRTFs). This project provides users with a tool to manipulate stereo sound fields for enhanced spatial audio perception. This plugin uses the JUCE FIR filter class and renders binaural audio in real-time with low latency. 

## HRTF Dataset
- MARL-NYU MAT Format
- “captured using logarithmic sine-sweep signals at 44100 Hz. 10 different elevations were measured starting at -45◦ and ending at 90◦ in 15◦ vertical increments. The number of azimuth locations varies from 24 (15◦ azimuth increments in 0◦ elevation) to just 1 (in 90◦ elevation)”
- Collection: Andreopoulou, A., & Roginska, A. (2011). Description of the HRIR Repository. Music and Audio Research Laboratory, New York University
- Original Dataset: Warusfel, O. (2003). LISTEN HRTF database, http://recherche.ircam.fr/equipes/salles/listen/.

## Features
- Spatial sound localization using HRTFs
- Efficient processing for both mono and stereo inputs
- Real-time audio rendering with minimal latency
- User-friendly interface for intuitive control

HRTF dataset 
![block_diagram drawio (2)](https://github.com/nick7ong/BinauralPanner/assets/105762930/56f53905-831d-49e3-be55-35e97c4df521)
