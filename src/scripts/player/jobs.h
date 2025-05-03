#ifndef JOBS_H
#define JOBS_H
//not in use rn
typedef enum {
    JOBTYPE_NONE,
    JOBTYPE_BEAT_NPC,
    JOBTYPE_FETCH_ITEM,
    JOBTYPE_DISTRACT_GUARD,
    // Add more as needed
} JobTypePlayer;

typedef struct {
    JobTypePlayer type;
    int targetNPCId;      // for beat up / distract
    int itemId;           // for fetch
    int itemAmount;
    float duration;       // for distract time, etc.
    bool completed;
    bool active;
} JobPlayer;


#endif