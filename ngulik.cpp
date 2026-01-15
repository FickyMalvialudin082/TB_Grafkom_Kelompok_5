#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

// --- Variabel Kontrol ---
bool lampuNyala = true; 
bool showAxes = true; 
bool raketGerak = false;
float bolaPosisiX = 0.4f; 
float bolaPosisiZ = 0.4f;
float bolaPosisiY = 0.4f; 
float tinggiTiang = 6.0f;
float puter = true; // <--- Variabel baru untuk tinggi tiang
float raketZ = 0.0f;

// --- Kamera ---
float cameraAngleX = 0.8f;
float cameraAngleY = 0.5f;
float cameraDistance = 25.0f;
int isDragging = 0; 
int lastX, lastY;

// --- FUNGSI SUMBU KOORDINAT ---
void drawAxes() {
    if (!showAxes) return;
    glDisable(GL_LIGHTING); 
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-50, 0, 0);
    glVertex3f(50, 0, 0);   // X
    glColor3f(0.0, 1.0, 0.0); 
    glVertex3f(0, -50, 0);
    glVertex3f(0, 50, 0);   // Y
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -50);
    glVertex3f(0, 0, 50);   // Z
    glEnd();
    glEnable(GL_LIGHTING);
}

void init() {
    glClearColor(0.1, 0.1, 0.15, 1.0); 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void setLighting() {
    if (!lampuNyala) {
        glDisable(GL_LIGHTING);
        return;
    }

    glEnable(GL_LIGHTING);

    // Aktifkan 4 lampu
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);

    // Properti cahaya (sama semua biar konsisten)
    GLfloat diffuse[]  = {0.9f, 0.9f, 0.85f, 1.0f};
    GLfloat ambient[]  = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat specular[] = {0.6f, 0.6f, 0.6f, 1.0f};

    // Posisi 4 lampu (sesuai tiang)
    GLfloat pos0[] = {  5.5f, tinggiTiang,  10.0f, 1.0f };
    GLfloat pos1[] = { -5.5f, tinggiTiang,  10.0f, 1.0f };
    GLfloat pos2[] = {  5.5f, tinggiTiang, -10.0f, 1.0f };
    GLfloat pos3[] = { -5.5f, tinggiTiang, -10.0f, 1.0f };

    // Set posisi lampu TANPA terpengaruh kamera
    glPushMatrix();
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glLightfv(GL_LIGHT3, GL_POSITION, pos3);

    glPopMatrix();

    // Terapkan properti ke semua lampu
    for (int i = 0; i < 4; i++) {
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE,  diffuse);
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT,  ambient);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, specular);

        // Atenuasi biar gak terlalu terang
        glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION,  1.0f);
        glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION,    0.05f);
        glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 0.01f);
    }
}

void idle() {
    if (raketGerak) {
        raketZ += 0.05f;   // maju
        if (raketZ > 10)  // batas
            raketZ = -10;
    }
    glutPostRedisplay();
}

// anu aa ngabahas lapangan,net,garis lapang,dingding kaca
void drawNet() {
    glColor3f(0.1, 0.1, 0.1); 
    glPushMatrix();
    glTranslatef(5.0, 0.5, 0); 
    glScalef(0.15, 1.0, 0.15); 
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.0, 0.5, 0);
    glScalef(0.15, 1.0, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0); 
    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glScalef(10.0, 0.8, 0.02);
    glutWireCube(1.0); 
    glPopMatrix();

    glColor3f(0.9, 0.9, 0.9);
    glPushMatrix();
    glTranslatef(0, 0.9, 0);
    glScalef(10.0, 0.05, 0.03);
    glutSolidCube(1.0);
    glPopMatrix();
}

// dingding kaca transparan
void drawDindingKaca(float x, float z, float width, float length, bool isRotated) {
    glColor4f(0.7, 0.9, 1.0, 0.2);
    glPushMatrix();
    glTranslatef(x, 2.5, z); 
    if (isRotated) glRotatef(90, 0, 1, 0);
    glScalef(width, 5.0, length);
    glutSolidCube(1.0);
    glPopMatrix();
}
// lantai lapangan.
void drawLantaiDanPagar() {
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glScalef(25.0, 0.1, 30.0);
    glutSolidCube(1.2);
    glPopMatrix();

    glColor3f(0.1, 0.3, 0.6);
    glPushMatrix();
    glTranslatef(0, 0.1, 0); 
    glScalef(10, 0.1, 20);
    glutSolidCube(1.0);
    glPopMatrix();

    drawDindingKaca(0, 10, 10, 0.1, false);
    drawDindingKaca(0, -10, 10, 0.1, false);
    drawDindingKaca(5, 8, 4, 0.1, true);
    drawDindingKaca(-5, 8, 4, 0.1, true);
    drawDindingKaca(5, -8, 4, 0.1, true);
    drawDindingKaca(-5, -8, 4, 0.1, true);

    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(5, 1.5, 0);
    glScalef(0.05, 3.0, 12.0);
    glutWireCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5, 1.5, 0);
    glScalef(0.05, 3.0, 12.0);
    glutWireCube(1.0);
    glPopMatrix();
}
// garis lapangan
void drawGaris(){
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    float yPos = 0.16f;
    float luasSetengahX = 5.0f;
    float luasSetengahZ = 10.0f;
    float garisServisZ = 6.95f;

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(-luasSetengahX, yPos, -luasSetengahZ); glVertex3f(-luasSetengahX, yPos, luasSetengahZ);
    glVertex3f(luasSetengahX, yPos, -luasSetengahZ); glVertex3f(luasSetengahX, yPos, luasSetengahZ);
    glVertex3f(-luasSetengahX, yPos, -luasSetengahZ); glVertex3f(luasSetengahX, yPos, -luasSetengahZ);
    glVertex3f(-luasSetengahX, yPos, luasSetengahZ); glVertex3f(luasSetengahX, yPos, luasSetengahZ);
    glVertex3f(-luasSetengahX, yPos, garisServisZ); glVertex3f(luasSetengahX, yPos, garisServisZ);
    glVertex3f(-luasSetengahX, yPos, -garisServisZ); glVertex3f(luasSetengahX, yPos, -garisServisZ);
    glVertex3f(0.0f, yPos, -garisServisZ); glVertex3f(0.0f, yPos, garisServisZ);
    glEnd();
    glEnable(GL_LIGHTING);
}
// nopal
void drawTribunKanan() {
    float xPos = 8.0f; 
    float yStart = 0.1f;
    float zSize = 18.0f;
    for (int i = 0; i < 5; i++) {

        if (lampuNyala)
            glColor3f(0.4f, 0.4f, 0.4f);
        else
            glColor3f(0.08f,0.08f, 0.08f);
            
        glPushMatrix();
        glTranslatef(xPos + (i * 1.2), yStart + (i * 0.6), 0);
        glScalef(2.0 , 0.6, zSize);
        glutSolidCube(1.0);
        glPopMatrix();
        if (lampuNyala)
            glColor3f(0.8f, 0.2f, 0.2f);
        else
            glColor3f(0.1f, 0.02f, 0.02f);

        for (float z = -zSize/2 + 1; z < zSize/2; z += 2.0) {
            glPushMatrix();
            glTranslatef(xPos + (i * 1.2), yStart + 0.3 + (i * 0.6), z);
            glScalef(0.8, 0.5, 0.8);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }
}
void drawTribunKiri() {
    float xPos = -8.0f;   // ← sisi kiri lapangan
    float yStart = 0.1f;
    float zSize = 18.0f;

    for (int i = 0; i < 5; i++) {

        // Warna undakan
        if (lampuNyala)
            glColor3f(0.4f, 0.4f, 0.4f);
        else
            glColor3f(0.08f, 0.08f, 0.08f);

        // Undakan tribun
        glPushMatrix();
        glTranslatef(xPos - (i * 1.2), yStart + (i * 0.6), 0);
        glScalef(2.0, 0.6, zSize);
        glutSolidCube(1.0);
        glPopMatrix();

        // Warna kursi
        if (lampuNyala)
            glColor3f(0.8f, 0.2f, 0.2f);
        else
            glColor3f(0.1f, 0.02f, 0.02f);

        // Kursi tribun
        for (float z = -zSize / 2 + 1; z < zSize / 2; z += 2.0) {
            glPushMatrix();
            glTranslatef(
                xPos - (i * 1.2),
                yStart + 0.3 + (i * 0.6),
                z
            );
            glScalef(0.8, 0.5, 0.8);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }
}

// rio
void drawRaketHitam(float x, float z) {
    glPushMatrix();
    glTranslatef(0, 2, z); // Letakkan sedikit di atas lantai
    glRotatef(puter, 0, 2, 0); // Tidurkan raket di lantai
    glRotatef(0, 0, 0, 5);  // Sedikit miring agar artistik

    // Kepala Raket (Hitam)
    glColor3f(0.02, 0.02, 0.02); 
    glPushMatrix();
    glScalef(1.0, 1.2, 0.15); 
    glutSolidSphere(0.7, 30, 30);
    glPopMatrix();

    // Pegangan / Handle (Hitam)
    glPushMatrix();
    glTranslatef(0, -1.2, 0);
    glScalef(0.15, 1.0, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix();
}
//yakvi
void drawTiangLampu(float x, float z) {
    glColor3f(0.1, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(x, 0, z); 
    
    // Batang Tiang (Skala Y menggunakan variabel tinggiTiang)
    glPushMatrix();
    glScalef(0.2, tinggiTiang, 0.2); 
    glTranslatef(0, 0.5, 0); 
    glutSolidCube(1.0);
    glPopMatrix();

    // Kepala Lampu (Translasi Y mengikuti tinggiTiang)
    glTranslatef(0, tinggiTiang, 0); 
    glRotatef(x > 0 ? -40 : 40, 0, 0, 1);
    if (lampuNyala) glColor3f(1.0, 1.0, 0.5); else glColor3f(0.2, 0.2, 0.2);
    glScalef(1.0, 0.2, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
    float eyeX = cameraDistance * sin(cameraAngleX) * cos(cameraAngleY);
    float eyeY = cameraDistance * sin(cameraAngleY);
    float eyeZ = cameraDistance * cos(cameraAngleX) * cos(cameraAngleY);
	setLighting();
    gluLookAt(eyeX, eyeY, eyeZ, 0, 0, 0, 0, 1, 0); 
// yakvi
    drawAxes(); 
    setLighting();
    drawLantaiDanPagar();
    drawGaris();
    drawNet(); 
    drawTribunKanan();
    drawTribunKiri();
    drawRaketHitam(6.0, 2.0);
    // Lampu di empat pojok lapangan
    drawTiangLampu(5.5, 10.0);
    drawTiangLampu(-5.5, 10.0);
    drawTiangLampu(5.5, -10.0);
    drawTiangLampu(-5.5, -10.0);
// rio
    // Render Bola
    glColor3f(0.8, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(bolaPosisiX, bolaPosisiY, bolaPosisiZ); 
    glutSolidSphere(0.18, 15, 15);
    glPopMatrix();

    glutSwapBuffers();
}
// noupal
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
        case 'X':
        showAxes = !showAxes;
        break;
        case 'l':
        case 'L': 
        lampuNyala = !lampuNyala;
        break;
        case 'w':
        case 'W': 
        bolaPosisiZ -= 0.4f; 
        break; 
        case 's':
        case 'S': 
        bolaPosisiZ += 0.4f;
        break;
        case 'a':
        case 'A': 
        bolaPosisiX -= 0.4f; 
        break; 
        case 'd': 
        case 'D': 
        bolaPosisiX += 0.4f; 
        break;
        case '+':
        bolaPosisiY += 0.4f;
        break; 
        case '-':
        bolaPosisiY -= 0.4f;
        break;
        case 'r':
        case 'R': 
        tinggiTiang += 0.2f; 
        break; 
        case 'f': 
        case 'F': 
        if (tinggiTiang > 0.5f) tinggiTiang -= 0.2f;
        break;
		case 'm':
        case 'M': 
        puter += 1.0f; 
        raketGerak = !raketGerak;
        break;

        case 27:
        exit(0);
        break;
    }
    glutPostRedisplay();
}
// anu aa
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) { isDragging = 1; lastX = x; lastY = y; }
        else isDragging = 0;
    }
    if (button == 3) cameraDistance -= 1.0f; 
    if (button == 4) cameraDistance += 1.0f;
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (isDragging) {
        cameraAngleX += (x - lastX) * 0.005f;
        cameraAngleY += (y - lastY) * 0.005f;
        if (cameraAngleY > 1.4f) cameraAngleY = 1.4f;
        if (cameraAngleY < 0.1f) cameraAngleY = 0.1f;
        lastX = x; lastY = y;
        glutPostRedisplay();
    }
}
// yakvi
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Lapangan padel");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}