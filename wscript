#!/usr/local/bin/python

def configure(conf):
    conf.load("clang++")

def build(bld):
    bld(features="cxx cxxprogram",
            source="./src/Main.cpp ./src/Geo.cpp ./src/Piece.cpp ./src/Solver.cpp",
            target="solver",
            includes=[".", ".."],
            cxxflags=["-std=c++14", "-O3", "-march=native"])
