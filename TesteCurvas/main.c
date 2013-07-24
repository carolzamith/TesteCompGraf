//
//  main.c
//  TesteCurvas
//
//  Created by Carolina Zamith Cunha e Marcelle Guine on 7/23/13.
//  Copyright (c) 2013 Carolina Zamith Cunha e Marcelle Guine. All rights reserved.
//

/* Todos os pontos de controle sao convertidos para pontos Bezier para
 permitir o uso do OpenGL evaluators */


#include <GLUT/glut.h>

GLint tx,ty,angulo;
GLfloat ex,ey;

typedef enum
{
    BEZIER,
    BSPLINE
} tipoCurva;

void calculaMatriz(tipoCurva type, float m[4][4]);
void mult(float m[4][4], float v[4][3], float r[4][3]);

/* Cores para desenhar */
GLfloat colors[][3] =
{
    { 1.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 1.0 }
};


/* Numero maximo de pontos de controle */
#define MAX_CPTS  25

GLfloat cpts[MAX_CPTS][3];
int ncpts = 0;
int grauCurva = 3;
tipoCurva tipo = BEZIER;

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


/* B-spline para matriz de Bezier  */
static float mbspline[4][4] =
{
    { 1.0/6.0, 4.0/6.0, 1.0/6.0, 0.0 },
    { 0.0, 4.0/6.0, 2.0/6.0, 0.0 },
    { 0.0, 2.0/6.0, 4.0/6.0, 0.0 },
    { 0.0, 1.0/6.0, 4.0/6.0, 1.0/6.0 },
};

static float midentity[4][4] =
{
	{ 1.0, 0.0, 0.0, 0.0},
	{ 0.0, 1.0, 0.0, 0.0},
	{ 0.0, 0.0, 1.0, 0.0},
	{ 0.0, 0.0, 0.0, 1.0}
};



/* Calcula a matriz usada na transformacao dos pontos de controle */
void calculaMatriz(tipoCurva type, float m[4][4])
{
    int i, j;
    
    switch (type)
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
static void desenhaCurva(tipoCurva type)
{
    int i;
    int step;
    GLfloat newcpts[4][3];
    
    float m[4][4];
    
    /* Calcula a matriz de pontos de controle, e o step */
    calculaMatriz(type, m);
    
	if(type == BSPLINE)
        step = 1;
	else
        step = 3;
    
    glColor3fv(colors[type]);
    
    /* Desenha as curvas */
    i = 0;
    while (i + 3 < ncpts)
    {
        /* Calcula com os devidos pontos de controle */
        mult(m, &cpts[i], newcpts);
        
        /* Desenha a curva com o OpenGL evaluators */
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &newcpts[0][0]);
        glMapGrid1f(30, 0.0, 1.0);
        glEvalMesh1(GL_LINE, 0, 30);
        
        /* Avanca ao proximo segmento */
        i += step;
    }
    
    glFlush();
}

static void rotacao(float angulo_em_graus, float eixox, float eixoy, float eixoz){
    
    #if defined(ASSIGNMENT_ROTACAO_ESCALA_TRANSLACAO)
        matrix_struct matrix;
        make_rotation_matrix(angulo_em_graus, eixox, eixoy, eixoz, matrix);
        glMultMatrixf(matrix.data());
    #else
        glRotatef(angulo_em_graus, eixox, eixoy, eixoz);
    #endif
    
}

static void translacao(){
    
}

static void escala(){


}

static void desenha(){
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-150.0f, -30.0f, 0);
    glScalef(-40.0f, -20.0f, 1);
    glRotatef(0, 0, 0, 1);
    glFlush();
}


/* Mostra os pontos de controle */
static void display(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
    
    glBegin(GL_POINTS);
    for (i = 0; i < ncpts; i++)
        glVertex3fv(cpts[i]);
    glEnd();
    
    glFlush();
}


void menu(int num)
{    
    switch(num) {
            
        /*Sair*/
		case 0:
            glutDestroyWindow(window);
			exit(0);
			break;
        
        /*Limpar tela*/
        case 1:
            ncpts = 0;
			glutPostRedisplay();
            break;
        
        /*Não definido*/
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
            
        /*Criar curva*/
        case 11:
            if (tipo == BSPLINE)
                desenhaCurva(BSPLINE);
            else
                desenhaCurva(BEZIER);
            break;
            
        /*Excluir curva*/
		case 12:
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
            break;
            
        /*Rotacioar curva*/
		case 16:
            break;
            
        /*Escalar curva*/
        case 17:
            break;
            
        /*Abrir arquivo*/
		case 18:
            break;
            
        /*Salvar arquivo*/
		case 19:{
            break;
        }
	}
}


void createMenu(void)
{
    arquivoSubmenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Abrir", 18);
    glutAddMenuEntry("Salvar", 19);
    
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
    glutAddMenuEntry("Criar curva", 11);
    glutAddMenuEntry("Excluir curvas", 12);
    glutAddSubMenu("Alterar tipo", tipoCurvaSubmenu_id);
    glutAddSubMenu("Alterar grau", grauCurvaSubmenu_id);
    glutAddSubMenu("Transformar curva", transformarCurvaSubmenu_id);
    
    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Curva", curvaSubmenu_id);
    glutAddMenuEntry("Limpar tela", 1);
    glutAddSubMenu("Arquivo", arquivoSubmenu_id);
    glutAddMenuEntry("Sair do programa", 0);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/* Esse metodo pega os novos pontos de controle */
static void mouse(int button, int state, int x, int y)
{
    float wx, wy;
    
    /* Apenas nos interessa os cliques com o botao esquerdo */
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;
    
    /* Translada para o nosso sistema de coordenadas */
    wx = (2.0 * x) / (float)(width - 1) - 1.0;
    wy = (2.0 * (height - 1 - y)) / (float)(height - 1) - 1.0;
    
    
    /* Armazena pontos ate o maximo permitido */
    if (ncpts == MAX_CPTS)
        return;
    
    /* Salva o ponto */
    cpts[ncpts][0] = wx;
    cpts[ncpts][1] = wy;
    cpts[ncpts][2] = 0.0;
    ncpts++;
    
    /* Desenha o ponto */
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);

    glBegin(GL_POINTS);
        glVertex3f(wx, wy, 0.0);
    glEnd();
    
    glFlush();
}


/* Essa rotina lida com a dimensao da janela */
void reshape(int w, int h)
{
    width = w;
    height = h;
    
    /* Lida com transformacoes */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
    
    
    #if defined(ASSIGNMENT_AVISO)
    
        #if defined(ASSIGNMENT_ROTACAO_ESCALA_TRANSLACAO)
            const int assignment_number = 1;
        #else
            const int assignment_number = 0;
        #endif
    
    #endif
    
}


int main(int argc, char **argv)

{
    /* Inicializando o programa */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("curvas");
    
    /* Chamadas de funções */
    
    glutCreateWindow("Computação Gráfica - Construção de Curvas");
    
    /* Chamadas de funções */
    createMenu();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
	glClearColor(2.0, 2.0, 1.0, 1.0);
    glEnable(GL_MAP1_VERTEX_3);
    
    glutMainLoop();
}