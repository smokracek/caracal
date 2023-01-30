#ifndef CARACAL_ALERT_H
#define CARACAL_ALERT_H

#ifdef __cplusplus
extern "C" {
#endif

struct alert {
    char type[100];
    char message[100];
};

#ifdef __cplusplus
}
#endif
#endif