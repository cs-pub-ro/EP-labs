#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "tracking.h"
#include "util.h"

#define FPS_OUT_FILE "fps_stats.csv"
#define IO_OUT_FILE  "io_stats.csv"

#define MAX_SIZE_STATS 6000

static size_t      fps_stats_cnt   = 0;       /* fps statistics counter */
static fps_stats_t fps_stats[MAX_SIZE_STATS]; /* fps statistics for max 10 minutes */
static size_t      io_stats_cnt    = 0;       /* io statistics counter */
static io_stats_t  io_stats[MAX_SIZE_STATS];  /* io statistics for max 10 minutes  */

/* track fps once every second */
void
track_fps(double *last_time, size_t vertices, size_t *nb_frames)
{
    if (fps_stats_cnt >= MAX_SIZE_STATS)
        return;

    double current_time = glfwGetTime();
    double delta = current_time - *last_time;
    *nb_frames += 1;

    if (delta >= 0.1) {
        double fps = double(*nb_frames) / delta;

        /* store fps statistics */
        fps_stats[fps_stats_cnt] = {
            .time = current_time,
            .vertices = vertices,
            .fps = fps
        };

        fps_stats_cnt++;
        *nb_frames = 0;
        *last_time = current_time;
    }
}

/* track io events */
void
track_io_event(double event_time, double generate_duration, double memcpy_duration, size_t vertices)
{
    if (io_stats_cnt >= MAX_SIZE_STATS)
        return;

    static double last_time = 0;
    double current_time = glfwGetTime();
    double delta = current_time - last_time;

    /* store io statistics */
    io_stats[io_stats_cnt] = {
        .time = current_time,
        .vertices = vertices,
        .event = event_time,
        .generate_duration = generate_duration,
        .memcpy_duration = memcpy_duration
    };

    io_stats_cnt++;
    last_time = current_time;
}

/* print all statistics to output files */
void
print_all_stats()
{
    FILE *fps_fp = fopen(FPS_OUT_FILE, "w");
    FILE *io_fp = fopen(IO_OUT_FILE, "w");

    if (!fps_fp || !io_fp) {
        ERROR("failed to open output files");
        return;
    }

    /* print fps statistics */
    fprintf(fps_fp, "time,vertices,fps\n");
    for (size_t i = 0; i < fps_stats_cnt; i++) {
        fprintf(fps_fp, "%f,%lu,%f\n",
                fps_stats[i].time, fps_stats[i].vertices, fps_stats[i].fps);
    }

    /* print io statistics */
    fprintf(io_fp, "time,vertices,event,generate_duration,memcpy_duration\n");
    for (size_t i = 0; i < io_stats_cnt; i++) {
        fprintf(io_fp, "%f,%lu,%f,%f,%f\n",
                io_stats[i].time, io_stats[i].vertices, io_stats[i].event,
                io_stats[i].generate_duration, io_stats[i].memcpy_duration);
    }

    fclose(fps_fp);
    fclose(io_fp);
}
