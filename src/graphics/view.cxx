#include "view.hxx"

View::View(GLFWwindow *win)
{
    this->win = win;
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    this->ht = height;
    this->wid = width;
    this->updateAspect();

    this->nCameras = 0;
    this->maxCameras = NCAMS;
    this->cameras = new Camera*[this->maxCameras];
}

View::~View(){}

void View::addCamera(Camera *cam)
{
    if(this->nCameras >= this->maxCameras) return;
    
    this->cameras[this->nCameras++] = cam;

    //if we only have this one camera, make it active
    if(this->nCameras == 1) this->activeCamerai = 0;
}

void View::switchCamera(int cami)
{
    if(cami < 0 || cami > this->nCameras || cami > this->maxCameras) return;

    this->activeCamerai = cami;
}

Camera *View::activeCamera()
{
    if(this->nCameras == 0) return NULL;

    return this->cameras[this->activeCamerai];
}