clear all 
close all
clc
M=[7,5,9,4,9,9,7,0,9,;9,7,9,0,0,8,0,2,9,;9,0,9,0,7,9,9,9,9,;3,9,2,9,9,9,8,1,7,;9,0,9,8,2,9,5,3,4,;9,0,7,9,4,9,2,9,9,;1,9,9,9,0,9,9,0,9,;8,0,0,9,0,4,9,7,9,;9,9,5,1,7,9,9,9,3,;];
global maxrlevel;
global NbIteration
maxrlevel = 0;
NbIteration = 0;

tic
Solution = SolveSudoku(M,maxrlevel)
toc 
duration = toc;
dispSudoku(M, Solution, NbIteration, maxrlevel, duration,'')