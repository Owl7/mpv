#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "libmpv/client.h"
#include "shared.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("pass a single media file as argument\n");
        return 1;
    }

    mpv_handle *ctx = mpv_create();
    if (!ctx) {
        printf("failed creating context\n");
        return 1;
    }

    // Enable default key bindings, so the user can actually interact with
    // the player (and e.g. close the window).
    check_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"));
    check_error(mpv_set_option_string(ctx, "osc", "yes"));

    // Done setting up options.
    check_error(mpv_initialize(ctx));

    // Play this file.
    const char *cmd[] = {"loadfile", argv[1], NULL};
    check_error(mpv_command(ctx, cmd));

    // Let it play, and wait until the user quits.
    while (1) {
        mpv_event *event = mpv_wait_event(ctx, 10000);
        printf("event: %s\n", mpv_event_name(event->event_id));
        if (event->event_id == MPV_EVENT_SHUTDOWN)
            break;
    }

    mpv_destroy(ctx);
    return 0;
}
