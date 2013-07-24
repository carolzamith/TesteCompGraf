//
//  main.c
//  TesteCurvas
//
//  Created by Carolina Zamith Cunha on 7/23/13.
//  Copyright (c) 2013 Carolina Zamith Cunha. All rights reserved.
//

// **
// **
// **   Menu do programa:
// **
// **    q - Sai do programa
// **    c - Limpa a tela
// **    e - Apaga as curvas
// **    b - Desenha curvas de Bezier
// **    s - Desenha curvas de B-spline
// **
// */

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

void keyboard(unsigned char key, int x, int y);
void calculaMatriz(tipoCurva type, float m[4][4]);
void mult(float m[4][4], float v[4][3], float r[4][3]);

/* Cores para desenhar */
GLfloat colors[][3] =
{
    { 1.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 1.0 }
};


#define MAX_CPTS  25            /* Numero maximo de pontos de controle */

GLfloat cpts[MAX_CPTS][3];
int ncpts = 0;

static int width = 500, height = 500;           /* tamanho da janela */


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
    
	if(type == BSPLINE) step = 1;
	else step = 3;
    
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


/* Aqui nós lidamos com a entrada do usuario */
void keyboard(unsigned char key, int x, int y)
{
    static tipoCurva lasttype = BEZIER;
    
    switch (key)
    {
        case 'q': case 'Q':
            exit(0);
            break;
        case 'c': case 'C':
			ncpts = 0;
			glutPostRedisplay();
            break;
        case 'e': case 'E':
            glutPostRedisplay();
            break;
        case 'b': case 'B':
            desenhaCurva(BEZIER);
            lasttype = BEZIER;
            break;
        case 's': case 'S':
            desenhaCurva(BSPLINE);
            lasttype = BSPLINE;
            break;
    }
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
    /* Inicializando o programa !!! */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("curvas");
    
    /* Chamadas de funções */
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
	glClearColor(2.0, 2.0, 1.0, 1.0);
    glEnable(GL_MAP1_VERTEX_3);
    
    
    glutMainLoop();
}