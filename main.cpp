#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

// ===== Camera =====
float camX = -1.0f, camY = 2.0f, camZ = 5.9f;
float camAngleX = -0.0f, camAngleY = 13.0f;

// ===== Light animation =====
bool lightOn = true;

// ===== Fan animation =====
float fanAngle = 0.0f;
float fanSpeed  = 3.0f;
bool  fanOn     = true;

// ===== AC =====
bool  acOn   = true;
float acFlow = 0.0f;

// ===== Curtain animation =====
float curtainWave = 0.0f;

// ===== TV Screen flicker =====
float tvFlicker = 0.0f;
bool  tvOn      = true;

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
    // Floor
    setColor(0.88f, 0.88f, 0.86f);
    drawQuad(-6,-0.01f,-6,  6,-0.01f,-6,  6,-0.01f,6,  -6,-0.01f,6);

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
    drawQuad(-6,0,6,  6,0,6,  6,5,6,  -6,5,6);

}

// ===== Front Wall Decorations =====
void drawFrontWallDecor() {

    float shift = -3.8f;   // move everything 3.5f left

    // ── Wall Shelf (front wall, center-left) ──
    setColor(0.55f, 0.35f, 0.14f);
    drawQuad(-1.5f+shift,3.2f,5.99f,  1.5f+shift,3.2f,5.99f,  1.5f+shift,3.42f,5.99f,  -1.5f+shift,3.42f,5.99f);

    // Shelf depth
    setColor(0.48f, 0.30f, 0.12f);
    glPushMatrix();
    glTranslatef(0+shift, 3.31f, 5.78f);
    glScalef(3.0f, 0.10f, 0.44f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Shelf bracket left
    setColor(0.40f, 0.25f, 0.10f);
    glPushMatrix(); glTranslatef(-1.3f+shift, 3.0f, 5.82f); glScalef(0.06f, 0.42f, 0.06f); glutSolidCube(1.0); glPopMatrix();

    // Shelf bracket right
    glPushMatrix(); glTranslatef(1.3f+shift, 3.0f, 5.82f); glScalef(0.06f, 0.42f, 0.06f); glutSolidCube(1.0); glPopMatrix();

    // Small items on shelf — 3 small books
    float bkC[3][3]={{0.85f,0.15f,0.15f},{0.15f,0.55f,0.85f},{0.15f,0.72f,0.25f}};
    float bkX[3]={-0.9f, -0.6f, -0.25f};

    for(int b=0;b<3;b++){
        setColor(bkC[b][0],bkC[b][1],bkC[b][2]);
        glPushMatrix();
        glTranslatef(bkX[b]+shift, 3.52f, 5.82f);
        glScalef(0.12f, 0.28f, 0.18f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Small plant on shelf
    setColor(0.62f, 0.38f, 0.18f);
    glPushMatrix(); glTranslatef(0.7f+shift, 3.42f, 5.82f); drawCylinder(0.07f, 0.10f, 10); glPopMatrix();

    setColor(0.18f, 0.65f, 0.22f);
    glPushMatrix(); glTranslatef(0.7f+shift, 3.56f, 5.82f); glutSolidSphere(0.10f, 8, 8); glPopMatrix();

    // Small vase on shelf
    setColor(0.75f, 0.55f, 0.80f);
    glPushMatrix(); glTranslatef(1.1f+shift, 3.42f, 5.82f); drawCylinder(0.055f, 0.14f, 10); glPopMatrix();

    // ── Wall Art / Painting ──
    setColor(0.28f, 0.16f, 0.06f);
    drawQuad(-0.9f+shift,1.2f,5.99f,  0.9f+shift,1.2f,5.99f,  0.9f+shift,2.8f,5.99f,  -0.9f+shift,2.8f,5.99f);

    setColor(0.96f, 0.92f, 0.82f);
    drawQuad(-0.78f+shift,1.32f,5.985f,  0.78f+shift,1.32f,5.985f,  0.78f+shift,2.68f,5.985f,  -0.78f+shift,2.68f,5.985f);

    setColor(0.40f, 0.62f, 0.88f);
    drawQuad(-0.78f+shift,2.10f,5.982f,  0.78f+shift,2.10f,5.982f,  0.78f+shift,2.68f,5.982f,  -0.78f+shift,2.68f,5.982f);

    setColor(0.25f, 0.55f, 0.25f);
    drawQuad(-0.78f+shift,1.32f,5.982f,  0.78f+shift,1.32f,5.982f,  0.78f+shift,2.10f,5.982f,  -0.78f+shift,2.10f,5.982f);

    setColor(1.0f, 0.90f, 0.20f);
    glPushMatrix(); glTranslatef(0.3f+shift, 2.35f, 5.978f); glutSolidSphere(0.15f, 10, 10); glPopMatrix();

    setColor(0.55f, 0.55f, 0.58f);
    glPushMatrix(); glTranslatef(0+shift, 2.82f, 5.995f); glutSolidSphere(0.018f, 6, 6); glPopMatrix();

    // ── Power socket ──
    setColor(0.88f, 0.88f, 0.86f);
    drawQuad(1.65f+shift,0.4f,5.99f,  1.65f+shift,0.75f,5.99f,  2.0f+shift,0.75f,5.99f,  2.0f+shift,0.4f,5.99f);

    setColor(0.30f, 0.30f, 0.30f);
    glPushMatrix(); glTranslatef(1.77f+shift, 0.52f, 5.995f); glutSolidSphere(0.025f,5,5); glPopMatrix();
    glPushMatrix(); glTranslatef(1.88f+shift, 0.52f, 5.995f); glutSolidSphere(0.025f,5,5); glPopMatrix();
}

/// ===== SINGLE Window on back wall (LOWERED) with curtains =====
void drawSingleWindowWithCurtain() {

    float ox   = -1.0f;
    float drop = 0.65f;     // window up/dwon

    // Outer frame
    setColor(0.30f, 0.18f, 0.08f);
    drawQuad(ox-0.15f, 2.75f-drop,-5.99f,
             ox+2.15f, 2.75f-drop,-5.99f,
             ox+2.15f, 4.35f-drop,-5.99f,
             ox-0.15f, 4.35f-drop,-5.99f);

    // Glass
    setColor(0.68f, 0.85f, 0.97f);
    drawQuad(ox,       3.0f-drop, -5.985f,
             ox+2.0f,  3.0f-drop, -5.985f,
             ox+2.0f,  4.2f-drop, -5.985f,
             ox,       4.2f-drop, -5.985f);

    // Vertical divider
    setColor(0.30f, 0.18f, 0.08f);
    drawQuad(ox+0.97f, 3.0f-drop,-5.975f,
             ox+1.03f, 3.0f-drop,-5.975f,
             ox+1.03f, 4.2f-drop,-5.975f,
             ox+0.97f, 4.2f-drop,-5.975f);

    // Horizontal divider
    drawQuad(ox,      3.57f-drop,-5.975f,
             ox+2.0f, 3.57f-drop,-5.975f,
             ox+2.0f, 3.63f-drop,-5.975f,
             ox,      3.63f-drop,-5.975f);

    // Window sill
    setColor(0.42f, 0.26f, 0.10f);
    drawQuad(ox-0.15f, 2.73f-drop,-5.99f,
             ox+2.15f, 2.73f-drop,-5.99f,
             ox+2.15f, 2.75f-drop,-5.99f,
             ox-0.15f, 2.75f-drop,-5.99f);

    // Curtain rod
    setColor(0.65f, 0.52f, 0.35f);
    glPushMatrix();
    glTranslatef(ox - 0.4f, 2.30f-drop, -5.90f);
    drawCylinder(0.025f, 2.10f, 8);
    glPopMatrix();

    // End caps
    setColor(0.95f, 0.78f, 0.18f);

    // Bottom cap
    glPushMatrix();
    glTranslatef(ox - 0.4f, 2.30f-drop, -5.90f);
    glutSolidSphere(0.05f, 8,8);
    glPopMatrix();

    // Top cap
    glPushMatrix();
    glTranslatef(ox - 0.4f, 4.40f-drop, -5.90f);
    glutSolidSphere(0.05f, 8,8);
    glPopMatrix();

    // Curtain settings
    int folds = 8;
    float panelW = 0.85f;
    float topY   = 4.40f - drop;
    float botY   = 2.60f - drop;
    float cz     = -5.89f;

    // Left curtain
    setColor(0.72f, 0.35f, 0.28f);
    float startX = ox - 0.35f;

    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=folds;i++){
        float t = (float)i/folds;
        float xp = startX + t*panelW;
        float wave = sin(t*3.14159f*2.5f + curtainWave)*0.06f;

        glVertex3f(xp, topY, cz + wave);
        glVertex3f(xp, botY, cz + wave*0.5f);
    }
    glEnd();

    // Right curtain
    float startX2 = ox + 1.5f;

    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=folds;i++){
        float t = (float)i/folds;
        float xp = startX2 + t*panelW;
        float wave = sin(t*3.14159f*2.5f - curtainWave)*0.06f;

        glVertex3f(xp, topY, cz + wave);
        glVertex3f(xp, botY, cz + wave*0.5f);
    }
    glEnd();
}

// ===== ★ NEW: Wall-Mounted TV on Front Wall =====
void drawWallMountedTV() {
    // TV is centered at x = -3.5f on front wall (z = 5.99f)
    // TV screen area: x from -5.2 to -1.8, y from 1.80 to 3.50

    float cx = 0.00f;   // center X
    float cy = 2.65f;    // center Y
    float tw = 3.40f;    // total width
    float th = 1.70f;    // total height
    float wz = 5.99f;    // wall z

    // ── Outer bezel (very slim, matte black) ──
    setColor(0.08f, 0.08f, 0.10f);
    drawQuad(cx - tw*0.5f,      cy - th*0.5f,      wz,
             cx + tw*0.5f,      cy - th*0.5f,      wz,
             cx + tw*0.5f,      cy + th*0.5f,      wz,
             cx - tw*0.5f,      cy + th*0.5f,      wz);

    // ── Screen surface ──
    if(tvOn) {
        // Dynamic blue-teal screen glow
        float g = 0.38f + 0.04f * sin(tvFlicker);
        setColor(0.05f + 0.02f*sin(tvFlicker*0.7f), g, 0.62f);
    } else {
        setColor(0.04f, 0.04f, 0.05f);
    }
    float bw = 0.06f; // bezel width
    drawQuad(cx - tw*0.5f + bw,  cy - th*0.5f + bw,  wz + 0.005f,
             cx + tw*0.5f - bw,  cy - th*0.5f + bw,  wz + 0.005f,
             cx + tw*0.5f - bw,  cy + th*0.5f - bw,  wz + 0.005f,
             cx - tw*0.5f + bw,  cy + th*0.5f - bw,  wz + 0.005f);

    if(tvOn) {
        // ── Simulated UI on screen: dark bars + colorful blocks ──
        glDisable(GL_LIGHTING);

        // Top bar (status bar)
        setColor(0.02f, 0.12f, 0.25f);
        drawQuad(cx - 1.60f, cy + 0.72f, wz+0.008f,
                 cx + 1.60f, cy + 0.72f, wz+0.008f,
                 cx + 1.60f, cy + 0.80f, wz+0.008f,
                 cx - 1.60f, cy + 0.80f, wz+0.008f);

        // Main content blocks (simulated app/show grid)
        float colors[4][3] = {
            {0.80f,0.20f,0.20f},
            {0.20f,0.50f,0.85f},
            {0.18f,0.72f,0.45f},
            {0.85f,0.55f,0.10f}
        };
        float blockW = 0.70f, blockH = 0.50f;
        for(int i = 0; i < 4; i++) {
            setColor(colors[i][0]*0.6f, colors[i][1]*0.6f, colors[i][2]*0.6f);
            float bx = cx - 1.45f + i * 0.78f;
            drawQuad(bx,          cy - 0.42f, wz+0.009f,
                     bx+blockW,   cy - 0.42f, wz+0.009f,
                     bx+blockW,   cy + 0.10f, wz+0.009f,
                     bx,          cy + 0.10f, wz+0.009f);
            // highlight on block
            setColor(colors[i][0], colors[i][1], colors[i][2]);
            drawQuad(bx+0.04f,    cy - 0.38f, wz+0.010f,
                     bx+blockW*0.5f, cy - 0.38f, wz+0.010f,
                     bx+blockW*0.5f, cy - 0.18f, wz+0.010f,
                     bx+0.04f,    cy - 0.18f, wz+0.010f);
        }

        // Bottom navigation bar
        setColor(0.02f, 0.08f, 0.18f);
        drawQuad(cx - 1.60f, cy - 0.76f, wz+0.008f,
                 cx + 1.60f, cy - 0.76f, wz+0.008f,
                 cx + 1.60f, cy - 0.50f, wz+0.008f,
                 cx - 1.60f, cy - 0.50f, wz+0.008f);

        // Nav icons (small colored dots)
        float navX[] = {cx-1.2f, cx-0.6f, cx, cx+0.6f, cx+1.2f};
        float navC[5][3] = {
            {0.9f,0.9f,0.9f},{0.5f,0.8f,1.0f},{0.3f,0.9f,0.5f},
            {1.0f,0.7f,0.2f},{0.8f,0.4f,0.8f}
        };
        for(int n=0;n<5;n++) {
            setColor(navC[n][0],navC[n][1],navC[n][2]);
            glPushMatrix();
            glTranslatef(navX[n], cy-0.63f, wz+0.010f);
            glutSolidSphere(0.045f, 6, 6);
            glPopMatrix();
        }

        glEnable(GL_LIGHTING);

        // ── Screen ambient glow (subtle colored quad slightly in front) ──
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.10f, 0.35f, 0.65f, 0.08f + 0.03f*sin(tvFlicker));
        drawQuad(cx - tw*0.5f + bw, cy - th*0.5f + bw, wz+0.012f,
                 cx + tw*0.5f - bw, cy - th*0.5f + bw, wz+0.012f,
                 cx + tw*0.5f - bw, cy + th*0.5f - bw, wz+0.012f,
                 cx - tw*0.5f + bw, cy + th*0.5f - bw, wz+0.012f);
        glDisable(GL_BLEND);
    }

    // ── Wall bracket (metal mount) ──
    setColor(0.45f, 0.45f, 0.48f);
    // Horizontal arm
    glPushMatrix();
    glTranslatef(cx, cy - 0.08f, wz + 0.10f);
    glScalef(0.55f, 0.05f, 0.20f);
    glutSolidCube(1.0f);
    glPopMatrix();
    // Vertical plate on wall
    setColor(0.35f, 0.35f, 0.38f);
    glPushMatrix();
    glTranslatef(cx, cy - 0.08f, wz + 0.02f);
    glScalef(0.30f, 0.28f, 0.04f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ── Power LED indicator ──
    if(tvOn) setColor(0.10f, 0.95f, 0.30f);
    else     setColor(0.90f, 0.10f, 0.10f);
    glPushMatrix();
    glTranslatef(cx + tw*0.5f - 0.15f, cy - th*0.5f + 0.05f, wz + 0.008f);
    glutSolidSphere(0.015f, 5, 5);
    glPopMatrix();

    // ── Soundbar below TV (sleek) ──
    setColor(0.10f, 0.10f, 0.12f);
    glPushMatrix();
    glTranslatef(cx, cy - th*0.5f - 0.12f, wz + 0.06f);
    glScalef(2.60f, 0.12f, 0.12f);
    glutSolidCube(1.0f);
    glPopMatrix();
    // Soundbar grille dots
    glDisable(GL_LIGHTING);
    setColor(0.22f, 0.22f, 0.24f);
    for(int d = 0; d < 18; d++) {
        glPushMatrix();
        glTranslatef(cx - 1.20f + d * 0.135f, cy - th*0.5f - 0.12f, wz + 0.125f);
        glutSolidSphere(0.018f, 4, 4);
        glPopMatrix();
    }
    // Soundbar LED strip (blue)
    if(tvOn) setColor(0.20f, 0.55f, 1.0f);
    else     setColor(0.12f, 0.12f, 0.14f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex3f(cx - 1.25f, cy - th*0.5f - 0.065f, wz + 0.125f);
        glVertex3f(cx + 1.25f, cy - th*0.5f - 0.065f, wz + 0.125f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}


// ===== ★ NEW: TV Cabinet below TV =====
void drawTVCabinet() {
    // Cabinet positioned in front of TV wall, centered at x=-3.5
    float cx = 0.00f;
    float cz = 5.40f;  // slightly away from wall

    glPushMatrix();
    glTranslatef(cx, 0.0f, cz);

    // ── Main cabinet body ──
    setColor(0.18f, 0.18f, 0.20f);
    glPushMatrix();
    glTranslatef(0, 0.32f, 0);
    glScalef(3.20f, 0.64f, 0.62f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ── Cabinet top surface (light wood veneer) ──
    setColor(0.70f, 0.55f, 0.38f);
    glPushMatrix();
    glTranslatef(0, 0.655f, 0);
    glScalef(3.24f, 0.035f, 0.66f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ── Front panel face (medium dark, 3 door sections) ──
    float doorX[] = {-1.02f, 0.0f, 1.02f};
    for(int d = 0; d < 3; d++) {
        // Door panel
        setColor(0.22f, 0.22f, 0.25f);
        glPushMatrix();
        glTranslatef(doorX[d], 0.32f, 0.315f);
        glScalef(0.93f, 0.58f, 0.02f);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Door handle (small gold bar)
        setColor(0.80f, 0.68f, 0.22f);
        glPushMatrix();
        glTranslatef(doorX[d], 0.32f, 0.328f);
        glScalef(0.18f, 0.025f, 0.015f);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Subtle door edge highlight
        setColor(0.32f, 0.32f, 0.36f);
        glPushMatrix();
        glTranslatef(doorX[d], 0.32f, 0.308f);
        glScalef(0.95f, 0.60f, 0.005f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // ── Slim legs / feet (4 metal cylinders) ──
    setColor(0.55f, 0.55f, 0.58f);
    float lx[] = {-1.45f, 1.45f, -1.45f, 1.45f};
    float lz[] = {-0.27f, -0.27f, 0.27f, 0.27f};
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(lx[i], 0.0f, lz[i]);
        drawCylinder(0.04f, 0.065f, 8);
        glPopMatrix();
    }

    // ── Items ON cabinet top ──

    // Set-top box / streaming device (left side)
    setColor(0.12f, 0.12f, 0.14f);
    glPushMatrix();
    glTranslatef(-1.10f, 0.675f, 0.05f);
    glScalef(0.40f, 0.06f, 0.26f);
    glutSolidCube(1.0f);
    glPopMatrix();
    // STB LED
    if(tvOn) setColor(0.10f, 0.90f, 0.30f);
    else     setColor(0.80f, 0.10f, 0.10f);
    glPushMatrix();
    glTranslatef(-0.92f, 0.708f, 0.18f);
    glutSolidSphere(0.012f, 5, 5);
    glPopMatrix();

    // Small decorative plant (right side)
    setColor(0.52f, 0.32f, 0.14f);
    glPushMatrix();
    glTranslatef(1.10f, 0.655f, 0.0f);
    drawCylinder(0.065f, 0.10f, 10);
    glPopMatrix();
    setColor(0.15f, 0.65f, 0.25f);
    glPushMatrix();
    glTranslatef(1.10f, 0.785f, 0.0f);
    glutSolidSphere(0.10f, 8, 8);
    glPopMatrix();

    // Small photo frame (center-right on top)
    setColor(0.28f, 0.16f, 0.06f);
    glPushMatrix();
    glTranslatef(0.42f, 0.660f, -0.05f);
    glScalef(0.18f, 0.24f, 0.04f);
    glutSolidCube(1.0f);
    glPopMatrix();
    setColor(0.55f, 0.70f, 0.90f);
    glPushMatrix();
    glTranslatef(0.42f, 0.665f, -0.03f);
    glScalef(0.14f, 0.18f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ── Cable management (thin line down to floor) ──
    glDisable(GL_LIGHTING);
    setColor(0.15f, 0.15f, 0.18f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
        glVertex3f(0.0f, 0.655f, 0.31f);
        glVertex3f(0.0f, 0.0f,   0.31f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

// ===== Door on left wall — INSIDE ROOM (swing inside) =====
void drawDoor() {
    // Door frame embedded in left wall
    setColor(0.35f, 0.22f, 0.10f);
    drawQuad(-5.99f,0,1.2f, -5.99f,0,3.2f, -5.99f,3.8f,3.2f, -5.99f,3.8f,1.2f);

    // Door panels
    setColor(0.55f, 0.38f, 0.18f);
    drawQuad(-5.98f,0.05f,1.3f, -5.98f,0.05f,3.1f, -5.98f,3.65f,3.1f, -5.98f,3.65f,1.3f);

    // Door details
    setColor(0.48f, 0.32f, 0.14f);
    drawQuad(-5.97f,0.3f,1.5f, -5.97f,0.3f,2.9f, -5.97f,1.7f,2.9f, -5.97f,1.7f,1.5f);
    drawQuad(-5.97f,2.0f,1.5f, -5.97f,2.0f,2.9f, -5.97f,3.3f,2.9f, -5.97f,3.3f,1.5f);

    // Door handle (gold)
    setColor(0.85f, 0.72f, 0.10f);
    glPushMatrix();
    glTranslatef(-5.96f, 1.8f, 3.0f);
    glutSolidSphere(0.08, 10, 10);
    glPopMatrix();

    // ===== Door stopper on floor (inside room) =====
    setColor(0.60f, 0.60f, 0.62f);
    glPushMatrix();
    glTranslatef(-5.60f, 0.0f, 3.15f);
    drawCylinder(0.04f, 0.08f, 8);
    glPopMatrix();
    // Rubber tip
    setColor(0.15f, 0.15f, 0.15f);
    glPushMatrix();
    glTranslatef(-5.60f, 0.08f, 3.15f);
    glutSolidSphere(0.045f, 6, 6);
    glPopMatrix();

    // ===== Light switch on wall beside door (inside room) =====
    // Switch plate
    setColor(0.92f, 0.92f, 0.90f);
    drawQuad(-5.97f, 1.85f, 3.35f,
             -5.97f, 1.85f, 3.65f,
             -5.97f, 2.25f, 3.65f,
             -5.97f, 2.25f, 3.35f);

    // Two switch toggles
    setColor(0.80f, 0.80f, 0.78f);
    drawQuad(-5.965f, 1.95f, 3.42f,
             -5.965f, 1.95f, 3.52f,
             -5.965f, 2.15f, 3.52f,
             -5.965f, 2.15f, 3.42f);
    drawQuad(-5.965f, 1.95f, 3.55f,
             -5.965f, 1.95f, 3.65f,
             -5.965f, 2.15f, 3.65f,
             -5.965f, 2.15f, 3.55f);
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

// ===== SMART Study Table (modern look) =====
void drawTable() {
    glPushMatrix();
    glTranslatef(3.5f, 0, -5.2f);

    // Modern thick top with rounded feel (white/light gray)
    setColor(0.92f, 0.92f, 0.95f);
    glPushMatrix(); glTranslatef(0, 1.55f, 0); glScalef(2.5f, 0.12f, 1.4f); glutSolidCube(1.0); glPopMatrix();

    // Edge strip (accent color — teal/dark)
    setColor(0.15f, 0.45f, 0.55f);
    glPushMatrix(); glTranslatef(0, 1.48f, 0); glScalef(2.5f, 0.04f, 1.4f); glutSolidCube(1.0); glPopMatrix();

    // Metal frame legs (thin, angular — modern)
    setColor(0.55f, 0.55f, 0.58f);
    // Two U-shaped side frames
    float lx[] = {-1.1f, 1.1f};
    for(int s = 0; s < 2; s++){
        // Vertical leg front
        glPushMatrix(); glTranslatef(lx[s], 0.77f, 0.6f); glScalef(0.07f, 1.54f, 0.07f); glutSolidCube(1.0); glPopMatrix();
        // Vertical leg back
        glPushMatrix(); glTranslatef(lx[s], 0.77f, -0.6f); glScalef(0.07f, 1.54f, 0.07f); glutSolidCube(1.0); glPopMatrix();
        // Horizontal foot bar
        glPushMatrix(); glTranslatef(lx[s], 0.04f, 0); glScalef(0.07f, 0.07f, 1.2f); glutSolidCube(1.0); glPopMatrix();
    }

    // Under-shelf (white)
    setColor(0.88f, 0.88f, 0.90f);
    glPushMatrix(); glTranslatef(0, 0.65f, 0); glScalef(2.3f, 0.06f, 1.2f); glutSolidCube(1.0); glPopMatrix();

    // Monitor (slim bezel)
    setColor(0.12f, 0.12f, 0.14f);
    glPushMatrix(); glTranslatef(0.2f, 1.62f, 0.0f); glScalef(0.95f, 0.05f, 0.55f); glutSolidCube(1.0); glPopMatrix();
    // Monitor screen stand
    setColor(0.20f, 0.20f, 0.22f);
    glPushMatrix(); glTranslatef(0.2f, 1.80f, -0.22f); glScalef(0.05f, 0.45f, 0.05f); glutSolidCube(1.0); glPopMatrix();
    // Monitor screen
    setColor(0.12f, 0.12f, 0.16f);
    glPushMatrix(); glTranslatef(0.2f, 2.02f, -0.24f); glRotatef(-5, 1,0,0); glScalef(0.92f, 0.55f, 0.04f); glutSolidCube(1.0); glPopMatrix();
    // Screen display (blue glow)
    setColor(0.25f, 0.50f, 0.90f);
    glPushMatrix(); glTranslatef(0.2f, 2.02f, -0.22f); glRotatef(-5, 1,0,0); glScalef(0.84f, 0.48f, 0.01f); glutSolidCube(1.0); glPopMatrix();

    // Keyboard (smart flat)
    setColor(0.82f, 0.82f, 0.85f);
    glPushMatrix(); glTranslatef(0.2f, 1.62f, 0.30f); glScalef(0.75f, 0.025f, 0.28f); glutSolidCube(1.0); glPopMatrix();
    // Keyboard key rows (dark)
    setColor(0.35f, 0.35f, 0.38f);
    for(int row=0;row<3;row++){
        glPushMatrix(); glTranslatef(0.2f, 1.636f, 0.22f + row*0.08f); glScalef(0.68f, 0.008f, 0.05f); glutSolidCube(1.0); glPopMatrix();
    }

    // Wireless mouse
    setColor(0.90f, 0.90f, 0.92f);
    glPushMatrix(); glTranslatef(0.82f, 1.62f, 0.28f); glScalef(0.12f, 0.025f, 0.20f); glutSolidCube(1.0); glPopMatrix();

    // Desk organizer / pen holder
    setColor(0.25f, 0.45f, 0.50f);
    glPushMatrix(); glTranslatef(-0.85f, 1.62f, -0.42f); drawCylinder(0.09f, 0.22f, 10); glPopMatrix();
    // Pens in holder
    float penC[3][3]={{0.9f,0.2f,0.2f},{0.2f,0.7f,0.3f},{0.2f,0.3f,0.9f}};
    float pa[] = {0.0f, 2.0f, 4.0f};
    for(int p=0;p<3;p++){
        setColor(penC[p][0],penC[p][1],penC[p][2]);
        glPushMatrix();
        glTranslatef(-0.85f + cos(pa[p])*0.04f, 1.68f, -0.42f + sin(pa[p])*0.04f);
        drawCylinder(0.012f, 0.28f, 6);
        glPopMatrix();
    }

    // Small potted plant on corner of desk
    setColor(0.40f, 0.25f, 0.10f);
    glPushMatrix(); glTranslatef(-0.85f, 1.62f, 0.45f); drawCylinder(0.07f, 0.10f, 10); glPopMatrix();
    setColor(0.15f, 0.62f, 0.22f);
    glPushMatrix(); glTranslatef(-0.85f, 1.76f, 0.45f); glutSolidSphere(0.10f, 8, 8); glPopMatrix();

    // Notepad
    setColor(0.98f, 0.95f, 0.85f);
    glPushMatrix(); glTranslatef(-0.30f, 1.63f, 0.42f); glScalef(0.32f, 0.01f, 0.22f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.50f, 0.65f, 0.90f);
    glPushMatrix(); glTranslatef(-0.30f, 1.638f, 0.42f); glScalef(0.30f, 0.005f, 0.04f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.30f, 1.638f, 0.46f); glScalef(0.30f, 0.005f, 0.04f); glutSolidCube(1.0); glPopMatrix();

    glPopMatrix();
}

// ===== SMART Chair =====
void drawChair() {
    glPushMatrix();
    glTranslatef(3.5f, 0, -3.8f);
    glRotatef(180.0f, 0, 1, 0);

    // Seat — ergonomic contoured (dark gray)
    setColor(0.22f, 0.22f, 0.24f);
    glPushMatrix(); glTranslatef(0, 1.0f, 0); glScalef(0.92f, 0.10f, 0.90f); glutSolidCube(1.0); glPopMatrix();

    // Seat cushion
    setColor(0.28f, 0.55f, 0.72f);
    glPushMatrix(); glTranslatef(0, 1.06f, 0.03f); glScalef(0.84f, 0.06f, 0.80f); glutSolidCube(1.0); glPopMatrix();

    // Back rest (tall ergonomic)
    setColor(0.22f, 0.22f, 0.24f);
    glPushMatrix(); glTranslatef(0, 1.60f, -0.42f); glScalef(0.88f, 1.00f, 0.10f); glutSolidCube(1.0); glPopMatrix();

    // Back cushion
    setColor(0.28f, 0.55f, 0.72f);
    glPushMatrix(); glTranslatef(0, 1.60f, -0.38f); glScalef(0.80f, 0.90f, 0.07f); glutSolidCube(1.0); glPopMatrix();

    // Lumbar support bump
    setColor(0.20f, 0.48f, 0.65f);
    glPushMatrix(); glTranslatef(0, 1.35f, -0.34f); glScalef(0.50f, 0.18f, 0.06f); glutSolidCube(1.0); glPopMatrix();

    // Headrest
    setColor(0.22f, 0.22f, 0.24f);
    glPushMatrix(); glTranslatef(0, 2.18f, -0.42f); glScalef(0.42f, 0.22f, 0.10f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.28f, 0.55f, 0.72f);
    glPushMatrix(); glTranslatef(0, 2.18f, -0.38f); glScalef(0.38f, 0.18f, 0.07f); glutSolidCube(1.0); glPopMatrix();

    // Armrests
    setColor(0.18f, 0.18f, 0.20f);
    float ax[] = {-0.50f, 0.50f};
    for(int a=0;a<2;a++){
        // Arm post
        glPushMatrix(); glTranslatef(ax[a], 1.02f, 0.0f); glScalef(0.06f, 0.50f, 0.06f); glutSolidCube(1.0); glPopMatrix();
        // Arm pad
        setColor(0.30f, 0.30f, 0.32f);
        glPushMatrix(); glTranslatef(ax[a], 1.30f, 0.05f); glScalef(0.10f, 0.05f, 0.40f); glutSolidCube(1.0); glPopMatrix();
        setColor(0.18f, 0.18f, 0.20f);
    }

    // Gas lift cylinder (center post)
    setColor(0.60f, 0.60f, 0.62f);
    glPushMatrix(); glTranslatef(0, 0.50f, 0); drawCylinder(0.055f, 0.55f, 10); glPopMatrix();

    // Star base (5 legs)
    setColor(0.25f, 0.25f, 0.28f);
    for(int leg=0;leg<5;leg++){
        float angle = leg * 72.0f * 3.14159f / 180.0f;
        float ex = cos(angle)*0.58f;
        float ez = sin(angle)*0.58f;
        glPushMatrix();
        glTranslatef(0, 0.04f, 0);
        glBegin(GL_QUADS);
            glVertex3f(0,       0,      0);
            glVertex3f(ex-0.04f, 0, ez-0.04f);
            glVertex3f(ex,      0.08f, ez);
            glVertex3f(0.04f,  0.08f, 0.04f);
        glEnd();
        glPopMatrix();
        // Wheel
        setColor(0.15f, 0.15f, 0.15f);
        glPushMatrix(); glTranslatef(ex, 0.04f, ez); drawCylinder(0.04f, 0.05f, 6); glPopMatrix();
    }

    glPopMatrix();
}

// ===== Bookshelf =====
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

// ===== Book Rack =====
void drawBookRack() {
    glPushMatrix();
    glTranslatef(5.30f, 0.0f, -5.50f);

    setColor(0.38f, 0.24f, 0.09f);
    glPushMatrix(); glTranslatef(-0.28f, 1.70f, 0); glScalef(0.06f, 3.40f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef( 0.28f, 1.70f, 0); glScalef(0.06f, 3.40f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 3.38f, 0); glScalef(0.62f, 0.06f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.04f, 0); glScalef(0.62f, 0.08f, 0.52f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.30f, 0.18f, 0.07f);
    glPushMatrix(); glTranslatef(0, 1.70f, -0.23f); glScalef(0.56f, 3.30f, 0.04f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.48f, 0.30f, 0.11f);
    float shelfY[] = { 0.72f, 1.38f, 2.04f, 2.70f };
    for(int s = 0; s < 4; s++){
        glPushMatrix(); glTranslatef(0, shelfY[s], 0); glScalef(0.56f, 0.05f, 0.48f); glutSolidCube(1.0); glPopMatrix();
    }

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
            float w = bookW[b % 5], h = bookH[b % 5];
            setColor(c.r, c.g, c.b);
            glPushMatrix(); glTranslatef(x + w * 0.5f, baseY + h * 0.5f, 0.0f); glScalef(w, h, 0.34f); glutSolidCube(1.0); glPopMatrix();
            setColor(c.r * 0.70f, c.g * 0.70f, c.b * 0.70f);
            glPushMatrix(); glTranslatef(x + w * 0.5f, baseY + h * 0.5f, 0.175f); glScalef(w * 0.88f, h * 0.92f, 0.01f); glutSolidCube(1.0); glPopMatrix();
            x += w + 0.015f;
        }
    }

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

// ===== Ceiling Fan =====
void drawFan() {
    glPushMatrix();
    glTranslatef(0.0f, 4.85f, 0.0f);

    // Rod
    setColor(0.55f, 0.55f, 0.58f);
    glPushMatrix();
    glTranslatef(0, -0.05f, 0);
    drawCylinder(0.04f, 0.35f, 12);
    glPopMatrix();

    // Motor body
    setColor(0.60f, 0.60f, 0.62f);
    glPushMatrix();
    glTranslatef(0, -0.45f, 0);
    drawCylinder(0.18f, 0.22f, 20);
    glPopMatrix();

    // Fan blades
    glPushMatrix();
    glTranslatef(0, -0.36f, 0);
    glRotatef(fanAngle, 0, 1, 0);

    int numBlades = 4;
    for(int b = 0; b < numBlades; b++) {
        glPushMatrix();
        glRotatef(b * 90.0f, 0, 1, 0);

        // Blade arm
        setColor(0.50f, 0.32f, 0.10f);
        glPushMatrix();
        glTranslatef(0.18f, 0, 0);
        glRotatef(90, 0, 1, 0);
        drawCylinder(0.03f, 0.25f, 8);
        glPopMatrix();

        // Blade top
        setColor(0.65f, 0.42f, 0.18f);
        glPushMatrix();
        glTranslatef(0.72f, 0.01f, 0);
        glRotatef(-8, 0, 0, 1);
        glScalef(0.90f, 0.04f, 0.28f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Blade bottom
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

// ===== AC =====
void drawAC() {
    glPushMatrix();
    glTranslatef(-4.0f, 4.65f, -5.95f);

    setColor(0.90f, 0.90f, 0.92f);
    glPushMatrix(); glScalef(3.20f, 0.48f, 0.30f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.95f, 0.95f, 0.97f);
    glPushMatrix(); glTranslatef(0, 0, 0.155f); glScalef(3.10f, 0.43f, 0.01f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.70f, 0.70f, 0.74f);
    for(int s = 0; s < 5; s++){
        glPushMatrix(); glTranslatef(0, -0.13f + s*0.065f, 0.16f); glScalef(2.90f, 0.014f, 0.04f); glutSolidCube(1.0); glPopMatrix();
    }

    setColor(0.85f, 0.85f, 0.88f);
    glPushMatrix(); glTranslatef(0, -0.23f, 0.15f); glRotatef(acOn ? 30.0f : 5.0f, 1, 0, 0); glScalef(2.95f, 0.03f, 0.20f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.15f, 0.55f, 0.85f);
    glPushMatrix(); glTranslatef(1.35f, 0.10f, 0.16f); glutSolidSphere(0.028f, 8, 8); glPopMatrix();

    if(acOn) setColor(0.10f, 0.90f, 0.30f);
    else     setColor(0.90f, 0.10f, 0.10f);
    glPushMatrix(); glTranslatef(1.20f, 0.10f, 0.16f); glutSolidSphere(0.020f, 6, 6); glPopMatrix();

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

// ===== SOFA =====
void drawSofa() {
    glPushMatrix();
    glTranslatef(5.2f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0, 1, 0);

    // Base frame
    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(0, 0.15f, 0); glScalef(2.80f, 0.28f, 0.95f); glutSolidCube(1.0); glPopMatrix();

    // Seat cushions
    float cc[3][3] = {{0.42f,0.52f,0.62f},{0.40f,0.50f,0.60f},{0.42f,0.52f,0.62f}};
    for(int c = 0; c < 3; c++){
        setColor(cc[c][0], cc[c][1], cc[c][2]);
        glPushMatrix(); glTranslatef(-0.86f + c * 0.86f, 0.44f, 0.04f); glScalef(0.80f, 0.20f, 0.80f); glutSolidCube(1.0); glPopMatrix();
        setColor(0.32f, 0.40f, 0.50f);
        glPushMatrix(); glTranslatef(-0.86f + c * 0.86f, 0.54f, 0.04f); glScalef(0.78f, 0.01f, 0.78f); glutSolidCube(1.0); glPopMatrix();
    }

    // Backrest
    setColor(0.38f, 0.48f, 0.58f);
    glPushMatrix(); glTranslatef(0, 0.82f, -0.40f); glScalef(2.65f, 0.68f, 0.20f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(0, 1.18f, -0.40f); glScalef(2.76f, 0.10f, 0.22f); glutSolidCube(1.0); glPopMatrix();

    // Armrests
    setColor(0.35f, 0.45f, 0.55f);
    glPushMatrix(); glTranslatef(-1.28f, 0.62f, 0.0f); glScalef(0.22f, 0.56f, 0.88f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(-1.28f, 0.94f, 0.0f); glScalef(0.24f, 0.09f, 0.92f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.35f, 0.45f, 0.55f);
    glPushMatrix(); glTranslatef(1.28f, 0.62f, 0.0f); glScalef(0.22f, 0.56f, 0.88f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.25f, 0.22f, 0.20f);
    glPushMatrix(); glTranslatef(1.28f, 0.94f, 0.0f); glScalef(0.24f, 0.09f, 0.92f); glutSolidCube(1.0); glPopMatrix();

    // Legs
    setColor(0.20f, 0.15f, 0.08f);
    float lx[] = {-1.20f, 1.20f, -1.20f, 1.20f};
    float lz[] = {-0.40f, -0.40f, 0.40f, 0.40f};
    for(int i = 0; i < 4; i++){
        glPushMatrix(); glTranslatef(lx[i], 0.06f, lz[i]); drawCylinder(0.05f, 0.12f, 8); glPopMatrix();
    }

    // Throw pillows
    setColor(0.80f, 0.55f, 0.30f);
    glPushMatrix(); glTranslatef(-0.75f, 0.70f, -0.18f); glRotatef(14, 0, 0, 1); glScalef(0.28f, 0.28f, 0.11f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.60f, 0.28f, 0.35f);
    glPushMatrix(); glTranslatef(0.72f, 0.70f, -0.18f); glRotatef(-11, 0, 0, 1); glScalef(0.26f, 0.26f, 0.11f); glutSolidCube(1.0); glPopMatrix();

    glPopMatrix(); // end sofa
}

// ===== T-TABLE (Coffee table) in front of sofa =====
void drawTTable() {
    glPushMatrix();
    glTranslatef(3.80f, 0.0f, 0.0f);

    //Rotate table (Y axis)
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // Table top (glass-look — light blue gray)
    setColor(0.82f, 0.88f, 0.92f);
    glPushMatrix(); glTranslatef(0, 0.72f, 0); glScalef(1.20f, 0.06f, 0.65f); glutSolidCube(1.0); glPopMatrix();

    // Glass edge highlight
    setColor(0.65f, 0.75f, 0.85f);
    glPushMatrix(); glTranslatef(0, 0.74f, 0); glScalef(1.22f, 0.02f, 0.67f); glutSolidCube(1.0); glPopMatrix();

    // Metal cross-frame legs
    setColor(0.65f, 0.65f, 0.68f);
    glPushMatrix(); glTranslatef(0, 0.36f, 0); glRotatef(35, 0, 0, 1); glScalef(0.06f, 0.80f, 0.06f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.36f, 0); glRotatef(-35, 0, 0, 1); glScalef(0.06f, 0.80f, 0.06f); glutSolidCube(1.0); glPopMatrix();

    // Foot bar (Z direction)
    glPushMatrix(); glTranslatef(0, 0.06f, 0); glScalef(0.06f, 0.06f, 0.55f); glutSolidCube(1.0); glPopMatrix();

    // Remote
    setColor(0.15f, 0.15f, 0.18f);
    glPushMatrix(); glTranslatef(0.30f, 0.76f, 0.10f); glScalef(0.06f, 0.015f, 0.20f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.80f, 0.10f, 0.10f);
    glPushMatrix(); glTranslatef(0.30f, 0.768f, 0.04f); glutSolidSphere(0.018f, 6,6); glPopMatrix();

    // Small tray
    setColor(0.55f, 0.42f, 0.28f);
    glPushMatrix(); glTranslatef(-0.20f, 0.755f, -0.05f); glScalef(0.30f, 0.02f, 0.22f); glutSolidCube(1.0); glPopMatrix();

    // Cup on tray
    setColor(0.88f, 0.88f, 0.85f);
    glPushMatrix(); glTranslatef(-0.20f, 0.78f, -0.05f); drawCylinder(0.055f, 0.10f, 10); glPopMatrix();
    setColor(0.55f, 0.30f, 0.10f);
    glPushMatrix(); glTranslatef(-0.20f, 0.87f, -0.05f); glutSolidSphere(0.048f, 8,8); glPopMatrix();

    glPopMatrix();
}

// ===== BIG FLOOR PLANT in corner =====
void drawCornerFlowerPot() {
    glPushMatrix();
    glTranslatef(5.5f, 0.0f, 2.0f);

    // Pot base (terracotta)
    setColor(0.78f, 0.38f, 0.18f);
    glPushMatrix(); glScalef(0.5f, 0.06f, 0.5f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.80f, 0.40f, 0.20f);
    glPushMatrix(); glTranslatef(0, 0.03f, 0); drawCylinder(0.22f, 0.45f, 16); glPopMatrix();

    setColor(0.70f, 0.33f, 0.15f);
    glPushMatrix(); glTranslatef(0, 0.46f, 0); drawCylinder(0.24f, 0.06f, 16); glPopMatrix();

    setColor(0.22f, 0.15f, 0.08f);
    glPushMatrix(); glTranslatef(0, 0.50f, 0); glutSolidSphere(0.21f, 10, 5); glPopMatrix();

    setColor(0.35f, 0.22f, 0.10f);
    glPushMatrix(); glTranslatef(0, 0.52f, 0); drawCylinder(0.04f, 1.10f, 8); glPopMatrix();

    glPushMatrix(); glTranslatef(0, 1.30f, 0); glRotatef(-35, 0, 0, 1); drawCylinder(0.025f, 0.55f, 6); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 1.20f, 0); glRotatef(40, 0, 0, 1); glRotatef(60, 0, 1, 0); drawCylinder(0.022f, 0.50f, 6); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 1.10f, 0); glRotatef(30, 1, 0, 0); drawCylinder(0.020f, 0.45f, 6); glPopMatrix();

    float leafColors[5][3] = {
        {0.15f,0.65f,0.20f},{0.10f,0.58f,0.15f},{0.20f,0.72f,0.15f},
        {0.12f,0.55f,0.22f},{0.18f,0.68f,0.12f}
    };
    struct Cluster { float x,y,z,r; };
    Cluster clusters[] = {
        {0,      1.62f, 0,    0.28f},
        {-0.35f, 1.70f, 0,   0.22f},
        { 0.38f, 1.65f, 0.1f,0.20f},
        { 0.1f,  1.72f,-0.2f,0.18f},
        {-0.2f,  1.58f, 0.2f,0.16f},
        { 0,     1.85f, 0,   0.16f},
    };
    for(int i=0;i<6;i++){
        setColor(leafColors[i%5][0], leafColors[i%5][1], leafColors[i%5][2]);
        glPushMatrix();
        glTranslatef(clusters[i].x, clusters[i].y, clusters[i].z);
        glutSolidSphere(clusters[i].r, 8, 8);
        glPopMatrix();
    }

    glPopMatrix();
}

// ===== DRESSING TABLE =====
void drawDressingTable() {
    glPushMatrix();
    glTranslatef(5.5f, 0.0f, 4.0f);
    glRotatef(270.0f, 0, 1, 0);

    setColor(0.58f, 0.38f, 0.16f);
    glPushMatrix(); glTranslatef(0, 0.70f, 0); glScalef(1.80f, 1.40f, 0.52f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.68f, 0.45f, 0.18f);
    glPushMatrix(); glTranslatef(0, 1.42f, 0); glScalef(1.85f, 0.06f, 0.55f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.50f, 0.32f, 0.12f);
    float dY[] = {0.35f, 0.72f, 1.08f};
    for(int d = 0; d < 3; d++){
        glPushMatrix(); glTranslatef(0.32f, dY[d], 0.27f); glScalef(0.82f, 0.28f, 0.02f); glutSolidCube(1.0); glPopMatrix();
        setColor(0.85f, 0.70f, 0.15f);
        glPushMatrix(); glTranslatef(0.32f, dY[d], 0.29f); glutSolidSphere(0.028f, 6, 6); glPopMatrix();
        setColor(0.50f, 0.32f, 0.12f);
    }

    setColor(0.48f, 0.30f, 0.12f);
    glPushMatrix(); glTranslatef(-0.62f, 0.60f, 0.27f); glScalef(0.52f, 1.15f, 0.02f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.85f, 0.70f, 0.15f);
    glPushMatrix(); glTranslatef(-0.62f, 0.60f, 0.29f); glutSolidSphere(0.030f, 6, 6); glPopMatrix();

    setColor(0.50f, 0.32f, 0.12f);
    glPushMatrix(); glTranslatef(0, 2.35f, 0.20f); glScalef(1.40f, 1.55f, 0.06f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.78f, 0.88f, 0.95f);
    glPushMatrix(); glTranslatef(0, 2.35f, 0.24f); glScalef(1.28f, 1.42f, 0.015f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.92f, 0.96f, 1.0f);
    glPushMatrix(); glTranslatef(-0.25f, 2.55f, 0.248f); glScalef(0.18f, 0.55f, 0.005f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.55f, 0.20f, 0.45f);
    glPushMatrix(); glTranslatef(-0.50f, 1.50f, 0.05f); drawCylinder(0.05f, 0.22f, 10); glPopMatrix();
    setColor(0.70f, 0.30f, 0.60f);
    glPushMatrix(); glTranslatef(-0.50f, 1.72f, 0.05f); glutSolidSphere(0.055f, 8, 8); glPopMatrix();

    setColor(0.30f, 0.55f, 0.72f);
    glPushMatrix(); glTranslatef(-0.30f, 1.50f, 0.05f); drawCylinder(0.040f, 0.18f, 10); glPopMatrix();
    setColor(0.40f, 0.65f, 0.82f);
    glPushMatrix(); glTranslatef(-0.30f, 1.68f, 0.05f); glutSolidSphere(0.044f, 8, 8); glPopMatrix();

    setColor(0.80f, 0.68f, 0.20f);
    glPushMatrix(); glTranslatef(-0.40f, 1.47f, 0.05f); glScalef(0.40f, 0.025f, 0.22f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.78f, 0.72f, 0.65f);
    glPushMatrix(); glTranslatef(0.45f, 1.49f, -0.05f); drawCylinder(0.06f, 0.16f, 10); glPopMatrix();
    float brC[3][3]={{0.85f,0.70f,0.55f},{0.60f,0.42f,0.30f},{0.90f,0.78f,0.60f}};
    for(int b=0;b<3;b++){
        setColor(brC[b][0],brC[b][1],brC[b][2]);
        float ba = b * 1.8f;
        glPushMatrix();
        glTranslatef(0.45f + cos(ba)*0.03f, 1.65f, -0.05f + sin(ba)*0.03f);
        drawCylinder(0.008f, 0.18f, 5);
        glPopMatrix();
    }

    setColor(0.65f, 0.20f, 0.30f);
    glPushMatrix(); glTranslatef(0.22f, 1.50f, 0.10f); glScalef(0.20f, 0.12f, 0.14f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.75f, 0.28f, 0.38f);
    glPushMatrix(); glTranslatef(0.22f, 1.568f, 0.10f); glScalef(0.21f, 0.04f, 0.15f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.55f, 0.35f, 0.14f);
    glPushMatrix(); glTranslatef(0, 0.72f, 0.78f); glScalef(0.60f, 0.07f, 0.45f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.72f, 0.35f, 0.45f);
    glPushMatrix(); glTranslatef(0, 0.78f, 0.78f); glScalef(0.56f, 0.07f, 0.42f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.42f, 0.26f, 0.10f);
    float slx[] = {-0.24f, 0.24f, -0.24f, 0.24f};
    float slz[] = { 0.60f,  0.60f,  0.96f,  0.96f};
    for(int l=0;l<4;l++){
        glPushMatrix(); glTranslatef(slx[l], 0.36f, slz[l]); glScalef(0.06f, 0.72f, 0.06f); glutSolidCube(1.0); glPopMatrix();
    }

    glPopMatrix();
}

// ===== Digital Wall Clock (FIXED POSITION) =====
void drawDigitalClock() {
    glPushMatrix();

    // 👉 Clock wall position (change only here)
    glTranslatef(3.3f, 4.0f, -5.9f);

    // 👉 Face direction fix (wall facing adjustment)
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);

    // ===== Outer Frame =====
    setColor(0.15f, 0.15f, 0.15f);
    glBegin(GL_QUADS);
        glVertex3f(-0.40f, -0.15f, 0.0f);
        glVertex3f( 0.40f, -0.15f, 0.0f);
        glVertex3f( 0.40f,  0.15f, 0.0f);
        glVertex3f(-0.40f,  0.15f, 0.0f);
    glEnd();

    // ===== Screen =====
    setColor(0.02f, 0.05f, 0.02f);
    glBegin(GL_QUADS);
        glVertex3f(-0.35f, -0.10f, 0.001f);
        glVertex3f( 0.35f, -0.10f, 0.001f);
        glVertex3f( 0.35f,  0.10f, 0.001f);
        glVertex3f(-0.35f,  0.10f, 0.001f);
    glEnd();

    // ===== Time Text =====
    glPushMatrix();

        // 👉 Center text properly
        glTranslatef(-0.12f, -0.03f, 0.002f);

        setColor(0.0f, 1.0f, 0.2f);
        glLineWidth(2.5f);
        glDisable(GL_LIGHTING);
        glScalef(0.0012f, 0.0012f, 0.001f);
        char timeStr[] = "10:30";
        for (int i = 0; timeStr[i] != '\0'; i++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, timeStr[i]);
        glEnable(GL_LIGHTING);

        glLineWidth(1.0f);

    glPopMatrix();

    glPopMatrix();
}

// ===== Picture Frame on back wall =====
void drawPicture() {
    setColor(0.45f, 0.28f, 0.10f);
    drawQuad(2.5f,2.2f,-5.99f,  4.2f,2.2f,-5.99f,  4.2f,3.8f,-5.99f,  2.5f,3.8f,-5.99f);

    setColor(0.95f, 0.55f, 0.10f);
    drawQuad(2.65f,2.35f,-5.98f, 4.05f,2.35f,-5.98f, 4.05f,3.05f,-5.98f, 2.65f,3.05f,-5.98f);

    setColor(0.35f, 0.60f, 0.90f);
    drawQuad(2.65f,3.05f,-5.98f, 4.05f,3.05f,-5.98f, 4.05f,3.65f,-5.98f, 2.65f,3.65f,-5.98f);

    setColor(1.0f, 0.90f, 0.20f);
    glPushMatrix();
    glTranslatef(3.35f, 3.05f, -5.97f);
    glutSolidSphere(0.18f, 10, 10);
    glPopMatrix();
}

// ===== Decorative Photo Frame above Sofa (right wall, x=6) =====
void drawSofaPhotoFrame() {
    // ── Outer walnut frame ──
    setColor(0.28f, 0.16f, 0.06f);
    drawQuad(5.99f, 2.50f, -1.05f,
             5.99f, 2.50f,  1.05f,
             5.99f, 4.20f,  1.05f,
             5.99f, 4.20f, -1.05f);

    // Outer frame border — four strips for a thick-frame look
    // Top border strip (lighter wood)
    setColor(0.42f, 0.26f, 0.10f);
    drawQuad(5.988f, 4.05f, -1.05f,
             5.988f, 4.05f,  1.05f,
             5.988f, 4.20f,  1.05f,
             5.988f, 4.20f, -1.05f);
    // Bottom border strip
    drawQuad(5.988f, 2.50f, -1.05f,
             5.988f, 2.50f,  1.05f,
             5.988f, 2.65f,  1.05f,
             5.988f, 2.65f, -1.05f);
    // Left border strip (in wall z coords, left = more negative z)
    drawQuad(5.988f, 2.50f, -1.05f,
             5.988f, 4.20f, -1.05f,
             5.988f, 4.20f, -0.90f,
             5.988f, 2.50f, -0.90f);
    // Right border strip
    drawQuad(5.988f, 2.50f,  0.90f,
             5.988f, 4.20f,  0.90f,
             5.988f, 4.20f,  1.05f,
             5.988f, 2.50f,  1.05f);

    // ── Inner mat (warm cream) ──
    setColor(0.96f, 0.93f, 0.86f);
    drawQuad(5.982f, 2.65f, -0.90f,
             5.982f, 2.65f,  0.90f,
             5.982f, 4.05f,  0.90f,
             5.982f, 4.05f, -0.90f);

    // ── Photo content: warm sunset landscape ──
    // Sky top half (deep blue-violet dusk)
    setColor(0.28f, 0.38f, 0.72f);
    drawQuad(5.975f, 3.40f, -0.80f,
             5.975f, 3.40f,  0.80f,
             5.975f, 3.95f,  0.80f,
             5.975f, 3.95f, -0.80f);

    // Sky lower half (golden-orange horizon glow)
    setColor(0.95f, 0.62f, 0.22f);
    drawQuad(5.975f, 2.85f, -0.80f,
             5.975f, 2.85f,  0.80f,
             5.975f, 3.40f,  0.80f,
             5.975f, 3.40f, -0.80f);

    // Sun disc — glowing gold sphere
    setColor(1.0f, 0.92f, 0.30f);
    glPushMatrix();
    glTranslatef(5.962f, 3.42f, 0.12f);
    glutSolidSphere(0.18f, 14, 14);
    glPopMatrix();

    // Sun inner highlight (brighter core)
    setColor(1.0f, 1.0f, 0.80f);
    glPushMatrix();
    glTranslatef(5.960f, 3.42f, 0.12f);
    glutSolidSphere(0.09f, 10, 10);
    glPopMatrix();

    // Rolling hills — far left (dark green silhouette)
    setColor(0.14f, 0.36f, 0.16f);
    drawQuad(5.970f, 2.85f, -0.80f,
             5.970f, 2.85f, -0.22f,
             5.970f, 3.18f, -0.22f,
             5.970f, 3.18f, -0.80f);

    // Rolling hills — center (mid green)
    setColor(0.18f, 0.48f, 0.20f);
    drawQuad(5.970f, 2.85f, -0.30f,
             5.970f, 2.85f,  0.35f,
             5.970f, 3.26f,  0.35f,
             5.970f, 3.26f, -0.30f);

    // Rolling hills — far right (dark silhouette)
    setColor(0.12f, 0.30f, 0.14f);
    drawQuad(5.970f, 2.85f,  0.28f,
             5.970f, 2.85f,  0.80f,
             5.970f, 3.12f,  0.80f,
             5.970f, 3.12f,  0.28f);

    // Reflective water strip at base
    setColor(0.55f, 0.72f, 0.92f);
    drawQuad(5.972f, 2.85f, -0.80f,
             5.972f, 2.85f,  0.80f,
             5.972f, 3.00f,  0.80f,
             5.972f, 3.00f, -0.80f);

    // Water shimmer lines
    glDisable(GL_LIGHTING);
    setColor(1.0f, 0.98f, 0.72f);
    glLineWidth(1.2f);
    glBegin(GL_LINES);
        glVertex3f(5.968f, 2.91f, -0.30f); glVertex3f(5.968f, 2.91f,  0.28f);
        glVertex3f(5.968f, 2.95f, -0.50f); glVertex3f(5.968f, 2.95f,  0.50f);
        glVertex3f(5.968f, 2.88f, -0.15f); glVertex3f(5.968f, 2.88f,  0.15f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);

    // ── Thin gold inset line (inner frame accent) ──
    setColor(0.82f, 0.65f, 0.12f);
    // Top inset
    drawQuad(5.985f, 4.02f, -0.92f,
             5.985f, 4.02f,  0.92f,
             5.985f, 4.05f,  0.92f,
             5.985f, 4.05f, -0.92f);
    // Bottom inset
    drawQuad(5.985f, 2.65f, -0.92f,
             5.985f, 2.65f,  0.92f,
             5.985f, 2.68f,  0.92f,
             5.985f, 2.68f, -0.92f);

    // ── Small hanging wire / nail at top ──
    setColor(0.55f, 0.55f, 0.58f);
    glPushMatrix();
    glTranslatef(5.993f, 4.22f, 0.0f);
    glutSolidSphere(0.028f, 6, 6);
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
    drawSingleWindowWithCurtain();
    drawDoor();
    drawBed();
    drawTable();
    drawChair();
    drawBookshelf();
    drawBookRack();
    drawCarpet();
    drawSideTable();
    drawCeilingLight();
    drawFan();
    drawAC();
    drawSofa();
    drawTTable();
    drawCornerFlowerPot();
    drawDressingTable();
    drawDigitalClock();
    drawPicture();
    drawSofaPhotoFrame();
    drawFrontWallDecor();
    drawWallMountedTV();
    drawTVCabinet();

    // HUD overlay
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i(10, 30);
    const char* hint1 = "W/S: Fwd/Back  A/D: Left/Right  Q/E: Up/Down  Arrows: Look  L: Light  F: Fan  +/-: Fan Speed  O: AC On  P: AC Off T:TV on/off ESC: Quit";
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
    glEnable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

// ===== Idle =====
void idle() {
    if(fanOn) {
        fanAngle += fanSpeed;
        if(fanAngle >= 360.0f) fanAngle -= 360.0f;
    }
    if(acOn) {
        acFlow += 0.008f;
        if(acFlow > 1.0f) acFlow -= 1.0f;
    }
    curtainWave += 0.015f;
    if(curtainWave > 6.2832f) curtainWave -= 6.2832f;

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
        case 't': case 'T': tvOn    = !tvOn;    break;
        case 'l': case 'L': lightOn = !lightOn; break;
        case 'f': case 'F': fanOn   = !fanOn;   break;
        case 'o': case 'O': acOn    = true;      break;
        case 'p': case 'P': acOn    = false;     break;
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
    glutCreateWindow("Interior Design of a Room");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
