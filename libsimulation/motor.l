

%{

#define YY_DECL int motorscan(char *name, double *diameter, double *length, double *masse_totale, double *masse_poudre, double **thrust, int *ndata)

/* entête C */

/* #include "c++rocket.h" */

double P[2][200];
char bidon[32];
int foo;
int count;
int i;
int j;

int tmp1, tmp2, tmp3;
double dtmp1, dtmp2;
%}

%x body
/*entete de flex*/

%%

<*>#+.*\n     ;/*elimination des caracteres inutile */
^[ ]+         ;
<*>^\t+       ;
<*>\n         ;
<INITIAL>";"+.*\n ;





01.*[ ].* {
  sscanf(yytext, "%s%s%s", bidon, bidon, name);
  /*printf("%s\n", name);*/
  count = 0;
}

07.*[ ].* {
  sscanf(yytext, "%s%s%lf", bidon, bidon, &dtmp1);
  diameter[0] = dtmp1*2.54/100;
  /*printf("%f\n", diameter[0]);*/
}

08.*[ ].*[ ].* {
  sscanf(yytext, "%s%s%s%lf", bidon, bidon, bidon, &dtmp1);
  length[0] = dtmp1*2.54/100;
  /*printf("%f\n", length[0]);*/
}

11.*[ ].*[ ].*[ ].* {
  sscanf(yytext, "%s%s%s%s%lf", bidon, bidon, bidon, bidon, &dtmp1);
  masse_poudre[0] = dtmp1/1000;
  /*printf("%f\n", masse_poudre[0]);*/
}

12.*[ ].*[ ].*[ ].* {
  sscanf(yytext, "%s%s%s%s%lf", bidon, bidon, bidon, bidon, &dtmp1);
  masse_totale[0] = dtmp1/1000;
  /*printf("%f\n", masse_totale[0]);*/
}

20.*[ ].*[ ].*[ ].* {
  sscanf(yytext, "%s%s%s%lf%lf", bidon, bidon, bidon, &P[0][count], &P[1][count]);
  count++;
}

64.* {
  //thrust[0] = (double*) malloc(sizeof(double[count]));
  thrust[0] = new double[count];
  //thrust[1] = (double*) malloc(sizeof(double[count]));
  thrust[1] = new double[count];
  
  ndata[0] = count;
  
  for (j = 0; j < count; j++) {
    thrust[0][j] = P[0][j];
    thrust[1][j] = P[1][j] * 4.448221615; //1lb X 9.80665 X 0.45359237 kg/lb
  }

}


[0-9]{2}.*[ ].* { /*garbage all other data*/
}


[A-Z0-9]+[ ]*[0-9]+[ ]*[0-9]+[ ]*[0-9]+[ ]*[0-9]+"."[0-9]+[ ]*[0-9]+"."[0-9]+[ ]*[A-Z0-9]+[ ]* {
  sscanf(yytext, "%s%d%d%d%lf%lf%s", name, &tmp1, &tmp2, &foo, masse_poudre, masse_totale, bidon);
  
  diameter[0] = (double)tmp1/1000;
  length[0] = (double)tmp2/1000;
  
  count = 0;
  BEGIN(body);
}

<body>[0-9]+"."[0-9]+[ ]*[0-9]+"."[0-9]+[ ]* {
  sscanf(yytext, "%lf%lf", &P[0][count], &P[1][count]);
  count++;
}

<body>";"+.*\n {

  //thrust[0] = (double*) malloc(sizeof(double[count]));
  thrust[0] = new double[count];
  //thrust[1] = (double*) malloc(sizeof(double[count]));
  thrust[1] = new double[count];
  ndata[0] = count;
  for (i = 0; i < 2; i++)
    for (j = 0; j < count; j++)
      thrust[i][j] = P[i][j];  
  BEGIN(INITIAL);
}



%%

int yywrap()
{
     
  return 1;
}






