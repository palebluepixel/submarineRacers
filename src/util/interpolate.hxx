#ifndef _INTERPOLATE_HXX_
#define _INTERPOLATE_HXX_

#define LERP_FLOAT( x,  xmin,  xmax,  ymin,  ymax) \
                x >= xmax ? ymax : ( x<= xmin ? ymin : \
                (ymin + (x - xmin) * ((ymax-ymin)/(xmax-xmin))))

#endif