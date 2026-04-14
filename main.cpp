#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

// ===== Camera =====
float camX = 0.0f, camY = 2.0f, camZ = 8.0f;
float camAngleX = -10.0f, camAngleY = 0.0f;

// ===== Light animation =====
bool lightOn = true;

// ===== Fan animation =====
float fanAngle = 0.0f;
float fanSpeed  = 3.0f;
bool  fanOn     = true;

// ===== AC =====
bool  acOn   = true;
float acFlow = 0.0f;

// ===== Helpers =====
void setColor(float r, float g, float b) { glColor3f(r, g, b); }

void drawQuad(float x1,float y1,float z1,
              float x2,float y2,float z2,
              float x3,float y3,float z3,
              float x4,float y4,float z4) {
    glBegin(GL_QUADS);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
        glVertex3f(x3,y3,z3);
        glVertex3f(x4,y4,z4);
    glEnd();
}

// ===== Custom Cylinder =====
void drawCylinder(float radius, float height, int slices) {
    float step = 2.0f * 3.14159f / slices;
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= slices; i++){
        float a = i * step;
        float x = cos(a)*radius, z = sin(a)*radius;
        glNormal3f(x/radius, 0, z/radius);
        glVertex3f(x, 0, z);
        glVertex3f(x, height, z);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,-1,0);
    glVertex3f(0,0,0);
    for(int i=0;i<=slices;i++){float a=i*step;glVertex3f(cos(a)*radius,0,sin(a)*radius);}
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,1,0);
    glVertex3f(0,height,0);
    for(int i=slices;i>=0;i--){float a=i*step;glVertex3f(cos(a)*radius,height,sin(a)*radius);}
    glEnd();
}

// ===== Custom Cone =====
void drawCone(float baseRadius, float height, int slices) {
    float step = 2.0f * 3.14159f / slices;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, height, 0);
    for(int i=0;i<=slices;i++){float a=i*step;glVertex3f(cos(a)*baseRadius,0,sin(a)*baseRadius);}
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,-1,0);
    glVertex3f(0,0,0);
    for(int i=slices;i>=0;i--){float a=i*step;glVertex3f(cos(a)*baseRadius,0,sin(a)*baseRadius);}
    glEnd();
}

// ===== Room Structure =====
void drawRoom() {
    // Floor (white + black mix marble type)
    setColor(0.88f, 0.88f, 0.86f);
    drawQuad(-6,-0.01f,-6,  6,-0.01f,-6,  6,-0.01f,6,  -6,-0.01f,6);

    // Floor tile lines (blackish gray)
    setColor(0.20f, 0.20f, 0.20f);
    glLineWidth(1.2f);
    glBegin(GL_LINES);
    for(int i=-6;i<=6;i+=2) {
        glVertex3f(i,0,-6); glVertex3f(i,0,6);
        glVertex3f(-6,0,i); glVertex3f(6,0,i);
    }
    glEnd();

    // Ceiling
    setColor(0.95f, 0.93f, 0.88f);
    drawQuad(-6,5,-6,  6,5,-6,  6,5,6,  -6,5,6);

    // Back wall
    setColor(0.78f, 0.82f, 0.88f);
    drawQuad(-6,0,-6,  6,0,-6,  6,5,-6,  -6,5,-6);

    // Left wall
    setColor(0.85f, 0.80f, 0.75f);
    drawQuad(-6,0,-6,  -6,0,6,  -6,5,6,  -6,5,-6);

    // Right wall
    setColor(0.85f, 0.80f, 0.75f);
    drawQuad(6,0,-6,  6,0,6,  6,5,6,  6,5,-6);

    // Front wall (with opening)
    setColor(0.78f, 0.82f, 0.88f);
    drawQuad(-6,0,6,  -2,0,6,  -2,5,6,  -6,5,6);
    drawQuad(2,0,6,   6,0,6,   6,5,6,   2,5,6);
    drawQuad(-6,3,6,  6,3,6,   6,5,6,  -6,5,6);
}

// ===== Double Window on back wall =====
void drawDoubleWindow() {
    float wx[] = {-4.5f, 0.5f};
    for(int w = 0; w < 2; w++) {
        float ox = wx[w];

        setColor(0.30f, 0.18f, 0.08f);
        drawQuad(ox-0.15f, 2.75f,-5.99f,
                 ox+2.15f, 2.75f,-5.99f,
                 ox+2.15f, 4.35f,-5.99f,
                 ox-0.15f, 4.35f,-5.99f);

        setColor(0.68f, 0.85f, 0.97f);
        drawQuad(ox,       3.0f, -5.985f,
                 ox+2.0f,  3.0f, -5.985f,
                 ox+2.0f,  4.2f, -5.985f,
                 ox,       4.2f, -5.985f);

        setColor(0.30f, 0.18f, 0.08f);
        drawQuad(ox+0.97f, 3.0f,-5.975f,
                 ox+1.03f, 3.0f,-5.975f,
                 ox+1.03f, 4.2f,-5.975f,
                 ox+0.97f, 4.2f,-5.975f);

        drawQuad(ox,      3.57f,-5.975f,
                 ox+2.0f, 3.57f,-5.975f,
                 ox+2.0f, 3.63f,-5.975f,
                 ox,      3.63f,-5.975f);

        setColor(0.42f, 0.26f, 0.10f);
        drawQuad(ox-0.15f, 2.73f,-5.99f,
                 ox+2.15f, 2.73f,-5.99f,
                 ox+2.15f, 2.75f,-5.99f,
                 ox-0.15f, 2.75f,-5.99f);
    }
}

// ===== Door on left wall =====
void drawDoor() {
    setColor(0.35f, 0.22f, 0.10f);
    drawQuad(-5.99f,0,1.2f, -5.99f,0,3.2f, -5.99f,3.8f,3.2f, -5.99f,3.8f,1.2f);
    setColor(0.55f, 0.38f, 0.18f);
    drawQuad(-5.98f,0.05f,1.3f, -5.98f,0.05f,3.1f, -5.98f,3.65f,3.1f, -5.98f,3.65f,1.3f);
    setColor(0.48f, 0.32f, 0.14f);
    drawQuad(-5.97f,0.3f,1.5f, -5.97f,0.3f,2.9f, -5.97f,1.7f,2.9f, -5.97f,1.7f,1.5f);
    drawQuad(-5.97f,2.0f,1.5f, -5.97f,2.0f,2.9f, -5.97f,3.3f,2.9f, -5.97f,3.3f,1.5f);
    setColor(0.85f, 0.72f, 0.10f);
    glPushMatrix();
    glTranslatef(-5.96f, 1.8f, 3.0f);
    glutSolidSphere(0.08, 10, 10);
    glPopMatrix();
}

// ===== Bed =====
void drawBed() {
    glPushMatrix();
    glTranslatef(-3.5f, 0, -3.5f);

    setColor(0.40f, 0.25f, 0.10f);
    glPushMatrix(); glScalef(2.6f, 0.35f, 4.5f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.92f, 0.88f, 0.82f);
    glPushMatrix(); glTranslatef(0, 0.30f, 0); glScalef(2.4f, 0.22f, 4.2f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.98f, 0.95f, 0.92f);
    glPushMatrix(); glTranslatef(0, 0.50f, -1.6f); glScalef(2.0f, 0.18f, 0.7f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.60f, 0.20f, 0.25f);
    glPushMatrix(); glTranslatef(0, 0.48f, 0.4f); glScalef(2.4f, 0.14f, 3.0f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.35f, 0.20f, 0.08f);
    glPushMatrix(); glTranslatef(0, 0.9f, -2.35f); glScalef(2.6f, 1.5f, 0.15f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.28f, 0.16f, 0.06f);
    float lx[] = {-1.1f, 1.1f, -1.1f, 1.1f};
    float lz[] = {-2.1f, -2.1f, 2.1f, 2.1f};
    for(int i=0;i<4;i++){
        glPushMatrix(); glTranslatef(lx[i], -0.22f, lz[i]); glScalef(0.15f, 0.45f, 0.15f); glutSolidCube(1.0); glPopMatrix();
    }
    glPopMatrix();
}

// ===== Study Table =====
void drawTable() {
    glPushMatrix();
    glTranslatef(3.5f, 0, -3.5f);

    setColor(0.60f, 0.38f, 0.15f);
    glPushMatrix(); glTranslatef(0, 1.55f, 0); glScalef(2.5f, 0.1f, 1.4f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.45f, 0.28f, 0.10f);
    float lx[] = {-1.1f, 1.1f, -1.1f, 1.1f};
    float lz[] = {-0.6f, -0.6f, 0.6f, 0.6f};
    for(int i=0;i<4;i++){
        glPushMatrix(); glTranslatef(lx[i], 0.77f, lz[i]); glScalef(0.1f, 1.54f, 0.1f); glutSolidCube(1.0); glPopMatrix();
    }

    setColor(0.20f, 0.20f, 0.22f);
    glPushMatrix(); glTranslatef(0.3f, 1.62f, 0.0f); glScalef(0.9f, 0.05f, 0.6f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.15f, 0.15f, 0.18f);
    glPushMatrix(); glTranslatef(0.3f, 1.95f, -0.28f); glRotatef(-25, 1,0,0); glScalef(0.9f, 0.55f, 0.03f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.30f, 0.55f, 0.90f);
    glPushMatrix(); glTranslatef(0.3f, 1.95f, -0.26f); glRotatef(-25, 1,0,0); glScalef(0.82f, 0.48f, 0.01f); glutSolidCube(1.0); glPopMatrix();

    float bColors[3][3] = {{0.8f,0.2f,0.2f},{0.2f,0.6f,0.3f},{0.2f,0.3f,0.8f}};
    for(int i=0;i<3;i++){
        setColor(bColors[i][0],bColors[i][1],bColors[i][2]);
        glPushMatrix(); glTranslatef(-0.6f, 1.63f+i*0.07f, -0.1f); glScalef(0.45f, 0.06f, 0.35f); glutSolidCube(1.0); glPopMatrix();
    }
    glPopMatrix();
}

void drawChair() {
    glPushMatrix();
    glTranslatef(3.5f, 0, -2.0f);

    // 👉 FIX: 180 degree rotation
    glRotatef(180.0f, 0, 1, 0);

    setColor(0.50f, 0.30f, 0.12f);

    glPushMatrix();
        glTranslatef(0, 1.0f, 0);
        glScalef(0.9f, 0.08f, 0.9f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 1.55f, -0.42f);
        glScalef(0.9f, 0.9f, 0.08f);
        glutSolidCube(1.0);
    glPopMatrix();

    setColor(0.38f, 0.22f, 0.08f);
    float lx[] = {-0.38f, 0.38f, -0.38f, 0.38f};
    float lz[] = {-0.38f, -0.38f, 0.38f, 0.38f};

    for(int i = 0; i < 4; i++) {
        glPushMatrix();
            glTranslatef(lx[i], 0.5f, lz[i]);
            glScalef(0.07f, 1.0f, 0.07f);
            glutSolidCube(1.0);
        glPopMatrix();
    }

    glPopMatrix();
}

// ===== Improved Bookshelf =====
void drawBookshelf() {
    glPushMatrix();
    glTranslatef(-5.6f, 0, -1.5f);
    glRotatef(90, 0,1,0);

    setColor(0.35f, 0.22f, 0.09f);
    glPushMatrix(); glScalef(2.4f, 4.8f, 0.55f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.28f, 0.16f, 0.06f);
    glPushMatrix(); glTranslatef(0,0,0.24f); glScalef(2.2f, 4.7f, 0.04f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.50f, 0.32f, 0.13f);
    float shelfY[] = {-2.0f, -0.9f, 0.2f, 1.3f, 2.2f};
    for(int s=0;s<5;s++){
        glPushMatrix();
        glTranslatef(0, shelfY[s], 0.05f);
        glScalef(2.28f, 0.08f, 0.50f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    struct BookColor { float r,g,b; };
    BookColor palette[] = {
        {0.80f,0.10f,0.10f},{0.10f,0.50f,0.85f},{0.10f,0.72f,0.22f},
        {0.90f,0.60f,0.10f},{0.65f,0.10f,0.72f},{0.85f,0.40f,0.20f},
        {0.20f,0.55f,0.60f},{0.75f,0.75f,0.10f},{0.50f,0.20f,0.60f},
        {0.30f,0.65f,0.50f},{0.90f,0.30f,0.45f},{0.15f,0.35f,0.75f}
    };

    for(int s=0;s<5;s++){
        float baseY = shelfY[s] + 0.06f;
        float x = -1.05f;
        int bookIdx = (s*7)%12;
        if(s % 2 == 0) {
            float widths[]  = {0.14f,0.18f,0.13f,0.16f,0.15f,0.19f,0.12f};
            float heights[] = {0.55f,0.45f,0.60f,0.40f,0.50f,0.38f,0.52f};
            for(int b=0;b<7;b++){
                BookColor &c = palette[(bookIdx+b)%12];
                setColor(c.r,c.g,c.b);
                glPushMatrix();
                glTranslatef(x + widths[b]*0.5f, baseY + heights[b]*0.5f, 0.05f);
                glScalef(widths[b], heights[b], 0.32f);
                glutSolidCube(1.0);
                glPopMatrix();
                setColor(c.r*0.7f, c.g*0.7f, c.b*0.7f);
                glPushMatrix();
                glTranslatef(x + widths[b]*0.5f, baseY + heights[b]*0.5f, 0.215f);
                glScalef(widths[b]*0.9f, heights[b]*0.95f, 0.01f);
                glutSolidCube(1.0);
                glPopMatrix();
                x += widths[b] + 0.02f;
            }
        } else {
            float widths[] = {0.16f,0.14f,0.18f,0.15f};
            float heights[] = {0.50f,0.55f,0.42f,0.48f};
            for(int b=0;b<4;b++){
                BookColor &c = palette[(bookIdx+b)%12];
                setColor(c.r,c.g,c.b);
                glPushMatrix();
                glTranslatef(x + widths[b]*0.5f, baseY + heights[b]*0.5f, 0.05f);
                glScalef(widths[b], heights[b], 0.32f);
                glutSolidCube(1.0);
                glPopMatrix();
                x += widths[b] + 0.02f;
            }
            float stackX = x + 0.22f;
            for(int h=0;h<3;h++){
                BookColor &c = palette[(bookIdx+4+h)%12];
                setColor(c.r,c.g,c.b);
                glPushMatrix();
                glTranslatef(stackX, baseY + 0.05f + h*0.10f, 0.05f);
                glScalef(0.42f, 0.09f, 0.30f);
                glutSolidCube(1.0);
                glPopMatrix();
            }
        }
    }

    setColor(0.70f, 0.45f, 0.20f);
    glPushMatrix();
    glTranslatef(0.85f, 2.28f, 0.05f);
    drawCylinder(0.07f, 0.22f, 12);
    glPopMatrix();
    setColor(0.20f, 0.65f, 0.25f);
    glPushMatrix();
    glTranslatef(0.85f, 2.50f, 0.05f);
    glutSolidSphere(0.09f, 8, 8);
    glPopMatrix();

    glPopMatrix();
}

// ===== NEW: Book Rack beside study table =====
void drawBookRack() {
    glPushMatrix();
    // Place beside study table (table at x=3.5, z=-3.5), rack against right wall
    glTranslatef(5.30f, 0.0f, -3.50f);

    // === Frame: two vertical side panels ===
    setColor(0.38f, 0.24f, 0.09f);
    // Left side panel
    glPushMatrix(); glTranslatef(-0.28f, 1.70f, 0); glScalef(0.06f, 3.40f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    // Right side panel
    glPushMatrix(); glTranslatef( 0.28f, 1.70f, 0); glScalef(0.06f, 3.40f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    // Top panel
    glPushMatrix(); glTranslatef(0, 3.38f, 0); glScalef(0.62f, 0.06f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    // Bottom base
    glPushMatrix(); glTranslatef(0, 0.04f, 0); glScalef(0.62f, 0.08f, 0.52f); glutSolidCube(1.0); glPopMatrix();
    // Back panel
    setColor(0.30f, 0.18f, 0.07f);
    glPushMatrix(); glTranslatef(0, 1.70f, -0.23f); glScalef(0.56f, 3.30f, 0.04f); glutSolidCube(1.0); glPopMatrix();

    // === 4 Shelves ===
    setColor(0.48f, 0.30f, 0.11f);
    float shelfY[] = { 0.72f, 1.38f, 2.04f, 2.70f };
    for(int s = 0; s < 4; s++){
        glPushMatrix();
        glTranslatef(0, shelfY[s], 0);
        glScalef(0.56f, 0.05f, 0.48f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // === Books per shelf ===
    struct BColor { float r,g,b; };
    BColor bp[] = {
        {0.80f,0.15f,0.15f},{0.15f,0.55f,0.80f},{0.15f,0.72f,0.25f},
        {0.88f,0.58f,0.12f},{0.65f,0.15f,0.70f},{0.85f,0.38f,0.22f},
        {0.20f,0.58f,0.62f},{0.75f,0.72f,0.12f},{0.50f,0.22f,0.62f},
        {0.28f,0.65f,0.50f},{0.88f,0.28f,0.42f},{0.18f,0.38f,0.78f}
    };

    float bookW[] = {0.10f, 0.12f, 0.09f, 0.11f, 0.10f};
    float bookH[] = {0.48f, 0.42f, 0.52f, 0.38f, 0.45f};

    for(int s = 0; s < 4; s++){
        float baseY = shelfY[s] + 0.05f;
        float x = -0.24f;
        for(int b = 0; b < 4; b++){
            BColor &c = bp[(s * 4 + b) % 12];
            float w = bookW[b % 5];
            float h = bookH[b % 5];
            setColor(c.r, c.g, c.b);
            glPushMatrix();
            glTranslatef(x + w * 0.5f, baseY + h * 0.5f, 0.0f);
            glScalef(w, h, 0.34f);
            glutSolidCube(1.0);
            glPopMatrix();
            // Spine detail (darker)
            setColor(c.r * 0.70f, c.g * 0.70f, c.b * 0.70f);
            glPushMatrix();
            glTranslatef(x + w * 0.5f, baseY + h * 0.5f, 0.175f);
            glScalef(w * 0.88f, h * 0.92f, 0.01f);
            glutSolidCube(1.0);
            glPopMatrix();
            x += w + 0.015f;
        }
    }

    // === Small decorative plant on top ===
    setColor(0.55f, 0.35f, 0.12f);
    glPushMatrix(); glTranslatef(0.10f, 3.45f, 0.05f); drawCylinder(0.055f, 0.12f, 10); glPopMatrix();
    setColor(0.18f, 0.62f, 0.22f);
    glPushMatrix(); glTranslatef(0.10f, 3.60f, 0.05f); glutSolidSphere(0.085f, 8, 8); glPopMatrix();

    glPopMatrix();
}

// ===== Carpet =====
void drawCarpet() {
    setColor(0.55f, 0.22f, 0.18f);
    glBegin(GL_QUADS);
        glVertex3f(-1.5f, 0.01f, -1.0f);
        glVertex3f( 1.5f, 0.01f, -1.0f);
        glVertex3f( 1.5f, 0.01f,  2.5f);
        glVertex3f(-1.5f, 0.01f,  2.5f);
    glEnd();
    setColor(0.80f, 0.70f, 0.35f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-1.4f, 0.02f, -0.9f);
        glVertex3f( 1.4f, 0.02f, -0.9f);
        glVertex3f( 1.4f, 0.02f,  2.4f);
        glVertex3f(-1.4f, 0.02f,  2.4f);
    glEnd();
    glLineWidth(1.0f);
}

// ===== Small Side Table =====
void drawSideTable() {
    glPushMatrix();
    glTranslatef(-1.5f, 0, -4.5f);

    setColor(0.50f, 0.32f, 0.14f);
    glPushMatrix(); glTranslatef(0, 0.8f, 0); glScalef(0.7f, 0.07f, 0.7f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.38f, 0.22f, 0.08f);
    float lx[] = {-0.3f, 0.3f, -0.3f, 0.3f};
    float lz[] = {-0.3f, -0.3f, 0.3f, 0.3f};
    for(int i=0;i<4;i++){
        glPushMatrix(); glTranslatef(lx[i], 0.4f, lz[i]); glScalef(0.06f, 0.8f, 0.06f); glutSolidCube(1.0); glPopMatrix();
    }

    // Lamp
    setColor(0.82f, 0.72f, 0.18f);
    glPushMatrix(); glTranslatef(0, 0.87f, 0); glScalef(0.08f, 0.45f, 0.08f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.95f, 0.90f, 0.65f);
    glPushMatrix(); glTranslatef(0, 1.22f, 0); drawCone(0.22f, 0.30f, 12); glPopMatrix();

    glPopMatrix();
}

// ===== Ceiling Light =====
void drawCeilingLight() {
    setColor(0.75f, 0.75f, 0.75f);
    glPushMatrix(); glTranslatef(0, 4.85f, -1.0f); glScalef(0.5f, 0.12f, 0.5f); glutSolidCube(1.0); glPopMatrix();

    if(lightOn) setColor(1.0f, 0.98f, 0.80f);
    else        setColor(0.4f, 0.4f, 0.4f);
    glPushMatrix(); glTranslatef(0, 4.72f, -1.0f); glutSolidSphere(0.15f, 12, 12); glPopMatrix();
}

// ===== CEILING FAN =====
void drawFan() {
    glPushMatrix();
    glTranslatef(0.0f, 4.85f, 0.0f);

    // Down-rod
    setColor(0.55f, 0.55f, 0.58f);
    glPushMatrix();
    glTranslatef(0, -0.05f, 0);
    drawCylinder(0.04f, 0.35f, 12);
    glPopMatrix();

    // Motor housing
    setColor(0.60f, 0.60f, 0.62f);
    glPushMatrix();
    glTranslatef(0, -0.45f, 0);
    drawCylinder(0.18f, 0.22f, 20);
    glPopMatrix();

    // Light globe below motor
    setColor(0.95f, 0.92f, 0.75f);
    glPushMatrix();
    glTranslatef(0, -0.58f, 0);
    glutSolidSphere(0.12f, 12, 12);
    glPopMatrix();

    // Rotating blades
    glPushMatrix();
    glTranslatef(0, -0.36f, 0);
    glRotatef(fanAngle, 0, 1, 0);

    int numBlades = 4;
    for(int b = 0; b < numBlades; b++) {
        glPushMatrix();
        glRotatef(b * 90.0f, 0, 1, 0);

        setColor(0.50f, 0.32f, 0.10f);
        glPushMatrix();
        glTranslatef(0.18f, 0, 0);
        glRotatef(90, 0, 1, 0);
        drawCylinder(0.03f, 0.25f, 8);
        glPopMatrix();

        setColor(0.65f, 0.42f, 0.18f);
        glPushMatrix();
        glTranslatef(0.72f, 0.01f, 0);
        glRotatef(-8, 0, 0, 1);
        glScalef(0.90f, 0.04f, 0.28f);
        glutSolidCube(1.0);
        glPopMatrix();

        setColor(0.52f, 0.33f, 0.12f);
        glPushMatrix();
        glTranslatef(0.72f, -0.01f, 0);
        glRotatef(-8, 0, 0, 1);
        glScalef(0.90f, 0.04f, 0.28f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }
    glPopMatrix();

    glPopMatrix();
}

// ===== UPDATED: Wall-Mounted AC above windows on back wall, with ON/OFF =====
void drawAC() {
    glPushMatrix();
    // Mount on back wall above the double windows, centered
    glTranslatef(-1.0f, 4.65f, -5.95f);

    // Main body
    setColor(0.90f, 0.90f, 0.92f);
    glPushMatrix();
    glScalef(3.20f, 0.48f, 0.30f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Front face panel
    setColor(0.95f, 0.95f, 0.97f);
    glPushMatrix();
    glTranslatef(0, 0, 0.155f);
    glScalef(3.10f, 0.43f, 0.01f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Vent slits (horizontal)
    setColor(0.70f, 0.70f, 0.74f);
    for(int s = 0; s < 5; s++){
        glPushMatrix();
        glTranslatef(0, -0.13f + s*0.065f, 0.16f);
        glScalef(2.90f, 0.014f, 0.04f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Bottom angled vane — opens wider when ON
    setColor(0.85f, 0.85f, 0.88f);
    glPushMatrix();
    glTranslatef(0, -0.23f, 0.15f);
    glRotatef(acOn ? 30.0f : 5.0f, 1, 0, 0);
    glScalef(2.95f, 0.03f, 0.20f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Display dot
    setColor(0.15f, 0.55f, 0.85f);
    glPushMatrix();
    glTranslatef(1.35f, 0.10f, 0.16f);
    glutSolidSphere(0.028f, 8, 8);
    glPopMatrix();

    // Power LED: green=ON, red=OFF
    if(acOn) setColor(0.10f, 0.90f, 0.30f);
    else     setColor(0.90f, 0.10f, 0.10f);
    glPushMatrix();
    glTranslatef(1.20f, 0.10f, 0.16f);
    glutSolidSphere(0.020f, 6, 6);
    glPopMatrix();

    // Airflow lines — ONLY when acOn == true
    if(acOn) {
        glDisable(GL_LIGHTING);
        setColor(0.75f, 0.88f, 0.98f);
        glLineWidth(1.5f);
        int numStreams = 8;
        for(int i = 0; i < numStreams; i++){
            float offset = fmod(acFlow + i * 0.125f, 1.0f);
            float len    = 0.35f;
            float startZ = 0.20f + offset * 1.0f;
            float endZ   = startZ + len;
            float y      = -0.38f - offset * 0.25f;
            float x      = -1.30f + i * 0.38f;
            if(endZ > 1.40f) endZ = 1.40f;
            glBegin(GL_LINES);
                glVertex3f(x, y, startZ);
                glVertex3f(x, y, endZ);
            glEnd();
        }
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

// ===== UPDATED: Sofa on carpet, facing RIGHT wall =====
void drawSofa() {
    glPushMatrix();


    // Move sofa near RIGHT WALL (+X side)
    glTranslatef(5.2f, 0.0f, 0.0f);   // <-- change position

    // Rotate sofa to face RIGHT WALL
    glRotatef(-90.0f, 0, 1, 0);

    // Base frame
    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(0, 0.15f, 0); glScalef(2.80f, 0.28f, 0.95f); glutSolidCube(1.0); glPopMatrix();

    // Seat cushions (3 sections)
    float cc[3][3] = {
        {0.42f, 0.52f, 0.62f},
        {0.40f, 0.50f, 0.60f},
        {0.42f, 0.52f, 0.62f}
    };
    for(int c = 0; c < 3; c++){
        setColor(cc[c][0], cc[c][1], cc[c][2]);
        glPushMatrix();
        glTranslatef(-0.86f + c * 0.86f, 0.44f, 0.04f);
        glScalef(0.80f, 0.20f, 0.80f);
        glutSolidCube(1.0);
        glPopMatrix();
        // Cushion crease
        setColor(0.32f, 0.40f, 0.50f);
        glPushMatrix();
        glTranslatef(-0.86f + c * 0.86f, 0.54f, 0.04f);
        glScalef(0.78f, 0.01f, 0.78f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Backrest
    setColor(0.38f, 0.48f, 0.58f);
    glPushMatrix(); glTranslatef(0, 0.82f, -0.40f); glScalef(2.65f, 0.68f, 0.20f); glutSolidCube(1.0); glPopMatrix();

    // Backrest top rail
    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(0, 1.18f, -0.40f); glScalef(2.76f, 0.10f, 0.22f); glutSolidCube(1.0); glPopMatrix();

    // Left armrest
    setColor(0.35f, 0.45f, 0.55f);
    glPushMatrix(); glTranslatef(-1.28f, 0.62f, 0.0f); glScalef(0.22f, 0.56f, 0.88f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(-1.28f, 0.94f, 0.0f); glScalef(0.24f, 0.09f, 0.92f); glutSolidCube(1.0); glPopMatrix();

    // Right armrest
    setColor(0.35f, 0.45f, 0.55f);
    glPushMatrix(); glTranslatef(1.28f, 0.62f, 0.0f); glScalef(0.22f, 0.56f, 0.88f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(1.28f, 0.94f, 0.0f); glScalef(0.24f, 0.09f, 0.92f); glutSolidCube(1.0); glPopMatrix();

    // Legs (4 cylindrical)
    setColor(0.20f, 0.15f, 0.08f);
    float lx[] = {-1.20f, 1.20f, -1.20f, 1.20f};
    float lz[] = {-0.40f, -0.40f, 0.40f, 0.40f};
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslatef(lx[i], 0.06f, lz[i]);
        drawCylinder(0.05f, 0.12f, 8);
        glPopMatrix();
    }

    // Throw pillow left
    setColor(0.80f, 0.55f, 0.30f);
    glPushMatrix();
    glTranslatef(-0.75f, 0.70f, -0.18f);
    glRotatef(14, 0, 0, 1);
    glScalef(0.28f, 0.28f, 0.11f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Throw pillow right
    setColor(0.60f, 0.28f, 0.35f);
    glPushMatrix();
    glTranslatef(0.72f, 0.70f, -0.18f);
    glRotatef(-11, 0, 0, 1);
    glScalef(0.26f, 0.26f, 0.11f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

// ===== Digital Wall Clock (RIGHT WALL FIXED) =====
void drawDigitalClock() {
    glPushMatrix();

    // 1. Right wall position (+X wall)
    glTranslatef(4.8f, 3.5f, 0.0f);

    // 2. Face inside room
    // Keeping your rotation values for the right wall
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    // 3. Digital Clock Body (Dark Gray Outer Frame)
    setColor(0.15f, 0.15f, 0.15f); // Dark gray color
    glBegin(GL_QUADS);
        // Drawing a rectangle (Width: 0.8, Height: 0.3)
        glVertex3f(-0.40f, -0.15f, 0.0f); // Bottom-left
        glVertex3f( 0.40f, -0.15f, 0.0f); // Bottom-right
        glVertex3f( 0.40f,  0.15f, 0.0f); // Top-right
        glVertex3f(-0.40f,  0.15f, 0.0f); // Top-left
    glEnd();

    // 4. Digital Screen (Black / Dark Green Background)
    glTranslatef(0.0f, 0.0f, 0.01f);  // Bring the screen slightly forward
    setColor(0.02f, 0.05f, 0.02f);    // Very dark green (almost black)
    glBegin(GL_QUADS);
        // Slightly smaller rectangle for the inner screen
        glVertex3f(-0.35f, -0.10f, 0.0f);
        glVertex3f( 0.35f, -0.10f, 0.0f);
        glVertex3f( 0.35f,  0.10f, 0.0f);
        glVertex3f(-0.35f,  0.10f, 0.0f);
    glEnd();

    // 5. Digital Time Text (e.g., "10:30")
    glTranslatef(-0.25f, -0.05f, 0.01f); // Move forward again and align text to the left
    setColor(0.0f, 1.0f, 0.2f);          // Bright Neon Green text color

    // GLUT stroke fonts are very large by default, so we must scale them down heavily
    glScalef(0.0012f, 0.0012f, 0.001f);
    glLineWidth(2.5f); // Make the text slightly bold

    // The time string to display
    char timeStr[] = "10:30";

    // Loop through the characters and draw them
    for (int i = 0; timeStr[i] != '\0'; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, timeStr[i]);
    }

    // Reset line width for other objects in your room
    glLineWidth(1.0f);

    glPopMatrix();
}

// ===== Picture Frame =====
void drawPicture() {
    setColor(0.45f, 0.28f, 0.10f);
    drawQuad(3.5f,2.2f,-5.99f,  5.2f,2.2f,-5.99f,  5.2f,3.8f,-5.99f,  3.5f,3.8f,-5.99f);
    setColor(0.95f, 0.55f, 0.10f);
    drawQuad(3.65f,2.35f,-5.98f, 5.05f,2.35f,-5.98f, 5.05f,3.05f,-5.98f, 3.65f,3.05f,-5.98f);
    setColor(0.35f, 0.60f, 0.90f);
    drawQuad(3.65f,3.05f,-5.98f, 5.05f,3.05f,-5.98f, 5.05f,3.65f,-5.98f, 3.65f,3.65f,-5.98f);
    setColor(1.0f, 0.90f, 0.20f);
    glPushMatrix();
    glTranslatef(4.35f, 3.05f, -5.97f);
    glutSolidSphere(0.18f, 10, 10);
    glPopMatrix();
}

// ===== Lighting Setup =====
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    float pos[]  = {0.0f, 4.5f, -1.0f, 1.0f};
    float amb[]  = {0.20f, 0.18f, 0.14f, 1.0f};
    float spec[] = {0.5f, 0.5f, 0.5f, 1.0f};
    float diff[4];
    if(lightOn){ diff[0]=1.0f; diff[1]=0.95f; diff[2]=0.80f; diff[3]=1.0f; }
    else        { diff[0]=0.1f; diff[1]=0.10f; diff[2]=0.10f; diff[3]=1.0f; }

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

    glEnable(GL_LIGHT1);
    float pos2[]  = {0.0f, 3.5f, -5.5f, 1.0f};
    float diff2[] = {0.50f, 0.65f, 0.80f, 1.0f};
    float amb2[]  = {0.10f, 0.12f, 0.15f, 1.0f};
    float spec2[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, pos2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  diff2);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  amb2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec2);
}

// ===== Display =====
void display() {
    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(camAngleX, 1,0,0);
    glRotatef(camAngleY, 0,1,0);
    glTranslatef(-camX, -camY, -camZ);

    setupLighting();

    drawRoom();
    drawDoubleWindow();
    drawDoor();
    drawBed();
    drawTable();
    drawChair();
    drawBookshelf();
    drawBookRack();    // NEW: book rack beside study table
    drawCarpet();
    drawSideTable();
    drawCeilingLight();
    drawFan();
    drawAC();          // UPDATED: back wall above windows, ON/OFF
    drawSofa();        // UPDATED: on carpet, facing right wall
    //drawClock();
    drawDigitalClock();
    drawPicture();

    // HUD overlay
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i(10, 30);
    const char* hint1 = "W/S: Fwd/Back  A/D: Left/Right  Q/E: Up/Down  Arrows: Look  L: Light  F: Fan On/Off  +/-: Fan Speed  O: AC On  P: AC Off  ESC: Quit";
    for(int i=0; hint1[i]; i++) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, hint1[i]);

    glRasterPos2i(10, 14);
    char info[160];
    sprintf(info, "Fan Speed: %.1f deg/frame  Fan: %s  Light: %s  AC: %s",
            fanSpeed,
            fanOn    ? "ON"  : "OFF",
            lightOn  ? "ON"  : "OFF",
            acOn     ? "ON"  : "OFF");
    for(int i=0; info[i]; i++) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, info[i]);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

// ===== Idle (animation) =====
void idle() {
    if(fanOn) {
        fanAngle += fanSpeed;
        if(fanAngle >= 360.0f) fanAngle -= 360.0f;
    }

    // Airflow animates ONLY when AC is on
    if(acOn) {
        acFlow += 0.008f;
        if(acFlow > 1.0f) acFlow -= 1.0f;
    }

    glutPostRedisplay();
}

// ===== Reshape =====
void reshape(int w, int h) {
    if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// ===== Keyboard =====
void keyboard(unsigned char key, int x, int y) {
    float rad = camAngleY * 3.14159f / 180.0f;
    float speed = 0.3f;
    switch(key) {
        case 'w': case 'W': camX -= sin(rad)*speed; camZ -= cos(rad)*speed; break;
        case 's': case 'S': camX += sin(rad)*speed; camZ += cos(rad)*speed; break;
        case 'a': case 'A': camX -= cos(rad)*speed; camZ += sin(rad)*speed; break;
        case 'd': case 'D': camX += cos(rad)*speed; camZ -= sin(rad)*speed; break;
        case 'q': case 'Q': camY += speed; break;
        case 'e': case 'E': camY -= speed; break;
        case 'l': case 'L': lightOn = !lightOn; break;
        case 'f': case 'F': fanOn   = !fanOn;   break;
        case 'o': case 'O': acOn    = true;      break;   // AC ON
        case 'p': case 'P': acOn    = false;     break;   // AC OFF
        case '+': case '=': fanSpeed += 1.0f; if(fanSpeed > 20.0f) fanSpeed = 20.0f; break;
        case '-': case '_': fanSpeed -= 1.0f; if(fanSpeed <  0.5f) fanSpeed =  0.5f; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

// ===== Special Keys =====
void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT:  camAngleY -= 3.0f; break;
        case GLUT_KEY_RIGHT: camAngleY += 3.0f; break;
        case GLUT_KEY_UP:    camAngleX -= 3.0f; break;
        case GLUT_KEY_DOWN:  camAngleX += 3.0f; break;
    }
    glutPostRedisplay();
}

// ===== Main =====
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("3D Room Interior - Extended | OpenGL/GLUT");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
