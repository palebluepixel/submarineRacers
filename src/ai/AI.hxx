#ifndef _AI_HXX_
#define _AI_HXX_

#include <ent/Submarine.hxx>

class AI {
public:
    AI();
    ~AI();

    virtual void updateAI() = 0;

};


class SubmarineAI : public AI {
public:
    SubmarineAI();
    ~SubmarineAI();

    inline void bindToSubAct(SubmarineActuator *subAct) { this->subAct = subAct; }

    void updateAI();

private:
    int curSeekPoint;

    SubmarineActuator *subAct;

};


#endif
