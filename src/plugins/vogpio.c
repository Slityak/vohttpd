/*  vogpio common library
 *
 *  author: Kornél Kovács (kornel.kovacs@draftgate.com)
 *
 */
#include <stdio.h>
#include <string.h>

#include "../vohttpd.h"

int control_gpio(socket_data *d, string_reference *pa)
{
    char buf[MESSAGE_SIZE] = "Invalid parameter.";
    FILE *fp;

    fp = fopen("/sys/class/gpio/gpio0/direction", "w");
    if(fp == NULL) {
        sprintf(buf, "update gpio 0 direction failed.");
        return d->set->send(d->sock, buf, strlen(buf), 0);
    }
    fwrite("out", 1, 4, fp);
    fclose(fp);

    if(memcmp(pa->ref, "on", 2) == 0) {
        fp = fopen("/sys/class/gpio/gpio0/value", "w");
        if(fp != NULL) {
            fwrite("1", 1, 2, fp);
            fclose(fp);

            sprintf(buf, "update gpio 0 value to 1 success.");
        } else {
            sprintf(buf, "update gpio 0 value to 1 failed.");
        }
    }
    if(memcmp(pa->ref, "off", 3) == 0) {
        fp = fopen("/sys/class/gpio/gpio0/value", "w");
        if(fp != NULL) {
            fwrite("0", 1, 2, fp);
            fclose(fp);

            sprintf(buf, "update gpio 0 value to 0 success.");
        } else {
            sprintf(buf, "update gpio 0 value to 0 failed.");
        }
    }

    return d->set->send(d->sock, buf, strlen(buf), 0);
}

int vohttpd_library_query(int id, plugin_info *out)
{
    static plugin_info info[] = {
    { ".", "contains GPIO control functions for vohttpd." },
    { "control_gpio", "turn on/off GPIO pin" },
    };
    if(id >= sizeof(info) / sizeof(plugin_info))
        return -1;
    memcpy(out, &info[id], sizeof(plugin_info));
    return id;
}

