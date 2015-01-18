#include <pebble.h>
#include "schedules.h"

#define SAVE_LIMIT (10)
#define NOTIFY_IF_MISSED false
#define SECONDS_IN_WEEK (604800)
#define SECOND_OFFSET (45)//necessary because the scheduler seems to lag behind about a minute 

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
      if ((timestamp - time(NULL)) > SECONDS_IN_WEEK) timestamp -= SECONDS_IN_WEEK;
      APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Querying with %d hours, %d mins, day: %d", hours, mins, day);
      APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "time: %lu", (long) time(NULL));
      APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "timestamp: %lu", timestamp);

  
      
      //if (!wakeup_query(id, &timestamp)){
        wakeup_schedule(timestamp-SECOND_OFFSET, id, NOTIFY_IF_MISSED);
        APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "timestamp: %lu", timestamp);
        APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Scheduled (hopefully) with %d hours, %d mins, day: %d", hours, mins, day);

      //} 
    }
  }
}

void clear_all(){
  int i;
  for (i=0; i<SAVE_LIMIT; i++){
    if(persist_exists(i)){
      persist_delete(i);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Deleting id: %d", i);
    }
  }
}