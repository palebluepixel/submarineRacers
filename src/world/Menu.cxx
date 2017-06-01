#include "Menu.hxx"
#include <world/world.hxx>

Menu::Menu(string im)
{
    static int nextID = MENUID_START;
    this->sq = new ScreenQuad(++nextID, MENUPOSITION, quaternion(), "baby", TYPE1, SPAWNED, 0.1f, vec3(1,1,1), "../assets/models/cube.obj", strdup(im.c_str()));
}

/* Adds a menu entry. Overwrites the previous entry for this int, if one existed */
void Menu::addMenuOption(int userinp, void *func)
{
    this->menuEntries[userinp] = func;
}

/* Returns 1 if the menu contains a function for the given integer */
int Menu::menuContainsEntry(int userinp)
{
    return this->menuEntries.find(userinp) != menuEntries.end();
}

/* Calls the function in the entry for this integer, if it exists */
void Menu::selectMenuEntry(int userinp)
{
    void *__func = this->menuEntries[userinp];
    if(!__func)
        return;
    //__func();
}

