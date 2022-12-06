input = """
%REGOLE DI EDIT ESPRESSE ATTRAVERSO VINCOLI DI INTEGRITA`
%della PROGRAMMAZIONE LOGICA DISGIUNTIVA


% RAPPRESENTAZIONE DEI DATI
% predicati di input: parentela(X,Y), eta(X,Y), statocivile(X,Y), sesso(X,Y).
% Ad esempio, parentela(X,Y) indica che il componente X e' in relazione di 
% parentela Y con il capofamiglia. 
% parentela(2,madre) indica cioe' che il componente numero 2 e' 
% la madre del capofamiglia. 
% predicati di output: Parentela(X,Y), Eta(X,Y), StatoCivile(X,Y), Sesso(X,Y).

%DOMINI
% Supponiamo che le eta` dei componenti del nucleo familiare
% possano variare in questi range.
etaPadre(E) :- 33<E, E<110. %#int(E), 33<E, E<110.
etaMadre(E) :- 30<E, E<110. %#int(E), 30<E, E<110.
etaFiglio(E) :- E<110. %#int(E), E<110.
etaCapofamiglia(E) :- 18<E, E<110. %#int(E), 18<E, E<110.
etaSposa(E) :- 12<E, E<110. %#int(E), 12<E, E<110.

% Supponiamo inoltre, di avere i seguenti dati statistici 
etaMedia(40,capofamiglia,30,sposa).
etaMedia(50,capofamiglia,40,sposa).
etaMedia(60,capofamiglia,50,sposa).
etaMedia(70,capofamiglia,60,sposa).
etaMedia(80,capofamiglia,70,sposa).
etaMedia(90,capofamiglia,80,sposa).
etaMedia(40,capofamiglia,10,figlio).
etaMedia(50,capofamiglia,20,figlio).
etaMedia(60,capofamiglia,30,figlio).
etaMedia(70,capofamiglia,40,figlio).
etaMedia(80,capofamiglia,50,figlio).
etaMedia(90,capofamiglia,60,figlio).
etaMedia(30,sposa,10,figlio).
etaMedia(40,sposa,20,figlio).
etaMedia(50,sposa,30,figlio).
etaMedia(60,sposa,40,figlio).
etaMedia(70,sposa,50,figlio).
etaMedia(80,sposa,60,figlio).
etaMedia(40,madre,20,capofamiglia).
etaMedia(50,madre,30,capofamiglia).
etaMedia(60,madre,40,capofamiglia).
etaMedia(70,madre,50,capofamiglia).
etaMedia(80,madre,60,capofamiglia).
etaMedia(90,madre,70,capofamiglia).
etaMedia(50,padre,20,capofamiglia).
etaMedia(60,padre,30,capofamiglia).
etaMedia(70,padre,40,capofamiglia).
etaMedia(80,padre,50,capofamiglia).
etaMedia(90,padre,60,capofamiglia).


%VINCOLI
:- Parentela(X,capofamiglia), X!=1.

%Il capofamiglia deve essere maggiorenne.
:- Eta(1,X), X<18.

% La moglie del capofamiglia deve avere almeno 12 anni.
:- Parentela(X,sposa), Eta(X,Y), Y<12.

%la madre del capo-famiglia deve avere piu` di 15 anni
%in piu` del capo-famiglia.
:- Parentela(X,madre), Eta(X,Y), Eta(1,Z), T=Z+15, Y < T.

%il padre del capo-famiglia deve avere piu` di 15 anni
%in piu` del capo-famiglia.
:- Parentela(X,padre), Eta(X,Y), Eta(1,Z),  T=Z+15, Y < T.

%la madre del capo-famiglia deve avere piu` di 30 anni di eta`
:- Parentela(X,madre), Eta(X,Y), Y<30.

%il padre del capo-famiglia deve avere piu` di 30 anni di eta`
:- Parentela(X,padre), Eta(X,Y), Y<30.

%Il figlio del capo-famiglia deve avere almeno 15 anni meno del capo-famiglia.
:- Parentela(X,figlio), Eta(X,Y), Eta(1,Z), T=Y+15, Z < T.

%Il figlio del capo-famiglia deve avere almeno 12 anni meno della sposa.
:- Parentela(X,figlio), Eta(X,Y), Parentela(H,sposa), Eta(H,Z), T=Y+12, Z < T.

%il padre non puo` essere femmina
:- Parentela(X,padre), Sesso(X,femmina).

%la madre non puo` essere maschio
:- Parentela(X,madre), Sesso(X,maschio).

%assumo che in una coppia il capo-famiglia debba essere il marito
%(non so` se e` sempre vero)
:- Parentela(X,sposa), Sesso(X,maschio).

%assumo quindi che il capo-famiglia sia maschio)
:- Sesso(1,femmina).

%la moglie deve essere coniugata
:- Parentela(X,sposa), StatoCivile(X,single).

%se ha moglie, allora il capo-famiglia deve essere coniugato
:- Parentela(X,sposa), StatoCivile(1,single).

%non si puo` avere due diverse relazioni di parentela col capofamiglia
:- Parentela(X,padre), Parentela(X,madre).
:- Parentela(X,padre), Parentela(X,sposa).
:- Parentela(X,madre), Parentela(X,sposa).
:- Parentela(X,padre), Parentela(X,figlio).
:- Parentela(X,madre), Parentela(X,figlio).
:- Parentela(1,X), X <> capofamiglia.

% Non e' possibile che due persone diverse abbiano la stessa relazione di 
% parentela col capofamiglia.
:- Parentela(X,Z), Parentela(Y,Z), X<Y, Z<>figlio.

% Nel caso in cui un modulo sia errato ed esistano diverse possibilita'
% per correggerlo, preferiamo sempre quella che richiede il minor numero 
% di cambiamenti. Per questo utilizziamo i seguenti weak constraints.
%:- parentelaErrata(X,Y). [1:2]  
%:- etaErrata(X,Y). [1:2] 
%:- sessoErrato(X,Y). [1:2] 
%:- statocivileErrato(X,Y). [1:2] 

% Inoltre, se esistono due possibili correzioni che richiedono lo 
% stesso numero di cambiamenti e uno di questi cambiamenti riguarda l'eta`,
% scegliamo quella in cui il nuovo valore di eta` generato e` 
% il piu` vicino possibile ai dati statistici.
%:- etaErrata(X1,_), Parentela(X1,P1), Eta(X2,Y), Parentela(X2,P2), etaMedia(EM,P1,Y,P2), Eta(X1,E1), E1>EM, E1=EM+D. [D:1]

%:- etaErrata(X1,_), Parentela(X1,P1), Eta(X2,Y), Parentela(X2,P2), etaMedia(Y,P2,EM,P1), Eta(X1,E1), E1>EM, E1=EM+D. [D:1]

%:- etaErrata(X1,_), Parentela(X1,P1), Eta(X2,Y), Parentela(X2,P2), etaMedia(EM,P1,Y,P2), Eta(X1,E1), EM>E1, EM=E1+D. [D:1]

%:- etaErrata(X1,_), Parentela(X1,P1), Eta(X2,Y), Parentela(X2,P2), etaMedia(Y,P2,EM,P1), Eta(X1,E1), EM>E1, EM=E1+D. [D:1]


%definizione del dominio
%(puo` essere evitata se "persona" e` gia` nota)
persona(X):- sesso(X,maschio).
persona(X):- sesso(X,femmina).
persona(X):- parentela(X,padre).
persona(X):- parentela(X,madre).
persona(X):- eta(X,_).
persona(X):- parentela(X,sposa).
persona(X):- statocivile(X,sposato).
persona(X):- parentela(X,figlio).

% nel caso in cui manchino dati queste regole lo segnalano.  
sessoSpecificato(X) :- sesso(X,Y).
sessoErrato(X,nessuno) :- not sessoSpecificato(X), persona(X). 

parentelaSpecificata(X) :- parentela(X,Y).
parentelaErrata(X,nessuno) :- not parentelaSpecificata(X), persona(X). 

statocivileSpecificato(X) :- statocivile(X,Y).
statocivileErrato(X,nessuno) :- not statocivileSpecificato(X), persona(X).

etaSpecificata(X) :- eta(X,Y).
etaErrata(X,nessuno) :- not etaSpecificata(X), persona(X).

%INDIVIDUAZIONE DEGLI ERRORI
% le seguenti regole vengono innescate (il corpo diventa vero) in caso di 
% vincoli violati e specificano i cambiamenti possibili da effettuare 
% per soddisfare i vincoli.

%nessun altro al di fuori di '1' e' capo-famiglia
parentelaErrata(X,capofamiglia) :- parentela(X,capofamiglia), X!=1.

%Il capofamiglia deve essere maggiorenne.
etaErrata(1,X) :- eta(1,X), X<18.

% La moglie del capofamiglia deve avere almeno 12 anni.
parentelaErrata(X,sposa) | etaErrata(X,Y) 
        :- parentela(X,sposa), eta(X,Y), Y<12.

%la madre del capo-famiglia deve avere piu` di 15 anni
%in piu` del capo-famiglia.
parentelaErrata(X,madre) | etaErrata(X,Y) | etaErrata(1,Z) 
        :- parentela(X,madre), eta(X,Y), eta(1,Z), T=Z+15, Y < T.

%il padre del capo-famiglia deve avere piu` di 15 anni
%in piu` del capo-famiglia.
parentelaErrata(X,padre) | etaErrata(X,Y) | etaErrata(1,Z) 
        :- parentela(X,padre), eta(X,Y), eta(1,Z),  T=Z+15, Y < T.

%la madre del capo-famiglia deve avere piu` di 30 anni di eta`
parentelaErrata(X,madre) | etaErrata(X,Y) 
        :- parentela(X,madre), eta(X,Y), Y<30.

%il padre del capo-famiglia deve avere piu` di 30 anni di eta`
parentelaErrata(X,padre) | etaErrata(X,Y) 
        :- parentela(X,padre), eta(X,Y), Y<30.

%Il figlio del capofamiglia deve avere almeno 15 anni meno del capofamiglia.
parentelaErrata(X,figlio) | etaErrata(X,Y) | etaErrata(1,Z) 
        :- parentela(X,figlio), eta(X,Y), eta(1,Z), T=Y+15, Z < T.

%Il figlio del capofamiglia deve avere almeno 12 anni meno della sposa.
parentelaErrata(X,figlio) | etaErrata(X,Y) | etaErrata(H,Z) | 	parentelaErrata(H,sposa)
        :- parentela(X,figlio), eta(X,Y), parentela(H,sposa), eta(H,Z), T=Y+12, Z < T.

%il padre non puo` essere femmina
parentelaErrata(X,padre) | sessoErrato(X,femmina) 
        :- parentela(X,padre), sesso(X,femmina).

%la madre non puo` essere maschio
parentelaErrata(X,madre) | sessoErrato(X,maschio) 
        :- parentela(X,madre), sesso(X,maschio).

%assumo che in una coppia il capo-famiglia debba essere il marito
%(non so` se e` sempre vero)
parentelaErrata(X,sposa) | sessoErrato(X,maschio) 
        :- parentela(X,sposa), sesso(X,maschio).

%assumo quindi che il capofamiglia sia maschio
sessoErrato(1,femmina) :- sesso(1,femmina).

%la moglie deve essere coniugata
parentelaErrata(X,sposa) | statocivileErrato(X,single) 
        :- parentela(X,sposa), statocivile(X,single).

%se ha moglie, allora il capo-famiglia deve essere coniugato
parentelaErrata(X,sposa) | statocivileErrato(1,single) 
        :- parentela(X,sposa), statocivile(1,single).

%non si puo` avere due diverse relazioni di parentela col capofamiglia
parentelaErrata(X,madre) | parentelaErrata(X,padre) 
        :- parentela(X,padre), parentela(X,madre).
parentelaErrata(X,sposa) | parentelaErrata(X,padre) 
        :- parentela(X,padre), parentela(X,sposa).
parentelaErrata(X,madre) | parentelaErrata(X,sposa) 
        :- parentela(X,madre), parentela(X,sposa).
parentelaErrata(X,figlio) | parentelaErrata(X,padre) 
        :- parentela(X,padre), parentela(X,figlio).
parentelaErrata(X,madre) | parentelaErrata(X,figlio) 
        :- parentela(X,madre), parentela(X,figlio).
parentelaErrata(1,X) :- parentela(1,X), X <> capofamiglia.

% Non e' possibile che due persone diverse(eccetto i figli),
% abbiano la stessa relazione di parentela col capofamiglia.
parentelaErrata(X,Z) | parentelaErrata(Y,Z)
        :- parentela(X,Z), parentela(Y,Z), X<Y, Z<>figlio.

% CORREZIONE ERRORI

% Se i dati in input sono esatti, i predicati di output corrispondenti
% rappresenteranno le stesse relazioni.
% Ad esempio, se parentela(2,padre) e' vero, in outup avremo un atomo 
% Parentela(2,padre).
Parentela(X,Y) :- not parentelaErrata(X,Y), parentela(X,Y).
Sesso(X,Y) :- not sessoErrato(X,Y), sesso(X,Y).
Eta(X,Y) :- not etaErrata(X,Y), eta(X,Y).
StatoCivile(X,Y) :- not statocivileErrato(X,Y), statocivile(X,Y).


% In caso di errori, generiamo differenti predicati di output.
Parentela(X,madre) | Parentela(X,padre) 
| Parentela(X,figlio) | Parentela(X,sposa) | Parentela(X,capofamiglia)
:- parentelaErrata(X,_).

Sesso(X,maschio) | Sesso(X,femmina) :- sessoErrato(X,_).

StatoCivile(X,single) | StatoCivile(X,sposato):- statocivileErrato(X,_).

% I seguenti vincoli evitano che il nuovo predicato di output generato
% rappresenti la stessa relazione errata del predicato di input.
:- parentelaErrata(X,Z), Parentela(X,Z).
:- sessoErrato(X,Z), Sesso(X,Z).
:- statocivileErrato(X,Z), StatoCivile(X,Z).

% Ad ogni componente della famiglia e' assegnato un range di valori 
% in cui l'eta' puo' variare.
% Ad esempio, il capofamiglia deve avere almeno 18 anni e non piu' di 110.
% Il padre del capofamiglia deve avere almeno 40 anni e non piu' di 110.
% Percio', per la sostituzione dell'eta' di un componente X, 
% viene tenuta in considerazione la relazione di parentela di X e 
% la nuova eta' generata apparterra' al range relativo ad X.
 
Eta(1,Z) | diffeta(1,Z) 
:- etaErrata(1,Y), etaCapofamiglia(Z), Z<>Y.

Eta(X,Z) | diffeta(X,Z)
:- etaErrata(X,Y), etaPadre(Z), Z<>Y, Parentela(X,padre).

Eta(X,Z) | diffeta(X,Z)
:- etaErrata(X,Y), etaMadre(Z), Parentela(X,madre), Z<>Y.

Eta(X,Z) | diffeta(X,Z)
:- etaErrata(X,Y), etaSposa(Z), Parentela(X,sposa), Z<>Y.

Eta(X,Z) | diffeta(X,Z)
:- etaErrata(X,Y), etaFiglio(Z), Parentela(X,figlio), Z<>Y.

% diffeta e` un predicato ausiliario.
diffeta(X,Z) :- Eta(X,Y), Z<>Y. %#int(Z), Z<>Y.
eta1(X) :- Eta(X,_).
:- persona(X), not eta1(X).


% Famiglia di 4 persone.
parentela(1,capofamiglia).
eta(1,50).
sesso(1,maschio).
statocivile(1,sposato).

parentela(2,figlio).
eta(2,40).
sesso(2,femmina).
statocivile(2,sposato).

parentela(3,figlio).
statocivile(3,single).
eta(3,20).
sesso(3,maschio).

parentela(4,padre).
sesso(4,maschio).
eta(4,80).
statocivile(4,sposato).
"""
output = """
"""
