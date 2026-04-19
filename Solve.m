clear all 
close all
clc
M=[0,5,0,4,0,0,7,0,0,;0,6,0,0,0,8,0,2,0,;0,0,0,0,7,9,0,0,0,;3,0,2,0,0,0,8,1,7,;0,0,0,8,2,0,5,3,4,;0,0,7,0,4,0,2,6,0,;1,0,0,0,0,0,0,0,0,;8,0,0,0,0,4,0,7,0,;0,0,5,1,8,0,0,0,3,;];
global maxrlevel;
global NbIteration
maxrlevel = 0;
NbIteration = 0;

tic
Solution = SolveSudoku(M,maxrlevel)
toc 
duration = toc;
dispSudoku(M, Solution, NbIteration, maxrlevel, duration,'')