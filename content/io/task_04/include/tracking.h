
#include <stdint.h>
#include <stdio.h>

#ifndef _TRACKING_H
#define _TRACKING_H

/* fps statistics */
typedef struct {
    double time;
    size_t vertices;
    double fps;
} fps_stats_t;

/* io statistics */
typedef struct {
    double time;
    size_t vertices;
    double event;
    double generate_duration;
    double memcpy_duration;
} io_stats_t;

int32_t gen_sphere(size_t, uint32_t *, double *);

void print_all_stats();
void track_io_event(double, double, double, size_t);
void track_fps(double *, size_t, size_t *);

#endif /* _TRACKING_H */