#ifndef _MENU_HXX_
#define _MENU_HXX_

#include <unordered_map>
#include <ent/ScreenQuad.hxx>

/* TODO: the whole function pointer thing isn't actually implemeted, I realized that every menu
was just selecting a number between 0 and 3 so we just use raw user input. This is just a wrapper
class for a screenquad (which is actually a cube), my life is falling apart */

/* Where we place the menu and camera in the world so we can look at it. */
#define MENUPOSITION vec3(8.2,0,0)
#define CAMERALOOKPOSITION vec3(-10,0,0)
#define CAMERAMENUPOSITION vec3(10,0,0)
#define CAMERAUP vec3(0,1,0)

/* Contains mappings of menu options to function pointers. We represent user input as an integer
which is an index into the menuFunctions hashmap(this isn't very abstract but hey the project is
due in like 10 hours)*/
class Menu {
public:
    /* im is the file path to the image display of the menu */
    Menu(string im);

    /* Adds a menu entry. Overwrites the previous entry for this int, if one existed */
    void addMenuOption(int userinp, void *func);

    /* Returns 1 if the menu contains a function for the given integer */
    int menuContainsEntry(int userinp);

    /* Calls the function in the entry for this integer, if it exists */
    void selectMenuEntry(int userinp);

    /* Get the screenquad for this menu */
    inline ScreenQuad *getDisplay() {return this->sq;}


private:
    /* Map of menu options (integer) to function pointers */
    unordered_map<int,void*> menuEntries;

    /* Screenquad textured with the visual for this menu */
    ScreenQuad *sq;

};

#endif