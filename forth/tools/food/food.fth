\ FOOD  Forth Object-Oriented Dialect
\ Version z‚ro 9/89 Michel ZUPAN

only forth also definitions

variable Active                         \ pointeur classe active
variable ClassLink                      \ lien derniŠre classe d‚finie


\ Classes, sous-classes, super-classes
\ ------------------------------------
: subclass   ( <nom-classe>  --- )      \ d‚finit sous-classe de cl.active
  create here Active    dup @ , !
         here ClassLink dup @ , !
         immediate
  does> Active ! ;

: class      ( <nom-classe> --- )       \ d‚finit nouvelle classe
  Active off subclass ;

: superclass ( <nom-classe> --- )       \ d‚finit super-classe de cl.active
  Active @ class Active @ swap ! ;


\ Gestion des classes
\ -------------------
: subclass? ( class1 class2 -- fl)      \ class2 sous-classe de class1 ?
  begin 2dup = 2dup 0= and while drop @ repeat -rot 2drop ;

: classes   ( --- )                     \ affiche liste des classes d‚finies
  ClassLink
     begin @ ?dup while dup 6 - >name .id repeat ;

: .active   ( --- )                     \ affiche classe active
  Active @ ?dup if body> >name .id else ." aucune " then
           ." classe active " ;


\ Proc‚dures et variables de classes
\ ----------------------------------

: (procedure) ( adr -- cfa )            \ traduction proc‚dure-m‚thode
  begin @ dup 2+ @ Active @ subclass? over 0= or until
  dup 0= if cr .active true ?missing
         then 4 + @ ;

: Procedure   ( <nom-proc> --- )        \ d‚finit proc‚dure de cl.active
  variable immediate
  does> (procedure)
        state @ if , else execute then ;

: ClassVar    ( <nom-cl.var> --- )      \ d‚finit variable de cl.active
  variable immediate
  does> (procedure)
        state @ if [compile] literal then ;

\ M‚thodes
\ --------
vocabulary Methods

: Method  ( <procedure> cfa -- )        \ vectorise une m‚thode
  here ' >body dup @ , ! Active @ , , ;

: That    ( -- cfa )                    \ cfa dernier mot d‚fini
  last @ name> ;

: For     ( <classvar>  n -- )          \ affecte une variable de classe
  method ;                              \ (synonyme de method ! )



\ Application: class data
\ ------------------------

only forth also definitions

class data
ClassVar  DataSize

procedure Object
procedure D! ( Data Store )
procedure D@ ( Data Fetch )

procedure array
procedure ?indice
procedure cell

methods definitions

: (DataCreate)
  create active @ , immediate
         [compile] DataSize here over erase allot
         immediate
  does>  dup @ active ! 2+
         state @ if [compile] literal then ;
that method Object

: (?indice) ( n adr -- n adr )
  2dup @ u> abort" indice hors limite" ;
that method ?indice

: (cell)  ( n adr -- adr )
  dup 2+ @ rot * + 4 + ;
that method cell

: (array)
  create active @ , dup , [compile] DataSize dup ,
         * here over erase allot
         immediate
  does>  dup @ active ! 2+
         state @ if [compile] literal then
         [compile] ?indice
         [compile] cell ;
that method array

forth definitions

data subclass entier
  2 for DataSize
' ! method D!
' @ method D@

data subclass simple
   1 for DataSize
' c! method D!
' c@ method D@

data subclass double
   4 for DataSize
' 2! method D!
' 2@ method D@

eof

TURBO-FORTH ORIENTE OBJET

Cahier des charges et d‚finitions: brouillon de travail
                                   OCT/89 Michel ZUPAN

               Principes g‚n‚raux
               Notion d'objet et de classe.
               Proc‚dure et M‚thode
               H‚ritage: SubClass SuperClass
               H‚ritage multiple
               ChaŒnage dynamique
               Encapsulation
               Notion de "Early and Late Binding"
               Pile d'objets


Principes g‚n‚raux
------------------
L'incorporation du concept d'objets r‚pond au besoin d'accroissement de
complexit‚ d'un langage oblig‚ de manipuler des entit‚s de plus en plus
structur‚es.

Le but de la programmation par objets est d'utiliser un plus haut niveau de
structuration et d'abstraction de fa‡on … simplifier la conception,
l'‚criture, la maintenance et la r‚-utilisation des programmes. A l'inverse,
les consid‚rations d'optimisation aux plus bas niveaux (espaces m‚moire,
vitesse d'ex‚cution) peuvent passer pour secondaires.

Le vocable Forth orient‚ objet s'inscrit cependant comme un sous-vocable
d'extension au vocabulaire Forth dont il conserve la syntaxe propre. Le
mixage entre forth proc‚dural et forth objet doit rester naturel sans aucune
restriction.

Notion d'objet et de classe.
---------------------------
Une classe est une collection d'objets quelconques.
Un objet est une instance terminale d'une et d'une seule classe.
Chaque classe est d‚finie
-par ses 'ClassVar' (taille des objets,segment allou‚,types,champs,attributs
etc...) qui servent … d‚finir la repr‚sentation des objets.
-par ses 'Procedure' qui sont les messages auquels peuvent r‚pondre un objet.

 CLASS VOITURE            / une CLASSE
   CLASSVAR  MOTEUR       / une VARIABLE DE CLASSE
   PROCEDURE ROULE        / une PROCEDURE
 VOITURE OBJECT FERRARI   / un  OBJET

 FERRARI ROULE    / passage d'un message … un objet


Un objet possŠde une existence et un comportement exclusivement d‚termin‚s
par la classe … laquelle il appartient.


Procedure et Method
-------------------
Un objet ne peut r‚pondre … une proc‚dure que si celle-ci fait partie des
proc‚dures de sa classe. Un avortement d'erreur est g‚n‚r‚ dans le cas
contraire.

Plusieurs classes peuvent poss‚der la mˆme proc‚dure: il s'agit d'un verbe
forth g‚n‚rique dont l'action est vectoris‚e pour chaque classe.

Un vecteur donn‚ de proc‚dure est une m‚thode.

Une proc‚dure peut avoir plusieurs m‚thodes, jusqu'… autant qu'il y a de
classes poss‚dant cette proc‚dure.

Ainsi: Class Integer Procedure Store
       ' !   Method  Store
       Class Double  Procedure Store
       ' 2!  Method  Store

Selon qu'un objet sera Integer ou Double, la proc‚dure Store qui lui sera
appliqu‚e r‚pondra … la m‚thode ! ou 2!

Pour une classe donn‚e, il n'y a qu'une m‚thode par proc‚dure. Cette m‚thode
est la mˆme pour tous les objets de la classe.

On appelle s‚lection (s‚lection de m‚thode) le processus qui traduit une
proc‚dure en sa m‚thode pour une classe donn‚e.


H‚ritage: SubClass SuperClass
-----------------------------
Tous les objets cr‚‚s dans une classe h‚ritent des proc‚dures et m‚thodes
propres de cette classe.

Une classe peut ˆtre d‚finie comme sous-classe d'une autre: elle h‚rite
alors de toutes les proc‚dures de sa super-classe.

Une classe peut ˆtre d‚finie comme super-classe d'une autre: toutes les
proc‚dures qui y seront d‚finies appartiendront aussi … sa ou ses sous-
classes mais elle ne poss‚dera pas celles d‚finies dans ces derniŠres.

Une classe peut avoir plusieurs sous-classes mais au plus une super-classe:
le systŠme veille … la conformit‚ de l'arbre des classes.

Chaque proc‚dure de classe peut avoir sa m‚thode propre, ‚ventuellement
red‚finissable selon le principe de "r‚‚criture" des m‚thodes.

Par d‚faut la s‚lection dans une classe s'opŠre dans sa super-classe.

L'exemple pr‚c‚dent est mieux ‚crit:
      Class Variables Procedure Store
      Variables SubClass Integer  ' !  method Store
      Variables SubClass Double   ' 2! method Store
      Integer Object Petit  64 Petit Store
      Double  Object Grand  64.5678 Grand Store

A noter que Store n'a pas de m‚thode d‚finie dans Variables: un objet de
Variables ne pourra r‚pondre … Store sauf s'il est formul‚ sous la forme
'Integer Store' ou 'Double Store' en deux mots alors que tout objet de
Integer ou Double r‚pondra correctement au seul message Store par la bonne
m‚thode.

      Class Variables SuperClass MotForth Procedure .Name
      : (.Name) 2- body> >name .id ; that method .Name

Un objet Variables, Integer ou Double r‚pondra … .Name affichant son nom
mais un objet MotForth ne r‚pondra pas … Store. On peut d'ailleurs imaginer
que MotForth n'ait pas d'objets propres mais soit seulement le pŠre de
plusieurs classes.


H‚ritage multiple
-----------------
On entend par h‚ritage multiple la possibilit‚ pour une classe d'h‚riter des
proc‚dures de plusieurs classes. Dans le cas de classes organis‚es en arbre
comme ci-dessus, une classe n'h‚rite que de son unique super-classe (et
implicitement des super-classes au dessus). Ce mode d'h‚ritage est dit simple.
L'h‚ritage multiple suppose une organisation en r‚seau avec un systŠme
complexe de gestion des ambigu‹t‚s par priorit‚s.

Comme dans la plupart des extensions objets aux langages proc‚duraux actuels,
nous ne proposerons d'abord qu'un h‚ritage simple. L'h‚ritage multiple pourrait
ˆtre une option ult‚rieure 'greffable' sur l'h‚ritage simple.


ChaŒnage dynamique
------------------
A n'importe quel moment, il est possible d'ajouter une proc‚dure dans une
classe. Tous les objets d‚j… d‚finis dans cette classe et tous ceux de ses
sous-classes h‚riteront de cette proc‚dure. Il n'est pas n‚cessaire de
d‚finir un nombre fixe de proc‚dures lors de la d‚finition d'une nouvelle
classe. Les m‚thodes qui sont h‚rit‚es avec chaque proc‚dure, peuvent ˆtre
red‚finies ult‚rieurement pour chaque classe.

Une proc‚dure est en terme Forth un mot vectoris‚ sur une chaŒne dynamique
de m‚thodes, chaque m‚thode ‚tant propre … une classe. Dans le cas o— une
proc‚dure aurait ‚t‚ successivement vectoris‚e sur plus d'une m‚thode pour
une mˆme classe, c'est toujours la derniŠre m‚thode qui sera s‚lectionn‚e.


encapsulation
-------------
Le chaŒnage dynamique des m‚thodes d'une proc‚dure ne fait pas appel aux
chaŒnages des vocabulaires du langage Forth. Par contre, pour ne pas
alourdir les vocabulaires avec des m‚thodes, un vocabulaire sp‚cial appel‚
METHODS est charg‚ de contenir cach‚es (concept Hiding des langages O.O.)
les d‚finitions des diverses m‚thodes utilis‚es par les proc‚dures visibles.

      Integer SubClass Annee
      Methods definition
      : Annee! dup 1980 2000 between not abort" Ann‚e invalide"
               Store ;
      that method Store
      Forth definitions
      Annee Object AnneeCourante
      1988 AnneeCourante Store

Le mot Annee! est cach‚: seule la proc‚dure g‚n‚rale Store est utile.
Notez que la m‚thode Annee! utilise dans sa d‚finition la proc‚dure auquelle
elle est destin‚e. En effet il s'agit ici de la m‚thode Integer Store.

On appelle encapsulation le proc‚d‚ visant … cacher et … limiter l'accŠs aux
m‚thodes pour ne laisser visible que les proc‚dures (ou messages). Le
vocabulaire METHODS n'est qu'une forme primitive d'encapsulation.


Notion de "Early and Late Binding"
----------------------------------

Forth est un langage rapide. Pour cette raison, le "binding" des m‚thodes
est de type "early" ce qu'on peut traduire par transmission imm‚diate.
Lorsqu'on utilise dans une d‚finition forth une proc‚dure, c'est une m‚thode
qui est compil‚e. Ainsi le dernier mot compil‚ dans Annee! ci dessus est !
(qui est bien la m‚thode Integer Store) comme on peut le v‚rifier par
Methods SEE annee!

L'utilisation des objets en Forth garantit donc un run-time aussi rapide
qu'en Forth proc‚dural, plus rapide mˆme que la compilation de mots
vectoris‚s. Cette notion d'early binding est fondamentale pour comprendre la
philosophie du Forth Objet.

Les L.O.O. utilisent pourtant souvent le "late binding" : dans ce cas, une
proc‚dure est compil‚e et ce n'est qu'… l'ex‚cution que la m‚thode de classe
appropri‚e est d‚termin‚e et ex‚cut‚e. Le "late binding" ou transmission
terminale est lent … l'ex‚cution justifiant des techniques dynamiques
complexes et gourmandes en m‚moire.

L'‚criture en Forth Objet a besoin du "late binding", tout particuliŠrement
dans les mots de d‚finition. Ceci est autoris‚ en Forth naturel par le
concept [COMPILE] auquel s'ajoute en Turbo-Forth celui d' $EXECUTE.

Ex:  Variables Procedure ValeurParDefaut Procedure Initialise
     : (initialise) [compile] ValeurParDefaut [compile] Store ;
     that method Initialise

     Integer ' 0 method ValeurParDefaut
     Double 0 0 2constant 00 that method ValeurParDefaut
     Annee  1980 constant Debut that method ValeurParDefaut

La proc‚dure Initialise n'a qu'une m‚thode dans Variables et utilise une
transmission terminale des proc‚dures Store et ValeurParDefaut. Seules
doivent ˆtre d‚finies les diverses ValeurParDefaut des sous-classes.

On dira qu' Initialise est une proc‚dure virtuelle.

L'usage du "late-binding" doit ˆtre prudente et limit‚e … un seul niveau
tant que la s‚lection n'est pas r‚entrante.


Pile d'objets
-------------
C'est au niveau du late-binding que se pose le problŠme d'une s‚lection
r‚entrante. Comme souvent en matiŠre d'algorithme r‚cursifs, la solution passe
par une pile. Pile d'objets ou plus exactement pile de classes.

Par rapport au modŠle non r‚entrant, ceci remplacerait la variable ACTIVE par
une pile de classes actives. La manipulation de la pile par les proc‚dures
devrait cependant rester transparente pour le programmeur sous peine d'alourdir
le concept de fa‡on inacceptable.

