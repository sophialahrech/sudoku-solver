function [solvability] = checkSolved(A)
    R=zeros(9,9); % on initialise à 0 une matrice 9x9
    for k=1:9
        R=R+A(:,:,k); % on fait la somme de la matrice A, niveau par niveau
    end
    if ~isempty(find(R==0, 1)) % s'il existe une case de M qui n'a aucune solution possible, alors le sudoku n'est pas résolvable
        solvability = -1;
    elseif length(find(R==1)) == 81 % toutes les cases de R sont à 1 : toutes les cases sont donc remplies ou n'ont qu'une seule solution
        solvability = 1;
    else % dans les autres cas, le sudoku est en cours de résolution, on n'a pas plus d'informations
        solvability = 0; 
    end
end