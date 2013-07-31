//
//  main.c
//  TesteCurvas
//
//  Created by Carolina Zamith Cunha e Marcelle Guine on 7/23/13.
//  Copyright (c) 2013 Carolina Zamith Cunha e Marcelle Guine. All rights reserved.
//

/*
 Bezier   ->  grauCurva = ncpnts - 1
 */


#include <GLUT/glut.h>

/* Cores para desenhar */
GLfloat colors[][3] = { { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };

typedef enum{
    BEZIER,
    BSPLINE
} tipoCurva;

tipoCurva tipo = BEZIER;

typedef enum{
    NO_ESTADO,
    DESENHANDO,
    TRANSLADAR,
    TRANSLADANDO,
    ROTACIONANDO,
    ESCALANDO
} estadoPrograma;

estadoPrograma estado = NO_ESTADO;


GLint tx,ty,angulo;
GLfloat ex,ey;

GLfloat pontosDeControle[10][3];
int numPontosDeControle = 0;
int grauCurva = 3;

static int window;
static int width = 800, height = 600;

static int menu_id;
static int arquivoSubmenu_id;
static int curvaSubmenu_id;
static int tipoCurvaSubmenu_id;
static int grauCurvaSubmenu_id;
static int transformarCurvaSubmenu_id;


/* Matriz */
void mult(float m[4][4], float v[4][3], float r[4][3])
{
    int i, j, k;
    
    for (i = 0; i < 4; i++)
        for (j = 0; j <3; j++)
            for (k = 0, r[i][j] = 0.0; k < 4; k++)
                r[i][j] += m[i][k] * v[k][j];
}

static float mbspline[4][4] =
{
    { 1.0/6.0, 4.0/6.0, 1.0/6.0, 0.0 },
    { 0.0, 4.0/6.0, 2.0/6.0, 0.0 },
    { 0.0, 2.0/6.0, 4.0/6.0, 0.0 },
    { 0.0, 1.0/6.0, 4.0/6.0, 1.0/6.0 }
};

static float midentity[4][4] =
{
	{ 1.0, 0.0, 0.0, 0.0},
	{ 0.0, 1.0, 0.0, 0.0},
	{ 0.0, 0.0, 1.0, 0.0},
	{ 0.0, 0.0, 0.0, 1.0}
};



/* Calcula a matriz usada na transformacao dos pontos de controle */
void calculaMatriz(float m[4][4])
{
    int i, j;
    
    switch (tipo)
    {
        case BEZIER:
            for (i = 0; i < 4; i++)
                for (j = 0; j < 4; j++)
					m[i][j] = midentity[i][j];
            break;
        case BSPLINE:
            for (i = 0; i < 4; i++)
                for (j = 0; j < 4; j++)
                    m[i][j] = mbspline[i][j];
            break;
    }
}

/* Desenha a curva indicada, usando os pontos de controle */
static void desenharCurva()
{
    int i;
    int aux = 3;
    GLfloat novoPontoDeControle[4][3];
    float m[4][4];
    
    if (tipo == BSPLINE) {
        aux = 1;
    }
    
    /* Calcula a matriz de pontos de controle */
    calculaMatriz(m);
    
    glColor3fv(colors[tipo]);
    /* Desenha as curvas */
    i = 0;
    while (i + 3 < numPontosDeControle)
    {
        /* Calcula com os devidos pontos de controle */
        mult(m, &pontosDeControle[i], novoPontoDeControle);
        
        /* Desenha a curva com o OpenGL evaluators */
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &novoPontoDeControle[0][0]);
        glMapGrid1f(30, 0.0, 1.0);
        glEvalMesh1(GL_LINE, 0, 30);
        
        /* Avanca ao proximo segmento */
        i += aux;
    }
    
    glFlush();
}

static void rotacionar(tempontosdecontrole){
//    if(tempontosdecontrole.x!=0){
//        for (int i=0; tempontosdecontrole.size; i++) {
//            
//        }
//    }

}

/* Mostra os pontos de controle */
static void display(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
    
    glBegin(GL_POINTS);
    for (i = 0; i < numPontosDeControle; i++)
        glVertex3fv(pontosDeControle[i]);
    glEnd();
    
    glFlush();
}

void mouse_to_coordenada(int x, int y, float *wx_p,  float *wy_p)
{
    /* Adapta ao nosso sistema de coordenadas */
    *wx_p = (2.0 * x) / (float)(width - 1) - 1.0;
    *wy_p = (2.0 * (height - 1 - y)) / (float)(height - 1) - 1.0;
}

static void transladar(int x, int y){
    
    float topX = -10, topY = -10;
    
    int num_pontos = sizeof(pontosDeControle)/sizeof(pontosDeControle[0]);
    
    for (int i = 0; i < num_pontos; i++) {
        if (pontosDeControle[i][0] && pontosDeControle[i][0] > topX) topX = pontosDeControle[i][0];
        if (pontosDeControle[i][1] && pontosDeControle[i][1] > topY) topY = pontosDeControle[i][1];
    }
    
    float wx, wy;
    mouse_to_coordenada(x, y, &wx, &wy);
    
    float translationXOffset = wx - topX;
    float translationYOffset = wy - topY;
    
    for (int i = 0; i < num_pontos; i++) {
        if (pontosDeControle[i][0]) pontosDeControle[i][0] += translationXOffset;
        if (pontosDeControle[i][1]) pontosDeControle[i][1] += translationYOffset;
    }
    
    
    estado = TRANSLADANDO;
    display();
    desenharCurva();
}

static void escalar(){


}

//static void desenha(){
//    
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glClear(GL_COLOR_BUFFER_BIT);
//    glColor3f(0.0, 0.0, 0.0);
//    glTranslatef(-150.0f, -30.0f, 0);
//    glScalef(-40.0f, -20.0f, 1);
//    glRotatef(0, 0, 0, 1);
//    glFlush();
//}


void menu(int num)
{    
    switch(num) {
            
        /*Sair*/
		case 0:
            glutDestroyWindow(window);
			exit(0);
			break;
        
        /*Abrir arquivo*/
        case 1:
           	 break;
        
        /*Salvar arquivo*/
        case 2:
            break;
            
        /*Alterar grau da curva 3-10*/
		case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            grauCurva = num;
            break;
            
        /*Desenhar curva*/
        case 11:
            estado = DESENHANDO;
            break;
            
        /*Excluir curvas*/
		case 12:
            numPontosDeControle = 0;
			glutPostRedisplay();
            break;
            
        /*Escolher tipo: BSPLINE */
		case 13:
            tipo = BSPLINE;
            break;
            
        /*Escolher tipo: BEZIER*/
        case 14:
            tipo = BEZIER;
            break;
            
        /*Transladar curva*/
		case 15:
            estado = TRANSLADAR;
            break;
            
        /*Rotacioar curva*/
		case 16:
            estado = ROTACIONANDO;
            break;
            
        /*Escalar curva*/
        case 17:
            estado = ESCALANDO;
            break;
	}
}


void createMenu(void)
{
    arquivoSubmenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Abrir", 1);
    glutAddMenuEntry("Salvar", 2);
    
    tipoCurvaSubmenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("B-spline ", 13);
    glutAddMenuEntry("Bezier", 14);
    
    grauCurvaSubmenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("3", 3);
    glutAddMenuEntry("4", 4);
    glutAddMenuEntry("5", 5);
    glutAddMenuEntry("6", 6);
    glutAddMenuEntry("7", 7);
    glutAddMenuEntry("8", 8);
    glutAddMenuEntry("9", 9);
    glutAddMenuEntry("10", 10);
    
    transformarCurvaSubmenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Transladar", 15);
    glutAddMenuEntry("Rotacionar", 16);
    glutAddMenuEntry("Escalar", 17);
    
    curvaSubmenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Desenhar curva", 11);
    glutAddMenuEntry("Excluir curvas", 12);
    glutAddSubMenu("Alterar tipo", tipoCurvaSubmenu_id);
    glutAddSubMenu("Alterar grau", grauCurvaSubmenu_id);
    glutAddSubMenu("Transformar curva", transformarCurvaSubmenu_id);
    
    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Curva", curvaSubmenu_id);
    glutAddSubMenu("Arquivo", arquivoSubmenu_id);
    glutAddMenuEntry("Sair do programa", 0);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}




/* Esse metodo pega os novos pontos de controle */
static void mouse(int botao, int tecla, int x, int y)
{
    float wx;
    float wy;
    mouse_to_coordenada(x, y, &wx, &wy);
    
    /*Desenha na tela*/
    if (estado == DESENHANDO)
    {    
        if (botao != GLUT_LEFT_BUTTON || tecla != GLUT_DOWN)
            return;
        
        /* Armazena pontos ate o maximo permitido */
        if (numPontosDeControle == 10)
            return;

        /* Salva o ponto */
        pontosDeControle[numPontosDeControle][0] = wx;
        pontosDeControle[numPontosDeControle][1] = wy;
        pontosDeControle[numPontosDeControle][2] = 0.0;
        numPontosDeControle++;
            
        /* Desenha o ponto */
        glColor3f(0.0, 0.0, 0.0);
        glPointSize(5.0);
            
        glBegin(GL_POINTS);
            glVertex3f(wx, wy, 0.0);
        glEnd();
            
        glFlush();
        
        /*Desenha curva*/
        if(grauCurva == (numPontosDeControle - 1)){
            desenharCurva();
            //TODO: guardar a curva atual em um array de curvas para a manipulação de curvas individuais
//            numPontosDeControle = 0;
        }
    }
    
    /*Aplica transformações*/
    else{
        if (estado == TRANSLADAR)
            transladar(x, y);
        else if (estado == ROTACIONANDO)
            rotacionar(1);
        else if (estado == ESCALANDO)
            escalar();
    }
}


int main(int argc, char **argv)
{
    /* Inicializando o programa */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Computação Gráfica - Construção de Curvas");
    
    
    /* Chamadas de funções */
    createMenu();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
	glClearColor(2.0, 2.0, 1.0, 1.0);
    glEnable(GL_MAP1_VERTEX_3);
    
    glutMainLoop();
}