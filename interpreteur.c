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
    INTO_ERR, DOWNTO_ERR, DDOT_ERR, DD_ERR,
    ND_ERR, ID_NUM_PO_ERR,INST_PCODE_ERR
} CODES_ERR;

FILE *fichier;
char Car_Cour;
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

INSTRUCTION PCODE[TAILLECODE];
int OFFSET = -1;
int SP = -1;
int  PC = -1;

void Lire_Car() {
    Car_Cour = fgetc(fichier);
}


void ECRIRE2(MNEMONIQUES M, int A) {
    if (PC == TAILLECODE) {
        printf("ERROR: PC is equal to TAILLECODE.\n");
    } else {
        PC = PC + 1;
        PCODE[PC].MNE = M;
        PCODE[PC].SUITE = A;
    }
}

void ECRIRE1(MNEMONIQUES M) {
    if (PC == TAILLECODE) {
        printf("ERROR: PC is equal to TAILLECODE.\n");
    } else {
        ECRIRE2(M,-1);
    }
}


void lire_instruction(){
    char mot[20];
    char nombreString[20];
    Lire_Car();
    int indice = 0;
    while (isalpha(Car_Cour)){
        mot[indice++] = Car_Cour;
        Lire_Car();
    }
    mot[indice] = '\0';
    if(Car_Cour=='\t') Lire_Car();
    indice = 0;
    while(isdigit(Car_Cour)){
        nombreString[indice++] = Car_Cour;
        Lire_Car();
    }
    nombreString[indice] = '\0';
    int nombre = atoi(nombreString);
    if (strcmp(mot, "LDA") == 0) {
        ECRIRE2(LDA, nombre);
    } else if (strcmp(mot, "LDI") == 0) {
        ECRIRE2(LDI, nombre);
    } else if (strcmp(mot, "INT") == 0) {
        ECRIRE2(INT, nombre);
    } else if (strcmp(mot, "BZE") == 0) {
        ECRIRE2(BZE, nombre);
    } else if (strcmp(mot, "BRN") == 0) {
        ECRIRE2(BRN, nombre);
    } else if (strcmp(mot, "LDV") == 0) {
        ECRIRE1(LDV);
    } else if (strcmp(mot, "ADD") == 0) {
        ECRIRE1(ADD);
    } else if (strcmp(mot, "SUB") == 0) {
        ECRIRE1(SUB);
    } else if (strcmp(mot, "MUL") == 0) {
        ECRIRE1(MUL);
    } else if (strcmp(mot, "DIV") == 0) {
        ECRIRE1(DIV);
    } else if (strcmp(mot, "LEQ") == 0) {
        ECRIRE1(LEQ);
    } else if (strcmp(mot, "GEQ") == 0) {
        ECRIRE1(GEQ);
    } else if (strcmp(mot, "NEQ") == 0) {
        ECRIRE1(NEQ);
    } else if (strcmp(mot, "LSS") == 0) {
        ECRIRE1(LSS);
    } else if (strcmp(mot, "GTR") == 0) {
        ECRIRE1(GTR);
    } else if (strcmp(mot, "HLT") == 0) {
        ECRIRE1(HLT);
    } else if (strcmp(mot, "STO") == 0) {
        ECRIRE1(STO);
    } else if (strcmp(mot, "INN") == 0) {
        ECRIRE1(INN);
    } else if (strcmp(mot, "PRN") == 0) {
        ECRIRE1(PRN);
    } else {
        printf("%s\n",mot);
        printf("instruction non reconnue");
        exit(EXIT_FAILURE);
    }
}

void INTER_INST(INSTRUCTION INST) {
    int val1, adr, val2;
    switch (INST.MNE) {
        case INT:
            OFFSET = SP = INST.SUITE;
            PC++;
            break;
        case LDI:
            MEM[++SP] = INST.SUITE;
            PC++;
            break;
        case LDA:
            MEM[++SP] = INST.SUITE;
            PC++;
            break;
        case STO:
            val1 = MEM[SP--];
            adr = MEM[SP--];
            MEM[adr] = val1;
            PC++;
            break;
        case LDV:
            adr = MEM[SP--];
            MEM[++SP] = MEM[adr];
            PC++;
            break;
        case ADD:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = (val2 + val1);
            PC++;
            break;
        case SUB:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = (val2 - val1);
            PC++;
            break;
        case MUL:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = (val2 * val1);
            PC++;
            break;
        case DIV:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = (val2 / val1);
            PC++;
            break;
        case EQL:
            val1 = MEM[SP--];
            val2 = MEM[SP--];
            MEM[++SP] = (val1 == val2);
            PC++;
            break;
        case LEQ:
            val2 = MEM[SP--];
            val1 = MEM[SP--];
            MEM[++SP] = (val1 <= val2);
            PC++;
            break;
        case NEQ:
            val2 = MEM[SP--];
            val1 = MEM[SP--];
            MEM[++SP] = (val1 != val2);
            PC++;
            break;
        case GTR:
            val2 = MEM[SP--];
            val1 = MEM[SP--];
            MEM[++SP] = (val1 > val2);
            PC++;
            break;
        case LSS:
            val2 = MEM[SP--];
            val1 = MEM[SP--];
            MEM[++SP] = (val1 < val2);
            PC++;
            break;
        case GEQ:
            val2 = MEM[SP--];
            val1 = MEM[SP--];
            MEM[++SP] = (val1 >= val2);
            PC++;
            break;
        case BZE:
            if (MEM[SP--] == 0)
                PC = INST.SUITE;
            else
                PC++;
            break;
        case BRN:
            PC = INST.SUITE;
            break;
        case PRN:
            val1 = MEM[SP--];
            printf("%d\n",val1);
            PC++;
            break;
        case INN:
            scanf("%d",&val1);
            adr = MEM[SP--];
            MEM[adr] = val1;
            PC++;
            break;
        case HLT:
            break;
        default:
            break;
    }
}



const char* MNEString(MNEMONIQUES mne) {
    switch (mne) {
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case EQL:
            return "EQL";
        case NEQ:
            return "NEQ";
        case GTR:
            return "GTR";
        case LSS:
            return "LSS";
        case GEQ:
            return "GEQ";
        case LEQ:
            return "LEQ";
        case PRN:
            return "PRN";
        case INN:
            return "INN";
        case INT:
            return "INT";
        case LDI:
            return "LDI";
        case LDA:
            return "LDA";
        case LDV:
            return "LDV";
        case STO:
            return "STO";
        case BRN:
            return "BRN";
        case BZE:
            return "BZE";
        case HLT:
            return "HLT";
        default:
            return "UNKNOWN";
    }
}

void INTER_PCODE(){
    PC = 0;
    while(PCODE[PC].MNE!=HLT) INTER_INST(PCODE[PC]);
}

int main(){
    fichier = fopen("fichierSortie.txt", "r");
    if (fichier == NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    lire_instruction();
    while(!feof(fichier)){
        lire_instruction();
    }

    INTER_PCODE();

    fclose(fichier);
}