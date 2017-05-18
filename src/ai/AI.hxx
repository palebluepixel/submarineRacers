#ifndef _AI_HXX_
#define _AI_HXX_

#include <ent/Submarine.hxx>
#include <ent/Checkpoint.hxx>

#define CLOSEENOUGH 4

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
    inline void unbindFromSubAct() { this->subAct = NULL; }

    inline Submarine *getOurSub() { return this->subAct->getSub(); }

    /* Called every AI tick.*/
    void updateAI();

    /* Currently, AI turning and acceleration speeds are calculated as a linear 
    interpolation between zero and the maximum speed (based on the submarine's 
    stats), where the x value in the interpolation is the angle between the
    submarine's target turning vector and its current look direction. The thresholds
    might eventually be calculated using, e.g.
    * The submarine's current velocity
    * The distance of the target point
    */

    /* Turn left/right so that our direction is closer to the target vector 
    in (x,z) space */
    void turnTowards(float angle, float threshAny, float threshFull);

    /* Accelerate based on the difference between our current facing direction
    and our target facing direction */
    void accelerateBasedOn(float angle, float threshAny, float threshFull);

    /* Very simple seek function, just turn towards the direction and then accelerate */
    void seekPoint(vec2 point);

private:

    SubmarineActuator *subAct;

};


#endif
