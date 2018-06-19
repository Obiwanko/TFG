#include "Utilidades.h" // Biblioteca de Utilidades


void planoXY(int resolucion)
// resolucion: numero de divisiones del lado (>0)
/* dibuja el cuadrado unidad (-0.5,-0.5)(0.5,0.5) con
"resolucion" divisiones usando strips horizontales.
Las normales se generan como (0,0,1)            */
{
	if (resolucion < 1) resolucion = 1;				//resolucion minima
	static const int NP = resolucion + 1;			//numero de puntos en una dimension
	static const float delta = 1.0f / resolucion;		//distancia entre dos consecutivos
														// generacion de coordenadas
	GLfloat* vertices = new GLfloat[NP*NP * 3];
	int cont = 0;
	for (int i = 0; i < NP; i++)
		for (int j = 0; j < NP; j++) {
			vertices[cont++] = -0.5f + delta * j;		//x
			vertices[cont++] = -0.5f + delta * i;		//y
			vertices[cont++] = 0.0;				//z
		}
	// generacion de normales
	GLfloat* normales = new GLfloat[NP*NP * 3];
	cont = 0;
	for (int i = 0; i < NP; i++)
		for (int j = 0; j < NP; j++) {
			normales[cont++] = 0.0;					//nx
			normales[cont++] = 0.0;					//ny
			normales[cont++] = 1.0;					//nz
		}
	// generacion de coordenadas de textura
	GLfloat * texcoor = new GLfloat[NP*NP * 2];
	cont = 0;
	for (int i = 0; i < NP; i++)
		for (int j = 0; j < NP; j++) {
			texcoor[cont++] = 0 + delta * j;			//s
			texcoor[cont++] = 0 + delta * i;			//t
		}
	// carga en gpu
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normales);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoor);
	// generacion de indices para cada strip horizontal
	GLuint* indices = new GLuint[2 * NP];				// indices por strip
	for (int k = 0; k < resolucion; k++) {			// k: strip corriente
		cont = 0;
		for (int i = 0; i < NP; i++) {
			indices[cont++] = i + (k + 1)*NP;			// counterclock
			indices[cont++] = i + k * NP;
		}
		glDrawElements(GL_TRIANGLE_STRIP, 2 * NP, GL_UNSIGNED_INT, indices);
	}
	// limpiando
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	delete[] vertices;
	delete[] normales;
	delete[] texcoor;
	delete[] indices;
}
void quad(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int M, int N)
// Dibuja un cuadrilatero con resolucion MxN y fija la normal 
{
	if (M<1) M = 1; if (N<1) N = 1;	// Resolucion minima
	GLfloat ai[3], ci[3], bj[3], dj[3], p0[3], p1[3];
	// calculo de la normal (v1-v0)x(v3-v0) unitaria 
	GLfloat v01[] = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
	GLfloat v03[] = { v3[0] - v0[0], v3[1] - v0[1], v3[2] - v0[2] };
	GLfloat normal[] = { v01[1] * v03[2] - v01[2] * v03[1] ,
		v01[2] * v03[0] - v01[0] * v03[2] ,
		v01[0] * v03[1] - v01[1] * v03[0] };
	float norma = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	glNormal3f(normal[0] / norma, normal[1] / norma, normal[2] / norma);
	// ai: punto sobre el segmento v0v1, bj: v1v2, ci: v3v2, dj: v0v3
	for (int i = 0; i<M; i++) {
		// puntos sobre segmentos a y c
		for (int k = 0; k<3; k++) {
			ai[k] = v0[k] + i * (v1[k] - v0[k]) / M;
			ci[k] = v3[k] + i * (v2[k] - v3[k]) / M;
		}
		// strip vertical. i=s, j=t
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= N; j++) {
			for (int k = 0; k<3; k++) {
				// puntos sobre los segmentos b y d
				bj[k] = v1[k] + j * (v2[k] - v1[k]) / N;
				dj[k] = v0[k] + j * (v3[k] - v0[k]) / N;

				// p0= ai + j/N (ci-ai)
				p0[k] = ai[k] + j * (ci[k] - ai[k]) / N;
				// p1= p0 + 1/M (bj-dj)
				p1[k] = p0[k] + (bj[k] - dj[k]) / M;
			}
			// punto izquierdo
			glTexCoord2f(i*1.0f / M, j*1.0f / N);  // s,t
			glVertex3f(p0[0], p0[1], p0[2]);
			// punto derecho
			glTexCoord2f((i + 1)*1.0f / M, j*1.0f / N);
			glVertex3f(p1[0], p1[1], p1[2]);
		}
		glEnd();
	}
}
void quadtex(Point3D v0, Point3D v1, Point3D v2, Point3D v3,
	GLfloat smin, GLfloat smax, GLfloat tmin, GLfloat tmax,
	int M, int N)
	// Dibuja un cuadrilatero con resolucion MxN con normales y coordenadas de textura
{
	if (M<1) M = 1; if (N<1) N = 1;	// Resolucion minima
	Point3D ai, ci, bj, dj, p0, p1;
	// calculo de la normal (v1-v0)x(v3-v0) unitaria 
	Point3D v01( v1.x - v0.x, v1.y - v0.y, v1.z - v0.z );
	Point3D v03( v3.x - v0.x, v3.y - v0.y, v3.z - v0.z );
	Point3D normal(v01.y * v03.z - v01.z * v03.y ,
		v01.z * v03.x - v01.x * v03.z ,
		v01.x * v03.y - v01.y * v03.x );
	float norma = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	glNormal3f(normal[0] / norma, normal[1] / norma, normal[2] / norma);
	// ai: punto sobre el segmento v0v1, bj: v1v2, ci: v3v2, dj: v0v3
	for (int i = 0; i<M; i++) {
		// puntos sobre segmentos a y c
		for (int k = 0; k<3; k++) {
			ai[k] = v0[k] + i * (v1[k] - v0[k]) / M;
			ci[k] = v3[k] + i * (v2[k] - v3[k]) / M;
		}
		// strip vertical. i=s, j=t
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= N; j++) {
			for (int k = 0; k<3; k++) {
				// puntos sobre los segmentos b y d
				bj[k] = v1[k] + j * (v2[k] - v1[k]) / N;
				dj[k] = v0[k] + j * (v3[k] - v0[k]) / N;

				// p0= ai + j/N (ci-ai)
				p0[k] = ai[k] + j * (ci[k] - ai[k]) / N;
				// p1= p0 + 1/M (bj-dj)
				p1[k] = p0[k] + (bj[k] - dj[k]) / M;
			}
			// punto izquierdo
			glTexCoord2f(smin + (smax - smin)*i / M, tmin + (tmax - tmin)*j / N);  // s,t
			glVertex3f(p0[0], p0[1], p0[2]);
			// punto derecho
			glTexCoord2f(smin + (smax - smin)*(i + 1) / M, tmin + (tmax - tmin)*j / N);
			glVertex3f(p1[0], p1[1], p1[2]);
		}
		glEnd();
	}
}
void ejes()
{
	//Construye la Display List compilada de una flecha vertical
	GLuint id = glGenLists(1);
	glNewList(id, GL_COMPILE);
	//Brazo de la flecha
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();
	//Punta de la flecha
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0.0, 0.0, -1 / 10.0);
	glutSolidCone(1 / 50.0, 1 / 10.0, 10, 1);
	glPopMatrix();
	glEndList();

	//Ahora construye los ejes
	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	//Eje X en rojo
	glColor3fv(ROJO);
	glPushMatrix();
	glRotatef(-90, 0, 0, 1);
	glCallList(id);
	glPopMatrix();
	//Eje Y en verde
	glColor3fv(VERDE);
	glPushMatrix();
	glCallList(id);
	glPopMatrix();
	//Eje Z en azul
	glColor3fv(AZUL);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glCallList(id);
	glPopMatrix();
	//Esferita en el origen
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(0.05, 8, 8);
	glPopAttrib();
	//Limpieza
	glDeleteLists(id, 1);
}

void texto(GLfloat x, GLfloat y, char *text, const GLfloat *color, void *font, bool WCS)
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glColor3fv(color);

	if (!WCS) {
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		cout << "viewport " << viewport[0] << "," << viewport[1] << "," << viewport[2] << "," << viewport[3] << "\n";
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glRasterPos2f(x, y);

		while (*text)
		{
			glutBitmapCharacter(font, *text++);
		}
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	else {
		glRasterPos2f(x, y);
		while (*text)
		{
			glutBitmapCharacter(font, *text++);
		}
	}

	glPopAttrib();
}


void textoStroke(GLfloat x, GLfloat y, GLfloat z, char * text, GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ, const GLfloat *color, void *font) {
	
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3fv(color);
	//reduccion de la escala teniendo en cuenta los tamaños estandar de las letras
	glScalef(1 / 152.38, 1 / 152.38, 1 / 152.38);
	//reduccion de escala para que quede un texto visible en el viewport orthogolnal standar de -1 a 1
	glScalef(scaleX, scaleY, scaleZ);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)text);

	glPopMatrix();
	glPopAttrib();

}

void loadImageFile(char* nombre)
{
	// Detección del formato, lectura y conversion a BGRA
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(nombre, 0);
	FIBITMAP* imagen = FreeImage_Load(formato, nombre);
	if (imagen == NULL) cerr << "Fallo carga de imagen " << nombre << endl;
	FIBITMAP* imagen32b = FreeImage_ConvertTo32Bits(imagen);

	// Lectura de dimensiones y colores
	int w = FreeImage_GetWidth(imagen32b);
	int h = FreeImage_GetHeight(imagen32b);
	GLubyte* texeles = FreeImage_GetBits(imagen32b);

	// Carga como textura actual
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texeles);

	// Liberar recursos
	FreeImage_Unload(imagen);
	FreeImage_Unload(imagen32b);
}

void saveScreenshot(char* nombre, int ancho, int alto)
{
	int pix = ancho * alto;
	BYTE *pixels = new BYTE[3 * pix];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, ancho, alto, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, ancho, alto, ancho * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_PNG, img, nombre, 0);
	delete pixels;
}

void texturarFondo()
{
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Texel menor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Texel mayor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			//La textura se repite en abcisas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			//La textura se repite en ordenadas
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);				//Asigna solo el color de la textura al fragmento

																			//Cargar el fondo con la textura corriente
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, 0);
	glTexCoord2f(1, 0);
	glVertex3f(1, -1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(1, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, 0);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void inicializarTextura(GLuint &id, char* nombre)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	loadImageFile(nombre);
}