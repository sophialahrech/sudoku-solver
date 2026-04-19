 function [x,y,choices] = getChoices(A,M)
    R=zeros(9,9); % on initialise à 0 une matrice 9x9
    for k=1:9
        R=R+A(:,:,k); % on fait la somme de la matrice A, niveau par niveau
    end
    q=find(M~=0); % on indexe toutes les cases déjà remplies de M
    R(q)=10; % on associe 10 aux cases de R d'index q, afin qu'à la ligne suivante, ces cases ne puissent pas être le minimum
    [~, p]=min(R,[],"all"); % on cherche l'index de la case avec le moins de choix possibles
    [y,x]=ind2sub(size(R),p); % on associe des coordonnées x et y dans R à cet index
    choices=find(A(y,x,:)~=0); % on attribue à choices les positions des cases de A contenant 1 aux coordonnées x et y
end
    
    
