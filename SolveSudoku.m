function [M,solvability] = solveSudoku(M,rlevel)
    global maxrlevel % initialisation de la variable globale maxrlevel à l'intérieur de cette fonction
    global NbIteration % initialisation de la variable globale NbIteration à l'intérieur de cette fonction
    maxrlevel = max(maxrlevel,rlevel+1); % on prend le niveau de récursion maximum
    NbIteration = NbIteration + 1; % à chaque appel de la fonction, la variable NbIteration incrémente 
    [A] = FillHypothesis(M); % appel de la fonction FillHypothesis
    [solvability] = checkSolved(A); % appel de la fonction checkSolved
    if solvability~=1 % si le sudoku n'est pas encore résolu
        rlevel=rlevel+1; % le niveau de récursion augmente
        Int(:,:,rlevel)=M; % cette variable permet de stocker les différents états de M pour chaque rlevel
        if solvability==0 % si le sudoku est résolvable
            [x,y,choices] = getChoices(A,M); % appel de la fonction getChoices
            for i=1:length(choices)
                if solvability~=1 % on s'assure ici que le sudoku n'est pas résolu entre deux i successifs 
                    M(y,x)=choices(i); % on associe à la case M de coordonnée (x,y) la valeur d'index i de choices
                    [M,solvability] = SolveSudoku(M,rlevel); % appel de la fonction solveSudoku
                end
            end 
        end
        if solvability==-1 % si le sudoku est non résolvable
           % rlevel = rlevel - 1; % le niveau de récursion revient une étape en arrière
            M=Int(:,:,rlevel); % idem pour le sudoku
        end
    end
    if (solvability==1)&(~isempty(find(M==0, 1))) % lorsque solvability = 1 le sudoku n'est pas forcément complètement résolu. En effet certaines cases vides n'ont qu'une seule possibilité: c'est à cela que sert cette boucle if
        pfinal=find(M==0); % on indexe toutes les cases vides de M
        [yfinal,xfinal]=ind2sub(size(M),pfinal); % on transforme l'index pfinal en coordonées xfinal et yfinal, liées à M pour pouvoir accéder à la matrice A
        for i=1:length(pfinal) 
            M(yfinal(i),xfinal(i))=find(A(yfinal(i),xfinal(i),:)~=0); % on attribue pour chaque case vide de M la position de l'unique 1 de A aux coordonnées xfinal et yfinal associées
        end 
    end
end