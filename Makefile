######################################################################
# Project: TMVA - Toolkit for Multivariate Data Analysis             #
# Code   : Example usage for TMVA analyses                           #
###################################################################### 

CXX = g++
CCFLAGS = $(shell root-config --cflags) -ggdb


LD = g++
LDFLAGS = 

LIBS = $(shell root-config --libs) -lMLP -lMinuit -lTreePlayer -lTMVA -lTMVAGui -lXMLIO  -lMLP -lm

all : exp

exp : CSV2root.cpp rapidcsv.h
	$(CXX) CSV2root.cpp rapidcsv.h $(CCFLAGS) $(LIBS) -o CSV2root

clean: 
	rm *.out
