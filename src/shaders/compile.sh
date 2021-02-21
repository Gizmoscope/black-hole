#!/bin/sh

path="/home/markus/VulkanSDK/1.1.85.0/x86_64"

$path/bin/glslangValidator -V sphere.vert -o vert.spv
$path/bin/glslangValidator -V sphere.frag -o frag.spv
