#include "MouseOn.h"

double matModelView[16], matProjection[16];
int viewport[4];
glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
glGetIntegerv(GL_VIEWPORT, viewport);
double winX = (double)mouseX;
double winY = viewport[3] - (double)mouseY;
gluUnProject(winX, winY, 0.0, matModelView, matProjection,
    viewport, m_start.x, &m_start.y, &m_start.z);
gluUnProject(winX, winY, 1.0, matModelView, matProjection,
    viewport, m_end.x, &m_end.y, &m_end.z);

Select::Select(glm::mat4* m, glm::mat4* p, glm::mat4* v) {
    model = m;
    proj = p;
    view = v;

}

Select::~Select()
{
}

glm::vec3 Select::ClosestPoint(const glm::vec3 A, const glm::vec3 B, const glm::vec3 P, double* t)
{
    glm::vec3 AB = B - A;
    double ab_square = DotProduct(AB, AB);
    glm::vec3 AP = P - A;
    double ap_dot_ab = DotProduct(AP, AB);
    // t is a projection param when we project vector AP onto AB 
    *t = ap_dot_ab / ab_square;
    // calculate the closest point 
    glm::vec3 Q = A + AB * (*t);
    return Q;
}

bool Select::RayTest(const glm::vec3, const glm::vec3 start, const glm::vec3 end, glm::vec3* pt, double* t, double epsilon)
{
    *pt = ClosestPoint(start, end, center, t);
    double len = Distance(*pt, m_pos);
    return len < (m_radius + epsilon);
}
// note that "t" param can be used further 
// the same is with "pt" 

void Select::Render()
{
    // clear screen ... 
    // setup camera ... 

    // HIT test: 
    CalculateRay();
    CheckHitTestBetweenPointsInTheScene();

    // render the scene... 
}

bool Select::RayTestPoints(const glm::vec3& start, const glm::vec3& end, unsigned int* id, double* t, double epsilon)
{
    unsigned int pointID = m_count + 1;
    bool foundCollision = false;
    double minDistToStart = 10000000.0;
    double dst;
    glm::vec3 pt;
    for (unsigned int i = 0; i < m_count; ++i)
    {
        if (m_points[i].RayTest(start, end, &pt, t, epsilon))
        {
            dst = Distance(start, pt);
            if (dst < minDistToStart)
            {
                minDistToStart = dst;
                pointID = i;
                foundCollision = true;
            }
        }
    }

    *id = pointID;

    return foundCollision;
}

void Select::ProcessMouse(int button, int state, int x, int y) 
{ 
    g_camera.ProcessMouse(button, state, x, y); 
    // can we move any ball? 
    if (g_camera.m_isLeftPressed && g_we_got_hit) 
    { 
        g_draggedPointID = g_lastHitPointID; 
        g_areWeDraggingPoint = true; 
    } 
    else  
        g_areWeDraggingPoint = false; 
}

glm::vec3 v2 = m_end - m_start;
glm::vec3 point_in_3d = m_start + v2 * m_lastT;
// m_lastT is "T" param found in hit test 

our_model_pos = point_in_3d;