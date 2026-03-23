#!/usr/bin/env python3
"""
Reads live output from `vmstat` and plots free memory, used memory,
and buffer memory over the last N seconds.

Usage:
    python3 plot.py           # default: last 60 seconds
    python3 plot.py 120       # last 120 seconds
"""
import subprocess
import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# config
N = int(sys.argv[1]) if len(sys.argv) > 1 else 60   # window size in seconds

# TODO 1:
# Implement get_total_mem_mb() so that it reads /proc/meminfo and returns
# the total physical RAM in MB as an integer.
# Hint: MemTotal is reported in kB
def get_total_mem_mb() -> int:
    pass

total_mb = get_total_mem_mb()

# rolling window of N samples
timestamps = deque(maxlen=N)   # seconds since start
free_data  = deque(maxlen=N)   # MB free
used_data  = deque(maxlen=N)   # MB used (total - free - buff - cache)
buff_data  = deque(maxlen=N)   # MB used as buffer

tick = 0   # incremented every time a valid vmstat line is parsed

# starts vmstat as a background process
# vmstat -S m 1  →  output in MB, one sample per second
# Column layout (no -a flag):
#   r  b  swpd  free  buff  cache  si  so  bi  bo  in  cs  us  sy  id  wa  st
#   0  1   2     3     4      5    ...
vmstat_proc = subprocess.Popen(
    ["vmstat", "-S", "m", "1"],
    stdout=subprocess.PIPE,
    text=True,
    bufsize=1,          # line-buffered
)

# plot setup
fig, ax = plt.subplots(figsize=(10, 5))
fig.suptitle("Real-time Memory Usage", fontsize=13, fontweight="bold")

line_free, = ax.plot([], [], color="green",     linewidth=2, label="Free")
line_used, = ax.plot([], [], color="red",       linewidth=2, label="Used")
line_buff, = ax.plot([], [], color="steelblue", linewidth=2, label="Buff")

ax.set_xlabel("Time (seconds)")
ax.set_ylabel("Memory (MB)")
ax.legend(loc="upper right")
ax.grid(True, linestyle="--", alpha=0.4)

info_text = ax.text(
    0.01, 0.97, "", transform=ax.transAxes,
    verticalalignment="top", fontsize=9,
    bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow", alpha=0.7),
)

# animation update function
def update(_frame):
    global tick

    raw_line = vmstat_proc.stdout.readline()
    if not raw_line:
        return line_free, line_used, line_buff, info_text

    parts = raw_line.split()

    # TODO 2:
    # Parse the correct columns from the vmstat output line (stored in `parts`).
    # You need: free memory, buffer memory, and cache memory in MB.
    # Wrap your code in a try/except (ValueError, IndexError) and return early
    # if parsing fails, note that vmstat prints two header lines before any data.

    # TODO 3:
    # Compute `used` memory in MB.
    # Hint: used = total - free - buff - cache
    # Make sure the result is never negative

    timestamps.append(tick)
    free_data.append(free)
    used_data.append(used)
    buff_data.append(buff)
    tick += 1

    # TODO 4
    # Update the three plot lines (line_free, line_used, line_buff) with the
    # latest data from the rolling windows.

    # TODO 5
    # Dynamically adjust the axis ranges so the plot always fits the data.
    # x-axis: from the oldest timestamp to at least t[0] + N
    # y-axis: from just below the minimum value to just above the maximum,
    #           clamped between 0 and total_mb.

    # TODO 6
    # Update the info box (info_text) to display the current values of:
    #   Total RAM, Free, Used, Buff — all in MB.
    # Use set_text() with an f-string. Format numbers with comma separators.

    return line_free, line_used, line_buff, info_text

# run
ani = animation.FuncAnimation(
    fig,
    update,
    interval=250,
    blit=False,
    cache_frame_data=False,
)

plt.tight_layout()

try:
    plt.show()
finally:
    vmstat_proc.terminate()
    vmstat_proc.wait()
    print("ended.")
