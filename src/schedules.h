#pragma once
#include <pebble.h>
#define SCHEDULES_H_

  

typedef struct  {
  int begin_time_mins;//begining time in minutes since midnight (12:00am =0 )
  //int duration;//duration in mins
  int day;//the day of the week
  //bool repeat;//whether the task repeats every week
} interval;

int interval_write(interval interval_object);
void schedule_all();