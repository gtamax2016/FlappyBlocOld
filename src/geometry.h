#include <gint/std/stdlib.h>

#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct {
    uint8_t x, y;
} Point;
Point new_point(uint8_t x, uint8_t y) {
    Point *tmp;
    tmp = (Point *)malloc(sizeof(Point));
    
    tmp->x = x;
    tmp->y = y;
    
    return *tmp;
}

typedef struct {
    Point v1, v2;
} Rect;
Rect new_rect(Point v1, Point v2) {
    Rect *tmp;
    tmp = (Rect *)malloc(sizeof(Rect));

    tmp->v1 = v1;
    tmp->v2 = v2;

    return *tmp;
}

#endif /* GEOMETRY_H */