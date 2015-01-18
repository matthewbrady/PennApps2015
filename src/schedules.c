#include <pebble.h>
#include "schedules.h"

#define SAVE_LIMIT (10)
#define NOTIFY_IF_MISSED false

int interval_write(interval interval_object){
  int i;
  for (i=0; i < SAVE_LIMIT; i++){
    if (!persist_exists(i)) {
      persist_write_data(i, &interval_object, sizeof(interval_object));
      APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "attempted to write to with id: %d", i);
      return i;
    }
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "fell out of loop");
  return -1;
}

void schedule_all(){
  int id;
  for (id=0; id < SAVE_LIMIT; id++){
    if(persist_exists(id)){
      
      interval interval_object;
      persist_read_data(id, &interval_object, sizeof(interval_object));
      
      int hours = interval_object.begin_time_mins / 60;//isolate mins
      int mins = interval_object.begin_time_mins % 60; //isolate hours
      int day = interval_object.day;
      long timestamp = clock_to_timestamp(day, hours, mins); //find next time it should be scheduled 
      APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Querying with %d hours, %d mins, day: %d", hours, mins, day);

      
      if (!wakeup_query(id, &timestamp)){
        wakeup_schedule(timestamp, id, NOTIFY_IF_MISSED);
        APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Scheduled (hopefully) with %d hours, %d mins, day: %d", hours, mins, day);

      } 
    }
  }
}