RamJam Engine
=============

The RamJam Engine is a game/rendering engine for learning and personal showcase purposes only. I've been really impressed by all the next-gen engines, such as Frostbite, CryEngine or Unreal over the years that I have decided to go further into 3D graphics and build my own engine from scratch. I've been working on it for the past few months now.
 
The RJE is coded in C++ and uses DirectX 11 features (especially compute shaders).
I wanted to do my own 3D engine while reading these 2 books : 
 - Game Engine Architecture (by Jason Gregory - http://www.gameenginebook.com/)
 - Introduction to 3D Game Programming with Direct3D 11.0 (http://www.d3dcoder.net/)
    
Videos and descritpions can be found on my website : http://rramillien.wix.com/ramjam

The RJE has the following features
----------------------------------
 - Tiled Deferred Rendering with MSAA support
    + based on the work of Andrew Lauritzen (Intel) : http://software.intel.com/en-us/articles/deferred-rendering-for-current-and-future-rendering-pipelines
 - Sample Distibution Shadow Mapping : 
    + still based on Andrew Lauritzen's work : http://visual-computing.intel-research.net/art/publications/sdsm/
 - Normal Mapping
 - Frustum Culling
 - CPU & GPU Profiling
