#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLEINDEX 100
#define TAILLEMEM 100
#define TAILLECODE 100


typedef enum {
    ID_TOKEN, PROGRAM_TOKEN, CONST_TOKEN, VAR_TOKEN,
    BEGIN_TOKEN, END_TOKEN, IF_TOKEN, THEN_TOKEN,
    WHILE_TOKEN, DO_TOKEN, READ_TOKEN, WRITE_TOKEN,
    PV_TOKEN, PT_TOKEN, PLUS_TOKEN, MOINS_TOKEN,
    MULT_TOKEN, DIV_TOKEN, VIR_TOKEN, AFF_TOKEN,
    INF_TOKEN, INFEG_TOKEN, SUP_TOKEN, SUPEG_TOKEN,
    DIFF_TOKEN, PO_TOKEN, PF_TOKEN, EOF_TOKEN,
    NUM_TOKEN, ERREUR_TOKEN, FIN_TOKEN, EG_TOKEN,
    REPEAT_TOKEN, UNTIL_TOKEN, FOR_TOKEN, ELSE_TOKEN,
    CASE_TOKEN, OF_TOKEN, INTO_TOKEN, DOWNTO_TOKEN,
    DDOT_TOKEN
} CODES_LEX;

typedef enum {
    ID_ERR, PROGRAM_ERR, CONST_ERR, VAR_ERR,
    BEGIN_ERR, END_ERR, IF_ERR, THEN_ERR,
    WHILE_ERR, DO_ERR, READ_ERR, WRITE_ERR,
    PV_ERR, PT_ERR, PLUS_ERR, MOINS_ERR,
    MULT_ERR, DIV_ERR, VIR_ERR, AFF_ERR,
    INF_ERR, INFEG_ERR, SUP_ERR, SUPEG_ERR,
    DIFF_ERR, PO_ERR, PF_ERR, EOF_ERR,
    NUM_ERR, ERREUR_ERR, FIN_ERR, EG_ERR,
    CONST_VAR_BEGIN_ERR, VAR_BEGIN_ERR, REPEAT_ERR, UNTIL_ERR,
    FOR_ERR, ELSE_ERR, CASE_ERR, OF_ERR,
    INTO_ERR, DOWNTO_ERR, DDOT_ERR
} CODES_ERR;


typedef struct
{
    CODES_LEX CODE;
    char NOM[20];
} TSym_Cour;

TSym_Cour SYM_COUR;

typedef struct{
    char NOM[20];
    CODES_LEX CLASSE;
    int ADRESSE;
}SYMBOLE;

SYMBOLE TABLESYM[TABLEINDEX];

int OFFSET = -1;
int IND_DER_SYM_ACC = 0;
int PC = 1;
int SP;
int index_Mots = 0;
int choice;
int IND_BZE;
int LABEL_BRN;
int AVANT_BEGIN = 1;

int MEM[TAILLEMEM];

typedef enum {
    ADD, SUB, MUL, DIV, EQL, NEQ, GTR,
    LSS, GEQ, LEQ, PRN, INN, INT, LDI, LDA, LDV,
    STO, BRN, BZE, HLT
} MNEMONIQUES;

typedef struct{
    MNEMONIQUES MNE;
    int SUITE;
}INSTRUCTION;

INSTRUCTION PCODE[TAILLECODE]



int RechercherSym(char mot[20]){
    for(int i = 0;i<index_Mots;i++){
        if(!strcmp(TABLESYM[i].NOM,mot)){
            return i;
        }
    }
}

void AJOUTER(){
    TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
    TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
    TABLESYM[IND_DER_SYM_ACC].NOM = sym.NOM;
    index_Mots++;
}

void Codage_Lex(char mot[20]) {
    int indice_token = -1;

    indice_token = RechercherSym(mot);

    if (indice_token != -1) {
        if ((AVANT_BEGIN == 1) && (indice_token > 10)) {
            ERREUR(DD_ERR);
        } else {
            IND_DER_SYM_ACC = indice_token;
        }
    } else {
        if (AVANT_BEGIN == 1) {
            IND_DER_SYM_ACC = index_Mots;
            AJOUTER();
        } else {
            ERREUR(ND_ERR);
        }
    }
}


void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR){
    if (SYM_COUR.CODE == cl){
        Sym_Suiv();
    }
    else{
        Erreur(COD_ERR);
    }
}

void Erreur(CODES_ERR code){
    printf("Erreur: %d\n", code);
    printf("Current Token: %d\n", SYM_COUR.CODE);
    printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}

void Test_entrer(CODES_LEX cl,CODES_ERR COD_ERR){
    TABLESYM[IND_DER_SYM_ACC].CLASSE = cl;
    TABLESYM[IND_DER_SYM_ACC].NOM = SYM_COUR.NOM;
}

void GENERER1(MNEMONIQUES M) {
    if (PC == TAILLECODE) {
        printf("ERROR: PC is equal to TAILLECODE.\n");
    }else {
        PC = PC + 1;
        PCODE[PC].MNE = M;
    }
}


void GENERER2(MNEMONIQUES M, int A) {
    if (PC == TAILLECODE) {
        printf("ERROR: PC is equal to TAILLECODE.\n");
    } else {
        PC = PC + 1;
        PCODE[PC].MNE = M;
        PCODE[PC].SUITE = A;
    }
}


TSym_Cour sym;

void BLOCK(){
    OFFSET = 0;
    CONSTS();
    VARS();
    PCODE[0].MNE = INT;
    PCODE[0].SUITE = OFFSET;
    INSTS();
}

void PROGRAM(){
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    Test_entrer(ID_TOKEN, ID_ERR);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    GENERER1(HLT);
    Test_entrer(PT_TOKEN, PT_ERR);
}

void AFFEC(){
    Test_Symbole(ID_TOKEN, ID_ERR);
    GENERER2(LDA,TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    Test_Symbole(AFFECT_TOKEN, AFFECT_ERR);
    EXPR();
    GENERER1(STO);
}

void EXPR(){
    TERM();
    while ((SYM_COUR.CODE==PLUS_TOKEN) || (SYM_COUR.CODE==MOINS_TOKEN)){
        Sym_Suiv();
        TERM();
        if(SYM_COUR.CODE==PLUS_TOKEN) GENERER1(ADD);
        else GENERER1(SUB);
    }
}

void TERM(){
    FACT();
    while ((SYM_COUR.CODE==MULTI_TOKEN) || (SYM_COUR.CODE==DIV_TOKEN)){
        Sym_Suiv();
        FACT();
        if(SYM_COUR.CODE==MULTI_TOKEN) GENERER1(MUL);
        else GENERER1(DIV);
    }
}

void FACT(){
    switch (SYM_COUR.CODE) {
        case ID_TOKEN:
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
            GENERER1(LDV);
            Sym_Suiv();
            break;

        case NUM_TOKEN:
            GENERER2(LDI, SYM_COUR.NOM);
            Sym_Suiv();
            break;

        case PO_TOKEN:
            {
                Sym_Suiv();
                EXPR();
                Test_Symbole(PF_TOKEN, PF_ERR);
            };
            break;
        default:
            Erreur(ID_NUM_PO_ERR);
            break;
    }
}

void ECRIRE(){
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();
    GENERER1(PRN);
    while (SYM_COUR.CODE==VIR_TOKEN){
        Sym_Suiv();
        EXPR();
        GENERER1(PRN);
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}

void LIRE(){
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    GENERER1(INN);
    while (SYM_COUR.CODE==VIR_TOKEN){
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
        GENERER1(INN);
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}



void RELOP(){
    switch (SYM_COUR.CODE){
    case EG_TOKEN:
        Test_Symbole(EG_TOKEN, EG_ERR);
        choice = 1;
        break;
    case DIFF_TOKEN:
        Test_Symbole(DIFF_TOKEN, DIFF_ERR);
        choice = 2;
        break;
    case INF_TOKEN:
        Test_Symbole(INF_TOKEN, INF_ERR);
        choice = 3;
        break;
    case SUP_TOKEN:
        Test_Symbole(SUP_TOKEN, SUP_ERR);
        choice = 4;
        break;
    case INFEG_TOKEN:
        Test_Symbole(INFEG_TOKEN, INFEG_ERR);
        choice = 5;
        break;
    case SUPEG_TOKEN:
        Test_Symbole(SUPEG_TOKEN, SUPEG_ERR);
        choice = 6;
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}


void COND(){
    choice = 0;
    EXPR();
    RELOP();
    EXPR();
    switch(choice){
        case 1:
            GENERER1(EQL);
            break;
        case 2:
            GENERER1(NEQ);
            break;
        case 3:
            GENERER1(GTR);
            break;
        case 4:
            GENERER1(GEQ);
            break;
        case 5:
            GENERER1(LSS);
            break;
        case 6:
            GENERER1(LEQ);
            break;
    }
}



void SI(){
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
    GENERER2(BZE,PC+1);
    IND_BZE = PC;
    Test_Symbole(THEN_TOKEN, THEN_ERR);
    INST();
    PCODE[IND_BZE].SUITE = PC+1;
}




void TANTQUE(){
    Test_Symbole(WHILE_TOKEN, WHILE_ERR);
    LABEL_BRN = PC+1;
    COND();
    GENERER1(BZE);
    IND_BZE = PC;
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
    GENERER2(BRN, LABEL_BRN);
    PCODE[INDICE_BZE].SUITE=PC+1;
}


void CONSTS() {
    switch (SYM_COUR.CODE) {
        case CONST_TOKEN: {
            Sym_Suiv();
            sym = SYM_COUR:
            Test_Symbole(ID_TOKEN, ID_ERR);
            Codage_Lex(sym.NOM);
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
            Test_Symbole(EG_TOKEN, EG_ERR);
            sym = SYM_COUR;
            Test_Symbole(NUM_TOKEN, NUM_ERR);
            GENERER2(LDI, sym.NOM);
            GENERER1(STO);
            Test_Symbole(PV_TOKEN, PV_ERR);

            while (SYM_COUR.CODE == ID_TOKEN) {
                sym = SYM_COUR;
                Sym_Suiv();
                Codage_Lex(sym.NOM);
                GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
                Test_Symbole(EG_TOKEN, EG_ERR);
                sym = SYM_COUR;
                Test_Symbole(NUM_TOKEN, NUM_ERR);
                GENERER2(LDI, sym.NOM);
                GENERER1(STO);
                Test_Symbole(PV_TOKEN, PV_ERR);
            };
            break;
        }
        case VAR_TOKEN: break;
        case BEGIN_TOKEN: break;
        default: Erreur(CONST_VAR_BEGIN_ERR); break;
    }
}

void VARS(){
    switch (SYM_COUR.CODE){
    case VAR_TOKEN:
        Sym_Suiv();
        sym = SYM_COUR;
        Test_Symbole(ID_TOKEN, ID_ERR);
        Codage_Lex(sym.NOM);
        while (SYM_COUR.CODE == VIR_TOKEN){
            Sym_Suiv();
            sym = SYM_COUR;
            Test_Symbole(ID_TOKEN, ID_ERR);
            Codage_Lex(sym.NOM);
        }
        Test_Symbole(PV_TOKEN, PV_ERR);
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(VAR_BEGIN_ERR);
        break;
    }
}





void INSTS(){
    if (SYM_COUR.CODE == BEGIN_TOKEN){
        Sym_Suiv();
        INST();
        while (SYM_COUR.CODE == PV_TOKEN){
            Sym_Suiv();
            INST();
        }
        if (SYM_COUR.CODE == END_TOKEN){
            Sym_Suiv();
        }
        else{
            Erreur(END_ERR);
        }
    }
    else{
        Erreur(BEGIN_ERR);
    }
}


void INST(){
    switch (SYM_COUR.CODE){
    case BEGIN_TOKEN:
        INSTS();
        break;
    case ID_TOKEN:
        AFFEC();
        break;
    case IF_TOKEN:
        SI();
        break;
    case WHILE_TOKEN:
        TANTQUE();
        break;
    case WRITE_TOKEN:
        ECRIRE();
        break;
    case READ_TOKEN:
        LIRE();
        break;
    case REPEAT_TOKEN:
        REPETER();
        break;
    case FOR_TOKEN:
        POUR();
        break;
    case CASE_TOKEN:
        CAS();
        break;
    default:
        break;
    }
}