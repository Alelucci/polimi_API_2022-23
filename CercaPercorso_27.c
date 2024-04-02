#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int listlenght=0;

typedef enum{false, true}bool;

typedef struct stazione{//Salvo le stazioni in una lista doppiamente concatenata
    int distanza;//distanza della stazione dall'inizio dell'autostrada
    struct stazione *next;//stazione successiva
    struct stazione *prev;//stazione precedente
    int numeromacchine;//numero di macchine disponibili alla stazione
    int *autonomia;//array delle macchine disponibili
    int bestautonomia;
}stazione;

stazione *HEAD=NULL;
stazione *TAIL=NULL;

void aggiungi_stazione(int distanza);
void demolisci_stazione(int distanza);
void aggiungi_auto(int distanza);
void rottama_auto(int distanza);
void pianifica_percorso(int distanzaP);

//stazione *nextReach(stazione *x);
stazione *prevReach(stazione *x);
stazione *nuovaStazione(int distanza, int numeromacchine);
void merge_sort(int *array,int  p,int r);
void merge(int *array, int p, int q, int r);
stazione *cercaStazione(int distanza);

//void printList(stazione *head);
void freeList(stazione *head);

int main(){

    char AddS[19]="aggiungi-stazione";
    char DelS[19]="demolisci-stazione";
    char AddA[15]="aggiungi-auto";
    char DelA[14]="rottama-auto";
    char Start[20]="pianifica-percorso";

    int distanza=0;

    char cmd[20];
    //scan comandi
    while(fscanf(stdin,"%s %d",cmd,&distanza)>0){
        
        if(strncmp(cmd,AddS,18)==0){
            aggiungi_stazione(distanza);   
        }
        
        else if(strncmp(cmd,DelS,18)==0){
            demolisci_stazione(distanza);
        }

        else if(strncmp(cmd,AddA,14)==0){
            aggiungi_auto(distanza);
        }

        else if(strncmp(cmd,DelA,13)==0){
            rottama_auto(distanza);
        }

        else if(strncmp(cmd,Start,19)==0){
            pianifica_percorso(distanza);
        }
               
    }
    
    //printList(HEAD);
    freeList(HEAD);
    exit(0);
}

void aggiungi_stazione(int distanza){//aggiunge una stazione alla lista
    bool aggiunta=false;
    int k=0;
    //creazione nuova stazione
    if(fscanf(stdin,"%d",&k));
    stazione* newS=nuovaStazione(distanza,k);
    
    if (HEAD == NULL || TAIL == NULL){//fprintf(stdout,"primo inserimento\n");
        HEAD = TAIL = newS;
        aggiunta=true;
    }

    else if(distanza < HEAD->distanza){//fprintf(stdout,"inserimento in testa\n");       
        newS->next = HEAD;
        newS->prev = NULL;
        HEAD->prev = newS;
        HEAD = newS;
        aggiunta=true;
    }

    else if(distanza > TAIL->distanza){//fprintf(stdout,"inserimento in coda\n");           
        newS->prev = TAIL;
        newS->next = NULL;
        TAIL->next = newS;
        TAIL = newS; 
        aggiunta=true;
    }

    else if(distanza-HEAD->distanza <= TAIL->distanza-distanza){//fprintf(stdout,"inserimento controllando da head in avanti\n");
        stazione* nextS = HEAD;

        while (nextS!=NULL && nextS->distanza < distanza) nextS = nextS->next;//ricerca stazione 'nextS' prima della quale inserire 'newS'    

        if(nextS!=NULL && nextS->distanza > distanza){
            newS->prev = nextS->prev;
            nextS->prev = newS;
            newS->next = nextS;
            if(newS->prev!=NULL)
                newS->prev->next = newS;
              
            aggiunta=true;
        }
        
    }
    else if(distanza-HEAD->distanza > TAIL->distanza-distanza){//{fprintf(stdout,"inserimento controllando da tail: ");
        stazione* prevS = TAIL;

        while (prevS!=NULL && prevS->distanza > distanza) prevS = prevS->prev;//ricerca stazione 'prevS' dopo della quale inserire 'newS'
        
        if(prevS!=NULL && prevS->distanza < distanza){
            newS->next = prevS ->next;
            prevS->next = newS;
            newS->prev = prevS;
            if(newS->next!=NULL)
                newS->next->prev = newS;
            
            aggiunta=true;
        }     
    }

    if(aggiunta){
        listlenght++;
        fprintf(stdout,"aggiunta\n");
        
    }
    else{
        fprintf(stdout,"non aggiunta\n");//stazione già esistente
        free(newS->autonomia);
        free(newS);
        
    }
    return;
}

void demolisci_stazione(int distanza){//elimina una stazione alla lista
    //fprintf(stdout,"demolisci_stazione\n");
    bool demolita = false;
    stazione *x=cercaStazione(distanza);
    if((HEAD->distanza == distanza || TAIL->distanza==distanza)&& HEAD==TAIL){//eliminazione unico elemento in lista
        HEAD=TAIL=NULL;
        demolita=true;
    }
    else if(x==HEAD){//elimina testa
        HEAD = x->next;
        if(x->next!=NULL) x->next->prev = x->prev;
        
        demolita=true;
    }
    else if(x==TAIL){//elimina coda
        TAIL = x->prev;
        if(x->prev!=NULL) x->prev->next = x->next; 
        
        demolita=true;
    }
    else if(x!=NULL){
        if(x->prev!=NULL) x->prev->next = x->next;
            
        if(x->next!=NULL) x->next->prev = x->prev;   
            
        demolita=true;   
    }

    if(demolita){   
        if(x!=NULL){
            free(x->autonomia);
            free(x);
        }
        fprintf(stdout,"demolita\n");
        listlenght--;
    }
    else fprintf(stdout,"non demolita\n");//stazione non trovata
    return;
    
}

void aggiungi_auto(int distanza){//aggiunge un auto ad una stazione
    int autonomia=0,size=0;
    if(fscanf(stdin,"%d",&autonomia));
    
    stazione *x=cercaStazione(distanza);
    
    if(x!=NULL){//resize array + aggiunta macchina in fondo all'array
        x->numeromacchine++;
        size = x->numeromacchine;
    
        x->autonomia=realloc(x->autonomia,(size)*sizeof(int));
        x->autonomia[size-1]=autonomia;
        if(autonomia > x->bestautonomia) x->bestautonomia=autonomia;
        fprintf(stdout,"aggiunta\n");
    }
    
    else fprintf(stdout,"non aggiunta\n");//stazione non trovata

    return;
}

void rottama_auto(int distanza){//rottama un auto di una stazione
    int autonomia=0,i=0,size=0;
    if(fscanf(stdin,"%d",&autonomia));

    //ricerca stazione x a cui rottamare l'auto "autonomia"
    stazione *x=cercaStazione(distanza);

    if(x!=NULL){
        size = x->numeromacchine;
        merge_sort(x->autonomia,0,size-1);
        while(i<size && x->autonomia[i]>autonomia){//ricerca macchina da rottamare
            i++;
        }

        if(x->autonomia[i]==autonomia){//eliminazione macchina tramite shift + resize array
            
            x->numeromacchine--;
            size=x->numeromacchine;
            if(x->bestautonomia==autonomia) {
                if(size>0) x->bestautonomia=x->autonomia[i+1];
                else x->bestautonomia=0;
            }
            while(i<size){
                x->autonomia[i]=x->autonomia[i+1];
                i++;
            }
            
            x->autonomia=realloc(x->autonomia,(size)*sizeof(int));
            
            fprintf(stdout,"rottamata\n");   
        }
        else fprintf(stdout,"non rottamata\n");//macchina non trovata
    }

    else fprintf(stdout,"non rottamata\n");//stazione non trovata
    
    return;
}

void pianifica_percorso(int distanzaP){//calcola il percorso migliore tra la stazione di partenza e arrivo
    int distanzaA=0;
    int numerotappe=0;
    bool percorso=false;
    bool reverseprint=false;

    if(fscanf(stdin,"%d",&distanzaA));

    stazione *x = HEAD;
    stazione *partenza=NULL;
    stazione *arrivo=NULL;
    
    //assegnamento stazioni di partenza e di arrivo, calcolo del numero massimo di tappe possibili tra i due ed ordinamento delle macchine delle stazioni intermedie
    int maxNtappe=2;
    while (x!= NULL && (arrivo==NULL || partenza==NULL)){
        
        if(x->distanza==distanzaA && arrivo==NULL)
            arrivo=x;
        if(x->distanza==distanzaP && partenza==NULL)
            partenza=x;
        if(arrivo!=NULL || partenza!=NULL){ 
            maxNtappe++;
        }
        x = x->next;
    }
    stazione **tappe=malloc(maxNtappe*sizeof(stazione));
    
    int dapercorrere = distanzaA-distanzaP;  
    if(distanzaA<distanzaP) dapercorrere = distanzaP-distanzaA;

    int percorribili = 0;
    if(partenza->numeromacchine==0) percorribili=0;
    else percorribili=partenza->bestautonomia;

    int percorsi=0;

    if(arrivo==partenza){//fprintf(stdout,"stessa stazione\n");
        tappe[0]=partenza;
        percorso=true;
    }
    else if(partenza->next == arrivo || partenza->prev == arrivo){//fprintf(stdout,"stazioni adiacenti");
        if(percorribili<dapercorrere){//fprintf(stdout,"non raggiungibili\n");
        }
    
        else {//fprintf(stdout,"raggiungibili\n");
            tappe[0]=partenza;
            tappe[1]=arrivo;
            percorsi=dapercorrere;
            numerotappe++;
            percorso=true;
        }
    }
    
    else if(percorribili>0 && distanzaA>distanzaP){//fprintf(stdout,"senso di percorrenza distanza crescente\n");
        reverseprint=true;
        int eccessox=0;
        
        stazione *tappa=arrivo;

        for(numerotappe=0; percorsi < dapercorrere; numerotappe++){//fprintf(stdout,"calcolo un percorso a tappe minime\n");
            tappe[numerotappe]=tappa; 

            percorsi = distanzaA - tappa->distanza;
            x=partenza;
            eccessox = x->distanza + x->bestautonomia;

            while(x!=NULL && eccessox < tappa->distanza){//scorro fino a trovare la prima stazione che raggiunga la tappa
                x=x->next;
                eccessox = x->distanza + x->bestautonomia;
            }

            if(x==NULL || x==tappa){
                numerotappe++;
                break; 
            }
            tappa=x;
        }
        if(percorsi>=dapercorrere){
            percorso=true;
        }
    }

    else if(percorribili>0 && distanzaA<distanzaP){//fprintf(stdout,"senso di percorrenza distanza decrescente\n");
        
        stazione *tappa=partenza;
        stazione *nextappa=tappa;
        stazione *reachX=tappa;
        stazione *bestreach=tappa;
        
        for(numerotappe=0;percorsi < dapercorrere; numerotappe++){//fprintf(stdout,"calcolo un percorso a tappe minime\n");
            tappe[numerotappe]=tappa;
            if(tappa->numeromacchine>0)percorribili = tappa->bestautonomia;
            else percorribili=0;
            percorsi = distanzaP - tappa->distanza; 
            
            if(tappa!=arrivo && tappa->prev!=NULL && tappa->distanza - percorribili > tappa->prev->distanza) break;//autonomia massima insufficiente 
            
            nextappa=tappa;
            bestreach=0;
            for(x=tappa; x!=NULL && nextappa!=arrivo && x->distanza >= tappa->distanza-percorribili; x=x->prev){//fprintf(stdout,"scelta tappa migliore\n");
                
                reachX=prevReach(x); 
                if(reachX->distanza < distanzaA) reachX=arrivo;
                
                if(bestreach==0 || reachX->distanza <= bestreach->distanza){
                    bestreach=reachX;
                    nextappa=x;
                }
            }
            tappa=nextappa;
        }

        tappe[numerotappe]=arrivo;
        int reachprev=0;
        //controllo se esistono tappe adatte più vicine all'inizio dell'autostrada 
        for(int i=numerotappe-1; i>0; i--){
            reachprev=prevReach(tappe[i-1])->distanza;
            for(x=tappe[i];x!=NULL && x->distanza>tappe[i+1]->distanza && x->distanza>=reachprev;x=x->prev){
                reachX=prevReach(x);
                if(reachX->distanza <= tappe[i+1]->distanza) tappe[i]=x;
            }
        }
        if(percorsi>=dapercorrere){
            percorso=true;
        }
    }

    if(percorso){
        int i;
        if(reverseprint){
            for(i=numerotappe-1; i>0;i--) fprintf(stdout,"%d ",tappe[i]->distanza);
            fprintf(stdout,"%d\n",tappe[i]->distanza);
        }
        else{
            for(i=0; i<numerotappe-1;i++) fprintf(stdout,"%d ",tappe[i]->distanza);
            fprintf(stdout,"%d\n",tappe[i]->distanza);
        }
    }
    else fprintf(stdout,"nessun percorso\n");

    free(tappe);
    //printList(HEAD);
    
}

/*stazione *nextReach(stazione *x){//trova la stazione piu lontana raggiungibile da x (senso di percorrenza crescente)
    int eccesso = (x->distanza - x->bestautonomia);
    stazione *reach=x; 
    while(reach!=TAIL && reach->distanza < eccesso) reach=reach->next;
    if(reach->distanza > eccesso) reach=reach->prev;
    return reach;
}*/

stazione *prevReach(stazione *x){//trova la stazione più lontana raggiungibile da x (senso di percorrenza decrescente)
    if(x->numeromacchine>0){
        int eccesso = (x->distanza - x->bestautonomia);
        stazione *reach=x; 
        while(reach!=HEAD && reach->distanza > eccesso) reach=reach->prev;
        if(reach->distanza < eccesso) reach=reach->next;
        return reach;
    }
    else return x;
}

stazione *nuovaStazione(int distanza,int numeromacchine){//crea una nuova stazione
    //fprintf(stdout,"createNode\n");
    stazione *newS = malloc(sizeof(stazione));
    newS->numeromacchine=numeromacchine;
    newS->distanza = distanza;
    newS->autonomia = calloc(numeromacchine,sizeof(int));
    newS->next = NULL;
    newS->prev = NULL;
    newS->bestautonomia=0;

    int inputsize = numeromacchine + numeromacchine*9+1;
    char *input=calloc(inputsize,sizeof(char));

    if(numeromacchine>0){
        if(fgets(input,inputsize,stdin)!=NULL);
        int base=1;
        int i,j;
        for(j=numeromacchine-1,i=inputsize-1;i>=0 && j>=0;i--){
            if(input[i]==' '){
                base=1;
                if(newS->autonomia[j]>newS->bestautonomia)newS->bestautonomia=newS->autonomia[j];
                j--;
            }
            else if(input[i]!='\0' && input[i]!='\n'){
                newS->autonomia[j]=newS->autonomia[j]+((input[i]-'0')*base);
                base=base*10;
            }
        }
    }
    free(input);
    return newS;
}

void merge_sort(int *array,int p,int r){//funzioni per ordinamento merge sort (decrescente)
    //fprintf(stdout,"merge_sort\n");
    if(p<r){
        int q=(p+r)/2;
        
        merge_sort(array,p,q);
        merge_sort(array,q+1,r);
        merge(array,p,q,r);
    }
}

void merge(int *array, int p, int q, int r){
    //fprintf(stdout,"merge\n");
    
    int n1=q-p+1;
    int n2=r-q;

    int *L=malloc(n1*sizeof(int));
    int *R=malloc(n2*sizeof(int));

    int i,j,kk;
    for(i=0;i<n1;i++)
        L[i]=array[p+i];
    //L[n1]=INT_MAX;
 
    for(j=0;j<n2;j++)
        R[j]=array[q+j+1];
    //R[n2]=INT_MAX;
    
    i=0;j=0;kk=p;
    while(i<n1 && j<n2){
        if(L[i]>=R[j]){
            array[kk]=L[i];
            i++;
        }
        else{
            array[kk]=R[j];
            j++;
        }
        kk++;
    }

    while (i<n1){
        array[kk] = L[i];
        i++;
        kk++;
    }

    while (j<n2){
        array[kk]=R[j];
        j++;
        kk++;
    }
    free(L);free(R);
}

stazione *cercaStazione(int distanza){//restituisce la stazione situata alla distanza data in input o NULL se non esiste
    stazione *z=NULL;
    if(distanza==HEAD->distanza) z=HEAD;

    else if(distanza==TAIL->distanza) z=TAIL;

    else if(distanza - HEAD->distanza >= TAIL->distanza - distanza){
        z = HEAD;
        while(z!=NULL && z->distanza<distanza) z=z->next;
    }
    else if(distanza - HEAD->distanza < TAIL->distanza - distanza){
        z = TAIL;
        while(z!=NULL && z->distanza > distanza) z=z->prev;
    }
    if(z==NULL || z->distanza!=distanza) return NULL;
    else return z;   
}

/*void printList(stazione *head){//funzione per stampare la lista di stazioni e macchine corrispondenti
    //fprintf(stdout,"printlist\n");
    if(head!=NULL){
        int i=0;
        fprintf(stdout,"%d [",head->distanza);

        if(head->numeromacchine >0){
        for(i=0; i<head->numeromacchine-1;i++) fprintf(stdout,"%d, ",head->autonomia[i]);
        fprintf(stdout,"%d",head->autonomia[i]);
        }
        fprintf(stdout,"]\n");
        printList(head->next);
    }    
}*/

void freeList(stazione *head){//funzione per liberare la lista di stazioni
    if(head!=NULL){
        freeList(head->next);
        free(head->autonomia);
        free(head);
    }
}
