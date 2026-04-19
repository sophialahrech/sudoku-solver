function dispSudoku(Initial,Solution,NBiterations,recLevel,duration,file)
    p=find(Initial==0); % index de toutes les cases vides de la matrice initiale
    q=find(Initial~=0); % index de toutes les cases remplies de la matrice initiale
    [X,Y]=meshgrid(1:9); % attribue à X et Y les coordonnées d'une grille carrée de côté length(x).
    C=ones(9,9); % on remplit une matrice 9x9 de 1
    C(p)=0; % on attribue 0 aux cases de coordonées p
    colormap([1,1,1;.9,.9,.9]); % définit l'usage de la palette de couleurs spécifiée
    imagesc([0.5 8.5],[0.5 8.5],C); % imagesc(C) % affiche, aux coordonnées précisées, les données de C en tant qu'image qui utilise tout la gamme de la colormap
    axis off % afin de cacher les axes 
    set(gca,'Ydir','reverse') % permet d'inverser l'axe y
    for n=0:9
        if mod(n,3)==0 % pour tracer les lignes plus épaisses, qui sont toutes multiples de 3
            line([0, 9], [n, n], 'LineWidth',1.5,'Color', 'k');
            line([n, n], [0, 9], 'LineWidth',1.5,'Color', 'k');
        else % pour tracer le reste des lignes
            line([0, 9], [n, n], 'LineWidth',.75,'Color', 'k');
            line([n, n], [0, 9], 'LineWidth',.75,'Color', 'k');
        end
    end
    text(X(p)-0.6,Y(p)-0.5,num2str(Solution(p)),'FontSize', 12,'Color','[.3 .3 .3]','FontAngle','italic') 
    text(X(q)-0.6,Y(q)-0.5,num2str(Solution(q)),'FontSize', 14,'Color','[0 0 0]','Fontweight','bold')
    axis square % utilise des lignes de même taille pour les axes
    text(0,-0.35,file,'Color','b','FontSize',14)
    text(0,9.35,"#iterations =" +NBiterations+ ",",'FontSize',6) % on concatène des mots avec des données 
    text(3,9.35,"deepest recursion level =" +recLevel+ ",",'FontSize',6) % idem ici
    text(7,9.35,"duration =" +duration+ "[s]",'FontSize',6)
    creationDateTime=datetime('now','Format','dd-MMMM-yyyy hh:mm:ss'); % format spécifique
    CharDateTime=char(creationDateTime); % on transforme la variable creationDateTime en chaîne de caractères afin de pouvoir l'afficher avec text
    text(6,-0.2, CharDateTime,'FontSize',6)
     % if solvability = -1 ->   text(6,6, "erreur",'FontSize',6)

    print('-dpdf',file)
end
% on ne l'a pas précisé pour chaque text ou line afin d'éviter de la
% redondance, mais on a associé des caractéristiques de taille, de police,
% de couleur et d'épaisseur spécifiques.        