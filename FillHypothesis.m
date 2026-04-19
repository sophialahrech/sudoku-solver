function [A] = FillHypothesis(M)
    A=ones(9,9,9); % on remplit tout d'abord la matrice 9x9x9 de 1
    for n=1:9 % pour chaque chiffre on va associer un plan
        p=find(M==n); % on indexe toutes les cases de M remplies de n
        [y,x]=ind2sub(size(M),p); % on transforme cet index en coordonnées x,y
        A(y,:,n)=0; % toutes les lignes contenant n sont mises à 0
        A(:,x,n)=0; % toutes les colonnes contenant n sont mises à 0
        xmin=x-mod(x-1,3); % on délimite ici les carrés 3x3 où se situent les différents n
        ymin=y-mod(y-1,3); 
        for k=1:length(p)
            A(ymin(k):ymin(k)+2,xmin(k):xmin(k)+2,n)=0; % toutes les carrés contenant n sont mis à 0
            A(y(k),x(k),n)=1; % on remet les cases contenant n à 1
        end
        q=find(M ~= 0 & M ~= n); % on met à 0 toutes les cases non vides différentes de n
        [yb,xb]=ind2sub(size(M),q);
        for l=1:length(q)
            A(yb(l),xb(l),n)=0;
        end
    end
end